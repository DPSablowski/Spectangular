#include "continuum.h"

#include <armadillo>

#include <algorithm>
#include <cmath>
#include <limits>

namespace cont {

std::vector<std::pair<size_t, size_t>> pieces(const std::vector<double> &w, double gapFactor)
{
    std::vector<std::pair<size_t, size_t>> p;
    const size_t n = w.size();
    if (n == 0) return p;
    if (n < 3) { p.emplace_back(0, n); return p; }
    std::vector<double> d(n - 1);
    for (size_t i = 0; i + 1 < n; ++i) d[i] = w[i + 1] - w[i];
    std::vector<double> s = d;
    std::nth_element(s.begin(), s.begin() + s.size() / 2, s.end());
    const double med = s[s.size() / 2];
    size_t a = 0;
    for (size_t i = 0; i + 1 < n; ++i) {
        if (d[i] > gapFactor * med) {
            p.emplace_back(a, i + 1);
            a = i + 1;
        }
    }
    p.emplace_back(a, n);
    return p;
}

namespace {

// Cubic B-spline basis on [0, 1] with nInt equidistant interior knots: values of the four
// basis functions that are non-zero at x and the index of the first of them.
void basis(double x, int nInt, double N[4], int &first)
{
    const int nSpan = nInt + 1;
    int span = int(std::floor(x * nSpan));
    span = std::min(std::max(span, 0), nSpan - 1);
    // knot vector: 0,0,0,0, 1/nSpan, ..., 1,1,1,1 -> knot(i) for i = 0 .. nSpan + 6
    auto knot = [&](int i) {
        const int j = i - 3;
        if (j <= 0) return 0.0;
        if (j >= nSpan) return 1.0;
        return double(j) / nSpan;
    };
    const int k = span + 3;                    // index of the knot interval [knot(k), knot(k+1))
    double left[4], right[4];
    N[0] = 1.0;
    for (int j = 1; j <= 3; ++j) {
        left[j] = x - knot(k + 1 - j);
        right[j] = knot(k + j) - x;
        double saved = 0.0;
        for (int r = 0; r < j; ++r) {
            const double den = right[r + 1] + left[j - r];
            const double t = den != 0.0 ? N[r] / den : 0.0;
            N[r] = saved + right[r + 1] * t;
            saved = left[j - r] * t;
        }
        N[j] = saved;
    }
    first = span;
}

// Least-squares fit of one piece with the selected points; model values for all points
bool fitPiece(const std::vector<double> &x, const std::vector<double> &y, const std::vector<char> &use,
              int nInt, int polyDeg, std::vector<double> &model)
{
    const size_t n = x.size();
    model.assign(n, 0.0);
    if (nInt >= 0 && polyDeg < 0) {
        const int nb = nInt + 4;
        arma::mat A(nb, nb, arma::fill::zeros);
        arma::vec b(nb, arma::fill::zeros);
        size_t used = 0;
        for (size_t i = 0; i < n; ++i) {
            if (!use[i]) continue;
            double N[4];
            int f;
            basis(x[i], nInt, N, f);
            for (int r = 0; r < 4; ++r) {
                b(f + r) += N[r] * y[i];
                for (int c = 0; c < 4; ++c) A(f + r, f + c) += N[r] * N[c];
            }
            ++used;
        }
        if (used < size_t(nb)) return false;
        A.diag() += 1e-10 * arma::trace(A) / nb;   // basis functions without selected points
        arma::vec c;
        if (!arma::solve(c, A, b)) return false;
        for (size_t i = 0; i < n; ++i) {
            double N[4];
            int f;
            basis(x[i], nInt, N, f);
            double v = 0.0;
            for (int r = 0; r < 4; ++r) v += N[r] * c(f + r);
            model[i] = v;
        }
        return true;
    }
    // polynomial of degree polyDeg (0 or 1)
    const int m = polyDeg + 1;
    arma::mat A(m, m, arma::fill::zeros);
    arma::vec b(m, arma::fill::zeros);
    size_t used = 0;
    for (size_t i = 0; i < n; ++i) {
        if (!use[i]) continue;
        double p[2] = {1.0, x[i] - 0.5};
        for (int r = 0; r < m; ++r) {
            b(r) += p[r] * y[i];
            for (int c = 0; c < m; ++c) A(r, c) += p[r] * p[c];
        }
        ++used;
    }
    if (used < size_t(m)) return false;
    arma::vec c;
    if (!arma::solve(c, A, b)) return false;
    for (size_t i = 0; i < n; ++i) model[i] = c(0) + (m > 1 ? c(1) * (x[i] - 0.5) : 0.0);
    return true;
}

} // namespace

bool fit(const std::vector<double> &w, const std::vector<double> &f, const Options &o,
         std::vector<double> &continuum, std::string &err)
{
    const size_t n = w.size();
    if (n < 3 || f.size() != n) { err = "fewer than 3 points"; return false; }
    for (size_t i = 0; i < n; ++i) {
        if (!std::isfinite(w[i]) || !std::isfinite(f[i])) { err = "non-finite values"; return false; }
        if (i > 0 && !(w[i] > w[i - 1])) { err = "wavelengths not increasing"; return false; }
    }
    if (!(o.knotA > 0)) { err = "knot distance must be positive"; return false; }
    continuum.assign(n, 1.0);
    for (const auto &pc : pieces(w, o.gapFactor)) {
        const size_t a = pc.first, m = pc.second - pc.first;
        const double L = w[pc.second - 1] - w[a];
        std::vector<double> x(m), y(m), model;
        for (size_t i = 0; i < m; ++i) {
            x[i] = L > 0 ? (w[a + i] - w[a]) / L : 0.0;
            y[i] = f[a + i];
        }
        int nInt = -1, deg = -1;
        if (L >= o.knotA && m >= 8) nInt = int(std::floor(L / o.knotA)) - 1;   // knots every knotA
        if (nInt < 0) deg = (L < 20.0 || m < 4) ? 0 : 1;
        else nInt = std::max(nInt, 0);
        std::vector<char> use(m, 1);
        bool ok = false;
        for (int it = 0; it < o.maxIter; ++it) {
            if (!fitPiece(x, y, use, nInt, deg, model)) break;
            ok = true;
            // noise from the points above the fit only: lines lie below it and would inflate a
            // symmetric estimate (and with it the threshold below the fit)
            double s2 = 0.0;
            size_t cnt = 0;
            for (size_t i = 0; i < m; ++i) {
                if (!use[i]) continue;
                const double r = y[i] - model[i];
                if (r > 0) { s2 += r * r; ++cnt; }
            }
            const double s = std::sqrt(s2 / std::max<size_t>(cnt, 1));
            if (!(s > 0)) break;
            bool changed = false;
            size_t kept = 0;
            std::vector<char> nu(m);
            for (size_t i = 0; i < m; ++i) {
                const double r = y[i] - model[i];
                nu[i] = r > -o.low * s && r < o.high * s;
                kept += nu[i];
                changed |= nu[i] != use[i];
            }
            if (!changed || kept < 4) break;
            use.swap(nu);
        }
        if (!ok) {                        // too few points: median as a constant
            std::vector<double> s = y;
            std::nth_element(s.begin(), s.begin() + s.size() / 2, s.end());
            model.assign(m, s[s.size() / 2]);
        }
        for (size_t i = 0; i < m; ++i) continuum[a + i] = model[i];
    }
    return true;
}

namespace {

// normalised boxcar: sum(v w) / sum(w) over [i - h, i + h]; where sum(w) = 0: NaN
std::vector<double> boxcar(const std::vector<double> &v, const std::vector<double> &w, int h)
{
    const size_t n = v.size();
    std::vector<double> cv(n + 1, 0.0), cw(n + 1, 0.0), out(n);
    for (size_t i = 0; i < n; ++i) { cv[i + 1] = cv[i] + v[i] * w[i]; cw[i + 1] = cw[i] + w[i]; }
    for (size_t i = 0; i < n; ++i) {
        const size_t a = i >= size_t(h) ? i - h : 0, b = std::min(n, i + h + 1);
        const double sw = cw[b] - cw[a];
        out[i] = sw > 0 ? (cv[b] - cv[a]) / sw : std::numeric_limits<double>::quiet_NaN();
    }
    return out;
}

} // namespace

bool alignToMean(std::vector<std::vector<double>> &flux, const std::vector<std::vector<char>> &marked,
                 int widthPx, std::vector<double> &corrRms, std::string &err)
{
    const size_t num = flux.size();
    if (num < 2) { err = "fewer than 2 spectra"; return false; }
    const size_t n = flux[0].size();
    for (const auto &f : flux) if (f.size() != n || n < 3) { err = "spectra of different length"; return false; }
    const bool useMask = marked.size() == num;
    auto isMarked = [&](size_t j, size_t i) { return useMask && i < marked[j].size() && marked[j][i]; };
    // mean spectrum (unmarked pixels)
    std::vector<double> mean(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double s = 0.0;
        int c = 0;
        for (size_t j = 0; j < num; ++j) if (!isMarked(j, i) && std::isfinite(flux[j][i])) { s += flux[j][i]; ++c; }
        mean[i] = c > 0 ? s / c : std::numeric_limits<double>::quiet_NaN();
    }
    std::vector<double> am;
    for (double m : mean) if (std::isfinite(m)) am.push_back(std::fabs(m));
    if (am.empty()) { err = "no usable pixels"; return false; }
    std::nth_element(am.begin(), am.begin() + am.size() / 2, am.end());
    const double floorM = 0.05 * am[am.size() / 2];
    const int h = std::max(1, widthPx / 2);
    corrRms.assign(num, 0.0);
    for (size_t j = 0; j < num; ++j) {
        std::vector<double> r(n, 0.0), w(n, 0.0);
        for (size_t i = 0; i < n; ++i) {
            if (isMarked(j, i) || !(mean[i] > floorM) || !std::isfinite(flux[j][i])) continue;
            r[i] = flux[j][i] / mean[i];
            w[i] = 1.0;
        }
        std::vector<double> s1 = boxcar(r, w, h);
        std::vector<double> w1(n);
        for (size_t i = 0; i < n; ++i) { w1[i] = std::isfinite(s1[i]) ? 1.0 : 0.0; if (!w1[i]) s1[i] = 0.0; }
        std::vector<double> corr = boxcar(s1, w1, h);
        // bridge pixels without any valid neighbour: nearest valid value
        double last = std::numeric_limits<double>::quiet_NaN();
        for (size_t i = 0; i < n; ++i) { if (std::isfinite(corr[i])) last = corr[i]; else corr[i] = last; }
        last = std::numeric_limits<double>::quiet_NaN();
        for (size_t i = n; i-- > 0;) { if (std::isfinite(corr[i])) last = corr[i]; else if (!std::isfinite(corr[i])) corr[i] = last; }
        double q = 0.0;
        for (size_t i = 0; i < n; ++i) {
            if (!(corr[i] > 0) || !std::isfinite(corr[i])) corr[i] = 1.0;
            flux[j][i] /= corr[i];
            q += (corr[i] - 1.0) * (corr[i] - 1.0);
        }
        corrRms[j] = std::sqrt(q / n);
    }
    return true;
}

} // namespace cont
