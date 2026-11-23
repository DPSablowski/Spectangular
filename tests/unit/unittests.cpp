#include <algorithm>
#include <cstdlib>
// Unit tests of the GUI-free modules: orbit, dataio, disentangle, neldermead.
// Output: one line "PASS|FAIL  name: detail" per check; exit code 1 on FAIL.

#include "dataio.h"
#include "errorestimate.h"
#include "levmar.h"
#include "disentangle.h"
#include "fluxratio.h"
#include "rvfit.h"
#include "templatefit.h"
#include "continuum.h"
#include "barycorr.h"
#include <stdexcept>
#include "neldermead.h"
#include "orbit.h"

#include <armadillo>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

namespace {

int g_fail = 0;

void check(const std::string &name, bool ok, const std::string &detail = "")
{
    std::cout << (ok ? "PASS" : "FAIL") << "  " << name << (detail.empty() ? "" : ": " + detail)
              << "\n";
    if (!ok)
        ++g_fail;
}

std::string fmt(double v)
{
    std::ostringstream s;
    s.precision(3);
    s << std::scientific << v;
    return s.str();
}

std::string tmpFile(const std::string &content)
{
    char name[] = "/tmp/spectest_unitXXXXXX";
    int fd = mkstemp(name);
    close(fd);
    std::ofstream(name) << content;
    return name;
}

// ------------------------------------------------------------------ orbit

void testOrbit()
{
    double worst = 0;
    bool allConverged = true;
    for (double e = 0.0; e < 0.995; e += 0.01) {
        for (double M = -10.0; M <= 10.0; M += 0.37) {
            bool ok;
            double E = orbit::eccentricAnomaly(M, e, &ok);
            double Mr = std::remainder(M, 2 * M_PI);
            worst = std::max(worst, std::fabs(E - e * std::sin(E) - Mr));
            allConverged = allConverged && ok;
        }
    }
    check("Kepler equation satisfied for e = 0 ... 0.99", worst < 1e-12 && allConverged,
          "max. error " + fmt(worst));

    // Circular orbit: RV = gamma + K cos(2 pi (t - T)/P + omega)
    double dmax = 0;
    for (double t = 0; t < 20; t += 0.7) {
        double v = orbit::radialVelocity(t, 5.0, 0.0, 30.0, -4.0, 1.0, 0.3);
        dmax = std::max(dmax, std::fabs(v - (-4.0 + 30.0 * std::cos(2 * M_PI * (t - 1.0) / 5.0 + 0.3))));
    }
    check("Circular orbit analytic", dmax < 1e-12, fmt(dmax));

    // Periastron: true anomaly 0, RV = gamma + K (1 + e) cos omega
    double v = orbit::radialVelocity(3.0, 7.0, 0.6, 20.0, 1.0, 3.0, 0.4);
    check("RV at periastron",std::fabs(v - (1.0 + 20.0 * 1.6 * std::cos(0.4))) < 1e-12);

    check("Validity of the elements",
          orbit::elementsValid(1, 0) && orbit::elementsValid(1, 0.99) && !orbit::elementsValid(1, 1.0)
              && !orbit::elementsValid(1, -0.01) && !orbit::elementsValid(0, 0.1)
              && !orbit::elementsValid(-1, 0.1) && !orbit::elementsValid(NAN, 0.1));
}

// ----------------------------------------------------------------- dataio

void testDataio()
{
    std::vector<std::vector<double>> rows;
    std::string err;
    std::string f = tmpFile("# header\n\n1 2 3\n  4\t5\n\n# end\n   \n");
    bool ok = dataio::readTable(f, 2, rows, err);
    check("Table with comments and blank lines", ok && rows.size() == 2 && rows[1][1] == 5.0,
          ok ? "" : err);
    ok = dataio::readTable(f, 3, rows, err);
    check("Too few columns are reported",!ok && err.find("line 4") != std::string::npos, err);
    std::remove(f.c_str());

    f = tmpFile("1 2\nabc 3\n");
    ok = dataio::readTable(f, 2, rows, err);
    check("Unreadable line is reported",!ok && err.find("line 2") != std::string::npos, err);
    std::remove(f.c_str());

    ok = dataio::readTable("/nonexistent/x.txt", 1, rows, err);
    check("Missing file is reported", !ok);

    std::vector<std::vector<double>> w(2);
    for (int i = 0; i < 50; ++i) {
        w[0].push_back(3.8 + i * 1e-5);
        w[1].push_back(3.8 + i * 1e-5);
    }
    check("Identical grid is accepted", dataio::checkCommonGrid(w, err), err);
    w[1].pop_back();
    check("Different length is detected", !dataio::checkCommonGrid(w, err), err);
    w[1].push_back(3.8 + 49e-5);
    w[0][20] += 0.5e-5;
    check("Non-equidistant grid is detected",!dataio::checkCommonGrid(w, err), err);

    // Wavelengths stored as float32 (FITS): rounding up to ~9 % of a step per
    // pixel, but position correct to < 0.1 pixel -> accept
    std::vector<std::vector<double>> wf(2);
    for (int i = 0; i < 1417; ++i) {
        double x = 3.8258238909452 + i * 2.4131314353104e-6;
        wf[0].push_back(double(float(x)));
        wf[1].push_back(double(float(x)));
    }
    check("float32-rounded grid is accepted",dataio::checkCommonGrid(wf, err), err);

    // Linear lambda grid (same length, 6695..6745 A) in log10: not equidistant
    std::vector<std::vector<double>> wl(1);
    for (int i = 0; i < 1417; ++i)
        wl[0].push_back(std::log10(6695.0 + i * 50.0 / 1416));
    check("Linear lambda grid is detected",!dataio::checkCommonGrid(wl, err), err);

    // Same linear Angstrom grid for all spectra (equidistant in lambda): must not pass as
    // a log grid, otherwise the Angstrom values would be used as log10(lambda)
    std::vector<std::vector<double>> wa(3);
    for (auto &s : wa)
        for (int i = 0; i < 1060; ++i)
            s.push_back(6696.0 + i * 0.05);
    check("Common linear Angstrom grid is detected", !dataio::checkCommonGrid(wa, err), err);

    // Resampling to a common log grid
    {
        std::vector<double> x, y, xi;
        for (int i = 0; i < 20; ++i) { x.push_back(i * 0.5 + 0.03 * std::sin(i)); y.push_back(2.0 - 0.3 * x.back()); }
        for (int j = 0; j < 30; ++j) xi.push_back(0.1 + j * 0.3);
        std::vector<double> yi = dataio::splineInterpolate(x, y, xi);
        double d = 0.0;
        for (size_t j = 0; j < xi.size(); ++j) d = std::max(d, std::fabs(yi[j] - (2.0 - 0.3 * xi[j])));
        check("Spline reproduces a straight line", d < 1e-12, fmt(d));
        // Gaussian line on linear Angstrom grids with different starts, lengths and steps
        std::vector<std::vector<double>> wv(3), fl(3);
        auto line = [](double lam) { return 1.0 - 0.4 * std::exp(-0.5 * std::pow((lam - 6360.0) / 0.5, 2)); };
        const double start[3] = {6330.00, 6331.07, 6329.52}, step[3] = {0.137, 0.121, 0.242};
        const int npx[3] = {420, 470, 240};
        for (int a = 0; a < 3; ++a)
            for (int i = 0; i < npx[a]; ++i) {
                const double lam = std::round((start[a] + i * step[a]) * 100.0) / 100.0;
                wv[a].push_back(lam);
                fl[a].push_back(line(lam));
            }
        dataio::GridInfo gi;
        bool ok = dataio::resampleToLogGrid(wv, fl, 0.0, gi, err);
        const double lo = std::log10(6331.07), hi = std::log10(std::min({6330.00 + 419 * 0.137, 6331.07 + 469 * 0.121, 6329.52 + 239 * 0.242}));
        check("Resampling: all spectra Angstrom, common range, finest step",
              ok && gi.linearInputs == 3 && std::fabs(gi.w0 - lo) < 1e-6 && gi.w0 + (gi.n - 1) * gi.step <= hi + 1e-12
              && std::fabs(gi.step - std::log10(1.0 + 0.12 / 6360.0)) < 0.1 * gi.step,
              ok ? std::to_string(gi.n) + " pixels, step " + fmt(gi.step) : err);
        double dev = 0.0;
        for (int a = 0; ok && a < 3; ++a)
            for (int i = 0; i < gi.n; ++i)
                dev = std::max(dev, std::fabs(fl[a][i] - line(std::pow(10.0, wv[a][i]))));
        check("Resampled line profile matches (well-sampled spectra < 2e-3)", ok && dev < 2e-3, fmt(dev));
        std::vector<std::vector<double>> bw = {{1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10}}, bf = {std::vector<double>(11, 1.0)};
        check("Resampling: non-increasing wavelengths are reported",
              !dataio::resampleToLogGrid(bw, bf, 0.0, gi, err) && err.find("do not increase") != std::string::npos, err);
        std::vector<std::vector<double>> nw = {{}, {}}, nf = {{}, {}};
        for (int i = 0; i < 20; ++i) { nw[0].push_back(6000 + i); nw[1].push_back(6100 + i); nf[0].push_back(1); nf[1].push_back(1); }
        check("Resampling: no common range is reported",
              !dataio::resampleToLogGrid(nw, nf, 0.0, gi, err) && err.find("common") != std::string::npos, err);
    }
}

// ------------------------------------------------------------- disentangle

void testDisentangle()
{
    const double dlog = 2e-5;
    dis::MatrixSpec s;
    s.num = 4;
    s.logbin = 60;
    s.dlog = dlog;
    s.sb2 = true;
    s.telluric = true;
    const double shiftsA[4] = {0.0, 2.0, -3.0, 1.0};
    const double shiftsB[4] = {1.5, -2.25, 3.7, 0.0};
    for (int a = 0; a < 4; ++a) {
        s.rvA.push_back(dis::kSpeedOfLight * (std::pow(10.0, shiftsA[a] * dlog) - 1));
        s.rvB.push_back(dis::kSpeedOfLight * (std::pow(10.0, shiftsB[a] * dlog) - 1));
        s.wA.push_back(0.6);
        s.wB.push_back(0.4);
        s.wT.push_back(1.0);
    }
    check("pixelShift is the inverse of v = c(10^(s dlog) - 1)",
          std::fabs(dis::pixelShift(s.rvB[2], dlog) - 3.7) < 1e-9);

    arma::sp_mat M;
    dis::Layout L = dis::buildMatrix(s, M);
    const arma::mat Md(M);
    check("Column count A: 60 + span of the shifts", L.nA == 60 + 5, std::to_string(L.nA));
    check("Column count B with fractional shifts", L.nB == 60 + 6 + 1, std::to_string(L.nB));
    check("Telluric columns", L.nT == 60);
    arma::rowvec colsum = arma::sum(arma::abs(Md), 0);
    check("No empty columns", arma::all(colsum > 0));
    arma::vec rowsum = arma::sum(Md, 1);
    check("Row sum = sum of the weights",arma::abs(rowsum - 2.0).max() < 1e-12);
    // Positive shift = redshift: rest position x0A + k lies at
    // observed pixel x0A + k + s.
    check("Axis A starts at the smallest rest position", L.x0A == -2, std::to_string(L.x0A));
    check("Column wavelength",
          std::fabs(dis::columnLog10Lambda(L, 'A', 5, 3.8, dlog) - (3.8 + 3 * dlog)) < 1e-15);

    // Exactly solvable system: random components, C = M X
    arma::arma_rng::set_seed(7);
    arma::vec Xtrue(L.cols(), arma::fill::randu);
    arma::vec C = M * Xtrue, X;
    dis::SolveResult r = dis::solveSvdDc(M, C, X);
    check("SVD (DaC) reproduces C", r.ok && arma::norm(M * X - C) < 1e-10, fmt(arma::norm(M * X - C)));
    arma::vec Xp = arma::pinv(Md, r.threshold) * C;
    check("SVD (DaC) = pinv solution", arma::abs(X - Xp).max() < 1e-9, fmt(arma::abs(X - Xp).max()));
    check("Matrix rank-deficient (test of the null space)", r.rank < int(L.cols()),
          std::to_string(r.rank) + " of " + std::to_string(L.cols()));
    dis::SolveResult rl = dis::solveLsqr(M, C, X);
    check("LSQR = pinv solution (minimum norm)", rl.ok && rl.message.empty() && arma::abs(X - Xp).max() < 1e-8,
          fmt(arma::abs(X - Xp).max()) + ", " + std::to_string(rl.iterations) + " iterations");

    // Not exactly solvable (noise): least-squares solutions must agree
    arma::vec Cn = C + 1e-2 * arma::randn<arma::vec>(C.n_elem);
    arma::vec Xs, Xl;
    dis::SolveResult rs = dis::solveSvdDc(M, Cn, Xs);
    dis::SolveResult rl2 = dis::solveLsqr(M, Cn, Xl);
    double dr = std::fabs(arma::norm(M * Xs - Cn) - arma::norm(M * Xl - Cn));
    check("LSQR and SVD: same residual with noise", rs.ok && rl2.ok && dr < 1e-12, fmt(dr));
    check("LSQR and SVD: same solution with noise",arma::abs(Xs - Xl).max() < 1e-8,
          fmt(arma::abs(Xs - Xl).max()));
    arma::vec Xz;
    dis::SolveResult r0 = dis::solveLsqr(M, arma::vec(C.n_elem, arma::fill::zeros), Xz);
    check("LSQR with C = 0",r0.ok && arma::norm(Xz) == 0.0);

    // Multiple cores: same solution (up to rounding from summation order)
    const int before = dis::threads();
    arma::vec X1, X4;
    dis::setThreads(1);
    dis::SolveResult t1 = dis::solveLsqr(M, Cn, X1);
    dis::setThreads(4);
    dis::SolveResult t4 = dis::solveLsqr(M, Cn, X4);
    dis::setThreads(before);
    check("LSQR with 1 and 4 cores identical", t1.ok && t4.ok && arma::abs(X1 - X4).max() < 1e-11
          && std::abs(t1.iterations - t4.iterations) <= std::max(2, t1.iterations / 20),
          fmt(arma::abs(X1 - X4).max()) + ", " + std::to_string(t1.iterations) + "/" + std::to_string(t4.iterations) + " iterations");
    check("Number of cores adjustable",dis::threads() == before && before >= 1, std::to_string(before));

    // Large enough for 4 threads (4 x kRowsPerThread rows): same solution
    {
        dis::MatrixSpec big;
        big.num = 20;
        big.logbin = int(4 * dis::kRowsPerThread / 20) + 100;
        big.dlog = 2.4e-6;
        arma::arma_rng::set_seed(11);
        for (int a = 0; a < big.num; ++a) {
            big.rvA.push_back(30 + 25 * std::sin(0.3 * a));
            big.rvB.push_back(30 - 28 * std::sin(0.3 * a));
            big.wA.push_back(0.5); big.wB.push_back(0.5); big.wT.push_back(1.0);
        }
        arma::sp_mat Mb;
        dis::buildMatrix(big, Mb);
        arma::vec Cb = 1.0 + 0.01 * arma::randn<arma::vec>(Mb.n_rows), Xb1, Xb4;
        dis::setThreads(1);
        dis::SolveResult b1 = dis::solveLsqr(Mb, Cb, Xb1);
        dis::setThreads(4);
        dis::SolveResult b4 = dis::solveLsqr(Mb, Cb, Xb4);
        dis::setThreads(before);
        double dx = arma::abs(Xb1 - Xb4).max();
        check("LSQR parallel: threads actually running (OpenMP " + std::string(dis::openmpAvailable() ? "on" : "off") + ")",
              dis::openmpAvailable() ? (b4.threads == 4 && b1.threads == 1) : b4.threads == 1,
              std::to_string(b1.threads) + " / " + std::to_string(b4.threads) + " threads with 1 / 4 cores");
        check("LSQR parallel (" + std::to_string(Mb.n_rows) + " rows) same as single-core",
              b1.ok && b4.ok && dx < 1e-9, fmt(dx) + ", " + std::to_string(b1.iterations) + "/" + std::to_string(b4.iterations) + " iterations");

        // threads by size: one per kRowsPerThread rows, at most the chosen number
        dis::setThreads(14);
        const int t0 = dis::lsqrThreads(20000), t1 = dis::lsqrThreads(96660), t2 = dis::lsqrThreads(5000000);
        dis::setThreads(before);
        check("LSQR threads by size (14 cores): 20 000 / 96 660 / 5 000 000 rows",
              t0 == 1 && t1 == 3 && t2 == 14, std::to_string(t0) + " / " + std::to_string(t1) + " / " + std::to_string(t2));

        // progress hook: called during a long solve, can abort it; without abort same solution
        int calls = 0;
        dis::setProgressHook([&]{ ++calls; return false; });
        arma::vec Xh;
        dis::SolveResult h = dis::solveLsqr(Mb, Cb, Xh, 1e-14, 0);
        const int callsFull = calls;
        const double dh = arma::abs(Xh - Xb1).max();
        int callsAbort = 0;
        dis::setProgressHook([&]{ ++callsAbort; return true; });
        arma::vec Xa;
        dis::SolveResult a = dis::solveLsqr(Mb, Cb, Xa, 1e-14, 0);
        dis::setProgressHook(nullptr);
        check("LSQR progress hook: no effect on the solution, abort ends the solve",
              h.ok && !h.aborted && dh < 1e-9 && callsAbort <= 1
              && (callsAbort == 0 || (a.aborted && !a.ok && a.iterations < h.iterations)),
              std::to_string(callsFull) + " calls; aborted after " + std::to_string(a.iterations) + " of "
              + std::to_string(h.iterations) + " iterations" + (callsAbort == 0 ? " (solve under 0.1 s, no call)" : ""));
    }
}


// Warm start: start from the previous solution (other RVs, other layout) gives the same
// minimum-norm solution as a cold start, in fewer iterations
void testWarmStart()
{
    const double dlog = 2e-5;
    auto spec = [&](double phase, bool tel, bool varW, bool cubic = false) {
        dis::MatrixSpec s;
        s.num = 12; s.logbin = 400; s.dlog = dlog; s.sb2 = true; s.telluric = tel;
        if (cubic) s.interp = dis::Interpolation::Cubic;
        for (int a = 0; a < s.num; ++a) {
            const double sa = 9.0 * std::sin(0.55 * a + phase), sb = -11.0 * std::sin(0.55 * a + phase);
            s.rvA.push_back(dis::kSpeedOfLight * (std::pow(10.0, sa * dlog) - 1));
            s.rvB.push_back(dis::kSpeedOfLight * (std::pow(10.0, sb * dlog) - 1));
            const double wa = varW ? 0.5 + 0.2 * std::cos(0.9 * a) : 0.6;
            s.wA.push_back(wa); s.wB.push_back(1.0 - wa);
            s.wT.push_back(tel && varW ? 0.7 + 0.3 * std::sin(1.3 * a) : 1.0);
        }
        return s;
    };
    // observations: two line spectra (Gaussians) shifted with the "true" RVs (phase 0) plus noise
    auto observe = [&](const dis::MatrixSpec &s, unsigned seed) {
        arma::arma_rng::set_seed(seed);
        arma::vec C(s.num * s.logbin);
        for (int a = 0; a < s.num; ++a)
            for (int i = 0; i < s.logbin; ++i) {
                const double xa = i - dis::pixelShift(s.rvA[a], dlog), xb = i - dis::pixelShift(s.rvB[a], dlog);
                double f = s.wA[a] * (1 - 0.4 * std::exp(-0.5 * std::pow((xa - 150) / 4.0, 2))
                                        - 0.2 * std::exp(-0.5 * std::pow((xa - 260) / 6.0, 2)))
                         + s.wB[a] * (1 - 0.3 * std::exp(-0.5 * std::pow((xb - 200) / 3.0, 2)));
                if (s.telluric) f *= 1.0 - 0.1 * s.wT[a] * std::exp(-0.5 * std::pow((i - 320) / 1.5, 2));
                C(a * s.logbin + i) = f;
            }
        return arma::vec(C + 0.01 * arma::randn<arma::vec>(C.n_elem));
    };
    // tolX: the ramp direction with static lines is only nearly null (relative singular value
    // ~1e-10); along it neither LSQR solution is determined beyond ~1e-6; the residual is identical
    struct Case { const char *name; bool tel, varW; int nullDim; double tolX; bool cubic; };
    const Case cases[] = {{"SB2", false, false, 1, 1e-7, false}, {"SB2 + telluric", true, false, 3, 1e-5, false},
                          {"varying light ratios", true, true, 0, 1e-7, false},
                          {"SB2, cubic", false, false, 1, 1e-7, true},
                          {"SB2 + telluric, cubic", true, false, 3, 1e-5, true}};
    for (const Case &c : cases) {
        const dis::MatrixSpec truth = spec(0.0, c.tel, c.varW, c.cubic);
        const arma::vec C = observe(truth, 3);
        // previous evaluation (RVs slightly off) and the current one (other layout)
        dis::MatrixSpec s1 = spec(0.04, c.tel, c.varW, c.cubic), s2 = spec(-0.03, c.tel, c.varW, c.cubic);
        for (int a = 0; a < s2.num; ++a) { s1.rvA[a] *= 1.03; s2.rvB[a] *= 0.98; }
        arma::sp_mat M1, M2;
        dis::Layout L1 = dis::buildMatrix(s1, M1), L2 = dis::buildMatrix(s2, M2);
        arma::vec X1, Xc, Xw;
        dis::solveLsqr(M1, C, X1);
        dis::SolveResult rc = dis::solveLsqr(M2, C, Xc);
        const arma::vec x0 = dis::transferSolution(L1, X1, L2);
        dis::SolveResult rw = dis::solveLsqr(M2, C, Xw, 1e-12, 0, &x0);
        const int nd = dis::removeOffsetNullspace(L2, M2, Xw);
        const double dx = arma::abs(Xw - Xc).max();
        const double dr = std::fabs(arma::norm(M2 * Xw - C) - arma::norm(M2 * Xc - C)) / arma::norm(M2 * Xc - C);
        check(std::string("Warm start = cold start (") + c.name + ")",
              rc.ok && rw.ok && rw.message.empty() && dx < c.tolX && dr < 1e-12 && nd == c.nullDim,
              "max |dX| " + fmt(dx) + ", residual " + fmt(dr) + ", null directions " + std::to_string(nd)
              + ", layout " + std::to_string(L1.cols()) + " -> " + std::to_string(L2.cols()));
        // large RV change here (3 %): the start vector helps little, it must not cost much more
        check(std::string("Warm start needs no more iterations, +-10 % (") + c.name + ")",
              rw.iterations <= 1.1 * rc.iterations,
              std::to_string(rc.iterations) + " -> " + std::to_string(rw.iterations));
    }
    // start vector that already solves the system: returned unchanged, no iteration
    {
        dis::MatrixSpec s = spec(0.0, false, false);
        arma::sp_mat M;
        dis::Layout L = dis::buildMatrix(s, M);
        arma::arma_rng::set_seed(5);
        arma::vec Xt(L.cols(), arma::fill::randu), C = M * Xt, X;
        dis::solveLsqr(M, C, X, 1e-12, 0, &Xt);
        check("Warm start with exact start vector", arma::abs(X - Xt).max() < 1e-12);
    }
    // continuum levels of A and B (and static spectrum) along the offset null directions:
    // light ratio 0.6 / 0.4; the minimum-norm solution has A and B at different levels
    for (bool tel : {false, true}) {
        const dis::MatrixSpec s = spec(0.0, tel, false);
        arma::sp_mat M;
        const dis::Layout L = dis::buildMatrix(s, M);
        const arma::vec C = observe(s, 7);
        arma::vec X;
        dis::solveLsqr(M, C, X);
        auto levels = [&](const arma::vec &v, double &a, double &b, double &t) {
            a = dis::continuumLevel(arma::vec(v.subvec(20, L.nA - 21)));
            b = dis::continuumLevel(arma::vec(v.subvec(L.startB() + 20, L.startB() + L.nB - 21)));
            t = tel ? dis::continuumLevel(arma::vec(v.subvec(L.startT() + 20, L.startT() + L.nT - 21))) : 0.0;
        };
        double a0, b0, t0, a1, b1, t1;
        levels(X, a0, b0, t0);
        arma::vec Y = X;
        const bool ok = dis::levelComponents(L, M, dis::offsetNullspace(L, M), Y);
        levels(Y, a1, b1, t1);
        const double dr = std::fabs(arma::norm(M * Y - C) - arma::norm(M * X - C)) / arma::norm(M * X - C);
        check(std::string("Continuum levels of A and B at 1") + (tel ? " and static spectrum at 0" : "")
              + " (minimum norm: A " + fmt(a0) + ", B " + fmt(b0) + (tel ? ", T " + fmt(t0) : "") + ")",
              // with static lines the solution has smooth deviations of a few per cent (slowly
              // varying functions are nearly undetermined, noise is amplified there)
              ok && std::fabs(a1 - 1) < (tel ? 0.08 : 0.005) && std::fabs(b1 - 1) < (tel ? 0.08 : 0.005)
              && std::fabs(t1) < (tel ? 0.05 : 0.005)
              && std::fabs(a0 - b0) > 0.05 && dr < 1e-12,
              "A " + fmt(a1) + ", B " + fmt(b1) + (tel ? ", T " + fmt(t1) : "") + ", residual " + fmt(dr));
    }
    // light ratios varying between the spectra: no offset null direction, nothing to level
    {
        const dis::MatrixSpec s = spec(0.0, false, true);
        arma::sp_mat M;
        const dis::Layout L = dis::buildMatrix(s, M);
        arma::vec X(L.cols(), arma::fill::randu), Y = X;
        const arma::mat N = dis::offsetNullspace(L, M);
        check("No levelling without offset null direction", !dis::levelComponents(L, M, N, Y) && arma::approx_equal(X, Y, "absdiff", 0.0));
    }
    // continuum level: noise only and line-rich spectra (up to 95 % of the pixels in lines)
    {
        arma::arma_rng::set_seed(3);
        const int n = 4000;
        const arma::vec x = arma::linspace(0, 1, n);
        for (int nl : {0, 30, 150, 400}) {
            arma::vec v(n, arma::fill::ones);
            const arma::vec c = arma::randu(nl), dep = 0.05 + 0.5 * arma::randu(nl), w = 0.0005 + 0.002 * arma::randu(nl);
            for (int k = 0; k < nl; ++k) v -= dep(k) * arma::exp(-arma::square(x - c(k)) / (2 * w(k) * w(k)));
            const double sig = 0.01;
            const double lev = dis::continuumLevel(arma::vec(v + sig * arma::randn(n)));
            const double inLines = arma::mean(arma::conv_to<arma::vec>::from(v < 1 - sig));
            check("Continuum level, " + std::to_string(nl) + " lines (" + fmt(100 * inLines) + " % of the pixels in lines)",
                  std::fabs(lev - 1) < sig, fmt(lev));
        }
    }
}


// Cubic interpolation (Catmull-Rom): partition of unity, reproduces quadratics, smooth in the shift
void testCubic()
{
    const double dlog = 2e-5;
    auto spec = [&](double extra) {
        dis::MatrixSpec s;
        s.num = 5; s.logbin = 80; s.dlog = dlog; s.sb2 = true; s.telluric = false;
        s.interp = dis::Interpolation::Cubic;
        const double sa[5] = {0.0, 2.3, -3.6, 1.5, 4.0}, sb[5] = {1.25, -2.0, 3.7, 0.0, -1.1};
        for (int a = 0; a < 5; ++a) {
            s.rvA.push_back(dis::kSpeedOfLight * (std::pow(10.0, (sa[a] + (a == 1 ? extra : 0.0)) * dlog) - 1));
            s.rvB.push_back(dis::kSpeedOfLight * (std::pow(10.0, sb[a] * dlog) - 1));
            s.wA.push_back(0.6); s.wB.push_back(0.4); s.wT.push_back(1.0);
        }
        return s;
    };
    arma::sp_mat M;
    dis::MatrixSpec s = spec(0.0);
    dis::Layout L = dis::buildMatrix(s, M);
    const arma::mat Md(M);
    arma::rowvec colsum = arma::sum(arma::abs(Md), 0);
    check("Cubic: no empty columns", arma::all(colsum > 0));
    check("Cubic: row sum = sum of the weights", arma::abs(arma::sum(Md, 1) - 1.0).max() < 1e-12);
    // A(x) = quadratic in the rest pixel; the model must give it at x = i - s exactly
    arma::vec Xq(L.cols(), arma::fill::zeros);
    auto q = [](double x) { return 0.3 + 0.01 * x - 0.002 * x * x; };
    for (int k = 0; k < L.nA; ++k) Xq(k) = q(L.x0A + k);
    arma::vec y = M * Xq;
    double dq = 0.0;
    for (int a = 0; a < s.num; ++a)
        for (int i = 0; i < s.logbin; ++i)
            dq = std::max(dq, std::fabs(y(a * s.logbin + i) - 0.6 * q(i - dis::pixelShift(s.rvA[a], dlog))));
    check("Cubic reproduces quadratics", dq < 1e-12, fmt(dq));
    // derivative of the model with respect to the shift is continuous at a whole-pixel shift
    // (linear: jumps). Spectrum 1 of A has shift 2.3; move it through 3.0.
    auto model = [&](double extra, dis::Interpolation ip) {
        dis::MatrixSpec t = spec(extra);
        t.interp = ip;
        arma::sp_mat Mt;
        dis::Layout Lt = dis::buildMatrix(t, Mt);
        arma::vec Xs(Lt.cols(), arma::fill::zeros);
        for (int k = 0; k < Lt.nA; ++k) Xs(k) = std::sin(0.7 * (Lt.x0A + k));   // non-polynomial
        arma::vec yy = Mt * Xs;
        return arma::vec(yy.subvec(s.logbin, 2 * s.logbin - 1));
    };
    const double e = 1e-6;
    for (auto ip : {dis::Interpolation::Linear, dis::Interpolation::Cubic}) {
        arma::vec dl = (model(0.7, ip) - model(0.7 - e, ip)) / e;   // left of 3.0
        arma::vec dr = (model(0.7 + e, ip) - model(0.7, ip)) / e;   // right of 3.0
        const double jump = arma::abs(dl - dr).max();
        if (ip == dis::Interpolation::Cubic)
            check("Cubic: model smooth in the shift at whole pixels", jump < 1e-4, fmt(jump));
        else
            check("Linear: kink at whole pixels (reference)", jump > 1e-2, fmt(jump));
    }
}

// Levenberg-Marquardt: Rosenbrock as residuals; exponential fit with known covariance
void testLevmar()
{
    {
        lm::Options o;
        o.h = arma::vec{1e-7, 1e-7};
        o.maxIter = 200;
        o.relTolS = 0.0;
        o.tolX = 1e-6;
        auto f = [](const arma::vec &x, arma::vec &r, bool) {
            r = arma::vec{10.0 * (x(1) - x(0) * x(0)), 1.0 - x(0)};
            return true;
        };
        lm::Result r = lm::minimize(arma::vec{-1.2, 1.0}, f, o);
        const double d = arma::norm(r.x - arma::vec{1.0, 1.0});
        check("LM: Rosenbrock minimum (1, 1)", r.ok && d < 1e-5, fmt(d) + ", " + std::to_string(r.iterations) + " steps");
    }
    {
        // y = a exp(-b t) + noise; covariance compared with sigma^2 (J^T J)^-1 from the analytic J
        arma::arma_rng::set_seed(3);
        const int m = 50;
        arma::vec t = arma::linspace(0, 4, m), yv = 2.0 * arma::exp(-0.7 * t) + 0.01 * arma::randn<arma::vec>(m);
        lm::Options o;
        o.h = arma::vec{1e-6, 1e-6};
        o.tolS = 1e-20;
        o.secant = false;      // curvature = J^T J (small-residual problem)
        auto f = [&](const arma::vec &x, arma::vec &r, bool) {
            r = x(0) * arma::exp(-x(1) * t) - yv;
            return true;
        };
        int bad = 0;
        auto g = [&](const arma::vec &x, arma::vec &r, bool trial) {
            if (x(1) < 0) { ++bad; return false; }      // inadmissible region
            return f(x, r, trial);
        };
        lm::Result r = lm::minimize(arma::vec{1.0, 0.1}, g, o);
        arma::mat J(m, 2);
        J.col(0) = arma::exp(-r.x(1) * t);
        J.col(1) = -r.x(0) * t % arma::exp(-r.x(1) * t);
        // reference minimum: Gauss-Newton to convergence
        arma::vec xr = r.x;
        for (int k = 0; k < 20; ++k) {
            arma::vec rr; f(xr, rr, true);
            arma::mat Jr(m, 2);
            Jr.col(0) = arma::exp(-xr(1) * t);
            Jr.col(1) = -xr(0) * t % arma::exp(-xr(1) * t);
            xr -= arma::solve(Jr.t() * Jr, Jr.t() * rr);
        }
        arma::mat cA = lm::covariance(J.t() * J, 1e-4), cN = lm::covariance(r.JtJ, 1e-4);
        const double dc = arma::abs(cA - cN).max() / arma::abs(cA).max();
        check("LM: exponential fit at the least-squares minimum", r.converged && arma::abs(r.x - xr).max() < 1e-7,
              fmt(arma::abs(r.x - xr).max()) + ", " + std::to_string(r.iterations) + " steps");
        check("LM: covariance sigma^2 (J^T J)^-1 from the difference Jacobian", dc < 1e-4, fmt(dc));
    }
    {
        // abort before the first step
        lm::Options o;
        o.h = arma::vec{1e-6};
        auto f = [](const arma::vec &x, arma::vec &r, bool) { r = x - 3.0; return true; };
        lm::Result r = lm::minimize(arma::vec{0.0}, f, o, [] { return true; });
        check("LM: abort", r.aborted && r.ok && r.x(0) == 0.0);
    }
}

// --------------------------------------------------------------- neldermead

void testNelderMead()
{
    std::vector<double> y = {1, 1, 1};
    int h, sh, l;
    nm::order(y, h, sh, l);
    check("order: sh != h on a tie", sh != h);
    y = {3, 1, 2, 5, 4};
    nm::order(y, h, sh, l);
    check("order: indices",h == 3 && sh == 4 && l == 1);

    nm::Coefficients c;
    auto noAbort = [] { return false; };

    // Quadric in 4 dimensions
    {
        auto f = [](const arma::rowvec &x) {
            double s = 0;
            for (arma::uword i = 0; i < x.n_elem; ++i) s += (i + 1.0) * std::pow(x(i) - double(i), 2);
            return s;
        };
        arma::mat P(5, 4, arma::fill::zeros);
        for (int i = 0; i < 4; ++i) P(i + 1, i) = 1.0;
        std::vector<double> yv(5);
        for (int i = 0; i < 5; ++i) yv[i] = f(P.row(i));
        bool monotone = true;
        double best = *std::min_element(yv.begin(), yv.end());
        for (int it = 0; it < 600; ++it) {
            nm::step(P, yv, c, f, noAbort);
            double b = *std::min_element(yv.begin(), yv.end());
            if (b > best + 1e-15) monotone = false;
            best = b;
        }
        check("Quadric: minimum found", best < 1e-10, fmt(best));
        check("Best value never rises", monotone);
    }

    // Rosenbrock 2D
    {
        auto f = [](const arma::rowvec &x) {
            return 100 * std::pow(x(1) - x(0) * x(0), 2) + std::pow(1 - x(0), 2);
        };
        arma::mat P = {{-1.2, 1.0}, {-1.0, 1.0}, {-1.2, 1.2}};
        std::vector<double> yv(3);
        for (int i = 0; i < 3; ++i) yv[i] = f(P.row(i));
        for (int it = 0; it < 800; ++it) nm::step(P, yv, c, f, noAbort);
        nm::order(yv, h, sh, l);
        double d = std::hypot(P(l, 0) - 1, P(l, 1) - 1);
        check("Rosenbrock: minimum (1, 1)", d < 1e-4, "distance " + fmt(d));
    }

    // Shrinking does not re-evaluate the best point
    {
        int calls = 0;
        // Function with a spike in the interior forces shrinking
        auto f = [&](const arma::rowvec &x) {
            ++calls;
            double r2 = arma::dot(x, x);
            return r2 < 0.2 ? 10.0 : r2;
        };
        arma::mat P = {{1.0, 0.0}, {0.0, 1.0}, {-0.7, -0.7}};
        std::vector<double> yv(3);
        for (int i = 0; i < 3; ++i) yv[i] = f(P.row(i));
        calls = 0;
        nm::StepResult r = nm::step(P, yv, c, f, noAbort);
        bool shrunk = r.log.find("total contraction") != std::string::npos;
        check("Shrinking: n instead of n+1 evaluations",shrunk && r.evaluations == 2 + 2, r.log);
    }

    // Abort after the first evaluation
    {
        auto f = [](const arma::rowvec &x) { return arma::dot(x, x); };
        arma::mat P = {{1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}};
        std::vector<double> yv = {1, 1, 2};
        nm::StepResult r = nm::step(P, yv, c, f, [] { return true; });
        check("Abort after the first evaluation",r.aborted && r.evaluations == 1);
    }
}

// --------------------------------------------------------- Stopping criterion

void testProgressMonitor()
{
    // S^2 falls fast at first, then slowly: 1/t; converged when the decrease over 50 < 0.01
    nm::ProgressMonitor m(50);
    int stopAt = -1;
    for (int t = 1; t <= 10000 && stopAt < 0; ++t) {
        m.add(100.0 + 1.0 / t);
        if (m.converged(0.01)) stopAt = t;
    }
    // 1/(t-50) - 1/t < 0.01  <=>  t(t-50) > 5000  <=>  t > 100 (at 100 exactly on the boundary)
    check("Progress: stop as soon as the decrease over the window < tol", (stopAt == 100 || stopAt == 101), std::to_string(stopAt));
    nm::ProgressMonitor m2(10);
    for (int t = 0; t < 10; ++t) m2.add(5.0);
    check("Progress: no verdict before the window is full", !m2.converged(1.0) && !m2.ready());
    m2.add(5.0);
    check("Progress: constant S^2 converges", m2.converged(1e-12));
    // Rounding noise (improvements ~1e-13) does not count as progress
    nm::ProgressMonitor m3(20);
    double v = 7.0;
    bool any = false;
    for (int t = 0; t < 100; ++t) {
        v -= 1e-13;
        m3.add(v);
        any = any || m3.converged(1e-6);
    }
    check("Progress: rounding-level improvements count as converged", any);
    check("Window: 10 per parameter, at least 200",nm::defaultWindow(6) == 200 && nm::defaultWindow(40) == 400);
}

// ------------------------------------------------------------- errorestimate

void testErrorEstimate()
{
    // S(t) = 3 + (t - t0)^T A (t - t0): covariance (sigma^2 = 1) = A^-1, minimum t0
    arma::mat A = {{4.0, 1.0, 0.5}, {1.0, 3.0, -0.4}, {0.5, -0.4, 2.0}};
    arma::rowvec t0 = {1.0, -2.0, 0.5};
    auto f = [&](const arma::rowvec &t) {
        arma::rowvec d = t - t0;
        return 3.0 + arma::as_scalar(d * A * d.t());
    };
    arma::arma_rng::set_seed(3);
    arma::mat P = arma::repmat(t0 + 0.2, 4, 1) + 0.3 * arma::randu<arma::mat>(4, 3);
    std::vector<double> S(4);
    for (int i = 0; i < 4; ++i) S[i] = f(P.row(i));
    errest::Result r = errest::quadraticFit(P, S, f, [] { return false; });
    double dc = r.ok ? arma::abs(r.covUnit - arma::inv(A)).max() : 1e9;
    double dm = r.ok ? arma::abs(r.minimum - t0).max() : 1e9;
    check("Error estimate: covariance of a quadric", r.ok && dc < 1e-10, r.ok ? fmt(dc) : r.message);
    check("Error estimate: minimum of a quadric", r.ok && dm < 1e-10, r.ok ? fmt(dm) : r.message);
    check("Error estimate: n(n+1)/2 evaluations",r.evaluations == 6, std::to_string(r.evaluations));

    // No minimum (saddle): is detected
    auto g = [](const arma::rowvec &t) { return t(0) * t(0) - t(1) * t(1); };
    arma::mat P2 = {{0.1, 0.2}, {0.5, 0.1}, {0.2, 0.6}};
    std::vector<double> S2(3);
    for (int i = 0; i < 3; ++i) S2[i] = g(P2.row(i));
    errest::Result r2 = errest::quadraticFit(P2, S2, g, [] { return false; });
    check("Error estimate: saddle point is reported",!r2.ok && !r2.message.empty(), r2.message);
}

// New simplex around the best point (final simplex is degenerate after full convergence)
void testProbeSimplex()
{
    // Quadric with correlations and very different scales, minimum t0
    arma::mat A = {{4.0e4, 1.0e2, 0.0}, {1.0e2, 3.0, -0.01}, {0.0, -0.01, 2.0e-4}};
    arma::rowvec t0 = {100.0, -2.0, 2448000.0};
    auto f = [&](const arma::rowvec &t) {
        arma::rowvec d = t - t0;
        return 3.0 + arma::as_scalar(d * A * d.t());
    };
    const double target = 1.0;
    // Start: step sizes far off (the final simplex has collapsed to 1e-12)
    arma::vec h0 = {1e-12, 1e-12, 1e-12};
    errest::ProbeResult p = errest::probeSimplex(t0, f(t0), h0, target, f, [] { return false; });
    bool sizeOk = p.ok && p.P.n_rows == 4 && p.P.n_cols == 3 && p.S.size() == 4;
    check("New simplex: built", sizeOk, p.message);
    if (!sizeOk)
        return;
    double worst = 0;
    for (int i = 1; i <= 3; ++i)
        worst = std::max(worst, std::abs(std::log(p.S[i] - p.S[0]) - std::log(target)));
    check("New simplex: Delta S per vertex close to target (factor 2)",worst < std::log(2.0), fmt(worst));
    errest::Result r = errest::quadraticFit(p.P, p.S, f, [] { return false; });
    double rel = r.ok ? arma::abs((r.covUnit - arma::inv(A)) / arma::sqrt(arma::diagvec(arma::inv(A))
                                  * arma::sqrt(arma::diagvec(arma::inv(A))).t())).max() : 1e9;
    check("New simplex: covariance of the quadric",r.ok && rel < 1e-6, r.ok ? fmt(rel) : r.message);

    // Best point not exactly at the minimum: covariance still correct
    // (distance of several standard deviations)
    arma::rowvec x1 = t0 + arma::rowvec{0.05, 3.0, 300.0};
    errest::ProbeResult p1 = errest::probeSimplex(x1, f(x1), h0, target, f, [] { return false; });
    errest::Result r1 = p1.ok ? errest::quadraticFit(p1.P, p1.S, f, [] { return false; }) : errest::Result();
    double dm = r1.ok ? arma::abs((r1.minimum - t0) / arma::sqrt(arma::diagvec(arma::inv(A))).t()).max() : 1e9;
    check("New simplex: minimum from an offset start",r1.ok && dm < 1e-6, r1.ok ? fmt(dm) : p1.message + r1.message);

    // Parameter without influence (like gamma in disentangling): is reported
    auto g = [](const arma::rowvec &t) { return 1.0 + t(0) * t(0); };
    errest::ProbeResult p2 = errest::probeSimplex(arma::rowvec{0.0, 5.0}, 1.0, arma::vec{0.1, 0.1}, 0.01, g,
                                                  [] { return false; });
    check("New simplex: undeterminable parameter is reported",
          !p2.ok && p2.undetermined.size() == 1 && p2.undetermined[0] == 1, p2.message);

    // Boundary of the allowed range (penalty value for e < 0): one-sided
    auto h = [](const arma::rowvec &t) { return t(0) < 0 ? 1e30 : 2.0 + 100.0 * (t(0) - 0.01) * (t(0) - 0.01); };
    errest::ProbeResult p3 = errest::probeSimplex(arma::rowvec{0.01}, 2.0, arma::vec{0.05}, 1e-3, h,
                                                  [] { return false; });
    check("New simplex: only valid points at the boundary",p3.ok && p3.S[1] < 1e29, p3.message);

    // Strongly correlated valley with small roughness (like the kinks of linear
    // interpolation): the second pass along the principal axes hits the
    // marginal errors, the first (axis-parallel) underestimates them.
    {
        const double rho = 0.995, s1 = 0.05, s2 = 0.04, sig2 = 1e-3;
        arma::mat Cv = {{s1 * s1, rho * s1 * s2}, {rho * s1 * s2, s2 * s2}};
        arma::mat H = arma::inv(Cv);                 // S = S0 + sig2 * d^T H d
        arma::rowvec c0 = {104.0, 2448180.0};
        auto q = [&](const arma::rowvec &t) {
            arma::rowvec d = t - c0;
            double rough = 0.03 * sig2 * std::sin(d(0) / 0.0011) * std::sin(d(1) / 0.0007);
            return 5.0 + sig2 * arma::as_scalar(d * H * d.t()) + rough;
        };
        arma::vec hq = {1e-12, 1e-12};
        errest::ProbeResult a1 = errest::probeSimplex(c0, q(c0), hq, sig2, q, [] { return false; });
        errest::Result f1 = a1.ok ? errest::quadraticFit(a1.P, a1.S, q, [] { return false; }) : errest::Result();
        errest::ProbeResult a2 = errest::probePrincipal(c0, q(c0), f1, sig2, q, [] { return false; });
        errest::Result f2 = a2.ok ? errest::quadraticFit(a2.P, a2.S, q, [] { return false; }) : errest::Result();
        auto relErr = [&](const errest::Result &fr) {
            if (!fr.ok) return 1e9;
            arma::vec e = arma::sqrt(sig2 * fr.covUnit.diag());
            return std::max(std::abs(e(0) / s1 - 1), std::abs(e(1) / s2 - 1));
        };
        double e1 = relErr(f1), e2 = relErr(f2);
        check("Principal axes: marginal errors in the correlated valley within 5 %", f2.ok && e2 < 0.05,
              "1st pass " + (f1.ok ? fmt(e1) : f1.message) + ", 2nd pass " +(f2.ok ? fmt(e2) : f2.message + a2.message));
        double rc = f2.ok ? std::abs(f2.covUnit(0, 1) / std::sqrt(f2.covUnit(0, 0) * f2.covUnit(1, 1)) - rho) : 1;
        check("Principal axes: correlation recovered",rc < 0.002, fmt(rc));
        // Exact quadric: second pass changes nothing
        errest::Result fe1 = errest::quadraticFit(p.P, p.S, f, [] { return false; });
        errest::ProbeResult pe = errest::probePrincipal(t0, f(t0), fe1, target, f, [] { return false; });
        errest::Result fe2 = pe.ok ? errest::quadraticFit(pe.P, pe.S, f, [] { return false; }) : errest::Result();
        double dce = fe2.ok ? arma::abs(fe2.covUnit - arma::inv(A)).max() / arma::abs(arma::inv(A)).max() : 1e9;
        check("Principal axes: exact quadric unchanged",fe2.ok && dce < 1e-6, fe2.ok ? fmt(dce) : pe.message + fe2.message);
    }

    // Abort
    int calls = 0;
    errest::ProbeResult p4 = errest::probeSimplex(t0, f(t0), h0, target, f, [&] { return ++calls >= 3; });
    check("New simplex: abort",p4.aborted && !p4.ok && p4.evaluations == 3, std::to_string(p4.evaluations));
}

} // namespace

