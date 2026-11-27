#ifndef LINEWIDTH_H
#define LINEWIDTH_H

// Line width of the disentangled component spectra. Absorption lines are defined for each component
// (found in its own spectrum and edited by hand); the mean line profile of the lines of a component in
// that component is fitted with a Gaussian: a wrong radial-velocity amplitude smears the lines of a
// component and broadens them, so the width is a sharp criterion for the amplitude (Stage 1: scan over
// a grid of orbits). Pure numeric module without Qt.

#include <array>
#include <string>
#include <vector>

namespace linewidth {

struct Params {
    int envelopeSize = 2400;        // continuum window [pixels]
    double percentA = 90.0;         // percentile of the continuum envelope, component A
    double percentB = 90.0;         // percentile of the continuum envelope, component B
    double minDepth = 0.12;         // minimum depth of a line in the reference component
    int minSeparation = 120;        // minimum distance of two lines [pixels]
    int smooth = 5;                 // smoothing of the depth for the line search [pixels]
    double lamMin = 4700.0, lamMax = 6500.0;   // wavelength range of the line search
    double dvMax = 200.0, dvStep = 4.0;        // velocity grid of the mean profile [km/s]
    int corrSmooth = 6;             // smoothing for depth ratio and correlation [pixels]
    // wavelength ranges of the depth ratio and the correlation (pairs; empty = whole spectrum)
    std::vector<double> corrRanges = {4750.0, 6750.0, 7000.0, 7500.0};
};

// depth 1 - f / envelope, envelope = percentile of f in a window of `size` pixels (evaluated on a
// coarse grid and interpolated linearly, edges use the nearest window)
std::vector<double> depth(const std::vector<double> &f, double percent, int size);

// running mean of width n (edge values repeated)
std::vector<double> smooth(const std::vector<double> &y, int n);

// lines in the depth profile d (smoothed by `smooth` pixels): local maxima of at least minDepth,
// at least minSeparation pixels from a deeper one, wavelength in [lamMin, lamMax]; returns the
// wavelengths, ascending
std::vector<double> findLines(const std::vector<double> &lambda, const std::vector<double> &d, const Params &p);

// Gaussian b0 + A exp(-(x-x0)^2 / (2 s^2)); parameters {A, x0, s, b0}, errors from the
// covariance scaled by the reduced chi^2 (as scipy curve_fit); false if the fit does not converge
struct Gauss { double a = 0, x0 = 0, s = 0, b0 = 0, ea = 0, ex0 = 0, es = 0, eb0 = 0; double fwhm() const; double efwhm() const; };
bool fitGauss(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &sigma, Gauss &g);

// Result for one component: Gaussian fit of the mean profile of its lines
struct Profile {
    bool ok = false;
    int nLines = 0;
    Gauss fit;
    std::vector<double> prof, err;   // mean depth against velocity and its standard error
};
struct Measure {
    bool ok = false;           // at least one component measured
    Profile a, b;
    std::vector<double> dv;    // velocities of the profiles [km/s]
    double ratio = 0.0;        // slope of the depth of B against the depth of A
    double corr = 0.0;         // correlation of both depths
    std::string error;
};
double gaussValue(const Gauss &g, double x);

// lambda, fA, fB: both component spectra on the same wavelength grid; linesA / linesB: the wavelengths
// of the lines of each component (an empty list: the component is not measured)
Measure measure(const std::vector<double> &lambda, const std::vector<double> &fA, const std::vector<double> &fB,
                const std::vector<double> &linesA, const std::vector<double> &linesB, const Params &p);

// y(x) at the points xq (linear, constant outside; x ascending)
std::vector<double> resample(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &xq);

// Line list file of the scan: comment lines "# key value ..." with the settings (percent_a, percent_b,
// envelope, min_depth, min_separation, range, corr_ranges; missing keys keep the values of p), then the
// sections "# component A" and "# component B", each followed by one wavelength per line.
bool readLineList(const std::string &file, Params &p, std::vector<double> &linesA, std::vector<double> &linesB, std::string &err);
bool writeLineList(const std::string &file, const Params &p, const std::vector<double> &linesA, const std::vector<double> &linesB, std::string &err);

// Orbit grid file: 7 rows (P, e, K_A, K_B, gamma, T_peri, omega as in the orbital element file), one
// column per orbit; lines starting with # are comments
bool readOrbitGrid(const std::string &file, std::vector<std::array<double, 7>> &orbits, std::string &err);

// One orbit of the scan: elements, residuum, measurement
struct ScanRow {
    std::array<double, 7> el{};
    double residuum = 0.0;
    bool ok = false;
    Measure m;
};
struct ScanInfo {
    std::string linesFile;
};
// Summary of a scan, one row per orbit (columns: number, the seven elements, residuum, then for A and B:
// number of lines, depth, v0, FWHM and its error, then depth ratio and correlation; "-" for a component
// without measurement), and the mean profiles with the fits, one block per orbit. readProfiles fills
// the profiles into rows read by readScan (same number of orbits; false with err otherwise).
bool writeScan(const std::string &file, const ScanInfo &info, const std::vector<ScanRow> &rows, std::string &err);
bool readScan(const std::string &file, ScanInfo &info, std::vector<ScanRow> &rows, std::string &err);
bool writeProfiles(const std::string &file, const std::vector<ScanRow> &rows, std::string &err);
bool readProfiles(const std::string &file, std::vector<ScanRow> &rows, std::string &err);

}  // namespace linewidth

#endif
