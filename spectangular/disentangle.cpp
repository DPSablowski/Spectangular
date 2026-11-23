#include "disentangle.h"
#include <stdexcept>

#include <dlfcn.h>
#ifdef _OPENMP
#include <omp.h>
#define SPEC_OMP(x) _Pragma(#x)
#else
#define SPEC_OMP(x)   // without OpenMP: single-core, no warning about unknown pragmas
#endif

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <chrono>

namespace dis {

// Set the number of cores for BLAS/LAPACK (dense SVD) at runtime, without a hard dependency:
// OpenBLAS, BLIS or MKL, if loaded; otherwise no effect.
int setBlasThreads(int n)
{
    using SetFn = void (*)(int);
    int found = 0;
    for (const char *name : {"openblas_set_num_threads", "bli_thread_set_num_threads",
                             "MKL_Set_Num_Threads"}) {
        if (void *f = dlsym(RTLD_DEFAULT, name)) {
            reinterpret_cast<SetFn>(f)(n);
            ++found;
        }
    }
#ifdef _OPENMP
    omp_set_num_threads(n);
#endif
    return found;
}


double pixelShift(double v, double dlog)
{
    return std::log10(1.0 + v / kSpeedOfLight) / dlog;
}

namespace {

// Splits the rest position x into integer part k and fraction f in [0, 1).
// Values very close to integers are snapped, so that integer shifts
// do not produce weights close to 0.
void splitPosition(double x, long &k, double &f)
{
    const double tol = 1e-9;
    double fl = std::floor(x);
    k = long(fl);
    f = x - fl;
    if (f < tol) {
        f = 0.0;
    } else if (f > 1.0 - tol) {
        k += 1;
        f = 0.0;
    }
}

// Interpolation weights for the rest position x: columns k0 ... k0+n-1 with weights w.
// Linear: 2 pixels. Cubic: Catmull-Rom / Keys (a = -1/2) over 4 pixels; interpolating,
// reproduces polynomials up to degree 2, weights continuously differentiable in x.
// At (snapped) whole-pixel positions both give the single weight 1.
int stencil(double x, Interpolation ip, long &k0, double w[4])
{
    long k;
    double f;
    splitPosition(x, k, f);
    if (f == 0.0) {
        k0 = k;
        w[0] = 1.0;
        return 1;
    }
    if (ip == Interpolation::Linear) {
        k0 = k;
        w[0] = 1.0 - f;
        w[1] = f;
        return 2;
    }
    const double f2 = f * f, f3 = f2 * f;
    k0 = k - 1;
    w[0] = 0.5 * (-f3 + 2.0 * f2 - f);
    w[1] = 0.5 * (3.0 * f3 - 5.0 * f2 + 2.0);
    w[2] = 0.5 * (-3.0 * f3 + 4.0 * f2 + f);
    w[3] = 0.5 * (f3 - f2);
    return 4;
}

// Range of occupied rest positions for a component with shifts s.
void columnRange(const std::vector<double> &s, int logbin, Interpolation ip, long &lo, long &hi)
{
    lo = LONG_MAX;
    hi = LONG_MIN;
    double w[4];
    for (double sa : s) {
        long k0;
        stencil(0.0 - sa, ip, k0, w);
        lo = std::min(lo, k0);
        const int n = stencil(double(logbin - 1) - sa, ip, k0, w);
        hi = std::max(hi, k0 + n - 1);
    }
}

// Collects the entries (row, column, value) of a component.
struct Triplets {
    std::vector<arma::uword> row, col;
    std::vector<double> val;
    void add(arma::uword r, arma::uword c, double v)
    {
        if (v == 0.0) return;
        row.push_back(r);
        col.push_back(c);
        val.push_back(v);
    }
};

// w: weight per spectrum; wRow (if not empty): weight per row, replaces w.
// colMap (if not empty): column of rest pixel x0 + j, or -1 (pixel not in the unknowns)
// colGroup/wGroup (if not empty): weight wGroup[group of the column][spectrum] instead of w.
void addComponent(Triplets &T, const std::vector<double> &s, const std::vector<double> &w,
                  const std::vector<double> &wRow, int logbin, long x0, int colStart, Interpolation ip,
                  const std::vector<int> &colMap = std::vector<int>(),
                  const std::vector<int> &colGroup = std::vector<int>(),
                  const std::vector<std::vector<double>> &wGroup = std::vector<std::vector<double>>())
{
    const bool grouped = !colGroup.empty() && !wGroup.empty();
    double wt[4];
    const bool perRow = !wRow.empty();
    const bool mapped = !colMap.empty();
    for (size_t a = 0; a < s.size(); ++a) {
        for (int i = 0; i < logbin; ++i) {
            long k0;
            const int n = stencil(double(i) - s[a], ip, k0, wt);
            arma::uword row = arma::uword(a) * logbin + i;
            const double wa = perRow ? wRow[row] : w[a];
            for (int q = 0; q < n; ++q) {
                const long j = k0 - x0 + q;
                if (mapped) {
                    if (j < 0 || j >= long(colMap.size()) || colMap[j] < 0) continue;
                    const double wc = grouped ? wGroup[size_t(colGroup[size_t(colMap[j])])][a] : wa;
                    T.add(row, arma::uword(colStart + colMap[j]), wc * wt[q]);
                }
                else
                    T.add(row, arma::uword(colStart + j), wa * wt[q]);
            }
        }
        keepAlive();       // long spectra: GUI events between the spectra
    }
}

} // namespace

Layout buildMatrix(const MatrixSpec &spec, arma::sp_mat &M)
{
    Layout L;
    L.num = spec.num;
    L.logbin = spec.logbin;
    const size_t nRows = size_t(spec.num) * size_t(spec.logbin);
    if ((!spec.wArow.empty() && spec.wArow.size() != nRows)
        || (spec.sb2 && !spec.wBrow.empty() && spec.wBrow.size() != nRows))
        throw std::invalid_argument("buildMatrix: per-row weights do not match spectra x pixels");

    std::vector<double> sA(spec.num), sB;
    for (int a = 0; a < spec.num; ++a)
        sA[a] = pixelShift(spec.rvA[a], spec.dlog);
    long lo, hi;
    columnRange(sA, spec.logbin, spec.interp, lo, hi);
    L.x0A = int(lo);
    L.nA = int(hi - lo + 1);

    if (spec.sb2) {
        sB.resize(spec.num);
        for (int a = 0; a < spec.num; ++a)
            sB[a] = pixelShift(spec.rvB[a], spec.dlog);
        columnRange(sB, spec.logbin, spec.interp, lo, hi);
        L.x0B = int(lo);
        L.nB = int(hi - lo + 1);
    }
    std::vector<double> sT;
    const bool shiftT = spec.telluric && !spec.rvT.empty();
    if (shiftT) {
        if (int(spec.rvT.size()) < spec.num)
            throw std::invalid_argument("buildMatrix: fewer velocities of the static lines than spectra");
        sT.resize(spec.num);
        for (int a = 0; a < spec.num; ++a)
            sT[a] = pixelShift(spec.rvT[a], spec.dlog);
        columnRange(sT, spec.logbin, spec.interp, lo, hi);
        L.x0T = int(lo);
        L.nT = int(hi - lo + 1);
    }
    else
        L.nT = spec.telluric ? spec.logbin : 0;
    // static lines only in ranges: columns of the rest pixels inside them
    std::vector<int> tMap;
    const bool grouped = spec.telluric && !spec.tRanges.empty() && spec.tRangeGroup.size() == spec.tRanges.size()
                         && !spec.wTgroup.empty();
    if (grouped) {
        for (int g : spec.tRangeGroup)
            if (g < 0 || g >= int(spec.wTgroup.size()) || int(spec.wTgroup[size_t(g)].size()) < spec.num)
                throw std::invalid_argument("buildMatrix: weights of the groups of static lines do not match");
    }
    if (spec.telluric && !spec.tRanges.empty()) {
        tMap.assign(size_t(L.nT), -1);
        int k = 0;
        for (int j = 0; j < L.nT; ++j) {
            const double lw = spec.w0 + double(L.x0T + j) * spec.dlog;
            for (size_t ri = 0; ri < spec.tRanges.size(); ++ri) {
                const auto &r = spec.tRanges[ri];
                if (lw >= r.first && lw <= r.second) {
                    tMap[size_t(j)] = k++;
                    L.tPix.push_back(L.x0T + j);
                    if (grouped) L.tGroup.push_back(spec.tRangeGroup[ri]);
                    break;
                }
            }
        }
        L.nT = k;
    }

    Triplets T;
    const size_t per = spec.interp == Interpolation::Cubic ? 4 : 2;
    const size_t est = size_t(L.rows()) * (spec.sb2 ? 2 * per : per) + (spec.telluric ? size_t(L.rows()) : 0);
    T.row.reserve(est);
    T.col.reserve(est);
    T.val.reserve(est);
    addComponent(T, sA, spec.wA, spec.wArow, spec.logbin, L.x0A, 0, spec.interp);
    if (spec.sb2)
        addComponent(T, sB, spec.wB, spec.wBrow, spec.logbin, L.x0B, L.startB(), spec.interp);
    if (shiftT)
        addComponent(T, sT, spec.wT, std::vector<double>(), spec.logbin, L.x0T, L.startT(), spec.interp, tMap,
                     L.tGroup, grouped ? spec.wTgroup : std::vector<std::vector<double>>());
    else if (spec.telluric) {
        for (int a = 0; a < spec.num; ++a)
            for (int i = 0; i < spec.logbin; ++i) {
                const int c = tMap.empty() ? i : tMap[size_t(i)];
                if (c >= 0) T.add(arma::uword(a) * spec.logbin + i, arma::uword(L.startT() + c),
                                  grouped ? spec.wTgroup[size_t(L.tGroup[size_t(c)])][a] : spec.wT[a]);
            }
    }
    arma::umat loc(2, T.val.size());
    for (size_t k = 0; k < T.val.size(); ++k) {
        loc(0, k) = T.row[k];
        loc(1, k) = T.col[k];
    }
    // add_values = true: entries at the same position are summed
    M = arma::sp_mat(true, loc, arma::vec(T.val), arma::uword(L.rows()), arma::uword(L.cols()));
    return L;
}

double columnLog10Lambda(const Layout &L, char comp, int k, double w0, double dlog)
{
    switch (comp) {
    case 'A': return w0 + double(L.x0A + k) * dlog;
    case 'B': return w0 + double(L.x0B + k) * dlog;
    default:  return w0 + double(L.tPix.empty() ? L.x0T + k : L.tPix[size_t(k)]) * dlog;
    }
}

SolveResult solveSvdDc(const arma::sp_mat &M, const arma::vec &C, arma::vec &X)
{
    SolveResult r;
    r.method = "SVD (DaC)";
    arma::mat U, V;
    arma::vec w;
    bool ok;
    {
        arma::mat D(M);   // dense copy only for the duration of the decomposition
        ok = arma::svd_econ(U, w, V, D, "both", "dc");
    }
    if (!ok || w.n_elem == 0) {
        r.message = "SVD failed (not enough memory?).";
        return r;
    }
    const double eps = std::numeric_limits<double>::epsilon();
    r.threshold = 0.5 * std::sqrt(double(M.n_rows + M.n_cols + 1)) * w.max() * eps;
    arma::uvec keep = arma::find(w > r.threshold);
    r.rank = int(keep.n_elem);
    // U^T C without copying U
    arma::vec utc = U.t() * C;
    U.reset();
    arma::vec coef = utc.elem(keep) / w.elem(keep);
    X = V.cols(keep) * coef;
    r.ok = true;
    return r;
}

namespace {

int g_threads = 1;
std::function<bool()> g_progressHook;

// y = A^T x for A in CSC format: one dot product per column, parallel over columns.
void mulTransposed(const arma::sp_mat &A, const arma::vec &x, arma::vec &y, int nt)
{
    (void)nt;   // unused without OpenMP
    const arma::uword *cp = A.col_ptrs;
    const arma::uword *ri = A.row_indices;
    const double *val = A.values;
    const double *px = x.memptr();
    double *py = y.memptr();
    const long n = long(A.n_cols);
SPEC_OMP(omp parallel for schedule(static) num_threads(nt) if(nt > 1))
    for (long j = 0; j < n; ++j) {
        double s = 0.0;
        for (arma::uword k = cp[j]; k < cp[j + 1]; ++k)
            s += val[k] * px[ri[k]];
        py[j] = s;
    }
}

// y = A x - a*y, returns |y|. At = A^T (CSC), so that each row of A is a column.
double mulSubNorm(const arma::sp_mat &At, const arma::vec &x, double a, arma::vec &y, int nt)
{
    (void)nt;   // unused without OpenMP
    const arma::uword *cp = At.col_ptrs;
    const arma::uword *ri = At.row_indices;
    const double *val = At.values;
    const double *px = x.memptr();
    double *py = y.memptr();
    const long n = long(At.n_cols);
    double ss = 0.0;
SPEC_OMP(omp parallel for schedule(static) reduction(+:ss) num_threads(nt) if(nt > 1))
    for (long i = 0; i < n; ++i) {
        double s = 0.0;
        for (arma::uword k = cp[i]; k < cp[i + 1]; ++k)
            s += val[k] * px[ri[k]];
        const double v = s - a * py[i];
        py[i] = v;
        ss += v * v;
    }
    return std::sqrt(ss);
}

void scale(arma::vec &y, double f, int nt)
{
    (void)nt;   // unused without OpenMP
    double *p = y.memptr();
    const long n = long(y.n_elem);
SPEC_OMP(omp parallel for schedule(static) num_threads(nt) if(nt > 1))
    for (long i = 0; i < n; ++i)
        p[i] *= f;
}

// v = M^T u - b*v (without a separate loop for M^T: tmp), returns |v|
double mulTSubNorm(const arma::sp_mat &M, const arma::vec &u, double b, arma::vec &v, arma::vec &tmp, int nt)
{
    (void)nt;   // unused without OpenMP
    mulTransposed(M, u, tmp, nt);
    double *pv = v.memptr();
    const double *pt = tmp.memptr();
    const long n = long(v.n_elem);
    double ss = 0.0;
SPEC_OMP(omp parallel for schedule(static) reduction(+:ss) num_threads(nt) if(nt > 1))
    for (long j = 0; j < n; ++j) {
        const double x = pt[j] - b * pv[j];
        pv[j] = x;
        ss += x * x;
    }
    return std::sqrt(ss);
}

// X += c1*w; w = v - c2*w; returns |X|
double updateXW(arma::vec &X, arma::vec &w, const arma::vec &v, double c1, double c2, int nt)
{
    (void)nt;   // unused without OpenMP
    double *px = X.memptr(), *pw = w.memptr();
    const double *pv = v.memptr();
    const long n = long(X.n_elem);
    double ss = 0.0;
SPEC_OMP(omp parallel for schedule(static) reduction(+:ss) num_threads(nt) if(nt > 1))
    for (long j = 0; j < n; ++j) {
        px[j] += c1 * pw[j];
        ss += px[j] * px[j];
        pw[j] = pv[j] - c2 * pw[j];
    }
    return std::sqrt(ss);
}

} // namespace

void setThreads(int n)
{
    g_threads = std::max(1, n);
    setBlasThreads(g_threads);
}

int threads()
{
    return g_threads;
}

bool openmpAvailable()
{
#ifdef _OPENMP
    return true;
#else
    return false;
#endif
}

int lsqrThreads(arma::uword rows)
{
    const arma::uword n = rows / kRowsPerThread;
    return int(std::max<arma::uword>(1, std::min<arma::uword>(n, arma::uword(g_threads))));
}

void setProgressHook(std::function<bool()> hook)
{
    g_progressHook = std::move(hook);
}

void keepAlive()
{
    if (!g_progressHook) return;
    using Clock = std::chrono::steady_clock;
    static Clock::time_point last = Clock::now();
    if (Clock::now() - last < std::chrono::milliseconds(100)) return;
    g_progressHook();
    last = Clock::now();
}

SolveResult solveLsqr(const arma::sp_mat &M, const arma::vec &C, arma::vec &X, double tol, int maxIter,
                      const arma::vec *x0, const arma::vec *colDamp)
{
    // LSQR after Paige & Saunders (1982), without damping, starting at x = 0:
    // converges to the minimum-norm least-squares solution.
    // Warm start (x0): solve M d = C - M x0 for the correction d (minimum norm), X = x0 + d.
    // Matrix-vector products and vector operations run on threads() cores.
    SolveResult r;
    r.method = "LSQR";
    // One thread per kRowsPerThread rows: with fewer rows, distributing to (passively
    // waiting) threads costs more than it gains (2 cores, 45 spectra: 45 000 rows 92 -> 101 ms,
    // 96 660 rows 354 -> 283 ms, 270 000 rows 1.50 -> 1.34 s).
    const int nt = lsqrThreads(M.n_rows);
#ifdef _OPENMP
    {   // threads the runtime actually gives (OMP_THREAD_LIMIT, nested regions, ...)
        int used = 1;
#pragma omp parallel num_threads(nt) if(nt > 1)
        {
#pragma omp single
            used = omp_get_num_threads();
        }
        r.threads = used;
    }
#endif
    using Clock = std::chrono::steady_clock;
    Clock::time_point lastHook = Clock::now();
    const arma::uword m = M.n_cols;
    if (maxIter <= 0) maxIter = int(std::max<arma::uword>(1000, 4 * m));
    X.zeros(m);
    if (M.n_rows != C.n_elem) {
        r.message = "LSQR: dimensions of M and C do not match.";
        return r;
    }
    M.sync();
    const arma::sp_mat Mt = M.t();     // rows of M as columns (for M*v)
    const bool warm = x0 != nullptr && x0->n_elem == m;
    // damping of single columns: augmented rows d_k x_k = 0, second part u2 of u
    const bool damped = colDamp != nullptr && colDamp->n_elem == m && arma::any(*colDamp != 0.0);
    arma::vec u2;
    if (damped) u2 = warm ? arma::vec(-(*colDamp) % (*x0)) : arma::vec(m, arma::fill::zeros);

    arma::vec u = warm ? arma::vec(C - M * (*x0)) : C;
    double beta = damped ? std::sqrt(arma::dot(u, u) + arma::dot(u2, u2)) : arma::norm(u);
    if (!(beta > 0)) {      // C = 0 (or x0 solves exactly): solution 0 (or x0)
        if (warm) X = *x0;
        r.ok = true;
        return r;
    }
    scale(u, 1.0 / beta, nt);
    if (damped) u2 /= beta;
    arma::vec v(m, arma::fill::zeros), tmp(m);
    double alfa = mulTSubNorm(M, u, 0.0, v, tmp, nt);
    if (damped) { v += (*colDamp) % u2; alfa = arma::norm(v); }
    if (!(alfa > 0)) {      // C orthogonal to the range of M: solution 0 (or x0)
        if (warm) X = *x0;
        r.ok = true;
        return r;
    }
    scale(v, 1.0 / alfa, nt);
    arma::vec w = v;

    const double bnorm = beta;
    double phibar = beta, rhobar = alfa, anorm = 0.0;
    double rnorm = beta, arnorm = alfa * beta;

    for (int itn = 1; itn <= maxIter; ++itn) {
        beta = mulSubNorm(Mt, v, alfa, u, nt);          // u = M v - alfa u
        if (damped) {
            u2 = (*colDamp) % v - alfa * u2;
            beta = std::sqrt(beta * beta + arma::dot(u2, u2));
            if (beta > 0) u2 /= beta;
        }
        if (beta > 0) scale(u, 1.0 / beta, nt);
        anorm = std::sqrt(anorm * anorm + alfa * alfa + beta * beta);
        alfa = mulTSubNorm(M, u, beta, v, tmp, nt);     // v = M^T u - beta v
        if (damped) { v += (*colDamp) % u2; alfa = arma::norm(v); }
        if (alfa > 0) scale(v, 1.0 / alfa, nt);

        const double rho = std::hypot(rhobar, beta);
        const double cs = rhobar / rho, sn = beta / rho;
        const double theta = sn * alfa;
        rhobar = -cs * alfa;
        const double phi = cs * phibar;
        phibar = sn * phibar;

        const double xnorm = updateXW(X, w, v, phi / rho, theta / rho, nt);

        rnorm = phibar;
        arnorm = alfa * std::fabs(sn * phi);
        r.iterations = itn;

        // GUI events / abort about every 0.1 s (the clock is read every 8th iteration only)
        if (g_progressHook && (itn & 7) == 0) {
            const Clock::time_point now = Clock::now();
            if (now - lastHook >= std::chrono::milliseconds(100)) {
                if (g_progressHook()) {
                    r.aborted = true;
                    r.message = "LSQR aborted.";
                    X.zeros();
                    return r;              // ok = false: no solution
                }
                lastHook = Clock::now();
            }
        }

        // Stopping criteria as in LSQR: compatible system or least-squares optimum
        const double test1 = rnorm / bnorm;
        const double test2 = arnorm / (anorm * rnorm + 1e-300);
        if (test1 <= tol + tol * anorm * xnorm / bnorm || test2 <= tol || alfa == 0.0) {
            r.ok = true;
            break;
        }
    }
    if (!r.ok)
        r.message = "LSQR did not converge within " + std::to_string(maxIter) + " iterations.";
    if (warm)
        X += *x0;
    r.ok = true;            // solution is usable even without convergence; message points this out
    return r;
}

arma::vec transferSolution(const Layout &from, const arma::vec &X, const Layout &to)
{
    arma::vec Y(to.cols(), arma::fill::zeros);
    if (int(X.n_elem) != from.cols() || from.cols() == 0)
        return Y;
    // component columns at the same rest-frame pixel; outside the old range the nearest edge value
    auto copy = [&](int nFrom, int startFrom, int x0From, int nTo, int startTo, int x0To) {
        if (nFrom <= 0 || nTo <= 0)
            return;
        for (int k = 0; k < nTo; ++k) {
            int j = x0To + k - x0From;
            j = std::min(std::max(j, 0), nFrom - 1);
            Y(startTo + k) = X(startFrom + j);
        }
    };
    copy(from.nA, 0, from.x0A, to.nA, 0, to.x0A);
    copy(from.nB, from.startB(), from.x0B, to.nB, to.startB(), to.x0B);
    if (from.tPix.empty() && to.tPix.empty())
        copy(from.nT, from.startT(), from.x0T, to.nT, to.startT(), to.x0T);
    else if (from.nT > 0 && to.nT > 0) {
        // static lines in ranges: same rest pixel; pixels not in the old layout start at 0
        std::vector<int> fromPix(from.tPix);
        if (fromPix.empty()) for (int k = 0; k < from.nT; ++k) fromPix.push_back(from.x0T + k);
        for (int k = 0; k < to.nT; ++k) {
            const int p = to.tPix.empty() ? to.x0T + k : to.tPix[size_t(k)];
            const auto it = std::lower_bound(fromPix.begin(), fromPix.end(), p);
            if (it != fromPix.end() && *it == p) Y(to.startT() + k) = X(from.startT() + int(it - fromPix.begin()));
        }
    }
    return Y;
}

arma::mat offsetNullspace(const Layout &L, const arma::sp_mat &M, bool ramps)
{
    // Candidate directions per component (A, B, static lines T): a constant and a linear
    // ramp over the component's pixel axis. Combinations that M maps to (numerically)
    // zero are null directions of M that the data cannot determine:
    //  - constant offsets A + a, B + b (and T + t) with wA a + wB b (+ wT t) = 0 when the
    //    light ratios are the same in all spectra;
    //  - with static lines additionally linear ramps: a ramp shifted by s_j pixels changes
    //    by a constant per spectrum, and for a Keplerian orbit the shifts of B are a linear
    //    function of those of A, so ramps in A and B with an opposite ramp in T cancel.
    // A minimum-norm solution has no component along them; after a warm start it is
    // inherited from the start vector, so it is removed here.
    //  - with excluded wavelength ranges (rows removed): columns occupied only by them are
    //    empty and cut a component into pieces; each piece has its own constant (and ramp)
    //    candidate, so that the offsets of the pieces against each other are found as well.
    //    Without this they would drift apart over warm-started solves (steps at the ranges).
    // The candidates have disjoint supports: sparse arithmetic (G = M E has about as many
    // entries as M), also for long spectra.
    M.sync();
    std::vector<arma::uword> eRow, eCol;
    std::vector<double> eVal;
    int k = 0;
    auto addRun = [&](int start, int n) {                 // columns [start, start + n)
        for (int i = 0; i < n; ++i) {
            eRow.push_back(arma::uword(start + i));
            eCol.push_back(arma::uword(k));
            eVal.push_back(1.0);
        }
        ++k;
        if (n < 3 || !ramps) return;
        for (int i = 0; i < n; ++i) {
            eRow.push_back(arma::uword(start + i));
            eCol.push_back(arma::uword(k));
            eVal.push_back((i - 0.5 * (n - 1)) / n);      // rest-frame (A, B) or observed (T) pixel
        }
        ++k;
    };
    auto add = [&](int start, int n) {                    // one component: its non-empty runs
        if (n <= 0) return;
        int i = 0;
        while (i < n) {
            while (i < n && M.col_ptrs[start + i + 1] == M.col_ptrs[start + i]) ++i;
            const int a = i;
            while (i < n && M.col_ptrs[start + i + 1] != M.col_ptrs[start + i]) ++i;
            if (i > a) addRun(start + a, i - a);
        }
    };
    add(0, L.nA);
    add(L.startB(), L.nB);
    add(L.startT(), L.nT);
    if (k < 2)
        return arma::mat(L.cols(), 0);
    arma::umat loc(2, eRow.size());
    for (size_t i = 0; i < eRow.size(); ++i) { loc(0, i) = eRow[i]; loc(1, i) = eCol[i]; }
    const arma::sp_mat E(loc, arma::vec(eVal), arma::uword(L.cols()), arma::uword(k));
    arma::sp_mat G = M * E;
    // normalise the candidates so that the eigenvalue threshold is scale free (before forming
    // G'G, for its accuracy)
    arma::vec sc(k);
    for (int i = 0; i < k; ++i) {
        sc(i) = arma::norm(arma::vec(G.col(i)));
        if (!(sc(i) > 0)) return arma::mat(L.cols(), 0);
    }
    G = G * arma::sp_mat(arma::diagmat(1.0 / sc));
    const arma::mat GtG(G.t() * G);
    arma::vec lam;
    arma::mat U;
    if (!arma::eig_sym(lam, U, GtG))
        return arma::mat(L.cols(), 0);
    // threshold: relative singular value 1e-7, far below any determined direction but
    // above rounding (exact null directions give ~1e-16; the ramp direction is only
    // nearly null because the shifts are linear in log(1 + v/c), not in v)
    const arma::mat Ed(E);
    arma::mat N(L.cols(), 0);
    for (int j = 0; j < k; ++j) {
        if (lam(j) > 1e-14)
            continue;
        const arma::vec n = Ed * (U.col(j) / sc);
        N.insert_cols(N.n_cols, n);
    }
    return N;
}

size_t dropRows(arma::sp_mat &M, const std::vector<char> &marked)
{
    if (marked.size() != M.n_rows)
        return 0;
    M.sync();
    size_t kept = 0, dropped = 0;
    for (arma::uword k = 0; k < M.n_nonzero; ++k)
        (marked[M.row_indices[k]] ? dropped : kept) += 1;
    if (dropped == 0)
        return 0;
    arma::umat loc(2, kept);
    arma::vec val(kept);
    size_t j = 0;
    for (arma::uword c = 0; c < M.n_cols; ++c) {
        for (arma::uword k = M.col_ptrs[c]; k < M.col_ptrs[c + 1]; ++k) {
            const arma::uword r = M.row_indices[k];
            if (marked[r]) continue;
            loc(0, j) = r;
            loc(1, j) = c;
            val(j) = M.values[k];
            ++j;
        }
    }
    M = arma::sp_mat(loc, val, M.n_rows, M.n_cols, false, false);   // column order: already sorted
    return dropped;
}

std::vector<char> emptyColumns(const arma::sp_mat &M)
{
    M.sync();
    std::vector<char> e(M.n_cols, 0);
    for (arma::uword c = 0; c < M.n_cols; ++c)
        e[c] = M.col_ptrs[c + 1] == M.col_ptrs[c];
    return e;
}

int removeOffsetNullspace(const Layout &L, const arma::sp_mat &M, arma::vec &X)
{
    if (int(X.n_elem) != L.cols())
        return 0;
    const arma::mat N = offsetNullspace(L, M);
    if (N.n_cols == 0)
        return 0;
    arma::mat Q, R;
    arma::qr_econ(Q, R, N);
    X -= Q * (Q.t() * X);
    return int(N.n_cols);
}

void removeAlong(const arma::mat &N, arma::vec &X)
{
    if (N.n_cols == 0 || N.n_rows != X.n_elem)
        return;
    arma::mat Q, R;
    arma::qr_econ(Q, R, N);
    X -= Q * (Q.t() * X);
}

double continuumLevel(const arma::vec &v, arma::uvec *kept)
{
    if (v.n_elem == 0) return 0.0;
    double m = arma::median(v);
    arma::uvec k = arma::regspace<arma::uvec>(0, v.n_elem - 1);
    for (int it = 0; it < 30; ++it) {
        const arma::vec cur = v.elem(k);
        const arma::vec up = cur.elem(arma::find(cur > m));
        if (up.n_elem < 3) break;
        const double s = std::sqrt(arma::mean(arma::square(up - m)));
        const arma::uvec nk = arma::find(v > m - 1.5 * s && v < m + 4.0 * s);
        if (nk.n_elem < 3) break;
        const double nm = arma::mean(v.elem(nk));
        const bool same = nk.n_elem == k.n_elem && arma::all(nk == k);
        k = nk;
        m = nm;
        if (same) break;
    }
    if (kept) *kept = k;
    return m;
}

bool levelComponents(const Layout &L, const arma::sp_mat &M, const arma::mat &N, arma::vec &X)
{
    removeAlong(N, X);
    if (N.n_cols == 0 || L.nA < 3 || L.nB < 3 || int(X.n_elem) != L.cols()) return false;
    // well-covered columns
    arma::uvec cnt(L.cols());
    for (int c = 0; c < L.cols(); ++c) cnt(c) = M.col_ptrs[c + 1] - M.col_ptrs[c];
    auto interior = [&](int start, int n) {
        const arma::uvec c = cnt.subvec(start, start + n - 1);
        arma::uvec idx = arma::find(c >= 0.9 * double(c.max()));
        if (idx.n_elem < 3) idx = arma::regspace<arma::uvec>(0, n - 1);
        return arma::uvec(idx + start);
    };
    const arma::uvec iA = interior(0, L.nA), iB = interior(L.startB(), L.nB);
    const bool withT = L.nT >= 3;
    const arma::uvec iT = withT ? interior(L.startT(), L.nT) : arma::uvec();
    // X is orthogonal to the span of N: minimise ||X + N c||^2 = ||X||^2 + c' G c subject to
    // conditions D c = -d, linear in c for fixed continuum masks:
    //   1. level_A = level_B;
    //   2. with static lines: level_T = 0 (the static spectrum holds only the lines; with the
    //      light fractions of A and B adding up to one, A and B then lie at about 1);
    //   3. with static lines: slope of the continuum of T = 0 (the ramp null direction would
    //      otherwise tilt all three continua).
    // Solution c = -G^-1 D' (D G^-1 D')^-1 d. Conditions that cannot be met along N (e.g. the
    // light fraction of the static spectrum differs between the spectra) are dropped from the
    // end.
    const arma::mat G = N.t() * N;
    arma::mat Gi;
    if (!arma::inv_sympd(Gi, G) && !arma::pinv(Gi, G)) return false;
    arma::vec Y = X;
    bool done = false;
    for (int it = 0; it < 10; ++it) {
        auto mask = [&](const arma::uvec &idx) {
            arma::uvec k;
            continuumLevel(arma::vec(Y.elem(idx)), &k);
            return arma::uvec(idx.elem(k));
        };
        const arma::uvec mA = mask(iA), mB = mask(iB);
        const arma::uvec mT = withT ? mask(iT) : arma::uvec();
        // linear functionals of a vector v
        auto meanOf = [&](const arma::vec &v, const arma::uvec &m) { return arma::mean(v.elem(m)); };
        arma::vec pT;
        if (withT) { pT = arma::conv_to<arma::vec>::from(mT); pT -= arma::mean(pT); }
        auto slopeT = [&](const arma::vec &v) { return arma::dot(pT, v.elem(mT)) / arma::dot(pT, pT); };
        const int nc = withT ? 3 : 1;
        arma::mat D(nc, N.n_cols);
        arma::vec d(nc);
        auto fill = [&](const arma::vec &v, arma::vec &out) {
            out(0) = meanOf(v, mA) - meanOf(v, mB);
            if (withT) { out(1) = meanOf(v, mT); out(2) = slopeT(v); }
        };
        fill(X, d);
        for (arma::uword j = 0; j < N.n_cols; ++j) {
            arma::vec col(nc);
            fill(arma::vec(N.col(j)), col);
            D.col(j) = col;
        }
        arma::vec c;
        for (int k = nc; k >= 1 && c.n_elem == 0; --k) {
            const arma::mat Dk = D.rows(0, k - 1);
            const arma::mat Q = Dk * Gi * Dk.t();
            if (Q(0, 0) > 1e-300 && arma::rcond(Q) > 1e-10)
                c = -Gi * Dk.t() * arma::solve(Q, arma::vec(d.subvec(0, k - 1)));
        }
        if (c.n_elem == 0) return false;
        const arma::vec Yn = X + N * c;
        const bool conv = arma::norm(Yn - Y, "inf") < 1e-12 * std::max(1.0, arma::norm(Yn, "inf"));
        Y = Yn;
        done = true;
        if (conv) break;
    }
    if (done) X = Y;
    return done;
}

SolveResult solve(const arma::sp_mat &M, const arma::vec &C, arma::vec &X, Solver solver,
                  const arma::vec *x0, const arma::vec *colDamp)
{
    return solver == Solver::SvdDc ? solveSvdDc(M, C, X) : solveLsqr(M, C, X, 1e-12, 0, x0, colDamp);
}

} // namespace dis
