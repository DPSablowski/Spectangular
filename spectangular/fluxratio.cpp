#include "fluxratio.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

namespace fr {

namespace {
// second radiation constant h c / k in A K
constexpr double kC2 = 1.438776877e8;
}

double planck(double lambdaA, double t)
{
    const double x = kC2 / (lambdaA * t);
    // lambda^-5 / (exp(x) - 1); expm1 keeps the Rayleigh-Jeans limit accurate
    return std::pow(lambdaA, -5.0) / std::expm1(x);
}

double interpLinear(const std::vector<double> &x, const std::vector<double> &y, double xi)
{
    const size_t n = x.size();
    if (n == 0) return 0.0;
    if (xi <= x.front()) return y.front();
    if (xi >= x.back()) return y.back();
    const size_t k = size_t(std::upper_bound(x.begin(), x.end(), xi) - x.begin());   // x[k-1] <= xi < x[k]
    const double f = (xi - x[k - 1]) / (x[k] - x[k - 1]);
    return y[k - 1] + f * (y[k] - y[k - 1]);
}

bool loadTable(const std::string &file, Model &m, std::string &err)
{
    std::ifstream in(file);
    if (!in) {
        err = "Flux-ratio table " + file + " cannot be opened.";
        return false;
    }
    m.tabLambda.clear();
    m.tabRatio.clear();
    std::string line;
    int no = 0;
    while (std::getline(in, line)) {
        ++no;
        const size_t c = line.find('#');
        if (c != std::string::npos) line.erase(c);
        std::istringstream ls(line);
        double l, r;
        if (!(ls >> l)) continue;                     // blank or comment line
        if (!(ls >> r)) {
            err = "Flux-ratio table " + file + ", line " + std::to_string(no) + ": two columns expected.";
            return false;
        }
        if (!(r > 0) || !std::isfinite(r) || !std::isfinite(l)) {
            err = "Flux-ratio table " + file + ", line " + std::to_string(no) + ": ratio must be positive.";
            return false;
        }
        if (!m.tabLambda.empty() && !(l > m.tabLambda.back())) {
            err = "Flux-ratio table " + file + ", line " + std::to_string(no) + ": wavelengths must increase.";
            return false;
        }
        m.tabLambda.push_back(l);
        m.tabRatio.push_back(r);
    }
    if (m.tabLambda.size() < 2) {
        err = "Flux-ratio table " + file + " needs at least two rows.";
        return false;
    }
    return true;
}

double shape(const Model &m, double lambdaA, double lambdaRef)
{
    switch (m.mode) {
    case Mode::Planck:
        return (planck(lambdaA, m.tA) / planck(lambdaA, m.tB))
               / (planck(lambdaRef, m.tA) / planck(lambdaRef, m.tB));
    case Mode::Table:
        return interpLinear(m.tabLambda, m.tabRatio, lambdaA) / interpLinear(m.tabLambda, m.tabRatio, lambdaRef);
    case Mode::Constant:
    default:
        return 1.0;
    }
}

bool shapeOnGrid(const Model &m, const std::vector<double> &lambdaA, std::vector<double> &s,
                 double &lambdaRefUsed, std::string &err)
{
    s.assign(lambdaA.size(), 1.0);
    lambdaRefUsed = m.lambdaRef;
    if (m.mode == Mode::Constant)
        return true;
    if (lambdaA.empty()) {
        err = "No wavelengths for the flux-ratio model.";
        return false;
    }
    if (!(lambdaRefUsed > 0))
        lambdaRefUsed = std::sqrt(lambdaA.front() * lambdaA.back());   // centre in log(lambda)
    if (m.mode == Mode::Planck && !(m.tA > 0 && m.tB > 0)) {
        err = "Flux-ratio model Planck: both temperatures must be positive.";
        return false;
    }
    if (m.mode == Mode::Table && m.tabLambda.size() < 2) {
        err = "Flux-ratio model table: no table loaded.";
        return false;
    }
    for (size_t i = 0; i < lambdaA.size(); ++i) {
        s[i] = shape(m, lambdaA[i], lambdaRefUsed);
        if (!(s[i] > 0) || !std::isfinite(s[i])) {
            err = "Flux-ratio model gives an invalid value at " + std::to_string(lambdaA[i]) + " A.";
            return false;
        }
    }
    return true;
}

void normaliseToContinuum(const std::vector<double> &lamA, std::vector<double> &A,
                          const std::vector<double> &cA,
                          const std::vector<double> &lamB, std::vector<double> &B,
                          const std::vector<double> &ratioAtB)
{
    std::vector<double> dA(lamA.size());
    for (size_t i = 0; i < lamA.size(); ++i) {
        dA[i] = 1.0 - cA[i];
        A[i] += dA[i];
    }
    for (size_t i = 0; i < lamB.size(); ++i)
        B[i] -= ratioAtB[i] * interpLinear(lamA, dA, lamB[i]);
}

} // namespace fr
