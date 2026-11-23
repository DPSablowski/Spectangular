#ifndef RVFIT_H
#define RVFIT_H

// Fit of a Keplerian orbit to radial velocities (SB1 or SB2), without GUI.
// Levenberg-Marquardt (levmar) on the free elements of
//   P [d], e, K_A [km/s], K_B [km/s], gamma [km/s], T_peri [d], omega_A [rad]
// (the program's order, omega_B = omega_A + pi). Residuals (RV_obs - RV_model) / sigma.
// Without given errors the fit is done twice: with equal weights, then with
// sigma_A = RMS_A and sigma_B = RMS_B of the first fit (so that the more precise component
// carries more weight). Standard errors: covariance s^2 (J^T J)^-1, s^2 = chi^2 / dof,
// i.e. scaled to the scatter (conservative also when the given errors are too small).

#include <armadillo>
#include <string>
#include <vector>

namespace rvfit {

struct Data {
    std::vector<double> t, rvA, rvB;     // rvB empty: SB1
    std::vector<double> sigA, sigB;      // optional errors (km/s); empty: unknown
    // SB2: fit an offset of the RVs of B (own zero point). RVs from disentangling without
    // static lines have an arbitrary zero point per component.
    bool offsetB = false;
    bool sb2() const { return !rvB.empty(); }
};

struct Result {
    bool ok = false;
    std::string message;
    double el[7] = {0, 0, 0, 0, 0, 0, 0};
    double err[7] = {0, 0, 0, 0, 0, 0, 0};   // 0 for fixed elements
    std::vector<int> freeIdx;                 // indices of the free elements (7: offset of B)
    double offB = 0.0, errOffB = 0.0;         // offset of the RVs of B (Data::offsetB)
    arma::mat cov;                            // covariance of the free elements
    double rmsA = 0.0, rmsB = 0.0;            // km/s
    double chi2 = 0.0;                        // with the sigmas used
    int dof = 0;
    int iterations = 0;
    std::vector<double> resA, resB;           // O - C per observation
    std::vector<double> sigUsedA, sigUsedB;
};

// Reads a table: t RV_A [RV_B [sigma_A sigma_B]] (sb2 = true: at least three columns).
bool readTable(const std::string &file, bool sb2, Data &d, std::string &err);

// Model RVs of A and B at time t.
void model(const double el[7], double t, double &vA, double &vB);

// T_peri of the result is the periastron passage nearest to the mean time of the data
// (unless T_peri is fixed); add multiples of P for another epoch.
Result fit(const Data &d, const double start[7], const bool fixed[7], int maxIter = 200);

// Orbital phase from periastron, in [0, 1).
double phase(double t, double period, double tPeri);

} // namespace rvfit

#endif // RVFIT_H
