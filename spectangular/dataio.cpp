#include "dataio.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

namespace dataio {

namespace {

bool skipLine(const std::string &line)
{
    for (char c : line) {
        if (c == '#')
            return true;
        if (c != ' ' && c != '\t' && c != '\r')
            return false;
    }
    return true;   // empty
}

} // namespace

bool readLastColumn(const std::string &file, std::vector<double> &values, std::string &err)
{
    values.clear();
    std::ifstream in(file.c_str());
    if (!in) {
        err = "File " + file + " cannot be opened.";
        return false;
    }
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        if (skipLine(line))
            continue;
        std::istringstream ls(line);
        double x, last = 0.0;
        int n = 0;
        while (ls >> x) { last = x; ++n; }
        if (n == 0 || !std::isfinite(last)) {
            err = "File " + file + ", line " + std::to_string(lineNo) + ": no number.";
            return false;
        }
        values.push_back(last);
    }
    if (values.empty()) {
        err = "File " + file + " contains no data.";
        return false;
    }
    return true;
}

bool readTable(const std::string &file, int minCols, std::vector<std::vector<double>> &rows,
               std::string &err)
{
    rows.clear();
    std::ifstream in(file.c_str());
    if (!in) {
        err = "File " + file + " cannot be opened.";
        return false;
    }
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        if (skipLine(line))
            continue;
        std::istringstream ls(line);
        std::vector<double> r(minCols);
        for (int c = 0; c < minCols; ++c) {
            if (!(ls >> r[c])) {
                err = "File " + file + ", line " + std::to_string(lineNo) + ": expected "
                      + std::to_string(minCols) + " number(s).";
                return false;
            }
        }
        rows.push_back(r);
    }
    if (rows.empty()) {
        err = "File " + file + " contains no data.";
        return false;
    }
    return true;
}

bool readNumbers(const std::string &file, std::vector<double> &values, std::string &err)
{
    values.clear();
    std::ifstream in(file.c_str());
    if (!in) {
        err = "File " + file + " cannot be opened.";
        return false;
    }
    std::string tok;
    while (in >> tok) {
        std::istringstream ts(tok);
        double v;
        if (!(ts >> v)) {
            err = "File " + file + ": '" + tok + "' is not a number.";
            return false;
        }
        values.push_back(v);
    }
    return true;
}

bool checkCommonGrid(const std::vector<std::vector<double>> &waves, std::string &err)
{
    if (waves.empty() || waves[0].size() < 10) {
        err = "Spectra need at least 10 pixels.";
        return false;
    }
    const std::vector<double> &w0 = waves[0];
    const size_t n = w0.size();
    // Wavelengths in Angstrom: a linear grid can be equidistant and identical for all
    // spectra and would pass the checks below, but it is not a log10(lambda) grid
    // (log10 of optical wavelengths is about 3-5).
    for (const auto &w : waves) {
        if (!w.empty() && w.front() > 100.0) {
            err = "Wavelengths are in Angstrom on a linear scale, not log10(lambda).";
            return false;
        }
    }
    const double step = (w0[n - 1] - w0[0]) / double(n - 1);
    if (!(step > 0)) {
        err = "Wavelengths of the first spectrum are not increasing.";
        return false;
    }
    // Each pixel must lie at its assumed position w0 + i*step (within 0.1 pixel).
    // The position is checked, not the individual step: rounding (e.g. float32 in
    // FITS) makes steps jump by a few percent without distorting the positions;
    // a linear lambda grid, by contrast, deviates systematically.
    for (size_t i = 1; i < n; ++i) {
        if (std::fabs(w0[i] - (w0[0] + double(i) * step)) > 0.1 * step) {
            err = "Spectrum grid is not equidistant in log10(lambda) (pixel "
                  + std::to_string(i) + " is off by more than 0.1 pixel).";
            return false;
        }
    }
    for (size_t a = 1; a < waves.size(); ++a) {
        if (waves[a].size() != n) {
            err = "Spectrum " + std::to_string(a) + " has " + std::to_string(waves[a].size())
                  + " pixels, the first one " + std::to_string(n) + ".";
            return false;
        }
        if (std::fabs(waves[a][0] - w0[0]) > 0.01 * step
            || std::fabs(waves[a][n - 1] - w0[n - 1]) > 0.01 * step) {
            err = "Spectrum " + std::to_string(a) + " is on a different wavelength grid.";
            return false;
        }
        // all pixels, not only the ends (a duplicated or missing wavelength inside)
        for (size_t i = 1; i + 1 < n; ++i) {
            if (std::fabs(waves[a][i] - w0[i]) > 0.1 * step) {
                err = "Spectrum " + std::to_string(a) + " is on a different wavelength grid (pixel "
                      + std::to_string(i) + ").";
                return false;
            }
        }
    }
    return true;
}

