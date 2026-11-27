# Changelog

All notable changes to Spectangular. Identifiers such as **K1**, **H5** or **M3**
refer to the bug report of 27 October 2026 (K = critical, H = high, M = medium,
N = low). The commit is given in parentheses.

## [2.0] – 2026-11, branch `korrekturen`, based on 1.11 (commit 0485d66)

### Attention: results change

These changes alter numbers and outputs compared with version 1.11. Older results are
therefore not directly comparable.

- **Sign of the radial velocity.** Positive RV now means redshift
  (λ_obs = λ_rest · (1 + v/c)). Previously the sign was reversed. For similar
  amplitudes (K_A ≈ K_B) the components were therefore effectively swapped. In the
  test data the star with the narrow lines at 6716 and 6725 Å is now in B.
  Orbital elements from older fits, for an otherwise identical solution, have γ with the
  opposite sign and ω shifted by 180°. (ae1ef2a)
- **Interpolation instead of whole pixels.** The components are shifted by linear
  interpolation between neighbouring pixels. Previously the shifts were rounded to whole
  pixels, and the objective function was step-shaped in the RVs and orbital elements. (ae1ef2a)
- **Solution of the linear system (K1).** All unknowns are computed (LSQR or
  truncated SVD). Previously unknowns whose index coincided with a small singular value
  were set to 0. The residuum was therefore too high: by 9 % on the test data with
  4× binned spectra. (ae1ef2a)
- **Column layout of the matrix (H1).** Component B no longer starts one column too
  early; there are no more empty columns. (ae1ef2a)
- **Output of the component spectra (H3).** The wavelengths are given in the rest frame
  of the respective component and are computed from the column index. All columns are
  written, including values close to 0. The wavelength axis of A was previously offset by
  one pixel; edge columns were missing. (ae1ef2a)
