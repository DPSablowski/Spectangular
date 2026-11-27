# spectangular
Spectangular 2.0 - spectral disentangling

# installation

## 1. install the libraries

Spectangular needs Qt 5, Armadillo, CCfits/cfitsio and a C++17 compiler (GCC or Clang). A
multi-threaded BLAS (OpenBLAS) is recommended. With Clang, install `libomp-dev` as well to
use several cores; without it the program is built for one core. The program is always
linked with the compiler that compiles it (a Qt Creator kit with Clang otherwise links with
g++, and the link fails with `undefined reference to __kmpc_global_thread_num`). After
changing the compiler or the system, run qmake again in a clean build folder.

| distribution | command |
| --- | --- |
| Debian, Ubuntu, Mint | `sudo apt install build-essential pkg-config qtbase5-dev libarmadillo-dev libccfits-dev libopenblas0-pthread` |
| Fedora | `sudo dnf install gcc-c++ make pkgconf qt5-qtbase-devel armadillo-devel CCfits-devel openblas` |
| openSUSE | `sudo zypper install gcc-c++ make pkg-config libqt5-qtbase-devel armadillo-devel CCfits-devel openblas-devel` |

`make deps` checks whether everything is found and prints these commands if not.
Tested on Ubuntu 24.04 (Qt 5.15, Armadillo 12.6, CCfits 2.6); the package names for
Fedora and openSUSE are given without a test run.

## 2. build and install

```bash
git clone https://github.com/DPSablowski/Spectangular.git
cd Spectangular
make                  # builds in build/
sudo make install     # installs to /usr/local
```

Then start `Spectangular` from a terminal or from the application menu.

Without root rights, install into your home directory:

```bash
make install PREFIX=$HOME/.local
```

(`$HOME/.local/bin` must be in your `PATH`; most distributions add it automatically.)

`make install` copies:

| file | to |
| --- | --- |
| program | `PREFIX/bin/Spectangular` |
| database of orbital elements | `PREFIX/share/spectangular/BinaryData.dat` |
| manual, README, CHANGELOG, LICENSE | `PREFIX/share/doc/spectangular/` |
| menu entry and icon | `PREFIX/share/applications/`, `PREFIX/share/pixmaps/` |

Remove it again with `sudo make uninstall` (use the same `PREFIX` as for the installation).

Other targets: `make check` (test suite, needs Python 3 with numpy, the FITS tests also
astropy), `make clean`, `make distclean` (deletes `build/`), `make help`.
Variables: `PREFIX`, `DESTDIR` (for packaging), `BUILDDIR`, `QMAKE` (if the Qt 5 qmake is
not found automatically), `JOBS`.

`BinaryData.dat` and the manual are searched in this order: the current folder (so you can
keep an extended copy of `BinaryData.dat` next to your data), `~/.local/share/spectangular`,
the program folder, and the installation folder.

## development with Qt Creator

Open `spectangular/Spectangular.pro` in Qt Creator as before. Development is based on
Qt Creator 4.12.2 and Qt 5.15.

## main window

- **Top row – the workflow:** 1 Read data › 2 Solve › 3 Optimise › 4 Error estimation, Stop,
  and the work folder (set by File → Open Configuration).
- **Left – settings in four tabs:**
  - *Data*: spectra (file name, index range, ASCII/FITS, resampling to a log grid), system
    (SB1/SB2, static lines, flux ratio), the source of the RVs, which also decides what is
    fitted (file or cross-correlation: individual RVs; orbit: orbital elements), noise (S/N,
    weighting) and the values found when reading.
  - *Model*: interpolation, solver and cores, residuum (criterion, edges, wavelength range).
  - *Optimisation*: free orbital elements (ticked = free) or fixed RVs (spectrum numbers),
    method, start (New / Continue / initial file), maximum iterations, Auto Stop,
    Reinitiate, error estimation (curvature or Monte Carlo), flux ratios, line width scan.
  - *Output*: common file name, result files, optimisation files.