std::vector<double> splineInterpolate(const std::vector<double> &x, const std::vector<double> &y,
                                      const std::vector<double> &xi)
{
    const size_t n = x.size();
    std::vector<double> out(xi.size(), 0.0);
    if (n < 2)
        return out;
    // second derivatives M_i, natural boundary (M_0 = M_n-1 = 0), tridiagonal system
    std::vector<double> m(n, 0.0), c(n, 0.0), d(n, 0.0);
    for (size_t i = 1; i + 1 < n; ++i) {
        const double h0 = x[i] - x[i - 1], h1 = x[i + 1] - x[i];
        const double a = h0 / 6.0, b = (h0 + h1) / 3.0, cc = h1 / 6.0;
        const double r = (y[i + 1] - y[i]) / h1 - (y[i] - y[i - 1]) / h0;
        // forward elimination (Thomas algorithm)
        const double den = b - a * c[i - 1];
        c[i] = cc / den;
        d[i] = (r - a * d[i - 1]) / den;
    }
    for (size_t i = n - 2; i >= 1; --i) {
        m[i] = d[i] - c[i] * m[i + 1];
        if (i == 1) break;
    }
    size_t k = 0;
    for (size_t j = 0; j < xi.size(); ++j) {
        const double t = xi[j];
        while (k + 2 < n && x[k + 1] < t) ++k;
        while (k > 0 && x[k] > t) --k;
        const double h = x[k + 1] - x[k];
        const double A = (x[k + 1] - t) / h, B = (t - x[k]) / h;
        out[j] = A * y[k] + B * y[k + 1]
                 + ((A * A * A - A) * m[k] + (B * B * B - B) * m[k + 1]) * h * h / 6.0;
    }
    return out;
}

bool resampleToLogGrid(std::vector<std::vector<double>> &waves, std::vector<std::vector<double>> &fluxes,
                       double step, GridInfo &info, std::string &err)
{
    info = GridInfo();
    if (waves.empty() || waves.size() != fluxes.size()) {
        err = "No spectra to resample.";
        return false;
    }
    double lo = -1e300, hi = 1e300;
    std::vector<double> steps;
    for (size_t a = 0; a < waves.size(); ++a) {
        std::vector<double> &w = waves[a];
        if (w.size() < 10 || w.size() != fluxes[a].size()) {
            err = "Spectrum " + std::to_string(a) + " has fewer than 10 pixels.";
            return false;
        }
        const bool angstrom = *std::min_element(w.begin(), w.end()) > 100.0;
        if (angstrom) {
            for (double &v : w) v = std::log10(v);
            ++info.linearInputs;
        }
        std::vector<double> dw;
        for (size_t i = 1; i < w.size(); ++i) {
            if (!(w[i] > w[i - 1])) {
                err = "Spectrum " + std::to_string(a) + ": wavelengths do not increase at pixel "
                      + std::to_string(i) + ".";
                return false;
            }
            dw.push_back(w[i] - w[i - 1]);
        }
        std::nth_element(dw.begin(), dw.begin() + dw.size() / 2, dw.end());
        steps.push_back(dw[dw.size() / 2]);
        lo = std::max(lo, w.front());
        hi = std::min(hi, w.back());
    }
    if (!(step > 0))
        step = *std::min_element(steps.begin(), steps.end());
    if (!(hi > lo)) {
        err = "The spectra have no common wavelength range.";
        return false;
    }
    const int n = int(std::floor((hi - lo) / step * (1.0 + 1e-12))) + 1;
    if (n < 10) {
        err = "The common wavelength range has fewer than 10 pixels.";
        return false;
    }
    std::vector<double> grid(n);
    for (int i = 0; i < n; ++i)
        grid[i] = lo + double(i) * step;
    grid[n - 1] = std::min(grid[n - 1], hi);   // rounding
    for (size_t a = 0; a < waves.size(); ++a) {
        fluxes[a] = splineInterpolate(waves[a], fluxes[a], grid);
        waves[a] = grid;
    }
    info.w0 = lo;
    info.step = step;
    info.n = n;
    return true;
}