void testFluxRatio()
{
    // Planck: ratio of two equal temperatures is 1 everywhere; Wien/Rayleigh-Jeans limits
    check("Planck: equal temperatures give s = 1",
          std::fabs(fr::planck(5000.0, 8000.0) / fr::planck(5000.0, 8000.0) - 1.0) < 1e-15);
    {
        fr::Model m;
        m.mode = fr::Mode::Planck;
        m.tA = 10000.0; m.tB = 6000.0;
        // independent formula: s = [(e^(c2/l TB) - 1)/(e^(c2/l TA) - 1)] / same at lref, x l^-5 cancels
        const double c2 = 1.438776877e8;
        auto P = [&](double l) { return (std::exp(c2 / (l * 6000.0)) - 1.0) / (std::exp(c2 / (l * 10000.0)) - 1.0); };
        const double s1 = fr::shape(m, 4000.0, 5500.0), s2 = fr::shape(m, 7000.0, 5500.0);
        check("Planck shape against the closed formula",
              std::fabs(s1 - P(4000.0) / P(5500.0)) < 1e-12 * s1 && std::fabs(s2 - P(7000.0) / P(5500.0)) < 1e-12 * s2,
              fmt(s1) + " " + fmt(s2));
        check("Planck shape: hotter A brighter in the blue", s1 > 1.0 && s2 < 1.0);
        check("Planck shape is 1 at the reference wavelength", std::fabs(fr::shape(m, 5500.0, 5500.0) - 1.0) < 1e-15);
        std::vector<double> lam = {4000.0, 5000.0, 6000.0}, sh;
        double lref = 0;
        std::string err;
        check("Shape on a grid, reference = centre in log lambda",
              fr::shapeOnGrid(m, lam, sh, lref, err) && std::fabs(lref - std::sqrt(4000.0 * 6000.0)) < 1e-9, err);
        m.tB = -1;
        check("Planck with invalid temperature is rejected", !fr::shapeOnGrid(m, lam, sh, lref, err), err);
    }
    {
        fr::Model m;
        m.mode = fr::Mode::Table;
        m.tabLambda = {4000.0, 5000.0, 7000.0};
        m.tabRatio = {2.0, 1.5, 1.0};
        check("Table: linear interpolation", std::fabs(fr::interpLinear(m.tabLambda, m.tabRatio, 6000.0) - 1.25) < 1e-15);
        check("Table: constant beyond the ends", fr::interpLinear(m.tabLambda, m.tabRatio, 3000.0) == 2.0
                                                 && fr::interpLinear(m.tabLambda, m.tabRatio, 8000.0) == 1.0);
        check("Table shape relative to the reference", std::fabs(fr::shape(m, 4000.0, 5000.0) - 2.0 / 1.5) < 1e-15);
        // file reading
        const std::string f = "/tmp/spectangular_ratio_test.dat";
        { std::ofstream o(f); o << "# lambda ratio\n4000 2.0\n\n5000 1.5  # comment\n7000 1.0\n"; }
        fr::Model t;
        std::string err;
        check("Table file is read", fr::loadTable(f, t, err) && t.tabLambda.size() == 3 && t.tabRatio[1] == 1.5, err);
        { std::ofstream o(f); o << "5000 1.5\n4000 2.0\n"; }
        check("Table with decreasing wavelengths is rejected", !fr::loadTable(f, t, err), err);
        { std::ofstream o(f); o << "4000 2.0\n5000 0\n"; }
        check("Table with ratio <= 0 is rejected", !fr::loadTable(f, t, err), err);
        std::remove(f.c_str());
    }
    {
        // weights: constant shape reproduces F_A/(F_A+F_B), sum = W
        double wA, wB;
        fr::weights(1.5, 1.0, 1.0, wA, wB);
        check("Weights for s = 1 equal k/(1+k), 1/(1+k)", std::fabs(wA - 0.6) < 1e-15 && std::fabs(wB - 0.4) < 1e-15);
        fr::weights(1.5, 2.0, 0.8, wA, wB);
        check("Weights add up to W", std::fabs(wA + wB - 0.8) < 1e-15 && std::fabs(wA / wB - 3.0) < 1e-12);
    }
    {
        // matrix with per-row weights equal to the per-spectrum weights: identical matrix;
        // with varying per-row weights the rows are scaled accordingly
        dis::MatrixSpec s;
        s.num = 4; s.logbin = 60; s.dlog = 2e-5; s.sb2 = true; s.telluric = true;
        s.interp = dis::Interpolation::Cubic;
        const double sa[4] = {0.0, 2.3, -3.6, 1.5}, sb[4] = {1.25, -2.0, 3.7, 0.0};
        for (int a = 0; a < 4; ++a) {
            s.rvA.push_back(dis::kSpeedOfLight * (std::pow(10.0, sa[a] * s.dlog) - 1));
            s.rvB.push_back(dis::kSpeedOfLight * (std::pow(10.0, sb[a] * s.dlog) - 1));
            s.wA.push_back(0.6 - 0.05 * a); s.wB.push_back(0.4 + 0.05 * a); s.wT.push_back(1.0);
        }
        arma::sp_mat M0, M1, M2;
        dis::Layout L0 = dis::buildMatrix(s, M0);
        dis::MatrixSpec r = s;
        for (int a = 0; a < 4; ++a)
            for (int i = 0; i < 60; ++i) { r.wArow.push_back(s.wA[a]); r.wBrow.push_back(s.wB[a]); }
        dis::Layout L1 = dis::buildMatrix(r, M1);
        check("Per-row weights equal to the per-spectrum weights: identical matrix",
              L1.cols() == L0.cols() && arma::accu(arma::abs(arma::mat(M1 - M0))) == 0.0);
        dis::MatrixSpec v = s;
        for (int a = 0; a < 4; ++a)
            for (int i = 0; i < 60; ++i) {
                double wA, wB;
                fr::weights(s.wA[a] / s.wB[a], 1.0 + 0.01 * i, 1.0, wA, wB);
                v.wArow.push_back(wA); v.wBrow.push_back(wB);
            }
        dis::Layout L2 = dis::buildMatrix(v, M2);
        // X = 1 for A and B, 0 for T: the model is wA + wB = 1 in every row
        arma::vec X(L2.cols(), arma::fill::zeros);
        X.subvec(0, L2.nA + L2.nB - 1).ones();
        arma::vec y = M2 * X;
        check("Per-row weights: model of flat components = sum of the row weights",
              arma::abs(y - 1.0).max() < 1e-12, fmt(arma::abs(y - 1.0).max()));
        bool threw = false;
        dis::MatrixSpec bad = s;
        bad.wArow.assign(10, 0.5);
        try { arma::sp_mat Mb; dis::buildMatrix(bad, Mb); } catch (const std::invalid_argument &) { threw = true; }
        check("Per-row weights of wrong length are rejected", threw);
    }
    {
        // continuum correction keeps the model unchanged and puts both continua to 1
        std::vector<double> lamA, lamB, A, B, cA, rB;
        for (int i = 0; i < 200; ++i) {
            const double l = 5000.0 + 5.0 * i;
            lamA.push_back(l);
            lamB.push_back(l + 1.3);
        }
        auto ratio = [](double l) { return 1.5 * std::pow(l / 5500.0, -2.0); };
        auto offA = [](double l) { return 0.2 + 1e-4 * (l - 5000.0); };   // continuum error of A
        for (size_t i = 0; i < lamA.size(); ++i) {
            A.push_back(1.0 + offA(lamA[i]));
            cA.push_back(1.0 + offA(lamA[i]));
        }
        for (size_t i = 0; i < lamB.size(); ++i) {
            const double l = lamB[i];
            B.push_back(1.0 - ratio(l) * offA(l));                 // w_A dA + w_B dB = 0
            rB.push_back(ratio(l));
        }
        fr::normaliseToContinuum(lamA, A, cA, lamB, B, rB);
        double dev = 0.0;
        for (double v : A) dev = std::max(dev, std::fabs(v - 1.0));
        for (size_t i = 1; i + 1 < B.size(); ++i) dev = std::max(dev, std::fabs(B[i] - 1.0));
        check("Continuum correction with r(lambda): both components on 1", dev < 1e-12, fmt(dev));
    }
}

