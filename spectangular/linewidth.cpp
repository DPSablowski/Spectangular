#include "linewidth.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <sstream>

namespace linewidth {

namespace {
const double kC = 299792.458;

double percentileOf(std::vector<double> &v, double percent)
{
    // linear interpolation between order statistics (numpy default)
    double pos = percent / 100.0 * (v.size() - 1);
    size_t lo = (size_t)std::floor(pos), hi = std::min(v.size() - 1, lo + 1);
    std::nth_element(v.begin(), v.begin() + lo, v.end());
    double a = v[lo];
    double b = a;
    if (hi != lo) b = *std::min_element(v.begin() + hi, v.end());
    return a + (pos - lo) * (b - a);
}

// linear interpolation of y(x) with ascending x, constant outside
double interp(const std::vector<double> &x, const std::vector<double> &y, double v)
{
    if (v <= x.front()) return y.front();
    if (v >= x.back()) return y.back();
    size_t i = std::upper_bound(x.begin(), x.end(), v) - x.begin();
    double t = (v - x[i - 1]) / (x[i] - x[i - 1]);
    return y[i - 1] + t * (y[i] - y[i - 1]);
}
}  // namespace

std::vector<double> depth(const std::vector<double> &f, double percent, int size)
{
    int n = (int)f.size();
    std::vector<double> d(n, 0.0);
    if (n == 0) return d;
    size = std::max(3, std::min(size, n));
    int stride = std::max(1, size / 20);
    std::vector<double> cx, cy, buf;
    for (int i = 0; ; i += stride) {
        if (i > n - 1) i = n - 1;
        int lo = std::max(0, std::min(i - size / 2, n - size));
        buf.assign(f.begin() + lo, f.begin() + lo + size);
        cx.push_back(i);
        cy.push_back(percentileOf(buf, percent));
        if (i == n - 1) break;
    }
    for (int i = 0; i < n; i++) {
        double env = interp(cx, cy, i);
        d[i] = env != 0.0 ? 1.0 - f[i] / env : 0.0;
    }
    return d;
}

std::vector<double> smooth(const std::vector<double> &y, int w)
{
    int n = (int)y.size();
    if (w <= 1 || n == 0) return y;
    std::vector<double> out(n);
    int left = w / 2, right = w - 1 - left;   // as scipy uniform_filter1d
    for (int i = 0; i < n; i++) {
        double s = 0;
        for (int k = i - left; k <= i + right; k++) s += y[std::max(0, std::min(n - 1, k))];
        out[i] = s / w;
    }
    return out;
}

std::vector<double> findLines(const std::vector<double> &lambda, const std::vector<double> &d, const Params &p)
{
    std::vector<double> s = smooth(d, p.smooth);
    int n = (int)s.size();
    std::vector<int> cand;
    for (int i = 1; i < n - 1; i++) {
        if (s[i] < p.minDepth) continue;
        // local maximum with plateau handling: left strictly lower, right not higher
        int j = i;
        while (j + 1 < n && s[j + 1] == s[i]) j++;
        if (s[i - 1] < s[i] && j + 1 < n && s[j + 1] < s[i]) cand.push_back((i + j) / 2);
        i = j;
    }
    std::sort(cand.begin(), cand.end(), [&](int a, int b) { return s[a] > s[b]; });
    std::vector<int> kept;
    for (int c : cand) {
        bool ok = true;
        for (int k : kept) if (std::abs(k - c) < p.minSeparation) { ok = false; break; }
        if (ok) kept.push_back(c);
    }
    std::vector<double> out;
    for (int c : kept)
        if (lambda[c] > p.lamMin && lambda[c] < p.lamMax) out.push_back(lambda[c]);
    std::sort(out.begin(), out.end());
    return out;
}

double gaussValue(const Gauss &g, double x)
{
    double u = (x - g.x0) / g.s;
    return g.b0 + g.a * std::exp(-0.5 * u * u);
}

double Gauss::fwhm() const { return 2.355 * std::fabs(s); }
double Gauss::efwhm() const { return 2.355 * es; }

bool fitGauss(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &sigma, Gauss &g)
{
    const int n = (int)x.size();
    if (n < 6 || y.size() != x.size() || sigma.size() != x.size()) return false;
    double p[4] = {0.1, 0.0, 25.0, 0.02};
    auto model = [&](const double *q, double xv) {
        double u = (xv - q[1]) / q[2];
        return q[3] + q[0] * std::exp(-0.5 * u * u);
    };
    auto chi2 = [&](const double *q) {
        double c = 0;
        for (int i = 0; i < n; i++) { double r = (y[i] - model(q, x[i])) / sigma[i]; c += r * r; }
        return c;
    };
    double lambda = 1e-3, c0 = chi2(p);
    double A[4][4];
    bool conv = false;
    for (int it = 0; it < 200; it++) {
        double JTJ[4][4] = {{0}}, JTr[4] = {0};
        for (int i = 0; i < n; i++) {
            double u = (x[i] - p[1]) / p[2], e = std::exp(-0.5 * u * u);
            double J[4] = {e, p[0] * e * u / p[2], p[0] * e * u * u / p[2], 1.0};
            double r = (y[i] - model(p, x[i])) / sigma[i];
            for (int a = 0; a < 4; a++) {
                JTr[a] += J[a] / sigma[i] * r;
                for (int b = 0; b < 4; b++) JTJ[a][b] += J[a] * J[b] / (sigma[i] * sigma[i]);
            }
        }
        for (int a = 0; a < 4; a++) for (int b = 0; b < 4; b++) A[a][b] = JTJ[a][b];
        bool improved = false;
        for (int tries = 0; tries < 30; tries++) {
            double M[4][5];
            for (int a = 0; a < 4; a++) {
                for (int b = 0; b < 4; b++) M[a][b] = JTJ[a][b];
                M[a][a] *= (1.0 + lambda);
                M[a][4] = JTr[a];
            }
            bool sing = false;
            for (int c = 0; c < 4 && !sing; c++) {   // Gauss elimination with pivoting
                int piv = c;
                for (int r = c + 1; r < 4; r++) if (std::fabs(M[r][c]) > std::fabs(M[piv][c])) piv = r;
                if (std::fabs(M[piv][c]) < 1e-300) { sing = true; break; }
                for (int k = 0; k < 5; k++) std::swap(M[c][k], M[piv][k]);
                for (int r = 0; r < 4; r++) {
                    if (r == c) continue;
                    double f = M[r][c] / M[c][c];
                    for (int k = c; k < 5; k++) M[r][k] -= f * M[c][k];
                }
            }
            if (sing) { lambda *= 10; continue; }
            double q[4];
            for (int a = 0; a < 4; a++) q[a] = p[a] + M[a][4] / M[a][a];
            double c1 = chi2(q);
            if (std::isfinite(c1) && c1 < c0) {
                double rel = (c0 - c1) / std::max(c0, 1e-300);
                for (int a = 0; a < 4; a++) p[a] = q[a];
                c0 = c1; lambda = std::max(lambda / 10, 1e-12); improved = true;
                if (rel < 1e-12) conv = true;
                break;
            }
            lambda *= 10;
        }
        if (!improved) { conv = true; break; }
        if (conv) break;
    }
    if (!conv && !std::isfinite(c0)) return false;
    // covariance = inverse(JTJ) * chi2 / (n - 4)
    double M[4][8];
    for (int a = 0; a < 4; a++) for (int b = 0; b < 8; b++) M[a][b] = b < 4 ? A[a][b] : (b - 4 == a ? 1.0 : 0.0);
    for (int c = 0; c < 4; c++) {
        int piv = c;
        for (int r = c + 1; r < 4; r++) if (std::fabs(M[r][c]) > std::fabs(M[piv][c])) piv = r;
        if (std::fabs(M[piv][c]) < 1e-300) return false;
        for (int k = 0; k < 8; k++) std::swap(M[c][k], M[piv][k]);
        double d = M[c][c];
        for (int k = 0; k < 8; k++) M[c][k] /= d;
        for (int r = 0; r < 4; r++) {
            if (r == c) continue;
            double f = M[r][c];
            for (int k = 0; k < 8; k++) M[r][k] -= f * M[c][k];
        }
    }
    double scale = c0 / (n - 4);
    g.a = p[0]; g.x0 = p[1]; g.s = p[2]; g.b0 = p[3];
    g.ea = std::sqrt(std::max(0.0, M[0][4] * scale));
    g.ex0 = std::sqrt(std::max(0.0, M[1][5] * scale));
    g.es = std::sqrt(std::max(0.0, M[2][6] * scale));
    g.eb0 = std::sqrt(std::max(0.0, M[3][7] * scale));
    return std::isfinite(g.s) && std::isfinite(g.a);
}

Measure measure(const std::vector<double> &lambda, const std::vector<double> &fA, const std::vector<double> &fB,
                const std::vector<double> &linesA, const std::vector<double> &linesB, const Params &p)
{
    Measure m;
    size_t n = lambda.size();
    if (n < 10 || fA.size() != n || fB.size() != n) { m.error = "spectra of different length or too short"; return m; }
    if (linesA.empty() && linesB.empty()) { m.error = "no lines"; return m; }
    std::vector<double> dA = depth(fA, p.percentA, p.envelopeSize);
    std::vector<double> dB = depth(fB, p.percentB, p.envelopeSize);

    for (double v = -p.dvMax; v <= p.dvMax + 1e-9; v += p.dvStep) m.dv.push_back(v);
    for (int which = 0; which < 2; which++) {
        const std::vector<double> &d = which == 0 ? dA : dB;
        const std::vector<double> &lines = which == 0 ? linesA : linesB;
        Profile &pr = which == 0 ? m.a : m.b;
        int nl = (int)lines.size();
        if (nl == 0) continue;
        pr.nLines = nl;
        std::vector<double> mean(m.dv.size(), 0.0), sig(m.dv.size());
        for (size_t k = 0; k < m.dv.size(); k++) {
            double s = 0, s2 = 0;
            for (double l : lines) {
                double v = interp(lambda, d, l * (1 + m.dv[k] / kC));
                s += v; s2 += v * v;
            }
            mean[k] = s / nl;
            double var = std::max(0.0, s2 / nl - mean[k] * mean[k]);
            sig[k] = std::sqrt(var) / std::sqrt((double)nl) + 1e-4;
        }
        pr.prof = mean;
        pr.err = sig;
        pr.ok = fitGauss(m.dv, mean, sig, pr.fit);
        if (!pr.ok) m.error += std::string(m.error.empty() ? "" : "; ") + "profile fit of component " + (which == 0 ? "A" : "B") + " failed";
    }
    // depth ratio and correlation inside the wavelength ranges
    std::vector<double> sx = smooth(dA, p.corrSmooth), sy = smooth(dB, p.corrSmooth);
    std::vector<double> x, y;
    for (size_t i = 0; i < n; i++) {
        bool in = p.corrRanges.empty();
        for (size_t r = 0; r + 1 < p.corrRanges.size(); r += 2)
            if (lambda[i] > p.corrRanges[r] && lambda[i] < p.corrRanges[r + 1]) in = true;
        if (in) { x.push_back(sx[i]); y.push_back(sy[i]); }
    }
    if (x.size() > 10) {
        double mx = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
        double my = std::accumulate(y.begin(), y.end(), 0.0) / y.size();
        double sxx = 0, syy = 0, sxy = 0;
        for (size_t i = 0; i < x.size(); i++) {
            sxx += (x[i] - mx) * (x[i] - mx); syy += (y[i] - my) * (y[i] - my); sxy += (x[i] - mx) * (y[i] - my);
        }
        if (sxx > 0) m.ratio = sxy / sxx;
        if (sxx > 0 && syy > 0) m.corr = sxy / std::sqrt(sxx * syy);
    }
    m.ok = m.a.ok || m.b.ok;
    return m;
}

std::vector<double> resample(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &xq)
{
    std::vector<double> out(xq.size());
    if (x.empty() || x.size() != y.size()) return out;
    for (size_t i = 0; i < xq.size(); i++) out[i] = interp(x, y, xq[i]);
    return out;
}

bool readLineList(const std::string &file, Params &p, std::vector<double> &linesA, std::vector<double> &linesB, std::string &err)
{
    std::ifstream in(file.c_str());
    if (!in) { err = "File " + file + " cannot be opened."; return false; }
    linesA.clear();
    linesB.clear();
    std::string line;
    int ln = 0, section = -1;
    while (std::getline(in, line)) {
        ++ln;
        std::istringstream ss(line);
        std::string first;
        if (!(ss >> first)) continue;
        if (first[0] == '#') {
            std::string key;
            if (first.size() > 1) key = first.substr(1);
            else ss >> key;
            if (key == "component") {
                std::string c;
                ss >> c;
                if (c == "A") section = 0;
                else if (c == "B") section = 1;
                else { err = "File " + file + ", line " + std::to_string(ln) + ": component '" + c + "' is not A or B."; return false; }
                continue;
            }
            std::vector<double> v;
            double x;
            while (ss >> x) v.push_back(x);
            if (key == "percent_a" && v.size() == 1) p.percentA = v[0];
            else if (key == "percent_b" && v.size() == 1) p.percentB = v[0];
            else if (key == "envelope" && v.size() == 1) p.envelopeSize = (int)v[0];
            else if (key == "min_depth" && v.size() == 1) p.minDepth = v[0];
            else if (key == "min_separation" && v.size() == 1) p.minSeparation = (int)v[0];
            else if (key == "range" && v.size() == 2) { p.lamMin = v[0]; p.lamMax = v[1]; }
            else if (key == "corr_ranges") p.corrRanges = v;
            continue;
        }
        std::istringstream ts(first);
        double w;
        if (!(ts >> w) || !std::isfinite(w) || w <= 0) {
            err = "File " + file + ", line " + std::to_string(ln) + ": '" + first + "' is not a wavelength.";
            return false;
        }
        if (section < 0) { err = "File " + file + ", line " + std::to_string(ln) + ": a wavelength before the line '# component A' or '# component B'."; return false; }
        (section == 0 ? linesA : linesB).push_back(w);
    }
    if (linesA.empty() && linesB.empty()) { err = "File " + file + " contains no lines."; return false; }
    std::sort(linesA.begin(), linesA.end());
    std::sort(linesB.begin(), linesB.end());
    return true;
}

bool writeLineList(const std::string &file, const Params &p, const std::vector<double> &linesA, const std::vector<double> &linesB, std::string &err)
{
    std::ofstream out(file.c_str());
    if (!out) { err = "File " + file + " cannot be written."; return false; }
    out << std::setprecision(10);
    out << "# Spectangular line list of the line width scan (Optimisation tab, Find Lines)\n";
    out << "# percent_a " << p.percentA << "\n# percent_b " << p.percentB << "\n";
    out << "# envelope " << p.envelopeSize << "\n# min_depth " << p.minDepth << "\n# min_separation " << p.minSeparation << "\n";
    out << "# range " << p.lamMin << " " << p.lamMax << "\n";
    out << "# corr_ranges";
    for (double v : p.corrRanges) out << " " << v;
    out << "\n# component A\n";
    for (double l : linesA) out << l << "\n";
    out << "# component B\n";
    for (double l : linesB) out << l << "\n";
    if (!out) { err = "File " + file + " cannot be written."; return false; }
    return true;
}

bool readOrbitGrid(const std::string &file, std::vector<std::array<double, 7>> &orbits, std::string &err)
{
    std::ifstream in(file.c_str());
    if (!in) { err = "File " + file + " cannot be opened."; return false; }
    std::vector<std::vector<double>> rows;
    std::string line;
    int ln = 0;
    while (std::getline(in, line)) {
        ++ln;
        std::istringstream ss(line);
        std::string tok;
        std::vector<double> r;
        bool comment = false;
        while (ss >> tok) {
            if (tok[0] == '#') { comment = true; break; }
            std::istringstream ts(tok);
            double v;
            if (!(ts >> v) || !std::isfinite(v)) { err = "File " + file + ", line " + std::to_string(ln) + ": '" + tok + "' is not a number."; return false; }
            r.push_back(v);
        }
        (void)comment;
        if (!r.empty()) rows.push_back(r);
    }
    if (rows.size() != 7) {
        err = "File " + file + " must have 7 rows (P, e, K_A, K_B, gamma, T_peri, omega), found " + std::to_string(rows.size()) + ".";
        return false;
    }
    size_t n = rows[0].size();
    for (size_t r = 1; r < 7; r++)
        if (rows[r].size() != n) { err = "File " + file + ": row " + std::to_string(r + 1) + " has " + std::to_string(rows[r].size()) + " values, row 1 has " + std::to_string(n) + "."; return false; }
    orbits.assign(n, std::array<double, 7>());
    for (size_t k = 0; k < n; k++)
        for (int r = 0; r < 7; r++) orbits[k][r] = rows[r][k];
    return true;
}

namespace {
void putProfile(std::ostream &out, const Profile &pr)
{
    if (pr.ok) out << "\t" << pr.nLines << "\t" << pr.fit.a << "\t" << pr.fit.x0 << "\t" << pr.fit.fwhm() << "\t" << pr.fit.efwhm();
    else out << "\t-\t-\t-\t-\t-";
}
}  // namespace

bool writeScan(const std::string &file, const ScanInfo &info, const std::vector<ScanRow> &rows, std::string &err)
{
    std::ofstream out(file.c_str());
    if (!out) { err = "File " + file + " cannot be written."; return false; }
    out << std::setprecision(10);
    out << "# Line width scan: the lines of " << info.linesFile << " of each component measured in that component\n";
    out << "# n  P  e  K_A  K_B  gamma  T_peri  omega  residuum  nA depthA v0A FWHM_A eFWHM_A  nB depthB v0B FWHM_B eFWHM_B  depth_ratio(B/A)  correlation  (FWHM in km/s; - : not measured)\n";
    for (size_t k = 0; k < rows.size(); k++) {
        const ScanRow &r = rows[k];
        out << k + 1;
        for (int e = 0; e < 7; e++) out << "\t" << r.el[e];
        out << "\t" << r.residuum;
        putProfile(out, r.m.a);
        putProfile(out, r.m.b);
        if (r.ok) out << "\t" << r.m.ratio << "\t" << r.m.corr << "\n";
        else out << "\t-\t-\n";
    }
    if (!out) { err = "File " + file + " cannot be written."; return false; }
    return true;
}

bool readScan(const std::string &file, ScanInfo &info, std::vector<ScanRow> &rows, std::string &err)
{
    (void)info;
    std::ifstream in(file.c_str());
    if (!in) { err = "File " + file + " cannot be opened."; return false; }
    rows.clear();
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string first;
        if (!(ss >> first)) continue;
        if (first[0] == '#') continue;
        std::vector<std::string> tok{first};
        std::string t;
        while (ss >> t) tok.push_back(t);
        if (tok.size() != 21) { err = "File " + file + ": a row has " + std::to_string(tok.size()) + " columns, expected 21."; return false; }
        ScanRow r;
        auto num = [&](size_t i, double &v) { std::istringstream ts(tok[i]); return bool(ts >> v); };
        for (int e = 0; e < 7; e++) if (!num(1 + e, r.el[e])) { err = "File " + file + ": '" + tok[1 + e] + "' is not a number."; return false; }
        if (!num(8, r.residuum)) { err = "File " + file + ": '" + tok[8] + "' is not a number."; return false; }
        for (int which = 0; which < 2; which++) {
            Profile &pr = which == 0 ? r.m.a : r.m.b;
            size_t c0 = 9 + 5 * which;
            double nl, a, v0, w, ew;
            if (num(c0, nl) && num(c0 + 1, a) && num(c0 + 2, v0) && num(c0 + 3, w) && num(c0 + 4, ew)) {
                pr.ok = true;
                pr.nLines = (int)nl;
                pr.fit.a = a; pr.fit.x0 = v0; pr.fit.s = w / 2.355; pr.fit.es = ew / 2.355;
            }
        }
        r.ok = r.m.ok = r.m.a.ok || r.m.b.ok;
        num(19, r.m.ratio);
        num(20, r.m.corr);
        rows.push_back(r);
    }
    if (rows.empty()) { err = "File " + file + " contains no orbits."; return false; }
    return true;
}

