# Tests for Spectangular

Fast tests of individual program steps on reduced data. They safeguard the refactoring:
each unit is first pinned down by tests, then refactored, then checked again.

Total run time approx. 2 min (without `--slow`). One evaluation on `bin4` takes well under 1 s.

## Layout

| Path | Contents |
| --- | --- |
| `adapter/spectest.cpp` | Test driver. Calls individual slots of `MainWindow` and writes the results as text files. The only place that knows internals. |
| `adapter/spectest.pro` | qmake project; `SRC=` points to the source folder. |
| `run_tests.py` | Test run: fresh copy of the data per case, comparison with references, expected-value checks. |
| `tools/make_testdata.py` | Creates `data/bin4` and `data/bin8_n10` from `TestData`. |
| `tools/make_synthetic.py` | Synthetic data sets with known solution (`synthetic`, `synthetic_subpixel`, `synthetic_orbit`, `correlation`). |
| `unit/` | Unit tests of the GUI-free modules (separate qmake project). |
| `tools/make_orbits.py` | Orbital elements and epochs for the Kepler test (`data/orbit`). |
| `data/` | Test data sets. |
| `golden/` | Reference outputs; re-recorded with `--record` for intended changes (reason in the commit message). |

## Data sets

| Name | Spectra | Pixels | dv [km/s] | Purpose |
| --- | --- | --- | --- | --- |
| `bin4` | 20 | 354 | 6.66 | reading data, matrix, SVD, slow optimisation |
| `bin8_n10` | 10 | 177 | 13.33 | short optimisation (3 iterations) |
| `synthetic` | 10 | 177 | 13.33 | known solution, integer pixel shifts, without noise |
| `orbit` | – | – | – | 5 orbits (e = 0 … 0.95), 200 epochs |

Binning: mean over k neighbouring pixels on the log10(λ) grid; the grid stays equidistant.
RVs and times remain unchanged.

## Usage

```bash
# once: build the test driver (SRC = source folder of the program)
mkdir -p build && cd build && qmake ../adapter/spectest.pro && make -j && cd ..

python3 run_tests.py                 # all fast cases
python3 run_tests.py synthetic       # single case
python3 run_tests.py --slow          # incl. optimisation on bin4 (about 1–2 min)
python3 run_tests.py --keep          # keep the work directories for inspection
python3 run_tests.py --record CASE   # record the reference again (only for intended changes)
```

Exit code 0 if everything is PASS or XFAIL.

## Kinds of checks

- **Reference** – output equal to the stored output (numbers with tolerance). Pins down the current
  behaviour, even where it is wrong. During a pure refactoring all of them must stay green.
- **Expected** – mathematically/physically correct result, computed independently of the program.
- **XFAIL** – expected-value check that fails because of a known bug (reference to the
  bug report). When the bug is fixed, **XPASS** appears: remove the marker and re-record the
  affected references with `--record`. Both belong in the same commit.

## Cases