- **Right – results in two tabs:** Spectra (components and static lines, the y range
  ignores the outermost pixels, which are determined by single spectra and can be outliers;
  below them the residuals observation − model per spectrum against phase or time and
  against the number of the spectrum, optionally in a wavelength window, also written to
  `diffstatistic.dat`) and Convergence & messages (Δχ²; below it Messages, including the
  error estimation, and RVs side by side).
- **Bottom – status:** iteration, evaluations, best residuum, time; solver, interpolation,
  method and cores in use.
- Tools → Subtract component… opens the subtraction of a component as a panel.
- Tools → Orbit fit to RVs… fits a Keplerian orbit to the RVs of the main window (e.g. after
  an optimisation of individual RVs) or to a table, optionally with an own zero point for B;
  the result can be written to the orbital elements file (Data tab, default `orbitelements.dat`) as start for the orbit mode.
- Tools → Flux ratio from synthetic spectra… fits the flux ratio with a synthetic spectrum for
  each component (broadening, RV shift and a polynomial continuum included) and writes the
  disentangled spectra renormalised to their own continua (`<component>_renorm.<ext>`).
- Tools → Barycentric correction… computes the barycentric correction of every spectrum
  (star coordinates, observatory, times as JD/MJD/HJD/BJD) and writes the spectra shifted to
  the barycentre (and the start RVs + BC). Uses a subset of ERFA (IAU SOFA routines, BSD
  licence), included in `spectangular/erfa`.

Configuration files keep their format. The orbital elements are shown as "free" in the window
and stored as "locked" as before, so old and new files can be exchanged.

## converting spectra (tools/tab2spectangular.py)

Tables with wavelength [Å], flux and error (e.g. STELLA/SES `...botzfxsEcdtab.txt`) are
converted into Spectangular's ASCII input: common range, one common log10(λ) grid
(cubic spline, or flux-conserving averaging for coarser steps), files `<prefix><i>.txt`
in file-name order, plus `<prefix>snr.txt` (S/N per pixel, for "Weight by S/N") and
`<prefix>files.txt` (index → source file). Needs numpy (scipy optional).

    python3 tools/tab2spectangular.py "raw/*_botzfxsEcdtab.txt" -o itri_6400 --prefix itri_ \
            --wmin 6400 --wmax 6500 [--step-kms 4]

The times of observation are not in the tables; write the times file in the order of
`<prefix>files.txt`.

## Build identification

At the start the program prints "Spectangular build <commit>" (console and Messages): the git
commit of the sources, from the checkout (qmake) or, in a source archive made with
`git archive`, filled in by git (`.gitattributes`, export-subst).

## batch mode

```bash
cd <folder with the data and the configuration>
Spectangular --batch SB2_conf      # loads SB2_conf.sps, runs the optimisation, no window
```

Without a name the configuration is asked for on the console. The exit code is 1 on errors.
The folder of the configuration file is used as work path (in the GUI as well: File → Open
Configuration), so keep the configuration next to the data.
Further options: `--overwrite` (overwrite existing start files), `--threads N` (number of cores, default 2).

## weighting by S/N

With "Weight per spectrum by S/N from" (Data tab) checked, each spectrum is weighted with 1/sigma_j^2,
sigma_j = 1/(S/N)_j. Give a file with one S/N value per spectrum (same order as the
spectra), or "auto": the program estimates the S/N of each spectrum from the residuals at
the first solve (three iterations) and keeps it for the whole optimisation. "Solve" always
writes its estimate to `snr_estimated.dat`. The S/N refers to continuum-normalised spectra.

## Auto Stop

The optimisation has converged when chi^2 = residuum^2 / sigma^2 improved by less than
"Stop at Δχ²" (default 0.1) within the last N iterations ("over … it.", auto: 10 per
parameter, at least 200). sigma is 1/(S/N) of the continuum-normalised spectra; with
S/N = auto it is estimated from the residuum (residuum^2 / degrees of freedom).

