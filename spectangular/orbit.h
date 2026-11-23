#ifndef ORBIT_H
#define ORBIT_H

// Orbit model of a binary star: Kepler equation and radial velocities.
// Pure functions without GUI and without global variables.
//
// Elements in the program's order (orbele):
//   P [d], e, K_A [km/s], K_B [km/s], gamma [km/s], T_peri [d], omega_A [rad]

namespace orbit {

// Residuum for invalid orbital elements in the simplex (instead of NaN).
constexpr double kInvalidPenalty = 1e30;

// P > 0 and 0 <= e < 1.
bool elementsValid(double period, double ecc);

// Solves E - e sin E = M (Newton). M arbitrary, result in [-pi, pi].
// converged (optional) becomes false if the tolerance was not reached.
double eccentricAnomaly(double meanAnomaly, double ecc, bool *converged = nullptr);

// True anomaly at time t.
double trueAnomaly(double t, double period, double ecc, double tPeri, bool *converged = nullptr);

// RV of the component with amplitude k and argument of periastron omega:
//   gamma + k (cos(nu + omega) + e cos omega)
double radialVelocity(double t, double period, double ecc, double k, double gamma,
                      double tPeri, double omega, bool *converged = nullptr);

} // namespace orbit

#endif // ORBIT_H
