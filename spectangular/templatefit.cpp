#include "templatefit.h"

#include "dataio.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include <armadillo>

namespace tfit {

namespace {

const double C_KMS = 299792.458;
const double NaN = std::numeric_limits<double>::quiet_NaN();

double median(std::vector<double> v)
{
    if (v.empty()) return 0.0;
    std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
    return v[v.size() / 2];
}

// linear interpolation on an increasing grid; NaN outside
double interp(const Spectrum &t, double x)
{
    const auto &l = t.lam;
    if (l.size() < 2 || !(x >= l.front()) || !(x <= l.back())) return NaN;
    size_t i = size_t(std::upper_bound(l.begin(), l.end(), x) - l.begin());
    if (i >= l.size()) i = l.size() - 1;
    if (i == 0) i = 1;
    const double u = (x - l[i - 1]) / (l[i] - l[i - 1]);
    return t.f[i - 1] + u * (t.f[i] - t.f[i - 1]);
}

// template at the component wavelengths, shifted by v (km/s)
std::vector<double> evalShifted(const Spectrum &t, const std::vector<double> &lam, double v)
{
    std::vector<double> y(lam.size());
    const double f = 1.0 + v / C_KMS;
    for (size_t i = 0; i < lam.size(); ++i) y[i] = interp(t, lam[i] / f);
    return y;
}

// least squares of x = sum_u a_u P_u + b * g over the pixels in idx (P: continuum basis per
// pixel); returns chi2 (unweighted); c: continuum at the first pixel (for information)
double fitLine(const std::vector<double> &x, const std::vector<double> &g, const std::vector<size_t> &idx,
               const std::vector<std::vector<double>> &P, double &c, double &b)
{
    const int nd = P.empty() ? 1 : int(P[0].size());
    arma::mat N(nd + 1, nd + 1, arma::fill::zeros);
    arma::vec r(nd + 1, arma::fill::zeros), z(nd + 1);
    int n = 0;
    for (size_t i : idx) {
        if (!std::isfinite(g[i])) continue;
        for (int u = 0; u < nd; ++u) z(u) = P[i][u];
        z(nd) = g[i];
        N += z * z.t();
        r += z * x[i];
        ++n;
    }
    arma::vec a;
    if (n < nd + 2 || !arma::solve(a, N, r, arma::solve_opts::no_approx)) { c = b = NaN; return NaN; }
    b = a(nd);
    c = a(0);
    double q = 0;
    for (size_t i : idx) {
        if (!std::isfinite(g[i])) continue;
        for (int u = 0; u < nd; ++u) z(u) = P[i][u];
        z(nd) = g[i];
        q += std::pow(x[i] - arma::dot(z, a), 2);
    }
    return q;
}

struct Comp {
    const Spectrum *x = nullptr;
    Spectrum t;                     // broadened template
    std::vector<double> w;          // assumed light fraction of this component at its pixels
    std::vector<double> s;          // shape s(lambda) at its pixels
    std::vector<size_t> idx;        // pixels used
    std::vector<double> T;          // template at the pixels (shifted)
    std::vector<std::vector<double>> P;   // continuum basis at the pixels
    double v = 0.0;
};

// regressor of the separate fit: x = c(lambda) + l * g, g = -(lshape / w)(1 - T), where
// lshape is the wavelength dependence of the light fraction for the ratio k s(lambda),
// relative to lambda_ref (s = 1 there), so that l is the light fraction at lambda_ref
std::vector<double> regressor(const Comp &c, int q, double k)
{
    std::vector<double> g(c.T.size());
    const double l0 = q == 0 ? k / (1 + k) : 1 / (1 + k);
    for (size_t i = 0; i < g.size(); ++i) {
        const double r = k * c.s[i];
        const double l = q == 0 ? r / (1 + r) : 1 / (1 + r);
        g[i] = -(l / l0) * (1.0 - c.T[i]) / c.w[i];
    }
    return g;
}

// shift of the template by a grid search with parabolic refinement (separate fit, l > 0)
double findShift(Comp &c, int q, double k, double v0, double vmax, double step)
{
    double best = NaN, bestQ = std::numeric_limits<double>::infinity();
    const int n = int(std::ceil(vmax / step));
    std::vector<double> qs(2 * n + 1, NaN);
    for (int j = -n; j <= n; ++j) {
        const double v = v0 + j * step;
        c.T = evalShifted(c.t, c.x->lam, v);
        double a, l;
        const double qq = fitLine(c.x->f, regressor(c, q, k), c.idx, c.P, a, l);
        if (std::isfinite(qq) && l > 0) {
            qs[j + n] = qq;
            if (qq < bestQ) { bestQ = qq; best = v; }
        }
    }
    if (!std::isfinite(best)) return v0;
    const int kb = int(std::lround((best - v0) / step)) + n;
    if (kb > 0 && kb < 2 * n && std::isfinite(qs[kb - 1]) && std::isfinite(qs[kb + 1])) {
        const double d = qs[kb - 1] - 2 * qs[kb] + qs[kb + 1];
        if (d > 0) best += 0.5 * step * (qs[kb - 1] - qs[kb + 1]) / d;
    }
    return best;
}

} // namespace

bool readSpectrum(const std::string &file, Spectrum &s, std::string &err)
{
    std::vector<std::vector<double>> rows;
    if (!dataio::readTable(file, 2, rows, err)) return false;
    s = Spectrum();
    for (const auto &r : rows) {
        if (!std::isfinite(r[0]) || !std::isfinite(r[1])) continue;
        s.lam.push_back(r[0] <= 100.0 ? std::pow(10.0, r[0]) : r[0]);
        s.f.push_back(r[1]);
    }
    if (s.lam.size() < 10) {
        err = "File " + file + ": fewer than 10 pixels.";
        return false;
    }
    for (size_t i = 1; i < s.lam.size(); ++i)
        if (!(s.lam[i] > s.lam[i - 1])) {
            err = "File " + file + ": wavelengths must increase.";
            return false;
        }
    return true;
}

Spectrum broaden(const Spectrum &t, double vsini, double R, double eps)
{
    if (!(vsini > 0) && !(R > 0)) return t;
    // uniform grid in ln(lambda) with the median step of the input
    std::vector<double> d;
    for (size_t i = 1; i < t.lam.size(); ++i) d.push_back(std::log(t.lam[i] / t.lam[i - 1]));
    double h = median(d);
    const double span = std::log(t.lam.back() / t.lam.front());
    h = std::max(h, span / 2e6);
    // kernel resolution: at least 5 points per FWHM of the narrowest component
    const double fwhm = std::min(vsini > 0 ? vsini : 1e30, R > 0 ? C_KMS / R : 1e30) / C_KMS;
    h = std::min(h, fwhm / 5.0);
    const size_t n = size_t(span / h) + 1;
    Spectrum u;
    u.lam.resize(n);
    u.f.resize(n);
    for (size_t i = 0; i < n; ++i) {
        u.lam[i] = t.lam.front() * std::exp(i * h);
        u.f[i] = interp(t, std::min(u.lam[i], t.lam.back()));
    }
    // kernel in velocity: rotation convolved with the instrumental Gaussian
    const double dv = h * C_KMS;
    std::vector<double> rot{1.0};
    if (vsini > 0) {
        const int m = int(vsini / dv);
        rot.assign(2 * m + 1, 0.0);
        for (int k = -m; k <= m; ++k) {
            const double x = k * dv / vsini;
            const double q = std::max(0.0, 1.0 - x * x);
            rot[k + m] = 2.0 * (1.0 - eps) * std::sqrt(q) + 0.5 * M_PI * eps * q;
        }
        if (m == 0) rot = {1.0};
    }
    std::vector<double> gau{1.0};
    if (R > 0) {
        const double sig = C_KMS / R / 2.3548200450309493;
        const int m = int(std::ceil(4 * sig / dv));
        gau.assign(2 * m + 1, 0.0);
        for (int k = -m; k <= m; ++k) gau[k + m] = std::exp(-0.5 * std::pow(k * dv / sig, 2));
    }
    std::vector<double> ker(rot.size() + gau.size() - 1, 0.0);
    for (size_t i = 0; i < rot.size(); ++i)
        for (size_t j = 0; j < gau.size(); ++j) ker[i + j] += rot[i] * gau[j];
    double sum = 0;
    for (double k : ker) sum += k;
    for (double &k : ker) k /= sum;
    const int m = int(ker.size() / 2);
    Spectrum out = u;
    for (size_t i = 0; i < n; ++i) {
        double a = 0, wsum = 0;
        for (int k = -m; k <= m; ++k) {
            const long j = long(i) - k;
            if (j < 0 || j >= long(n)) continue;
            a += ker[k + m] * u.f[j];
            wsum += ker[k + m];
        }
        out.f[i] = a / wsum;      // renormalised at the ends
    }
    return out;
}

Result fit(const Spectrum &A, const Spectrum &B, const Spectrum &TA, const Spectrum &TB,
           const fr::Model &m, double k0, const Options &opt)
{
    Result res;
    if (!(k0 > 0)) { res.message = "The flux ratio of the disentangling must be positive."; return res; }
    if (A.lam.size() < 10 || B.lam.size() < 10) { res.message = "Component spectra too short."; return res; }
    Comp c[2];
    c[0].x = &A;
    c[1].x = &B;
    c[0].t = broaden(TA, opt.vsini[0], opt.resolvingPower);
    c[1].t = broaden(TB, opt.vsini[1], opt.resolvingPower);
    // shape and assumed light fractions
    double lref = m.lambdaRef;
    if (!(lref > 0)) lref = std::sqrt(A.lam.front() * A.lam.back());
    res.lambdaRef = lref;
    for (int q = 0; q < 2; ++q) {
        const auto &lam = c[q].x->lam;
        c[q].s.resize(lam.size());
        c[q].w.resize(lam.size());
        for (size_t i = 0; i < lam.size(); ++i) {
            const double s = m.mode == fr::Mode::Constant ? 1.0 : fr::shape(m, lam[i], lref);
            double wa, wb;
            fr::weights(k0, s, 1.0, wa, wb);
            c[q].s[i] = s;
            c[q].w[i] = q == 0 ? wa : wb;
        }
        const size_t n = lam.size();
        for (size_t i = size_t(std::max(0, opt.edgeSkip)); i + size_t(std::max(0, opt.edgeSkip)) < n; ++i) {
            if (opt.l1 > 0 && lam[i] < opt.l1) continue;
            if (opt.l2 > 0 && lam[i] > opt.l2) continue;
            if (!std::isfinite(c[q].x->f[i])) continue;
            // inside the template also for the largest shift
            const double f = 1.0 + (opt.fitShift[q] ? std::fabs(opt.shift[q]) + opt.vmax : std::fabs(opt.shift[q])) / C_KMS;
            if (lam[i] * f > c[q].t.lam.back() || lam[i] / f < c[q].t.lam.front()) continue;
            c[q].idx.push_back(i);
        }
        if (c[q].idx.size() < 20) {
            res.message = std::string("Too few pixels of component ") + (q ? "B" : "A")
                          + " in the fit range and inside the synthetic spectrum.";
            return res;
        }
    }
    const int nd = std::max(0, opt.contDegree) + 1;
    double tlo = 1e300, thi = -1e300;
    for (int q = 0; q < 2; ++q)
        for (size_t i : c[q].idx) { tlo = std::min(tlo, std::log(c[q].x->lam[i])); thi = std::max(thi, std::log(c[q].x->lam[i])); }
    auto basis = [&](double lam, std::vector<double> &P) {
        // Legendre polynomials in t in [-1, 1] over the fit range
        const double t = thi > tlo ? 2 * (std::log(lam) - tlo) / (thi - tlo) - 1 : 0.0;
        P.assign(nd, 1.0);
        if (nd > 1) P[1] = t;
        for (int n = 2; n < nd; ++n) P[n] = ((2 * n - 1) * t * P[n - 1] - (n - 1) * P[n - 2]) / n;
    };
    for (int q = 0; q < 2; ++q) {
        c[q].P.resize(c[q].x->lam.size());
        for (size_t i = 0; i < c[q].P.size(); ++i) basis(c[q].x->lam[i], c[q].P[i]);
    }
    // shifts and separate fits
    for (int q = 0; q < 2; ++q) {
        std::vector<double> d;
        const auto &lam = c[q].x->lam;
        for (size_t i = 1; i < lam.size(); ++i) d.push_back(std::log(lam[i] / lam[i - 1]) * C_KMS);
        const double pixv = median(d);
        c[q].v = opt.shift[q];
        if (opt.fitShift[q]) {
            c[q].v = findShift(c[q], q, k0, opt.shift[q], opt.vmax, std::max(0.05, 0.5 * pixv));
            c[q].v = findShift(c[q], q, k0, c[q].v, std::max(0.5, pixv), std::max(0.01, 0.05 * pixv));
        }
        c[q].T = evalShifted(c[q].t, lam, c[q].v);
    }
    // joint fit: chi2(k), the continuum c_A(lambda) (polynomial) linear; sigmas from the
    // separate fits; clipping
    double sig[2] = {1.0, 1.0};
    // y = f * c_A(lambda) for every pixel used:
    //   A: x + (l/w)(1-T) = c_A;   B: x + (l_B/w_B)(1-T) - 1/w_B = -(w_A/w_B) c_A
    auto rowsFor = [&](double k, int q, std::vector<double> &y, std::vector<double> &f) {
        const auto &x = c[q].x->f;
        y.clear(); f.clear();
        for (size_t i : c[q].idx) {
            if (!std::isfinite(c[q].T[i])) { y.push_back(NaN); f.push_back(NaN); continue; }
            const double r = k * c[q].s[i];
            const double l = q == 0 ? r / (1 + r) : 1 / (1 + r);
            const double w = c[q].w[i];
            if (q == 0) { y.push_back(x[i] + l / w * (1 - c[q].T[i])); f.push_back(1.0); }
            else { y.push_back(x[i] + l / w * (1 - c[q].T[i]) - 1.0 / w); f.push_back(-(1.0 - w) / w); }
        }
    };
    auto chi2At = [&](double k, arma::vec &a, arma::mat *Ninv = nullptr) {
        arma::mat N(nd, nd, arma::fill::zeros);
        arma::vec b(nd, arma::fill::zeros);
        std::vector<double> y[2], f[2];
        for (int q = 0; q < 2; ++q) {
            rowsFor(k, q, y[q], f[q]);
            const double wq = 1.0 / (sig[q] * sig[q]);
            for (size_t j = 0; j < y[q].size(); ++j) {
                if (!std::isfinite(y[q][j])) continue;
                const auto &Pi = c[q].P[c[q].idx[j]];
                for (int u = 0; u < nd; ++u) {
                    b(u) += wq * f[q][j] * Pi[u] * y[q][j];
                    for (int v = 0; v < nd; ++v) N(u, v) += wq * f[q][j] * f[q][j] * Pi[u] * Pi[v];
                }
            }
        }
        arma::mat Ni;
        if (!arma::inv_sympd(Ni, N)) return NaN;
        a = Ni * b;
        if (Ninv) *Ninv = Ni;
        double q2 = 0;
        for (int q = 0; q < 2; ++q)
            for (size_t j = 0; j < y[q].size(); ++j) {
                if (!std::isfinite(y[q][j])) continue;
                const auto &Pi = c[q].P[c[q].idx[j]];
                double cc = 0;
                for (int u = 0; u < nd; ++u) cc += a(u) * Pi[u];
                q2 += std::pow((y[q][j] - f[q][j] * cc) / sig[q], 2);
            }
        return q2;
    };
    auto contAt = [&](const arma::vec &a, int q, size_t i) {
        double cc = 0;
        for (int u = 0; u < nd; ++u) cc += a(u) * c[q].P[i][u];
        return cc;
    };
    // minimum of chi2 in k: scan in ln k (factor 20 around the start), then golden section
    auto minimiseK = [&](double kStart) {
        const double lo = std::log(kStart) - std::log(20.0), hi = std::log(kStart) + std::log(20.0);
        const int ns = 81;
        double bx = NaN, bq = std::numeric_limits<double>::infinity();
        arma::vec aa;
        for (int i = 0; i < ns; ++i) {
            const double t = lo + (hi - lo) * i / (ns - 1);
            const double qq = chi2At(std::exp(t), aa);
            if (std::isfinite(qq) && qq < bq) { bq = qq; bx = t; }
        }
        if (!std::isfinite(bx)) return NaN;
        double a = bx - (hi - lo) / (ns - 1), b = bx + (hi - lo) / (ns - 1);
        const double gr = 0.5 * (std::sqrt(5.0) - 1.0);
        double x1 = b - gr * (b - a), x2 = a + gr * (b - a);
        double f1 = chi2At(std::exp(x1), aa), f2 = chi2At(std::exp(x2), aa);
        for (int it = 0; it < 200 && b - a > 1e-12; ++it) {
            if (f1 < f2) { b = x2; x2 = x1; f2 = f1; x1 = b - gr * (b - a); f1 = chi2At(std::exp(x1), aa); }
            else { a = x1; x1 = x2; f1 = f2; x2 = a + gr * (b - a); f2 = chi2At(std::exp(x2), aa); }
        }
        return std::exp(0.5 * (a + b));
    };
    double kBest = k0, qBest = NaN;
    arma::vec aBest(nd, arma::fill::zeros);
    aBest(0) = 1.0;
    for (int pass = 0; pass < 4; ++pass) {
        // separate fits: light fractions (check) and sigmas
        for (int q = 0; q < 2; ++q) {
            double cc, l;
            const double qq = fitLine(c[q].x->f, regressor(c[q], q, pass > 0 ? kBest : k0), c[q].idx, c[q].P, cc, l);
            res.lInd[q] = l;
            if (pass == 0 && std::isfinite(qq) && c[q].idx.size() > 2)
                sig[q] = std::max(1e-12, std::sqrt(qq / (c[q].idx.size() - 2)));
        }
        const double kStart = pass > 0 ? kBest : ((res.lInd[0] > 0 && res.lInd[1] > 0) ? res.lInd[0] / res.lInd[1] : k0);
        kBest = minimiseK(kStart);
        if (!std::isfinite(kBest)) { res.message = "Flux-ratio fit failed (no finite chi2)."; return res; }
        qBest = chi2At(kBest, aBest);
        // sigmas from the joint fit, clipping of outliers (e.g. lines missing in the synthetic spectrum)
        bool changed = false;
        for (int q = 0; q < 2; ++q) {
            std::vector<double> y, f;
            rowsFor(kBest, q, y, f);
            double s2 = 0; int n = 0;
            for (size_t j = 0; j < y.size(); ++j)
                if (std::isfinite(y[j])) { s2 += std::pow(y[j] - f[j] * contAt(aBest, q, c[q].idx[j]), 2); ++n; }
            const double sq = n > nd ? std::sqrt(s2 / n) : sig[q];
            std::vector<size_t> keep;
            for (size_t j = 0; j < y.size(); ++j)
                if (std::isfinite(y[j]) && std::fabs(y[j] - f[j] * contAt(aBest, q, c[q].idx[j])) <= opt.clip * sq)
                    keep.push_back(c[q].idx[j]);
            if (std::fabs(sq - sig[q]) > 0.01 * sig[q]) changed = true;
            sig[q] = std::max(1e-12, sq);
            if (keep.size() != c[q].idx.size() && keep.size() >= 20) { c[q].idx = keep; changed = true; }
        }
        if (!changed) break;
    }
    qBest = chi2At(kBest, aBest);
    for (int q = 0; q < 2; ++q) {       // check: separate fits with the shape of the result
        double cc, l;
        fitLine(c[q].x->f, regressor(c[q], q, kBest), c[q].idx, c[q].P, cc, l);
        res.lInd[q] = l;
    }
    // errors: curvature of chi2 in ln k (profile over the continuum), scaled with chi2 / dof
    int nobs = 0;
    for (int q = 0; q < 2; ++q) nobs += int(c[q].idx.size());
    res.dof = nobs - 1 - nd;
    res.chi2 = qBest;
    const double s2 = res.dof > 0 ? qBest / res.dof : 1.0;
    const double h = 1e-3;
    arma::vec am, ap;
    const double qm = chi2At(kBest * std::exp(-h), am), qp = chi2At(kBest * std::exp(h), ap);
    const double d2 = (qm - 2 * qBest + qp) / (h * h);
    res.errK = d2 > 0 ? kBest * std::sqrt(2.0 * s2 / d2) : NaN;
    // block jackknife: the residuals are correlated (remaining waviness of the continua,
    // mismatch of single lines), the formal error is then too small. The fit range is cut
    // into blocks of equal ln(lambda); k is refitted without each block in turn.
    {
        const int K = opt.jackBlocks;
        std::vector<double> ks;
        if (K >= 3) {
            const std::vector<size_t> keep[2] = {c[0].idx, c[1].idx};
            for (int b = 0; b < K; ++b) {
                const double t0 = tlo + (thi - tlo) * b / K, t1 = tlo + (thi - tlo) * (b + 1) / K;
                bool enough = true;
                for (int q = 0; q < 2; ++q) {
                    c[q].idx.clear();
                    for (size_t i : keep[q]) {
                        const double t = std::log(c[q].x->lam[i]);
                        if (t < t0 || t >= t1 || (b == K - 1 && t > t1)) c[q].idx.push_back(i);
                    }
                    enough = enough && c[q].idx.size() >= size_t(nd + 20);
                }
                if (enough) {
                    const double kb = minimiseK(kBest);
                    if (std::isfinite(kb)) ks.push_back(kb);
                }
            }
            c[0].idx = keep[0];
            c[1].idx = keep[1];
        }
        if (ks.size() >= 3) {
            double m = 0;
            for (double v : ks) m += v;
            m /= ks.size();
            double v2 = 0;
            for (double v : ks) v2 += (v - m) * (v - m);
            res.errKJack = std::sqrt((ks.size() - 1.0) / ks.size() * v2);
        }
    }
    {
        arma::mat Ni;
        arma::vec a;
        chi2At(kBest, a, &Ni);
        std::vector<double> Pr;
        basis(res.lambdaRef, Pr);
        double v = 0, cr = 0;
        for (int u = 0; u < nd; ++u) {
            cr += a(u) * Pr[u];
            for (int w = 0; w < nd; ++w) v += Pr[u] * Ni(u, w) * Pr[w];
        }
        res.cA = cr;
        res.errCA = std::sqrt(std::max(0.0, v * s2));
    }
    res.k = kBest;
    res.lA = kBest / (1 + kBest);
    res.lB = 1 / (1 + kBest);
    // model, renormalised components, RMS
    for (int q = 0; q < 2; ++q) {
        const auto &x = c[q].x->f;
        const size_t n = x.size();
        std::vector<double> &mod = q == 0 ? res.modelA : res.modelB;
        std::vector<double> &ren = q == 0 ? res.renA : res.renB;
        std::vector<char> &used = q == 0 ? res.usedA : res.usedB;
        mod.assign(n, NaN);
        ren.assign(n, NaN);
        used.assign(n, 0);
        for (size_t i : c[q].idx) used[i] = 1;
        double qq = 0;
        for (size_t i = 0; i < n; ++i) {
            const double r = kBest * c[q].s[i];
            const double l = q == 0 ? r / (1 + r) : 1 / (1 + r);
            const double w = c[q].w[i];
            const double cAi = contAt(aBest, q, i);
            const double cont = q == 0 ? cAi : (1.0 - (1.0 - w) * cAi) / w;
            ren[i] = 1.0 - (cont - x[i]) * w / l;
            if (std::isfinite(c[q].T[i])) mod[i] = cont - l / w * (1.0 - c[q].T[i]);
            if (used[i]) qq += std::pow(x[i] - mod[i], 2);
        }
        res.rms[q] = std::sqrt(qq / std::max<size_t>(1, c[q].idx.size()));
        res.npix[q] = int(c[q].idx.size());
        res.shift[q] = c[q].v;
    }
    res.ok = std::isfinite(res.k) && res.k > 0;
    res.message = res.ok ? "converged" : "Flux-ratio fit failed.";
    return res;
}

} // namespace tfit