void testRvFit()
{
    // known eccentric SB2 orbit, 24 epochs, Gaussian noise 1.0 km/s (A) and 1.5 km/s (B)
    const double truth[7] = {20.54, 0.537, 68.6, 67.6, -5.6, 2436997.0, 1.85};
    auto make = [&](unsigned seed, rvfit::Data &d) {
        arma::arma_rng::set_seed(seed);
        d = rvfit::Data();
        for (int j = 0; j < 24; ++j) {
            const double t = 2448150.0 + 3.37 * j + 0.4 * std::sin(j);
            double vA, vB;
            rvfit::model(truth, t, vA, vB);
            d.t.push_back(t);
            d.rvA.push_back(vA + 1.0 * arma::randn());
            d.rvB.push_back(vB + 1.5 * arma::randn());
        }
    };
    const double start[7] = {20.6, 0.45, 60.0, 75.0, -3.0, 2436997.8, 1.6};
    const bool fixed[7] = {false, false, false, false, false, false, false};
    rvfit::Data d;
    make(11, d);
    rvfit::Result r = rvfit::fit(d, start, fixed);
    bool within = r.ok;
    std::string det;
    for (int i = 0; i < 7 && r.ok; ++i) {
        // T_peri: the passage near the data (same orbit), compared with the true one there
        double tv = truth[i];
        if (i == 5) tv += std::round((r.el[5] - truth[5]) / truth[0]) * truth[0];
        const double z = (r.el[i] - tv) / r.err[i];
        det += fmt(z) + " ";
        if (!(std::fabs(z) < 4.0)) within = false;
    }
    check("Orbit fit to RVs: elements within 4 sigma of the truth", within, det);
    check("Orbit fit: RMS of the residuals = noise", r.ok && std::fabs(r.rmsA - 1.0) < 0.35 && std::fabs(r.rmsB - 1.5) < 0.5,
          fmt(r.rmsA) + " " + fmt(r.rmsB));
    // fixed element stays
    bool fx[7] = {true, false, false, false, false, false, false};
    double st2[7];
    for (int i = 0; i < 7; ++i) st2[i] = start[i];
    st2[0] = truth[0];
    rvfit::Result rf = rvfit::fit(d, st2, fx);
    check("Orbit fit: fixed P unchanged, no error", rf.ok && rf.el[0] == truth[0] && rf.err[0] == 0.0
          && rf.freeIdx.size() == 6);
    // reported errors against the scatter of 150 noise realisations
    arma::mat est(150, 7);
    arma::vec meanErr(7, arma::fill::zeros);
    int okRuns = 0;
    for (int m = 0; m < 150; ++m) {
        rvfit::Data dm;
        make(100 + m, dm);
        rvfit::Result rm = rvfit::fit(dm, start, fixed);
        if (!rm.ok) continue;
        for (int i = 0; i < 7; ++i) { est(okRuns, i) = rm.el[i]; meanErr(i) += rm.err[i]; }
        ++okRuns;
    }
    est = est.rows(0, okRuns - 1);
    meanErr /= okRuns;
    bool errOk = okRuns > 140;
    std::string ratio;
    for (int i = 0; i < 7; ++i) {
        const double q = arma::stddev(est.col(i)) / meanErr(i);
        ratio += fmt(q) + " ";
        if (!(q > 0.75 && q < 1.3)) errOk = false;
    }
    check("Orbit fit: errors agree with the scatter of 150 realisations (ratio 0.75-1.3)", errOk, ratio);
    // SB1
    rvfit::Data d1 = d;
    d1.rvB.clear();
    rvfit::Result r1 = rvfit::fit(d1, start, fixed);
    check("Orbit fit SB1: K_B not fitted", r1.ok && r1.freeIdx.size() == 6 && r1.err[3] == 0.0
          && std::fabs(r1.el[2] - truth[2]) < 4 * r1.err[2], fmt(r1.el[2]) + " +- " + fmt(r1.err[2]));
    check("Orbit fit: far start epoch and P off by 0.3 % (several start phases)", r.ok && r.iterations > 0
          && std::fabs(r.el[1] - truth[1]) < 0.03);
    // own zero point of B: B shifted by +27 km/s (RVs from disentangling without static lines)
    rvfit::Data dofs = d;
    for (double &v : dofs.rvB) v += 27.0;
    dofs.offsetB = true;
    rvfit::Result ro = rvfit::fit(dofs, start, fixed);
    check("Orbit fit with an offset of B: offset and elements recovered",
          ro.ok && std::fabs(ro.offB - 27.0) < 4 * ro.errOffB && std::fabs(ro.el[2] - truth[2]) < 4 * ro.err[2]
          && std::fabs(ro.el[4] - truth[4]) < 4 * ro.err[4],
          fmt(ro.offB) + " +- " + fmt(ro.errOffB));
    // table reading
    const std::string f = "/tmp/spectangular_rvfit_test.dat";
    { std::ofstream o(f); o << "# t A B\n1.0 10 -10\n2.0 11 -11\n3.0 12 -12\n"; }
    rvfit::Data dt;
    std::string err;
    check("RV table read", rvfit::readTable(f, true, dt, err) && dt.t.size() == 3 && dt.rvB[2] == -12.0
          && dt.sigA.empty(), err);
    std::remove(f.c_str());
    check("Phase in [0, 1)", std::fabs(rvfit::phase(-1.0, 4.0, 0.0) - 0.75) < 1e-15
          && std::fabs(rvfit::phase(9.0, 4.0, 1.0)) < 1e-15);
}

