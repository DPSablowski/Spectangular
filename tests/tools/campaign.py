#!/usr/bin/env python3
"""Test campaign: user errors and combinations of options, beyond the regression suite.

Every scenario copies a data set, changes files or settings, loads the configuration in the
test adapter and calls a sequence of slots of the main window (adapter case "slots"). The
outcome is classified:

  ok       the slots ran; outputs (component files in the work folder) checked for NaN/inf
  message  a message box appeared (the user error was caught); its text is recorded
  silent   a slot failed without message (adapter FAILED) or outputs contain NaN/inf
  crash    the program ended by a signal or an unexpected exit code
  hang     no end within the time limit

Each scenario states what is expected ("ok", "message" or "any"); a deviation is a finding.

  python3 tests/tools/campaign.py [--only PATTERN] [--out DIR] [--timeout S] [--part errors|combos|all]

Environment as for run_tests.py (SPECTEST).
"""

import argparse
import itertools
import json
import math
import os
import re
import shutil
import signal
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(HERE))
import run_tests as rt  # noqa: E402

import numpy as np  # noqa: E402

READ, SOLVE, OPT, ERR, SHOWRV, OPTFR, PLOT, STAT, SUBTR, DETECT = (
    "on_pushButton_2_clicked", "on_pushButton_3_clicked", "on_pushButton_4_clicked",
    "on_pushButton_8_clicked", "on_pushButton_15_clicked", "on_pushButton_13_clicked",
    "on_pushButton_6_clicked", "on_pushButton_stat_clicked", "on_pushButton_12_clicked",
    "on_pushButton_telldetect_clicked")


# ---------------------------------------------------------------- helpers for mutations

def conf(w, idx, val):
    rt._conf_set(w, idx, str(val))


def v2(w, **kv):
    rt._conf_v2(w, **kv)


def spec(w, j):
    return w / f"spectram_{j}.txt"


def rewrite(w, j, fn):
    d = np.loadtxt(spec(w, j))
    d = fn(d)
    np.savetxt(spec(w, j), d, fmt="%.12g", delimiter="\t")


def write(w, name, text):
    (w / name).write_text(text)


def set_val(d, i, col, v):
    d = d.copy()
    d[i, col] = v
    return d


# ---------------------------------------------------------------- scenarios

S = []   # (name, dataset, mutate, slots, expect, description)


def sc(name, dataset, mutate, slots, expect, desc):
    S.append((name, dataset, mutate, slots, expect, desc))


RS = [READ, SOLVE]

# --- spectra
# (a missing work folder cannot be reached via a configuration: the work path is the folder of
# the configuration file)
sc("spec_prefix_wrong", "synthetic", lambda w: conf(w, 16, "nospec_"), RS, "message", "prefix matches no file")
sc("spec_file_missing", "synthetic", lambda w: spec(w, 3).unlink(), RS, "message", "one spectrum file missing")
sc("spec_file_empty", "synthetic", lambda w: write(w, "spectram_3.txt", ""), RS, "message", "empty spectrum file")
sc("spec_one_column", "synthetic", lambda w: rewrite(w, 3, lambda d: d[:, :1]), RS, "message", "spectrum with one column")
sc("spec_text_line", "synthetic", lambda w: write(w, "spectram_3.txt",
   "\n".join(l if i != 50 else "abc def" for i, l in enumerate(spec(w, 3).read_text().splitlines())) + "\n"),
   RS, "message", "a line with text in the middle of a spectrum")
sc("spec_nan_flux", "synthetic", lambda w: rewrite(w, 3, lambda d: set_val(d, 60, 1, np.nan)), RS, "message", "NaN flux")
sc("spec_inf_flux", "synthetic", lambda w: rewrite(w, 3, lambda d: set_val(d, 60, 1, np.inf)), RS, "message", "inf flux")
sc("spec_nan_wave", "synthetic", lambda w: rewrite(w, 3, lambda d: set_val(d, 60, 0, np.nan)), RS, "message", "NaN wavelength")
sc("spec_reversed", "synthetic", lambda w: rewrite(w, 3, lambda d: d[::-1]), RS, "any", "one spectrum in descending wavelength order")
sc("spec_all_reversed", "synthetic", lambda w: [rewrite(w, j, lambda d: d[::-1]) for j in range(10)], RS, "any",
   "all spectra in descending order")
sc("spec_duplicate_wave", "synthetic", lambda w: rewrite(w, 3, lambda d: set_val(d, 61, 0, d[60, 0])), RS, "any",
   "duplicate wavelength in one spectrum")
sc("spec_angstrom", "synthetic", lambda w: [rewrite(w, j, lambda d: np.column_stack([10 ** d[:, 0], d[:, 1]]))
                                            for j in range(10)], RS, "ok", "wavelengths in Angstrom instead of log10")
sc("spec_mixed_units", "synthetic", lambda w: rewrite(w, 3, lambda d: np.column_stack([10 ** d[:, 0], d[:, 1]])),
   RS, "any", "one spectrum in Angstrom, the others log10")
sc("spec_short_2px", "synthetic", lambda w: rewrite(w, 3, lambda d: d[:2]), RS, "message", "one spectrum with 2 pixels")
sc("spec_all_short_3px", "synthetic", lambda w: [rewrite(w, j, lambda d: d[:3]) for j in range(10)], RS, "message",
   "all spectra with 3 pixels")
sc("spec_no_overlap", "synthetic", lambda w: rewrite(w, 3, lambda d: np.column_stack([d[:, 0] + 0.1, d[:, 1]])),
   RS, "message", "one spectrum without common wavelength range")
sc("spec_first_gt_last", "synthetic", lambda w: (conf(w, 0, 5), conf(w, 1, 2)), RS, "message", "first index > last")
sc("spec_last_beyond", "synthetic", lambda w: conf(w, 1, 15), RS, "message", "last index beyond the files")
sc("spec_single", "synthetic", lambda w: conf(w, 1, 0), RS, "message", "only one spectrum")
sc("spec_two", "synthetic", lambda w: conf(w, 1, 1), RS, "any", "only two spectra")
sc("spec_zero_flux", "synthetic", lambda w: [rewrite(w, j, lambda d: np.column_stack([d[:, 0], 0 * d[:, 1]]))
                                             for j in range(10)], RS, "any", "all fluxes zero")
