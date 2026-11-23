#ifndef TEMPLATEFIT_H
#define TEMPLATEFIT_H

// Flux ratio from synthetic spectra, without GUI.
//
// The disentangled components x_A, x_B were computed with assumed light fractions
// w_A(lambda) = r0/(1 + r0), w_B = 1/(1 + r0), r0 = k0 s(lambda) (field "Flux ratio" and its
// lambda dependence). With the true light fractions l_A = r/(1 + r), l_B = 1/(1 + r),
// r = k s(lambda), and the intrinsic normalised spectra S_A, S_B, the line depths scale as
//   x_A = c_A - (l_A / w_A) (1 - S_A),
//   x_B = c_B - (l_B / w_B) (1 - S_B),   w_A c_A + w_B c_B = 1,
// where the continuum levels c_A, c_B are tied by the offset null direction of the
// disentangling. c_A(lambda) is a low-order polynomial: slowly varying functions are nearly
// undetermined by the disentangling, so the continua of the components can be wavy over a
// wide range (and the normalisation of the observations adds to it). With synthetic spectra
// T_A, T_B in place of S_A, S_B (broadened, shifted) the flux ratio k at lambda_ref and c_A
// follow from a least-squares fit to both components together (k nonlinear by a
// one-dimensional minimisation, c_A linear). The renormalised
// components are
//   S_A = 1 - (c_A - x_A) w_A / l_A,   S_B = 1 - (c_B - x_B) w_B / l_B.
//
// As a check, each component is also fitted alone with a free light fraction
// (x = c(lambda) - (l / w)(1 - T)); the two light fractions should add up to one. A clearly different
// sum means that the line strengths of the synthetic spectra do not match (temperature,
// gravity, metallicity, broadening), and the flux ratio is then not reliable.

#include <string>
#include <vector>

#include "fluxratio.h"

namespace tfit {

struct Spectrum {
    std::vector<double> lam;   // Angstrom, increasing
    std::vector<double> f;
};

// Reads two columns (wavelength, flux); wavelengths <= 100 are taken as log10(lambda).
bool readSpectrum(const std::string &file, Spectrum &s, std::string &err);

// Broadening of a synthetic spectrum: rotation (vsini, limb darkening eps) and instrumental
// profile (Gaussian with FWHM = c / R), on a grid uniform in ln(lambda). 0 = none.
Spectrum broaden(const Spectrum &t, double vsini, double resolvingPower, double eps = 0.6);

struct Options {
    double l1 = 0.0, l2 = 0.0;        // fit range [A]; 0: all
    bool fitShift[2] = {true, true};  // fit the RV shift of the synthetic spectrum A / B
    double shift[2] = {0.0, 0.0};     // km/s: value if not fitted, start otherwise
    double vmax = 100.0;              // km/s: search range of the shift (+-)
    double vsini[2] = {0.0, 0.0};     // km/s
    double resolvingPower = 0.0;      // lambda / dlambda; 0: no instrumental broadening
    int edgeSkip = 10;                // pixels left out at both ends of each component
    double clip = 5.0;                // pixels with |O - C| > clip * RMS are left out
    // degree of the continuum c_A(lambda) (Legendre polynomial over the fit range; c_B follows
    // from w_A c_A + w_B c_B = 1). Slowly varying functions are nearly undetermined by the
    // disentangling (and by the normalisation); 0: constant.
    int contDegree = 2;
    int jackBlocks = 10;              // blocks of the jackknife error of k (< 3: none)
};

struct Result {
    bool ok = false;
    std::string message;
    double k = 0.0, errK = 0.0;       // F_A / F_B at lambda_ref; formal error (chi2 curvature)
    double errKJack = 0.0;            // error from a block jackknife over the wavelength (0: none)
    double lambdaRef = 0.0;
    double lA = 0.0, lB = 0.0;        // light fractions at lambda_ref
    double cA = 0.0, errCA = 0.0;     // continuum level of x_A in the model at lambda_ref
    double shift[2] = {0.0, 0.0};     // km/s
    double rms[2] = {0.0, 0.0};       // RMS of O - C of the components
    int npix[2] = {0, 0};             // pixels used
    double lInd[2] = {0.0, 0.0};      // light fractions of the separate fits (check)
    double chi2 = 0.0;
    int dof = 0;
    std::vector<double> modelA, modelB;   // fitted model on the component grids (NaN: none)
    std::vector<double> renA, renB;       // renormalised components
    std::vector<char> usedA, usedB;       // pixels used in the fit
};

// k0: ratio at lambda_ref used in the disentangling; m: its lambda dependence (the same shape
// s(lambda) is assumed for the true ratio). m.lambdaRef <= 0: centre of the components.
Result fit(const Spectrum &A, const Spectrum &B, const Spectrum &TA, const Spectrum &TB,
           const fr::Model &m, double k0, const Options &opt);

} // namespace tfit

#endif // TEMPLATEFIT_H