// Flux ratio from synthetic spectra (templatefit): components disentangled with a wrong
// ratio k0 = 1, true k = 1.8 (constant and Planck shape), shifted and broadened templates
void testTemplateFit()
{
    const double c = 299792.458;
    // templates at 1 km/s, 5000-5400 A: A few broad lines, B many narrow lines
    auto makeTemplate = [&](int nl, double wmin, double wmax, double dmax, unsigned seed, double depthScale) {
        tfit::Spectrum t;
        std::mt19937 g(seed);
        std::uniform_real_distribution<double> u(0, 1);
        std::vector<double> lc, wd, dp;
        for (int k = 0; k < nl; ++k) { lc.push_back(5010 + 380 * u(g)); wd.push_back(wmin + (wmax - wmin) * u(g)); dp.push_back((0.05 + dmax * u(g)) * depthScale); }
        for (double l = 4990; l < 5410; l *= 1 + 1.0 / c) {
            double f = 1;
            for (int k = 0; k < nl; ++k) f -= dp[k] * std::exp(-0.5 * std::pow((l - lc[k]) / wd[k], 2));
            t.lam.push_back(l);
            t.f.push_back(f);
        }
        return t;
    };
    const tfit::Spectrum TA = makeTemplate(12, 0.6, 1.5, 0.3, 11, 1.0), TB = makeTemplate(80, 0.05, 0.15, 0.5, 12, 1.0);
    // broadening preserves the equivalent width
    {
        const tfit::Spectrum b = tfit::broaden(TB, 30.0, 40000.0);
        auto ew = [](const tfit::Spectrum &s, double l1, double l2) {
            double e = 0;
            for (size_t i = 1; i < s.lam.size(); ++i)
                if (s.lam[i] > l1 && s.lam[i] < l2) e += (1 - s.f[i]) * (s.lam[i] - s.lam[i - 1]);
            return e;
        };
        const double e0 = ew(TB, 5020, 5380), e1 = ew(b, 5020, 5380);
        check("Template broadening (vsini 30 km/s, R 40000): equivalent width kept", std::fabs(e1 / e0 - 1) < 0.005,
              fmt(e1 / e0));
    }
    const double vsA = 40, vsB = 10, R = 50000, vA = 4.0, vB = -7.0, cA = 1.02, k0 = 1.0, sigma = 0.003;
    for (int planck = 0; planck < 2; ++planck) {
        fr::Model m;
        if (planck) { m.mode = fr::Mode::Planck; m.tA = 15000; m.tB = 6000; m.lambdaRef = 5200; }
        const double kTrue = 1.8;
        const tfit::Spectrum bA = tfit::broaden(TA, vsA, R), bB = tfit::broaden(TB, vsB, R);
        tfit::Spectrum A, B;
        std::mt19937 g(5);
        std::normal_distribution<double> nd(0, sigma);
        double lref = 5200;
        for (double l = 5005; l < 5395; l *= 1 + 3.0 / c) {
            const double s = planck ? fr::shape(m, l, lref) : 1.0;
            double wa, wb, la, lb;
            fr::weights(k0, s, 1, wa, wb);
            fr::weights(kTrue, s, 1, la, lb);
            const double ta = fr::interpLinear(bA.lam, bA.f, l / (1 + vA / c)), tb = fr::interpLinear(bB.lam, bB.f, l / (1 + vB / c));
            const double cB = (1 - wa * cA) / wb;
            A.lam.push_back(l); A.f.push_back(cA - la / wa * (1 - ta) + nd(g));
            B.lam.push_back(l); B.f.push_back(cB - lb / wb * (1 - tb) + nd(g));
        }
        tfit::Options o;
        o.vsini[0] = vsA; o.vsini[1] = vsB; o.resolvingPower = R;
        if (!planck) m.lambdaRef = 0;
        const tfit::Result r = tfit::fit(A, B, TA, TB, m, k0, o);
        const std::string tag = planck ? " (Planck shape)" : " (constant)";
        check("Flux ratio from synthetic spectra" + tag + ": k recovered", r.ok && std::fabs(r.k - kTrue) < 0.01 * kTrue
              && std::fabs(r.k - kTrue) < 4 * r.errK, fmt(r.k) + " +- " + fmt(r.errK));
        check("Flux ratio from synthetic spectra" + tag + ": shifts and continuum level", std::fabs(r.shift[0] - vA) < 0.5
              && std::fabs(r.shift[1] - vB) < 0.3 && std::fabs(r.cA - cA) < 0.002,
              "v_A " + fmt(r.shift[0]) + ", v_B " + fmt(r.shift[1]) + ", c_A " + fmt(r.cA));
        check("Flux ratio from synthetic spectra" + tag + ": separate light fractions add up to 1",
              std::fabs(r.lInd[0] + r.lInd[1] - 1) < 0.01, fmt(r.lInd[0]) + " + " + fmt(r.lInd[1]));
        // renormalised components against the (broadened, shifted) templates
        double qa = 0, qb = 0; int n = 0;
        for (size_t i = 20; i + 20 < A.lam.size(); ++i) {
            qa += std::pow(r.renA[i] - fr::interpLinear(bA.lam, bA.f, A.lam[i] / (1 + vA / c)), 2);
            qb += std::pow(r.renB[i] - fr::interpLinear(bB.lam, bB.f, B.lam[i] / (1 + vB / c)), 2);
            ++n;
        }
        qa = std::sqrt(qa / n); qb = std::sqrt(qb / n);
        // noise of the renormalised spectra: sigma * w / l
        check("Renormalised components match the intrinsic spectra" + tag, qa < 2.0 * sigma * 0.5 / 0.64 && qb < 2.0 * sigma * 0.5 / 0.36,
              "RMS A " + fmt(qa) + ", B " + fmt(qb));
    }
    // wavy continua (slowly varying functions are nearly undetermined by the disentangling):
    // A + p, B - (w_A/w_B) p; constant continuum biased, polynomial of degree 4 not
    {
        const tfit::Spectrum bA = tfit::broaden(TA, vsA, R), bB = tfit::broaden(TB, vsB, R);
        tfit::Spectrum A, B;
        std::mt19937 g(9);
        std::normal_distribution<double> nd(0, sigma);
        const double kTrue = 1.8, wa = 0.5, wb = 0.5, la = kTrue / (1 + kTrue), lb = 1 / (1 + kTrue);
        for (double l = 5005; l < 5395; l *= 1 + 3.0 / c) {
            const double p = 0.05 * std::sin(2 * M_PI * (l - 5005) / 300.0);
            A.lam.push_back(l); A.f.push_back(1 + p - la / wa * (1 - fr::interpLinear(bA.lam, bA.f, l)) + nd(g));
            B.lam.push_back(l); B.f.push_back(1 - wa / wb * p - lb / wb * (1 - fr::interpLinear(bB.lam, bB.f, l)) + nd(g));
        }
        tfit::Options o;
        o.vsini[0] = vsA; o.vsini[1] = vsB; o.resolvingPower = R;
        o.fitShift[0] = o.fitShift[1] = false;
        o.contDegree = 0;
        const tfit::Result r0 = tfit::fit(A, B, TA, TB, fr::Model(), 1.0, o);
        o.contDegree = 4;
        const tfit::Result r4 = tfit::fit(A, B, TA, TB, fr::Model(), 1.0, o);
        check("Wavy continua: constant continuum biased, degree 4 recovers k (jackknife error not below formal)",
              r0.ok && r4.ok && std::fabs(r0.k - kTrue) > 0.02 && std::fabs(r4.k - kTrue) < 0.01 * kTrue
              && std::fabs(r4.k - kTrue) < 4 * r4.errKJack && r4.errKJack > 0.5 * r4.errK,
              "degree 0: " + fmt(r0.k) + ", degree 4: " + fmt(r4.k) + " +- " + fmt(r4.errKJack) + " (formal " + fmt(r4.errK) + ")");
    }
    // synthetic spectrum of B with too weak lines: the separate light fractions do not add up to 1
    {
        const tfit::Spectrum TBw = makeTemplate(80, 0.05, 0.15, 0.5, 12, 0.7);
        tfit::Spectrum A, B;
        const tfit::Spectrum bA = tfit::broaden(TA, vsA, R), bB = tfit::broaden(TB, vsB, R);
        for (double l = 5005; l < 5395; l *= 1 + 3.0 / c) {
            const double la = 1.8 / 2.8, lb = 1 / 2.8;
            A.lam.push_back(l); A.f.push_back(1 - la / 0.5 * (1 - fr::interpLinear(bA.lam, bA.f, l)));
            B.lam.push_back(l); B.f.push_back(1 - lb / 0.5 * (1 - fr::interpLinear(bB.lam, bB.f, l)));
        }
        tfit::Options o;
        o.vsini[0] = vsA; o.vsini[1] = vsB; o.resolvingPower = R;
        const tfit::Result r = tfit::fit(A, B, TA, TBw, fr::Model(), 1.0, o);
        check("Synthetic spectrum with wrong line strengths: separate light fractions do not add up to 1",
              r.ok && std::fabs(r.lInd[0] + r.lInd[1] - 1) > 0.1, fmt(r.lInd[0]) + " + " + fmt(r.lInd[1]));
    }
}