sc("spec_const_flux", "synthetic", lambda w: [rewrite(w, j, lambda d: np.column_stack([d[:, 0], 1 + 0 * d[:, 1]]))
                                              for j in range(10)], RS, "ok", "constant flux (no lines)")
sc("spec_crlf", "synthetic", lambda w: spec(w, 3).write_bytes(spec(w, 3).read_bytes().replace(b"\n", b"\r\n")),
   RS, "ok", "Windows line ends")
sc("spec_comma_decimal", "synthetic", lambda w: write(w, "spectram_3.txt", spec(w, 3).read_text().replace(".", ",")),
   RS, "message", "comma as decimal separator")
sc("spec_header_line", "synthetic", lambda w: write(w, "spectram_3.txt", "# lambda flux\n" + spec(w, 3).read_text()),
   RS, "ok", "comment header line")
sc("spec_grid_inner", "synthetic", lambda w: rewrite(w, 3, lambda d: np.column_stack([d[:, 0] + np.where(np.arange(len(d)) > 80, 0.3 * (d[1, 0] - d[0, 0]), 0), d[:, 1]])),
   RS, "any", "one spectrum with a jump of 0.3 pixel in its grid (ends unchanged)")
sc("spec_extra_columns", "synthetic", lambda w: rewrite(w, 3, lambda d: np.column_stack([d, d[:, 1]])), RS, "ok",
   "third column (error)")

# --- RVs (RV mode, synthetic)
sc("rv_missing", "synthetic", lambda w: (w / "velocities.txt").unlink(), RS, "message", "RV file missing")
sc("rv_short", "synthetic", lambda w: write(w, "velocities.txt", "\n".join((w / "velocities.txt").read_text().splitlines()[:5]) + "\n"),
   RS, "message", "RV file with too few lines")
sc("rv_long", "synthetic", lambda w: write(w, "velocities.txt", (w / "velocities.txt").read_text() * 2), RS, "message",
   "RV file with too many lines")
sc("rv_one_column", "synthetic", lambda w: write(w, "velocities.txt", "\n".join(l.split()[0] for l in (w / "velocities.txt").read_text().splitlines()) + "\n"),
   RS, "message", "SB2 but one RV column")
sc("rv_nan", "synthetic", lambda w: write(w, "velocities.txt", (w / "velocities.txt").read_text().replace((w / "velocities.txt").read_text().split()[2], "nan", 1)),
   RS, "message", "NaN RV")
sc("rv_huge", "synthetic", lambda w: write(w, "velocities.txt", "1e6\t-1e6\n" + "\n".join((w / "velocities.txt").read_text().splitlines()[1:]) + "\n"),
   RS, "message", "RV 1e6 km/s")
sc("rv_text", "synthetic", lambda w: write(w, "velocities.txt", "a b\n" + "\n".join((w / "velocities.txt").read_text().splitlines()[1:]) + "\n"),
   RS, "message", "text in the RV file")

# --- orbit mode (synthetic_orbit)
OR = "synthetic_orbit"
sc("times_missing", OR, lambda w: (w / "times.dat").unlink(), RS, "message", "times file missing")
sc("times_short", OR, lambda w: write(w, "times.dat", "\n".join((w / "times.dat").read_text().splitlines()[:4]) + "\n"), RS, "message", "too few times")
sc("times_nan", OR, lambda w: write(w, "times.dat", "nan\n" + "\n".join((w / "times.dat").read_text().splitlines()[1:]) + "\n"), RS, "message", "NaN time")
sc("orbit_missing", OR, lambda w: (w / "orbitelements.dat").unlink(), RS, "message", "orbital elements missing")
sc("orbit_six", OR, lambda w: write(w, "orbitelements.dat", "\n".join((w / "orbitelements.dat").read_text().split()[:6]) + "\n"), RS, "message", "6 of 7 elements")
for nm, i, v in (("orbit_e_ge1", 1, "1.2"), ("orbit_e_neg", 1, "-0.1"), ("orbit_P_zero", 0, "0"), ("orbit_P_neg", 0, "-5"),
                 ("orbit_nan", 2, "nan"), ("orbit_K_neg", 2, "-40"), ("orbit_K_huge", 2, "1e6")):
    sc(nm, OR, (lambda i, v: lambda w: write(w, "orbitelements.dat", "\n".join(
        v if k == i else x for k, x in enumerate((w / "orbitelements.dat").read_text().split())) + "\n"))(i, v),
       RS, "message" if nm not in ("orbit_K_neg",) else "any", f"orbital element {i} = {v}")

# --- S/N weighting
sc("snr_missing", "synthetic", lambda w: v2(w, checkBox_41=1, lineEdit_29="nofile.dat"), RS, "message", "S/N file missing")
sc("snr_count", "synthetic", lambda w: (write(w, "snr.dat", "100\n100\n"), v2(w, checkBox_41=1, lineEdit_29="snr.dat")), RS, "message", "S/N file too short")
sc("snr_zero", "synthetic", lambda w: (write(w, "snr.dat", "0\n" + "100\n" * 9), v2(w, checkBox_41=1, lineEdit_29="snr.dat")), RS, "message", "S/N 0")
sc("snr_nan", "synthetic", lambda w: (write(w, "snr.dat", "nan\n" + "100\n" * 9), v2(w, checkBox_41=1, lineEdit_29="snr.dat")), RS, "message", "S/N NaN")
sc("snr_auto", "synthetic", lambda w: v2(w, checkBox_41=1, lineEdit_29="auto"), RS, "ok", "S/N auto")
sc("snr_empty_field", "synthetic", lambda w: v2(w, checkBox_41=1, lineEdit_29=""), RS, "message", "S/N field empty")