| Case | Data | Checks |
| --- | --- | --- |
| `unit` | – | GUI-free modules: Kepler, table reader, grid check, matrix construction, SVD, Nelder-Mead (quadric, Rosenbrock, shrinking, termination), error estimation (quadric) |
| `config`, `config_roundtrip`, `config_path_space` | bin4 | loading, saving → loading, path with spaces |
| `read_data`, `construct_matrix` | bin4 | characteristic quantities, C, W, RVs, matrix M |
| `divide_conquer`, `divide_conquer_repeat` | bin4 | residuum, solution X, output files; repeatability |
| `svd_check`, `svd_check_synthetic` | bin4, synthetic | X = pinv(M)·C, no empty columns |
| `divide_conquer_svd`, `svd_check_svd` | bin4 | as above with SVD (DaC) instead of LSQR, against the same references |
| `orbit_rv`, `orbit_invalid` | orbit, bin8_n10 | RV against Newton-Kepler (e up to 0.95); penalty residuum for invalid elements |
| `synthetic` | synthetic | residuum ≈ 0 for physical RVs, components and axis against the truth |
| `interpolation` | synthetic_subpixel | residuum minimal at the true RV and continuous |
| `subtract`, `subtract_static` | synthetic | subtracting A yields 0.5·B; no crash with static lines |
| `correlation` | correlation | cross-correlation finds a known shift |
| `optimisation`, `optimisation_bin4` | bin8_n10, bin4 | initial simplex and 3 iterations (reference); `--slow` |
| `read_blank_lines`, `read_short_spectrum`, `read_missing_velocities`, `read_short_velocities`, `read_bad_range` | bin4 modified | blank lines/comments; expected error messages |
| `sb1` | bin4 as SB1 | SB1 run |
| `optimisation_converges` | synthetic_subpixel, perturbed RVs | RV optimisation approaches the truth |
| `optimisation_converges_svd`, `optimisation_converges_weighted` | synthetic_subpixel | the same with SVD (DaC), and with S/N weights estimated automatically from spectra of different noise |
| `orbit_fit`, `error_estimation` | synthetic_orbit | orbit fit finds e, K_A, K_B, ω; error estimation yields standard errors and correlations |
| `orbit_fit_cubic`, `orbit_fit_lm`, `orbit_fit_lm_cubic`, `error_estimation_lm` | synthetic_orbit | the same with cubic interpolation and/or Levenberg-Marquardt (converged, elements found, error estimation on the LM result) |
| `optimisation_converges_lm` | synthetic_subpixel | RV optimisation with Levenberg-Marquardt and cubic interpolation moves the RVs to the truth |
| `mask_tellurics` | synthetic binary 6950–7550 Å with a telluric band of varying depth | without excluded ranges a step of A and B across the band; with the default `telluric_mask.dat` (created in the work folder) no step, no output inside the band, clean gap edges, differences file without the excluded pixels |
| `align_continua` | synthetic binary 5000–5600 Å, 3 % normalisation differences between the spectra | with the continua aligned to the mean spectrum the broad deviation of A and B from the truth falls at least threefold |
| `continuum_norm` | as `mask_tellurics`, band excluded, option continuum-normalised | `*_norm` files beside the raw components on the same wavelengths; upper envelope as that of the truth (< 0.03); renormalisation keeps the composite (B' − B = r (A − A'), < 1e-4) and the residuum (ratio 1 ± 0.01) |
| `mask_optimisation` | as `mask_tellurics`, band excluded, RVs perturbed by 3 km/s, 12 Nelder–Mead iterations | the pieces on both sides of the excluded band keep the same level after warm-started solves (step < 0.1) |
| `cores_warning` | bin4, `SPECTANGULAR_CORES=8` | default 2 cores; note when the user chooses more than 2 (once per crossing), none when the program sets the value |
| `abort_long_spectra` | synthetic binary 4500–6500 Å at 3 km/s (10 × 36 748 pixels) | window responsive during the optimisation (largest gap between two 50 ms timer events < 1 s); Stop after 1 s ends the running LSQR solve |
| `abort_long_spectra_solve` | as `abort_long_spectra` | Stop 0.8 s after Solve ends the running LSQR solve at once ("Solve stopped.", no error box) |
| `lm_reinit` | synthetic_subpixel | Levenberg-Marquardt with Reinitiate: restart from the best point, ends after a run without improvement |
| `continue_method_switch`, `continue_method_switch_orbit` | synthetic_subpixel, synthetic_orbit | Nelder-Mead (New) → Levenberg-Marquardt (Continue, start values in the inputs changed) → Nelder-Mead (Continue, simplex file missing): each run starts from the best state of the previous one (`beststate_*.dat`) |
| `custom_file_names` | synthetic_orbit | own names for the best state, the convergence data (Output tab) and the orbital elements (Data tab), files with the default names of another setup in the same folder: these stay untouched and unused; simplex backups named after the simplex; Continue ignores a best state of the other kind (RVs under the orbit name) and starts from its own; Levenberg-Marquardt writes the elements to its own file |
| `bc_obs_static`, `bc_corrected_static`, `bc_obs_nostatic` | synthetic, generated (16 spectra 6400–6440 Å, 2 km/s per pixel, S/N 400, 287 days, BC ±28 km/s, telluric lines) | orbit mode (Levenberg-Marquardt, e, K_A, K_B, ω free) with the barycentric correction: spectra in the observatory frame with static lines, corrected spectra with static lines (static lines recovered in the observatory frame), observatory frame without static lines: K within 0.3 km/s, e 0.005, ω 0.01 |
| `bc_obs_static_nobc` | as above | without the correction file: note, and the orbit is wrong (K off by more than 2 km/s) |
| `barycorr_file`, `barycorr_file_corrected` | synthetic_orbit (times near transit) | Tools → Barycentric correction, "Write BC file": one value per spectrum, no spectra written, file and frame (observatory / corrected) set in the Data tab; airmass file within 0.001 of Pickering (2002) with astropy altitudes, set as start strength |
| `bc_tell_nofit`, `bc_tell_fit`, `bc_tell_file`, `bc_tell_file_count` | as `bc_obs_static`, telluric strength 0.64–1.37 per spectrum | fixed strength: residuum high; fitted per spectrum: strengths within 0.02 rms, residuum as for noise, orbit found; strengths from a file (true values); file with too few values: Error 49 |
| `res_static_excl`, `res_static_excl_file`, `res_static_noexcl`, `res_static_excl_missing` | 16 corrected spectra with telluric lines (as `bc_tell_fit`), strength 0.64–1.37, one strength in the model, orbit at the truth | residuum over all pixels > 1; without the ranges of the static lines (shifted by BC; also with the ranges read only for the residuum) < 0.5 (noise); missing file: Error 50 |
| `bc_tell_misaligned` | as `bc_tell_fit`, wavelength scale of spectrum 5 shifted by +20 km/s | its strength at the lower bound (> 0, < 0.1) with a warning naming it; the other strengths as the truth |
| `tellcheck`, `tellcheck_corrected` | 10 spectra 6860–6930 Å (O2 B), spectra 3 and 7 shifted by +20 and −5 km/s; observatory frame / corrected with BC from astropy | Check telluric lines: deviations +20, −5 and 0 within 0.3 km/s, exactly 3 and 7 marked |
| `tellcheck_reject` | as `tellcheck_corrected` | Reject marked spectra: `spectram_bc_rejected_3.txt`, `_7.txt`, the others numbered 0–7 in order, times, RV and S/N file without the lines (copies), last index 7, log |
| `tellcheck_noband` | synthetic_orbit (6400–6440 Å) | message: no strong telluric band in the data |
| `bc_tell_groups`, `bc_tell_groups_single` | as `bc_obs_static`, O2 and H2O lines of independent strength, ranges file with groups | two groups: both strengths within 0.03 rms, residuum as for noise, orbit found; ranges without group names (one strength): residuum high |
| `lm_rv_static` | synthetic with static lines | RV mode, Levenberg-Marquardt: the mean RV of each component stays (zero point not determined, also with static lines) |
| `static_long_damp`, `static_long_nodamp` (slow) | generated: 16 spectra 5000–5800 Å, 4 km/s per pixel, 60 static lines, corrected spectra | static lines over a long range: with the default damping fewer than 3500 LSQR iterations and continua of A, B near 1; without more than 4500 |
| `static_ranges` (slow) | as `static_long_*` | Solve over the whole range, Detect: all 60 static lines in the detected ranges (< 15 % of the range); Solve only in these ranges: fewer LSQR iterations, residuum up only by the noise the static spectrum no longer fits; without a file the default list is written; empty file: Error 50 |
| `bc_gamma_free` | as `bc_obs_static` | orbit mode with γ free: note that γ is not determined |
| `bc_corrected_static_nofile`, `bc_wrong_count` | as above | Error 48: static lines in corrected spectra without the file; file with one value too few |
| `error_collapsed` | synthetic_orbit | error estimation after full convergence (final simplex collapsed to a point) |
| `branch_ratio`, `branch_static`, `branch_static_svd`, `branch_edit_weights`, `branch_sb1`, `branch_few_spectra` | synthetic, regenerated | F_A/F_B = 2, static lines, weights from `edit.dat`, SB1, 4 spectra: residuum ≈ 0, components = truth |
| `opt_ratios` | synthetic, ratios per spectrum | "Opt. Ratios" finds the flux ratios; `--slow` |
| `fits_read`, `fits_solve`, `fits_float32`, `fits_bad_extension` | bin4 as FITS | identical to ASCII; float32 columns; wrong extension → Error 32 |
| `linear_grid_resample`, `linear_grid_auto` | bin4 on linear, individually cropped Å grids | resampled onto a common log grid (flux equal to the original within the noise); without the option automatically, with the reason |
| `gui_smoke` | bin8_n10 | all buttons and menu items, auxiliary windows; no crash |
| `screenshot` (not in the suite) | any | images of the main window, after Solve and Plot, of every tab, of the Tools windows and the Subtract panel: `spectest screenshot <dir> <out>`; with `SPECTEST_SHOT_RUN=1` optimisation and error estimation run before (images of the manual) |
| `batch` | bin8_n10 | program with `--batch`: success and missing configuration |
| `config_old_sequence` | bin4 | configuration with the settings of the removed sequence mode loads without error, same widgets as `config` |
| `residual_stats`, `residual_stats_rv` | synthetic orbit | residuals per spectrum against phase (orbit mode) or time (RV mode); a spectrum with an extra emission stands out in its wavelength window; differences plotted as one graph per spectrum |
| `residual_stats_model` | synthetic orbit | as `residual_stats`, window taken from the residuum range of the Model tab |
| `residual_stats_named` | synthetic orbit | as `residual_stats` with an own name for the statistics (Output tab): written there, no `diffstatistic.dat` |
| `fixed_rvs`, `fixed_rvs_lm`, `fixed_rvs_all`, `fixed_rvs_invalid` | synthetic | RVs listed as fixed keep their start values (Nelder-Mead, Levenberg-Marquardt), the others converge; all fixed or invalid list → Error 45 |
| `barycorr` | synthetic orbit | Tools → Barycentric correction (ι Tri, STELLA, JD): one BC per spectrum, corrected spectra log10(λ) + log10(1 + BC/c), start RVs + BC, files used in the main window |
| `flux_templates` | synthetic binary 4500–6500 Å (make_eclipse, no eclipses, S/N 300) | disentangled with the ratio 1.0 instead of 1.5, Tools → Flux ratio from synthetic spectra with the true spectra: ratio within 0.5 %, jackknife error covers the deviation, separate light fractions add up to 1, renormalised components match the truth, ratio taken over |
| `output_name_missing` | synthetic | no output file name for component A (Output tab) → Error 46 before solving |
| `orbit_fit_rvs` | synthetic orbit | Tools → Orbit fit to RVs on the RVs of the main window: elements within 4σ, RMS at the noise, own zero point of B consistent with 0, `orbitelements.dat` written |
| `editor_orbit` | synthetic orbit | the Editor shows the elements of the orbital elements file of the Data tab when it is opened |
| `error_mc` | synthetic orbit, S/N 200 | Monte Carlo error estimation: realisations converged, errors of the same order as the curvature errors |