// Barycentric correction against astropy 7/8 (radial_velocity_correction); astropy adds the
// relativistic and solar gravitational terms (about +4.8 m/s, nearly constant)
void testBaryCorr()
{
    struct Case { const char *ra, *dec; double lat, lon, h; bc::TimeScale sc; double t, ref; };
    const Case cs[] = {
        {"02 12 22.28", "+30 18 11.1", 28.301, -16.51, 2390, bc::TimeScale::HJD_UTC, 2458415.65203308, 4.725282},
        {"02 12 22.28", "+30 18 11.1", 28.301, -16.51, 2390, bc::TimeScale::HJD_UTC, 2458480.3258, -23.635318},
        {"02 12 22.28", "+30 18 11.1", 28.301, -16.51, 2390, bc::TimeScale::HJD_UTC, 2458540.34613775, -26.554353},
        {"18 36 56.34", "+38 47 01.3", -24.6275, -70.4044, 2635, bc::TimeScale::JD_UTC, 2451545.0, -0.908684},
        {"18 36 56.34", "+38 47 01.3", -24.6275, -70.4044, 2635, bc::TimeScale::JD_UTC, 2455000.25, 4.580679},
        {"18 36 56.34", "+38 47 01.3", -24.6275, -70.4044, 2635, bc::TimeScale::JD_UTC, 2460000.7, 11.198751},
        {"05 14 32.27", "-08 12 05.9", 50.98, 11.7111, 341, bc::TimeScale::BJD_TDB, 2459000.4, -3.527081},
        {"05 14 32.27", "-08 12 05.9", 50.98, 11.7111, 341, bc::TimeScale::BJD_TDB, 2461000.1, 8.117010},
    };
    double mx = 0;
    bool ok = true;
    for (const Case &c : cs) {
        double ra, dec, v;
        std::string err;
        ok = ok && bc::parseRa(c.ra, ra) && bc::parseDec(c.dec, dec);
        bc::Site s;
        s.latDeg = c.lat; s.lonDeg = c.lon; s.heightM = c.h;
        ok = ok && bc::correction(ra, dec, s, c.t, c.sc, v, nullptr, err);
        mx = std::max(mx, std::fabs(v - c.ref));
    }
    check("Barycentric correction agrees with astropy within 10 m/s (JD, HJD, BJD; three sites)", ok && mx < 0.010,
          "max difference " + fmt(1000 * mx) + " m/s");
    double a1 = 0, a2 = 0, d = 0, dummy;
    const bool pOk = bc::parseRa("02:12:22.28", a1) && bc::parseRa("33.0928", a2) && bc::parseDec("-08 12 05.9", d)
                     && !bc::parseDec("95", dummy) && !bc::parseRa("abc", dummy);
    check("Coordinates: hours, degrees, negative declination",
          pOk && std::fabs(a1 - 33.092833) < 1e-5 && std::fabs(a2 - 33.0928) < 1e-9 && std::fabs(d + 8.201639) < 1e-5,
          fmt(a1) + ", " + fmt(a2) + ", " + fmt(d));
    // MJD = JD - 2400000.5
    double v1 = 0, v2 = 0;
    std::string err;
    bc::Site s;
    s.latDeg = 28.3; s.lonDeg = -16.5; s.heightM = 2390;
    const bool mOk = bc::correction(33.09, 30.3, s, 58480.3, bc::TimeScale::MJD_UTC, v1, nullptr, err)
                     && bc::correction(33.09, 30.3, s, 2458480.8, bc::TimeScale::JD_UTC, v2, nullptr, err);
    check("MJD and JD give the same correction", mOk && std::fabs(v1 - v2) < 1e-9, fmt(v1) + " " + fmt(v2));
    // altitude for the airmass, against astropy (AltAz without refraction)
    struct Alt { const char *ra, *dec; double lat, lon, h, jd, ref; };
    const Alt as[] = {
        {"02 12 22.28", "+30 18 11.1", 28.301, -16.51, 2390, 2458415.60, 73.701036},
        {"02 12 22.28", "+30 18 11.1", 28.301, -16.51, 2390, 2458480.40, 80.670039},
        {"18 36 56.34", "+38 47 01.3", -24.6275, -70.4044, 2635, 2455000.25, -74.838852},
        {"05 14 32.27", "-08 12 05.9", 50.98, 11.7111, 341, 2459000.95, 29.286984},
    };
    double ma = 0;
    bool aOk = true;
    for (const Alt &c : as) {
        double ra, dec, v, alt = 0;
        std::string e;
        bc::Site st;
        st.latDeg = c.lat; st.lonDeg = c.lon; st.heightM = c.h;
        aOk = aOk && bc::parseRa(c.ra, ra) && bc::parseDec(c.dec, dec)
              && bc::correction(ra, dec, st, c.jd, bc::TimeScale::JD_UTC, v, nullptr, e, &alt);
        ma = std::max(ma, std::fabs(alt - c.ref));
    }
    check("Altitude agrees with astropy within 0.02 deg (also below the horizon)", aOk && ma < 0.02, "max difference " + fmt(ma) + " deg");
    check("Airmass: 1 at the zenith, 2.00 at 30 deg, finite at the horizon, NaN below",
          std::fabs(bc::airmass(90) - 1) < 1e-3 && std::fabs(bc::airmass(30) - 1.995) < 0.01
          && bc::airmass(0.01) > 30 && bc::airmass(0.01) < 45 && std::isnan(bc::airmass(-1)),
          fmt(bc::airmass(90)) + " " + fmt(bc::airmass(30)) + " " + fmt(bc::airmass(0.01)));
}