# --- barycentric correction
BCF = "\n".join(str(x) for x in np.linspace(-20, 20, 10)) + "\n"
sc("bc_missing", OR, lambda w: v2(w, lineEdit_bc="nobc.txt"), RS, "message", "BC file missing")
sc("bc_count", OR, lambda w: (write(w, "bc.txt", "1\n2\n"), v2(w, lineEdit_bc="bc.txt")), RS, "message", "BC file too short")
sc("bc_nan", OR, lambda w: (write(w, "bc.txt", "nan\n" + BCF.split("\n", 1)[1]), v2(w, lineEdit_bc="bc.txt")), RS, "message", "BC NaN")
sc("bc_huge", OR, lambda w: (write(w, "bc.txt", "5000\n" + BCF.split("\n", 1)[1]), v2(w, lineEdit_bc="bc.txt")), RS, "message", "BC 5000 km/s")
sc("bc_text", OR, lambda w: (write(w, "bc.txt", "abc\n" + BCF.split("\n", 1)[1]), v2(w, lineEdit_bc="bc.txt")), RS, "message", "text in BC file")
sc("bc_ok_obs", OR, lambda w: (write(w, "bc.txt", BCF), v2(w, lineEdit_bc="bc.txt")), RS, "ok", "BC file, observatory frame")
sc("bc_ok_corr_static", OR, lambda w: (write(w, "bc.txt", BCF), v2(w, lineEdit_bc="bc.txt", comboBox_bcframe=1), conf(w, 43, 1)), RS, "ok",
   "BC file, corrected spectra, static lines")
sc("bc_corr_static_nofile", OR, lambda w: (v2(w, comboBox_bcframe=1), conf(w, 43, 1)), RS, "message", "corrected + static without BC file")

# --- strength of the static lines
def _stat(w):
    conf(w, 43, 1)
sc("tellw_missing", OR, lambda w: (_stat(w), v2(w, lineEdit_tellw="noam.txt")), RS, "message", "strength file missing")
sc("tellw_count", OR, lambda w: (_stat(w), write(w, "am.txt", "1\n2\n"), v2(w, lineEdit_tellw="am.txt")), RS, "message", "strength file too short")
sc("tellw_zero", OR, lambda w: (_stat(w), write(w, "am.txt", "0\n" + "1\n" * 9), v2(w, lineEdit_tellw="am.txt")), RS, "message", "strength 0")
sc("tellw_nan", OR, lambda w: (_stat(w), write(w, "am.txt", "nan\n" + "1\n" * 9), v2(w, lineEdit_tellw="am.txt")), RS, "message", "strength NaN")
sc("tellw_unused", OR, lambda w: v2(w, lineEdit_tellw="noam.txt"), RS, "ok", "strength file missing, but no static lines")
sc("tellfit_nostatic", OR, lambda w: v2(w, checkBox_tellfit=1), RS, "ok", "fit per spectrum without static lines")
sc("tellfit_static", OR, lambda w: (_stat(w), v2(w, checkBox_tellfit=1)), RS, "ok", "fit per spectrum with static lines")

# --- excluded ranges (mask) and static ranges
LAM = None
sc("mask_missing_typo", "synthetic", lambda w: v2(w, checkBox_mask=1, lineEdit_mask="mask.dat"), RS, "message",
   "mask file with another name missing (typo): error, default only for telluric_mask.dat")
sc("mask_missing_default", "synthetic", lambda w: v2(w, checkBox_mask=1, lineEdit_mask="telluric_mask.dat"), RS, "ok",
   "telluric_mask.dat missing: default written")
