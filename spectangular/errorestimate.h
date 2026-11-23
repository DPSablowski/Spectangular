#ifndef ERRORESTIMATE_H
#define ERRORESTIMATE_H

// Error estimate from the final simplex after Nelder & Mead (1965), appendix.
// Without GUI.
//
// Around the best point P0, S (sum of squared errors) is approximated quadratically:
//   S(P0 + Q x) = S0 + 2 a^T x + x^T B x,   Q = [P1-P0 ... Pn-P0]
//   a_i  = 2 S_0i - (S_i + 3 S_0) / 2
//   b_ii = 2 (S_i + S_0 - 2 S_0i)
//   b_ij = 2 (S_ij + S_0 - S_0i - S_0j)
// with S_0i, S_ij at the edge midpoints. Covariance of the parameters for
// S = sigma^2 chi^2:  C = sigma^2 Q B^-1 Q^T.

#include <armadillo>
#include <cmath>
#include <functional>
#include <string>
#include <vector>

namespace errest {

struct Result {
    bool ok = false;
    bool aborted = false;
    std::string message;
    int best = 0;            // index of the best simplex point
    arma::mat Q, B;
    arma::vec a;
    arma::rowvec minimum;    // minimum of the quadratic model
    arma::mat covUnit;       // Q B^-1 Q^T (sigma^2 = 1)
    int evaluations = 0;
};

// P: (n+1) x n simplex, S: values of S at the vertices, f: S at an arbitrary point.
Result quadraticFit(const arma::mat &P, const std::vector<double> &S,
                    const std::function<double(const arma::rowvec &)> &f,
                    const std::function<bool()> &abort);

// New simplex around the best point x0 for the error estimate.
//
// After full convergence the final simplex has shrunk to a point; the
// differences of S are then within rounding noise and Q is singular. As
// suggested by Nelder & Mead (1965, appendix), a new simplex x0 + h_i e_i is
// therefore spanned around x0. The step size h_i is adjusted per parameter so
// that S at the vertex is larger than S0 by about target (within a factor of 2);
// a sensible choice is target = sigma^2, i.e. about one standard deviation.
// The curvature is determined symmetrically from S(x0+h) and S(x0-h); if one
// side lies outside the allowed range (S not finite or >= invalidAbove),
// only the other one is used.
struct ProbeResult {
    bool ok = false;
    bool aborted = false;
    std::string message;
    arma::mat P;                 // (n+1) x n, row 0 = x0
    std::vector<double> S;       // S at the vertices
    arma::vec steps;             // h_i (with the sign of the side used)
    std::vector<int> undetermined;   // parameters without measurable influence on S
    int evaluations = 0;
};

ProbeResult probeSimplex(const arma::rowvec &x0, double S0, const arma::vec &h0, double target,
                         const std::function<double(const arma::rowvec &)> &f,
                         const std::function<bool()> &abort,
                         double invalidAbove = 1e29, int maxIter = 40);

// Second pass: simplex along the principal axes of the quadratic model from
// a first quadraticFit (directions Q u_j, u_j eigenvectors of B), length per
// direction again such that S rises by about target. With strong correlations
// the axis-parallel steps of the first pass are much shorter along the valley
// than the uncertainty; the curvature in the valley (it determines the marginal
// errors) is only measured on the right scale here. steps: t_j (multiples of the direction).
ProbeResult probePrincipal(const arma::rowvec &x0, double S0, const Result &first, double target,
                           const std::function<double(const arma::rowvec &)> &f,
                           const std::function<bool()> &abort,
                           double invalidAbove = 1e29, int maxIter = 40);

} // namespace errest

#endif // ERRORESTIMATE_H
