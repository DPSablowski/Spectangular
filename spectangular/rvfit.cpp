#include "rvfit.h"

#include "dataio.h"
#include "levmar.h"
#include "orbit.h"

#include <cmath>

namespace rvfit {

bool readTable(const std::string &file, bool sb2, Data &d, std::string &err)
{
    std::vector<std::vector<double>> rows;
    if (!dataio::readTable(file, sb2 ? 3 : 2, rows, err))
        return false;
    d = Data();
    const bool withErr = !rows.empty() && rows[0].size() >= size_t(sb2 ? 5 : 3);
    for (const auto &r : rows) {
        d.t.push_back(r[0]);
        d.rvA.push_back(r[1]);
        if (sb2) d.rvB.push_back(r[2]);
        if (withErr && r.size() >= size_t(sb2 ? 5 : 3)) {
            d.sigA.push_back(r[sb2 ? 3 : 2]);
            if (sb2) d.sigB.push_back(r[4]);
        }
    }
    if (withErr && d.sigA.size() != d.t.size()) {
        err = "File " + file + ": errors must be given in all rows or in none.";
        return false;
    }
    for (double s : d.sigA) if (!(s > 0)) { err = "File " + file + ": errors must be positive."; return false; }
    for (double s : d.sigB) if (!(s > 0)) { err = "File " + file + ": errors must be positive."; return false; }
    if (d.t.size() < 3) {
        err = "File " + file + ": at least three observations are needed.";
        return false;
    }
    return true;
}

void model(const double el[7], double t, double &vA, double &vB)
{
    vA = orbit::radialVelocity(t, el[0], el[1], el[2], el[4], el[5], el[6]);
    vB = orbit::radialVelocity(t, el[0], el[1], el[3], el[4], el[5], el[6] + M_PI);
}

double phase(double t, double period, double tPeri)
{
    double p = std::fmod((t - tPeri) / period, 1.0);
    return p < 0 ? p + 1.0 : p;
}

namespace {

Result fitOnce(const Data &d, const double start[7], const bool fixed[7], int maxIter,
               const std::vector<double> &sA, const std::vector<double> &sB, double off0 = 0.0)
{
    Result res;
    const bool sb2 = d.sb2();
    const size_t n = d.t.size();
    for (int i = 0; i < 7; ++i) {
        res.el[i] = start[i];
        if (!fixed[i] && (sb2 || i != 3)) res.freeIdx.push_back(i);   // K_B is meaningless for SB1
    }
    if (sb2 && d.offsetB) res.freeIdx.push_back(7);
    const int nf = int(res.freeIdx.size());
    const int nobs = int(n) * (sb2 ? 2 : 1);
    res.dof = nobs - nf;
    if (nf == 0) {
        res.message = "All orbital elements are fixed.";
        return res;
    }
    if (res.dof < 1) {
        res.message = "Too few RVs for the free elements (" + std::to_string(nobs) + " RVs, "
                      + std::to_string(nf) + " free elements).";
        return res;
    }
    double el[8];
    auto toEl = [&](const arma::vec &x) {
        for (int i = 0; i < 7; ++i) el[i] = start[i];
        el[7] = off0;
        for (int k = 0; k < nf; ++k) el[res.freeIdx[k]] = x(k);
    };
    lm::ResidualFn f = [&](const arma::vec &x, arma::vec &r, bool) {
        toEl(x);
        if (!orbit::elementsValid(el[0], el[1])) return false;
        // K >= 0: otherwise (K, omega) and (-K, omega + pi) describe the same orbit
        if (el[2] < 0 || (sb2 && el[3] < 0)) return false;
        r.set_size(nobs);
        for (size_t j = 0; j < n; ++j) {
            double vA, vB;
            model(el, d.t[j], vA, vB);
            vB += el[7];
            r(j) = (d.rvA[j] - vA) / sA[j];
            if (sb2) r(n + j) = (d.rvB[j] - vB) / sB[j];
        }
        return r.is_finite();
    };
    arma::vec x0(nf), h(nf);
    const double scaleK = std::max(1.0, std::fabs(start[2]));
    for (int k = 0; k < nf; ++k) {
        const int i = res.freeIdx[k];
        x0(k) = i < 7 ? start[i] : off0;
        switch (i) {
        case 0: h(k) = 1e-7 * std::max(1.0, start[0]); break;        // P
        case 1: h(k) = 1e-6; break;                                    // e
        case 5: h(k) = 1e-6 * std::max(1.0, start[0]); break;          // T_peri (relative to P)
        case 6: h(k) = 1e-6; break;                                    // omega [rad]
        default: h(k) = 1e-6 * scaleK; break;                          // K, gamma
        }
    }
    lm::Options opt;
    opt.h = h;
    opt.maxIter = maxIter;
    opt.secant = false;        // ordinary least squares: Gauss-Newton curvature is right
    opt.relTolS = 1e-14;
    opt.tolX = 1e-6;
    lm::Result r = lm::minimize(x0, f, opt);
    if (!r.ok) {
        res.message = "Orbit fit failed: " + r.message;
        return res;
    }
    res.iterations = r.iterations;
    res.chi2 = r.S;
    const double s2 = r.S / res.dof;
    // covariance from J^T J at the solution (central differences; the curvature model of
    // levmar may belong to an earlier point)
    arma::mat J(nobs, nf);
    bool jok = true;
    for (int k = 0; k < nf && jok; ++k) {
        arma::vec xp = r.x, xm = r.x, rp, rm;
        xp(k) += h(k);
        xm(k) -= h(k);
        jok = f(xp, rp, false) && f(xm, rm, false);
        if (jok) J.col(k) = (rp - rm) / (2.0 * h(k));
    }
    toEl(r.x);
    res.cov = jok ? lm::covariance(J.t() * J, s2) : arma::mat();
    for (int i = 0; i < 7; ++i) res.el[i] = el[i];
    res.el[6] = std::fmod(res.el[6], 2.0 * M_PI);       // omega in [0, 2 pi)
    if (res.el[6] < 0) res.el[6] += 2.0 * M_PI;
    for (int i = 0; i < 7; ++i) res.err[i] = 0.0;
    res.offB = el[7];
    if (res.cov.n_rows == arma::uword(nf))
        for (int k = 0; k < nf; ++k) {
            const double e = std::sqrt(std::max(0.0, res.cov(k, k)));
            if (res.freeIdx[k] < 7) res.err[res.freeIdx[k]] = e;
            else res.errOffB = e;
        }
    double qa = 0.0, qb = 0.0;
    res.resA.assign(n, 0.0);
    if (sb2) res.resB.assign(n, 0.0);
    for (size_t j = 0; j < n; ++j) {
        double vA, vB;
        model(res.el, d.t[j], vA, vB);
        vB += res.offB;
        res.resA[j] = d.rvA[j] - vA;
        qa += res.resA[j] * res.resA[j];
        if (sb2) {
            res.resB[j] = d.rvB[j] - vB;
            qb += res.resB[j] * res.resB[j];
        }
    }
    res.rmsA = std::sqrt(qa / n);
    res.rmsB = sb2 ? std::sqrt(qb / n) : 0.0;
    res.sigUsedA = sA;
    res.sigUsedB = sB;
    res.ok = true;
    res.message = r.converged ? "converged" : ("not converged: " + r.message);
    return res;
}

} // namespace

Result fit(const Data &d, const double start0[7], const bool fixed[7], int maxIter)
{
    const size_t n = d.t.size();
    if (n == 0 || d.rvA.size() != n || (d.sb2() && d.rvB.size() != n)) {
        Result r;
        r.message = "Times and RVs do not match.";
        return r;
    }
    // T_peri is moved by whole periods to the periastron passage nearest to the mean time of
    // the observations (the same orbit): far from the data, T and P are strongly correlated
    // and a small error of the start P puts the start phase anywhere.
    double start[7];
    for (int i = 0; i < 7; ++i) start[i] = start0[i];
    if (!fixed[5] && start[0] > 0) {
        double tm = 0.0;
        for (double t : d.t) tm += t;
        tm /= double(n);
        start[5] += std::round((tm - start[5]) / start[0]) * start[0];
    }
    const bool given = d.sigA.size() == n && (!d.sb2() || d.sigB.size() == n);
    std::vector<double> one(n, 1.0);
    const std::vector<double> &wA = given ? d.sigA : one;
    const std::vector<double> wB = d.sb2() ? (given ? d.sigB : one) : std::vector<double>();
    // Several start phases: T_peri + k P/8 (if T is free). Least squares in the orbital
    // elements has local minima (e.g. e -> 0); the best of the eight fits is kept.
    Result r1;
    const int nStart = fixed[5] ? 1 : 8;
    for (int k = 0; k < nStart; ++k) {
        double s[7];
        for (int i = 0; i < 7; ++i) s[i] = start[i];
        s[5] += k * start[0] / nStart;
        Result rk = fitOnce(d, s, fixed, maxIter, wA, wB);
        if (rk.ok && (!r1.ok || rk.chi2 < r1.chi2)) r1 = rk;
    }
    auto epoch = [&](Result &r) {
        // periastron passage nearest to the mean time of the data; covariance of the shifted
        // T = T + m P: Var T + m^2 Var P + 2 m Cov(T, P), Cov(T', x) = Cov(T, x) + m Cov(P, x)
        if (!r.ok || fixed[5]) return;
        double tm = 0.0;
        for (double t : d.t) tm += t;
        tm /= double(n);
        const double m = std::round((tm - r.el[5]) / r.el[0]);
        if (m == 0.0) return;
        r.el[5] += m * r.el[0];
        int kT = -1, kP = -1;
        for (size_t k = 0; k < r.freeIdx.size(); ++k) {
            if (r.freeIdx[k] == 5) kT = int(k);
            if (r.freeIdx[k] == 0) kP = int(k);
        }
        if (kT >= 0 && kP >= 0 && r.cov.n_rows == r.freeIdx.size()) {
            arma::mat A = arma::eye(r.cov.n_rows, r.cov.n_cols);
            A(kT, kP) = m;
            r.cov = A * r.cov * A.t();
            r.err[5] = std::sqrt(std::max(0.0, r.cov(kT, kT)));
        }
    };
    if (given || !r1.ok) {
        epoch(r1);
        return r1;
    }
    // unknown errors: the RMS of each component as its error
    if (!r1.ok || !d.sb2() || !(r1.rmsA > 0) || !(r1.rmsB > 0))
        return r1;
    std::vector<double> sA(n, r1.rmsA), sB(n, r1.rmsB);
    Result r2 = fitOnce(d, r1.el, fixed, maxIter, sA, sB, r1.offB);
    Result &best = r2.ok ? r2 : r1;
    epoch(best);
    return best;
}

} // namespace rvfit
