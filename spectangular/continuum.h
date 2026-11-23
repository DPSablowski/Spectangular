#ifndef CONTINUUM_H
#define CONTINUUM_H

// Continuum normalisation of a disentangled component, without GUI.
//
// The spectrum is split into pieces at gaps (excluded ranges, more than gapFactor times the
// median wavelength step). In each piece the continuum is a least-squares cubic B-spline with
// knots every knotA Angstrom through the upper points: points more than `low` sigma below the
// fit (lines) or more than `high` sigma above it (spikes) are left out, iterated until the
// selection no longer changes. sigma is estimated from the points above the fit only (the
// lines lie below it). Pieces shorter than knotA get a straight line, shorter than
// 20 A a constant. The normalised spectrum is flux / continuum.

#include <string>
#include <utility>
#include <vector>

namespace cont {

struct Options {
    double knotA = 80.0;        // knot distance [A]
    double low = 1.5;           // rejection below the fit [sigma]
    double high = 3.0;          // rejection above the fit [sigma]
    double gapFactor = 5.0;     // gap: step > gapFactor * median step
    int maxIter = 30;
};

// Pieces [begin, end) of an increasing wavelength vector, split at gaps
std::vector<std::pair<size_t, size_t>> pieces(const std::vector<double> &w, double gapFactor);

// Continuum for all points (same length as w). false with err if the input is unusable
// (fewer than 3 points, not increasing, non-finite values).
bool fit(const std::vector<double> &w, const std::vector<double> &f, const Options &o,
         std::vector<double> &continuum, std::string &err);

// Aligns the continua of spectra on a common grid to their mean: each spectrum is divided by
// its ratio to the mean spectrum, smoothed by a boxcar of `widthPx` pixels applied twice
// (normalised convolution: pixels with marked[j][i] or a mean near zero are left out and
// bridged). flux[j][i]: spectrum j, pixel i. corrRms: per spectrum the rms of (correction - 1).
// false if the input is unusable (fewer than 2 spectra, different lengths).
bool alignToMean(std::vector<std::vector<double>> &flux, const std::vector<std::vector<char>> &marked,
                 int widthPx, std::vector<double> &corrRms, std::string &err);

} // namespace cont

#endif // CONTINUUM_H