// Excluded wavelength ranges: reading, marking, removing rows; the solution with removed
// rows equals the solution of the system without these rows
// Static lines shifted per spectrum (rvT: tellurics in barycentrically corrected spectra)
void testShiftedStatic()
{
    {
        // BC file as written by the tool or ses-writetxt.py: last number of each line, a trailing
        // "# file" comment and comment lines ignored
        const std::string f = "unittest_bc.txt";
        std::ofstream o(f);
        o << "# Barycentric correction\n# spectrum\tHJD\tVCORRECT\tBC\t# file\n"
          << "0\t2458990.25\t25.1\t25.102102\t# sci_a.fits\n1\t2459000.5\t-12.3\t-12.299495\t# sci_b.fits\n-3.5\n";
        o.close();
        std::vector<double> v;
        std::string err;
        const bool ok = dataio::readLastColumn(f, v, err);
        check("readLastColumn: BC column of the tool / ses-writetxt.py format and one value per line",
              ok && v.size() == 3 && v[0] == 25.102102 && v[1] == -12.299495 && v[2] == -3.5, err);
        std::remove(f.c_str());
    }
    const double dlog = 2e-5;
    dis::MatrixSpec s;
    s.num = 3; s.logbin = 40; s.dlog = dlog; s.sb2 = true; s.telluric = true;
    const double shA[3] = {0, 2, -1}, shB[3] = {1, -2, 0}, shT[3] = {0.5, -1.5, 2.0};
    auto v = [&](double px){ return dis::kSpeedOfLight * (std::pow(10.0, px * dlog) - 1); };
    for (int a = 0; a < 3; ++a) {
        s.rvA.push_back(v(shA[a])); s.rvB.push_back(v(shB[a])); s.rvT.push_back(v(shT[a]));
        s.wA.push_back(0.5); s.wB.push_back(0.5); s.wT.push_back(1.0);
    }
    arma::sp_mat M;
    const dis::Layout L = dis::buildMatrix(s, M);
    check("Shifted static lines: columns 40 + span of the shifts (rest -2 to 41)", L.nT == 44,
          std::to_string(L.nT));
    check("Shifted static lines: axis starts at the smallest rest position", L.x0T == -2, std::to_string(L.x0T));
    check("Shifted static lines: column wavelength", std::fabs(dis::columnLog10Lambda(L, 'T', 3, 3.8, dlog)
                                                             - (3.8 + 1 * dlog)) < 1e-15);
    // a line at rest pixel 20 of T appears at 20 + sT in spectrum a (linear interpolation)
    arma::vec X(L.cols(), arma::fill::zeros);
    X(L.startT() + (20 - L.x0T)) = 1.0;
    const arma::vec C = M * X;
    bool ok = true;
    for (int a = 0; a < 3; ++a) {
        const double pos = 20 + shT[a];
        const int i0 = int(std::floor(pos));
        const double f = pos - i0;
        for (int i = 0; i < 40; ++i) {
            const double expect = i == i0 ? 1 - f : (i == i0 + 1 ? f : 0.0);
            ok &= std::fabs(C(a * 40 + i) - expect) < 1e-9;   // shifts via 10^(s dlog): rounding
        }
    }
    check("Shifted static lines: a line moves with rvT", ok);
    // without rvT: unshifted, as before
    dis::MatrixSpec u = s; u.rvT.clear();
    arma::sp_mat Mu;
    const dis::Layout Lu = dis::buildMatrix(u, Mu);
    check("Static lines without rvT: unshifted (40 columns, x0T = 0)", Lu.nT == 40 && Lu.x0T == 0);
    // warm start: T values at the same rest pixel
    arma::vec Xu(Lu.cols(), arma::fill::zeros);
    for (int k = 0; k < Lu.nT; ++k) Xu(Lu.startT() + k) = k;
    const arma::vec Y = dis::transferSolution(Lu, Xu, L);
    check("transferSolution: static lines at the same rest pixel",
          Y(L.startT() + (10 - L.x0T)) == 10 && Y(L.startT()) == 0 && Y(L.startT() + L.nT - 1) == 39);
    // static lines only in ranges: columns only for rest pixels inside, wavelengths, entries
    {
        dis::MatrixSpec r = s;                       // shifted static lines (rvT)
        r.w0 = 3.8;
        r.tRanges = {{3.8 + 5.5 * dlog, 3.8 + 9.5 * dlog}, {3.8 + 30 * dlog, 3.8 + 31 * dlog}};
        arma::sp_mat Mr;
        const dis::Layout Lr = dis::buildMatrix(r, Mr);
        bool okPix = Lr.nT == 6 && Lr.tPix.size() == 6 && Lr.tPix[0] == 6 && Lr.tPix[3] == 9 && Lr.tPix[4] == 30;
        okPix = okPix && std::fabs(dis::columnLog10Lambda(Lr, 'T', 4, 3.8, dlog) - (3.8 + 30 * dlog)) < 1e-15;
        // a line at rest pixel 20 is outside: not representable; at rest pixel 8 it moves with rvT
        arma::vec Xr(Lr.cols(), arma::fill::zeros);
        Xr(Lr.startT() + 2) = 1.0;                   // rest pixel 8
        const arma::vec Cr = Mr * Xr;
        bool okMove = true;
        for (int a = 0; a < 3; ++a) {
            const double pos = 8 + shT[a];
            const int i0 = int(std::floor(pos));
            const double f = pos - i0;
            okMove &= std::fabs(Cr(a * 40 + i0) - (1 - f)) < 1e-9 && (f < 1e-12 || std::fabs(Cr(a * 40 + i0 + 1) - f) < 1e-9);
        }
        // unshifted static lines in ranges
        dis::MatrixSpec u2 = r; u2.rvT.clear();
        arma::sp_mat Mu2;
        const dis::Layout Lu2 = dis::buildMatrix(u2, Mu2);
        const bool okU = Lu2.nT == 6 && Lu2.tPix[0] == 6 && arma::accu(arma::abs(arma::mat(Mu2.cols(Lu2.startT(), Lu2.startT() + 5)))) == 3 * 6;
        // warm start from the full layout keeps the values at the same rest pixel
        arma::vec Xf(L.cols(), arma::fill::zeros);
        for (int k = 0; k < L.nT; ++k) Xf(L.startT() + k) = L.x0T + k;
        const arma::vec Yr = dis::transferSolution(L, Xf, Lr);
        const bool okW = Yr(Lr.startT()) == 6 && Yr(Lr.startT() + 4) == 30;
        check("Static lines only in ranges: columns, wavelengths, shift with rvT, unshifted, warm start",
              okPix && okMove && okU && okW, std::to_string(Lr.nT) + " columns");
        // groups: own weight per group and spectrum
        dis::MatrixSpec gr = u2;
        gr.tRangeGroup = {0, 1};
        gr.wTgroup = {{1.0, 2.0, 3.0}, {0.5, 0.25, 4.0}};
        arma::sp_mat Mg;
        const dis::Layout Lg = dis::buildMatrix(gr, Mg);
        const arma::mat Dg(Mg);
        const bool okG = Lg.tGroup.size() == 6 && Lg.tGroup[0] == 0 && Lg.tGroup[4] == 1
                         && Dg(1 * 40 + 6, Lg.startT()) == 2.0 && Dg(2 * 40 + 30, Lg.startT() + 4) == 4.0
                         && Dg(0 * 40 + 31, Lg.startT() + 5) == 0.5;
        // labels of the ranges file
        std::ofstream o("unittest_ranges.dat");
        o << "# c\n6274 6331 O2 gamma band\n7159 7341 H2O\n5000 5001\n";
        o.close();
        std::vector<std::pair<double, double>> rr;
        std::vector<std::string> lab;
        std::string e;
        const bool okL = dataio::readRanges("unittest_ranges.dat", rr, e, &lab) && lab.size() == 3 && lab[0] == "O2"
                         && lab[1] == "H2O" && lab[2].empty();
        std::remove("unittest_ranges.dat");
        check("Groups of static lines: weights per group and spectrum; labels from the ranges file", okG && okL);
    }
}