bool readRanges(const std::string &file, std::vector<std::pair<double, double>> &ranges,
                std::string &err, std::vector<std::string> *labels)
{
    ranges.clear();
    if (labels) labels->clear();
    std::ifstream in(file.c_str());
    if (!in) {
        err = "Cannot open " + file + ".";
        return false;
    }
    std::string line;
    int n = 0;
    while (std::getline(in, line)) {
        ++n;
        const size_t p = line.find_first_not_of(" \t\r");
        if (p == std::string::npos || line[p] == '#') continue;
        std::istringstream is(line);
        double a, b;
        if (!(is >> a >> b) || !std::isfinite(a) || !std::isfinite(b)) {
            err = file + ", line " + std::to_string(n) + ": two numbers (from, to in Angstrom) expected.";
            return false;
        }
        if (!(a < b) || a <= 0) {
            err = file + ", line " + std::to_string(n) + ": range must have 0 < from < to.";
            return false;
        }
        ranges.emplace_back(a, b);
        if (labels) {
            std::string word;
            is >> word;
            labels->push_back(word);
        }
    }
    return true;
}

size_t markRanges(const std::vector<double> &log10Lambda,
                  const std::vector<std::pair<double, double>> &ranges, std::vector<char> &marked)
{
    marked.assign(log10Lambda.size(), 0);
    size_t n = 0;
    for (size_t i = 0; i < log10Lambda.size(); ++i) {
        const double lam = std::pow(10.0, log10Lambda[i]);
        for (const auto &r : ranges) {
            if (lam >= r.first && lam <= r.second) {
                marked[i] = 1;
                ++n;
                break;
            }
        }
    }
    return n;
}

// Default list of excluded ranges: the strongest telluric bands and the strong chromospheric
// lines (both vary from spectrum to spectrum in a way two moving components cannot describe).
// Air wavelengths. Telluric bands widened by about 1 A for barycentrically corrected spectra
// (|BC| <= 30 km/s: at most 0.9 A at 8800 A). Chromospheric lines: core half-width (1-3 A)
// plus the shift for stellar RVs up to +-100 km/s (lambda * 100 / c), since they move with
// the stars in the frame of the input spectra.
const char *const kTelluricMask =
    "# Wavelength ranges excluded from the solution and the residuum (Spectangular).\n"
    "# Per line: from to [A], further text is ignored; '#' starts a comment line.\n"
    "# Air wavelengths, frame of the input spectra.\n"
    "#\n"
    "# Strongest telluric bands, widened by about 1 A for barycentrically corrected spectra\n"
    "# (|BC| <= 30 km/s). Weaker H2O bands, if needed: 5880-5990 and 6470-6600 A.\n"
    "6274  6331   O2 gamma band\n"
    "6865  6936   O2 B band\n"
    "7159  7341   H2O\n"
    "7593  7701   O2 A band\n"
    "8119  8351   H2O\n"
    "8929  9851   H2O (strongest; beyond most optical spectra)\n"
    "#\n"
    "# Strong chromospheric lines (variable cores): core half-width 1-3 A plus the shift for\n"
    "# stellar RVs up to +-100 km/s (lambda * 100 km/s / c, 1.3 A at 3934 A, 2.9 A at 8662 A).\n"
    "# For other RV ranges: half-width = core + lambda * |v|max / c. Only the cores are excluded,\n"
    "# the broad Balmer wings stay in. Optional: He I D3 5875.6 (5871.7 5879.6).\n"
    "3930.8  3936.5   Ca II K 3933.66\n"
    "3965.1  3973.4   Ca II H 3968.47 + H epsilon 3970.07\n"
    "4098.4  4105.1   H delta 4101.74\n"
    "4337.0  4343.9   H gamma 4340.46\n"
    "4857.7  4864.9   H beta 4861.32\n"
    "5164.6  5175.4   Mg I b4 5167.32 + b2 5172.68\n"
    "5180.9  5186.3   Mg I b1 5183.60\n"
    "5887.0  5898.9   Na I D2 5889.95 + D1 5895.92 (also interstellar and telluric)\n"
    "6557.6  6568.0   H alpha 6562.80\n"
    "8493.7  8502.4   Ca II 8498.02 (IR triplet)\n"
    "8537.7  8546.4   Ca II 8542.09 (IR triplet)\n"
    "8657.8  8666.5   Ca II 8662.14 (IR triplet)\n";

