#include "orbit.h"

#include <cmath>

namespace orbit {

bool elementsValid(double period, double ecc)
{
    return period > 0.0 && ecc >= 0.0 && ecc < 1.0 && std::isfinite(period) && std::isfinite(ecc);
}

double eccentricAnomaly(double meanAnomaly, double ecc, bool *converged)
{
    // Reduce M to [-pi, pi]; the solution then lies there as well.
    double M = std::remainder(meanAnomaly, 2.0 * M_PI);
    if (ecc == 0.0) {
        if (converged)
            *converged = true;
        return M;
    }
    // Starting value after Danby: robust even for e close to 1.
    double E = M + (M >= 0 ? 0.85 : -0.85) * ecc;
    bool ok = false;
    for (int i = 0; i < 50; ++i) {
        double f = E - ecc * std::sin(E) - M;
        double d = f / (1.0 - ecc * std::cos(E));
        E -= d;
        if (std::fabs(d) < 1e-14) {
            ok = true;
            break;
        }
    }
    if (converged)
        *converged = ok;
    return E;
}

double trueAnomaly(double t, double period, double ecc, double tPeri, bool *converged)
{
    double M = 2.0 * M_PI * (t - tPeri) / period;
    double E = eccentricAnomaly(M, ecc, converged);
    return 2.0 * std::atan2(std::sqrt(1.0 + ecc) * std::sin(0.5 * E),
                            std::sqrt(1.0 - ecc) * std::cos(0.5 * E));
}

double radialVelocity(double t, double period, double ecc, double k, double gamma,
                      double tPeri, double omega, bool *converged)
{
    double nu = trueAnomaly(t, period, ecc, tPeri, converged);
    return gamma + k * (std::cos(nu + omega) + ecc * std::cos(omega));
}

} // namespace orbit
