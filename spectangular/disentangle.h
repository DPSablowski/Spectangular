#ifndef DISENTANGLE_H
#define DISENTANGLE_H

// Core of the disentangling, without GUI and without global variables:
// construction of the (sparse) transformation matrix and least-squares solution
// with LSQR or truncated SVD.
//
// Model: observation a, pixel i (log10-lambda grid with step dlog)
//   C(a*logbin + i) = wA[a] * A(i - sA[a]) + wB[a] * B(i - sB[a]) + wT[a] * T(i - sT[a])
// (with wArow / wBrow the weights of A and B depend on the pixel as well)
// with the Doppler shift in pixels s = log10(1 + v/c) / dlog
// (positive RV = redshift). A and B are interpolated between whole pixels, linearly
// (2 pixels) or cubically (Catmull-Rom, 4 pixels); T (static lines) is unshifted (sT = 0,
// spectra in the frame of the static lines, e.g. tellurics in the observatory frame) or, with
// rvT, shifted like A and B (e.g. tellurics in barycentrically corrected spectra: rvT = BC).

#include <armadillo>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace dis {

constexpr double kSpeedOfLight = 299792.458;   // km/s

// Shift in pixels for the velocity v [km/s].
double pixelShift(double v, double dlog);

enum class Interpolation { Linear, Cubic };

struct MatrixSpec {
    int num = 0;                   // number of spectra
    int logbin = 0;                // pixels per spectrum
    double dlog = 0.0;             // grid step in log10(lambda)
    std::vector<double> rvA, rvB;  // km/s, per spectrum (rvB only for SB2)
    std::vector<double> wA, wB, wT;  // weights (light fractions) per spectrum
    std::vector<double> rvT;       // km/s per spectrum for T; empty: T unshifted
    // optional: T only at rest wavelengths in these ranges (log10 lambda, frame of T, i.e. the
    // observatory frame); w0 = log10 lambda of pixel 0 of the observation grid. Empty: all.
    std::vector<std::pair<double, double>> tRanges;
    double w0 = 0.0;
    // optional, with tRanges: group of each range (0 .. G-1) and the weights of T per group and
    // spectrum, wTgroup[g][a] (e.g. O2 and H2O with their own strength); replace wT
    std::vector<int> tRangeGroup;
    std::vector<std::vector<double>> wTgroup;
    // optional: weights of A and B per pixel (row a*logbin + i), e.g. for a wavelength-
    // dependent flux ratio; if not empty they replace wA / wB
    std::vector<double> wArow, wBrow;
    bool sb2 = true;
    bool telluric = false;
    Interpolation interp = Interpolation::Linear;
};

// Column layout of the matrix: [A | B | T]
struct Layout {
    int num = 0, logbin = 0;
    int nA = 0, nB = 0, nT = 0;    // columns per component
    int x0A = 0, x0B = 0, x0T = 0; // rest pixel position of the first column of A, B or T
    std::vector<int> tPix;         // T restricted to ranges: rest pixel of each T column (else empty)
    std::vector<int> tGroup;       // with groups: group of each T column (else empty)
    int rows() const { return num * logbin; }
    int cols() const { return nA + nB + nT; }
    int startB() const { return nA; }
    int startT() const { return nA + nB; }
};

// Builds M (sparse, rows x cols). Every column is occupied.
Layout buildMatrix(const MatrixSpec &spec, arma::sp_mat &M);

// log10(lambda) of column k of component comp ('A', 'B', 'T');
// w0 = log10(lambda) of the first pixel of the observation grid.
double columnLog10Lambda(const Layout &L, char comp, int k, double w0, double dlog);

struct SolveResult {
    bool ok = false;
    std::string method;
    int rank = 0;                  // SVD: number of singular values used
    double threshold = 0.0;        // SVD: threshold
    int iterations = 0;            // LSQR
    std::string message;           // error or note (e.g. no convergence)
    bool aborted = false;          // LSQR: aborted by the progress hook
    int threads = 1;               // LSQR: threads actually running the parallel loops
};

enum class Solver { Lsqr, SvdDc };

// Number of cores for the solvers. LSQR: own OpenMP loops (program built with
// -fopenmp). SVD (DaC): passed on to OpenBLAS/BLIS/MKL, if loaded.
void setThreads(int n);
// LSQR uses one thread per kRowsPerThread rows (spectra x pixels), at most threads():
// with fewer rows per thread, waking the (passively waiting) threads for each of the
// parallel loops costs more than it gains.
constexpr arma::uword kRowsPerThread = 25000;
int threads();
// true if the program was compiled with OpenMP (otherwise LSQR always uses one core)
bool openmpAvailable();
// Threads LSQR uses for a matrix with this number of rows
int lsqrThreads(arma::uword rows);

