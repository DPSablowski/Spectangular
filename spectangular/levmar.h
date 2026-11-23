#ifndef LEVMAR_H
#define LEVMAR_H

// Levenberg-Marquardt for nonlinear least squares, without GUI:
//   minimise S(x) = |r(x)|^2
// with a difference Jacobian (central by default). In Spectangular r(x) is the (weighted) residual
// vector after solving for the component spectra at the parameters x (RVs or orbital
// elements): variable projection (Golub & Pereyra 1973) with the component spectra
// eliminated by the linear solve.
//
// Damping after Marquardt (scaled with diag(J^T J)), update of lambda after Nielsen
// (1999): after a successful step lambda *= max(1/3, 1 - (2 rho - 1)^3), otherwise
// lambda *= nu, nu *= 2 (rho = actual / predicted decrease). Curvature model J^T J plus a
// bounded secant correction, line search along the step, stop on the Newton decrement.

#include <armadillo>
#include <functional>
#include <string>
#include <vector>

namespace lm {

// r(x): returns false if x is not admissible (e.g. e outside [0, 1)) or the solve failed.
// trial = true for a step candidate, false for a Jacobian column (no side effects needed).
using ResidualFn = std::function<bool(const arma::vec &x, arma::vec &r, bool trial)>;

struct Options {
    arma::vec h;                // finite-difference step per parameter (required)
    bool central = true;        // central differences (2 evaluations per parameter), see levmar.cpp
    int maxIter = 100;          // Jacobian evaluations
    double tolS = 0.0;          // stop when the model predicts a remaining decrease of S
                                // (Newton decrement g^T H^-1 g) below this
    double relTolS = 1e-12;     // ... or an accepted step decreases S by less than relTolS * S
    double tolX = 1e-3;         // ... or when every |dx_i| < tolX * h_i
    double lambda0 = 1e-3;      // initial damping relative to diag(J^T J)
    int maxRejects = 20;        // consecutive rejected steps before giving up
    // Secant correction of the Gauss-Newton curvature (Dennis, Gay & Welsch 1981, NL2SOL):
    // H = J^T J + T with T ~ sum r_i Hess(r_i) from the changes of J and of the gradient.
    // For disentangling the second-order term is large: the component spectra absorb part
    // of the noise, the true curvature is 1.5-7 x smaller than J^T J, and pure
    // Gauss-Newton converges only linearly.
    bool secant = true;
    double minCurv = 0.1, maxCurv = 1.5;   // bounds of H relative to J^T J (generalised eigenvalues)
    // After a successful step: try the minimum of the parabola through S(0), S'(0), S(step)
    // along the step (one more evaluation), at most maxStretch times the step
    bool lineSearch = true;
    double maxStretch = 4.0;
};

struct Result {
    bool ok = false;            // x is a valid point (at least the start)
    bool converged = false;
    bool aborted = false;
    arma::vec x;
    double S = 0.0;             // |r|^2 at x
    arma::mat JtJ;              // curvature model (J^T J [+ T]) at JtJx; covariance sigma^2 JtJ^-1
    arma::vec JtJx;
    int iterations = 0;         // accepted steps
    int jacobians = 0;
    int evaluations = 0;        // residual evaluations in total
    std::string message;
    std::string log;
};

// progress(it, x, S) after every accepted step
Result minimize(const arma::vec &x0, const ResidualFn &f, const Options &opt,
                const std::function<bool()> &abort = nullptr,
                const std::function<void(int, const arma::vec &, double)> &progress = nullptr);

// Covariance sigma2 (J^T J)^-1 (pseudo-inverse if singular); empty on failure
arma::mat covariance(const arma::mat &JtJ, double sigma2);

} // namespace lm

#endif // LEVMAR_H