## Test campaign (`tools/campaign.py`)

Beyond the regression cases: every scenario copies a data set, changes files or settings,
loads the configuration in the driver (case `slots`, steps given in `SPECTEST_SLOTS`, also
`set:widget=value` between the steps) and classifies the outcome as `ok`, `message` (the user
error was caught by a message box), `silent` (a step failed without message, or outputs contain
NaN/inf), `crash` (signal, unexpected exit code or sanitizer report) or `hang`. Each scenario
states what it expects; a deviation is a *finding*.

```bash
python3 tools/campaign.py --part errors  --out DIR   # 146 user errors (also group user_errors of the suite)
python3 tools/campaign.py --part combos  --out DIR   # 87 pairwise combinations of 15 options
python3 tools/campaign.py --part fuzz    --out DIR   # random input into all widgets of all windows (case fuzz)
python3 tools/campaign.py --part meta    --out DIR   # 14 metamorphic relations
python3 tools/campaign.py --only PATTERN ...         # only matching scenarios
```

The results are in `DIR/results.json`. With a driver built with
`-fsanitize=address,undefined` (`SPECTEST=...`) the same scenarios and the whole suite check memory
errors and undefined behaviour.

The group `user_errors` of `run_tests.py` runs the 146 error scenarios (about 20 s).

