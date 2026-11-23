#ifndef DATAIO_H
#define DATAIO_H

// Reading of text files (spectra, RV tables, times, orbital elements) and
// check of the common wavelength grid. Without GUI.

#include <string>
#include <utility>
#include <vector>

namespace dataio {

// Reads a table: at least minCols numbers per line (further columns are
// ignored). Empty lines and lines starting with '#' are skipped.
// On error: false, and err with file and line number.
bool readTable(const std::string &file, int minCols, std::vector<std::vector<double>> &rows,
               std::string &err);

// Last number of every line (empty lines and lines starting with '#' skipped), e.g. the BC
// column of the file written by Tools -> Barycentric correction (spectrum, time, JD_UTC, BC)
// or a file with one value per line.
bool readLastColumn(const std::string &file, std::vector<double> &values, std::string &err);

// All numbers of a file in reading order (for orbital elements).
bool readNumbers(const std::string &file, std::vector<double> &values, std::string &err);

// Wavelength ranges: per line the first two numbers are from and to (Angstrom); further text
// on the line (e.g. a name) and lines starting with '#' are ignored. from < to required.
// labels (optional): per range the first word of the text after the two numbers, "" if none
// (e.g. "O2" or "H2O": groups of static lines with their own strength).
bool readRanges(const std::string &file, std::vector<std::pair<double, double>> &ranges,
                std::string &err, std::vector<std::string> *labels = nullptr);

// Default list of excluded ranges: strongest telluric bands and strong chromospheric lines
// (file content in the format of readRanges)
extern const char *const kTelluricMask;

// Default file of the ranges of the static (telluric) lines: all telluric O2 and H2O systems
// of the optical, the weak ones included (format of readRanges)
extern const char *const kStaticRanges;

// Marks the pixels whose wavelength (log10 lambda) lies in one of the ranges [from, to].
// Returns the number of marked pixels.
size_t markRanges(const std::vector<double> &log10Lambda,
                  const std::vector<std::pair<double, double>> &ranges, std::vector<char> &marked);

// Checks that all spectra have the same length and the same grid and that the
// grid is equidistant (each pixel within 0.1 pixel of w0 + i*step).
// waves: log10(lambda) per spectrum.
bool checkCommonGrid(const std::vector<std::vector<double>> &waves, std::string &err);

// Result of resampleToLogGrid
struct GridInfo {
    double w0 = 0.0;        // log10(lambda) of the first pixel
    double step = 0.0;      // step in log10(lambda)
    int n = 0;              // pixels per spectrum
    int linearInputs = 0;   // spectra whose wavelengths were given in Angstrom (linear)
};

// Puts all spectra onto one common, equidistant log10(lambda) grid, for spectra on a
// linear wavelength scale (Angstrom) or on individual log grids. Per spectrum the
// wavelength column is taken as Angstrom if all values exceed 100, otherwise as
// log10(lambda). The common grid covers the wavelength range shared by all spectra;
// step: 0 = the finest median sampling among the spectra (no loss of resolution), else
// the given step in log10(lambda). Flux is interpolated with a natural cubic spline
// through the original pixels (wavelengths must increase strictly). On return waves
// holds log10(lambda) of the common grid for every spectrum.
bool resampleToLogGrid(std::vector<std::vector<double>> &waves, std::vector<std::vector<double>> &fluxes,
                       double step, GridInfo &info, std::string &err);

// Natural cubic spline through (x, y) (x strictly increasing), evaluated at xi (within
// [x.front(), x.back()]).
std::vector<double> splineInterpolate(const std::vector<double> &x, const std::vector<double> &y,
                                      const std::vector<double> &xi);

} // namespace dataio

#endif // DATAIO_H
