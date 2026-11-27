#ifndef PERIODOGRAM_H
#define PERIODOGRAM_H

// Period analysis of unevenly sampled data: generalised Lomb-Scargle periodogram (floating mean,
// Zechmeister & Kuerster 2009, A&A 496, 577), spectral window, false-alarm probability, and the
// sine fit at one frequency. Used for the residuals per spectrum (Tools -> Period analysis).

#include <string>
#include <vector>

namespace period {

struct Periodogram {
    std::vector<double> freq;     // frequencies [1/time unit]
    std::vector<double> power;    // normalised GLS power p in [0, 1] (reduction of chi^2)
    std::vector<double> window;   // spectral window |sum exp(2 pi i f t)|^2 / N^2
    double tspan = 0.0;           // time span of the data
    int nIndependent = 0;         // number of independent frequencies, (fmax - fmin) * tspan
};

// GLS periodogram on fmin..fmax with step 1/(oversampling * tspan). Weights w (1/sigma^2) may be
// empty (equal weights). Returns false with err for fewer than 4 points, an empty or invalid
// frequency range or a zero time span.
bool gls(const std::vector<double> &t, const std::vector<double> &y, const std::vector<double> &w,
         double fmin, double fmax, double oversampling, Periodogram &out, std::string &err);

// False-alarm probability of a peak of power p for N points and M independent frequencies:
// FAP = 1 - (1 - (1-p)^((N-3)/2))^M (Zechmeister & Kuerster 2009, eq. 24 and 25).
double fap(double p, int n, int m);
// power of a given false-alarm probability (inverse of fap)
double powerAtFap(double fapLevel, int n, int m);

// Sine fit y = c + a cos(2 pi f t) + b sin(2 pi f t) (weights w may be empty): amplitude
// sqrt(a^2+b^2), time of maximum t0 (in [t_min, t_min + 1/f)), offset c.
struct Sine { double amplitude = 0.0, offset = 0.0, tMax = 0.0; };
Sine sineFit(const std::vector<double> &t, const std::vector<double> &y, const std::vector<double> &w, double f);

// indices of the highest local maxima of the power, highest first (at most n; peaks closer than
// 1/tspan to a higher one are left out)
std::vector<size_t> peaks(const Periodogram &p, size_t n);

} // namespace period

#endif
