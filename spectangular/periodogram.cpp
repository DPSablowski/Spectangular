#include "periodogram.h"

#include <algorithm>
#include <cmath>

namespace period {

namespace {
const double kTwoPi = 6.283185307179586;
}

bool gls(const std::vector<double> &t, const std::vector<double> &y, const std::vector<double> &wIn,
         double fmin, double fmax, double oversampling, Periodogram &out, std::string &err)
{
    out = Periodogram();
    const size_t n = t.size();
    if (n < 4 || y.size() != n || (!wIn.empty() && wIn.size() != n)) {
        err = "at least 4 points are needed.";
        return false;
    }
    for (size_t i = 0; i < n; ++i)
        if (!std::isfinite(t[i]) || !std::isfinite(y[i]) || (!wIn.empty() && !(wIn[i] > 0))) {
            err = "times, values and weights must be finite (weights > 0).";
            return false;
        }
    const auto mm = std::minmax_element(t.begin(), t.end());
    const double tmin = *mm.first, tspan = *mm.second - *mm.first;
    if (!(tspan > 0)) { err = "the times span no interval."; return false; }
    if (!(fmin >= 0) || !(fmax > fmin) || !std::isfinite(fmax)) { err = "invalid frequency range."; return false; }
    if (!(oversampling >= 1)) oversampling = 1;
    const double df = 1.0 / (oversampling * tspan);
    const size_t nf = size_t(std::floor((fmax - fmin) / df)) + 1;
    if (nf > 20000000) { err = "too many frequencies (range or oversampling too large)."; return false; }
    // normalised weights
    std::vector<double> w(n, 1.0 / double(n));
    if (!wIn.empty()) {
        double s = 0.0;
        for (double x : wIn) s += x;
        for (size_t i = 0; i < n; ++i) w[i] = wIn[i] / s;
    }
    double Y = 0.0;
    for (size_t i = 0; i < n; ++i) Y += w[i] * y[i];
    double YYh = 0.0;
    for (size_t i = 0; i < n; ++i) YYh += w[i] * (y[i] - Y) * (y[i] - Y);
    out.tspan = tspan;
    out.nIndependent = std::max(1, int(std::lround((fmax - fmin) * tspan)));
    out.freq.resize(nf);
    out.power.resize(nf);
    out.window.resize(nf);
    for (size_t k = 0; k < nf; ++k) {
        const double f = fmin + df * double(k);
        double C = 0, S = 0, YC = 0, YS = 0, CC = 0, CS = 0, wc = 0, ws = 0;
        for (size_t i = 0; i < n; ++i) {
            const double ph = kTwoPi * f * (t[i] - tmin);
            const double c = std::cos(ph), s = std::sin(ph);
            C += w[i] * c;
            S += w[i] * s;
            YC += w[i] * y[i] * c;
            YS += w[i] * y[i] * s;
            CC += w[i] * c * c;
            CS += w[i] * c * s;
            wc += c;
            ws += s;
        }
        const double SS = 1.0 - CC;
        const double yc = YC - Y * C, ys = YS - Y * S;
        const double cc = CC - C * C, ss = SS - S * S, cs = CS - C * S;
        const double D = cc * ss - cs * cs;
        double p = 0.0;
        if (YYh > 0 && D > 1e-14) p = (ss * yc * yc + cc * ys * ys - 2.0 * cs * yc * ys) / (YYh * D);
        out.freq[k] = f;
        out.power[k] = std::min(1.0, std::max(0.0, p));
        out.window[k] = (wc * wc + ws * ws) / (double(n) * double(n));
    }
    return true;
}

double fap(double p, int n, int m)
{
    if (n <= 3) return 1.0;
    p = std::min(1.0, std::max(0.0, p));
    const double prob = std::pow(1.0 - p, 0.5 * (n - 3));        // single frequency
    // 1 - (1 - prob)^M, accurate for small prob
    return -std::expm1(double(std::max(1, m)) * std::log1p(-std::min(prob, 1.0 - 1e-300)));
}

double powerAtFap(double fapLevel, int n, int m)
{
    if (n <= 3) return 1.0;
    // 1 - prob = (1 - FAP)^(1/M)
    const double prob = -std::expm1(std::log1p(-fapLevel) / double(std::max(1, m)));
    return 1.0 - std::pow(prob, 2.0 / (n - 3));
}

Sine sineFit(const std::vector<double> &t, const std::vector<double> &y, const std::vector<double> &wIn, double f)
{
    Sine r;
    const size_t n = t.size();
    if (n < 3 || y.size() != n) return r;
    const double tmin = *std::min_element(t.begin(), t.end());
    // normal equations for (c, a, b)
    double A[3][3] = {{0}}, B[3] = {0};
    for (size_t i = 0; i < n; ++i) {
        const double w = wIn.empty() ? 1.0 : wIn[i];
        const double ph = kTwoPi * f * (t[i] - tmin);
        const double v[3] = {1.0, std::cos(ph), std::sin(ph)};
        for (int a = 0; a < 3; ++a) {
            B[a] += w * v[a] * y[i];
            for (int b = 0; b < 3; ++b) A[a][b] += w * v[a] * v[b];
        }
    }
    // Gauss elimination with pivoting
    int idx[3] = {0, 1, 2};
    for (int c = 0; c < 3; ++c) {
        int p = c;
        for (int r2 = c + 1; r2 < 3; ++r2) if (std::fabs(A[r2][c]) > std::fabs(A[p][c])) p = r2;
        if (std::fabs(A[p][c]) < 1e-300) return r;
        if (p != c) { std::swap(A[p], A[c]); std::swap(B[p], B[c]); std::swap(idx[p], idx[c]); }
        for (int r2 = c + 1; r2 < 3; ++r2) {
            const double q = A[r2][c] / A[c][c];
            for (int k = c; k < 3; ++k) A[r2][k] -= q * A[c][k];
            B[r2] -= q * B[c];
        }
    }
    double x[3];
    for (int c = 2; c >= 0; --c) {
        double s = B[c];
        for (int k = c + 1; k < 3; ++k) s -= A[c][k] * x[k];
        x[c] = s / A[c][c];
    }
    r.offset = x[0];
    r.amplitude = std::hypot(x[1], x[2]);
    // maximum: a cos(ph) + b sin(ph) = R cos(ph - phi), phi = atan2(b, a)
    double phi = std::atan2(x[2], x[1]);
    if (phi < 0) phi += kTwoPi;
    r.tMax = f > 0 ? tmin + phi / (kTwoPi * f) : tmin;
    return r;
}

std::vector<size_t> peaks(const Periodogram &p, size_t n)
{
    std::vector<size_t> loc;
    const size_t m = p.power.size();
    for (size_t k = 0; k < m; ++k) {
        const double v = p.power[k];
        if ((k == 0 || v >= p.power[k - 1]) && (k + 1 == m || v > p.power[k + 1])) loc.push_back(k);
    }
    std::sort(loc.begin(), loc.end(), [&](size_t a, size_t b) { return p.power[a] > p.power[b]; });
    std::vector<size_t> out;
    const double sep = p.tspan > 0 ? 1.0 / p.tspan : 0.0;
    for (size_t k : loc) {
        bool close = false;
        for (size_t o : out) if (std::fabs(p.freq[k] - p.freq[o]) < sep) { close = true; break; }
        if (!close) out.push_back(k);
        if (out.size() >= n) break;
    }
    return out;
}

} // namespace period