bool writeProfiles(const std::string &file, const std::vector<ScanRow> &rows, std::string &err)
{
    std::ofstream out(file.c_str());
    if (!out) { err = "File " + file + " cannot be written."; return false; }
    out << std::setprecision(10);
    out << "# Mean line profiles of the line width scan, one block per orbit (blocks separated by blank lines)\n";
    for (size_t k = 0; k < rows.size(); k++) {
        const ScanRow &r = rows[k];
        out << "# orbit " << k + 1 << "\n";
        if (!r.ok || r.m.dv.empty()) { out << "# no measurement\n\n"; continue; }
        auto fit = [&](const char *name, const Profile &pr) {
            if (!pr.ok) { out << "# " << name << " none\n"; return; }
            const Gauss &g = pr.fit;
            out << "# " << name << " " << g.a << " " << g.ea << " " << g.x0 << " " << g.ex0 << " " << g.s << " " << g.es << " " << g.b0 << " " << g.eb0 << "\n";
        };
        fit("fit_A", r.m.a);
        fit("fit_B", r.m.b);
        out << "# dv  depth_A  err_A  fit_A  depth_B  err_B  fit_B   (nan: component not measured)\n";
        for (size_t i = 0; i < r.m.dv.size(); i++) {
            out << r.m.dv[i];
            for (const Profile *pr : {&r.m.a, &r.m.b}) {
                if (pr->ok && i < pr->prof.size()) out << "\t" << pr->prof[i] << "\t" << pr->err[i] << "\t" << gaussValue(pr->fit, r.m.dv[i]);
                else out << "\tnan\tnan\tnan";
            }
            out << "\n";
        }
        out << "\n";
    }
    if (!out) { err = "File " + file + " cannot be written."; return false; }
    return true;
}

