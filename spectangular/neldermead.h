#ifndef NELDERMEAD_H
#define NELDERMEAD_H

// One iteration step of the downhill simplex after Nelder & Mead (1965), as used
// by Spectangular for RVs, orbital elements and flux ratios. Without GUI.
//
// Simplex: P with n+1 rows (points) and n columns (parameters), y[i] = f(P.row(i)).

#include <armadillo>
#include <deque>
#include <functional>
#include <string>
#include <vector>

namespace nm {

struct Coefficients {
    double alpha = 1.0;   // reflection
    double gamma = 2.0;   // expansion
    double beta = 0.5;    // contraction
    double delta = 0.5;   // shrinking towards the best point
};

// Indices of the largest (h), second largest (sh) and smallest (l) value.
// sh != h even for equal values; for n+1 == 1, sh == h.
void order(const std::vector<double> &y, int &h, int &sh, int &l);

struct StepResult {
    std::string log;      // short description of the steps for the log file
    int evaluations = 0;
    bool aborted = false; // abort() returned true; simplex contains all
                          // points accepted up to then
};

// Performs one step. f evaluates a point, abort is queried after each
// evaluation.
StepResult step(arma::mat &P, std::vector<double> &y, const Coefficients &c,
                const std::function<double(const arma::rowvec &)> &f,
                const std::function<bool()> &abort);

// Progress of the optimization: best S^2 (residuum^2) per iteration. Converged
// when it has decreased by less than tol over the last `window` iterations
// (tol = Delta-chi^2 * sigma^2). Robust against rounding noise, unlike
// comparing the simplex values for exact equality.
class ProgressMonitor {
public:
    explicit ProgressMonitor(int window) : w_(window < 1 ? 1 : window) {}
    void add(double bestS2)
    {
        h_.push_back(bestS2);
        while (int(h_.size()) > w_ + 1)
            h_.pop_front();
    }
    bool ready() const { return int(h_.size()) > w_; }
    // decrease of S^2 over the window (0 while there are too few values)
    double gain() const { return ready() ? h_.front() - h_.back() : 0.0; }
    bool converged(double tol) const { return ready() && gain() < tol; }
    int window() const { return w_; }

private:
    std::deque<double> h_;
    int w_;
};

// Default for the window: 10 iterations per parameter, at least 200
inline int defaultWindow(int nParams) { return nParams * 10 < 200 ? 200 : nParams * 10; }

} // namespace nm

#endif // NELDERMEAD_H
