#ifndef FLUXRATIO_H
#define FLUXRATIO_H

// Wavelength-dependent flux ratio of the components, without GUI.
//
// The flux ratio of spectrum j at wavelength lambda is
//   r_j(lambda) = k_j * s(lambda),   s(lambda_ref) = 1,
// where k_j is the ratio F_A/F_B at the reference wavelength (field "Flux ratio" or, per
// spectrum, edit.dat / the flux-ratio optimisation) and s the shape:
//   constant: s = 1 (as before);
//   Planck:   s = P(lambda) / P(lambda_ref),  P = B(lambda, T_A) / B(lambda, T_B);
//   table:    s = t(lambda) / t(lambda_ref),  t linearly interpolated from a table
//             (lambda [A], ratio), constant beyond its ends.
// The light fractions (weights in the model) of spectrum j are
//   w_A = W_j r_j / (1 + r_j),  w_B = W_j / (1 + r_j),  W_j = sum of the weights of A and B
// given per spectrum (1 for normalised spectra).
//
// Eclipses: if the fraction eps_j of the disc of A is covered, F_A,j = (1 - eps_j) F_A, so
// k_j = (1 - eps_j) k and the shape s stays the same (without limb darkening).

#include <string>
#include <vector>

namespace fr {

enum class Mode { Constant = 0, Planck = 1, Table = 2 };

struct Model {
    Mode mode = Mode::Constant;
    double tA = 10000.0, tB = 10000.0;     // K (Planck)
    double lambdaRef = 0.0;                // A; <= 0: centre of the data range
    std::vector<double> tabLambda, tabRatio;   // A, F_A/F_B (table, increasing lambda)
};

// Planck function B_lambda up to a constant factor; lambda in A, t in K.
double planck(double lambdaA, double t);

// Reads a table with two columns (lambda [A], ratio), comments '#' and blank lines allowed.
// Wavelengths must increase, ratios must be positive, at least two rows.
bool loadTable(const std::string &file, Model &m, std::string &err);

// Shape s(lambda) relative to lambdaRef (must be > 0 here).
double shape(const Model &m, double lambdaA, double lambdaRef);

// s for every wavelength of a grid. lambdaRef <= 0 in the model: geometric centre of the
// grid. Returns false with a message for invalid parameters.
bool shapeOnGrid(const Model &m, const std::vector<double> &lambdaA, std::vector<double> &s,
                 double &lambdaRefUsed, std::string &err);

// Weights of A and B for ratio k*s and total W.
inline void weights(double k, double s, double W, double &wA, double &wB)
{
    const double r = k * s;
    wA = W * r / (1.0 + r);
    wB = W / (1.0 + r);
}

// Continuum correction of the disentangled components with the flux-ratio model.
// The model of the observations is unchanged by A + dA, B + dB if w_A dA + w_B dB = 0.
// Given the continuum cA(lambda) of component A (e.g. a spline through continuum points),
// dA = 1 - cA and dB = -(w_A/w_B) dA = -r(lambda) (1 - cA) put both components on the
// continuum 1 (for normalised input). lamA/lamB: wavelengths of the two files; cA is given
// at lamA and interpolated (linearly, constant beyond the ends) to lamB. ratioAtB: k s(lambda)
// at lamB, the ratio out of eclipse. A and B are changed in place.
void normaliseToContinuum(const std::vector<double> &lamA, std::vector<double> &A,
                          const std::vector<double> &cA,
                          const std::vector<double> &lamB, std::vector<double> &B,
                          const std::vector<double> &ratioAtB);

// Linear interpolation of (x, y) at xi, constant beyond the ends; x increasing.
double interpLinear(const std::vector<double> &x, const std::vector<double> &y, double xi);

} // namespace fr

#endif // FLUXRATIO_H
