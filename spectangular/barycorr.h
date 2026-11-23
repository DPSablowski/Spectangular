#ifndef BARYCORR_H
#define BARYCORR_H

// Barycentric correction of radial velocities / wavelengths, without GUI.
//
// BC = v_obs . n: projection of the barycentric velocity of the observer onto the direction
// of the star (ICRS / J2000; proper motion neglected). Velocity of the Earth from
// ERFA eraEpv00, rotation of the Earth and position of the observatory from eraApco13
// (precession-nutation IAU 2006/2000A, polar motion and UT1-UTC neglected: < 1 m/s).
// Corrected:  v_bary = v_measured + BC (to first order in v/c),
//             lambda_bary = lambda_observed * (1 + BC / c).

#include <string>
#include <utility>
#include <vector>

namespace bc {

struct Site {
    double latDeg = 0.0;     // geodetic latitude, north positive
    double lonDeg = 0.0;     // longitude, east positive
    double heightM = 0.0;    // height above the ellipsoid
};

enum class TimeScale { JD_UTC, MJD_UTC, HJD_UTC, BJD_TDB };

// Correction in km/s for the time t of the given scale (mid-exposure). jdUtc: the time
// converted to JD (UTC), if not null. HJD and BJD are converted with the light time to the
// Sun / barycentre for the direction of the star.
// Returns false with err for invalid input.
// altDeg (if not null): altitude of the star above the horizon (geometric, without refraction
// and aberration: < 0.01 deg), for the airmass.
bool correction(double raDeg, double decDeg, const Site &site, double t, TimeScale scale,
                double &bcKms, double *jdUtc, std::string &err, double *altDeg = nullptr);

// Airmass for the altitude in degrees (Pickering 2002, within 0.1 % of the plane-parallel
// sec z up to z = 60 deg, finite at the horizon); NaN below the horizon.
double airmass(double altDeg);

// "02 12 22.28", "02:12:22.28" (hours) or "33.0928" (degrees) -> degrees; false if invalid.
bool parseRa(const std::string &s, double &deg);
// "+30 18 11.1", "+30:18:11.1" or "30.303" -> degrees; false if invalid.
bool parseDec(const std::string &s, double &deg);

// Position of the telluric lines in every spectrum relative to the first one, from the
// cross-correlation over the strong telluric bands (kTelluricBands, those inside the
// reference spectrum). Spectra: log10(lambda/A) and flux each; each window is normalised by
// its median and high-pass filtered (box of about 1 A), so continuum, ripple and broad stellar
// lines hardly enter. shiftKms[i]: velocity of the telluric lines of spectrum i minus that of
// spectrum 0 (positive: to the red), from the peak of the summed correlation within
// +-maxLagKms (parabola through the peak); corr[i]: correlation coefficient at the peak.
// For barycentrically corrected spectra the expected shift is BC_i - BC_0, in the observatory
// frame 0. Returns false with err if no band lies in the data.
struct TelluricShifts { std::vector<double> shiftKms, corr; std::string bands; };
extern const std::vector<std::pair<double, double>> kTelluricBands;
bool telluricShifts(const std::vector<std::vector<double>> &logw, const std::vector<std::vector<double>> &flux,
                    double maxLagKms, TelluricShifts &out, std::string &err);

} // namespace bc

#endif // BARYCORR_H