- Auto Stop on, Reinitiate off: stop at convergence.
- Reinitiate on: restart from the best solution at convergence; with Auto Stop the
  restarts continue as long as a run improves on its start point, and the optimisation ends
  with the first run that converges without improvement.
- Without Auto Stop the maximum number of iterations applies to all restarts together.
- Levenberg-Marquardt has its own convergence rule; with Reinitiate it restarts from its best
  point until a run gains less than 0.002 in chi^2.

## Excluded wavelength ranges (telluric bands)

Model tab, "Exclude ranges from": the pixels in the listed wavelength ranges (frame of the
input spectra) take no part in the solution and the residuum. Use it for telluric bands: the
two-component model cannot describe lines that stay in the observer frame and change with the
airmass, and the least-squares fit then shifts the poorly determined broad level of A against
B across the band (steps in the components). The same holds for chromospheric line cores,
whose strength varies with the activity. Default file `telluric_mask.dat`: if it does not
exist in the work folder it is created with the strongest telluric bands (O2 6274-6331,
6865-6936, 7593-7701 Å; H2O 7159-7341, 8119-8351, 8929-9851 Å; each widened by about 1 Å for
barycentric corrections) and the strong chromospheric lines (Ca II H&K, Hε-Hα, Mg I b,
Na I D, Ca II IR triplet; core half-width 1-3 Å plus the shift for stellar RVs up to
±100 km/s; only the cores, the Balmer wings stay in) and can be edited (per line: from to [Å], further text ignored, `#`
comments). A range cuts the components into pieces; each piece has its own free offset of A
against B, which the program removes after every solve like the global offset (otherwise the
pieces drift apart over an optimisation: steps at the ranges). Remaining jumps at the ranges
are mostly parts of broad wiggles from different normalisations of the observed spectra;
see "Aligning the continua". Component pixels that lose more
than half of their data to the ranges are not written (gaps in the output files), and with "Exclude edges" the pixels next to a range do
not count in the residuum, as at the spectrum edges. A file created by an earlier version (only
telluric bands) is kept; delete it to get the new default list.

## Aligning the continua of the observed spectra (Data tab, Noise)

"Align continua to the mean, smoothing 30 Å": before disentangling, each
spectrum is divided by its ratio to the mean of all spectra, smoothed by a boxcar of the given
width applied twice (excluded ranges left out). Different normalisations of the observed
spectra are the main source of broad anti-correlated wiggles of A and B: the disentangling
cannot attribute them and amplifies them strongly (simulation with ι Tri A-like components
and 3 % normalisation differences on 150-400 Å scales: wiggles of ±0.2 in A and B; aligned:
±0.04, residuum 10.7 -> 2.4). On the real ι Tri A spectra (47 STELLA spectra, 4500-7500 Å)
corrections of only 0.3-0.8 % rms removed the large wiggles and the jump at Hβ. The width must be much larger than the RV shifts of the lines;
real broad differences between the spectra (e.g. shifted Balmer wings) on this scale and
above are removed as well. The console lists the size of the correction per spectrum.

## Continuum-normalised components (Output tab)

"Also write continuum-normalised A and B (*_norm) and plot them": beside the components as
they come from the disentangling (`<name>.<ext>`), the program writes `<name>_norm.<ext>` and
plots these. The renormalisation keeps the fit (suggestion of the user): A' = A - e,
B' = B + r e with r = w_A / w_B, so that the composite w_A A' + w_B B' is the same at every
wavelength and only the smooth e is shifted differently in the two components. e brings the
continua of A and B as close to 1 as possible, weighted by how well each continuum is
defined (a line-rich component follows the other). The continua are cubic splines per piece
(the spectrum is split at excluded ranges) with knots every 80 Å (adjustable), through the
upper points: points more than 1.5 σ below the fit (adjustable) or 3 σ above it are left out,
σ from the points above the fit, iterated. The program computes the residuum with A' and B'
and reports it beside the original one (ι Tri A, 47 spectra: × 1.0006; dividing A and B by
their own continua would give × 2.4). If the light ratio differs between the spectra there is
no such conserving change; A and B are then divided by their continua. The flux ratio itself
is not determined by this; it needs an external reference (synthetic spectra, Tools).