void testMask()
{
    const std::string f = tmpFile("");
    {
        std::ofstream o(f);
        o << "# comment\n\n6865 6936   O2 B band\n7159\t7341\n";
    }
    std::vector<std::pair<double, double>> r;
    std::string err;
    const bool ok = dataio::readRanges(f, r, err);
    check("Mask file: two ranges, text after the numbers and comments ignored",
          ok && r.size() == 2 && r[0].first == 6865 && r[1].second == 7341, err);
    {
        std::ofstream o(f);
        o << "7000 6900\n";
    }
    const bool okBad = dataio::readRanges(f, r, err);
    check("Mask file: from >= to rejected with line number", !okBad && err.find("line 1") != std::string::npos, err);
    {
        std::ofstream o(f);
        o << dataio::kTelluricMask;
    }
    const bool okT = dataio::readRanges(f, r, err);
    check("Default list readable (6 telluric bands + 12 chromospheric lines)", okT && r.size() == 18, std::to_string(r.size()) + " ranges");
    std::remove(f.c_str());

    std::vector<double> lw;
    for (int i = 0; i < 100; ++i) lw.push_back(std::log10(7100.0 + 3.0 * i));   // 7100 ... 7397
    std::vector<char> mk;
    const size_t n = dataio::markRanges(lw, {{7159.0, 7341.0}}, mk);
    size_t expect = 0;
    for (double v : lw) expect += (std::pow(10.0, v) >= 7159.0 && std::pow(10.0, v) <= 7341.0);
    check("Mask: pixels in the range marked", n == expect && n == 61 && mk[19] == 0 && mk[20] == 1,
          std::to_string(n) + " pixels");

    // system: 6 spectra x 300 pixels, rows 100..159 of every spectrum excluded
    dis::MatrixSpec sp;
    sp.num = 6;
    sp.logbin = 300;
    sp.dlog = 2.4e-6;
    for (int a = 0; a < sp.num; ++a) {
        sp.rvA.push_back(20 * std::sin(a));
        sp.rvB.push_back(-25 * std::sin(a));
        sp.wA.push_back(0.5); sp.wB.push_back(0.5); sp.wT.push_back(1.0);
    }
    arma::sp_mat M;
    const dis::Layout L = dis::buildMatrix(sp, M);
    arma::arma_rng::set_seed(3);
    const arma::vec C = 1.0 + 0.01 * arma::randn<arma::vec>(M.n_rows);
    std::vector<char> masked(M.n_rows, 0);
    std::vector<arma::uword> keep;
    for (arma::uword i = 0; i < M.n_rows; ++i) {
        const int px = int(i % 300);
        masked[i] = px >= 100 && px < 160;
        if (!masked[i]) keep.push_back(i);
    }
    arma::sp_mat Md = M;
    const size_t dropped = dis::dropRows(Md, masked);
    arma::mat Dense(M);
    arma::mat Red = Dense.rows(arma::uvec(keep));
    arma::vec Xd, Xr;
    dis::solveLsqr(Md, C, Xd, 1e-14);
    dis::solveLsqr(arma::sp_mat(Red), arma::vec(C.elem(arma::uvec(keep))), Xr, 1e-14);
    const std::vector<char> empty = dis::emptyColumns(Md);
    int nEmpty = 0;
    for (char e : empty) nEmpty += e;
    check("Removed rows: same solution as without these rows; columns only in the range empty",
          dropped > 0 && Md.n_rows == M.n_rows && arma::abs(Xd - Xr).max() < 1e-9 && nEmpty > 0
          && L.cols() == int(Md.n_cols),
          fmt(arma::abs(Xd - Xr).max()) + ", " + std::to_string(nEmpty) + " empty columns");

    // the pieces on both sides of the range have their own offset null direction: a warm
    // start with different offsets on the two sides ends, after removing the null space
    // components, at the cold-start solution (no steps at the range)
    const arma::mat Nseg = dis::offsetNullspace(L, Md);
    arma::vec x0 = Xd;
    for (int part = 0; part < 2; ++part) {
        const int a = part == 0 ? 0 : L.startB();
        const int n = part == 0 ? L.nA : L.nB;
        int g = a;                                     // first empty column of the component
        while (g < a + n && !empty[g]) ++g;
        for (int c = a; c < a + n; ++c)
            if (!empty[c]) x0(c) += (c < g ? 0.3 : -0.4) * (part == 0 ? 1.0 : -1.0);
    }
    arma::vec Xw;
    dis::solveLsqr(Md, C, Xw, 1e-14, 0, &x0);
    const double before = arma::abs(Xw - Xd).max();
    dis::removeOffsetNullspace(L, Md, Xw);
    for (size_t c = 0; c < empty.size(); ++c) if (empty[c]) Xw(c) = Xd(c);
    const double after = arma::abs(Xw - Xd).max();
    check("Excluded range: one offset null direction per piece; warm start with different offsets "
          "on both sides returns to the cold-start solution",
          Nseg.n_cols == 2 && before > 0.2 && after < 1e-6,
          std::to_string(Nseg.n_cols) + " null directions; offset " + fmt(before) + " -> " + fmt(after));
}