// Called by LSQR about every 0.1 s during a solve (keeps a GUI responsive). Returns true
// to abort the solve (SolveResult: ok = false, aborted = true). Empty: none.
void setProgressHook(std::function<bool()> hook);
// Calls the progress hook if the last call is at least 0.1 s ago (its abort request is
// ignored here): for long loops outside LSQR (matrix construction, writing, plotting).
void keepAlive();
// Returns: number of BLAS libraries found (0: reference BLAS, single-core)
int setBlasThreads(int n);

// Minimum-norm least-squares solution of M X = C.
//
// LSQR (Paige & Saunders 1982): iterative, needs only M*x and M^T*y; memory
// and time per iteration grow linearly with the size. Relative tolerance.
// colDamp (optional, one value per column): minimises |M X - C|^2 + |colDamp % X|^2 (damping
// of single columns, e.g. of the static lines; LSQR on the augmented system [M; diag(d)]).
SolveResult solveLsqr(const arma::sp_mat &M, const arma::vec &C, arma::vec &X,
                      double tol = 1e-12, int maxIter = 0, const arma::vec *x0 = nullptr,
                      const arma::vec *colDamp = nullptr);

// Warm start: transfers a solution X of layout `from` to layout `to` (component columns at
// the same rest-frame pixel; columns outside the old range get the nearest edge value).
arma::vec transferSolution(const Layout &from, const arma::vec &X, const Layout &to);

// Removes from X its component along the null directions of M that consist of constant
// offsets and linear ramps of the components (A, B, static lines), e.g. A + a, B + b with
// wA a + wB b = 0. The minimum-norm solution has no such component; a warm-started one
// inherits it from the start vector. Returns the number of null directions found.
// Removes all entries of the marked rows (masked wavelength ranges): these rows take no part
// in the solution; columns occupied only by them become empty (undetermined). Returns the
// number of removed entries.
size_t dropRows(arma::sp_mat &M, const std::vector<char> &marked);

// Columns without any entry (undetermined, e.g. only in masked ranges)
std::vector<char> emptyColumns(const arma::sp_mat &M);

int removeOffsetNullspace(const Layout &L, const arma::sp_mat &M, arma::vec &X);

// The null directions found by removeOffsetNullspace, as columns (not orthonormal);
// ramps = false: only the constant offsets.
arma::mat offsetNullspace(const Layout &L, const arma::sp_mat &M, bool ramps = true);

// X minus its orthogonal projection onto the span of the columns of N.
void removeAlong(const arma::mat &N, arma::vec &X);

// Continuum level of a spectrum: mean of the pixels on the upper envelope. Iteratively,
// pixels below m - 1.5 s are rejected (lines), with s the RMS of the pixels above m (the
// noise of the continuum, little affected by lines) and m the mean of the kept pixels;
// pixels above m + 4 s are rejected as spikes. Start: median. For pure noise the result is
// about 0.2 sigma above the true level. Returns the level and the mask of the kept pixels.
double continuumLevel(const arma::vec &v, arma::uvec *kept = nullptr);

// Moves X along the null directions N (columns, same space as X) so that the continuum
// levels of A and B become equal; since the data are normalised and the light fractions add
// up to one, both then lie at about 1. With static lines the continuum of the static
// spectrum is put at 0 and made flat (it holds only the lines), if the null directions
// allow it. The model
// M X, and so the fit, does not change. The levels are taken over the well-covered pixels
// (column occupied by at least 90 % of the maximum number of rows), not over the poorly
// determined edges. Remaining freedom: minimum norm. Without B (SB1) or without a null
// direction that changes the difference of the levels: removeAlong only.
// Returns true if the levels were equalised.
bool levelComponents(const Layout &L, const arma::sp_mat &M, const arma::mat &N, arma::vec &X);

// Truncated SVD (divide and conquer, svd_econ "dc") with the threshold from
// Numerical Recipes: 0.5 sqrt(m+n+1) w_max eps. Works on a dense copy
// of M (memory rows*cols*8 bytes in addition to U and V).
SolveResult solveSvdDc(const arma::sp_mat &M, const arma::vec &C, arma::vec &X);

// x0: optional start vector for LSQR (warm start, ignored by SVD)
SolveResult solve(const arma::sp_mat &M, const arma::vec &C, arma::vec &X, Solver solver,
                  const arma::vec *x0 = nullptr, const arma::vec *colDamp = nullptr);

} // namespace dis

#endif // DISENTANGLE_H