## Barycentric correction

Data tab, "Barycentric correction": a file with BC [km/s] per spectrum (last number of each
line, e.g. `<prefix>barycorr.txt` of Tools → Barycentric correction) and the frame of the
spectra. Observatory frame (as observed): static (telluric) lines at rest, the RVs of the orbit
are converted to the observatory frame, so the orbital elements are barycentric. Corrected
spectra: the RVs are barycentric, the static lines are shifted by BC. Needed for the orbit mode
with static lines: without it the orbit is wrong (synthetic test with BC ±28 km/s: K_A 29
instead of 40 km/s).

Tools → Barycentric correction, "Check telluric lines": the position of the strong telluric
bands in every spectrum against the expected shift (BC_i − BC_0 for corrected spectra, 0 in the
observatory frame). A deviation means a wrong correction, time or wavelength scale of that
spectrum (its stellar lines are shifted as well). "Reject marked spectra" renames these spectra
to `<prefix>bc_rejected_<index><ext>`, numbers the others consecutively and removes their lines
from the times, RV, S/N, BC and strength files and `edit.dat` (copies `*.before_bc_reject`, log
`bc_rejected.txt`). When the strength of the static lines is fitted per spectrum, a spectrum
whose strength falls below 0.1 is named in a warning.

Model tab, Residuum, "Exclude the ranges of the static lines from the residuum": the ranges of
the Data-tab file "Static lines only in" do not count in the residuum (and the residual
statistics and S/N estimate) but stay in the solution; in corrected spectra they are shifted
by BC.

Spectra tab, below the components: RMS and mean of O−M per spectrum against the phase or time and
against the number of the spectrum (outliers labelled).

Optimisation tab, "Line width scan": "Find Lines" shows the spectrum of component A and then of
component B (selectable, one after the other; from the Output-tab files, normalised preferred, or the
current solution) with the absorption lines found; click on a line removes it, click elsewhere
adds one, "Apply" writes both line lists (`scanlines.dat`, sections per component). "Scan orbits" computes the solution for
every orbit of a grid (`orbitgrid.dat`: seven rows P, e, K_A, K_B, γ, T_peri, ω, one column per
orbit) and writes the residuum and the Gaussian FWHM of the mean line profile of each
component, measured in the lines found in that component (and depth ratio and correlation) to `linescan.dat`; window with FWHM and residuum
against the varying element and the mean line profile with its Gaussian fit per orbit (also written,
`linescan_profiles.dat`); optionally the normalised spectra of every orbit are saved; "Show results"
opens the window again from the files. The width of the lines is a sharp criterion for K when the
components are similar.

Tools → Period analysis: Lomb–Scargle periodogram against the time, with spectral window,
false-alarm levels and the orbital frequency marked, of the residual statistics (mean, RMS, std
of O−M), of the fit per spectrum (RV O−C and line strength of A and B, weighted with the errors;
`rv_oc.dat`) or of the statistics per line window (mean, RMS, equivalent width of O−M;
`lines.dat` → `linestat.dat`). "Analyse the current solution" in this dialog writes the last two
files for the current solution; what is written and the file names are set in the Output tab
(group "Analysis of the solution"). "Apply" of the common name sets all file names of the
Output tab, including these.

## Best state and Continue

