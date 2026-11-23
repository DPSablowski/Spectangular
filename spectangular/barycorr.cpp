#include "barycorr.h"

extern "C" {
#include "erfa/erfa.h"
#include "erfa/erfam.h"
}

#include <cmath>
#include <limits>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <vector>

namespace bc {

namespace {

const double C_KMS = 299792.458;

// unit vector to the star
void direction(double raDeg, double decDeg, double n[3])
{
    const double a = raDeg * ERFA_DD2R, d = decDeg * ERFA_DD2R;
    n[0] = std::cos(d) * std::cos(a);
    n[1] = std::cos(d) * std::sin(a);
    n[2] = std::sin(d);
}

// TAI-UTC in seconds at JD (UTC)
bool deltaAt(double jdUtc, double &dat)
{
    int iy, im, id;
    double fd;
    if (eraJd2cal(2400000.5, jdUtc - 2400000.5, &iy, &im, &id, &fd) != 0) return false;
    return eraDat(iy, im, id, fd, &dat) >= 0;
}

// Earth: heliocentric and barycentric position [au] at JD (TDB, approximately TT)
bool earth(double jdTdb, double ph[3], double pb[3])
{
    double pvh[2][3], pvb[2][3];
    eraEpv00(2400000.5, jdTdb - 2400000.5, pvh, pvb);
    for (int i = 0; i < 3; ++i) { ph[i] = pvh[0][i]; pb[i] = pvb[0][i]; }
    return true;
}

double dot(const double a[3], const double b[3]) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

bool sexagesimal(std::string s, double &value)
{
    for (char &c : s) if (c == ':' || c == 'h' || c == 'm' || c == 'd' || c == 's' || c == '\'' || c == '"') c = ' ';
    std::istringstream in(s);
    std::vector<double> parts;
    std::string tok;
    bool neg = false;
    while (in >> tok) {
        char *end = nullptr;
        const double v = std::strtod(tok.c_str(), &end);
        if (end == tok.c_str() || *end != '\0') return false;
        if (parts.empty() && tok[0] == '-') neg = true;
        parts.push_back(std::fabs(v));
    }
    if (parts.empty() || parts.size() > 3) return false;
    double v = 0, f = 1;
    for (double p : parts) { v += p * f; f /= 60.0; }
    value = neg ? -v : v;
    return true;
}

} // namespace

bool parseRa(const std::string &s, double &deg)
{
    double v;
    if (!sexagesimal(s, v)) return false;
    // several fields (h m s): hours; one field: degrees
    std::string t = s;
    for (char &c : t) if (c == ':' || c == 'h' || c == 'm' || c == 's') c = ' ';
    std::istringstream in(t);
    std::string tok;
    int fields = 0;
    while (in >> tok) ++fields;
    deg = fields > 1 ? v * 15.0 : v;
    return deg >= 0 && deg < 360;
}

bool parseDec(const std::string &s, double &deg)
{
    if (!sexagesimal(s, deg)) return false;
    return deg >= -90 && deg <= 90;
}

bool correction(double raDeg, double decDeg, const Site &site, double t, TimeScale scale,
                double &bcKms, double *jdUtcOut, std::string &err, double *altDeg)
{
    if (!(decDeg >= -90 && decDeg <= 90) || !std::isfinite(raDeg)) { err = "invalid coordinates"; return false; }
    if (!(site.latDeg >= -90 && site.latDeg <= 90) || !std::isfinite(site.lonDeg)) { err = "invalid site"; return false; }
    double n[3];
    direction(raDeg, decDeg, n);
    const double lt = ERFA_AULT / ERFA_DAYSEC;   // light time for 1 au in days
    // time -> JD (UTC)
    double jd = 0;
    switch (scale) {
    case TimeScale::JD_UTC: jd = t; break;
    case TimeScale::MJD_UTC: jd = t + 2400000.5; break;
    case TimeScale::HJD_UTC: {
        jd = t;
        for (int it = 0; it < 3; ++it) {
            double dat;
            if (!deltaAt(jd, dat)) { err = "time outside the range of the leap-second table"; return false; }
            double ph[3], pb[3];
            earth(jd + (dat + 32.184) / ERFA_DAYSEC, ph, pb);
            jd = t - dot(ph, n) * lt;
        }
        break;
    }
    case TimeScale::BJD_TDB: {
        double tdb = t;
        for (int it = 0; it < 3; ++it) {
            double ph[3], pb[3];
            earth(tdb, ph, pb);
            tdb = t - dot(pb, n) * lt;
        }
        jd = tdb;
        for (int it = 0; it < 3; ++it) {      // TDB -> UTC (TDB-TT < 2 ms neglected)
            double dat;
            if (!deltaAt(jd, dat)) { err = "time outside the range of the leap-second table"; return false; }
            jd = tdb - (dat + 32.184) / ERFA_DAYSEC;
        }
        break;
    }
    }
    if (!(jd > 2400000.0 && jd < 2600000.0)) { err = "time not a Julian date (JD 2400000-2600000)"; return false; }
    eraASTROM astrom;
    double eo;
    const int j = eraApco13(2400000.5, jd - 2400000.5, 0.0, site.lonDeg * ERFA_DD2R, site.latDeg * ERFA_DD2R,
                            site.heightM, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, &astrom, &eo);
    if (j < 0) { err = "time outside the range of ERFA"; return false; }
    // astrom.v: barycentric velocity of the observer in units of c
    bcKms = C_KMS * dot(astrom.v, n);
    if (jdUtcOut) *jdUtcOut = jd;
    if (altDeg) {
        // direction in CIRS (bias-precession-nutation), hour angle from the local Earth rotation
        // angle; altitude from the geodetic latitude
        double c[3];
        for (int i = 0; i < 3; ++i) c[i] = astrom.bpn[i][0] * n[0] + astrom.bpn[i][1] * n[1] + astrom.bpn[i][2] * n[2];
        const double ra = std::atan2(c[1], c[0]), de = std::asin(std::max(-1.0, std::min(1.0, c[2])));
        const double ha = astrom.eral - ra, phi = site.latDeg * ERFA_DD2R;
        const double sa = std::sin(phi) * std::sin(de) + std::cos(phi) * std::cos(de) * std::cos(ha);
        *altDeg = std::asin(std::max(-1.0, std::min(1.0, sa))) / ERFA_DD2R;
    }
    return true;
}

double airmass(double altDeg)
{
    if (!(altDeg > 0)) return std::numeric_limits<double>::quiet_NaN();
    const double h = altDeg + 244.0 / (165.0 + 47.0 * std::pow(altDeg, 1.1));
    return 1.0 / std::sin(h * ERFA_DD2R);
}

// strong telluric bands, A (Buton et al. 2013; O2 gamma left out: its regular line
// spacing gives side peaks of the correlation at +-37 km/s)
const std::vector<std::pair<double, double>> kTelluricBands = {
    {6860, 6967}, {7141, 7400}, {7584, 7705}, {8082, 8423}, {8914, 9932}};

namespace {
const double kC = 299792.458;

// linear interpolation of (x, y) (x ascending) at xi; false outside
bool interp(const std::vector<double> &x, const std::vector<double> &y, double xi, double &yi)
{
    if (x.size() < 2 || xi < x.front() || xi > x.back()) return false;
    const size_t k = size_t(std::upper_bound(x.begin(), x.end(), xi) - x.begin());
    if (k == 0) { yi = y.front(); return true; }
    if (k >= x.size()) { yi = y.back(); return true; }
    const double t = (xi - x[k - 1]) / (x[k] - x[k - 1]);
    yi = y[k - 1] + t * (y[k] - y[k - 1]);
    return std::isfinite(yi);
}

// window: median normalisation, minus a box mean (high-pass), mean removed
std::vector<double> prepare(std::vector<double> v, int box)
{
    std::vector<double> s = v;
    std::nth_element(s.begin(), s.begin() + long(s.size() / 2), s.end());
    const double med = s[s.size() / 2];
    if (!(med != 0)) return std::vector<double>(v.size(), 0.0);
    for (double &x : v) x = x / med - 1.0;
    const int n = int(v.size()), h = box / 2;
    std::vector<double> cum(size_t(n) + 1, 0.0), out(v.size());
    for (int i = 0; i < n; ++i) cum[size_t(i) + 1] = cum[size_t(i)] + v[size_t(i)];
    double mean = 0.0;
    for (int i = 0; i < n; ++i) {
        const int a = std::max(0, i - h), b = std::min(n, i + h + 1);
        out[size_t(i)] = v[size_t(i)] - (cum[size_t(b)] - cum[size_t(a)]) / (b - a);
        mean += out[size_t(i)];
    }
    mean /= std::max(1, n);
    for (double &x : out) x -= mean;
    return out;
}
} // namespace

bool telluricShifts(const std::vector<std::vector<double>> &logw, const std::vector<std::vector<double>> &flux,
                    double maxLagKms, TelluricShifts &out, std::string &err)
{
    const size_t n = logw.size();
    out = TelluricShifts();
    if (n == 0 || flux.size() != n) { err = "no spectra."; return false; }
    const std::vector<double> &w0 = logw[0];
    if (w0.size() < 10) { err = "spectrum 0 too short."; return false; }
    std::vector<double> d(w0.size() - 1);
    for (size_t i = 1; i < w0.size(); ++i) d[i - 1] = w0[i] - w0[i - 1];
    std::nth_element(d.begin(), d.begin() + long(d.size() / 2), d.end());
    const double step = d[d.size() / 2];
    if (!(step > 0)) { err = "wavelengths of spectrum 0 not ascending."; return false; }
    const double vpix = step * std::log(10.0) * kC;
    const int maxLag = std::max(1, int(std::ceil(maxLagKms / vpix)));
    const int box = std::max(5, int(std::lround(std::log10(1.0 + 1.0 / 6500.0) / step)) | 1);   // about 1 A
    // common grid per band (inside spectrum 0, without the lag margin)
    std::vector<std::vector<double>> grids;
    for (const auto &b : kTelluricBands) {
        const double lo = std::max(std::log10(b.first), w0.front()) + maxLag * step;
        const double hi = std::min(std::log10(b.second), w0.back()) - maxLag * step;
        if (hi - lo < 50 * step) continue;
        std::vector<double> g;
        for (double x = lo; x <= hi; x += step) g.push_back(x);
        grids.push_back(g);
        out.bands += (out.bands.empty() ? "" : ", ") + std::to_string(int(b.first)) + "-" + std::to_string(int(b.second));
    }
    if (grids.empty()) {
        err = "no strong telluric band (O2 B 6860-6967, H2O 7141-7400, O2 A 7584-7705, H2O 8082-8423 or "
              "8914-9932 A) lies in the spectra.";
        return false;
    }
    // reference windows (spectrum 0) on the grids; other spectra on the grids widened by the lags
    std::vector<std::vector<double>> ref;
    for (const auto &g : grids) {
        std::vector<double> v(g.size(), 0.0);
        for (size_t k = 0; k < g.size(); ++k) if (!interp(w0, flux[0], g[k], v[k])) v[k] = 0.0;
        ref.push_back(prepare(v, box));
    }
    out.shiftKms.assign(n, 0.0);
    out.corr.assign(n, 0.0);
    for (size_t s = 0; s < n; ++s) {
        std::vector<double> cc(size_t(2 * maxLag + 1), 0.0), rr(cc.size(), 0.0), gg(cc.size(), 0.0);
        for (size_t b = 0; b < grids.size(); ++b) {
            const auto &g = grids[b];
            const int m = int(g.size()) + 2 * maxLag;
            std::vector<double> v(static_cast<size_t>(m), 0.0);
            bool any = false;
            for (int k = 0; k < m; ++k) {
                const double x = g.front() + (k - maxLag) * step;
                if (interp(logw[s], flux[s], x, v[size_t(k)])) any = true; else v[size_t(k)] = std::nan("");
            }
            if (!any) continue;
            // gaps: the median of the window
            std::vector<double> ok;
            for (double x : v) if (std::isfinite(x)) ok.push_back(x);
            std::nth_element(ok.begin(), ok.begin() + long(ok.size() / 2), ok.end());
            for (double &x : v) if (!std::isfinite(x)) x = ok[ok.size() / 2];
            const std::vector<double> p = prepare(v, box);
            const auto &r = ref[b];
            for (int L = -maxLag; L <= maxLag; ++L) {
                double sxy = 0, sxx = 0, syy = 0;
                for (size_t k = 0; k < g.size(); ++k) {
                    const double y = p[size_t(int(k) + maxLag + L)];
                    sxy += r[k] * y;
                    sxx += r[k] * r[k];
                    syy += y * y;
                }
                cc[size_t(L + maxLag)] += sxy;
                rr[size_t(L + maxLag)] += sxx;
                gg[size_t(L + maxLag)] += syy;
            }
        }
        int best = 0;
        for (int i = 1; i < int(cc.size()); ++i) if (cc[size_t(i)] > cc[size_t(best)]) best = i;
        double off = best - maxLag;
        if (best > 0 && best + 1 < int(cc.size())) {
            const double y0 = cc[size_t(best - 1)], y1 = cc[size_t(best)], y2 = cc[size_t(best + 1)];
            const double den = y0 - 2 * y1 + y2;
            if (den < 0) off += 0.5 * (y0 - y2) / den;
        }
        // grid of spectrum s shifted by +L matches the reference: its lines lie L pixels to the red
        out.shiftKms[s] = off * vpix;
        const double nn = std::sqrt(rr[size_t(best)] * gg[size_t(best)]);
        out.corr[s] = nn > 0 ? cc[size_t(best)] / nn : 0.0;
    }
    return true;
}

} // namespace bc