sc("mask_bad_line", "synthetic", lambda w: (write(w, "m.dat", "6600 abc\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, "message", "mask: invalid line")
sc("mask_from_gt_to", "synthetic", lambda w: (write(w, "m.dat", "6700 6600\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, "message", "mask: from > to")
sc("mask_all", "synthetic", lambda w: (write(w, "m.dat", "1000 20000\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, "message", "mask covers everything")
sc("mask_outside", "synthetic", lambda w: (write(w, "m.dat", "3000 3100\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, "ok", "mask outside the data")
sc("mask_empty", "synthetic", lambda w: (write(w, "m.dat", "# none\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, "any", "mask file without ranges")
sc("mask_half", "synthetic", lambda w: (write(w, "m.dat", f"{10**3.8258:.1f} {10**3.8262:.1f}\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, "ok", "mask over part of the data")
sc("sranges_missing_typo", OR, lambda w: (_stat(w), v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat")), RS, "message",
   "static ranges file with another name missing (typo)")
sc("sranges_missing_default", OR, lambda w: (_stat(w), v2(w, checkBox_tellrange=1, lineEdit_tellrange="telluric_static.dat")), RS, "message",
   "telluric_static.dat missing: default written, but the data (6693-6750 A) lie outside all default ranges")
sc("sranges_outside", OR, lambda w: (_stat(w), write(w, "tr.dat", "3000 3100 O2\n"), v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat")), RS, "message",
   "static ranges outside the data")
sc("sranges_bad", OR, lambda w: (_stat(w), write(w, "tr.dat", "abc\n"), v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat")), RS, "message", "static ranges invalid")
sc("sranges_empty", OR, lambda w: (_stat(w), write(w, "tr.dat", "#\n"), v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat")), RS, "message", "static ranges empty")
sc("sranges_part", OR, lambda w: (_stat(w), write(w, "tr.dat", "6695 6700 O2\n6705 6710 H2O\n"), v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat", checkBox_tellfit=1)),
   RS, "ok", "static ranges with two groups, fit per spectrum")
sc("sranges_unchecked_static_off", OR, lambda w: v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat"), RS, "ok", "ranges ticked but no static lines")

# --- flux ratio
sc("fr_zero", "synthetic", lambda w: conf(w, 9, 0), RS, "message", "flux ratio 0")
sc("fr_neg", "synthetic", lambda w: conf(w, 9, -2), RS, "message", "flux ratio negative")
sc("fr_table_missing", "synthetic", lambda w: v2(w, comboBox_ratio=2, lineEdit_ratio="nofr.dat"), RS, "message", "ratio table missing")
sc("fr_table_nan", "synthetic", lambda w: (write(w, "fr.dat", "6000 1\n7000 nan\n"), v2(w, comboBox_ratio=2, lineEdit_ratio="fr.dat")), RS, "message", "ratio table with NaN")
sc("fr_table_one", "synthetic", lambda w: (write(w, "fr.dat", "6000 1\n"), v2(w, comboBox_ratio=2, lineEdit_ratio="fr.dat")), RS, "message", "ratio table with one row")
sc("fr_table_neg", "synthetic", lambda w: (write(w, "fr.dat", "6000 -1\n7000 1\n"), v2(w, comboBox_ratio=2, lineEdit_ratio="fr.dat")), RS, "message", "ratio table with negative ratio")
sc("fr_planck_T0", "synthetic", lambda w: v2(w, comboBox_ratio=1, doubleSpinBox_TA=0, doubleSpinBox_TB=5000), RS, "message", "Planck with T_A = 0")
sc("fr_planck_lref_out", "synthetic", lambda w: v2(w, comboBox_ratio=1, doubleSpinBox_TA=6000, doubleSpinBox_TB=5000, doubleSpinBox_lref=100000), RS, "any",
   "Planck with lambda_ref far outside")
sc("edit_count", "synthetic", lambda w: write(w, "edit.dat", "1\n2\n"), RS, "message", "edit.dat wrong count")
sc("edit_nan", "synthetic", lambda w: write(w, "edit.dat", "".join("1\n" for _ in range(10)) + "nan\n" + "0.5\n" * 9 + "0.5\n" * 10), RS, "message", "edit.dat NaN")
sc("edit_neg", "synthetic", lambda w: write(w, "edit.dat", "1\n" * 10 + "-0.5\n" * 10 + "0.5\n" * 10), RS, "message", "edit.dat negative weight")

# --- numeric settings
sc("iter_zero_opt", "synthetic", lambda w: conf(w, 2, 0), [READ, OPT], "any", "optimisation with 0 iterations")
sc("norm_knot_small", "synthetic", lambda w: v2(w, checkBox_norm=1, doubleSpinBox_normKnot=0.01), RS, "any", "normalisation knots every 0.01 A")
sc("align_width_1", "synthetic", lambda w: v2(w, checkBox_align=1, doubleSpinBox_alignWidth=1), RS, "any", "align width 1")
sc("align_width_huge", "synthetic", lambda w: v2(w, checkBox_align=1, doubleSpinBox_alignWidth=100000), RS, "any", "align width huge")
sc("tdamp_one", OR, lambda w: (_stat(w), v2(w, doubleSpinBox_tdamp=1)), RS, "ok", "damping 1")
sc("range_lo_gt_hi", "synthetic", lambda w: (conf(w, 55, 1), conf(w, 11, 6800), conf(w, 12, 6600)), RS, "message", "residuum range from > to")
sc("range_outside", "synthetic", lambda w: (conf(w, 55, 1), conf(w, 11, 3000), conf(w, 12, 3100)), RS, "message", "residuum range outside the data")
sc("fixed_rv_bad", "synthetic", lambda w: v2(w, lineEdit_fixA="99"), [READ, OPT], "message", "fixed RV of a spectrum that does not exist")
sc("fixed_rv_text", "synthetic", lambda w: v2(w, lineEdit_fixA="a,b"), [READ, OPT], "message", "fixed RVs as text")
sc("mc_zero", OR, lambda w: (rt.mut_orbit_fit(w), v2(w, comboBox_err=1, spinBox_mc=0)), [READ, OPT, ERR], "any", "Monte Carlo with 0 realisations")

# --- output
sc("out_name_empty", "synthetic", lambda w: conf(w, 18, ""), RS, "message", "output name of A empty")
sc("out_subdir_missing", "synthetic", lambda w: conf(w, 18, "nodir/A.txt"), RS, "message", "output in a missing subfolder")
sc("out_readonly", "synthetic", lambda w: os.chmod(w, 0o555), RS, "message" if os.geteuid() != 0 else "any",
   "work folder read-only (as root writable anyway)")
sc("out_is_dir", "synthetic", lambda w: ((w / "adir").mkdir(), conf(w, 18, "adir")), RS, "message", "output name is a folder")
sc("out_spaces", "synthetic", lambda w: conf(w, 18, "A with spaces.txt"), RS, "ok", "output name with spaces")

# --- order of operations (no Read data)
for nm, sl in (("noread_solve", [SOLVE]), ("noread_opt", [OPT]), ("noread_err", [ERR]), ("noread_showrv", [SHOWRV]),
               ("noread_optfr", [OPTFR]), ("noread_plot", [PLOT]), ("noread_stat", [STAT]), ("noread_subtract", [SUBTR]),
               ("noread_detect", [DETECT])):
    sc(nm, "synthetic", lambda w: None, sl, "any", f"{sl[0]} without Read data")
sc("err_rv_mode", "synthetic", lambda w: None, [READ, ERR], "message", "error estimation in the RV mode")
sc("err_no_simplex", OR, lambda w: None, [READ, ERR], "message", "error estimation without an optimisation")
sc("all_locked", OR, lambda w: (conf(w, 52, 1), conf(w, 53, 1), conf(w, 57, 1), v2(w, checkBox_31=1, checkBox_36=1, checkBox_37=1, checkBox_38=1)),
   [READ, OPT], "message", "all orbital elements locked")
sc("lm_peak", OR, lambda w: (rt.mut_orbit_fit(w), v2(w, comboBox_method=1), conf(w, 49, 0), conf(w, 50, 1)), [READ, OPT], "message",
   "Levenberg-Marquardt with residuum Peak")
sc("continue_nofiles", OR, lambda w: (rt.mut_orbit_fit(w), conf(w, 44, 0), conf(w, 46, 1)), [READ, OPT], "any", "Continue without earlier files")
sc("detect_nostatic", OR, lambda w: None, [READ, SOLVE, DETECT], "message", "Detect without static lines")
sc("sb3", "synthetic", lambda w: (conf(w, 56, 1), conf(w, 39, 0)), RS, "message", "SB3")
sc("no_sb", "synthetic", lambda w: (conf(w, 38, 0), conf(w, 39, 0)), RS, "message", "neither SB1 nor SB2")
sc("solve_twice_after_error", "synthetic", lambda w: (w / "velocities.txt").unlink(), [READ, SOLVE, SOLVE, OPT], "message",
   "after a read error: Solve and Optimise (state)", )


# --- settings changed between the steps (state after Read data / Solve)
def _tr(w):
    lam = 10 ** np.loadtxt(w / "spectram_0.txt")[:, 0]
    write(w, "tr.dat", f"{lam[60]:.2f} {lam[70]:.2f} O2\n{lam[100]:.2f} {lam[110]:.2f} H2O\n")
for nm, ds, steps, exp in (
        ("state_static_after_read", "synthetic", [READ, "set:checkBox_8=1", SOLVE], "any"),
        ("state_static_off_after_read", "synthetic", [READ, "set:checkBox_8=1", READ, SOLVE, "set:checkBox_8=0", SOLVE], "any"),
        ("state_orbit_after_read", "synthetic", [READ, "set:checkBox_2=0", "set:checkBox_16=1", SOLVE], "any"),
        ("state_fewer_spectra", "synthetic", [READ, SOLVE, "set:spinBox_2=5", SOLVE], "any"),
        ("state_prefix_after_read", "synthetic", [READ, "set:lineEdit_3=nospec_", SOLVE], "any"),
        ("state_ranges_after_read", "synthetic_orbit", [READ, "set:checkBox_8=1", "set:checkBox_tellrange=1",
                                                        "set:lineEdit_tellrange=tr.dat", "set:checkBox_tellfit=1", SOLVE], "any"),
        ("state_sb1_after_read", "synthetic", [READ, "set:checkBox_4=0", "set:checkBox_3=1", SOLVE], "any"),
        ("state_cubic_after_read", "synthetic", [READ, "set:comboBox_interp=1", SOLVE, OPT], "any"),
        ("state_mask_after_solve", "synthetic", [READ, SOLVE, "set:checkBox_mask=1", "set:lineEdit_mask=telluric_mask.dat", SOLVE, STAT], "any"),
        ("state_svd_after_lsqr", "synthetic", [READ, SOLVE, "set:checkBox_5=0", "set:checkBox_6=1", SOLVE], "any"),
        ("state_ratio_after_read", "synthetic", [READ, "set:doubleSpinBox_4=3", SOLVE], "any"),
        ("state_bc_after_read", "synthetic_orbit", [READ, "set:lineEdit_bc=bc.txt", SOLVE, OPT], "any"),
        ("state_detect_then_ranges", "synthetic_orbit", [READ, "set:checkBox_8=1", READ, SOLVE, DETECT,
                                                         "set:checkBox_tellrange=1", READ, SOLVE], "any"),
        ("state_repeat_read", "synthetic", [READ, READ, READ, SOLVE, SOLVE, OPT, OPT], "ok"),
        ("state_subtract_after_change", "synthetic", [READ, SOLVE, "set:spinBox_2=5", READ, SUBTR], "any")):
    sc(nm, ds, (lambda w: (_tr(w), write(w, "bc.txt", "\n".join(f"{x:.2f}" for x in 10 * np.sin(np.arange(10))) + "\n"))),
       steps, exp, "settings changed between steps: " + " ".join(x.replace("on_pushButton_", "b").replace("_clicked", "") for x in steps))

# --- configuration files
sc("conf_text_in_number", "synthetic", lambda w: conf(w, 2, "abc"), RS, "any", "configuration: text in a numeric field")
sc("conf_truncated", "synthetic", lambda w: write(w, "SB2_conf.sps", "\n".join((w / "SB2_conf.sps").read_text().splitlines()[:20]) + "\n"),
   RS, "any", "configuration truncated after 20 lines")
sc("conf_unknown_key", "synthetic", lambda w: v2(w, nonsense_key=5), RS, "message", "configuration with an unknown key: loaded, note")
sc("conf_huge_values", "synthetic", lambda w: (conf(w, 2, 999999999), conf(w, 9, 1e300)), RS, "any", "configuration with huge values")
sc("conf_empty", "synthetic", lambda w: write(w, "SB2_conf.sps", ""), RS, "any", "empty configuration")


# ---------------------------------------------------------------- metamorphic pairs
# Two runs that must give the same result (residuum and component spectra within tol).
META = []


def meta(name, dataset, mutA, mutB, slots, tol, desc):
    META.append((name, dataset, mutA, mutB, slots, tol, desc))


def _lam(w):
    return 10 ** np.loadtxt(w / "spectram_0.txt")[:, 0]


meta("meta_mask_outside", "synthetic", lambda w: None,
     lambda w: (write(w, "m.dat", "3000 3100\n"), v2(w, checkBox_mask=1, lineEdit_mask="m.dat")), RS, 1e-9,
     "excluded range outside the data = no excluded range")
meta("meta_bc_zero", "synthetic_orbit", lambda w: None,
     lambda w: (write(w, "bc.txt", "0\n" * 10), v2(w, lineEdit_bc="bc.txt")), RS, 1e-9,
     "BC file with zeros (observatory frame) = no BC file")
meta("meta_bc_zero_corr_static", "synthetic_orbit", lambda w: conf(w, 43, 1),
     lambda w: (conf(w, 43, 1), write(w, "bc.txt", "0\n" * 10), v2(w, lineEdit_bc="bc.txt", comboBox_bcframe=1)), RS, 1e-6,
     "static lines, corrected spectra with BC 0 = observatory frame without BC")
meta("meta_strength_ones", "synthetic_orbit", lambda w: conf(w, 43, 1),
     lambda w: (conf(w, 43, 1), write(w, "am.txt", "2\n" * 10), v2(w, lineEdit_tellw="am.txt")), RS, 1e-9,
     "strength file with equal values (scaled to mean 1) = none")
meta("meta_ranges_all", "synthetic_orbit", lambda w: conf(w, 43, 1),
     lambda w: (conf(w, 43, 1), write(w, "tr.dat", f"{_lam(w)[0] - 50:.1f} {_lam(w)[-1] + 50:.1f}\n"),
                v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat")), RS, 1e-6,
     "static ranges covering all data = static lines without ranges")
meta("meta_groups_same", "synthetic_orbit",
     lambda w: (conf(w, 43, 1), write(w, "tr.dat", f"{_lam(w)[40]:.2f} {_lam(w)[80]:.2f}\n{_lam(w)[100]:.2f} {_lam(w)[140]:.2f}\n"),
                v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat", checkBox_tellfit=1)),
     lambda w: (conf(w, 43, 1), write(w, "tr.dat", f"{_lam(w)[40]:.2f} {_lam(w)[80]:.2f} X\n{_lam(w)[100]:.2f} {_lam(w)[140]:.2f} X\n"),
                v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat", checkBox_tellfit=1)), RS, 1e-6,
     "two ranges with the same group name = without names")
meta("meta_snr_equal", "synthetic", lambda w: None,
     lambda w: (write(w, "snr.dat", "150\n" * 10), v2(w, checkBox_41=1, lineEdit_29="snr.dat")), RS, 1e-6,
     "S/N file with equal values = unweighted (components)")
meta("meta_ratio_table_const", "synthetic", lambda w: conf(w, 9, 2),
     lambda w: (conf(w, 9, 2), write(w, "fr.dat", "5000 2\n9000 2\n"), v2(w, comboBox_ratio=2, lineEdit_ratio="fr.dat")), RS, 1e-6,
     "flux-ratio table with a constant 2 = constant ratio 2")
meta("meta_planck_equal_T", "synthetic", lambda w: conf(w, 9, 2),
     lambda w: (conf(w, 9, 2), v2(w, comboBox_ratio=1, doubleSpinBox_TA=6000, doubleSpinBox_TB=6000)), RS, 1e-6,
     "Planck with equal temperatures = constant ratio")
meta("meta_lsqr_svd", "synthetic", lambda w: None, lambda w: v2(w, solver="svd_dc"), RS, 1e-6,
     "LSQR = SVD")
def _rv_from_orbit(w):
    el = np.loadtxt(w / "orbitelements.dat")
    t = np.loadtxt(w / "times.dat")
    np.savetxt(w / "velocities.txt", np.column_stack([rt._orbit_rv(t, el, "A"), rt._orbit_rv(t, el, "B")]),
               fmt="%.12f", delimiter="\t")
    conf(w, 51, 0); conf(w, 37, 1)


meta("meta_rv_vs_orbit", "synthetic_orbit", lambda w: None, _rv_from_orbit, RS, 1e-6,
     "orbit mode = RV mode with the RVs of the same orbit (velocities.txt)")
meta("meta_damp_zero_nostatic", "synthetic", lambda w: None, lambda w: v2(w, doubleSpinBox_tdamp=0.5), RS, 1e-12,
     "damping of static lines has no effect without static lines")
meta("meta_tellfit_nostatic", "synthetic", lambda w: None, lambda w: v2(w, checkBox_tellfit=1), RS, 1e-12,
     "fit per spectrum has no effect without static lines")
meta("meta_norm_raw", "synthetic", lambda w: None, lambda w: v2(w, checkBox_norm=1), RS, 1e-12,
     "writing normalised components leaves the raw components unchanged")


def run_meta(name, dataset, mutA, mutB, slots, tol, desc, root, timeout):
    ra = run(name + "_A", dataset, mutA, slots, "ok", desc, root, timeout)
    rb = run(name + "_B", dataset, mutB, slots, "ok", desc, root, timeout)
    if ra["outcome"] != "ok" or rb["outcome"] != "ok":
        return dict(name=name, desc=desc, expect="same", outcome="error", finding=True, seconds=ra["seconds"] + rb["seconds"],
                    detail=f"A: {ra['outcome']} {ra['detail'][:80]}; B: {rb['outcome']} {rb['detail'][:80]}")
    wa, wb = root / (name + "_A") / "work", root / (name + "_B") / "work"
    oa, ob = root / (name + "_A") / "out", root / (name + "_B") / "out"
    resA = float(re.search(r"residuum\s+(\S+)", (oa / "result.txt").read_text()).group(1))
    resB = float(re.search(r"residuum\s+(\S+)", (ob / "result.txt").read_text()).group(1))
    dmax = 0.0
    for f in ("SB2_1.txt", "SB2_2.txt"):
        if (wa / f).exists() and (wb / f).exists():
            a, b = np.loadtxt(wa / f, ndmin=2), np.loadtxt(wb / f, ndmin=2)
            if a.shape != b.shape:
                dmax = float("inf")
            else:
                dmax = max(dmax, float(np.max(np.abs(a - b))))
    rel = abs(resA - resB) / max(abs(resA), 1e-300)
    # exact (noise-free) data: residua near zero differ only by rounding
    ok = (rel < tol or max(abs(resA), abs(resB)) < 1e-8) and dmax < max(tol, 1e-9) * 10
    return dict(name=name, desc=desc, expect="same", outcome="same" if ok else "differs", finding=not ok,
                seconds=ra["seconds"] + rb["seconds"],
                detail=f"residuum {resA:.10g} / {resB:.10g} (rel {rel:.1e}), components max diff {dmax:.1e} ({desc})")


# ---------------------------------------------------------------- runner

def finite_outputs(w):
    bad = []
    for name in ("SB2_1.txt", "SB2_2.txt", "SB2_tell.txt", "SB2_diff.txt"):
        f = w / name
        if not f.exists() or f.stat().st_size == 0:
            continue
        try:
            d = np.genfromtxt(f, comments="#", invalid_raise=False)
        except Exception:  # noqa: BLE001
            continue
        if d.size and not np.all(np.isfinite(d)):
            bad.append(name)
    return bad


def run(name, dataset, mutate, slots, expect, desc, root, timeout, close_modals=False, case="slots", extra_env=None):
    w, o = root / name / "work", root / name / "out"
    if (root / name).exists():
        for p in (root / name).rglob("*"):
            try:
                os.chmod(p, 0o755)
            except OSError:
                pass
        shutil.rmtree(root / name, ignore_errors=True)
    rt.prepare(dataset, w)
    o.mkdir(parents=True)
    mutate(w)
    env = dict(os.environ, SPECTEST_SLOTS=",".join(slots), QT_QPA_PLATFORM="offscreen", **(extra_env or {}))
    if close_modals:
        env["SPECTEST_CLOSE_MODALS"] = "1"
    t0 = time.time()
    try:
        p = subprocess.run([str(rt.SPECTEST), case, str(w), str(o)], capture_output=True, text=True,
                           timeout=timeout, env=env)
        rc, out, err = p.returncode, p.stdout, p.stderr
        hang = False
    except subprocess.TimeoutExpired as e:
        rc, out, err, hang = None, (e.stdout or b"").decode(errors="replace") if isinstance(e.stdout, bytes) else (e.stdout or ""), "", True
    dt = time.time() - t0
    try:
        os.chmod(w, 0o755)
    except OSError:
        pass
    (o / "stdout.log").write_text((out or "") + "\n--- stderr ---\n" + (err or ""))
    detail = ""
    if hang:
        outcome = "hang"
    elif (o / "MODAL.txt").exists():
        outcome, detail = "message", (o / "MODAL.txt").read_text().strip()
    elif (o / "FAILED.txt").exists():
        detail = (o / "FAILED.txt").read_text().strip()
        # loading the configuration failed with the program's own message (in the GUI a box)
        outcome = "message" if detail.startswith("Configuration:") else "silent"
    elif rc is not None and rc < 0:
        outcome, detail = "crash", f"signal {-rc}: {signal.Signals(-rc).name if -rc in [s.value for s in signal.Signals] else ''}; " + (err or "")[-300:]
    elif rc not in (0, None):
        outcome, detail = "crash", f"exit {rc}: " + (err or "")[-300:]
    elif "runtime error:" in (err or "") or "ERROR: AddressSanitizer" in (err or ""):
        i = (err or "").find("runtime error:")
        outcome, detail = "crash", "sanitizer: " + (err or "")[max(0, i - 200):i + 300]
    else:
        bad = finite_outputs(w)
        if bad:
            outcome, detail = "silent", "non-finite values in " + ", ".join(bad)
        else:
            outcome = "ok"
            if (o / "MODALS.txt").exists():
                detail = "messages: " + (o / "MODALS.txt").read_text().strip().replace("\n", " | ")
    finding = not (expect == "any" and outcome in ("ok", "message")) and outcome != expect
    return dict(name=name, desc=desc, expect=expect, outcome=outcome, detail=detail[:400], seconds=round(dt, 1),
                finding=finding)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--only", default="")
    ap.add_argument("--out", default="")
    ap.add_argument("--timeout", type=float, default=120)
    ap.add_argument("--part", default="errors", choices=["errors", "combos", "fuzz", "meta", "all"])
    args = ap.parse_args()
    root = Path(args.out) if args.out else Path(tempfile.mkdtemp(prefix="campaign_"))
    root.mkdir(parents=True, exist_ok=True)
    scen = []
    if args.part in ("errors", "all"):
        scen += S
    if args.part in ("combos", "all"):
        scen += combos()
    if args.part in ("fuzz", "all"):
        scen += fuzz()
    res = []
    if args.part in ("meta", "all"):
        for m in META:
            if args.only and not re.search(args.only, m[0]):
                continue
            r = run_meta(*m, root, args.timeout)
            res.append(r)
            flag = "FINDING" if r["finding"] else "ok     "
            print(f"{flag} {r['name']:32s} {r['detail'][:170]}", flush=True)
    for s in scen:
        if args.only and not re.search(args.only, s[0]):
            continue
        extra = s[6] if len(s) > 6 else {}
        r = run(*s[:6], root, args.timeout, case=extra.pop("case", "slots"), extra_env=extra)
        res.append(r)
        flag = "FINDING" if r["finding"] else "ok     "
        print(f"{flag} {r['name']:32s} expect {r['expect']:7s} got {r['outcome']:7s} {r['seconds']:6.1f}s  {r['detail'][:150]}",
              flush=True)
    (root / "results.json").write_text(json.dumps(res, indent=1))
    n = sum(r["finding"] for r in res)
    print(f"\n{len(res)} scenarios, {n} findings; results in {root}")


FACTORS = {
    "mode": ["rv_sb2", "rv_sb1", "orbit"],
    "interp": [0, 1],
    "solver": ["lsqr", "svd"],
    "static": [0, 1],
    "ranges": ["none", "one", "groups"],
    "tellfit": [0, 1],
    "bc": ["none", "obs", "corr"],
    "mask": [0, 1],
    "ratio": ["const", "planck", "table", "fr2"],
    "align": [0, 1],
    "norm": [0, 1],
    "snr": ["off", "auto", "file"],
    "edges": [0, 1],
    "rrange": [0, 1],
    "action": ["solve", "nm", "lm", "stat", "detect", "err"],
}


def allpairs(factors, seed=1):
    """Greedy all-pairs: every pair of values of two factors occurs in at least one case."""
    import random
    rnd = random.Random(seed)
    names = list(factors)
    uncovered = {(a, va, b, vb) for i, a in enumerate(names) for b in names[i + 1:]
                 for va in factors[a] for vb in factors[b]}
    cases = []
    while uncovered:
        best, bestn = None, -1
        for _ in range(60):
            c = {n: rnd.choice(factors[n]) for n in names}
            # start from an uncovered pair
            a, va, b, vb = rnd.choice(sorted(uncovered, key=str))
            c[a], c[b] = va, vb
            n = sum((x, c[x], y, c[y]) in uncovered for i, x in enumerate(names) for y in names[i + 1:])
            if n > bestn:
                best, bestn = c, n
        cases.append(best)
        uncovered -= {(x, best[x], y, best[y]) for i, x in enumerate(names) for y in names[i + 1:]}
    return cases


def combo_mutate(c):
    def m(w):
        if c["mode"] == "rv_sb1":
            conf(w, 38, 1); conf(w, 39, 0)
            write(w, "velocities.txt", "\n".join(l.split()[0] for l in (w / "velocities.txt").read_text().splitlines()) + "\n")
        if c["mode"] == "orbit":
            rt.mut_orbit_fit(w)
        conf(w, 2, 25)                                    # iterations: short optimisations
        v2(w, comboBox_interp=c["interp"])
        if c["solver"] == "svd":
            v2(w, solver="svd_dc")
        conf(w, 43, c["static"])
        lam = 10 ** np.loadtxt(w / "spectram_0.txt")[:, 0]
        lo, hi = lam[0], lam[-1]
        if c["ranges"] != "none":
            a = lo + 0.3 * (hi - lo)
            txt = f"{a:.2f} {a + 4:.2f} O2\n" + (f"{a + 10:.2f} {a + 14:.2f} H2O\n" if c["ranges"] == "groups" else "")
            write(w, "tr.dat", txt)
            v2(w, checkBox_tellrange=1, lineEdit_tellrange="tr.dat")
        v2(w, checkBox_tellfit=c["tellfit"])
        if c["bc"] != "none":
            n = len(list(w.glob("spectram_*.txt")))
            write(w, "bc.txt", "\n".join(f"{x:.3f}" for x in 20 * np.sin(np.arange(n))) + "\n")
            v2(w, lineEdit_bc="bc.txt", comboBox_bcframe=1 if c["bc"] == "corr" else 0)
        if c["mask"]:
            a = lo + 0.6 * (hi - lo)
            write(w, "m.dat", f"{a:.2f} {a + 3:.2f}\n")
            v2(w, checkBox_mask=1, lineEdit_mask="m.dat")
        if c["ratio"] == "planck":
            v2(w, comboBox_ratio=1, doubleSpinBox_TA=7000, doubleSpinBox_TB=5000)
        elif c["ratio"] == "table":
            write(w, "fr.dat", f"{lo - 10:.1f} 1.5\n{hi + 10:.1f} 2.5\n")
            v2(w, comboBox_ratio=2, lineEdit_ratio="fr.dat")
        elif c["ratio"] == "fr2":
            conf(w, 9, 2)
        v2(w, checkBox_align=c["align"], checkBox_norm=c["norm"])
        if c["snr"] == "auto":
            v2(w, checkBox_41=1, lineEdit_29="auto")
        elif c["snr"] == "file":
            n = len(list(w.glob("spectram_*.txt")))
            write(w, "snr.dat", "\n".join(str(100 + 20 * k) for k in range(n)) + "\n")
            v2(w, checkBox_41=1, lineEdit_29="snr.dat")
        v2(w, checkBox_40=c["edges"])
        if c["rrange"]:
            conf(w, 55, 1); conf(w, 11, round(lo + 0.1 * (hi - lo), 2)); conf(w, 12, round(lo + 0.9 * (hi - lo), 2))
        if c["action"] == "lm":
            v2(w, comboBox_method=1)
    return m


def combo_slots(c):
    a = c["action"]
    return {"solve": [READ, SOLVE], "nm": [READ, OPT], "lm": [READ, OPT], "stat": [READ, SOLVE, STAT],
            "detect": [READ, SOLVE, DETECT], "err": [READ, OPT, ERR]}[a]


def combos():
    out = []
    for k, c in enumerate(allpairs(FACTORS, 1) + allpairs(FACTORS, 2) + allpairs(FACTORS, 3)):
        ds = "synthetic_orbit" if c["mode"] == "orbit" else "synthetic"
        desc = " ".join(f"{n}={v}" for n, v in c.items())
        # messages expected only for: error estimation outside the orbit mode or with whole-pixel
        # shifts (step-shaped objective: the curvature can be indefinite, Error 38), Detect without
        # static lines
        expect = "any" if (c["action"] == "err" and (c["mode"] != "orbit" or not c["interp"])) \
            or (c["action"] == "detect" and not c["static"]) else "ok"
        out.append((f"combo_{k:03d}", ds, combo_mutate(c), combo_slots(c), expect, desc))
    return out


def fuzz():
    """Every field of a window gets a bad value (8 value sets), then its buttons are clicked."""
    wins = [("main", "", [READ], ["pushButton_2", "pushButton_3", "pushButton_6", "pushButton_stat", "pushButton_15",
                                  "pushButton_telldetect", "pushButton_11", "pushButton_12"]),
            ("editor", "on_actionEditor_triggered", [READ], []),
            ("orbitfit", "on_actionOrbitFit_triggered", [READ, SOLVE], []),
            ("plotter", "on_actionSpectrum_Plotter_triggered", [READ, SOLVE], []),
            ("correlation", "on_actionCorrelation_triggered", [], []),
            ("arithmetic", "on_actionArithmetic_triggered", [], []),
            ("fluxtemplate", "on_actionFluxTemplate_triggered", [READ, SOLVE], []),
            ("barycorr", "on_actionBaryCorr_triggered", [READ], [])]
    # (Subtract opens a modal dialog, which the fuzz driver closes at once: tested by the
    # regression cases subtract*)
    out = []
    for dataset in ("synthetic", "synthetic_orbit"):
        for wname, opener, pre, buttons in wins:
            for mode in range(8):
                env = dict(case="fuzz", SPECTEST_PRE=",".join(pre), SPECTEST_OPEN=opener, SPECTEST_FUZZ=str(mode),
                           SPECTEST_BUTTONS=",".join(buttons), SPECTEST_KEEP="lineEdit_4,spinBox_3,spinBox_8")
                out.append((f"fuzz_{wname}_{dataset}_{mode}", dataset, lambda w: None, [], "ok",
                            f"{wname}: all fields value set {mode}, all buttons", env))
    return out


if __name__ == "__main__":
    main()