Every optimisation writes the best values found so far to the file set in "Best state"
(Output tab, default `beststate.dat`); the program adds `_rv` (RVs) or `_orbit` (orbital
elements), e.g. `beststate_rv.dat`. If it exists and belongs to the current mode (and, for RVs,
the same spectra) it is used, otherwise it is created anew. For two setups in one folder give
each its own name; the same holds for the residual statistics, the convergence data (Output
tab) and the orbital elements file (Data tab, default `orbitelements.dat`). "Continue" and every restart
start from these values, whichever method (Nelder-Mead or Levenberg-Marquardt) found them and
whatever start values are in the inputs. Nelder-Mead uses the stored simplex when it matches
the free parameters and the best state; otherwise it builds a new simplex around the best
values. "New" starts from the inputs.

Do not choose the window too small: Nelder-Mead has long phases of slow progress in many
dimensions.

## cores

"Cores" in the Model tab sets the number of cores; default 2 (if the machine has two). In a
scaling test on long spectra (14-core machine) 2 cores were fastest and more took longer;
choosing more than 2 shows a note. Measure on your own data if in doubt. LSQR uses them via OpenMP; the SVD uses
them through the BLAS library, which needs a multi-threaded BLAS such as OpenBLAS
(`libopenblas0-pthread`). LSQR uses one core per 25000 rows (spectra x pixels), at most
the number set: with fewer rows per core, waking the threads costs more than it gains.
45 spectra of 2148 pixels use 3 cores, long spectra all of them. Only the matrix-vector
products of LSQR run in parallel, so `top` shows less than the full number of cores. To run
many fits, start several batch runs side by side instead.

Check: the console prints after every solve `LSQR: … iterations, N threads (rows)`, and the
status line shows "LSQR N used". Built without OpenMP (e.g. Clang without `libomp-dev`;
qmake then prints "OpenMP not available"), the program says so at start and LSQR always uses
one core; `ldd Spectangular | grep omp` shows `libgomp` (GCC) or `libomp` (Clang) when OpenMP
is in. In `top` a process with N busy threads shows up to N·100 % (key `1`: per core).

During long solves LSQR lets the window update about every 0.1 s (no "not responding"
message), and "Stop" ends the running solve, during an optimisation as well as after
"Solve" (message "Solve stopped."). On long spectra each improvement also writes the
solution and redraws the plots (about 1.3 s for 10 x 37000 pixels); switch "Live update" off
to save the plotting.

## solver

The linear system is solved in the least-squares sense (minimum-norm solution).

- **LSQR** (default): iterative, works on the sparse matrix. Memory and time grow
  roughly linearly with the length of the spectra. 20 spectra × 1417 pixels: 0.6 s, 54 MB.
  During an optimisation it starts from the previous solution (warm start, about 10 %
  fewer iterations, same result); `SPECTANGULAR_COLD_START=1` switches this off.
- **SVD (DaC)**: truncated singular value decomposition (divide and conquer) on a dense
  copy of the matrix. Memory grows with the square, time with the cube of the spectrum
  length. 20 spectra × 1417 pixels: about 2.5 min, 2.2 GB. Useful as a reference.

Both give the same result within about 1e-11.

## optimisation method and interpolation

"Method" (Optimisation tab) selects the minimiser, "Interpolation" (Model tab) selects how
the component spectra are shifted between pixels.

- **Nelder-Mead** (default): simplex search as before. Robust from far away, but slow near
  the minimum.
- **Levenberg-Marquardt**: uses the derivatives of the residuals. The component spectra are
  solved for at every evaluation (variable projection). Details:
  - central-difference Jacobian; every column is a warm-started LSQR solve;
  - secant correction of the curvature (NL2SOL) and a line search;
  - stops when less than 0.002 in χ² is left to gain.

  It is a local method and needs a reasonable start, e.g. from a short Nelder-Mead run or
  from good elements. It works on individual RVs and on orbital elements, needs the
  residuum "Mean", and writes a simplex around its result to the optimisation files, so
  that "Error Estimation" and a Nelder-Mead "Continue" can follow directly. Without static
  lines the mean RV of each component stays at its start value in the RV mode (the zero
  point is not determined by disentangling).