const char *const kLineWindows =
    "# Windows of the line statistics (Spectangular, Output tab, Analysis of the solution):\n"
    "# from to [A, frame of the input spectra] label. Wide enough for the RVs (about +-150 km/s);\n"
    "# edit or add lines as needed.\n"
    "3929.0 3938.0 CaII_K\n"
    "3964.0 3973.0 CaII_H\n"
    "4095.0 4108.0 H_delta\n"
    "4334.0 4347.0 H_gamma\n"
    "4467.0 4476.5 HeI_4471\n"
    "4477.5 4484.5 MgII_4481\n"
    "4681.5 4690.5 HeII_4686\n"
    "4854.5 4868.0 H_beta\n"
    "5164.0 5187.0 Mg_b\n"
    "5871.0 5881.0 HeI_5876\n"
    "5885.0 5901.0 Na_D\n"
    "6555.5 6570.0 H_alpha\n"
    "6674.0 6683.0 HeI_6678\n"
    "8494.0 8503.5 CaII_8498\n"
    "8537.5 8547.0 CaII_8542\n"
    "8657.0 8667.0 CaII_8662\n";

const char *const kStaticRanges =
    "# Wavelength ranges of the static lines (Spectangular, Data tab: static lines only in ranges).\n"
    "# Per line: from to [A], further text is ignored; '#' starts a comment line.\n"
    "# Air wavelengths in the frame of the static lines, i.e. the observatory frame (also for\n"
    "# barycentrically corrected spectra; no widening for BC needed). The static spectrum is\n"
    "# solved only here; outside it is zero.\n"
    "# The first word after the numbers names the group (here O2 and H2O): with \"fit per\n"
    "# spectrum\" each group gets its own strength per spectrum (O2 follows the airmass, H2O\n"
    "# also the water vapour). Ranges without a word form one group.\n"
    "#\n"
    "# Telluric O2 and H2O absorption, strong and weak systems. Limits of the strong bands after\n"
    "# Buton et al. 2013 (A&A 549, A8, Table 1), widened by about 2 A; weak bands generously.\n"
    "# Check against your data: Data tab, \"Detect\" writes the ranges found in the static spectrum\n"
    "# of the last Solve (e.g. also sky emission lines such as [O I] 5577, 6300, 6364).\n"
    "5770  5810   O2 delta band (very weak)\n"
    "5880  6000   H2O (weak; around Na D)\n"
    "6268  6334   O2 gamma band (+ weak H2O)\n"
    "6450  6610   H2O (weak; around H alpha)\n"
    "6860  6967   O2 B band\n"
    "6967  7141   H2O (weak part of the 0.72 micron band)\n"
    "7141  7400   H2O (strong)\n"
    "7584  7705   O2 A band\n"
    "7840  8050   H2O (weak)\n"
    "8082  8423   H2O (strong)\n"
    "8914  9932   H2O (strongest)\n";

} // namespace dataio