- **Flux ratio.** F_A/F_B is used consistently, as the field is labelled ("Flux Ratio
  A/B") and as in the editor. The optimisation previously used the reciprocal. For
  ratios ≠ 1 this changes the weights and the normalisation of the components.
  (456bcf8)
- **Residuum.** It is computed over all bins or over the selected wavelength range.
  Previously bins whose model flux was below an SVD threshold were left out (M3).
  (ae1ef2a)
- **Residuum without edge pixels** ("Exclude edges", on by default). For each spectrum the
  outer ⌈1.2 · RV span in pixels⌉ + 2 pixels are not counted. There, individual spectra
  alone determine the edge columns, and the residuum jumped by about σ² whenever an
  extreme RV crossed a pixel boundary. The edge is fixed at the first matrix construction
  after reading the data and stays fixed during an optimisation or error estimation.
  The displayed residua become smaller as a result (test data 4× binned:
  0.861 → 0.829). Switching it off restores the previous residuum. (0c3be01)
- **Results on the test data** (20 spectra, RVs from `velocities.txt`,
  before "Exclude edges"):

  | | 1.11 | now |
  | --- | --- | --- |
  | Residuum, full resolution (1417 pixels) | 3.221 | 2.606 |
  | Residuum, 4× binned | 1.321 | 0.861 |
  | Residuum after orbit fit, 4× binned | 1.099 | 0.859 |
  | Synthetic binary, whole pixels | 0.925 | 5·10⁻¹⁴ |

### Added

- **All plots with a box:** the plots of the period analysis and of Find Lines also have the second axes visible without tick labels and ticks, like the spectrum plot; ticks of the unused second axes are off in all plots.
- **Line width scan** (Optimisation tab, group "Line width scan"). Stage 1 of the line width of a
  component as a criterion for the amplitudes: "Find Lines" opens a window with the spectrum of component A and then B (selectable; lines are defined for both components one after the other; from the component files of the Output tab, the normalised ones if present, otherwise from the current solution) and the absorption lines found (depth against a percentile
  continuum, thresholds and window adjustable); lines are removed or added by a click in the plot,
  X and Y range are set with fields, "Apply" writes both line lists (`scanlines.dat`, sections `# component A/B`, settings as
  comments). "Scan orbits" reads a grid of orbits (`orbitgrid.dat`: seven rows, one column per
  orbit), computes the solution for each and measures the Gaussian FWHM of the mean line profile of each component in the lines found in that same component (and the depth ratio and correlation, as
  in the script for ι Tri B); result file `linescan.dat` and a window with FWHM and residuum
  against the varying element. The result window also shows the mean line profile and its Gaussian fit for each orbit (written to `<result>_profiles.dat`); the continuum-normalised spectra of every orbit can be saved; "Show results" opens the window from the files of an earlier scan. The buttons are disabled and the cursor busy during the scan, a progress bar counts the orbits. The solution before the scan is restored. New module
  `linewidth.cpp/h`, windows in `linefinder.cpp/h`; new Errors 55 (no solution) and 56 (files or
  data mode). Synthetic test: K_B scanned from 33 to 57 km/s, the FWHM of B has its minimum
  (24.1 km/s) at the true 45 km/s and grows to 31 km/s at the ends, like the residuum. Tests:
  12 unit tests, `linescan`, `linescan_edit`, `linescan_nosolution`, `linescan_nogrid`,
  `linescan_nolines`.
- **Analysis of the solution** (Output tab, button "Analyse the current solution"). RV O−C and
  line strength per spectrum: with the component spectra fixed, each spectrum is fitted with its
  own shift and line strength of A and B (Gauss–Newton), with errors; the robust scatter of the
  O−C and spectra beyond 5 times it are reported (`rv_oc.dat`). Statistics per line: mean, RMS
  and equivalent width of O−M per spectrum in the windows of a line list (`lines.dat`, created
  with strong stellar lines; output `linestat.dat`). New Error 54. Synthetic test: a shift of
  1.5 km/s found as 1.36 km/s against the others (1/16 absorbed by the components), a line
  strength 1.3 as 1.26 (expected 1.28), the EW of an emission within 4 %. Tests: `analysis`,
  `analysis_nosolution`, `analysis_lines_missing`.
- **Residuals against the number of the spectrum and period analysis of the residuals.** The
  Convergence & residuals tab shows RMS and mean of O−M per spectrum also against the number of
  the spectrum (orbit mode: second plot beside the one against the phase; RV mode: one plot
  against the number only); outliers (RMS more than 5 × 1.48 MAD above the median) are labelled in both
  plots. New tool Tools → Period analysis of the residuals: generalised Lomb–Scargle periodogram
  (Zechmeister & Kürster 2009) of mean, RMS or std of O−M per spectrum against the time (column
  "time" of the statistics, which is now added when the phase is the abscissa, or the times
  file), outliers left out, spectral window, FAP levels 1 % and 0.1 %, orbital frequency and
  harmonics marked, the five highest peaks with FAP and sine amplitude, data folded with a
  selected period, periodogram written to `<statistics>_periodogram.dat`. Identical to
  astropy's LombScargle on the residuals of ι Tri A. Tests: unit `testPeriod`, `period`,
  `period_timecol`, `period_noise`, `period_nofile`, `residual_stats_outlier`.
- **Residuum without the ranges of the static lines** (Model tab, Residuum: "Exclude the ranges
  of the static lines from the residuum"). The pixels in the ranges of the Data-tab file "Static
  lines only in" (observatory frame, shifted by BC in corrected spectra) do not count in the
  residuum, the residual statistics and the S/N estimate, but stay in the solution (unlike
  "Exclude ranges from", which removes them from the system). The file is read also when the
  static lines or their ranges are off. New Error 53 if the ranges cover all pixels. Synthetic
  test: residuum 1.83 → 0.44 (noise) with telluric lines of varying strength and one strength
  in the model. Tests: `res_static_excl`, `res_static_excl_file`, `res_static_noexcl`,
  `res_static_excl_missing`.
- **Check of the telluric lines and rejection of spectra** (Tools → Barycentric correction).
  "Check telluric lines" cross-correlates the strong telluric bands (O2 B, O2 A, H2O 7200, 8200,
  9300 Å) of every spectrum with the first one and compares the shift with the expected one
  (BC_i − BC_0 for corrected spectra, 0 in the observatory frame); deviations beyond a limit
  (default 2 km/s) or without correlation are marked. "Reject marked spectra" renames them to
  `<prefix>bc_rejected_<index><ext>`, numbers the others consecutively and removes their lines
  from the times, RV, S/N, BC and strength files and edit.dat (copies `*.before_bc_reject`, log
  `bc_rejected.txt`). ι Tri A: spectrum 2 deviates by +23.0 km/s and spectrum 1 by −6.5 km/s
  (wrong correction in the reduction), all others by less than 0.3 km/s.
  Tests: `tellcheck`, `tellcheck_corrected`, `tellcheck_reject`, `tellcheck_noband`.
- **Align the continua to the mean spectrum** (Data tab, Noise; option with smoothing width,
  default 30 Å): divides each spectrum by its smoothed ratio to the mean of all spectra
  before disentangling. Different normalisations of the observed spectra cause most of the
  broad anti-correlated wiggles of A and B. Simulation with ι Tri A-like components and 3 %
  normalisation differences: offsets of the pieces against the truth ±0.20 → ±0.045,
  residuum 10.7 → 2.4. On the real ι Tri A spectra (47 spectra, 4500–7500 Å, RVs fitted to the
  user's model) corrections of 0.3–0.8 % rms removed the large wiggles (A 0.6–1.7 → 1.0–1.35)
  and the jump at Hβ. Tests: unit test (rms 2.1 % → 0.3 % with shifted deep lines),
  `align_continua` (broad deviation of A 0.185 → 0.055, B 0.282 → 0.082).
- **Build identification** at the start ("Spectangular build <commit>"), also in source
  archives (`version.h`, export-subst).
- **Strengths of the static lines fitted in windows** (suggestion tested at the request of the
  user): with "Static lines only in" the alternation of solve and scale runs on a sub-problem,
  the pixels in windows around the ranges widened by the span of the stellar shifts and the
  largest static shift (static weights updated in place), then one full solve. ι Tri A (47 ×
  70 000 pixels, default ranges, O2/H2O): 5.8 min instead of 28 min (10 iterations each; same
  strengths to 0.001, residuum 69.2955 / 69.2953).
- **Groups of static lines with their own strength** (request of the user): the first word
  after the numbers of a range in the ranges file names its group (default list: O2, H2O).
  With "fit per spectrum" each group gets its own strength per spectrum (matrix: weight per
  group and spectrum; regression per group). Synthetic test with O2 and H2O of independent
  strength: one strength per spectrum residuum 1.36, two groups 0.255 (noise), strengths within
  0.003 rms.
- **Static lines only in wavelength ranges** (request of the user): Data tab "Static lines
  only in" with a ranges file (observatory frame). Default `telluric_static.dat` with all
  telluric O2 and H2O systems of the optical, weak ones included; user-editable. "Detect" finds
  the ranges in the static spectrum of the last Solve (threshold in σ of the MAD noise,
  absorption and emission) and writes `<file>_detected.<ext>`. Synthetic test (800 Å, 60 static
  lines): all lines found (49 ranges, 73 Å), LSQR 2 664 → 1 532 iterations. With the generous
  default list over 4500–7500 Å (about 30 % of the range) hardly faster (ι Tri A: 3 870 → 3 830);
  narrow detected ranges save more. Error 50.
- **Damping of the static spectrum (LSQR)** (user report: Solve with static lines over
  4500–7500 Å took more than 15 min): the broad structure of the static spectrum is nearly
  undetermined; LSQR needed 11 720 instead of 1 325 iterations (10.8 min on 2 cores), and A and
  B came out between −2.8 and +4.1 with the opposite in the static spectrum. New Model-tab
  setting "Damping static" (default 1e-3, relative to the squared column norm; LSQR on the
  augmented system with per-column damping): 3 870 iterations (3.9 min), continua near 1,
  residuum +0.1 %. Configurations without the setting get the default.
- **Strength of the static lines per spectrum** (request of the user): Data tab "Static line
  strength" (start values from a file, e.g. the airmass, mean 1) and "fit per spectrum":
  alternating solve and least-squares scale per spectrum (static spectrum without its
  continuum level; without that the constant of the null direction dominated and the scales
  stayed at 1), at Solve, at the start and at every restart of an optimisation (best point),
  written to `<static lines>_strength.<ext>`. Synthetic test with strengths 0.64–1.37:
  residuum 1.80 with a fixed strength, 0.248 fitted (as with the true strengths), strengths
  within 0.0016 rms. Note if no ranges are excluded (saturated bands). Error 49.
- **Airmass in Tools → Barycentric correction**: altitude from ERFA (bias-precession-nutation,
  local Earth rotation angle; within 0.005° of astropy), airmass after Pickering (2002), shown
  in the table and written with the BC file (`<prefix>airmass.txt`), set as start strength of
  the static lines.
- **Zero point of the RVs** (found while testing the barycentric correction): γ and the mean
  RV of a component are not determined by disentangling, also not with static lines (a common
  shift is absorbed by the component spectrum). Levenberg-Marquardt in the RV mode now keeps
  the mean RVs also with static lines; the orbit fit presets "Own zero point for B" for every
  RV-mode result; γ is fixed by default, and a note appears if it is free (optimisation, error
  estimation).
- **Tools → Barycentric correction: "Write BC file"** (request of the user): writes only the
  table of the corrections (default `<prefix>barycorr.txt`) and sets it in the Data tab with the
  frame of the spectra ("Spectra already corrected": barycentric, else observatory frame); the
  spectra are not changed.
- **Barycentric correction in the model** (request of the user): Data tab "Barycentric
  correction" (file with BC per spectrum, the last number of each line, e.g. the
  `<prefix>barycorr.txt` of Tools → Barycentric correction) and the frame of the spectra.
  Observatory frame: in the orbit mode the RVs of the orbit are converted,
  (1 + v_obs/c) = (1 + v/c)/(1 + BC/c), the static lines stay at rest; in the RV mode the orbit
  fit gets barycentric RVs. Barycentrically corrected spectra (still possible): the static
  lines are shifted by BC per spectrum (`buildMatrix`: optional velocities of the static
  component, interpolated like A and B) and come out in the observatory frame. Without the
  file nothing changes; the orbit mode with static lines then gives a wrong orbit (synthetic
  test, BC ±28 km/s: K_A 29 instead of 40, K_B 74 instead of 45 km/s), which the program points
  out. "Use in main window" of the tool sets the file and the frame. Error 48.
- **File names per setup** (request of the user): the best state ("Best state", default
  `beststate.dat` → `beststate_rv.dat` / `beststate_orbit.dat`), the residual statistics
  (default `diffstatistic.dat`) and the convergence data (default `optdata.dat`) get a line
  in the Output tab, the orbital elements file (default `orbitelements.dat`) one in the Data
  tab; the Editor and the orbit fit use it. The best state is used only if it exists and
  belongs to the current mode (the kind in the first line is now checked: an RV file under the
  orbit name was read as elements before); otherwise it is created anew. Simplex backups are
  named after the current simplex (`<name>_backup.<ext>`, before fixed `optval_vel_backup.dat`
  for both modes), the RVs at a restart go to `<best state>_rv_restart.<ext>` (before
  `rein_vel.dat`). A configuration without these settings gets the defaults (not the names of
  the configuration loaded before).
- **Orbit mode, Nelder-Mead**: the best elements are written to the orbital elements file at
  the end of the optimisation, as the manual said and as Levenberg-Marquardt does (before only
  at a restart).
- **Continuum-normalised components keep the fit** (suggestion of the user): A' = A − e,
  B' = B + r e (r = w_A/w_B from the offset null direction), e from the continua of A and B
  weighted by the scatter of their continuum points; the composite is unchanged at every
  wavelength. The residuum with A', B' is computed and reported (ι Tri A, 47 spectra, A' and B'
  inserted into the model with the fitted RVs: residual rms × 1.0006; independent division by
  the continua: × 2.36). Without an offset null direction (light ratio varying) the
  components are divided by their continua as before.
- **Continuum-normalised components** (Output tab option): writes `<name>_norm.<ext>` beside
  the components as from the disentangling and plots them. Continuum per piece between
  excluded ranges: cubic B-spline (knots every 80 Å, adjustable) through the upper points
  (reject > 1.5 σ below, adjustable, and > 3 σ above; σ from the points above the fit;
  iterated); module `continuum`. Tested on ι Tri A (STELLA, 4500–7500 Å): steps and broad
  wiggles removed, B at 1.00 ± 0.01. Tests: unit tests (line-blanketed spectrum with a step
  1.4 → 0.8 at a gap and a wiggle: 95 % of the points within 1 %), `continuum_norm`.
- **Excluded wavelength ranges** (Model tab, "Exclude ranges from"; e.g. telluric bands): the
  pixels in the ranges are removed from the system (rows without entries) and from the
  residuum, the S/N estimate and the residual statistics. Default file `telluric_mask.dat`,
  created in the work folder if missing, with the strongest telluric bands and the strong
  chromospheric lines (Ca II H&K, Hε, Hδ, Hγ, Hβ, Hα, Mg I b, Na I D, Ca II IR triplet: core
  half-width 1-3 Å plus the shift for stellar RVs up to ±100 km/s). Component pixels
  that lose more than half of their data are not written; with "Exclude edges" the pixels
  within the edge margin of a range do not count in the residuum. Reason: on ι Tri A the
  components showed steps at the H2O bands at 7200 and 8300 Å. Test `mask_tellurics`
  (synthetic binary with a telluric band of varying depth: step of A 1.10 and B 1.61 across the
  band without, 0.04 and 0.01 with the range excluded); unit tests (file, marking, removing
  rows = solving without them).
- **Converter for three-column tables** (`tools/tab2spectangular.py`): wavelength [Å],
  flux, error (e.g. STELLA/SES `...botzfxsEcdtab.txt`) → `<prefix><i>.txt` with
  log10(λ) and flux on one common equidistant grid (range `--wmin/--wmax`, step
  `--step-kms`, default the finest original pixel; cubic spline, flux-conserving averaging
  for coarser steps), S/N file from the error column, list index → source file. Checked
  on a STELLA spectrum: own spline = scipy to 4·10⁻¹⁶, a spectrum shifted by 30 km/s is
  measured at 30.03 km/s, mean flux conserved to 10⁻⁵, read by Spectangular without
  resampling.
- **Barycentric correction** (Tools → Barycentric correction): BC = v_obs·n per spectrum from
  the star coordinates (ICRS), the observatory (list incl. STELLA/Teide, or coordinates) and
  the times (JD/MJD UTC, HJD UTC, BJD TDB); writes the spectra with λ(1 + BC/c) as
  `<prefix><index>.txt`, the table `<prefix>barycorr.txt` and optionally the start RVs + BC;
  "Use in main window" switches the Data tab to the corrected files. ERFA 2.0.1 subset
  (61 routines, BSD licence) in `spectangular/erfa`. Agrees with astropy within 5 m/s (astropy
  adds relativistic/gravitational terms of ~4.8 m/s). Reason: spectra of ι Tri A from STELLA
  were not corrected; the RV mode absorbed the correction per spectrum, the orbit mode could
  not (residuals per spectrum 0.0157 instead of 0.0111). Tests: unit tests (astropy reference
  values for three sites and JD/HJD/BJD, coordinate parsing, MJD), `barycorr`.
- **Gain per iteration in the convergence plot.** In addition to Δχ² to the current best
  (which moves up for all earlier points whenever a better solution is found, and when the
  estimated σ² gets smaller), the plot shows the decrease of the best χ² in each iteration
  with the σ² of the moment it was found; these values stay fixed. `optdata.dat` has σ² as a
  4th column. The Auto Stop threshold is drawn as a dashed line. Checked in the `autostop*`
  cases.
- **Flux ratio from synthetic spectra** (Tools → Flux ratio from synthetic spectra): a
  synthetic spectrum per component gives the flux ratio F_A/F_B, and the disentangled
  spectra are renormalised to their own continua. Model: x = c − (ℓ/w)(1 − T) per component,
  with the light fractions w of the disentangling and ℓ of the true ratio k s(λ) (same shape),
  continuum c_A(λ) a Legendre polynomial (c_B from w_A c_A + w_B c_B = 1), k by a
  one-dimensional minimisation. Synthetic spectra broadened (v sin i, resolving power) and
  shifted (fitted RV shift). Error of k from a block jackknife over the wavelength (the formal
  error is too small for correlated residuals); check: separate fits of the light fractions
  must add up to one. Writes `<component>_renorm.<ext>` and `fluxratio_templates.txt`; "Use
  ratio in main window" sets the field. Module `templatefit` without GUI. Tests: unit tests
  (ratio, shifts and continuum recovered with constant and Planck shape, wavy continua,
  broadening keeps the equivalent width, wrong line strengths detected), `flux_templates`
  (synthetic binary disentangled with 1.0 instead of 1.5: ratio within 0.5 %, renormalised
  components match the truth).
- **Fixed RVs** in the optimisation of individual RVs (Optimisation tab, "Fixed RVs
  (spectrum numbers)" A and B, e.g. `0, 3, 7-9`): these RVs stay at their start values,
  analogous to the fixed orbital elements. Nelder-Mead works on the free RVs only;
  Levenberg-Marquardt keeps them by constraints, and the mean RV is kept at its start value
  only for a component without a fixed RV (a fixed RV sets the zero point). Invalid lists or
  all RVs fixed: Error 45. Saved in the configuration. Tests: `fixed_rvs`, `fixed_rvs_lm`,
  `fixed_rvs_all`, `fixed_rvs_invalid`.
- **Orbit fit to RVs** (Tools → Orbit fit to RVs; the old, unreachable OrbitFit dialog
  rewritten): Keplerian orbit (SB1/SB2) fitted with Levenberg-Marquardt to the times and
  current RVs of the main window or to a table (t, RV_A [, RV_B [, σ_A, σ_B]]). K ≥ 0,
  eight start phases of T_peri, T_peri moved to the passage nearest to the data, two-pass
  weighting with the RMS of each component, errors from JᵀJ scaled by χ²/dof. Optional own
  zero point of B (preset without static lines, where the zero points of the components
  are arbitrary). RV curve and O−C plots; result with covariance and residuals in
  `orbitfit_result.txt`; "Write orbitelements.dat" gives the start for the orbit mode.
  Module `rvfit` without GUI. Tests: unit tests (orbit recovered, offset of B, SB1, table),
  `orbit_fit_rvs`.
- **Monte Carlo error estimation** (Optimisation tab, "Error estimation": Curvature /
  Monte Carlo, number of realisations, default 30). Parametric bootstrap: Gaussian noise
  with the scatter of the residuals of each spectrum (dof-corrected) or with the given S/N
  is added to the best model, the free elements are refitted with Levenberg-Marquardt from
  the best point; the errors are the scatter of the fits. Same output as the curvature
  method (covariance, correlation) plus the realisations. Reproducible (fixed seed).
  Test: `error_mc`.
- **Version 2.0**; window title "Spectangular - Spectral Disentangling".

- **Residuals per spectrum** (tab "Convergence & residuals", formerly "Convergence"): RMS
  and mean of observation − model for each spectrum, plotted against the orbital phase
  (orbit mode), the time of observation (times file present) or the spectrum number, with the
  RMS of all spectra and the noise level 1/(S/N). Optional wavelength window (e.g. one line);
  edge pixels left out. Written to `diffstatistic.dat` (now with a header and the columns
  spectrum, phase/time/number, pixels, mean, RMS, standard deviation), also in batch mode.
  Computed from the solution in memory; the old two-pass computation from the differences
  file (whose second pass mixed means of different quantities) is removed. Tests:
  `residual_stats` (orbit phase; a spectrum with an extra emission stands out in its window),
  `residual_stats_rv` (times).

- **Wavelength-dependent flux ratio** (Data tab: "λ dependence", "at λ_ref", T_A, T_B,
  table). F_A/F_B of spectrum j = k_j s(λ), s(λ_ref) = 1; k_j is the ratio at λ_ref (field
  "Flux ratio", Editor / `edit.dat`, or the flux-ratio optimisation); s is constant (as
  before), the ratio of two Planck functions, or a table (λ [Å], ratio) interpolated
  linearly. Covers eclipses (only k_j changes, without limb darkening).
  - The unknowns are the diluted spectra y = ρ x (light fractions ρ of a reference ratio
    k_ref s(λ), k_ref = median k_j); the rows carry f/ρ, which keeps the exact continuum
    null space of the constant case. With the ratio directly in the rows that null space
    became nearly null and LSQR filled it with amplified noise (continua off by ~10^3 on
    eclipse-free test data). Output x = y/ρ, offsets removed by minimum norm in x.
  - A constant shape (T_A = T_B, constant table) gives the same output as the constant ratio
    (tests with SB2, static lines and SB1; the constant mode itself is unchanged).
  - Synthetic eclipsing SB2 over 4500-6500 Å (Planck ratio 15000/6000 K): components within
    0.009/0.023 % of the truth (constant ratio: 0.57/1.42 %); flux-ratio optimisation
    recovers the eclipse pattern of k_j to 0.4 % (constant: 1.0 %, deepest eclipse +6 %).
  - Error 44: table missing or invalid, temperature ≤ 0.
- **Spectrum Plotter: "Flux-ratio model (A = File 1, B = File 2)"**: continuum correction
  A + (1 - s), B - r(λ)(1 - s) with the spline s through continuum points of A and the flux
  ratio of the main window; leaves the model of the observations unchanged and puts both
  components on the continuum 1 (test: RMS 20/27 % -> 0.43/0.61 % at S/N 200).
  It replaces "Correct to Ratio", which shifted both files by opposite amounts (correct only
  for equal weights) and is removed together with the unused field "To Continuum".
- `tests/tools/make_eclipse.py` (synthetic eclipsing binary with a Planck flux ratio,
  analytic line profiles) and `tests/tools/eval_eclipse.py`; manual section 2.2, example 9.3
  and appendix A.

- **User manual, second edition** (`Spectangular_Manual.pdf`, source in `doc/manual/`,
  rebuild with `make manual`). Rewritten for the revised program: method (model,
  interpolation, solvers, residuum and weighting, Nelder-Mead and Levenberg-Marquardt,
  Auto Stop, error estimation, observation planning), installation with `make`, data
  preparation and automatic resampling, every tab of the main window with screenshots,
  workflows (RVs, orbit, flux ratios, sequence, batch mode), the auxiliary windows, all
  input and output files, worked examples (test data, orbit fit with errors, Mizar),
  common problems, the list of error messages and notes for users of 1.11. Replaces
  `Spectangular_Manual_2020.pdf`; `make install` installs it as before.
- Test driver: the `screenshot` case also saves the Tools windows and the Subtract panel;
  with `SPECTEST_SHOT_RUN=1` it optimises and estimates the errors first (images for the
  manual).

- **Main window rebuilt.**
  - Top row: the workflow buttons "1 Read data › 2 Solve › 3 Optimise ›
    4 Error estimation", Stop, and the work folder.
  - Left: settings in the tabs Data, Model, Optimisation and Output, grouped by topic.
    Before, input, run parameters, file names and status shared the same rows; the index
    range of the spectra sat in the cross-correlation row.
  - Right: Spectra, Convergence, Messages (with the error estimation) and RVs.
  - Bottom: a status row with iteration, evaluations, best residuum and time, plus a
    summary of solver, interpolation, method and cores.
  - "Subtract" moved to Tools → Subtract component… (a panel).
  - The orbital elements are ticked when *free* (before: "lock"). Configuration files
    still store "locked", so old and new files remain interchangeable.
  - Logos, the Close button, the second toolbar and the fixed fonts and colours were
    removed. All widgets keep their names, so configurations and tests are unaffected.
- **Plot range without edge outliers.** The y range of the component, static-line and
  difference plots ignores the outermost pixels of each spectrum: at least 3 pixels or 3 %,
  and at least the "Exclude edges" margin. These columns are determined by single spectra
  and can be far off; on the Mizar data one edge pixel of component A at 57.7 squeezed the
  spectrum into a flat line. The data are still plotted, only the range ignores them.

- **Sparse matrix and LSQR.**
  - The transformation matrix is built as a sparse matrix (`arma::sp_mat`, 64-bit indices).
    Each row has only 2–5 entries.
  - The default solver is LSQR (Paige & Saunders 1982). Like the truncated SVD it yields
    the minimum-norm least-squares solution and needs only the products M·x and Mᵀ·y.
  - Alternatively "SVD (DaC)" can be selected: truncated SVD with divide and conquer on
    a dense copy of the matrix.
  - Test data at full resolution (20 × 1417 pixels): one evaluation takes 0.6 s instead of
    147 s, memory use drops from 2.2 GB to 54 MB. Spectra and residuum agree with the
    SVD to 10⁻¹¹, and the course of the optimisation is identical.
  - Memory and time per iteration grow linearly with the spectrum length, and so does,
    roughly, the number of iterations. Measured with 20 spectra:
    - 20,000 pixels: 5 s per evaluation,
    - 80,000 pixels: 2 min per evaluation, 100 MB.
  - With the dense SVD, 20,000 pixels would take 128 GB and several days.
- **Command-line mode.**
  - `Spectangular --batch <configuration>` loads `<configuration>.sps` from the current
    folder and optimises without a window. Without a name, the configuration is asked
    for on the console.
  - `--overwrite` overwrites existing start files without asking.
  - Exit code 1 if an error occurs or the optimisation does not end regularly.
  - (f105378, 456bcf8)
- **Error estimation of the orbital elements** after Nelder & Mead (1965, appendix).
  - The file (`lineEdit_24`) contains, for each free element, the best value, the
    standard error and the minimum of the quadratic model, plus covariance,
    correlations and the matrices Q and B.
  - σ² is estimated from the residuum: degrees of freedom = bins − unknowns of the
    spectra − free elements.
  - If the simplex is not at a minimum (B not positive definite), a message is shown
    instead of meaningless numbers.
  - (ca31693)
  - **New simplex around the best point.** After full convergence the final simplex has
    shrunk to a point (scatter of the residua ~1e-14); the estimation then aborted with
    "simplex is degenerate". Now, as suggested by Nelder & Mead, a new simplex
    x0 + h_i e_i is set up around the best point. The steps h_i are adjusted per element
    so that the squared residuum at the vertex rises by about σ² (tolerance factor 2,
    curvature symmetric from x0 ± h; one-sided at the boundary of the allowed range).
    The starting value is the extent of the final simplex. Cost for 4 free elements is
    about 30–50 evaluations (full test data: 3.4 s). The steps are written in the header
    line of the error file.
  - If a free element has no measurable influence on the residuum, the message
    (Error 38) names the element instead of reporting a singular matrix.
  - **Second pass along the principal axes.** The first (axis-parallel) simplex yields a
    quadratic model; along its eigendirections a second simplex is set up with steps of
    about one standard deviation. For strongly correlated elements (P–T, e–ω) the steps
    of the first pass along the valley were much shorter than the uncertainty; the
    marginal errors were underestimated (Monte Carlo example: P by 23 %, K_A by 14 %),
    and in ~10 % of the runs B was not positive definite because of small
    irregularities. If the second pass fails, the first one is used, with a notice.
    (7d42e02)
  - Degrees of freedom for σ²: bins − unknowns occurring in these bins − free
    elements. (0c3be01)
- **The number of cores ("Use") now takes effect.** Previously the value was not used.
  LSQR runs with its own OpenMP loops (matrix-vector products and vector operations
  fused) on the configured number of cores. The number is also passed at run time to
  OpenBLAS, BLIS or MKL if loaded (SVD (DaC)); without such a library the program
  prints a notice. Measurement (2 cores):
  20 × 1417 pixels LSQR 64 → 36 ms on one core (fused loops), with 2 cores 33 ms;
  20 × 20,000 pixels 3.0 → 1.8 s; SVD 6000 × 1500 3.4 → 2.3 s. For short spectra
  additional cores bring little, because one LSQR iteration takes only ~0.2 ms.
  Batch mode: `--threads N`. The "Use" field was disabled in the GUI and is now
  editable.
  - LSQR runs in parallel only from 100,000 rows (spectra × pixels) upwards; below that
    one core is faster. OpenMP threads wait passively (`OMP_WAIT_POLICY=PASSIVE`, unless
    set otherwise). Previously two simultaneous runs with 2 threads each on 2 cores
    became 90 times slower (293 s instead of 3.3 s), because actively waiting threads
    consumed the computing time of the others.
- **Auto Stop based on the progress in χ².** Previously Auto Stop waited for exactly equal
  simplex values in two consecutive iterations and only worked together with
  Reinitiate. With many parameters this never happened: the test data (40 RVs) ran all
  9999 iterations, although nothing measurable was gained after about 5000–7500.
  - New fields: "S/N" of the spectra (σ = 1/(S/N); "auto": σ² = residuum² /
    degrees of freedom), "Stop at Δχ² <" (default 0.1) and "over … it." (window; "auto":
    10 iterations per parameter, at least 200). Saved in the configuration.
  - A run has converged when χ² = residuum²/σ² has decreased by less than Δχ² over the
    window.
  - Auto Stop also works without Reinitiate. With Reinitiate, the optimisation restarts
    after convergence; Auto Stop ends it when a run has gained less than Δχ² over the
    previous one. In sequence mode the next sequence then follows.
  - Without Auto Stop the iteration limit applies across all restarts; Reinitiate
    without Auto Stop previously ran forever.
  - With Auto Stop the optimisation is finished regularly (best solution adopted and
    written) instead of being aborted in the middle of the loop.
  - Applies to the optimisation of the RVs, the orbital elements and the flux ratios
    ("Opt. Ratios", there only with the residuum criterion and without Reinitiate), in
    each case with residuum "Mean".
  - "Opt. Ratios": a common factor of all flux ratios can hardly be determined from the
    residuum (flat valley); what is determined is their variation from spectrum to
    spectrum. A long run moves the common factor into the noise (test: 0.94 instead of 1
    with an equally good variation); Auto Stop ends the optimisation earlier.
  - Do not choose too small a window: with 40 instead of 200 iterations a test stopped
    26 σ² before the optimum, with 200 after 0.1 σ².
- **Weighting by S/N per spectrum** ("Weight by S/N from", next to "Solve"). Weighted
  least squares: spectrum j enters with σ̄/σ_j, σ_j = 1/(S/N)_j, σ̄² = 1/mean(1/σ_j²).
  The residuum stays in flux units; χ² = residuum²/σ̄² is used for Auto Stop and
  error estimation (σ̄ instead of the "S/N" field). The outputs (components, differences)
  remain unweighted.
  - S/N from a file (one value per spectrum, in the same order as the spectra) or
    "auto": at the first solve after reading the data, the program estimates the S/N of
    each spectrum from the residua (3 iterations of solve – estimate – weight) and keeps
    the weights fixed for the whole optimisation, including restarts and error estimation.
  - "Solve" always writes an estimate per spectrum to `snr_estimated.dat`. The
    estimate takes the leverage of each spectrum into account (highly weighted spectra
    co-determine the components and have smaller residua).
  - Test with S/N between 20 and 300: component spectra 3.6 times more accurate than
    unweighted; "auto" as good as the true weights; estimated S/N within −8 … +18 %.
    An estimate from the unweighted solution, by contrast, is biased (down to −73 % for the
    best spectra), hence the iteration.
  - Error 40: S/N file missing, wrong number of values, or values ≤ 0.
- **Plot of residuum versus iteration:** Δχ² relative to the best value on a logarithmic axis
  (points: all evaluations; line: best solution so far). Previously the axis was
  dominated by the large initial values, and changes in the 4th digit were invisible.
- `optdata.dat`, the log and the console write the residuum with 15 digits (previously 6).
- **Installation with `make`** (without Qt Creator): `make`, `sudo make install`
  (default `/usr/local`, `PREFIX` and `DESTDIR` configurable), `make uninstall`,
  `make check`, `make deps`. Installed are the program, `BinaryData.dat`, the manual,
  a menu entry and an icon. The program now looks for `BinaryData.dat` and the manual in
  the current folder, in `~/.local/share/spectangular`, in the program folder and in the
  installation folder; previously only in the current folder, so that "Manual" and the
  orbit database of the editor were usually not found.
- **"File" menu:** Open Configuration… (Ctrl+O), Save Configuration (Ctrl+S, without
  asking, to the most recently opened/saved file), Save Configuration As…
  (Ctrl+Shift+S), Quit (Ctrl+Q); the window title shows the file. The row with the
  "Save"/"Load" buttons and the name fields has been dropped. On loading (menu and
  batch mode) the folder of the configuration file becomes the Work Path; the path
  stored in the file is no longer used. Configuration and data must therefore be in the
  same folder.
- **GUI:** stopping criterion (Auto Stop, S/N, Δχ², window) in a separate row
  below the optimisation; residuum range and "Exclude edges" in a separate row
  below the plot. The window fits again without a horizontal scroll bar.
- **FITS without extension names.** The given extension is searched for case-insensitively;
  if the field is empty or the name unknown, the first binary table with the two columns
  is used. Images and other tables are skipped.
  (cfdc45d)
- **Configuration (.sps).** The previously missing settings are now saved as well,
  among others:
  - fit checkboxes for K_A, K_B, γ and ω,
  - Auto Stop, sequence mode, Trunc. SVD,
  - plot options and file name templates.

  They are stored as `name=value` after the 58 existing lines. Old files remain
  readable, and version 1.11 still reads new files (first 58 lines). (a0acbfb)
- **Checks when reading data.**
  - All spectra must have the same length and the same equidistant log10(λ) grid.
  - Unreadable lines are reported with file and line number.
  - Blank lines and comment lines (`#`) are allowed.
  - (b03b391)
- **Test suite** (`tests/`).
  - Unit tests of the new modules.
  - A test driver that calls individual program steps.
  - Binned and synthetic data sets with known solution.
  - Reference outputs.
  - Run time about 2 minutes, 127 checks. Instructions: `tests/README.md`.
  - (01dbca6 and following)

- **LSQR warm start.** During an optimisation, LSQR starts from the solution of the
  previous evaluation (transferred to the new column layout by rest-frame pixel) and only
  solves for the correction. The parts of the start vector along null directions of the
  matrix are removed afterwards: constant offsets of the components and, with static
  lines, linear ramps in A, B and T (nearly null for a Keplerian orbit, because the shifts
  of B are a linear function of those of A). The result is the same minimum-norm solution
  as from a cold start: on the test data the optimisation takes the identical path over
  3000 iterations, component spectra agree to 1e-11. Saving: 6 % of the LSQR iterations
  at the start of a run, 11 % from about 2000 iterations on; run time −3 % (3000
  iterations on the test data: 257 s → 248 s). The gain is limited: consecutive
  Nelder–Mead evaluations differ by the simplex size, and even for an RV change of only
  1e-5 pixel a warm start saves just a quarter of the iterations at the required accuracy
  (synthetic test, 20 × 1417 pixels), because LSQR needs most of its iterations for the
  last digits. Cold start after "Read Data", with SVD and after a failed solve;
  environment variable `SPECTANGULAR_COLD_START=1` disables it.

- **Levenberg-Marquardt as an alternative to Nelder-Mead** ("Method" below the
  Optimization button; Nelder-Mead stays the default). It works on individual RVs and on
  orbital elements, uses variable projection, and is implemented in `levmar.h/.cpp`
  without GUI dependencies.
  - Every evaluation solves for the component spectra.
  - Central-difference Jacobian; every column is a warm-started LSQR solve. Forward
    differences biased the gradient by up to a factor of 10 near the minimum: there the
    residual is mostly noise, and r · ∂²r is large.
  - Secant correction of the Gauss-Newton curvature (Dennis-Gay-Welsch / NL2SOL), bounded
    to 0.1–1.5 × JᵀJ, plus a line search along the step. Reason: at S/N 65 the true
    curvature is 1.5–7 × smaller than JᵀJ because the component spectra absorb part of the
    noise, and pure Gauss-Newton converged only linearly (30–100 steps).
  - Stops when the model predicts less than 0.002 in χ² still to gain.
  - In the RV mode without static lines the mean RV of each component stays fixed. A
    common shift only moves the reconstructed spectrum, and the optimisation would
    otherwise drift slowly along this direction. Nelder-Mead drifts there as well.
  - Afterwards a simplex around the result is written, so that Error Estimation and a
    Nelder-Mead "Continue" can follow. Standard errors are not taken from JᵀJ: they
    would be 2–3 × too small.

  Results:
  - Test data, 40 RVs, linear interpolation: 10 steps, 44 s, lower residuum than
    Nelder-Mead after 3000 iterations (248 s).
  - Orbit fit, cubic interpolation, 16 Monte Carlo data sets: 55 s instead of 146 s
    (including error estimation 69 s instead of 163 s); χ² 0.002–0.014 above the
    Nelder-Mead minimum. Elements, scatter and error estimates agree within 0.02 σ.
  - Orbit fit with linear interpolation (one data set): 0.03 in χ² above the Nelder-Mead
    minimum, and the same errors. The kinks at whole-pixel shifts do not get in the way
    with central differences; forward differences stalled 7 χ² above the minimum.
- **Cubic interpolation** ("Interpolation": Linear / Cubic). The components are shifted
  with the 4-pixel Catmull-Rom (Keys) kernel. It reproduces quadratics exactly, and the
  model is continuously differentiable in the RVs, without kinks at whole-pixel shifts.
  Results:
  - On the coarse synthetic test grid (13.3 km/s per pixel) the bias of the orbit fit
    drops about 7×, e.g. K_A 38.89 → 39.85 (true 40).
  - On the test data the residuum drops by about 64 in χ².
  - Each solve takes 3–4 × as long: twice the nonzeros, and LSQR needs about twice the
    iterations.
  - Exclude edges uses one pixel more.
  - Configuration keys `comboBox_interp` and `comboBox_method`.

- **Resampling to a common log grid when reading** ("to log grid, step" next to the
  file format). All spectra are put onto one equidistant log10(λ) grid with a natural cubic
  spline, for spectra on a linear wavelength scale (Å) or on individual log grids. A column
  is taken as Å if all its values exceed 100. The common grid covers the range shared by
  all spectra. The step is given in km/s, or "auto": the finest median sampling among the
  spectra. Without the tick this happens automatically (step "auto") whenever the spectra
  are not on one common equidistant log grid; the message names the reason.
  - Real data (Mizar, 22 spectra from three set-ups, linear Å grids rounded to 0.01 Å):
    resampled to 416 pixels of 5.66 km/s; RV optimisation with Levenberg-Marquardt
    (cubic) in 49 s, residuum 0.956 → 0.560.
  - Tests: spline against SciPy (3·10⁻¹²) and a straight line (exact); resampling of
    three Gaussian-line spectra with different starts, steps and lengths (deviation
    1.4·10⁻⁴); error messages; `linear_grid_resample`: real spectra converted to linear,
    individually cropped Å grids and read back (common equidistant grid, flux RMS 0.003,
    below the noise); `linear_grid_auto`: the same without the tick, automatically.
  - Note: interpolation correlates the noise of neighbouring pixels. Spectra of lower
    resolution are not degraded or sharpened: a mix of resolutions remains in the data.

### Fixed

- **"Apply" of the Output tab** now also sets the names of the error estimation, the residual
  statistics, the best state, the convergence data and the files of the analysis
  (`<name>_errors`, `_diffstat`, `_beststate`, `_optdata`, `_rv_oc`, `_linestat`).
- **Strength of the static lines stuck at 0.** The strength per spectrum is updated
  multiplicatively; a spectrum whose static lines did not match (negative regression, set to
  0) lost its static columns and could never recover (ι Tri A, spectrum 2). Now the strength has
  a lower bound of 1e-3, and spectra below 0.1 are named in a warning (check the barycentric
  correction, time and wavelength scale). Test: `bc_tell_misaligned`.
- **Layout of the main window and manual figures.** At a screen width of 1440 px the settings
  panel needed horizontal scrolling (the new rows of the Data tab and the long label of the
  continuum alignment); now the left panel fits: the second widget of the rows Barycentric
  correction, Static line strength and Static lines only in spans two columns, "Align continua
  to the mean, smoothing", and below the spectra "Offset" and "best at eval." (with tooltips).
  "max |RV|" after reading shows the absolute value (before: the value with sign). The Editor
  shows the elements of the orbital elements file of the Data tab when it is opened (before:
  always the first system of BinaryData.dat, so "Apply Orbit" overwrote the file with it).
  Manual: all screenshots new (settings tabs as whole tabs without scrolling), "Show static
  lines", "Offset", "best at eval." and the group "Optimisation state" described.

- **Software test campaign (user errors, combinations, fuzzing, sanitizers).** New tool
  `tests/tools/campaign.py`: 146 user-error scenarios (wrong or missing files, invalid values,
  settings changed between steps, damaged configurations), 87 pairwise combinations of 15
  options, input fuzzing of all windows, 14 metamorphic relations, and the whole regression
  suite under AddressSanitizer/UBSan. No crash or hang; the following gaps were closed:
  - Invalid orbital elements (P ≤ 0, e < 0 or e ≥ 1, NaN) are rejected with Error 6 (file)
    or Error 12 (window) instead of being used; the orbit fit to RVs checks its start values.
  - Flux ratio ≤ 0 (field "Flux Ratio A/B" and "Opt. Ratios") → new Error 51.
  - Negative or zero weights in `edit.dat` → Error 18.
  - Nelder–Mead orbit fit with all elements locked → Error 37 (ran silently before).
  - "Static lines only in": a missing file with another name than `telluric_static.dat` is an
    error (typo), and ranges without overlap with the data → Error 50.
  - A duplicated or shifted wavelength inside a spectrum (grid not common) is detected
    (the grid check looked only at the ends).
  - Output files that cannot be written (missing folder, name is a folder) → new Error 52
    (before only on the console).
  - Out-of-bounds read when writing the result file of the orbit fit with a free RV offset of
    B (found by AddressSanitizer).
  - Unknown keys in a configuration are reported as "Unknown settings (ignored)", no longer as
    invalid values.
  Test: group `user_errors` of the suite (all 146 scenarios).

- **Stop did not work during Solve.** During "Solve" the window was kept updated, but user
  input (the Stop button) was only processed during an optimisation or error estimation, so
  one had to wait for the solve. Stop now ends a running Solve as well ("Solve stopped.", no
  error box). Test: `abort_long_spectra_solve` (Stop after 0.8 s ends the solve at once).
- **Steps at excluded ranges during an optimisation.** Removing the rows of a range cuts the
  components into pieces, each with its own free offset of A against B (a null direction).
  Only the global offset was removed after warm-started solves, so the pieces drifted apart
  over an optimisation (synthetic: A 2.3, B 3.4 across a telluric band after 12 iterations).
  The offset null space now has one candidate per piece (sparse arithmetic, also less memory
  for long spectra); step after the same optimisation 0.03 / 0.01. Replacing the excluded data
  by artificial ones (bridge) was tried: steps of 0.3–0.6 remained (weak coupling across
  wide ranges) and LSQR needed 2–6 times more iterations. Tests: unit test (warm start with
  different offsets on both sides returns to the cold-start solution), `mask_optimisation`.
- **Window not responding during long solves; only one core.** On long spectra a single LSQR
  solve takes seconds, and the window only updated between two evaluations: the system
  reported "Spectangular is not responding" and "Stop" had to wait. LSQR now calls a progress
  hook about every 0.1 s (clock read every 8th iteration; no measurable cost): the window
  repaints, and during an optimisation or error estimation "Stop" ends the running solve
  (the evaluation counts as invalid, no false best value). Reading the spectra, building the
  matrix (per spectrum), writing the solution and the plots after an improvement also let the
  window update every 0.1 s (on 10 × 37 000 pixels the window was blocked for up to 1.5 s,
  now at most 0.26 s; writing + plotting take 1.3 s per improvement there, "Live update" off
  saves the plotting part). LSQR ran in parallel only from 100 000 rows on, so 45 spectra × 2148
  pixels (96 660 rows) used one core whatever was chosen; it now uses one thread per
  25 000 rows (here 3), up to the chosen number. The console reports the threads LSQR
  actually got after every solve, the status line shows them, and a build without OpenMP
  says so at start (before, the core setting then had no effect without any note). Tests: unit tests (threads by size, hook
  without effect on the solution, abort), `abort_long_spectra` (10 × 37 000 pixels: largest
  gap between GUI events, Stop during a solve).
- **Reinitiate with Auto Stop ended too early; Levenberg-Marquardt did not reinitiate.**
  With Nelder-Mead the restarts ended when a run had gained less than Δχ² against the
  previous one; as in version 1.11 they now go on as long as a run improves on its start
  point at all, and end with the first run that converges without improvement.
  Levenberg-Marquardt ignored Reinitiate and stopped after one run; it now restarts from its
  best point (new damping, new Jacobian) until a run gains less than its convergence
  tolerance (0.002 in χ²), within the iteration limit over all runs. A reinitiation no longer
  changes the settings (RV source, rein_vel.dat, start option). Tests: `autostop_reinit`,
  `lm_reinit`.
- **Continue discarded the best solution after a change of method.** Levenberg-Marquardt
  ignored "Continue" and started from the start values of the inputs; Nelder-Mead refused a
  simplex that did not match (Error 24) or used a simplex older than the best values. Every
  optimisation now writes the best values to `beststate_rv.dat` / `beststate_orbit.dat` on
  each improvement; Continue and restarts start from them regardless of the inputs and of the
  method. Nelder-Mead keeps the stored simplex only if it matches the free parameters and the
  best state, otherwise it builds a new simplex around the best values. The edge margin of
  "Exclude edges" is taken from the best values, not from the start values of the inputs
  (otherwise the residuum of the same point differed, 3·10⁻⁵ relative in the orbit test).
  Tests: `continue_method_switch`, `continue_method_switch_orbit`.

- **Window of the residual statistics.** With "auto" the statistics now use the residuum
  range of the Model tab ("Only from … to"), if it is switched on, instead of the whole
  spectrum; values given in the Results tab still take precedence. Test:
  `residual_stats_model`.

- **Empty output file name.** With no name for component A or B in the Output tab, Solve
  wrote nothing and the plot then reported the work folder as an empty file ("File
  …/6TriA_6400_6500/ is empty."). Now Error 46 names the missing field before reading the
  data. Test: `output_name_missing`.

- **Continuum levels of the components.** The components were written as the minimum-norm
  solution. Along the exact null direction (A + a, B + b with w_A a + w_B b = 0) this puts the
  continua at different levels when the flux ratio is not 1, and the poorly determined edge
  pixels dominate the choice: in the RV optimisation of the Mizar spectra A ran to 0.8 and B to
  1.2, in longer runs to 2.6 and −0.6. Before writing and plotting, the components are now
  moved along the null directions so that the continuum levels of A and B are equal (about 1);
  with static lines the static spectrum gets a flat continuum 0. The level is the mean of the
  upper envelope (iterative rejection of lines below m − 1.5 s, s from the pixels above m) over
  the well-covered pixels. Model, residuum and differences are unchanged (Mizar: residuum
  identical to the last digit). Unit tests: levels at 1 with and without static lines, no
  change without null direction, level right within the noise with up to 93 % of the pixels
  in lines. Reference outputs of the components re-recorded (constant offsets only).

- **Build with Armadillo 15**, which needs at least C++14 ("C++14 compiler required"): the
  project is compiled as C++17 instead of C++11 (`CONFIG += c++17`). The global Nelder-Mead
  coefficient `beta` was renamed to `nmBeta`, because it clashed with `std::beta` of C++17.
- **Build with Clang on the g++ mkspec** (e.g. a Qt Creator kit with Clang): qmake kept g++
  as linker, which links GCC's libgomp, while Clang's OpenMP code needs LLVM's libomp —
  "undefined reference to `__kmpc_global_thread_num`" and similar. `openmp.pri` now links
  with the compiler (QMAKE_LINK = QMAKE_CXX).

- **Plot of the differences with long spectra.** All spectra were drawn as one graph; the
  spectra share the wavelengths, and the adaptive sampling of QCustomPlot then dropped the
  points of the middle spectra (gaps of hundreds of Å at 86 000 pixels). Now one graph per
  spectrum; checked in `residual_stats`. Found in the stress test with long spectra
  (14 × 86 164 and 14 × 258 489 pixels: read and solve 14 s / 0.76 GB and 52 s / 2.2 GB,
  residuum at the noise expectation).

- Spectrum Plotter: "Divide" changes File 1 only (before, with "Both", File 2 was divided by
  2 - s, which assumes equal light of the components); "Both" is set by the mode.
- Version 2.0 in About; unused remains of the old difference statistics removed.

- **Reinitiate: restart far from the optimum.** The new simplex after convergence was built
  with the steps of the very first construction (default P/10 and T/10 for the orbit,
  6 pixels per RV), so its vertices lay up to ~10^6 in chi^2 above the best value and the
  restarted run spent its iterations shrinking back. The steps are now adjusted per
  parameter so that chi^2 rises by about 4 per vertex (`reinitSteps`, using the probe of the
  error estimation; ~2-4 evaluations per parameter). In addition the data were read again
  with the best RVs and the edge margin of the residuum was recomputed from their smaller
  range: the same point had a different residuum after the restart (Monte Carlo orbit:
  2.38006 -> 2.38397, i.e. +78 in chi^2) and the "gain against the previous run" of Auto
  Stop was wrong. The margin now stays fixed within one optimisation. Tests: new case
  `orbit_fit_reinit`; `autostop_reinit` and `reinit_no_autostop` check that every restart
  starts at the best point (relative 1e-8) and that no vertex is more than 20 in chi^2
  above it (before: up to 2e6 for the orbit, 82 for RVs).

- **Spectra on a common linear Angstrom grid were read as log10(lambda).** If all spectra
  had the same equidistant wavelength grid in Angstrom (e.g. `TestData/spectra_*.txt`),
  the grid check passed and the Angstrom values were used as log10(lambda): the RVs
  shifted by a negligible fraction of a pixel. Wavelengths above 100 are now always
  recognised as Angstrom and resampled. Unit test added.
- **Levenberg-Marquardt in the orbit mode** wrote its result only to the log and the
  simplex files; "Read data" and "Solve" then continued from the old elements. The best
  elements are now written to `orbitelements.dat`, as with Nelder-Mead (checked in
  `orbit_fit_lm*`).
- **Convergence plot of Levenberg-Marquardt** showed every trial evaluation of the line
  search and the rejected steps as spikes. `optdata.dat` now holds the start value and one
  line per accepted step (checked: not increasing).
- **Spectrum Plotter, "find values":** the y range ignores the outermost pixels (at least
  3 or 3 %), like the plots in the main window; single edge pixels of a component (up to
  several hundred with cubic interpolation) made the plot unreadable.
- The "Bidiagonal Matrix" box (Model tab) had no effect and is hidden; the setting stays in
  the configuration files.
- The method list was too narrow for "Levenberg-Marquardt".

- Qt warning "connectSlotsByName ... on_spinBox_8_valueChanged()" at start-up: the slot now
  takes the `int` argument.
#### Disentangling and linear system
- SB1: read outside the vector `RV3` (K6); `bso2` is set to 0 (H5). (ae1ef2a)
- No truncation of `Mm` when there are more unknowns than measurements (M1). (ae1ef2a)
- ECON button: the method selection was inverted; crash when no method was selected. (ae1ef2a;
  the button has since been removed)
- SB3 reports an error instead of computing with an outdated matrix. (ae1ef2a)
- The error file contains all pixels, including the last one (N9). (ae1ef2a)

#### Optimisation (Nelder-Mead)
All three branches (RVs, orbital elements, flux ratios) now use the same tested
implementation (`neldermead.cpp`). (f54bffe)

- Orbit branch: the second-worst point was determined incorrectly (practically always the
  last point). Acceptance or contraction was therefore largely random (K4).
- RV and flux branches: after an accepted reflection, the contraction was done against the
  old worst value. The simplex could get worse as a result (H2).
- Expansion uniformly Z + γ(Co − Z); previously formulated differently in two branches (M5).
- When shrinking, the best point is not evaluated again (N1).
- Robust selection of points with equal function values (N3).
- Auto Stop with sequence restarted the optimisation recursively from within the running
  loop. The outer loop then continued with an outdated simplex for up to 9999
  iterations (K5). Restarts now run through a loop.
- The next sequence starts with the best RVs or orbital elements of the previous one (H5).
- Sequence mode ends regularly when the next sequence is missing. (456bcf8)
- `optval`/`optmat` were cleared before the loop and not written on stagnation, Auto Stop,
  reinitialisation or abort. "Continue" then failed (H12).
- At the end, the RVs, orbital elements or flux ratios are set to the best point, not
  to the last evaluated one (M6).
- The initial step for e is no longer 0 when e = 0; otherwise the initial simplex was
  degenerate (M1).
- Invalid orbital elements (P ≤ 0, e ∉ [0, 1)) and flux ratios ≤ 0 get a
  penalty residuum instead of NaN. Previously this could freeze the simplex (H10, M3).
- Orbit branch: with "New" set, old data were additionally loaded after the initial
  simplex, or the start data were overwritten. (f54bffe, 456bcf8)
- An outdated error status made the next optimisation abort silently (M3). (f105378)
- Clicking "Stop" while nothing was running aborted the next run after the first
  evaluation. (456bcf8)
- Output of all seven orbital elements at the end (N5).
- Variable-length arrays replaced by `std::vector` or Armadillo matrices.

#### Error estimation (ca31693)
- The residua were read from a file that had already been closed (K2).
- Writes beyond array bounds: stack and heap corruption, reproduced in tests
  as a crash (K3).
- The assignment of the free elements was wrong: lock flags were never set (H6).
- The chi matrix was indexed with an offset, the formulas for B and the covariance were
  wrong, and there were no standard errors (H7, M4).
- `orbitelements.dat` was read from the working directory instead of the data folder
  (M7).
- The error estimation locks the GUI while computing. (456bcf8)

#### Orbit model (7c68b50)
- Kepler's equation is solved with Newton's method. Previously a damped fixed-point iteration
  with errors up to 2.8·10⁻⁴ km/s at e = 0.95; now < 3·10⁻¹³ km/s.

#### Reading data (b03b391)
- Spectra of different length were stored misaligned in 1.11: the buffer was resized for every
  file with that file's length, so spectrum g started at g·n_g instead of g·n (e.g. 45 pixels
  off for spectrum 15 of the Mizar data). Spectra on individually started log grids (offsets
  up to 1.6 pixels = 7 km/s) were silently treated as one grid. Both are now resampled
  onto a common grid.
- Blank lines at the end of a file caused the last value to be read twice.
- SB1 with RV file: wrong index and line offset when the first spectrum was > 0.
- FITS read errors are reported; previously computation continued with empty or old
  data (H9).
- Wavelength range for the residuum: previously Å were compared with log10(Å).
- Check that the first spectrum does not come after the last one.
- **Grid check by pixel position** (532b2b6): every pixel must lie within 0.1 pixel of
  w₀ + i·step. Previously each individual step was checked to 1 %. That rejected
  wavelengths stored as float32 (FITS), although their rounding does not distort the
  positions, and let a linear λ grid of 1417 pixels pass.
  Note: in 1.11 the grid step was determined from a single pair of pixels
  (`W(5)-W(4)`). With float32 wavelengths this would have distorted the RV scale by up
  to 9 %; the step is now computed from the end points.

#### Configuration (a0acbfb)
- Paths with spaces and empty fields shifted all following entries on loading
  (H5).
- Numbers were saved with only 6 significant digits.
- "Bidiagonal Matrix", the working path and dependent switches were only displayed on
  loading, but did not take effect.

#### Batch mode (f105378, 456bcf8)
- Every error restarted loading and optimisation recursively; with missing input this
  went on endlessly until a crash (H4).
- Exit code 0 despite errors.
- The prompt for existing files blocked silently without input.

#### Subtract (f105378, 456bcf8)
- Crash with "Static Lines" active (H11).
- Errors when reading data were ignored.
- The solution of the optimisation was combined with a matrix built from different RVs.
  `compl.txt` now contains the RVs that belong to the solution.
- An invalid value for addition/multiplication was taken as 0.
- Rest frame exactly λ/(1 + v/c).

#### Auxiliary dialogs (13979fd, 456bcf8)
- **Editor:**
  - The initial step size for ω was stored in degrees and used as radians,
    i.e. about 57 times too large (H6). The label is now [deg].
- **Arithmetic:**
  - File B was not checked for existence; if B was shorter, it was read beyond its
    end (H7).
  - Equal length and equal grid are required.
  - When splitting, segment 0 was written with 6 digits and the last point was missing.
- **Spectrum plotter:**
  - "Add 1−f" used the straight line of spectrum 1 for spectrum 2 (H8).
  - S/N with uninitialised values.
  - "Write normalized data" read beyond the end of the second spectrum.
  - Spline points with equal x caused the program to abort; the error message was
    unreadable.
- **Correlation:**
  - After cropping, computation continued with old values.
  - With negative correlation, the shift from the previous run remained.
  - Normalisation per shift, symmetric shift range.
  - Fractional correction possible.
  - Sampling step uniformly from the whole spectrum.
  - Auxiliary files end up in the working folder.
- **Difference statistics** (`diffstatistic.dat`): per spectrum instead of per pixel; previously
  it read beyond the end of the array.
- **Difference statistics of the flux ratio optimisation** (`frdifferences.dat`): one entry
  too few, read beyond the end of the buffer. Found with the click test under
  AddressSanitizer. (2cbcf85)
- Message texts: "Run Solve" instead of "SVD, ECON", "Bug Report", file name in the plotter
  no longer duplicated. (2cbcf85)
- **`inclinations()`:** division by zero when no pixels were in the continuum window.
- **OrbitFit** (not reachable): missing `return` (undefined behaviour),
  array overflow.

#### Compiler warnings (Clang and GCC)
- Clang: the build aborted with "omp.h not found" when `libomp-dev` was missing. OpenMP is
  now checked during the qmake run (`openmp.pri`); without OpenMP the program builds
  single-threaded, with a notice.
- All warnings with `-Wall -Wextra` under GCC 13 and Clang 18 fixed (previously 84 under
  Clang): `&` instead of `&&` in conditions, unused variables, variable-length arrays in
  `orbitfit.cpp`, sign comparisons, self-assignments, possibly
  uninitialised values. In the third-party files `qcustomplot.cpp` (deprecated Qt functions)
  and `spline.h` the warnings concerned are selectively disabled.
- Editor, "Apply" of the light fractions: with an empty table cell the program crashed
  (null pointer); now a message with the row.

### Changed

- **Analysis of the solution merged with the period analysis.** The button "Analyse the current
  solution" moves from the Output tab to Tools → Period analysis (renamed from "Period analysis
  of the residuals"); the Output tab keeps what is written and the file names. The dialog gets
  a source: residual statistics (as before), fit per spectrum (`rv_oc.dat`: RV O−C and line
  strength of A and B, weighted with the errors) or statistics per line window (`linestat.dat`:
  mean, RMS, EW per line); after the analysis it selects the fit and lists its quantities, so
  that the periodogram of, e.g., the O−C of A or the EW of Hα follows at once. Outliers of the
  other sources are cut on both sides of the median. Tests: `period_fit`, `period_lines`,
  `period_fit_nofile`; `analysis*` run through the dialog.
- **Result tabs rearranged: two instead of four.** The plot of the differences per spectrum in
  the Spectra tab is removed (the differences file is still written); in its place the Spectra
  tab shows the residuals per spectrum (RMS and mean of O−M against phase/time and against
  the number, with window and Update). The second tab, now "Convergence & messages", holds the
  convergence plot and below it Messages and RVs side by side; the tabs Messages and RVs are
  gone. The field "Offset" (distance of the differences) is hidden and only kept for older
  configurations. "Save every improvement" saves `Residuals_<n>.png` instead of `Diff_<n>.png`.
  Test: `residual_stats` (layout of the tabs).
- **Default: 2 cores.** The number of cores ("Use", Model tab; batch mode without
  `--threads`) is 2 instead of all, if the machine has two. In a scaling test on long spectra
  (14-core machine) 2 cores were fastest, more and fewer took longer: only the
  matrix-vector products of LSQR run in parallel and are limited by the memory bandwidth.
  Choosing more than 2 cores shows a note (once per crossing of 2; not when the value is set
  by the program). `SPECTANGULAR_CORES` overrides the detected number of cores. Test:
  `cores_warning`.
- "Bidiagonal Matrix" is replaced by the interpolation and disabled. (ae1ef2a)
- **Solver selection in the GUI.** The row "ECON / Standard / DaC / Trunc. SVD /
  SVD / SpSolve" is replaced by "LSQR / SVD (DaC) / Solve". Exactly one option is
  active. "Solve" reads the data and solves once with the given RVs; the
  optimisation uses the same solver.
- **Configuration.**
  - The solver is saved as `solver=lsqr` or `solver=svd_dc`.
  - Files without this entry load with LSQR. The old checkboxes "Standard"/"DaC" only
    chose between two SVD variants.
  - "Trunc. SVD" is ignored on loading.
- The solution needs less memory: full test data 2.2 GB instead of 2.9 GB. (6b5b82b)
- Debug builds are also compiled with `-O2`. (01dbca6)
- `QT += multimedia` dropped (it was not used). (01dbca6)
- Error messages appear in the GUI as a dialog and in batch mode on stderr. New
  error numbers 25–39.

### Removed

- **Sequence mode** (several data sets, e.g. orders, processed one after the other). Long
  spectra can now be disentangled in one piece (stress test: 14 × 258 489 pixels). Its
  settings in older configurations are ignored (`config_old_sequence`); the case
  `sequence` is removed.
- The unfinished SimpleSubtraction dialog (never in the menu; subtraction: Tools → Subtract
  component).

- Buttons "ECON" and "SpSolve" (without function), option "Trunc. SVD" (ARPACK `svds`
  for all singular values, not sensible) and SVD variant "Standard".
- Output files `CompmA.dat`, `CompmB.dat`, `rcomp1.txt`, `rcomp2.txt`. They belonged
  to the whole-pixel and bidiagonal variants. (ae1ef2a)
- Unused or faulty code (ComputeDifferences, separation, Ofunction, Mt,
  loc/val). (ae1ef2a, 456bcf8)
- Unused headers `nr3.h`, `svd.h`, `redsvd.h` (Numerical Recipes and Eigen) and the
  Eigen licence files. (01dbca6)
- Checked-in build results: object files, `moc_*`, `ui_*`, `qrc_*`, Makefiles,
  binaries, `.pro.user`, two build folders. (01dbca6)

### Internal

- New modules without GUI and without global variables:
  - `orbit` – Kepler solution, RV
  - `dataio` – reading data, grid check
  - `disentangle` – sparse matrix, LSQR, SVD (DaC)
  - `neldermead` – simplex step
  - `errorestimate` – error estimation
- `mainwindow.cpp` reduced from 10,096 to 5,400 lines.
- Source list in `spectangular.pri`, shared by program and tests.
- Further tests (532b2b6, 2cbcf85):
  - Model branches on exact synthetic data: F_A/F_B ≠ 1, static lines
    (LSQR and SVD), weights from `edit.dat`, SB1, only 4 spectra.
  - Flux ratio optimisation ("Opt. Ratios"), only with `--slow`.
  - FITS: reading and solution identical to ASCII, float32 columns, wrong extension name.
  - Click test of all buttons and menu items (`gui_smoke`).
  - Monte Carlo tools for orbit fit and error estimation:
    `tests/tools/make_montecarlo.py`, `tests/tools/eval_montecarlo.py`.

### Migration notes

- RV files with positive values for redshift remain valid. If RVs were deliberately
  entered with the opposite sign to compensate for the old behaviour, they must be
  reversed. Also check `flip` (checkBox_13).
- Orbital elements from older fits as starting values: γ → −γ and ω → ω + π. For K_A ≈ K_B
  possibly also swap A and B.
- `edit.dat` still gives the weights directly. The "Flux Ratio A/B" field is now
  actually F_A/F_B.
- For SB1, too, A is weighted with F_A/(F_A+F_B) from "Flux Ratio A/B", as for SB2. The
  output component is thus normalised to the light of A alone (for F_A/F_B = 1
  continuum 2, if the observation is normalised to 1).
- Comparison with older residua: switch off "Exclude edges".
- Have the editor rewrite `initialstep.dat` (ω step now in radians).

### Known limitations

- The linear interpolation shifts results systematically by up to about 0.1 pixel
  (measured on the coarsest test grid). The error estimation does not include this
  contribution. Monte Carlo at full resolution (S/N 65, eccentric orbit): noise-free
  0.2–0.7 σ, e.g. K_B −0.018 km/s, e −0.0004.
- **Standard errors too small at moderate S/N, cause identified.** The error estimation
  yields the Cramér-Rao bound: on noise-free data at the true orbit it agrees
  with the mean of the reported errors from 40 Monte Carlo runs to within 1–4 %,
  and the residuum follows the quadratic model up to ±4σ. At S/N 65 the fits nevertheless
  scatter more by a factor of 1.1–1.9 (K_B 1.1, P/T/e/ω 1.3–1.4, K_A 1.9), with a
  small bias (K_A ≈ 1σ), even when the data are generated exactly with the program's
  model. The optimiser is not the cause: an independent Gauss-Newton
  fit finds no smaller residua. At S/N 260 the excess disappears
  (ratios 0.85–1.09, ±0.16), and the bias in units of σ drops to a quarter.
  It is therefore a second-order effect in the noise: the derivative of the model with
  respect to the RVs depends on the noisy component spectra estimated alongside (around
  2900 unknowns). In practice: at S/N below about 100 the errors are a lower bound;
  reliable errors come from Monte Carlo or bootstrap (`tests/tools/make_montecarlo.py`).
  If a value is entered in the "S/N" field, the error estimation also uses
  σ = 1/(S/N) instead of the estimate from the residuum.
- γ cannot be determined by disentangling alone. A common shift of both
  components only changes the reconstructed spectra. In the orbit fit γ should be
  fixed; the value and error of a free γ are not meaningful.
- LSQR has no explicit threshold for small singular values. For very poorly
  determined directions (e.g. extremely long spectra with little RV coverage)
  "SVD (DaC)" can serve for comparison, as long as memory suffices.
- For very long spectra, plots and file output become slow.
- OrbitFit and SimpleSubtraction are unfinished and not integrated into the GUI.
- The outermost pixels of the component spectra are seen by single spectra with small
  interpolation weights and can take arbitrary values (with cubic interpolation sometimes
  hundreds). They carry no information; the plots ignore them, the files still contain them.