bool readProfiles(const std::string &file, std::vector<ScanRow> &rows, std::string &err)
{
    std::ifstream in(file.c_str());
    if (!in) { err = "File " + file + " cannot be opened."; return false; }
    std::string line;
    int cur = -1;
    int blocks = 0;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string first;
        if (!(ss >> first)) continue;
        if (first == "#") {
            std::string key;
            ss >> key;
            if (key == "orbit") { ss >> cur; --cur; ++blocks; if (cur < 0 || cur >= (int)rows.size()) { err = "File " + file + ": orbit number out of range."; return false; } }
            else if ((key == "fit_A" || key == "fit_B") && cur >= 0) {
                Profile &pr = key == "fit_A" ? rows[cur].m.a : rows[cur].m.b;
                Gauss g;
                if (ss >> g.a >> g.ea >> g.x0 >> g.ex0 >> g.s >> g.es >> g.b0 >> g.eb0) { pr.fit = g; pr.ok = true; }
                else pr.ok = false;
            }
            continue;
        }
        if (cur < 0) continue;
        std::vector<double> v{std::atof(first.c_str())};
        std::string t;
        while (ss >> t) v.push_back(std::atof(t.c_str()));
        if (v.size() != 7) { err = "File " + file + ": a profile row has " + std::to_string(v.size()) + " columns, expected 7."; return false; }
        Measure &m = rows[cur].m;
        m.dv.push_back(v[0]);
        m.a.prof.push_back(v[1]); m.a.err.push_back(v[2]);
        m.b.prof.push_back(v[4]); m.b.err.push_back(v[5]);
    }
    if (blocks != (int)rows.size()) { err = "File " + file + " has " + std::to_string(blocks) + " orbits, the scan " + std::to_string(rows.size()) + "."; return false; }
    return true;
}

}  // namespace linewidth
