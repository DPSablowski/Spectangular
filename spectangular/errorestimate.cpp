#include "errorestimate.h"

#include <algorithm>
#include <sstream>
#include <cmath>

namespace errest {

Result quadraticFit(const arma::mat &P, const std::vector<double> &S,
                    const std::function<double(const arma::rowvec &)> &f,
                    const std::function<bool()> &abort)
{
    Result r;
    const int n = int(P.n_cols);
    if (int(P.n_rows) != n + 1 || int(S.size()) != n + 1 || n < 1) {
        r.message = "Simplex has wrong dimensions.";
        return r;
    }
    int b = 0;
    for (int i = 1; i <= n; ++i)
        if (S[i] < S[b]) b = i;
    r.best = b;

    // vertices without the best point, in fixed order
    std::vector<int> idx;
    for (int i = 0; i <= n; ++i)
        if (i != b) idx.push_back(i);

    auto eval = [&](const arma::rowvec &x, double &v) {
        v = f(x);
        ++r.evaluations;
        if (abort()) {
            r.aborted = true;
            r.message = "Error estimation aborted.";
            return false;
        }
        return true;
    };

    const double S0 = S[b];
    arma::vec S0i(n);
    for (int k = 0; k < n; ++k) {
        if (!eval(0.5 * (P.row(b) + P.row(idx[k])), S0i(k)))
            return r;
    }
    r.B.set_size(n, n);
    r.a.set_size(n);
    for (int k = 0; k < n; ++k) {
        r.a(k) = 2.0 * S0i(k) - 0.5 * (S[idx[k]] + 3.0 * S0);
        r.B(k, k) = 2.0 * (S[idx[k]] + S0 - 2.0 * S0i(k));
        for (int m = k + 1; m < n; ++m) {
            double Skm;
            if (!eval(0.5 * (P.row(idx[k]) + P.row(idx[m])), Skm))
                return r;
            r.B(k, m) = r.B(m, k) = 2.0 * (Skm + S0 - S0i(k) - S0i(m));
        }
    }

    r.Q.set_size(n, n);
    for (int k = 0; k < n; ++k)
        r.Q.col(k) = (P.row(idx[k]) - P.row(b)).t();

    if (arma::rcond(r.Q) < 1e-14) {
        r.message = "The simplex is degenerate (matrix Q is singular).";
        return r;
    }
    arma::mat Binv;
    if (!arma::inv_sympd(Binv, r.B)) {
        arma::vec ev = arma::eig_sym(arma::symmatu(r.B));
        std::ostringstream m;
        m.precision(3);
        m << "The quadratic model is not positive definite (eigenvalues of B:";
        for (double v : ev)
            m << " " << v;
        m << "); the simplex is probably not close to a minimum.";
        r.message = m.str();
        return r;
    }
    r.minimum = P.row(b) - (r.Q * Binv * r.a).t();
    r.covUnit = r.Q * Binv * r.Q.t();
    r.ok = true;
    return r;
}

namespace {

// Core: simplex x0 + t_i d_i with directions d_i (columns of D). t_i starts at 1 and
// is adjusted so that the curvature part of S rises by target (factor 2).
ProbeResult probeDirections(const arma::rowvec &x0, double S0, const arma::mat &D,
                            const arma::vec &tMax, double target,
                            const std::function<double(const arma::rowvec &)> &f,
                            const std::function<bool()> &abort, double invalidAbove, int maxIter)
{
    ProbeResult r;
    const int n = int(x0.n_cols);
    r.P.set_size(n + 1, n);
    r.P.row(0) = x0;
    r.S.assign(n + 1, S0);
    r.steps.zeros(n);

    auto valid = [&](double v) { return std::isfinite(v) && v < invalidAbove; };
    auto eval = [&](const arma::rowvec &x, double &v) {
        v = f(x);
        ++r.evaluations;
        if (abort()) {
            r.aborted = true;
            r.message = "Error estimation aborted.";
            return false;
        }
        return true;
    };

    for (int i = 0; i < n; ++i) {
        const arma::rowvec d = D.col(i).t();
        double t = 1.0;
        bool done = false, noEffect = false;
        double bestDev = HUGE_VAL, bestT = 0, bestS = 0;
        for (int it = 0; it < maxIter && !done; ++it) {
            const arma::rowvec xp = x0 + t * d, xm = x0 - t * d;
            double Sp, Sm;
            if (!eval(xp, Sp) || !eval(xm, Sm))
                return r;
            const bool vp = valid(Sp), vm = valid(Sm);
            double dS;
            if (vp && vm)
                dS = 0.5 * (Sp + Sm) - S0;            // curvature part
            else if (vp || vm)
                dS = (vp ? Sp : Sm) - S0;             // one-sided at the boundary
            else {
                t /= 10.0;
                continue;
            }
            // vertex on the side with larger S (with a gradient, distance from S0 >= dS)
            const bool usePlus = vp && (!vm || Sp >= Sm);
            if (dS > 0) {
                double dev = std::abs(std::log(dS / target));
                if (dev < bestDev) {
                    bestDev = dev;
                    bestT = usePlus ? t : -t;
                    bestS = usePlus ? Sp : Sm;
                }
                if (dev < std::log(2.0)) {
                    done = true;
                    break;
                }
            }
            if (dS <= 1e-3 * target) {
                if (t >= tMax(i)) {
                    noEffect = true;
                    break;
                }
                t = std::min(t * 100.0, tMax(i));
            }
            else {
                t *= std::min(100.0, std::max(0.01, std::sqrt(target / dS)));
            }
        }
        if (noEffect || bestT == 0) {
            r.undetermined.push_back(i);
            continue;
        }
        r.steps(i) = bestT;
        r.P.row(i + 1) = x0 + bestT * d;
        r.S[i + 1] = bestS;
    }
    if (!r.undetermined.empty()) {
        r.message = "Parameter(s) without measurable influence on the residuum:";
        for (int i : r.undetermined)
            r.message += " " + std::to_string(i + 1);
        r.message += ". Lock them for the error estimation.";
        return r;
    }
    r.ok = true;
    return r;
}

} // namespace

ProbeResult probeSimplex(const arma::rowvec &x0, double S0, const arma::vec &h0, double target,
                         const std::function<double(const arma::rowvec &)> &f,
                         const std::function<bool()> &abort, double invalidAbove, int maxIter)
{
    const int n = int(x0.n_cols);
    if (n < 1 || int(h0.n_elem) != n || !(target > 0) || !std::isfinite(S0)) {
        ProbeResult r;
        r.message = "Wrong input for the new simplex.";
        return r;
    }
    arma::mat D(n, n, arma::fill::zeros);
    arma::vec tMax(n);
    for (int i = 0; i < n; ++i) {
        const double scale = std::max(1.0, std::abs(x0(i)));
        double h = h0(i) > 0 ? h0(i) : 1e-6 * scale;
        h = std::max(h, 1e-13 * scale);             // below this, only rounding
        D(i, i) = h;
        tMax(i) = 1e8 * scale / h;
    }
    ProbeResult r = probeDirections(x0, S0, D, tMax, target, f, abort, invalidAbove, maxIter);
    for (int i = 0; i < n; ++i)
        r.steps(i) *= D(i, i);                       // step in parameter units
    return r;
}

ProbeResult probePrincipal(const arma::rowvec &x0, double S0, const Result &first, double target,
                           const std::function<double(const arma::rowvec &)> &f,
                           const std::function<bool()> &abort, double invalidAbove, int maxIter)
{
    const int n = int(x0.n_cols);
    ProbeResult r;
    if (int(first.B.n_rows) != n || int(first.Q.n_rows) != n || !(target > 0)) {
        r.message = "Wrong input for the principal-axis simplex.";
        return r;
    }
    arma::vec lam;
    arma::mat U;
    if (!arma::eig_sym(lam, U, arma::symmatu(first.B))) {
        r.message = "Eigen decomposition of B failed.";
        return r;
    }
    // Directions in parameter units: Q u_j, length such that the model raises S by
    // target. Non-positive or tiny eigenvalues: lower limit 1e-6 * largest.
    const double lmax = std::max(std::abs(lam.max()), std::abs(lam.min()));
    arma::mat D(n, n);
    for (int j = 0; j < n; ++j) {
        double l = std::max(std::abs(lam(j)), 1e-6 * lmax);
        D.col(j) = first.Q * U.col(j) * std::sqrt(target / l);
    }
    arma::vec tMax(n);
    tMax.fill(1e4);
    r = probeDirections(x0, S0, D, tMax, target, f, abort, invalidAbove, maxIter);
    return r;
}

} // namespace errest