// Continuum normalisation: line-rich spectrum on a wavy continuum with a step at a gap
void testContinuum()
{
    arma::arma_rng::set_seed(11);
    std::vector<double> w, f, trueCont;
    // lines: 400 Gaussians of 0.1-0.6 depth, width 0.15-0.4 A
    const int nl = 400;
    arma::vec lc = 5000.0 + 600.0 * arma::randu<arma::vec>(nl);
    arma::vec ld = 0.1 + 0.5 * arma::randu<arma::vec>(nl);
    arma::vec lw = 0.15 + 0.25 * arma::randu<arma::vec>(nl);
    for (double lam = 5000.0; lam < 5600.0; lam += 0.04) {
        if (lam > 5300.0 && lam < 5310.0) continue;                // gap (excluded range)
        const double c = (lam < 5300.0 ? 1.4 : 0.8) + 0.1 * std::sin(lam / 40.0);   // step + wiggle
        double line = 1.0;
        for (int i = 0; i < nl; ++i) line *= 1.0 - ld(i) * std::exp(-0.5 * std::pow((lam - lc(i)) / lw(i), 2));
        w.push_back(lam);
        trueCont.push_back(c);
        f.push_back(c * line * (1.0 + 0.003 * arma::randn()));
    }
    const auto pc = cont::pieces(w, 5.0);
    cont::Options o;
    o.low = 2.0;                   // white noise: symmetric about the continuum
    std::vector<double> c;
    std::string err;
    const bool ok = cont::fit(w, f, o, c, err);
    // deviation from the true continuum: 95th percentile (the ends of a piece, where the
    // spline is constrained from one side only, are worse) and largest
    std::vector<double> dev;
    for (size_t i = 0; i < w.size() && ok; ++i) dev.push_back(std::fabs(c[i] / trueCont[i] - 1.0));
    std::sort(dev.begin(), dev.end());
    const double p95 = dev.empty() ? 1.0 : dev[size_t(0.95 * (dev.size() - 1))];
    const double worst = dev.empty() ? 1.0 : dev.back();
    check("Continuum: two pieces at the gap; spline through the upper points follows step 1.4 -> 0.8 "
          "and wiggle (95 % of the points within 2 %, all within 6 %)",
          ok && pc.size() == 2 && p95 < 0.02 && worst < 0.06, std::to_string(pc.size()) + " pieces, 95 % within "
          + fmt(p95) + ", largest " + fmt(worst) + (ok ? "" : " " + err));
    // short piece: constant
    std::vector<double> ws = {1, 2, 3, 4, 5}, fs = {1.0, 0.5, 1.0, 1.0, 1.0}, cs;
    const bool okS = cont::fit(ws, fs, o, cs, err);
    check("Continuum: piece shorter than 20 A gets a constant at the upper points",
          okS && std::fabs(cs[0] - 1.0) < 1e-12 && std::fabs(cs[4] - 1.0) < 1e-12, okS ? fmt(cs[0]) : err);
}


// Aligning the continua: spectra with shifted lines and different broad normalisation errors
void testAlign()
{
    const int num = 10, n = 6000;
    arma::arma_rng::set_seed(31);
    std::vector<double> lc, ld;
    for (int l = 0; l < 150; ++l) { lc.push_back(arma::randu() * n); ld.push_back(0.1 + 0.6 * arma::randu()); }
    std::vector<std::vector<double>> f(num, std::vector<double>(n)), truth = f;
    std::vector<std::vector<char>> mk(num, std::vector<char>(n, 0));
    for (int j = 0; j < num; ++j) {
        const double shift = 15.0 * std::sin(2.0 * M_PI * j / num);        // pixels
        const double per = 3000.0 + 500.0 * j, phase = 0.7 * j;   // >= 7.5 times the width
        for (int i = 0; i < n; ++i) {
            double v = 1.0;
            for (size_t l = 0; l < lc.size(); ++l) v *= 1.0 - ld[l] * std::exp(-0.5 * std::pow((i - lc[l] - shift) / 4.0, 2));
            truth[j][i] = v;
            f[j][i] = v * (1.0 + 0.03 * std::sin(2.0 * M_PI * i / per + phase));
            mk[j][i] = i >= 3000 && i < 3100;                                // excluded range
        }
    }
    std::vector<double> rms;
    std::string err;
    const bool ok = cont::alignToMean(f, mk, 400, rms, err);
    // remaining difference of the normalisation between the spectra: ratio to the truth,
    // relative to its mean over the spectra (a common factor cannot and need not be removed)
    double worst = 0.0, q = 0.0, worstEdge = 0.0;
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        double m = 0.0;
        for (int j = 0; j < num; ++j) m += f[j][i] / truth[j][i] / num;
        for (int j = 0; j < num; ++j) {
            const double d = std::fabs(f[j][i] / truth[j][i] / m - 1.0);
            if (i < 400 || i >= n - 400) { worstEdge = std::max(worstEdge, d); continue; }
            worst = std::max(worst, d);
            q += d * d;
            ++cnt;
        }
    }
    const double rmsLeft = std::sqrt(q / std::max(cnt, 1));
    check("Align continua: normalisation errors of 3 % (periods 7.5-19 times the width) between the spectra "
          "reduced at least fivefold (lines of depth up to 0.7 shifted by up to 15 px, excluded range bridged)",
          ok && rmsLeft < 0.004 && rms.size() == size_t(num), "rms 2.1 % -> " + fmt(rmsLeft) + ", largest "
          + fmt(worst) + ", within one width of the ends " + fmt(worstEdge) + (ok ? "" : " " + err));
}

int main()
{
    testOrbit();
    testDataio();
    testDisentangle();
    testWarmStart();
    testCubic();
    testLevmar();
    testNelderMead();
    testProgressMonitor();
    testErrorEstimate();
    testProbeSimplex();
    testFluxRatio();
    testRvFit();
    testTemplateFit();
    testBaryCorr();
    testShiftedStatic();
    testMask();
    testContinuum();
    testAlign();
    std::cout << (g_fail ? "FAILED: " + std::to_string(g_fail) : std::string("all passed"))
              << "\n";
    return g_fail ? 1 : 0;
}