## Monte Carlo test (long, not part of the suite)

Checks whether the standard errors of the error estimation match the actual scatter:
known eccentric orbit on the full grid of the test data, components from a
disentangling of the real data, Gaussian noise, orbit fit (batch) and error estimation per
realisation (about 70 s).

```bash
python3 tools/make_montecarlo.py <full test data> <folder with SB2_1/SB2_2.txt> mc 40 --snr 65
for d in mc/r*; do (cd $d && Spectangular --batch SB2_conf --overwrite && spectest error_only . errout); done
python3 tools/eval_montecarlo.py mc --plot pulls.png
```

Environment variables: `SPECTEST` (test driver), `UNITTESTS`, `SPECTANGULAR` (program for `batch`),
`SPECTEST_SOLVER=svd` (all cases with SVD (DaC) instead of LSQR).

## After the refactoring

Only `adapter/spectest.cpp` is adapted: instead of slots and global variables it calls the new
classes and writes the same files. Test cases, data and references stay the same.
In addition, the new units get their own, smaller tests (e.g. simplex on test functions).

## flux ratio as a function of wavelength

`tools/make_eclipse.py` generates a synthetic eclipsing SB2 with a Planck flux ratio over a
wide range (description in the manual, appendix A); `tools/eval_eclipse.py` compares the
output components with the truth. Cases: `ratio_const`, `ratio_planck_equal`,
`ratio_table_const` (and `_static`, `_sb1`): a flat shape reproduces the constant ratio;
`ratio_eclipse_planck`, `ratio_eclipse_table`, `ratio_eclipse_constant`: component accuracy
on noise-free eclipse data; `ratio_plotter`: continuum correction "Flux-ratio model" of the
Spectrum Plotter (driver case `plotter_ratio`); `ratio_eclipse_optratios` (slow): flux-ratio
optimisation with the Planck shape; `ratio_table_missing`: Error 44.