- **Linear** interpolation (default) uses 2 pixels. The residuum has a kink whenever a shift
  crosses a whole pixel, and the result is biased by up to about 0.1 pixel.
- **Cubic** interpolation uses 4 pixels (Catmull-Rom). The residuum is smooth in the RVs
  and the model is more accurate for narrow lines, but each solve takes about 3–4× as long.

Measured on the test data (20 spectra × 1417 pixels, one core):

| task | Nelder-Mead | Levenberg-Marquardt |
|---|---|---|
| 40 RVs, linear | 3000 iterations: 248 s, not yet converged (Auto Stop: ~5900) | 10 steps: 44 s, lower residuum |
| orbit (6 elements), cubic, S/N 65, 16 Monte Carlo data sets | 600 iterations: 146 s | 55 s, χ² 0.002–0.014 above Nelder-Mead |

In the Monte Carlo runs the elements, their scatter and the error estimates of both
methods agree within 0.02 σ.

## flux ratio

"Flux ratio F_A/F_B" (Data tab) is the ratio at the reference wavelength λ_ref; per
spectrum it can be set in the Editor (`edit.dat`) or optimised ("Optimise flux ratios").
"λ dependence" makes it depend on wavelength: F_A/F_B = k_j s(λ) with s from two Planck
functions (T_A, T_B) or from a table (λ [Å], ratio). Needed over wide wavelength ranges with
eclipse spectra; without eclipses it changes only the continua of the components, which the
Spectrum Plotter puts on 1 with "Flux-ratio model". Details and tests: manual, sections 2.2
and 9.3.

## conventions

- Radial velocities: positive values are redshifts (`lambda_obs = lambda_rest (1 + v/c)`).
- Component spectra are interpolated between pixels, linearly or cubically (see above); the
  written component spectra are on the rest-frame wavelength grid of each component.
- The disentangling works on one equidistant log10(lambda) grid. Spectra on a linear
  wavelength scale (Angstrom) or on individual log grids are resampled automatically when
  reading (cubic spline onto the common wavelength range, step = finest sampling); tick
  "Resample to a common log grid" (Data tab) to set the step in km/s or to resample always.
- The residuum excludes the outer pixels of each spectrum by default ("Exclude edges":
  RV range in pixels × 1.2 + 2, cubic + 3). There the reconstructed spectra are constrained by single
  spectra only, and the residuum would jump whenever an RV crosses a pixel boundary.
- FITS input: binary table with the two columns given in the main window. The extension
  name is optional; if it is empty or not found, the first table with both columns is used.
- SB1 and SB2: component A is weighted with F_A/(F_A+F_B) from "Flux Ratio A/B".
- The optimisation step for omega in `initialstep.dat` is in radians (the editor shows degrees).
- Configuration files (`.sps`): the first 58 lines keep the old format; further
  settings follow as `name=value`. Old files can still be read.

## manual

`Spectangular_Manual.pdf` (Help → Manual) describes the method, the installation, the
data preparation, every part of the main window and the tools, all files, worked examples
and the error messages. Source: `doc/manual/spectangular_manual.tex`; `make manual`
rebuilds the PDF (needs LaTeX with latexmk and pgfplots).

## documentation of the revision

`doc/changes/spectangular_changes.pdf` describes all changes since version 1.11 with the
test results (source: `spectangular_changes.tex`, build with
`latexmk -pdf spectangular_changes.tex` in that folder).

## tests

`make check` builds the test driver and the unit tests and runs the suite. Details and
single cases: see `tests/README.md`. The suite includes 146 user-error scenarios (group
`user_errors`); `tests/tools/campaign.py` additionally runs pairwise combinations of options,
input fuzzing of all windows and metamorphic relations.
