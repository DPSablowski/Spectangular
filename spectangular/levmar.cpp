#include "levmar.h"

#include <cmath>
#include <sstream>

namespace lm {

namespace {

// Difference Jacobian at x (residual r). Central differences: the gradient g = J^T r must be
// accurate although r is mostly noise; a forward difference adds (h/2) r . d2r/dx2, and in
// variable projection r . d2r/dx2 is large (the second-order term), which biases g by up to
// an order of magnitude near the minimum. Forward differences only where one side is not
// admissible (e.g. e = 0).
bool jacobian(const ResidualFn &f, const arma::vec &x, const arma::vec &r, const arma::vec &h,
              bool central, arma::mat &J, int &evals, const std::function<bool()> &abort)
{
    J.set_size(r.n_elem, x.n_elem);
    arma::vec xp = x, rp, rm;
    for (arma::uword i = 0; i < x.n_elem; ++i) {
        if (abort && abort())
            return false;
        xp(i) = x(i) + h(i);
        ++evals;
        const bool okp = f(xp, rp, false) && rp.n_elem == r.n_elem;
        bool okm = false;
        if (central || !okp) {
            xp(i) = x(i) - h(i);
            ++evals;
            okm = f(xp, rm, false) && rm.n_elem == r.n_elem;
        }
        xp(i) = x(i);
        if (okp && okm)
            J.col(i) = (rp - rm) / (2.0 * h(i));
        else if (okp)
            J.col(i) = (rp - r) / h(i);
        else if (okm)
            J.col(i) = (r - rm) / h(i);
        else
            return false;
    }
    return true;
}

} // namespace

Result minimize(const arma::vec &x0, const ResidualFn &f, const Options &opt,
                const std::function<bool()> &abort,
                const std::function<void(int, const arma::vec &, double)> &progress)
{
    Result res;
    std::ostringstream log;
    log.precision(15);
    const arma::uword n = x0.n_elem;
    if (opt.h.n_elem != n || n == 0) {
        res.message = "Levenberg-Marquardt: number of step sizes does not match the parameters.";
        return res;
    }
    arma::vec x = x0, r;
    ++res.evaluations;
    if (!f(x, r, true) || r.n_elem == 0) {
        res.message = "Levenberg-Marquardt: the start point cannot be evaluated.";
        return res;
    }
    res.ok = true;
    double S = arma::dot(r, r);
    double lambda = -1.0, nu = 2.0;
    arma::mat J, Jold, T(n, n, arma::fill::zeros);
    arma::vec xold, rold;
    bool haveOld = false;
    log << "LM start: S = " << S << "\n";

    for (int it = 0; it < opt.maxIter; ++it) {
        if (abort && abort()) { res.aborted = true; break; }
        if (!jacobian(f, x, r, opt.h, opt.central, J, res.evaluations, abort)) {
            if (abort && abort()) { res.aborted = true; break; }
            res.message = "Levenberg-Marquardt: Jacobian could not be evaluated.";
            break;
        }
        ++res.jacobians;
        arma::mat A = J.t() * J;
        const arma::vec g = J.t() * r;
        if (opt.secant && haveOld) {
            // Dennis-Gay-Welsch (NL2SOL) secant update of the second-order term T ~ sum r_i Hess(r_i)
            const arma::vec sx = x - xold;
            const arma::vec y = g - Jold.t() * rold;          // change of the gradient of S/2
            const arma::vec yh = (J - Jold).t() * r;          // y minus the change of J^T J s
            const double ys = arma::dot(y, sx);
            if (ys > 0) {
                const double sTs = arma::dot(sx, T * sx);
                if (sTs != 0.0) T *= std::min(1.0, std::fabs(arma::dot(sx, yh)) / std::fabs(sTs));
                const arma::vec v = yh - T * sx;
                T += (v * y.t() + y * v.t()) / ys - (arma::dot(v, sx) / (ys * ys)) * (y * y.t());
                T = 0.5 * (T + T.t());
            }
        }
        Jold = J; xold = x; rold = r; haveOld = true;
        const arma::mat Agn = A;
        if (opt.secant) {
            A += T;
            // safeguard: curvature of the corrected model between minCurv and maxCurv times the
            // Gauss-Newton curvature in every direction (generalised eigenvalues of (A, J^T J));
            // an unbounded secant term can make the model nearly flat in one direction
            arma::mat L;
            const double eps = 1e-12 * std::max(1e-300, arma::trace(Agn) / double(n));
            if (arma::chol(L, Agn + eps * arma::eye(n, n), "lower")) {
                const arma::mat Li = arma::inv(arma::trimatl(L));
                arma::mat B = Li * A * Li.t();
                B = 0.5 * (B + B.t());
                arma::vec ev;
                arma::mat V;
                if (arma::eig_sym(ev, V, B)) {
                    ev = arma::clamp(ev, opt.minCurv, opt.maxCurv);
                    A = L * V * arma::diagmat(ev) * V.t() * L.t();
                    A = 0.5 * (A + A.t());
                }
            }
        }
        res.JtJ = A;
        res.JtJx = x;
        // Newton decrement: predicted decrease of S for the full model step, g^T A^-1 g
        {
            arma::vec z;
            if (arma::solve(z, A, g, arma::solve_opts::no_approx)) {
                const double dec = arma::dot(g, z);
                if (dec >= 0 && dec < opt.tolS) {
                    log << "LM converged: predicted remaining decrease " << dec << "\n";
                    res.converged = true;
                    break;
                }
            }
        }
        arma::vec d = Agn.diag();
        const double dmax = d.max();
        for (arma::uword i = 0; i < n; ++i)          // floor: parameters without effect
            if (!(d(i) > 1e-14 * dmax)) d(i) = 1e-14 * dmax > 0 ? 1e-14 * dmax : 1.0;
        if (lambda < 0) lambda = opt.lambda0;

        bool accepted = false, stop = false;
        for (int rej = 0; rej < opt.maxRejects; ++rej) {
            if (abort && abort()) { res.aborted = true; stop = true; break; }
            arma::mat Ad = A;
            Ad.diag() += lambda * d;
            arma::vec dx;
            arma::mat Rc;
            // the corrected model may be indefinite: more damping until positive definite
            if (!arma::chol(Rc, Ad) || !arma::solve(dx, Ad, -g, arma::solve_opts::no_approx)) {
                lambda = std::max(lambda * nu, 1e-12); nu *= 2;
                continue;
            }
            arma::vec xn = x + dx, rn;
            ++res.evaluations;
            bool ok = f(xn, rn, true) && rn.n_elem == r.n_elem;
            double Sn = ok ? arma::dot(rn, rn) : arma::datum::inf;
            // predicted decrease of the quadratic model: -2 g.dx - dx.A.dx = dx.(lambda D dx - g);
            // gain ratio for the damping update from the step itself (before the line search)
            const double pred = arma::dot(dx, lambda * (d % dx) - g);
            const double rho = pred > 0 ? (S - Sn) / pred : -1.0;
            if (ok && opt.lineSearch && Sn < S) {
                // parabola in the step length a through S(0), S'(0) = 2 g.dx and S(1):
                // the model curvature overestimates the true one, the minimum lies at a > 1
                const double s0 = 2.0 * arma::dot(g, dx);
                const double c = Sn - S - s0;
                if (c > 0) {
                    const double a = std::min(opt.maxStretch, -s0 / (2.0 * c));
                    if (a > 1.2) {
                        arma::vec xa = x + a * dx, ra;
                        ++res.evaluations;
                        if (f(xa, ra, true) && ra.n_elem == r.n_elem) {
                            const double Sa = arma::dot(ra, ra);
                            if (Sa < Sn) { xn = xa; rn = ra; Sn = Sa; dx *= a; }
                        }
                    }
                }
            }
            bool smallStep = true;
            for (arma::uword i = 0; i < n; ++i)
                if (std::fabs(dx(i)) >= opt.tolX * std::fabs(opt.h(i))) smallStep = false;
            if (ok && Sn < S) {
                const double dS = S - Sn;
                x = xn; r = rn; S = Sn;
                lambda *= std::max(1.0 / 3.0, 1.0 - std::pow(2.0 * std::min(rho, 1.0) - 1.0, 3));
                nu = 2.0;
                accepted = true;
                ++res.iterations;
                log << "LM it " << res.iterations << ": S = " << S << ", dS = " << dS
                    << ", lambda = " << lambda << "\n";
                if (progress) progress(res.iterations, x, S);
                if (dS < opt.relTolS * S || smallStep) {
                    res.converged = true;
                    stop = true;
                }
                break;
            }
            {
                double q = 0.0;
                for (arma::uword i = 0; i < n; ++i) q = std::max(q, std::fabs(dx(i) / opt.h(i)));
                log << "  rejected: S_new - S = " << (Sn - S) << ", predicted " << -pred << ", lambda "
                    << lambda << ", max |dx/h| " << q << (ok ? "" : " (not admissible)") << "\n";
            }
            if (smallStep) {          // no decrease even for a negligible step: at the minimum
                res.converged = true;
                stop = true;
                break;
            }
            lambda *= nu;
            nu *= 2.0;
        }
        if (stop) break;
        if (!accepted) {
            res.message = "Levenberg-Marquardt: no decrease found (lambda " + std::to_string(lambda) + ").";
            break;
        }
    }
    if (!res.converged && !res.aborted && res.message.empty())
        res.message = "Levenberg-Marquardt: maximum number of iterations reached.";
    // curvature at the final point (unless the last Jacobian already belongs to it)
    if ((res.converged || res.iterations > 0) && !(res.JtJx.n_elem == n && arma::all(res.JtJx == x))) {
        arma::mat Jf;
        if (!(abort && abort()) && jacobian(f, x, r, opt.h, opt.central, Jf, res.evaluations, abort)) {
            ++res.jacobians;
            res.JtJ = Jf.t() * Jf;
            res.JtJx = x;
        }
    }
    res.x = x;
    res.S = S;
    log << "LM end: S = " << S << " after " << res.iterations << " steps, " << res.jacobians
        << " Jacobians, " << res.evaluations << " evaluations"
        << (res.converged ? " (converged)" : "") << "\n";
    res.log = log.str();
    return res;
}

arma::mat covariance(const arma::mat &JtJ, double sigma2)
{
    arma::mat inv;
    if (JtJ.n_rows == 0 || !(sigma2 > 0))
        return arma::mat();
    if (!arma::inv_sympd(inv, JtJ) && !arma::pinv(inv, JtJ))
        return arma::mat();
    return sigma2 * inv;
}

} // namespace lm
