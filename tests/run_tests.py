#!/usr/bin/env python3
"""Test run for Spectangular.

Two kinds of checks:
  * Reference (golden): outputs of a step are compared with stored outputs.
    They capture today's behaviour, even where it is wrong, and safeguard the
    refactoring. Intentional behaviour changes (bug fixes) are re-recorded
    with --record and justified in the commit.
  * Expected (Soll): physically/mathematically correct result (synthetic binary,
    Kepler solution, repeatability). Known bugs are marked as XFAIL with a
    reference to the bug report; if a fix resolves the bug, XPASS appears
    and the marker is removed.

Usage:
  run_tests.py [--record] [--slow] [--keep] [case ...]
Environment:
  SPECTEST  path to the test driver (default: build/spectest)
"""
import argparse
import hashlib
import math
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

import numpy as np

HERE = Path(__file__).resolve().parent
DATA = HERE / "data"
GOLDEN = HERE / "golden"
SPECTEST = Path(os.environ.get("SPECTEST", HERE / "build" / "spectest"))
UNITTESTS = Path(os.environ.get("UNITTESTS", HERE / "build-unit" / "unittests"))
PROGRAM = Path(os.environ.get("SPECTANGULAR", HERE.parent / "spectangular" / "Spectangular"))
NUM_RE = re.compile(r"^[-+]?(\d+\.?\d*|\.\d+)([eE][-+]?\d+)?$|^[-+]?(nan|inf)$", re.I)


# -------------------------------------------------------------------- Helpers

class Check:
    def __init__(self, name, ok, detail="", xfail=None):
        self.name, self.ok, self.detail, self.xfail = name, bool(ok), detail, xfail

    @property
    def status(self):
        if self.xfail:
            return "XPASS" if self.ok else "XFAIL"
        return "PASS" if self.ok else "FAIL"


def scalars(path):
    out = {}
    for line in Path(path).read_text().splitlines():
        k, v = line.split()
        out[k] = float(v)
    return out


def compare_text(a, b, rtol, atol):
    """Compares two text files token by token; numbers with tolerance."""
    la, lb = Path(a).read_text().split("\n"), Path(b).read_text().split("\n")
    if len(la) != len(lb):
        return False, f"line count {len(la)} instead of {len(lb)}"
    worst = 0.0
    for i, (x, y) in enumerate(zip(la, lb)):
        ta, tb = x.split(), y.split()
        if len(ta) != len(tb):
            return False, f"line {i + 1}: {len(ta)} instead of {len(tb)} fields"
        for u, v in zip(ta, tb):
            if NUM_RE.match(u) and NUM_RE.match(v):
                fu, fv = float(u), float(v)
                if math.isnan(fu) and math.isnan(fv):
                    continue
                d = abs(fu - fv)
                if d > atol + rtol * abs(fv):
                    return False, f"line {i + 1}: {u} instead of {v}"
                if fv != 0:
                    worst = max(worst, d / abs(fv))
            elif u != v:
                return False, f"line {i + 1}: '{u}' instead of '{v}'"
    return True, f"max. rel. deviation {worst:.1e}"


def prepare(dataset, workdir):
    shutil.copytree(DATA / dataset, workdir)
    conf = workdir / "SB2_conf.sps"
    if conf.exists():
        conf.write_text(conf.read_text().replace("@DATADIR@", str(workdir)))


def run_adapter(case, workdir, outdir, timeout, env=None):
    t0 = time.time()
    p = subprocess.run([str(SPECTEST), case, str(workdir), str(outdir)],
                       capture_output=True, text=True, timeout=timeout,
                       env=dict(os.environ, **(env or {})))
    (outdir / "stdout.log").write_text(p.stdout + "\n--- stderr ---\n" + p.stderr)
    for marker in ("MODAL.txt", "FAILED.txt"):
        if (outdir / marker).exists():
            raise RuntimeError(f"{marker[:-4]}: {(outdir / marker).read_text().strip()}")
    if p.returncode != 0:
        raise RuntimeError(f"exit code {p.returncode}: {p.stderr.strip()[-300:]}")
    # Replace the work path with a placeholder so that references are location-independent
    for f in outdir.iterdir():
        if f.suffix in (".txt", ".sps", ".dat"):
            s = f.read_text()
            if str(workdir) in s:
                f.write_text(s.replace(str(workdir), "@DATADIR@"))
    return time.time() - t0


def golden_checks(name, outdir, files, rtol, atol, record):
    gdir = GOLDEN / name
    if record:
        if gdir.exists():
            shutil.rmtree(gdir)
        gdir.mkdir(parents=True)
        for f in files:
            shutil.copy(outdir / f, gdir / f)
        return [Check(f"Reference recorded ({len(files)} files)", True)]
    checks = []
    for f in files:
        if not (gdir / f).exists():
            checks.append(Check(f"Reference {f}", False, "no reference, run --record first"))
            continue
        ok, detail = compare_text(outdir / f, gdir / f, rtol, atol)
        checks.append(Check(f"Reference {f}", ok, detail))
    return checks


# ------------------------------------------------------------ Expected values

def kepler_rv(el, t):
    P, e, ka, kb, gam, tp, om = el
    M = np.mod(2 * np.pi * (t - tp) / P, 2 * np.pi)
    E = M + e * np.sin(M) if e < 0.8 else np.full_like(M, np.pi)
    for _ in range(100):
        E = E - (E - e * np.sin(E) - M) / (1 - e * np.cos(E))
    nu = 2 * np.arctan2(np.sqrt(1 + e) * np.sin(E / 2), np.sqrt(1 - e) * np.cos(E / 2))
    va = gam + ka * (np.cos(nu + om) + e * np.cos(om))
    vb = gam + kb * (np.cos(nu + om + np.pi) + e * np.cos(om + np.pi))
    return va, vb


def checks_orbit(workdir, outdir):
    orbits = np.loadtxt(workdir / "orbits.txt", ndmin=2)
    out = []
    for k, el in enumerate(orbits):
        d = np.loadtxt(outdir / f"rv_{k}.txt")
        va, vb = kepler_rv(el, d[:, 0])
        err = max(np.abs(d[:, 1] - va).max(), np.abs(d[:, 2] - vb).max())
        flag = (outdir / f"rv_{k}.txt").read_text().split("\n")[0]
        out.append(Check(f"RV against Kepler solution, e={el[1]:g}", err < 1e-10,
                         f"max. error {err:.2e} km/s, {flag.lstrip('# ')}"))
    return out


def component_match(outfile, truthfile, bso_offset_range=4):
    """Compares an output component with the truth; also checks for a pixel offset."""
    out = np.loadtxt(outfile)
    tr = np.loadtxt(truthfile)
    dlog = np.diff(np.log10(tr[:, 0])).mean()
    best = None
    for off in range(-bso_offset_range, bso_offset_range + 1):
        x = np.log10(out[:, 0]) + off * dlog
        inside = (x > np.log10(tr[5, 0])) & (x < np.log10(tr[-6, 0]))
        if inside.sum() < 20:
            continue
        ref = np.interp(x[inside], np.log10(tr[:, 0]), tr[:, 1])
        diff = out[inside, 1] - ref
        rms = np.sqrt(np.mean((diff - diff.mean()) ** 2))
        if best is None or rms < best[1]:
            best = (off, rms)
    return best


def checks_orbit_invalid(outdir):
    r = scalars(outdir / "result.txt")
    return [Check(f"Penalty residual for invalid elements ({k})", v >= 1e29, f"{v:.3g}")
            for k, v in r.items()]


def checks_synthetic(workdir, outdir):
    r = scalars(outdir / "result.txt")
    out = [
        Check("Residual for physical RVs (positive = redshift) ~ 0",
              r["residuum_physical"] < 1e-9, f"{r['residuum_physical']:.2e}"),
        Check("Negated RVs do not fit", r["residuum_negated"] > 1e-3,
              f"{r['residuum_negated']:.3e}"),
        Check("Residual rises when one spectrum is perturbed by 1 pixel",
              r["residuum_perturbed"] > 1e-3, f"{r['residuum_perturbed']:.3e}"),
        Check("Repeated evaluation after other evaluations ~ 0",
              r["residuum_physical_again"] < 1e-9, f"{r['residuum_physical_again']:.2e}"),
    ]
    for comp, f, t in (("A", "SB2_1.txt", "truth_A.txt"), ("B", "SB2_2.txt", "truth_B.txt")):
        off, rms = component_match(outdir / f, workdir / t)
        out.append(Check(f"Component {comp}: shape matches truth", rms < 1e-6,
                         f"RMS {rms:.2e} at offset {off} px"))
        out.append(Check(f"Component {comp}: wavelength axis without offset", off == 0,
                         f"best offset {off} px"))
    return out


def checks_interpolation(outdir):
    r = scalars(outdir / "result.txt")
    r0 = r["r_+0"]
    others = {k: v for k, v in r.items() if k != "r_+0"}
    return [
        Check("Residual minimal at the true RV", all(v > r0 for v in others.values()),
              f"r(0) = {r0:.4e}; " + ", ".join(f"{k[2:]}: {v:.4e}" for k, v in sorted(others.items()))),
        Check("Residual changes continuously (no plateau at 0.1 px)",
              abs(r["r_+0.1"] - r0) > 1e-6 * r0 and abs(r["r_-0.1"] - r0) > 1e-6 * r0,
              f"r(+0.1) - r(0) = {r['r_+0.1'] - r0:.2e}"),
        Check("Residual grows with distance",
              r0 < r["r_+0.1"] < r["r_+0.25"] < r["r_+0.5"] and r0 < r["r_-0.1"] < r["r_-0.25"] < r["r_-0.5"],
              ""),
    ]


def checks_repeat(outdir):
    r = scalars(outdir / "result.txt")
    same = abs(r["residuum_1"] - r["residuum_2"]) <= 1e-12 * abs(r["residuum_1"])
    return [Check("Second identical evaluation gives the same residual", same,
                  f"{r['residuum_1']:.15g} / {r['residuum_2']:.15g}, max|dX| {r['max_abs_dX']:.2e}")]


def checks_svd(outdir, expect_bug):
    r = scalars(outdir / "result.txt")
    xf_k1 = None
    return [
        Check("SVD solution equals minimum-norm solution pinv(M)*C", r["max_abs_dX"] < 1e-8,
              f"{int(r['n_unknowns_differing'])} unknowns differ, max|dX| {r['max_abs_dX']:.2e}; "
              f"|MX-C| program {r['norm_resid_program']:.6g}, pinv {r['norm_resid_pinv']:.6g}",
              xfail=xf_k1),
        Check("Matrix M without empty columns", r["n_zero_columns"] == 0,
              f"{int(r['n_zero_columns'])} empty columns"),
    ]


def checks_roundtrip(outdir):
    # lineEdit_22/23 are the names for saving/loading and naturally differ
    def rows(f):
        return [l for l in f.read_text().splitlines()
                if not l.startswith(("lineEdit_22 ", "lineEdit_23 "))]
    a, b = rows(outdir / "config_reloaded.txt"), rows(outdir / "config_loaded.txt")
    diff = [x for x, y in zip(a, b) if x != y]
    bidi = scalars(outdir / "state.txt")["bidi"]
    return [Check("Saving and loading gives the same settings", not diff,
                  f"{len(diff)} fields differ, e.g. {diff[0]}" if diff else ""),
            Check("'Bidiagonal' stays off (replaced by interpolation)", bidi == 0, f"bidi = {bidi:g}")]


def checks_path_space(outdir):
    ok, detail = compare_text(outdir / "config.txt", GOLDEN / "config" / "config.txt", 0, 0)
    # lineEdit_4 contains the work path and always differs; ignore it
    a = [l for l in (outdir / "config.txt").read_text().splitlines() if not l.startswith("lineEdit_4 ")]
    b = [l for l in (GOLDEN / "config" / "config.txt").read_text().splitlines()
         if not l.startswith("lineEdit_4 ")]
    diff = [x for x, y in zip(a, b) if x != y]
    return [Check("Configuration with spaces in the path loads correctly", not diff,
                  f"{len(diff)} fields differ, e.g. {diff[0]}" if diff else "")]


def checks_subtract(workdir, outdir):
    """After subtracting A, 0.5*B (shifted) plus a constant remains."""
    sh = np.loadtxt(workdir / "truth_shifts_px.txt")
    tb = np.loadtxt(workdir / "truth_B.txt")
    out = []
    for j in (0, 3):
        d = np.loadtxt(outdir / f"residB_{j}.txt")
        x = np.arange(len(d)) - sh[j, 1]
        ref = 0.5 * np.interp(x, np.arange(len(tb)), tb[:, 1])
        inner = slice(10, -10)
        diff = d[inner, 1] - ref[inner]
        rms = float(np.sqrt(np.mean((diff - diff.mean()) ** 2)))
        out.append(Check(f"Spectrum {j} after subtracting A = 0.5 B", rms < 1e-6, f"RMS {rms:.2e}"))
    return out


def run_user_error_checks(tmp_root, timeout):
    """User errors and changed settings between steps: the scenarios of the error part of the
    test campaign (tests/tools/campaign.py). Each scenario must end as expected: with a message
    box (the error is caught), without error, or one of the two ("any"); never silently, with a
    crash or a hang."""
    sys.path.insert(0, str(HERE / "tools"))
    import campaign  # noqa: E402
    root = tmp_root / "user_errors"
    root.mkdir(parents=True, exist_ok=True)
    out = []
    for s in campaign.S:
        extra = dict(s[6]) if len(s) > 6 else {}
        r = campaign.run(*s[:6], root, min(timeout, 300), case=extra.pop("case", "slots"), extra_env=extra)
        detail = f"expected {r['expect']}, got {r['outcome']}"
        if r["detail"]:
            detail += ": " + r["detail"].replace(str(root), "@TMP@")[:160]
        out.append(Check(f"{r['name']} ({r['desc']})", not r["finding"], detail))
    return out


def run_batch_checks(tmp_root, timeout):
    """Program in batch mode: optimisation and error case."""
    out = []
    if not PROGRAM.exists():
        return [Check("Batch mode", False, f"program missing: {PROGRAM}")]
    wd = tmp_root / "batch" / "work"
    prepare("bin8_n10", wd)
    env = dict(os.environ, QT_QPA_PLATFORM="offscreen")
    t0 = time.time()
    p = subprocess.run([str(PROGRAM), "--batch", "SB2_conf"], cwd=wd, capture_output=True, text=True,
                       timeout=timeout, env=env, stdin=subprocess.DEVNULL)
    ok = p.returncode == 0 and (wd / "optval_SB2.txt").exists() and "The END." in p.stdout
    out.append(Check("--batch runs the optimisation", ok,
                     f"Exit {p.returncode}, {time.time() - t0:.1f} s"))
    p = subprocess.run([str(PROGRAM), "--batch", "gibt_es_nicht"], cwd=wd, capture_output=True, text=True,
                       timeout=60, env=env, stdin=subprocess.DEVNULL)
    out.append(Check("--batch with missing configuration ends with an error", p.returncode == 1,
                     (p.stderr.strip().splitlines() or [""])[-1][:120]))

    def run(args, cwd):
        return subprocess.run([str(PROGRAM)] + args, cwd=cwd, capture_output=True, text=True,
                              timeout=timeout, env=env, stdin=subprocess.DEVNULL)
    p = run(["--batch", "SB2_conf"], wd)
    out.append(Check("Second run without --overwrite ends with an error", p.returncode == 1,
                     (p.stderr.strip().splitlines() or [""])[-1][:120]))
    p = run(["--batch", "SB2_conf", "--overwrite"], wd)
    out.append(Check("Second run with --overwrite", p.returncode == 0, f"Exit {p.returncode}"))
    p = run(["--batch", "SB2_conf", "--overwrite", "--threads", "1"], wd)
    out.append(Check("--threads 1", p.returncode == 0 and "Cores used: 1" in p.stdout, f"Exit {p.returncode}"))
    p = run(["--batch", "SB2_conf", "--threads", "0"], wd)
    out.append(Check("--threads 0 is rejected", p.returncode == 1, f"Exit {p.returncode}"))
    wd2 = tmp_root / "batch" / "work2"
    prepare("bin8_n10", wd2)
    (wd2 / "spectram_4.txt").unlink()
    p = run(["--batch", "SB2_conf"], wd2)
    out.append(Check("Missing spectrum: exit code 1", p.returncode == 1,
                     (p.stderr.strip().splitlines() or [""])[0][:120]))
    return out


def checks_editor_orbit(work, out):
    """The Editor shows the orbital elements file of the Data tab (omega in degrees)."""
    el = [float(x) for x in (work / "orbitelements.dat").read_text().split()]
    r = scalars(out / "result.txt")
    got = [r["P"], r["e"], r["K_A"], r["K_B"], r["gamma"], r["T_peri"], r["omega_deg"] * math.pi / 180]
    ok = all(abs(a - b) <= 1e-4 * max(1, abs(b)) for a, b in zip(got, el))
    return [Check("Editor shows the elements of orbitelements.dat when opened", ok,
                  "shown " + " ".join(f"{g:.6g}" for g in got) + "; file " + " ".join(f"{e:g}" for e in el))]


# ---------------------------------------------------------------- Test cases
# name: (driver case, data set, reference files, rtol, atol, expected-value check, slow)
CASES = {
    "config": ("config", "bin4", ["config.txt"], 0, 0, None, False),
    "config_roundtrip": ("config_roundtrip", "bin4", ["config_reloaded.txt", "roundtrip_conf.sps"],
                         0, 0, lambda w, o: checks_roundtrip(o), False),
    "config_path_space": ("config", "bin4", [], 0, 0, lambda w, o: checks_path_space(o), False),
    "read_data": ("read_data", "bin4", ["scalars.txt", "C.txt", "W.txt", "RV1.txt", "RV3.txt", "M.txt"],
                  1e-12, 1e-14, None, False),
    "construct_matrix": ("construct_matrix", "bin4", ["scalars.txt", "M.txt"], 1e-12, 1e-14, None, False),
    "divide_conquer": ("divide_conquer", "bin4",
                       ["result.txt", "X.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt", "SB2_error.txt"],
                       1e-8, 1e-10, None, False),
    "divide_conquer_repeat": ("divide_conquer_repeat", "bin4", [], 0, 0,
                              lambda w, o: checks_repeat(o), False),
    "svd_check": ("svd_check", "bin4", [], 0, 0, lambda w, o: checks_svd(o, True), False),
    "svd_check_synthetic": ("svd_check", "synthetic", [], 0, 0,
                            lambda w, o: checks_svd(o, False), False),
    "orbit_rv": ("orbit_rv", "orbit", [f"rv_{k}.txt" for k in range(5)], 1e-12, 1e-10,
                 checks_orbit, False),
    "orbit_invalid": ("orbit_invalid", "bin8_n10", [], 0, 0, lambda w, o: checks_orbit_invalid(o), False),
    # atol 1e-9: residuum_physical is ~1e-10 (rounding), depends on the summation order
    "synthetic": ("synthetic", "synthetic", ["result.txt"], 1e-6, 1e-9, checks_synthetic, False),
    "interpolation": ("interpolation", "synthetic_subpixel", ["result.txt"], 1e-8, 1e-12,
                      lambda w, o: checks_interpolation(o), False),
    "optimisation": ("optimisation", "bin8_n10",
                     ["result.txt", "initval_SB2.txt", "initmat_SB2.txt", "optval_SB2.txt",
                      "optmat_SB2.txt", "SB2_1.txt", "SB2_2.txt"], 1e-8, 1e-10, None, False),
    "subtract": ("subtract", "synthetic", [], 0, 0, checks_subtract, False),
    "subtract_static": ("subtract_static", "synthetic", [], 0, 0, None, False),
    "correlation": ("correlation", "correlation", [], 0, 0,
                    lambda w, o: [Check("Cross-correlation finds +3 pixels",
                                        scalars(o / "result.txt")["shift_px"] == 3,
                                        f"{scalars(o / 'result.txt')['shift_px']:g} px")], False),
    "editor_orbit": ("editor_orbit", "synthetic_orbit", [], 0, 0, lambda w, o: checks_editor_orbit(w, o), False),
    "optimisation_bin4": ("optimisation", "bin4",
                          ["result.txt", "optval_SB2.txt", "optmat_SB2.txt"], 1e-8, 1e-10, None, True),
}


# ------------------------------------------------- modified data, error cases

def _conf_set(workdir, index, value):
    conf = workdir / "SB2_conf.sps"
    lines = conf.read_text().split("\n")
    lines[index] = value
    conf.write_text("\n".join(lines))


def mut_blank_lines(w):
    for f in (w / "spectram_0.txt", w / "velocities.txt"):
        f.write_text("# comment line\n\n" + f.read_text() + "\n\n   \n")


def mut_short_spectrum(w):
    f = w / "spectram_5.txt"
    f.write_text("\n".join(f.read_text().splitlines()[:-3]) + "\n")


def mut_missing_velocities(w):
    (w / "velocities.txt").unlink()


def mut_short_velocities(w):
    f = w / "velocities.txt"
    f.write_text("\n".join(f.read_text().splitlines()[:-1]) + "\n")


def mut_bad_range(w):
    _conf_set(w, 0, "12")
    _conf_set(w, 1, "5")


def mut_sb1(w):
    _conf_set(w, 14 + 22 + 2, "1")   # checkBox_3 (SB1)
    _conf_set(w, 14 + 22 + 3, "0")   # checkBox_4 (SB2)


def mut_perturb_rvs(w):
    """Perturb the start RVs relative to the truth, 150 iterations."""
    truth = np.loadtxt(w / "velocities.txt")
    np.savetxt(w / "truth_velocities.txt", truth, fmt="%.17g")
    rng = np.random.default_rng(1)
    start = truth + rng.normal(0, 4.0, truth.shape)   # ~0.3 pixel
    np.savetxt(w / "velocities.txt", start, fmt="%.17g", delimiter="\t")
    # Start simplex 2*step*dv = 0.5 pixel instead of the default of 6 pixels
    (w / "initialstep.dat").write_text("0.25\n0\n0\n0\n0\n0\n0\n0\n")
    _conf_set(w, 2, "150")


def _autostop(w, reinit, autostop, iters, window=0, snr=0.0):
    """RV optimisation on noisy data (S/N 100) with Auto Stop / Reinitiate."""
    mut_perturb_rvs(w)
    rng = np.random.default_rng(7)
    for p in sorted(w.glob("spectram_*.txt")):
        d = np.loadtxt(p)
        d[:, 1] += rng.normal(0, 0.01, len(d))
        np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    _conf_set(w, 2, str(iters))
    _conf_set(w, 36 + 18, "1" if reinit else "0")      # checkBox_32: Reinitiate
    lines = (w / "SB2_conf.sps").read_text().splitlines()
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    lines += [f"checkBox_23={1 if autostop else 0}", f"spinBox_11={window}", f"doubleSpinBox_16={snr}",
              "doubleSpinBox_17=0.1"]
    (w / "SB2_conf.sps").write_text("\n".join(lines) + "\n")


def mut_autostop(w):
    _autostop(w, reinit=False, autostop=True, iters=20000)


def mut_autostop_reinit(w):
    _autostop(w, reinit=True, autostop=True, iters=20000)


def mut_reinit_no_autostop(w):
    # small window: many restarts, ends only at the iteration limit
    _autostop(w, reinit=True, autostop=False, iters=1500, window=40)


def mut_autostop_snr(w):
    _autostop(w, reinit=False, autostop=True, iters=20000, snr=100.0)


def checks_reinit_restart(workdir):
    """After each reinitiation the new simplex is centred on the best point (same residuum,
    i.e. same residuum definition) and its vertices are a few sigma away, not many."""
    log = (workdir / "SB2_log.txt").read_text()
    parts = log.split("Reinitiate optimisation with current best")
    if len(parts) < 2:
        return [Check("Reinitiation happened", False, "no reinitiation in the log")]
    worst_dev, worst_chi, n = 0.0, 0.0, 0
    for k in range(1, len(parts)):
        before = "".join(parts[:k])   # everything up to this reinitiation
        after = parts[k]
        vals = [float(v) for v in re.findall(r"Residuum: ([0-9.eE+-]+)", before)]
        vals += [float(v) for v in re.findall(r"Residuum of point \d+: ([0-9.eE+-]+)", before)]
        sig = re.findall(r"sigma = ([0-9.eE+-]+)", parts[k - 1])
        pts = [float(v) for v in re.findall(r"Residuum of point \d+: ([0-9.eE+-]+)", after.split("Initial simplex completed")[0])]
        if not vals or not sig or not pts:
            continue
        best = min(vals)
        s2 = float(sig[-1]) ** 2
        worst_dev = max(worst_dev, abs(pts[0] - best) / best)
        worst_chi = max(worst_chi, max((p * p - best * best) / s2 for p in pts[1:]))
        n += 1
    return [Check("Reinitiation: new simplex starts at the best point", n > 0 and worst_dev < 1e-8,
                  f"{n} restarts, max. rel. deviation {worst_dev:.1e}"),
            Check("Reinitiation: vertices within a few sigma (chi^2 rise < 20)", n > 0 and worst_chi < 20,
                  f"max. chi^2 rise of a vertex {worst_chi:.3g}")]


def mut_ratio_const(w):
    """Constant flux ratio 1.7 (reference for the wavelength-dependent models)."""
    _conf_v2(w, doubleSpinBox_4=1.7)


def mut_ratio_planck_equal(w):
    # Planck with T_A = T_B: s(lambda) = 1, but through the per-pixel weights
    _conf_v2(w, doubleSpinBox_4=1.7, comboBox_ratio=1, doubleSpinBox_TA=7000, doubleSpinBox_TB=7000)


def mut_ratio_table_const(w):
    # table with a constant ratio: s(lambda) = 1 through table interpolation
    (w / "ratio_const.dat").write_text("# lambda ratio\n3000 3.0\n6700 3.0\n9000 3.0\n")
    _conf_v2(w, doubleSpinBox_4=1.7, comboBox_ratio=2, lineEdit_ratio="ratio_const.dat")


def mut_residual_stats(w):
    """True orbit; spectrum 4 gets an emission bump (variable line); statistics in a window
    around it."""
    import shutil as _sh
    _sh.copy(w / "truth_orbit.txt", w / "orbitelements.dat")
    p = w / "spectram_4.txt"
    d = np.loadtxt(p)
    i0 = len(d) // 2
    x = np.arange(len(d))
    d[:, 1] += 0.05 * np.exp(-0.5 * ((x - i0) / 3.0) ** 2)
    np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    lam = 10 ** d[:, 0]
    l1, l2 = lam[i0 - 12], lam[i0 + 12]
    (w / "stat_window.txt").write_text(f"{l1} {l2}\n")
    _conf_v2(w, doubleSpinBox_st1=f"{l1:.4f}", doubleSpinBox_st2=f"{l2:.4f}")


def mut_residual_stats_model(w):
    """As residual_stats, but the window comes from the residuum range of the Model tab
    ("Only from ... to"); the fields of the statistics stay at auto."""
    mut_residual_stats(w)
    lines = (w / "SB2_conf.sps").read_text().splitlines()
    lines = [l for l in lines if not l.startswith(("doubleSpinBox_st1=", "doubleSpinBox_st2="))]
    (w / "SB2_conf.sps").write_text("\n".join(lines) + "\n")
    l1, l2 = (float(v) for v in (w / "stat_window.txt").read_text().split())
    _conf_v2(w, checkBox_33=1, doubleSpinBox_14=f"{l1:.4f}", doubleSpinBox_15=f"{l2:.4f}")


def checks_residual_stats(workdir, outdir):
    st = np.loadtxt(outdir / "diffstatistic.dat")
    head = (outdir / "diffstatistic.dat").read_text().splitlines()[1]
    el = np.loadtxt(workdir / "truth_orbit.txt")
    t = np.loadtxt(workdir / "times.dat")
    ph = np.mod((t - el[5]) / el[0], 1.0)
    dph = np.abs(st[:, 1] - ph).max()
    rms = st[:, 4]
    others = np.delete(rms, 4)
    g = np.loadtxt(outdir / "plot_differences.txt", ndmin=2)
    return [Check("Residual statistics against the orbital phase (orbit mode)",
                  "phase" in head and dph < 1e-9, f"max. phase deviation {dph:.1e}"),
            Check("Variable line: the changed spectrum stands out in the window",
                  int(np.argmax(rms)) == 4 and rms[4] > 5 * np.median(others),
                  f"RMS spectrum 4 {rms[4]:.4f}, median of the others {np.median(others):.4f}"),
            Check("Statistics only in the wavelength window", bool(np.all(st[:, 2] <= 26) and np.all(st[:, 2] >= 20)),
                  f"pixels {st[:, 2].astype(int).tolist()}"),
            Check("Plot of the differences: one graph per spectrum with all its pixels",
                  len(g) == len(st) and len(set(g[:, 1])) == 1 and g[0, 1] > 100,
                  f"{len(g)} graphs, pixels {sorted(set(g[:, 1].astype(int).tolist()))}")]


def mut_fixed_rvs(w, lm=False):
    """Perturbed start RVs; RV A of spectra 0 and 3 and RV B of spectrum 5 fixed."""
    mut_perturb_rvs(w)
    # fixed RVs at their true values (anchors); the others start perturbed
    truth = np.loadtxt(w / "truth_velocities.txt")
    start = np.loadtxt(w / "velocities.txt")
    start[0, 0], start[3, 0], start[5, 1] = truth[0, 0], truth[3, 0], truth[5, 1]
    np.savetxt(w / "velocities.txt", start, fmt="%.17g", delimiter="\t")
    kv = dict(lineEdit_fixA="0, 3", lineEdit_fixB="5")
    if lm:
        kv.update(comboBox_method=1, comboBox_interp=1)
    _conf_v2(w, **kv)


def checks_fixed_rvs(workdir, outdir):
    truth = np.loadtxt(workdir / "truth_velocities.txt")
    start = np.loadtxt(workdir / "velocities.txt")
    n = truth.shape[0]
    fixed = {0, 3, n + 5}          # full index: A 0..n-1, B n..2n-1
    free = [k for k in range(2 * n) if k not in fixed]
    y = np.loadtxt(outdir / "optval_SB2.txt")
    P = np.loadtxt(outdir / "optmat_SB2.txt").reshape(len(y), -1)
    best = P[np.argmin(y)]
    full = np.concatenate([start[:, 0], start[:, 1]]).copy()
    full[free] = best
    t = np.concatenate([truth[:, 0], truth[:, 1]])
    s = np.concatenate([start[:, 0], start[:, 1]])
    log = (workdir / "SB2_log.txt").read_text()
    lm = re.findall(r"^RV_([AB])_(\d+)\t(\S+)$", log, re.M)
    if lm:                                   # Levenberg-Marquardt: named result lines
        fin = np.zeros(2 * n)
        for c, j, v in lm[-2 * n:]:
            fin[int(j) + (n if c == "B" else 0)] = float(v)
    else:                                    # Nelder-Mead: last n lines "RV_A RV_B"
        rows = [l.split() for l in log.strip().splitlines()[-n:]]
        fin = np.array([[float(a), float(b)] for a, b in rows]).T.ravel() if all(len(r) == 2 for r in rows) else None
    fixed_ok = fin is not None and all(abs(fin[k] - s[k]) < 1e-9 for k in fixed)
    e0 = np.sqrt(np.mean((s[free] - t[free]) ** 2))
    e1 = np.sqrt(np.mean((full[free] - t[free]) ** 2))
    return [Check("Simplex only over the free RVs", P.shape[1] == len(free), f"{P.shape[1]} columns, {len(free)} free"),
            Check("Fixed RVs keep their start value", fixed_ok, "log: final RVs"),
            Check("Free RVs move towards the truth", e1 < 0.3 * e0, f"RMS {e0:.2f} -> {e1:.2f} km/s")]


def mut_orbit_fit_rvs(w):
    """RV mode: RVs from the true orbit plus noise (0.5 km/s) in velocities.txt, perturbed
    start orbit in orbitelements.dat; the Orbit fit dialog fits them (gamma fixed)."""
    sys.path.insert(0, str(HERE / "tools"))
    from make_synthetic import kepler_rv
    el = np.loadtxt(w / "truth_orbit.txt")
    # times covering the orbit (those of the data set sample only two phases); the spectra
    # are not used, only the RVs
    t = 2448150.0 + 2.13 * np.arange(10) + 0.3 * np.sin(np.arange(10))
    np.savetxt(w / "times.dat", t, fmt="%.10f")
    va, vb = kepler_rv(el, t)
    rng = np.random.default_rng(5)
    np.savetxt(w / "velocities.txt", np.column_stack([va + rng.normal(0, 0.5, len(t)),
                                                      vb + rng.normal(0, 0.5, len(t))]), fmt="%.10g", delimiter="\t")
    start = el.copy()
    start[0] *= 1.002; start[1] = 0.25; start[2] *= 0.9; start[3] *= 1.1; start[5] += 0.5; start[6] -= 0.2
    np.savetxt(w / "orbitelements.dat", start, fmt="%.14g")
    cb = 14 + 22
    _conf_set(w, cb + 1, "1")      # RVs from velocities.txt
    _conf_set(w, cb + 15, "0")     # orbit mode off


def checks_orbit_fit_rvs(workdir, outdir):
    truth = np.loadtxt(workdir / "truth_orbit.txt")
    txt = (outdir / "orbitfit_result.txt").read_text()
    vals = {m.group(1): (float(m.group(2)), m.group(3)) for m in
            re.finditer(r"^(P|e|K_A|K_B|gamma|T_peri|omega_A)\t(\S+)\t(\S+)$", txt, re.M)}
    names = ["P", "e", "K_A", "K_B", "gamma", "T_peri", "omega_A"]
    zs, ok = [], len(vals) == 7
    for i, nm in enumerate(names):
        if not ok or vals[nm][1] == "fixed":
            continue
        tv = truth[i]
        if nm == "T_peri":
            tv += round((vals[nm][0] - truth[5]) / truth[0]) * truth[0]
        z = (vals[nm][0] - tv) / float(vals[nm][1])
        zs.append(f"{nm} {z:+.1f}")
        ok = ok and abs(z) < 4
    el = np.loadtxt(outdir / "orbitelements.dat")
    written = ok and abs(el[1] - vals["e"][0]) < 1e-10 and abs(el[2] - vals["K_A"][0]) < 1e-9
    rms = re.search(r"RMS A (\S+) km/s, RMS B (\S+) km/s", txt)
    off = re.search(r"offset of the RVs of B: (\S+) \+- (\S+) km/s", txt)
    return [Check("Orbit fit to the RVs of the main window: elements within 4 sigma of the truth", ok, ", ".join(zs)),
            Check("Orbit fit: RMS of the residuals at the noise level (0.5 km/s)",
                  bool(rms) and 0.15 < float(rms.group(1)) < 0.9 and 0.15 < float(rms.group(2)) < 0.9,
                  rms.group(0) if rms else "no RMS"),
            Check("Orbit fit: result written to orbitelements.dat", written, ""),
            Check("Orbit fit: own zero point of B (RV mode without static lines) consistent with 0",
                  bool(off) and abs(float(off.group(1))) < 4 * float(off.group(2)), off.group(0) if off else "no offset line")]


def mut_error_mc(w):
    """Orbit fit (LM, cubic) on the synthetic orbit with noise (S/N 200), then errors from the
    curvature and from 20 Monte Carlo realisations."""
    mut_orbit_fit_lm_cubic(w)
    rng = np.random.default_rng(17)
    for p in sorted(w.glob("spectram_*.txt")):
        d = np.loadtxt(p)
        d[:, 1] += rng.normal(0, 0.005, len(d))
        np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    _conf_v2(w, spinBox_mc=20)


def _error_rows(path):
    rows = {}
    for line in path.read_text().splitlines():
        parts = line.split("\t")
        if len(parts) == 4 and not line.startswith("#"):
            rows[parts[0]] = [float(x) for x in parts[1:]]
    return rows


def checks_error_mc(workdir, outdir):
    mc, cu = _error_rows(outdir / "errors.dat"), _error_rows(outdir / "errors_curvature.dat")
    txt = (outdir / "errors.dat").read_text()
    m = re.search(r"Monte Carlo: (\d+) of (\d+) realisations converged", txt)
    out = [Check("Monte Carlo: realisations converged", bool(m) and int(m.group(1)) >= 18,
                 m.group(0) if m else "no Monte Carlo header"),
           Check("Monte Carlo: free elements in the error file", set(mc) == {"e", "K_A", "K_B", "omega"} and set(cu) == set(mc),
                 ", ".join(sorted(mc)))]
    ratios = []
    ok = bool(mc) and set(cu) == set(mc)
    for n in sorted(mc):
        best, err, mean = mc[n]
        r = err / cu[n][1] if n in cu and cu[n][1] > 0 else float("nan")
        ratios.append(f"{n} {r:.2f}")
        ok = ok and math.isfinite(err) and err > 0 and abs(mean - best) < 3 * err and 0.5 < r < 3.0
    out.append(Check("Monte Carlo errors: same order as the curvature errors, mean near the best value", ok,
                     "MC/curvature: " + ", ".join(ratios)))
    return out


def mut_ratio_const_static(w):
    mut_static(w)
    _conf_v2(w, doubleSpinBox_4=1.7)


def mut_ratio_planck_equal_static(w):
    mut_static(w)
    _conf_v2(w, doubleSpinBox_4=1.7, comboBox_ratio=1, doubleSpinBox_TA=7000, doubleSpinBox_TB=7000)


def mut_ratio_const_sb1(w):
    mut_synth_sb1(w)
    _conf_v2(w, doubleSpinBox_4=1.7)


def mut_ratio_planck_equal_sb1(w):
    mut_synth_sb1(w)
    _conf_v2(w, doubleSpinBox_4=1.7, comboBox_ratio=1, doubleSpinBox_TA=7000, doubleSpinBox_TB=7000)


def mut_ratio_table_missing(w):
    _conf_v2(w, comboBox_ratio=2, lineEdit_ratio="does_not_exist.dat")


def _eclipse(w, mode, snr=0.0, **kw):
    """Synthetic eclipsing binary over 4500-6500 A with a Planck flux ratio
    (tools/make_eclipse.py); configuration from the data set, RVs and k_j (edit.dat) true."""
    sys.path.insert(0, str(HERE / "tools"))
    import make_eclipse
    for f in w.glob("spectram_*.txt"):
        f.unlink()
    info = make_eclipse.make(w, w / "SB2_conf.sps", snr=snr, **kw)
    if mode == "table":
        # the same ratio as a table (41 points), shape relative to lambda_ref
        lam = np.linspace(info["l1"] - 50, info["l2"] + 50, 41)
        r = info["r_ref"] * make_eclipse.planck_ratio(lam, info["ta"], info["tb"]) \
            / make_eclipse.planck_ratio(info["lref"], info["ta"], info["tb"])
        np.savetxt(w / "ratio_table.dat", np.column_stack([lam, r]), fmt="%.10g")
        _conf_v2(w, comboBox_ratio=2, lineEdit_ratio="ratio_table.dat")
    elif mode == "constant":
        _conf_v2(w, comboBox_ratio=0)
    return info


def mut_ratio_plotter(w):
    """No eclipses (continuum levels of the components undetermined), S/N 200, Planck; continuum
    points of A at wavelengths where the true A is flat (cont_lambda.txt)."""
    _eclipse(w, "planck", snr=200.0, eps_a=0.0, eps_b=0.0)
    t = np.loadtxt(w / "truth_A.txt")
    cand = np.linspace(4650, 6350, 60)
    flat = lambda x: abs(np.interp(x, t[:, 0], t[:, 1]) - 1) < 2e-4 \
        and max(abs(np.interp(x + d, t[:, 0], t[:, 1]) - 1) for d in (-3, 3)) < 5e-4
    np.savetxt(w / "cont_lambda.txt", [x for x in cand if flat(x)], fmt="%.3f")


def checks_ratio_plotter(workdir, outdir):
    sys.path.insert(0, str(HERE / "tools"))
    import eval_eclipse
    before = {c: eval_eclipse.compare(outdir / f, workdir / t)
              for c, f, t in (("A", "SB2_1.txt", "truth_A.txt"), ("B", "SB2_2.txt", "truth_B.txt"))}
    after = {c: eval_eclipse.compare(outdir / f, workdir / t)
             for c, f, t in (("A", "A_norm.txt", "truth_A.txt"), ("B", "B_norm.txt", "truth_B.txt"))}
    det = ", ".join(f"{c}: RMS {before[c]['rms']:.3f} -> {after[c]['rms']:.4f}" for c in "AB")
    return [Check("Plotter, flux-ratio model: both components on the continuum, errors at the noise level",
                  all(after[c]["rms"] < 0.01 and abs(after[c]["mean"]) < 2e-3 for c in "AB"), det),
            Check("Before the correction the continua were off (undetermined without eclipses)",
                  all(before[c]["rms"] > 0.05 for c in "AB"), det)]


ECLIPSE_OPT = dict(l1=5000.0, l2=6500.0, dv=40.0, phases=[0.0, 0.015, 0.03, 0.5, 0.53, 0.15, 0.3, 0.65, 0.8, 0.9])


def mut_ratio_eclipse_optratios(w, mode="planck"):
    """Flux-ratio optimisation on eclipse spectra (S/N 200): start k_j = r_ref for all
    spectra (no knowledge of the eclipses), Auto Stop."""
    _eclipse(w, mode, snr=200.0, **ECLIPSE_OPT)
    (w / "edit.dat").unlink()
    _conf_set(w, 2, "20000")
    _conf_set(w, 36 + 8, "1")                          # New
    _conf_v2(w, checkBox_23=1, spinBox_11=0, doubleSpinBox_16=200, doubleSpinBox_17=0.1)


def checks_ratio_optratios(workdir, outdir):
    k = np.loadtxt(workdir / "truth_k.txt")[:, 0]
    y = np.loadtxt(outdir / "optval_flux.dat")
    P = np.loadtxt(outdir / "optmat_flux.dat").reshape(len(y), -1)
    best = P[np.argmin(y)]
    pattern = float(np.sqrt(np.mean((best / best.mean() - k / k.mean()) ** 2)))
    factor = float(best.mean() / k.mean())
    det = f"pattern RMS {pattern:.4f}, common factor {factor:.3f}; k {np.round(best, 3).tolist()}"
    return [Check("Flux ratios k_j with the Planck shape: eclipse pattern recovered", pattern < 0.02, det),
            Check("Flux ratios k_j: common factor within 10 %", abs(factor - 1) < 0.1, det)]


def mut_ratio_eclipse_planck(w):
    _eclipse(w, "planck")


def mut_ratio_eclipse_table(w):
    _eclipse(w, "table")


def mut_ratio_eclipse_constant(w):
    _eclipse(w, "constant")


def checks_ratio_eclipse(name, workdir, outdir):
    sys.path.insert(0, str(HERE / "tools"))
    import eval_eclipse
    r = eval_eclipse.evaluate(workdir, outdir)
    det = f"RMS A {r['A']['rms']:.1e}, B {r['B']['rms']:.1e} (lines only {r['A']['rms_lines']:.1e}, " \
          f"{r['B']['rms_lines']:.1e})"
    if name == "ratio_eclipse_constant":
        # the old model: one ratio per spectrum at lambda_ref, the same at all wavelengths
        return [Check("Constant ratio over 4500-6500 A: systematic errors in the components",
                      r["A"]["rms"] > 3e-3 and r["B"]["rms"] > 3e-3, det)]
    tol = 5e-4 if name == "ratio_eclipse_planck" else 1e-3
    return [Check("Flux ratio k_j s(lambda): component spectra match the truth (noise-free)",
                  r["A"]["rms"] < tol and r["B"]["rms"] < tol, det)]


def mut_flux_templates(w):
    """Synthetic binary over 4500-6500 A without eclipses (Planck flux ratio, r_ref 1.5,
    S/N 300), disentangled with the wrong ratio 1.0 (same Planck shape); the true component
    spectra serve as the synthetic spectra."""
    _eclipse(w, "planck", snr=300.0, eps_a=0.0, eps_b=0.0)
    (w / "edit.dat").unlink()
    _conf_set(w, 9, "1.0")


def checks_flux_templates(workdir, outdir):
    vals = {}
    for line in (outdir / "fluxratio_templates.txt").read_text().splitlines():
        if line and not line.startswith("#"):
            p = line.split("\t")
            vals[p[0]] = [float(x) for x in p[1:]]
    k, ek, ekf = vals["k"]
    s = vals["separate_light_fraction_A"][0] + vals["separate_light_fraction_B"][0]
    out = {}
    for comp, f in (("A", "SB2_1_renorm.txt"), ("B", "SB2_2_renorm.txt")):
        r = np.loadtxt(outdir / f)
        t = np.loadtxt(workdir / f"truth_{comp}.txt")
        tv = np.interp(r[:, 0], t[:, 0], t[:, 1])
        sel = slice(20, -20)
        out[comp] = float(np.sqrt(np.mean((r[sel, 1] - tv[sel]) ** 2)))
    field = float(next(l.split()[1] for l in (outdir / "result.txt").read_text().splitlines() if l.startswith("field_ratio")))
    det = f"k {k:.4f} +- {ek:.1e} (jackknife; formal {ekf:.1e}); separate light fractions sum {s:.4f}; shifts {vals['shift_A'][0]:.2f}, " \
          f"{vals['shift_B'][0]:.2f} km/s; renormalised RMS A {out['A']:.1e}, B {out['B']:.1e}; field {field}"
    # the formal error ignores the remaining waviness of the continua; the jackknife error
    # must cover the deviation
    return [Check("Flux ratio from synthetic spectra: true ratio 1.5 found within 0.5 % (disentangled with 1.0)",
                  abs(k - 1.5) < 0.0075, det),
            Check("Jackknife error of the ratio covers the deviation (formal error too small)",
                  abs(k - 1.5) < 3 * ek and ek > ekf, det),
            Check("Separate fits consistent (light fractions add up to 1), shifts about 0",
                  abs(s - 1) < 0.005 and abs(vals["shift_A"][0]) < 1.0 and abs(vals["shift_B"][0]) < 0.3, det),
            Check("Renormalised components match the true spectra", out["A"] < 0.004 and out["B"] < 0.006, det),
            Check("Ratio taken over into the main window", abs(field - k) < 1e-4, det)]


def mut_barycorr(w):
    """Start RVs from the file switched on (the tool corrects them as well)."""
    _conf_set(w, 37, "1")


def checks_barycorr(workdir, outdir):
    main = dict(l.split("\t") for l in (outdir / "main.txt").read_text().splitlines())
    tab = np.loadtxt(outdir / "bc_spectram_barycorr.txt", ndmin=2)
    t = np.loadtxt(workdir / "times.dat")
    c = 299792.458
    dev = 0.0
    for j in range(len(t)):
        a = np.loadtxt(workdir / f"spectram_{j}.txt")
        b = np.loadtxt(outdir / f"bc_spectram_{j}.txt")
        dev = max(dev, float(np.max(np.abs(b[:, 0] - a[:, 0] - np.log10(1 + tab[j, 3] / c)))),
                  float(np.max(np.abs(b[:, 1] - a[:, 1]))))
    rv0 = np.loadtxt(workdir / "velocities.txt", ndmin=2)
    rv1 = np.loadtxt(outdir / "velocities_bc.txt", ndmin=2)
    drv = float(np.max(np.abs(rv1 - rv0 - tab[:, 3:4])))
    det = f"BC {tab[:, 3].min():.3f} .. {tab[:, 3].max():.3f} km/s; max deviation {dev:.1e}; RVs {drv:.1e}; main {main}"
    return [Check("Barycentric correction: one value per spectrum, |BC| < 31 km/s, JD (UTC) = time",
                  len(tab) == len(t) and bool(np.all(np.abs(tab[:, 3]) < 31)) and bool(np.allclose(tab[:, 2], t, atol=1e-9)), det),
            Check("Corrected spectra: log10(lambda) + log10(1 + BC/c), flux unchanged", dev < 1e-12, det),
            Check("Start RVs corrected: RV + BC", drv < 1e-9, det),
            Check("Corrected files used in the main window (prefix, ASCII, RV file)",
                  main["prefix"] == "bc_spectram_" and main["rvfile"] == "velocities_bc.txt" and main["format"] == "1", det)]


# times near the transit of iota Tri at Tenerife, with the altitude from astropy (AltAz without
# refraction): airmass of the tool against Pickering (2002) of these altitudes
BC_NIGHT = [(2448152.595833, 74.000), (2448163.616667, 88.035), (2448173.639583, 73.968), (2448184.508333, 73.958),
            (2448194.531250, 88.033), (2448204.554167, 74.210), (2448215.425000, 74.365), (2448225.447917, 88.016),
            (2448236.466667, 74.249), (2448246.339583, 74.124)]


def mut_barycorr_night(w):
    np.savetxt(w / "times.dat", [t for t, _ in BC_NIGHT], fmt="%.6f")


def checks_barycorr_file(name, workdir, outdir):
    """BC file only: same values as the table written with the corrected spectra (barycorr case),
    spectra unchanged, file and frame set in the Data tab; then Read data uses it."""
    main = dict(l.split("\t") for l in (outdir / "main.txt").read_text().splitlines())
    tab = np.loadtxt(outdir / "spectram_barycorr.txt", ndmin=2)
    t = np.loadtxt(workdir / "times.dat")
    corrected = name.endswith("_corrected")
    untouched = all(not (workdir / f"bc_spectram_{j}.txt").exists() for j in range(len(t)))
    am = np.loadtxt(outdir / "spectram_airmass.txt", ndmin=1) if (outdir / "spectram_airmass.txt").exists() else np.array([])
    alt = np.array([h for _, h in BC_NIGHT])
    ref = 1 / np.sin(np.radians(alt + 244 / (165 + 47 * alt ** 1.1)))
    det = f"BC {tab[:, 3].min():.3f} .. {tab[:, 3].max():.3f} km/s; main {main}"
    return [Check("BC file: one value per spectrum, |BC| < 31 km/s, JD (UTC) = time; no spectra written",
                  len(tab) == len(t) and bool(np.all(np.abs(tab[:, 3]) < 31)) and bool(np.allclose(tab[:, 2], t, atol=1e-9))
                  and untouched, det),
            Check("BC file and frame set in the Data tab (" + ("corrected" if corrected else "observatory frame") + ")",
                  main["bcfile"] == "spectram_barycorr.txt" and main["frame"] == ("1" if corrected else "0")
                  and main["prefix"] == "spectram_", det),
            Check("Airmass file (Pickering 2002, against astropy altitudes within 0.001) set as start strength of the static lines",
                  len(am) == len(t) and float(np.max(np.abs(am - ref))) < 0.001 and main.get("tellw") == "spectram_airmass.txt",
                  (f"airmass {am.min():.4f} .. {am.max():.4f}, max deviation {np.max(np.abs(am - ref)):.1e}") if len(am) == len(t) else "missing")]


# ---- Check of the telluric lines (Tools -> Barycentric correction) -----------------------------
# 10 spectra 6860-6930 A (O2 B band, 1 km/s per pixel, S/N 300) at the times BC_NIGHT; telluric
# lines at rest in the observatory frame, two stellar components; spectra 3 and 7 with a wrong
# wavelength scale (whole spectrum shifted by +20 and -5 km/s, as by a wrong correction).
TELL_BAD = {3: 20.0, 7: -5.0}
TELL_ENV = {"tellcheck": {"SPECTEST_RADEC": "02 12 22.28;+30 18 11.1"},
            "tellcheck_corrected": {"SPECTEST_RADEC": "02 12 22.28;+30 18 11.1", "SPECTEST_BC_CORRECTED": "1"},
            "tellcheck_reject": {"SPECTEST_RADEC": "02 12 22.28;+30 18 11.1", "SPECTEST_BC_CORRECTED": "1",
                                 "SPECTEST_REJECT": "1"}}


def _bc_astropy(times):
    """Barycentric correction (km/s) of iota Tri at STELLA for JD (UTC), with astropy."""
    from astropy.utils import iers
    iers.conf.auto_download = False
    iers.conf.auto_max_age = None
    import warnings
    warnings.filterwarnings("ignore")
    from astropy.coordinates import SkyCoord, EarthLocation
    from astropy.time import Time
    import astropy.units as u
    site = EarthLocation(lat=28.3010 * u.deg, lon=-16.5100 * u.deg, height=2390 * u.m)
    star = SkyCoord("02 12 22.28 +30 18 11.1", unit=(u.hourangle, u.deg))
    return star.radial_velocity_correction(obstime=Time(times, format="jd", scale="utc"), location=site).to(u.km / u.s).value


def _tell_dataset(w, corrected):
    c = 299792.458
    t = np.array([x for x, _ in BC_NIGHT])
    bc = _bc_astropy(t) if corrected else np.zeros(len(t))
    dlog = np.log10(1 + 1.0 / c)
    lam = 10 ** (np.log10(6860) + dlog * np.arange(int(np.log10(6930 / 6860) / dlog) + 1))
    rng = np.random.default_rng(11)
    LT = [(x, d) for x, d in zip(np.sort(rng.uniform(6866, 6925, 26)), rng.uniform(0.2, 0.6, 26))]
    LS = [(x, d) for x, d in zip(rng.uniform(6865, 6925, 8), rng.uniform(0.1, 0.3, 8))]

    def prof(x, lines, sig):
        y = np.zeros_like(x)
        for x0, d in lines:
            y -= d * np.exp(-0.5 * ((x - x0) / sig) ** 2)
        return y
    v = rng.uniform(-60, 60, (len(t), 2))
    for f in w.glob("spectram_*.txt"):
        f.unlink()
    for j in range(len(t)):
        x = lam / (1 + TELL_BAD.get(j, 0.0) / c)          # wrong wavelength scale: all lines shifted
        tel = prof(x / (1 + bc[j] / c), LT, 0.06)          # corrected: tellurics moved by BC
        star = 0.5 * prof(x / (1 + v[j, 0] / c), LS, 0.3) + 0.5 * prof(x / (1 + v[j, 1] / c), LS[::-1], 0.35)
        fl = (1 + star) * (1 + tel) + rng.normal(0, 1 / 300, len(lam))
        np.savetxt(w / f"spectram_{j}.txt", np.column_stack([np.log10(lam), fl]), fmt=["%.12f", "%.10g"], delimiter="\t")
    np.savetxt(w / "times.dat", t, fmt="%.6f")
    np.savetxt(w / "velocities.txt", v, fmt="%.6f", delimiter="\t")
    np.savetxt(w / "snr.dat", 300 + np.arange(len(t)), fmt="%d")
    np.savetxt(w / "truth_bc.txt", bc, fmt="%.6f")
    (w / "orig_md5.txt").write_text("\n".join(hashlib.md5((w / f"spectram_{j}.txt").read_bytes()).hexdigest()
                                             for j in range(len(t))) + "\n")
    _conf_set(w, 1, str(len(t) - 1))
    _conf_v2(w, lineEdit_29="snr.dat")


def checks_tellcheck(name, workdir, outdir):
    rows = [l.split("\t") for l in (outdir / "tellcheck.txt").read_text().splitlines() if not l.startswith("#")]
    dev = np.array([float(r[3]) for r in rows])
    rej = [int(r[4]) for r in rows]
    exp = np.array([TELL_BAD.get(j, 0.0) for j in range(len(rows))])
    err = np.abs(dev - exp)
    txt = "deviations " + " ".join(f"{d:.2f}" for d in dev)
    out = [Check(("Corrected spectra" if "corrected" in name or "reject" in name else "Observatory frame")
                 + ": deviation of the telluric lines = wrong shift of spectra 3 and 7 (+20, -5 km/s), others 0 (within 0.3 km/s)",
                 len(rows) == 10 and float(err.max()) < 0.3, txt),
           Check("Spectra 3 and 7 marked for rejection (limit 2 km/s), no other", rej == [int(j in TELL_BAD) for j in range(10)],
                 f"marked {[j for j, r in enumerate(rej) if r]}")]
    if name == "tellcheck_reject":
        md5 = (workdir / "orig_md5.txt").read_text().split()
        keep = [j for j in range(10) if j not in TELL_BAD]
        now = [hashlib.md5((workdir / f"spectram_{k}.txt").read_bytes()).hexdigest() if (workdir / f"spectram_{k}.txt").exists() else ""
               for k in range(10)]
        renum = now[:len(keep)] == [md5[j] for j in keep] and not any(now[len(keep):])
        rj = all((workdir / f"spectram_bc_rejected_{j}.txt").exists()
                 and hashlib.md5((workdir / f"spectram_bc_rejected_{j}.txt").read_bytes()).hexdigest() == md5[j] for j in TELL_BAD)
        t0 = np.loadtxt(workdir / "times.dat.before_bc_reject")
        t1 = np.loadtxt(workdir / "times.dat")
        v1 = np.loadtxt(workdir / "velocities.txt")
        s1 = np.loadtxt(workdir / "snr.dat")
        main = dict(l.split("\t") for l in (outdir / "main.txt").read_text().splitlines())
        out += [Check("Rejected spectra renamed to spectram_bc_rejected_<index>.txt, the others numbered 0..7 in order",
                      renum and rj, f"files now: {sum(bool(x) for x in now)}"),
                Check("Times, RV and S/N file without the rejected lines (copies *.before_bc_reject), last index 7, log",
                      np.allclose(t1, t0[keep]) and len(v1) == 8 and np.allclose(s1, (300 + np.arange(10))[keep])
                      and main.get("last") == "7" and (workdir / "bc_rejected.txt").exists(),
                      f"times {len(t1)}, RVs {len(v1)}, S/N {len(s1)}, last {main.get('last')}")]
    return out


def mut_orbit_fit_reinit(w):
    mut_orbit_fit(w)
    _conf_set(w, 2, "3000")
    _conf_set(w, 36 + 18, "1")                       # checkBox_32: Reinitiate
    _conf_v2(w, checkBox_23=1, spinBox_11=40, doubleSpinBox_17=0.1)


def checks_autostop(name, workdir, outdir):
    log = (outdir / "stdout.log").read_text() + (workdir / "SB2_log.txt").read_text()
    iters = len(re.findall(r"^Iteration: \d+", (outdir / "stdout.log").read_text(), re.M))
    stops = (outdir / "stdout.log").read_text().count("Optimisation stopped (Auto Stop)")
    reinits = (outdir / "stdout.log").read_text().count("Reinitiate optimisation")
    conv = [l for l in log.splitlines() if l.startswith("Converged:")]
    out = []
    if name == "reinit_no_autostop":
        out.append(Check("Reinitiate without Auto Stop ends at the iteration limit (across all restarts)",
                         iters <= 1501 and reinits >= 2 and "Maximum number of iterations reached" in log and stops == 0,
                         f"{iters} iterations, {reinits} restarts"))
        return out + checks_reinit_restart(workdir)
    out.append(Check("Auto Stop stops before the iteration limit", stops == 1 and iters < 20000,
                     f"{iters} iterations, {reinits} restarts; {conv[-1] if conv else ''}"))
    if name == "autostop_reinit":
        out.append(Check("With Reinitiate: restart, stop after comparison with the previous run",
                         reinits >= 1 and "Gain of this run against the previous one" in log, f"{reinits} restarts"))
        out += checks_reinit_restart(workdir)
    if name == "autostop_snr":
        out.append(Check("sigma from the given S/N", bool(conv) and "sigma = 0.01, from S/N" in conv[-1],
                         conv[-1] if conv else ""))
    # convergence data: sigma^2 frozen per line (4th column), for the gain per iteration
    od = np.atleast_2d(np.loadtxt(workdir / "optdata.dat"))
    s2 = od[:, 3] if od.shape[1] >= 4 else np.array([np.nan])
    if name == "autostop_snr":
        ok = bool(np.allclose(s2, 1e-4, rtol=1e-12))
    else:
        # estimated: positive, at most the start residuum^2 per bin scale, falling with the residuum
        ok = bool(np.all(np.isfinite(s2)) and np.all(s2 > 0) and s2[-1] <= s2[0] * (1 + 1e-9))
    out.append(Check("Convergence data with sigma^2 per line (fixed gain per iteration)", ok,
                     f"{od.shape[1]} columns, sigma^2 {s2[0]:.4g} ... {s2[-1]:.4g}"))
    out += checks_converges(workdir, outdir)
    return out


def rv_error(rv, truth):
    """RMS distance to the truth after subtracting a constant offset per component."""
    d = rv - truth
    d -= d.mean(axis=0)
    return float(np.sqrt(np.mean(d ** 2)))


def checks_converges(workdir, outdir):
    truth = np.loadtxt(workdir / "truth_velocities.txt")
    start = np.loadtxt(workdir / "velocities.txt")
    y = np.loadtxt(outdir / "optval_SB2.txt")
    P = np.loadtxt(outdir / "optmat_SB2.txt").reshape(len(y), -1)
    best = P[np.argmin(y)]
    n = truth.shape[0]
    rv = np.column_stack([best[:n], best[n:]])
    e0, e1 = rv_error(start, truth), rv_error(rv, truth)
    return [Check("Optimisation moves the RVs towards the truth", e1 < 0.5 * e0,
                  f"RMS distance {e0:.2f} -> {e1:.2f} km/s, residual {y.min():.4g}")]


def mut_orbit_iterations(w):
    _conf_set(w, 2, "150")


def checks_orbit_fit(workdir, outdir):
    truth = np.loadtxt(workdir / "truth_orbit.txt")
    start = np.loadtxt(workdir / "orbitelements.dat.start")
    y = np.loadtxt(outdir / "optval_SB2.txt")
    P = np.loadtxt(outdir / "optmat_SB2.txt").reshape(len(y), -1)
    best = P[np.argmin(y)]          # free elements: e, KA, KB, omega
    # Tolerance: on this coarse grid (dv = 13.3 km/s) linear interpolation
    # distorts by about 0.1 pixel; K up to 2 km/s (0.15 px).
    names, idx, tol = ["e", "K_A", "K_B", "omega"], [1, 2, 3, 6], [0.03, 2.0, 2.0, 0.05]
    out = []
    for k, (n, i, tl) in enumerate(zip(names, idx, tol)):
        err, err0 = abs(best[k] - truth[i]), abs(start[i] - truth[i])
        out.append(Check(f"Orbit fit finds {n}", err < tl and err < 0.5 * err0,
                         f"start {start[i]:g}, result {best[k]:.4f}, true {truth[i]:g}"))
    return out


def mut_orbit_fit(w):
    import shutil as _sh
    _sh.copy(w / "orbitelements.dat", w / "orbitelements.dat.start")
    mut_orbit_iterations(w)


def _conf_v2(w, **kv):
    """Set name=value settings in the v2 part of the configuration."""
    lines = (w / "SB2_conf.sps").read_text().splitlines()
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    lines += [f"{k}={v}" for k, v in kv.items()]
    (w / "SB2_conf.sps").write_text("\n".join(lines) + "\n")


def mut_orbit_fit_lm(w):
    mut_orbit_fit(w)
    _conf_v2(w, comboBox_method=1)


def mut_orbit_fit_lm_cubic(w):
    mut_orbit_fit(w)
    _conf_v2(w, comboBox_method=1, comboBox_interp=1)


def mut_orbit_fit_cubic(w):
    mut_orbit_fit(w)
    _conf_v2(w, comboBox_interp=1)


def mut_perturb_rvs_lm(w):
    mut_perturb_rvs(w)
    _conf_v2(w, comboBox_method=1, comboBox_interp=1)


def mut_lm_reinit(w):
    mut_perturb_rvs_lm(w)
    _conf_set(w, 36 + 18, "1")                        # checkBox_32: Reinitiate


def checks_lm_reinit(workdir, outdir):
    out = (outdir / "stdout.log").read_text()
    log = (workdir / "SB2_log.txt").read_text()
    reinits = out.count("Reinitiate optimisation with current best RVs (Levenberg-Marquardt)")
    stop = re.findall(r"^Levenberg-Marquardt run \d+: gain .*$", log, re.M)
    return [Check("Levenberg-Marquardt with Reinitiate: restart from the best point, "
                  "stop after a run without improvement",
                  reinits >= 1 and bool(stop) and stop[-1].endswith("(no improvement: stop).")
                  and len(stop) == reinits,
                  f"{reinits} restarts; {stop[-1] if stop else 'no comparison in the log'}")]


def mut_continue_switch(w):
    """Run 1: Nelder-Mead, New, 150 iterations (runs 2 and 3 in checks_continue_switch)."""
    mut_perturb_rvs(w)


def checks_continue_switch(case, workdir, root, timeout, orbit=False):
    """Continue resumes from the best values whatever the method and the start values in the
    inputs: NM (New) -> LM (Continue, start RVs in the input file changed) -> NM (Continue,
    simplex file missing: new simplex around the best)."""
    def best_state():
        t = (workdir / ("beststate_orbit.dat" if orbit else "beststate_rv.dat")).read_text().splitlines()
        return float(t[0].split("residuum ")[1].split()[0]), np.loadtxt(t[1:])
    s1, rv1 = best_state()
    y1 = np.loadtxt(root / "out" / "optval_SB2.txt").min()
    checks = [Check("Best state written with the best value of the run", abs(s1 - y1) <= 1e-9 * y1,
                    f"{s1:.10g} / simplex {y1:.10g}")]
    # run 2: LM, Continue; the input start RVs are far off and must be ignored
    if orbit:
        el = np.loadtxt(workdir / "orbitelements.dat")
        el[2:4] += 15.0                              # K_A, K_B far off
        np.savetxt(workdir / "orbitelements.dat", el, fmt="%.17g")
    else:
        truth = np.loadtxt(workdir / "truth_velocities.txt")
        np.savetxt(workdir / "velocities.txt", truth + 25.0, fmt="%.17g", delimiter="\t")
    _conf_set(workdir, 36 + 8, "0")                  # New off
    _conf_set(workdir, 36 + 10, "1")                 # Continue
    _conf_v2(workdir, comboBox_method=1)
    run_adapter(case, workdir, root / "out2", timeout)
    log = (workdir / "SB2_log.txt").read_text()
    od = np.atleast_2d(np.loadtxt(workdir / "optdata.dat"))
    s2, _ = best_state()
    checks.append(Check("NM -> LM with Continue starts from the best values of the NM run",
                        "Start values from the best state" in log and od[0, 2] <= s1 * (1 + 1e-9) and s2 <= s1,
                        f"LM start {od[0, 2]:.10g}, NM best {s1:.10g}, LM best {s2:.10g}"))
    # run 3: NM, Continue, simplex file removed
    (workdir / "optval_SB2.txt").unlink()
    _conf_set(workdir, 2, "50")
    _conf_v2(workdir, comboBox_method=0)
    run_adapter(case, workdir, root / "out3", timeout)
    out = (root / "out3" / "stdout.log").read_text()
    od = np.atleast_2d(np.loadtxt(workdir / "optdata.dat"))
    s3, _ = best_state()
    checks.append(Check("LM -> NM with Continue without a matching simplex: new simplex around the best values",
                        f"Continue: new simplex around the current best {'orbit' if orbit else 'RVs'}" in out
                        and od[0, 2] <= s2 * (1 + 1e-9) and s3 <= s2,
                        f"first vertex {od[0, 2]:.10g}, LM best {s2:.10g}, NM best {s3:.10g}"))
    return checks


def mut_custom_names(w):
    """Own file names (Output tab: best state, convergence data; Data tab: orbital elements).
    Files with the default names that belong to another setup lie in the same folder and must
    not be touched or used."""
    mut_orbit_fit(w)
    (w / "orbitelements.dat").rename(w / "myorbit.dat")
    decoy = "# Spectangular best state (P e K_A K_B gamma T_peri omega) residuum 1e-9\n" + \
            "\n".join(["3.1", "0.5", "11", "22", "-7", "0.3", "1.0"]) + "\n"
    (w / "beststate_orbit.dat").write_text(decoy)
    _conf_v2(w, lineEdit_orbit="myorbit.dat", lineEdit_best="state_A.dat", lineEdit_optdata="conv_A.dat")


def checks_custom_names(case, workdir, root, timeout):
    start = np.loadtxt(workdir / "myorbit.dat.start") if (workdir / "myorbit.dat.start").exists() \
        else np.loadtxt(workdir / "orbitelements.dat.start")
    decoy = (workdir / "beststate_orbit.dat").read_text()
    checks = []
    st = workdir / "state_A_orbit.dat"
    ok_state = st.exists() and st.read_text().startswith("# Spectangular best state (P e")
    s1 = float(st.read_text().splitlines()[0].split("residuum ")[1].split()[0]) if ok_state else float("nan")
    od = np.atleast_2d(np.loadtxt(workdir / "conv_A.dat")) if (workdir / "conv_A.dat").exists() else None
    checks.append(Check("Best state written to the name of the Output tab (state_A.dat -> state_A_orbit.dat); "
                        "the file of another setup (beststate_orbit.dat) untouched",
                        ok_state and decoy.startswith("# Spectangular best state (P e K_A K_B gamma T_peri omega) residuum 1e-9")
                        and od is not None and abs(od[:, 2].min() - s1) <= 1e-9 * s1,
                        f"best {s1:.10g}, convergence data min {od[:, 2].min() if od is not None else float('nan'):.10g}"))
    checks.append(Check("Convergence data in conv_A.dat, no optdata.dat",
                        od is not None and not (workdir / "optdata.dat").exists(), ""))
    el1 = np.loadtxt(workdir / "myorbit.dat")
    checks.append(Check("Orbital elements read from myorbit.dat (Data tab) and the best ones written back "
                        "after Nelder-Mead; no orbitelements.dat needed or created",
                        "Error" not in (root / "out" / "stdout.log").read_text()
                        and not (workdir / "orbitelements.dat").exists() and np.max(np.abs(el1 - start)) > 0,
                        f"K_A {start[2]:g} -> {el1[2]:.4f}, K_B {start[3]:g} -> {el1[3]:.4f}"))
    checks.append(Check("Simplex backups named after the simplex files (optval_SB2_backup.txt)",
                        (workdir / "optval_SB2_backup.txt").exists() and not (workdir / "optval_vel_backup.dat").exists(), ""))
    # run 2: Continue with a best state of the wrong kind under the new name -> not used
    (workdir / "state_B_orbit.dat").write_text("# Spectangular best state (RVs A B per spectrum) residuum 1e-9 spectra 0 9\n"
                                               + "\n".join(["10\t-10"] * 10) + "\n")
    _conf_set(workdir, 2, "30")
    _conf_set(workdir, 36 + 8, "0")                  # New off
    _conf_set(workdir, 36 + 10, "1")                 # Continue
    _conf_v2(workdir, lineEdit_best="state_B.dat")
    run_adapter(case, workdir, root / "out2", timeout)
    log = (workdir / "SB2_log.txt").read_text()
    checks.append(Check("Continue ignores a best state of the other kind (RVs) under the orbit name",
                        "No best state of a previous optimisation" in log and "state_B_orbit.dat" in log
                        and "Start values from the best state" not in log, ""))
    # run 3: Levenberg-Marquardt, Continue with state_A.dat -> starts from it and writes the
    # best elements to myorbit.dat
    _conf_v2(workdir, lineEdit_best="state_A.dat", comboBox_method=1)
    run_adapter(case, workdir, root / "out3", timeout)
    log = (workdir / "SB2_log.txt").read_text()
    od = np.atleast_2d(np.loadtxt(workdir / "conv_A.dat"))
    checks.append(Check("Continue starts from state_A_orbit.dat (not from beststate_orbit.dat of the other setup)",
                        "Start values from the best state of the previous optimisation (state_A_orbit.dat" in log
                        and od[0, 2] <= s1 * (1 + 1e-9), f"first value {od[0, 2]:.10g}, best of run 1 {s1:.10g}"))
    el = np.loadtxt(workdir / "myorbit.dat")
    checks.append(Check("Best elements written to myorbit.dat, no orbitelements.dat",
                        np.max(np.abs(el - start)) > 0 and not (workdir / "orbitelements.dat").exists(),
                        f"K_A {start[2]:g} -> {el[2]:.4f}, K_B {start[3]:g} -> {el[3]:.4f}"))
    return checks


def mut_residual_stats_named(w):
    mut_residual_stats(w)
    _conf_v2(w, lineEdit_stat="stat_A.dat")


def mut_lm_rv_static(w):
    """RV mode with static lines, Levenberg-Marquardt from RVs off by 3 km/s +- 2 km/s: the mean
    RV of each component is not determined (also with static lines) and must stay."""
    mut_static(w)
    v = np.loadtxt(w / "velocities.txt", ndmin=2)
    rng = np.random.default_rng(3)
    v = v + 3.0 + rng.uniform(-2, 2, v.shape)
    np.savetxt(w / "velocities.txt", v, fmt="%.10g", delimiter="\t")
    np.savetxt(w / "velocities.start", v, fmt="%.10g", delimiter="\t")
    _conf_set(w, 2, "30")
    _conf_v2(w, comboBox_method=1)


def checks_lm_rv_static(workdir, outdir):
    log = (workdir / "SB2_log.txt").read_text()
    st = np.loadtxt(workdir / "velocities.start", ndmin=2)
    rows = [l for l in (workdir / "beststate_rv.dat").read_text().splitlines() if l and not l.startswith("#")]
    best = np.array([[float(x) for x in l.split()] for l in rows])
    dm = np.abs(best.mean(axis=0) - st.mean(axis=0)).max()
    moved = np.abs(best - st).max()
    return [Check("RV mode with static lines: mean RV of each component kept (zero point not determined)",
                  "Mean RV of a component without fixed RVs kept at its start value (zero point not determined by disentangling)" in log
                  and dm < 1e-6 and moved > 0.1, f"change of the means {dm:.1e} km/s, largest change of an RV {moved:.2f} km/s")]


def mut_bc_gamma_free(w):
    _bc_dataset(w, "observatory", free_gamma=True)


# ---- Barycentric correction: orbit with static (telluric) lines

BC_TRUTH = [20.54, 0.3, 40.0, 45.0, 5.0, 2448150.0, 1.2]      # P e K_A K_B gamma T_peri omega
BC_START = [20.54, 0.27, 37.0, 48.0, 2.0, 2448150.0, 1.15]


def _orbit_rv(t, el, comp):
    P, e, KA, KB, g, T0, om = el
    M = 2 * np.pi * (np.asarray(t) - T0) / P
    E = M.copy()
    for _ in range(50):
        E = E - (E - e * np.sin(E) - M) / (1 - e * np.cos(E))
    nu = 2 * np.arctan2(np.sqrt(1 + e) * np.sin(E / 2), np.sqrt(1 - e) * np.cos(E / 2))
    w_ = om if comp == "A" else om + np.pi
    K = KA if comp == "A" else KB
    return g + K * (np.cos(nu + w_) + e * np.cos(w_))


def _bc_dataset(w, frame="observatory", static=True, bc_file=True, free_gamma=False, tell_strength=None,
                tell_strength_h2o=None):
    """16 spectra 6400-6440 A (2 km/s per pixel, S/N 400) over 287 days: A and B on the orbit
    BC_TRUTH (barycentric), telluric lines at rest in the observatory frame, barycentric
    correction BC_j = 28 sin(2 pi (t - t0) / 365.25) km/s. frame 'observatory': spectra as
    observed; 'barycentric': corrected, lambda_bary = lambda_obs (1 + BC/c). The lines are
    evaluated analytically (no resampling). gamma is locked at the true value: a common shift of
    all RVs of a component is absorbed by its spectrum, also with static lines (the residuum
    does not depend on gamma)."""
    c = 299792.458
    n_sp = 16
    dlog = np.log10(1 + 2.0 / c)
    n = int(np.log10(6440 / 6400) / dlog) + 1
    lam = 10 ** (np.log10(6400) + dlog * np.arange(n))
    t = 2448150.3 + 19.13 * np.arange(n_sp)
    bc = 28 * np.sin(2 * np.pi * (t - 2448120) / 365.25)
    vA, vB = _orbit_rv(t, BC_TRUTH, "A"), _orbit_rv(t, BC_TRUTH, "B")

    def prof(x, lines, sig):
        y = np.zeros_like(x)
        for x0, d in lines:
            y -= d * np.exp(-0.5 * ((x - x0) / sig) ** 2)
        return y
    LA = [(6405.2, .5), (6409.8, .35), (6413.1, .45), (6418.7, .3), (6422.0, .5), (6427.4, .4), (6431.9, .35), (6436.3, .45)]
    LB = [(6404.1, .4), (6411.5, .5), (6416.0, .3), (6420.6, .45), (6425.2, .35), (6429.8, .5), (6434.0, .4)]
    LT = [(6407.5, .3), (6415.3, .25), (6419.9, .35), (6424.4, .2), (6433.1, .3)]
    A = lambda x: 1 + prof(x, LA, 0.18)
    B = lambda x: 1 + prof(x, LB, 0.22)
    T = lambda x: prof(x, LT, 0.07) if static else 0 * x
    # optional: the last two static lines ("H2O") with their own strength per spectrum
    T1 = lambda x: prof(x, LT[:3], 0.07) if static else 0 * x
    T2 = lambda x: prof(x, LT[3:], 0.07) if static else 0 * x
    rng = np.random.default_rng(7)
    sT = np.ones(n_sp) if tell_strength is None else np.asarray(tell_strength, float)
    sW = sT if tell_strength_h2o is None else np.asarray(tell_strength_h2o, float)
    for f in w.glob("spectram_*.txt"):
        f.unlink()
    for j in range(n_sp):
        if frame == "observatory":     # observed: stars at the observatory-frame RV, tellurics at rest
            f = 0.5 * A(lam / (1 + vA[j] / c) * (1 + bc[j] / c)) + 0.5 * B(lam / (1 + vB[j] / c) * (1 + bc[j] / c)) \
                + sT[j] * T1(lam) + sW[j] * T2(lam)
        else:                          # corrected: stars at the barycentric RV, tellurics moved by BC
            f = 0.5 * A(lam / (1 + vA[j] / c)) + 0.5 * B(lam / (1 + vB[j] / c)) \
                + sT[j] * T1(lam / (1 + bc[j] / c)) + sW[j] * T2(lam / (1 + bc[j] / c))
        f = f + rng.normal(0, 1 / 400, n)
        np.savetxt(w / f"spectram_{j}.txt", np.column_stack([np.log10(lam), f]), fmt=["%.12f", "%.10g"], delimiter="\t")
    np.savetxt(w / "times.dat", t, fmt="%.6f")
    np.savetxt(w / "truth_orbit.txt", BC_TRUTH, fmt="%.10g")
    np.savetxt(w / "orbitelements.dat", BC_START, fmt="%.10g")
    np.savetxt(w / "orbitelements.dat.start", BC_START, fmt="%.10g")
    np.savetxt(w / "truth_T.txt", np.column_stack([lam, T(lam)]), fmt="%.10g")
    np.savetxt(w / "truth_strength.txt", np.column_stack([sT / sT.mean(), sW / sW.mean()]), fmt="%.10g")
    with open(w / "static_groups.dat", "w") as fh:     # ranges of the static lines with groups
        for k, (x0, _) in enumerate(LT):
            fh.write(f"{x0 - 0.6:.2f} {x0 + 0.6:.2f} {'O2' if k < 3 else 'H2O'}\n")
    np.savetxt(w / "velocities.txt", np.column_stack([vA, vB]), fmt="%.6f", delimiter="\t")
    if bc_file:
        with open(w / "spectram_barycorr.txt", "w") as fh:
            fh.write("# Barycentric correction (synthetic)\n# spectrum\ttime\tJD_UTC\tBC [km/s]\n")
            for j in range(n_sp):
                fh.write(f"{j}\t{t[j]:.6f}\t{t[j]:.6f}\t{bc[j]:.6f}\n")
    _conf_set(w, 1, str(n_sp - 1))          # last spectrum
    _conf_set(w, 2, "60")                   # iterations
    _conf_set(w, 36 + 7, "1" if static else "0")   # static lines
    # orbital elements: the configuration stores "locked" (1) / free (0)
    _conf_set(w, 52, "0")                   # e free
    _conf_set(w, 53, "1")                   # P locked
    _conf_set(w, 57, "1")                   # T_peri locked
    _conf_v2(w, checkBox_31=0, checkBox_36=0, checkBox_37=0 if free_gamma else 1, checkBox_38=0,
             comboBox_method=1, lineEdit_bc="spectram_barycorr.txt" if bc_file else "",
             comboBox_bcframe=0 if frame == "observatory" else 1)
    if not free_gamma:
        el = list(BC_START); el[4] = BC_TRUTH[4]
        np.savetxt(w / "orbitelements.dat", el, fmt="%.10g")
        np.savetxt(w / "orbitelements.dat.start", el, fmt="%.10g")


# telluric strength per spectrum like an airmass 1.0 .. 2.2 (scaled to mean 1 in the truth file)
BC_STRENGTH = 1.0 + 1.2 * np.abs(np.sin(0.9 * np.arange(16) + 0.3)) ** 2


def mut_bc_tell_nofit(w):
    _bc_dataset(w, "observatory", tell_strength=BC_STRENGTH)


def mut_bc_tell_fit(w):
    _bc_dataset(w, "observatory", tell_strength=BC_STRENGTH)
    _conf_v2(w, checkBox_tellfit=1)


def mut_bc_tell_file(w):
    _bc_dataset(w, "observatory", tell_strength=BC_STRENGTH)
    np.savetxt(w / "airmass.txt", BC_STRENGTH, fmt="%.8g")
    _conf_v2(w, lineEdit_tellw="airmass.txt")


# O2 and H2O with independent strengths (like airmass and water vapour)
BC_STRENGTH_H2O = 1.0 + 1.5 * np.abs(np.cos(1.7 * np.arange(16) + 0.5)) ** 2


def mut_bc_tell_groups(w):
    _bc_dataset(w, "observatory", tell_strength=BC_STRENGTH, tell_strength_h2o=BC_STRENGTH_H2O)
    _conf_v2(w, checkBox_tellfit=1, checkBox_tellrange=1, lineEdit_tellrange="static_groups.dat")


def mut_bc_tell_groups_single(w):
    mut_bc_tell_groups(w)
    lines = [" ".join(l.split()[:2]) for l in (w / "static_groups.dat").read_text().splitlines()]
    (w / "static_nogroups.dat").write_text("\n".join(lines) + "\n")
    _conf_v2(w, lineEdit_tellrange="static_nogroups.dat")


def checks_bc_groups(name, workdir, outdir):
    out = (outdir / "stdout.log").read_text()
    m = re.findall(r"Levenberg-Marquardt: residuum ([0-9.eE+-]+)", out)
    res = float(m[-1]) if m else float("nan")
    el = np.loadtxt(workdir / "orbitelements.dat")
    dev = np.abs(el - np.array(BC_TRUTH))
    txt = f"residuum {res:.5g}; K_A {el[2]:.3f}, K_B {el[3]:.3f}"
    if name == "bc_tell_groups_single":
        return [Check("O2 and H2O of independent strength with one strength per spectrum: residuum high (> 0.5)", res > 0.5, txt)]
    truth = np.loadtxt(workdir / "truth_strength.txt")
    f = workdir / "SB2_tell_strength.txt"
    w = np.loadtxt(f, ndmin=2) if f.exists() else np.zeros((16, 2))
    rms = [float(np.sqrt(np.mean((w[:, g] - truth[:, g]) ** 2))) if w.shape[1] > g else 9.0 for g in range(2)]
    return [Check("Groups O2 and H2O from the ranges file, one strength each per spectrum, fitted in windows around "
                  "the ranges (rms against the truth < 0.03)",
                  "groups with their own strength: O2 (3), H2O (2)" in out and max(rms) < 0.03
                  and "in windows:" in out,
                  f"rms O2 {rms[0]:.4f}, H2O {rms[1]:.4f}"),
            Check("Residuum as for noise (< 0.3), orbit found (K within 0.3 km/s)",
                  res < 0.3 and dev[2] < 0.3 and dev[3] < 0.3, txt)]


def mut_bc_tell_file_count(w):
    mut_bc_tell_file(w)
    np.savetxt(w / "airmass.txt", BC_STRENGTH[:-2], fmt="%.8g")


def checks_bc_tell(name, workdir, outdir):
    out = (outdir / "stdout.log").read_text()
    m = re.findall(r"Levenberg-Marquardt: residuum ([0-9.eE+-]+)", out)
    res = float(m[-1]) if m else float("nan")
    el = np.loadtxt(workdir / "orbitelements.dat")
    dev = np.abs(el - np.array(BC_TRUTH))
    txt = f"residuum {res:.5g}; K_A {el[2]:.3f}, K_B {el[3]:.3f}, e {el[1]:.4f}, omega {el[6]:.4f}"
    good = dev[2] < 0.3 and dev[3] < 0.3 and dev[1] < 0.005 and dev[6] < 0.01
    checks = []
    if name == "bc_tell_nofit":
        checks.append(Check("Static lines of varying strength with a fixed strength: residuum high (> 1.0)", res > 1.0, txt))
        return checks
    truth = np.loadtxt(workdir / "truth_strength.txt")[:, 0]
    if name == "bc_tell_fit":
        f = workdir / "SB2_tell_strength.txt"
        w = np.loadtxt(f) if f.exists() else np.zeros(16)
        rms = float(np.sqrt(np.mean((w - truth) ** 2)))
        checks.append(Check("Strength of the static lines fitted per spectrum (written, rms against the truth < 0.02)",
                            "Strength of the static lines fitted per spectrum" in out and rms < 0.02,
                            f"rms {rms:.4f}; fitted {w.min():.3f} .. {w.max():.3f}, true {truth.min():.3f} .. {truth.max():.3f}"))
    else:
        checks.append(Check("Start strength from the file, scaled to mean 1", "Strength of the static lines from airmass.txt (mean 1)" in out, ""))
    checks.append(Check("Residuum as for noise only (< 0.3) and orbit found (K within 0.3 km/s, e 0.005, omega 0.01)",
                        res < 0.3 and good, txt))
    return checks


def mut_res_static(w, exclude=True, ranges=True):
    """Corrected spectra with telluric lines of varying strength (0.64-1.37), the model with
    static lines of one strength and the orbit at the truth: the misfit of the telluric lines is
    left in the residuals; "Exclude the ranges of the static lines from the residuum" (ranges
    +-0.6 A, observatory frame, shifted by BC up to 0.6 A), with the ranges also used for the
    static lines or only read for the residuum."""
    _bc_dataset(w, "barycentric", tell_strength=BC_STRENGTH)
    np.savetxt(w / "orbitelements.dat", BC_TRUTH, fmt="%.10g")
    _conf_v2(w, checkBox_resstatic=1 if exclude else 0, lineEdit_tellrange="static_groups.dat",
             checkBox_tellrange=1 if ranges else 0)


def checks_res_static(name, workdir, outdir):
    out = (outdir / "stdout.log").read_text()
    r = scalars(outdir / "result.txt")["residuum"]
    m = re.search(r"Residuum without the ranges of the static lines \(static_groups.dat\): ([0-9.]+) % of the pixels left out, shifted by BC", out)
    txt = f"residuum {r:.4f}" + (f", {m.group(1)} % left out" if m else "")
    if name == "res_static_noexcl":
        return [Check("Static lines of varying strength with one strength, residuum over all pixels: high (> 1)", r > 1.0 and not m, txt)]
    return [Check("Ranges of the static lines (shifted by BC) left out of the residuum: residuum at the noise (< 0.5)"
                  + (", ranges read only for the residuum" if name.endswith("_file") else ""),
                  m is not None and r < 0.5, txt)]


def mut_bc_tell_misaligned(w):
    """As bc_tell_fit, but the wavelength scale of spectrum 5 is shifted by +20 km/s (wrong
    correction in the reduction): its static lines do not match."""
    mut_bc_tell_fit(w)
    a = np.loadtxt(w / "spectram_5.txt")
    a[:, 0] += np.log10(1 + 20.0 / 299792.458)
    np.savetxt(w / "spectram_5.txt", a, fmt=["%.12f", "%.10g"], delimiter="\t")


def checks_bc_tell_misaligned(workdir, outdir):
    out = (outdir / "stdout.log").read_text()
    w = np.loadtxt(workdir / "SB2_tell_strength.txt")
    truth = np.loadtxt(workdir / "truth_strength.txt")[:, 0]
    others = [j for j in range(16) if j != 5]
    rel = w[others] / w[others].mean() - truth[others] / truth[others].mean()
    return [Check("Spectrum with shifted wavelength scale: strength near 0 but not 0 (the fit can recover), "
                  "warning naming the spectrum", 0 < w[5] < 0.1 and "the static lines do not fit spectrum 5 (" in out,
                  f"strength of spectrum 5: {w[5]:.4g}"),
            Check("Strengths of the other spectra as the truth (relative, rms < 0.05)", float(np.sqrt(np.mean(rel ** 2))) < 0.05,
                  f"rms {np.sqrt(np.mean(rel ** 2)):.4f}")]


def _long_static_dataset(w, damp):
    """Like the barycentric-correction data but 5000-5800 A at 4 km/s per pixel (11 124 pixels,
    16 spectra, S/N 300, random lines: 300 in A, 200 in B, 60 static lines in the observatory
    frame), spectra corrected: the broad structure of the static spectrum is nearly
    undetermined over this range."""
    c = 299792.458
    n_sp, step, wmin, wmax = 16, 4.0, 5000.0, 5800.0
    dlog = np.log10(1 + step / c)
    n = int(np.log10(wmax / wmin) / dlog) + 1
    lam = 10 ** (np.log10(wmin) + dlog * np.arange(n))
    t = 2448150.3 + 19.13 * np.arange(n_sp)
    bc = 28 * np.sin(2 * np.pi * (t - 2448120) / 365.25)
    vA, vB = _orbit_rv(t, BC_TRUTH, "A"), _orbit_rv(t, BC_TRUTH, "B")
    rng = np.random.default_rng(1)

    def lines(N, dmax):
        return [(rng.uniform(wmin + 5, wmax - 5), rng.uniform(0.05, dmax)) for _ in range(N)]

    def prof(x, L, sg):
        y = np.zeros_like(x)
        for x0, d in L:
            m = np.abs(x - x0) < 6 * sg
            y[m] -= d * np.exp(-0.5 * ((x[m] - x0) / sg) ** 2)
        return y
    LA, LB, LT = lines(300, 0.5), lines(200, 0.4), lines(60, 0.3)
    (w / "truth_static_lines.txt").write_text("\n".join(f"{x0:.4f}" for x0, _ in LT) + "\n")
    for f in w.glob("spectram_*.txt"):
        f.unlink()
    for j in range(n_sp):
        f = 0.5 * (1 + prof(lam / (1 + vA[j] / c), LA, 0.15)) + 0.5 * (1 + prof(lam / (1 + vB[j] / c), LB, 0.2)) \
            + prof(lam / (1 + bc[j] / c), LT, 0.06) + rng.normal(0, 1 / 300, n)
        np.savetxt(w / f"spectram_{j}.txt", np.column_stack([np.log10(lam), f]), fmt=["%.12f", "%.10g"], delimiter="\t")
    np.savetxt(w / "times.dat", t, fmt="%.6f")
    np.savetxt(w / "orbitelements.dat", BC_TRUTH, fmt="%.10g")
    np.savetxt(w / "spectram_barycorr.txt", bc, fmt="%.6f")
    _conf_set(w, 1, str(n_sp - 1))
    _conf_set(w, 36 + 7, "1")
    _conf_v2(w, lineEdit_bc="spectram_barycorr.txt", comboBox_bcframe=1, doubleSpinBox_tdamp=damp)


def checks_static_long(name, outdir):
    out = (outdir / "stdout.log").read_text()
    m = re.search(r"LSQR: (\d+) iterations", out)
    its = int(m.group(1)) if m else -1
    A, B = np.loadtxt(outdir / "SB2_1.txt"), np.loadtxt(outdir / "SB2_2.txt")
    pa, pb = np.percentile(A[:, 1], [2, 98]), np.percentile(B[:, 1], [2, 98])
    det = f"{its} LSQR iterations; A 2-98 %: {pa[0]:.3f} .. {pa[1]:.3f}, B {pb[0]:.3f} .. {pb[1]:.3f}"
    if name == "static_long_nodamp":
        return [Check("Static lines over 800 A without damping: many LSQR iterations (> 4500)", its > 4500, det)]
    return [Check("Static lines with damping (default 1e-3): fewer than 3500 LSQR iterations, continua of A and B "
                  "near 1 (98th percentile < 1.03)", 0 < its < 3500 and pa[1] < 1.03 and pb[1] < 1.03, det)]


def checks_static_ranges(case, workdir, root, timeout):
    """Long static data set: (1) Solve over the whole range (damped), Detect; (2) Solve with the
    static lines only in the detected ranges; (3) without a ranges file: default list written;
    (4) empty ranges file: Error 50."""
    out1 = root / "out"
    main = dict(l.split("\t") for l in (out1 / "main.txt").read_text().splitlines())
    det = main.get("tellrange", "")
    rng = []
    if det and (workdir / det).exists():
        for l in (workdir / det).read_text().splitlines():
            if l.strip() and not l.startswith("#"):
                a, b = l.split()[:2]
                rng.append((float(a), float(b)))
    truth = [float(x) for x in (workdir / "truth_static_lines.txt").read_text().split()]
    missed = [x for x in truth if not any(a <= x <= b for a, b in rng)]
    cover = sum(b - a for a, b in rng)
    stdout1 = (out1 / "stdout.log").read_text()
    its1 = int(re.search(r"LSQR: (\d+) iterations", stdout1).group(1))
    checks = [Check("Detect: all 60 static lines inside the detected ranges, which cover less than 15 % of 800 A",
                    det.endswith("_detected.dat") and not missed and 0 < cover < 120,
                    f"{len(rng)} ranges, {cover:.1f} A; missed {missed[:5]}")]
    # run 2: only in the detected ranges
    _conf_v2(workdir, checkBox_tellrange=1, lineEdit_tellrange=det)
    run_adapter("solve", workdir, root / "out2", timeout)
    stdout2 = (root / "out2" / "stdout.log").read_text()
    its2 = int(re.search(r"LSQR: (\d+) iterations", stdout2).group(1))
    r1 = float(re.search(r"residuum\s+([0-9.eE+-]+)", (out1 / "result.txt").read_text()).group(1)) \
        if (out1 / "result.txt").exists() else float("nan")
    r2 = float(re.search(r"residuum\s+([0-9.eE+-]+)", (root / "out2" / "result.txt").read_text()).group(1))
    T = np.loadtxt(workdir / "SB2_tell.txt")
    A = np.loadtxt(root / "out2" / "SB2_1.txt")
    pa = np.percentile(A[:, 1], [2, 98])
    # residuum: over the whole range the static spectrum also fits noise (one unknown per pixel
    # for 16 spectra: 1/16 of the variance, about 3 % of the residuum); in the ranges it does not
    checks.append(Check("Static lines only in the ranges: fewer LSQR iterations, residuum up by the noise the static "
                        "spectrum no longer fits (< 4.5 %), static spectrum written only there, continuum of A near 1",
                        f"Static lines only in {len(rng)} ranges" in stdout2 and its2 < its1 and 0 <= r2 / r1 - 1 < 0.045
                        and len(T) < 0.3 * len(A) and pa[1] < 1.03,
                        f"iterations {its1} -> {its2}, residuum {r1:.5g} -> {r2:.5g}, {len(T)} static pixels, A 98 %: {pa[1]:.3f}"))
    # run 3: file missing -> default list
    _conf_v2(workdir, lineEdit_tellrange="telluric_static.dat")
    run_adapter("solve", workdir, root / "out3", timeout)
    stdout3 = (root / "out3" / "stdout.log").read_text()
    checks.append(Check("Without a ranges file the default list (telluric O2, H2O, weak systems) is written and used",
                        (workdir / "telluric_static.dat").exists() and "default list" in stdout3
                        and "Static lines only in 11 ranges" in stdout3, ""))
    # run 4: file without ranges
    (workdir / "empty_ranges.dat").write_text("# nothing\n")
    _conf_v2(workdir, lineEdit_tellrange="empty_ranges.dat")
    try:
        run_adapter("solve", workdir, root / "out4", timeout)
        msg = (root / "out4" / "stdout.log").read_text()
    except RuntimeError as e:
        msg = str(e)
    checks.append(Check("Ranges file without ranges: Error 50", "Error 50" in msg, msg[:120]))
    return checks


def mut_bc_obs_static(w):
    _bc_dataset(w, "observatory")


def mut_bc_obs_static_nobc(w):
    _bc_dataset(w, "observatory", bc_file=False)


def mut_bc_corrected_static(w):
    _bc_dataset(w, "barycentric")


def mut_bc_corrected_static_nofile(w):
    _bc_dataset(w, "barycentric", bc_file=False)


def mut_bc_obs_nostatic(w):
    _bc_dataset(w, "observatory", static=False)


def mut_bc_wrong_count(w):
    _bc_dataset(w, "observatory")
    lines = (w / "spectram_barycorr.txt").read_text().splitlines()
    (w / "spectram_barycorr.txt").write_text("\n".join(lines[:-1]) + "\n")


def checks_bc(name, workdir, outdir):
    truth = np.array(BC_TRUTH)
    el = np.loadtxt(workdir / "orbitelements.dat")
    start = np.loadtxt(workdir / "orbitelements.dat.start")
    out = (outdir / "stdout.log").read_text()
    dev = el - truth
    txt = (f"e {el[1]:.4f}, K_A {el[2]:.3f}, K_B {el[3]:.3f}, omega {el[6]:.4f} "
           f"(true 0.3, 40, 45, 1.2; start 0.27, 37, 48, 1.15)")
    checks = []
    if name == "bc_obs_static_nobc":
        checks.append(Check("Note: orbit mode with static lines without barycentric correction",
                            "without barycentric correction" in out, ""))
        big = max(abs(dev[2]), abs(dev[3]))
        checks.append(Check("Without the correction the orbit is wrong (K off by more than 2 km/s)",
                            big > 2.0, txt))
        return checks
    used = {"bc_obs_static": "RVs of the orbit converted to the observatory frame",
            "bc_obs_nostatic": "RVs of the orbit converted to the observatory frame",
            "bc_corrected_static": "static lines shifted by BC"}[name]
    checks.append(Check("Barycentric correction read and used", "Barycentric correction (spectram_barycorr.txt): 16 spectra" in out
                        and used in out, ""))
    tolK, tolE, tolW = 0.3, 0.005, 0.01
    ok = abs(dev[2]) < tolK and abs(dev[3]) < tolK and abs(dev[1]) < tolE and abs(dev[6]) < tolW
    checks.append(Check("Orbit found with the barycentric correction (K within 0.3 km/s, e 0.005, omega 0.01)",
                        ok and np.max(np.abs(start - truth)) > 1, txt))
    if name == "bc_corrected_static":
        Tt = np.loadtxt(workdir / "truth_T.txt")
        To = np.loadtxt(workdir / "SB2_tell.txt")
        Ti = np.interp(Tt[:, 0], To[:, 0], To[:, 1])
        m = (Tt[:, 0] > 6403) & (Tt[:, 0] < 6437)
        rms = float(np.sqrt(np.mean((Ti[m] - Tt[m, 1]) ** 2)))
        checks.append(Check("Static lines in the observatory frame recovered (rms < 0.01)", rms < 0.01, f"rms {rms:.4f}"))
    return checks


def mut_abort_long(w):
    """Long spectra (10 x 37 000 pixels, 4500-6500 A at 3 km/s, S/N 100): one solve takes
    longer than the 0.1 s between two GUI updates."""
    _eclipse(w, "constant", snr=100.0, dv=3.0)
    _conf_set(w, 2, "200")
    _conf_set(w, 36 + 8, "1")                        # New
    _conf_v2(w, comboBox_interp=0)


def checks_abort_long(outdir, solve=False):
    r = {l.split()[0]: float(l.split()[1]) for l in (outdir / "result.txt").read_text().splitlines() if l.strip()}
    out = (outdir / "stdout.log").read_text()
    return [Check("Window responsive during long solves (largest gap between two 50 ms timer events < 1 s)",
                  r["ticks"] >= 5 and r["max_gap_ms"] < 1000,
                  f"{r['ticks']:.0f} timer events, largest gap {r['max_gap_ms']:.0f} ms"),
            Check(("Stop ends Solve at once, without an error box" if solve else
                   "Stop ends the optimisation within 1 s (during a solve: the solve itself)"),
                  r["abort_ms"] > 0 and r["end_ms"] - r["abort_ms"] < 1000
                  and (not solve or ("Solve stopped." in out and "LSQR aborted" in out
                                     and not (outdir / "MODALS.txt").exists())),
                  f"Stop at {r['abort_ms']:.0f} ms, end at {r['end_ms']:.0f} ms, {r['evaluations']:.0f} evaluations"
                  + ("; LSQR aborted" if "LSQR aborted" in out else "; between two solves"))]


def mut_mask_tellurics(w):
    """Synthetic binary 6950-7550 A (16 spectra, K 25/30 km/s, S/N 300) with an H2O-like telluric
    band 7165-7335 A fixed in the frame of the input spectra, its depth varying with the airmass
    (1-2.2) from spectrum to spectrum: the two-component model cannot describe it."""
    sys.path.insert(0, str(HERE / "tools"))
    import make_eclipse
    for f in w.glob("spectram_*.txt"):
        f.unlink()
    make_eclipse.make(w, w / "SB2_conf.sps", l1=6950.0, l2=7550.0, dv=3.0, snr=300.0, ka=25.0, kb=30.0,
                      eps_a=0.0, eps_b=0.0, phases=list(np.linspace(0, 0.95, 16)))
    rng = np.random.default_rng(5)
    tl = [(rng.uniform(7165, 7335), rng.uniform(3, 6), rng.uniform(0.05, 0.5)) for _ in range(120)]
    airmass = 1 + rng.uniform(0, 1.2, 16)
    for j in range(16):
        f = w / f"spectram_{j}.txt"
        d = np.loadtxt(f)
        d[:, 1] *= make_eclipse.spectrum(10 ** d[:, 0], tl) ** airmass[j]
        np.savetxt(f, d, fmt="%.15g", delimiter="\t")
    _conf_v2(w, comboBox_ratio=0)


def checks_mask_tellurics(case, workdir, root, timeout):
    """Run 1 (generic): without excluded ranges. Run 2: default telluric list."""
    def levels(out):
        res = {}
        for c, t in (("1", "A"), ("2", "B")):
            d = np.loadtxt(out / f"SB2_{c}.txt")
            tr = np.loadtxt(workdir / f"truth_{t}.txt")
            dev = d[:, 1] - np.interp(d[:, 0], tr[:, 0], tr[:, 1])
            left = np.median(dev[(d[:, 0] > 7050) & (d[:, 0] < 7150)])
            right = np.median(dev[(d[:, 0] > 7350) & (d[:, 0] < 7450)])
            gap = int(np.sum((d[:, 0] > 7200) & (d[:, 0] < 7300)))
            res[t] = (right - left, gap, d)
        return res
    r0 = levels(root / "out")
    _conf_v2(workdir, checkBox_mask=1, lineEdit_mask="telluric_mask.dat")
    run_adapter(case, workdir, root / "out2", timeout)
    out = (root / "out2" / "stdout.log").read_text()
    r1 = levels(root / "out2")
    created = (workdir / "telluric_mask.dat").exists() and "default list (strongest telluric bands, strong chromospheric lines)" in out
    m = re.search(r"Excluded ranges \(telluric_mask.dat\): (\d+) ranges, (\d+) of (\d+) pixels", out)
    diff = np.loadtxt(workdir / "SB2_diff.txt") if (workdir / "SB2_diff.txt").exists() else None
    steps0 = ", ".join(f"{t} {r0[t][0]:+.3f}" for t in "AB")
    steps1 = ", ".join(f"{t} {r1[t][0]:+.3f}" for t in "AB")
    # largest deviation from the truth next to the gap (columns that lost most of their data are not written)
    def edge_dev(t):
        d = r1[t][2]
        dev = np.abs(d[:, 1] - np.interp(d[:, 0], *np.loadtxt(workdir / f"truth_{t}.txt").T))
        i = int(np.searchsorted(d[:, 0], 7250.0))      # first written pixel after the gap
        return dev[i - 10:i + 10].max()                 # 10 pixels on each side of the gap
    edge = max(edge_dev(t) for t in "AB")
    return [Check("Default list (telluric bands, chromospheric lines) written to the work folder and used",
                  created and bool(m) and int(m.group(1)) == 18, m.group(0) if m else "no message"),
            Check("Without excluded ranges: step of A and B across the telluric band (> 0.3)",
                  all(abs(r0[t][0]) > 0.3 for t in "AB"), "level right - left of the band: " + steps0),
            Check("With the telluric band excluded: no step (< 0.05), no output inside the band, edges of the gap clean (< 0.1)",
                  all(abs(r1[t][0]) < 0.05 and r1[t][1] == 0 for t in "AB") and edge < 0.1,
                  "level right - left: " + steps1 + f"; largest deviation in the 10 pixels on each side of the gap {edge:.3f}"),
            Check("Differences file without the excluded pixels",
                  diff is not None and not np.any((diff[:, 0] > 7170) & (diff[:, 0] < 7330)),
                  f"{0 if diff is None else len(diff)} lines")]


def mut_mask_optimisation(w):
    """As mask_tellurics, with the band excluded from the start, RVs perturbed by 3 km/s and
    12 Nelder-Mead iterations (warm-started solves)."""
    mut_mask_tellurics(w)
    v = np.loadtxt(w / "velocities.txt")
    np.savetxt(w / "truth_velocities.txt", v, fmt="%.17g", delimiter="\t")
    v = v + np.random.default_rng(1).normal(0, 3, v.shape)
    np.savetxt(w / "velocities.txt", v, fmt="%.17g", delimiter="\t")
    _conf_set(w, 2, "12")
    _conf_set(w, 36 + 8, "1")                        # New
    _conf_v2(w, comboBox_interp=0, checkBox_mask=1, lineEdit_mask="telluric_mask.dat")


def checks_mask_optimisation(workdir, outdir):
    out = {}
    for c, t in (("1", "A"), ("2", "B")):
        d = np.loadtxt(outdir / f"SB2_{c}.txt")
        tr = np.loadtxt(workdir / f"truth_{t}.txt")
        dev = d[:, 1] - np.interp(d[:, 0], tr[:, 0], tr[:, 1])
        left = np.median(dev[(d[:, 0] > 6950) & (d[:, 0] < 7150)])
        right = np.median(dev[(d[:, 0] > 7350) & (d[:, 0] < 7580)])
        out[t] = (left, right)
    txt = ", ".join(f"{t} {out[t][0]:+.3f} / {out[t][1]:+.3f}" for t in "AB")
    return [Check("Optimisation with an excluded range: the pieces on both sides keep the same level "
                  "(step < 0.1; before: A 2.3, B 3.4)",
                  all(abs(out[t][1] - out[t][0]) < 0.1 for t in "AB"),
                  "deviation from the truth left / right of the band: " + txt)]


def mut_continuum_norm(w):
    """mask_tellurics data with the telluric band excluded, output option continuum-normalised
    components; the components as from the disentangling are scaled by the light fractions
    (continua at about 1/l), so the normalisation has real work to do."""
    mut_mask_tellurics(w)
    _conf_v2(w, checkBox_mask=1, lineEdit_mask="telluric_mask.dat", checkBox_norm=1,
             doubleSpinBox_normKnot=80, doubleSpinBox_normLow=1.5)


def checks_continuum_norm(workdir, outdir):
    res = []
    lv = {}
    for c, t in (("1", "A"), ("2", "B")):
        raw = np.loadtxt(workdir / f"SB2_{c}.txt")
        nf = workdir / f"SB2_{c}_norm.txt"
        if not nf.exists():
            return [Check("Normalised component files written", False, f"{nf.name} missing")]
        nm = np.loadtxt(nf)
        tr = np.loadtxt(workdir / f"truth_{t}.txt")
        ti = np.interp(nm[:, 0], tr[:, 0], tr[:, 1])
        L = (nm[:, 0] > 7050) & (nm[:, 0] < 7150)
        R = (nm[:, 0] > 7350) & (nm[:, 0] < 7450)
        # the synthetic spectra are line blanketed (hardly a pixel at the continuum): compare
        # the upper envelope (95th percentile) of the normalised component with that of the truth
        p95 = lambda v, m: np.percentile(v[m], 95)
        lv[t] = (0.0, p95(nm[:, 1], L) - p95(ti, L), p95(nm[:, 1], R) - p95(ti, R),
                 np.array_equal(raw[:, 0], nm[:, 0]))
    res.append(Check("Normalised files beside the files as from the disentangling (same wavelengths)",
                     all(lv[t][3] for t in "AB"), ", ".join(f"{t}: {lv[t][3]}" for t in "AB")))
    res.append(Check("Upper envelope (95th percentile) of the normalised A and B as that of the truth on both sides of the band (< 0.03)",
                     all(abs(lv[t][1]) < 0.03 and abs(lv[t][2]) < 0.03 for t in "AB"),
                     ", ".join(f"{t}: left {lv[t][1]:+.3f}, right {lv[t][2]:+.3f}" for t in "AB")))
    # conserving renormalisation: A' = A - e, B' = B + r e keeps the composite and the residuum
    log = (outdir / "stdout.log").read_text() if (outdir / "stdout.log").exists() else ""
    m = re.search(r"Renormalised A and B \(A - e, B \+ ([-0-9.eE+]+) e: composite unchanged\): "
                  r"residuum ([-0-9.eE+]+) -> ([-0-9.eE+]+)", log)
    if not m:
        res.append(Check("Conserving renormalisation reported", False, "message missing"))
        return res
    r, r0, r1 = float(m.group(1)), float(m.group(2)), float(m.group(3))
    A, An = np.loadtxt(workdir / "SB2_1.txt"), np.loadtxt(workdir / "SB2_1_norm.txt")
    B, Bn = np.loadtxt(workdir / "SB2_2.txt"), np.loadtxt(workdir / "SB2_2_norm.txt")
    # A and B can have different edge pixels: compare on the common wavelengths
    _, ia, ib = np.intersect1d(np.round(A[:, 0], 6), np.round(B[:, 0], 6), return_indices=True)
    dev = float(np.max(np.abs((Bn[ib, 1] - B[ib, 1]) - r * (A[ia, 1] - An[ia, 1]))))
    res.append(Check("Renormalisation keeps the composite: B' - B = r (A - A') at every pixel (< 1e-4)",
                     dev < 1e-4, f"r = {r:.5f}, max deviation {dev:.2e}"))
    res.append(Check("Residuum with the renormalised A and B as without (ratio within 1 +- 0.01)",
                     abs(r1 / r0 - 1) < 0.01, f"{r0:g} -> {r1:g} (x {r1 / r0:.4f})"))
    return res


def mut_align_continua(w):
    """Synthetic binary 5000-5600 A (16 spectra, K 25/30 km/s, S/N 300, light 0.6/0.4); each
    spectrum multiplied by 1 + 0.03 sin(2 pi lambda / P_j + phi_j), P_j 150-400 A: different
    normalisation of the observed spectra."""
    sys.path.insert(0, str(HERE / "tools"))
    import make_eclipse
    for f in w.glob("spectram_*.txt"):
        f.unlink()
    make_eclipse.make(w, w / "SB2_conf.sps", l1=5000.0, l2=5600.0, dv=3.0, snr=300.0, ka=25.0, kb=30.0,
                      eps_a=0.0, eps_b=0.0, phases=list(np.linspace(0, 0.95, 16)))
    rng = np.random.default_rng(7)
    for j in range(16):
        f = w / f"spectram_{j}.txt"
        d = np.loadtxt(f)
        lam = 10 ** d[:, 0]
        d[:, 1] *= 1 + 0.03 * np.sin(2 * np.pi * lam / rng.uniform(150, 400) + rng.uniform(0, 2 * np.pi))
        np.savetxt(f, d, fmt="%.15g", delimiter="\t")
    _conf_v2(w, comboBox_ratio=0)


def checks_align_continua(case, workdir, root, timeout):
    """Run 1 (generic): without aligning; run 2: continua aligned to the mean spectrum (30 A)."""
    def wiggle(out):
        r = []
        for c, t in (("1", "A"), ("2", "B")):
            d = np.loadtxt(out / f"SB2_{c}.txt")
            tr = np.loadtxt(workdir / f"truth_{t}.txt")
            dev = d[:, 1] - np.interp(d[:, 0], tr[:, 0], tr[:, 1])
            k = 301
            sm = np.convolve(dev, np.ones(k) / k, mode="valid")    # broad part (about 3 A)
            r.append(float(np.std(sm)))
        return r
    w0 = wiggle(root / "out")
    _conf_v2(workdir, checkBox_align=1, doubleSpinBox_alignWidth=30)
    run_adapter(case, workdir, root / "out2", timeout)
    out = (root / "out2" / "stdout.log").read_text()
    w1 = wiggle(root / "out2")
    m = re.search(r"Continua aligned to the mean spectrum \(30 A, (\d+) px\)", out)
    return [Check("Continua aligned to the mean spectrum (message with the width in pixels)", bool(m),
                  m.group(0) if m else "no message"),
            Check("Broad wiggles of A and B from different normalisations reduced at least threefold",
                  all(b < a / 3 for a, b in zip(w0, w1)),
                  "rms of the broad deviation from the truth A %.3f -> %.3f, B %.3f -> %.3f" % (w0[0], w1[0], w0[1], w1[1]))]


def checks_lm_log(workdir, outdir):
    log = (workdir / "SB2_log.txt").read_text()
    m = re.search(r"Levenberg-Marquardt result after (\d+) steps .*?(converged)?$", log, re.M)
    checks = [Check("Levenberg-Marquardt converged", bool(m) and m.group(2) == "converged",
                    m.group(0) if m else "no result in the log")]
    od = np.atleast_2d(np.loadtxt(workdir / "optdata.dat"))
    steps = od[:, 2]
    checks.append(Check("Convergence data: start and accepted steps only, residuum not increasing",
                        len(steps) >= 2 and bool(np.all(np.diff(steps) <= 1e-12 * steps[0])),
                        f"{len(steps)} lines, {steps[0]:.6g} -> {steps[-1]:.6g}"))
    if "orbital parameters" in log:
        # the result is the start for the next Read data / Solve
        mm = re.search(r"Levenberg-Marquardt result.*?\n((?:\w+\t\S+\n)+)", log)
        best = {k: float(v) for k, v in (l.split("\t") for l in mm.group(1).splitlines())} if mm else {}
        el = np.loadtxt(workdir / "orbitelements.dat")
        idx = {"P": 0, "e": 1, "K_A": 2, "K_B": 3, "gamma": 4, "T_peri": 5, "omega": 6}
        dev = max((abs(el[idx[k]] - v) / max(1.0, abs(v)) for k, v in best.items()), default=1.0)
        checks.append(Check("Best elements written to orbitelements.dat", bool(best) and dev < 1e-10,
                            f"max. rel. deviation {dev:.1e}"))
    return checks


def mut_linear_grid(w, resample=True):
    """Spectra on a linear wavelength scale (Angstrom, rounded to 0.01 A as in real
    reductions), each with its own start and length; resampled when reading."""
    from scipy.interpolate import CubicSpline
    files = sorted(w.glob("spectram_*.txt"), key=lambda p: int(p.stem.split("_")[1]))
    for j, p in enumerate(files):
        d = np.loadtxt(p)
        np.savetxt(w / f"orig_{j}.txt", d, fmt="%.17g", delimiter="\t")
        lam = 10 ** d[:, 0]
        step = (lam[-1] - lam[0]) / (len(lam) - 1)
        lo = lam[0] + (0.13 * j % 1.0) * step + (j % 3) * step
        hi = lam[-1] - (j % 2) * step
        grid = np.round(np.arange(lo, hi, step), 2)
        np.savetxt(p, np.column_stack([grid, CubicSpline(lam, d[:, 1])(grid)]), fmt="%.10g", delimiter="\t")
    if resample:
        _conf_v2(w, checkBox_42=1, doubleSpinBox_18=0)


def checks_linear_grid(workdir, outdir):
    from scipy.interpolate import CubicSpline
    out = (outdir / "stdout.log").read_text()
    m = re.search(r"Resampled (\d+) spectra \((\d+) on a linear scale\) to a common log grid: (\d+) pixels", out)
    W = np.loadtxt(outdir / "W.txt")
    C = np.loadtxt(outdir / "C.txt")
    n = int(m.group(3)) if m else 0
    checks = [Check("Linear wavelengths resampled to a common log grid", bool(m) and m.group(1) == m.group(2),
                    m.group(0) if m else "no message")]
    if not m:
        return checks
    w0 = W[:n]
    dev = np.abs(np.diff(w0) / np.diff(w0).mean() - 1).max()
    same = max(np.abs(W[j * n:(j + 1) * n] - w0).max() for j in range(len(W) // n))
    checks.append(Check("Common equidistant log grid for all spectra", dev < 1e-6 and same == 0,
                        f"step deviation {dev:.1e}"))
    # flux against the original log-grid spectrum, interpolated at the same wavelengths
    worst, rms = 0.0, []
    for j, p in enumerate(sorted(workdir.glob("orig_*.txt"), key=lambda q: int(q.stem.split("_")[1]))):
        d = np.loadtxt(p)
        dd = C[j * n:(j + 1) * n] - CubicSpline(d[:, 0], d[:, 1])(w0)
        worst = max(worst, np.abs(dd).max())
        rms.append(np.sqrt(np.mean(dd ** 2)))
    # two interpolations (test: log -> linear; program: linear -> log) of 4x binned spectra
    # with lines only 2-3 pixels wide; below the noise of these data (~0.008)
    checks.append(Check("Resampled flux equals the original spectrum (RMS < 0.005)",
                        max(rms) < 5e-3, f"RMS up to {max(rms):.2g}, max deviation {worst:.2g} (line depth ~0.3)"))
    return checks

def mut_perturb_rvs_weighted(w):
    """RV optimisation with S/N weights estimated automatically (spectra of different noise)."""
    mut_perturb_rvs(w)
    rng = np.random.default_rng(5)
    for k, p in enumerate(sorted(w.glob("spectram_*.txt"))):
        d = np.loadtxt(p)
        d[:, 1] += rng.normal(0, 0.003 * (1 + k % 3), len(d))
        np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    _conf_v2(w, checkBox_41=1, lineEdit_29="auto")


def checks_error_estimation(workdir, outdir):
    rows = {}
    for line in (outdir / "errors.dat").read_text().splitlines():
        parts = line.split("\t")
        if len(parts) == 4 and not line.startswith("#"):
            rows[parts[0]] = [float(x) for x in parts[1:]]
    out = [Check("Error file contains the free elements", set(rows) == {"e", "K_A", "K_B", "omega"},
                 ", ".join(sorted(rows)))]
    for n, (best, err, qmin) in sorted(rows.items()):
        ok = math.isfinite(err) and err > 0 and abs(qmin - best) < 5 * err
        out.append(Check(f"Standard error {n}", ok, f"{best:.5g} +/- {err:.2g} (model minimum {qmin:.5g})"))
    txt = (outdir / "errors.dat").read_text()
    corr = txt.split("Correlation:")[1].split("Matrix Q:")[0]
    vals = [float(x) for x in corr.split()]
    out.append(Check("Correlations in [-1, 1], diagonal 1",
                     all(abs(v) <= 1 + 1e-9 for v in vals) and len(vals) == 16
                     and all(abs(vals[i * 5] - 1) < 1e-9 for i in range(4)), ""))
    return out


# ---- Branches of the model on exact synthetic data (integer shifts)

def _synth_rebuild(w, wa, wb, static=None, sb1=False, keep=None, odd=False):
    """Regenerate observations from truth_A/B: wa[j] A(i-ka) + wb[j] B(i-kb) + T(i).
    odd: change every second shift of A by 1 pixel (even and odd mixed;
    otherwise an alternating pattern in A cannot be separated from T)."""
    sh = np.loadtxt(w / "truth_shifts_px.txt").astype(int)
    if odd:
        sh[1::2, 0] += 1
        dlog = np.diff(np.loadtxt(w / "spectram_0.txt")[:, 0]).mean()
        vel = 299792.458 * (10 ** (sh * dlog) - 1)
        np.savetxt(w / "velocities.txt", vel, fmt="%.17g", delimiter="\t")
    A = np.loadtxt(w / "truth_A.txt")[:, 1]
    B = np.loadtxt(w / "truth_B.txt")[:, 1]
    n = len(A)

    def shifted(f, k):
        out = np.ones(n)
        i = np.arange(n) - k
        ok = (i >= 0) & (i < n)
        out[ok] = f[i[ok]]
        return out

    for j in range(len(sh)):
        p = w / f"spectram_{j}.txt"
        d = np.loadtxt(p)
        obs = wa[j] * shifted(A, sh[j, 0])
        if not sb1:
            obs = obs + wb[j] * shifted(B, sh[j, 1])
        if static is not None:
            obs = obs + static
        d[:, 1] = obs
        np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    if keep is not None:
        _conf_set(w, 1, str(keep - 1))      # spinBox_2: last spectrum
        for f in ("velocities.txt", "times.dat"):
            lines = (w / f).read_text().splitlines()
            (w / f).write_text("\n".join(lines[:keep]) + "\n")


def mut_ratio(w):
    """F_A/F_B = 2: weights 2/3 and 1/3 via doubleSpinBox_4."""
    n = len(np.loadtxt(w / "times.dat"))
    _synth_rebuild(w, [2 / 3] * n, [1 / 3] * n)
    _conf_set(w, 9, "2")


def mut_static(w):
    """Additional static (telluric) lines, unshifted."""
    n = len(np.loadtxt(w / "times.dat"))
    x = np.arange(len(np.loadtxt(w / "truth_A.txt")))
    T = np.zeros(len(x))
    for x0, s_, d in ((40, 1.5, 0.15), (95, 2.0, 0.25), (130, 1.2, 0.1)):
        T -= d * np.exp(-0.5 * ((x - x0) / s_) ** 2)
    _synth_rebuild(w, [0.5] * n, [0.5] * n, static=T, odd=True)
    lam = np.loadtxt(w / "truth_A.txt")[:, 0]
    np.savetxt(w / "truth_T.txt", np.column_stack([lam, T]), fmt="%.17g")
    _conf_set(w, 36 + 7, "1")               # checkBox_8: static lines
    _conf_v2(w, doubleSpinBox_tdamp=0)      # exact data: no damping of the static spectrum


def mut_edit_weights(w):
    """Different light fractions per spectrum via edit.dat (Mtel, Mval1, Mval2)."""
    n = len(np.loadtxt(w / "times.dat"))
    wa = 0.35 + 0.3 * np.arange(n) / (n - 1)
    wb = 1 - wa
    _synth_rebuild(w, wa, wb)
    (w / "edit.dat").write_text("".join(f"{v:.17g}\n" for v in np.concatenate([np.ones(n), wa, wb])))


def mut_synth_sb1(w):
    """SB1: component A only. Even for SB1 the weight of A is F_A/(F_A+F_B) from
    "Flux Ratio" (here 1, i.e. 0.5) - behaviour as in 1.11, see finding in the report."""
    n = len(np.loadtxt(w / "times.dat"))
    _synth_rebuild(w, [0.5] * n, [0.0] * n, sb1=True)
    _conf_set(w, 36 + 2, "1")               # checkBox_3: SB1
    _conf_set(w, 36 + 3, "0")               # checkBox_4: SB2 off


def mut_few_spectra(w):
    """Only 4 spectra (poor phase coverage)."""
    n = len(np.loadtxt(w / "times.dat"))
    _synth_rebuild(w, [0.5] * n, [0.5] * n, keep=4)


def mut_opt_ratios(w):
    """Different F_A/F_B per spectrum (true), start 1 for all."""
    n = len(np.loadtxt(w / "times.dat"))
    fr = 1 + 0.5 * np.sin(2 * np.pi * np.arange(n) / n + 0.7)
    _synth_rebuild(w, fr / (1 + fr), 1 / (1 + fr))
    np.savetxt(w / "truth_ratios.txt", fr, fmt="%.15g")
    _conf_set(w, 9, "1")                    # Start F_A/F_B = 1
    _conf_set(w, 2, "3000")                 # iterations (10 parameters)
    _conf_set(w, 36 + 8, "1")               # New


def mut_opt_ratios_autostop(w):
    """As mut_opt_ratios, with noise (S/N 100) and Auto Stop; iteration limit far away."""
    mut_opt_ratios(w)
    rng = np.random.default_rng(3)
    for p in sorted(w.glob("spectram_*.txt")):
        d = np.loadtxt(p)
        d[:, 1] += rng.normal(0, 0.01, len(d))
        np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    _conf_set(w, 2, "20000")
    lines = (w / "SB2_conf.sps").read_text().splitlines()
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    lines += ["checkBox_23=1", "spinBox_11=0", "doubleSpinBox_16=0", "doubleSpinBox_17=0.1"]
    (w / "SB2_conf.sps").write_text("\n".join(lines) + "\n")


def checks_opt_ratios_autostop(workdir, outdir):
    log = (outdir / "stdout.log").read_text()
    iters = len(re.findall(r"^Iteration: \d+", log, re.M))
    fr = np.loadtxt(workdir / "truth_ratios.txt")
    y = np.loadtxt(outdir / "optval_flux.dat")
    P = np.loadtxt(outdir / "optmat_flux.dat").reshape(len(y), -1)
    best = P[np.argmin(y)]
    # A common factor of all ratios is hardly constrained by the residuum (flat valley);
    # the pattern from spectrum to spectrum is what the data determine.
    rel = best / best.mean() - fr / fr.mean()
    rms = float(np.sqrt(np.mean(rel ** 2)))
    return [Check("Auto Stop ends the flux ratio optimisation", "Optimisation stopped (Auto Stop)" in log
                  and iters < 20000, f"{iters} iterations"),
            Check("Flux ratio pattern within the noise", rms < 0.04,
                  f"RMS {rms:.3f}, common factor {best.mean() / fr.mean():.3f}")]


def checks_opt_ratios(workdir, outdir):
    fr = np.loadtxt(workdir / "truth_ratios.txt")
    y = np.loadtxt(outdir / "optval_flux.dat")
    P = np.loadtxt(outdir / "optmat_flux.dat").reshape(len(y), -1)
    best = P[np.argmin(y)]
    dev = np.abs(best - fr).max()
    return [Check("Residual goes to 0", y.min() < 1e-6, f"{y.min():.2e}"),
            Check("Flux ratios per spectrum recovered", dev < 1e-3,
                  f"max. deviation {dev:.1e}; true {np.round(fr, 3).tolist()}")]


def checks_branch(workdir, outdir, comps, rms_tol=1e-6):
    r = scalars(outdir / "result.txt")
    out = [Check("Residual at the true RVs ~ 0", r["residuum"] < 1e-9, f"{r['residuum']:.2e}")]
    for comp, f, t in comps:
        off, rms = component_match(outdir / f, workdir / t)
        out.append(Check(f"Component {comp}: matches truth, no offset", rms < rms_tol and off == 0,
                         f"RMS {rms:.2e}, offset {off} px"))
    X = np.loadtxt(outdir / "X.txt")
    out.append(Check("Solution finite", bool(np.all(np.isfinite(X))), f"{len(X)} unknowns"))
    return out


AB = [("A", "SB2_1.txt", "truth_A.txt"), ("B", "SB2_2.txt", "truth_B.txt")]


# ---- FITS input: the same spectra as a binary table (extension, columns from the configuration)

def _to_fits(w, dtype="f8", extname="DataVector", keep_case=True, extra=False, cols=("Arg", "Fun")):
    from astropy.io import fits
    fmt = "D" if dtype == "f8" else "E"
    for p in sorted(w.glob("spectram_*.txt")):
        d = np.loadtxt(p)
        hdu = fits.BinTableHDU.from_columns(
            [fits.Column(name=cols[0], format=fmt, array=d[:, 0].astype(dtype)),
             fits.Column(name=cols[1], format=fmt, array=d[:, 1].astype(dtype))], name=extname)
        hdus = [fits.PrimaryHDU()]
        if extra:   # image and table with other columns before the correct table
            hdus.append(fits.ImageHDU(np.zeros(5), name="BILD"))
            hdus.append(fits.BinTableHDU.from_columns(
                [fits.Column(name="X", format="D", array=np.zeros(3))], name="ANDERE"))
        fits.HDUList(hdus + [hdu]).writeto(p.with_suffix(".fits"), overwrite=True)
        if keep_case:
            # EXTNAME exactly as given (astropy otherwise writes upper case)
            with fits.open(p.with_suffix(".fits"), mode="update") as h:
                h[-1].header["EXTNAME"] = extname
        p.unlink()
    _conf_set(w, 13, "0")                   # comboBox: fits
    _conf_set(w, 32, ".fits")               # lineEdit_19: extension


def mut_fits(w):
    _to_fits(w)


def mut_fits_float32(w):
    _to_fits(w, "f4")


def mut_fits_other_ext(w):
    """Extension is named differently than in the configuration: first matching table."""
    _to_fits(w, extname="Spektrum")


def mut_fits_empty_ext(w):
    """No extension name given."""
    _to_fits(w)
    _conf_set(w, 22, "")                    # lineEdit_9


def mut_fits_uppercase(w):
    """astropy default: EXTNAME in upper case, configuration 'DataVector'."""
    _to_fits(w, keep_case=False)


def mut_fits_extra_hdus(w):
    _to_fits(w, extname="SPEKTRUM", keep_case=False, extra=True)
    _conf_set(w, 22, "")


def mut_fits_no_columns(w):
    _to_fits(w, cols=("WAVE", "FLUX"))


def checks_fits_float32(outdir):
    """float32: log10(lambda) rounded to ~2e-7, individual steps jump by a few
    percent; but the pixel positions are correct to < 0.1 pixel. Must be read in."""
    gold = np.loadtxt(GOLDEN / "read_data" / "C.txt")
    C = np.loadtxt(outdir / "C.txt")
    dev = np.abs(C - gold).max() / np.abs(gold).max()
    return [Check("float32 fluxes read (deviation within float precision)", dev < 1e-6, f"{dev:.1e}")]


def checks_gui_smoke(outdir):
    clicked = (outdir / "clicked.txt").read_text().splitlines()
    modals = (outdir / "MODALS.txt").read_text().splitlines() if (outdir / "MODALS.txt").exists() else []
    windows = [l for l in clicked if l.startswith("window/")]
    return [Check("All buttons and menu items without crash", clicked[-1:] == ["END"],
                  f"{len(clicked) - 1} actions, last: {clicked[-2] if len(clicked) > 1 else ''}"),
            Check("Auxiliary windows opened and clicked through", len(windows) >= 3,
                  ", ".join(w[7:] for w in windows)),
            Check("Messages without outdated button names", not any("ECON" in m for m in modals),
                  f"{len(modals)} messages")]


def checks_edge_coverage(outdir, on):
    r = scalars(outdir / "result.txt")
    if on:
        return [Check("Edge per spectrum from the RV span", r["margin"] > 0, f"{r['margin']:g} pixels"),
                Check("Counted rows only touch columns seen by all spectra", r["bad_entries"] == 0,
                      f"{r['bad_entries']:g} entries"),
                Check("Bins = rows without edge", r["bins"] == r["counted_rows"],
                      f"{r['bins']:g} / {r['counted_rows']:g}, unknowns {r['cols']:g}")]
    return [Check("Without edge exclusion: edge rows with columns of single spectra",
                  r["margin"] == 0 and r["bad_entries"] > 0, f"{r['bad_entries']:g} entries")]


def mut_edges_off(w):
    lines = (w / "SB2_conf.sps").read_text().splitlines()
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    lines.append("checkBox_40=0")
    (w / "SB2_conf.sps").write_text("\n".join(lines) + "\n")


# ---- weighting by S/N per spectrum

SNR_PER_SPECTRUM = [300, 20, 150, 40, 300, 25, 200, 60, 100, 30]


def _set_weighting(w, on, snr_file="snr.dat"):
    lines = (w / "SB2_conf.sps").read_text().splitlines()
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    lines = [l for l in lines if not l.startswith(("checkBox_41=", "lineEdit_29="))]
    lines += [f"checkBox_41={1 if on else 0}", f"lineEdit_29={snr_file}"]
    (w / "SB2_conf.sps").write_text("\n".join(lines) + "\n")


def mut_weights_exact(w):
    """Exact data, weighting on with very different S/N: the exact solution must not change."""
    np.savetxt(w / "snr.dat", SNR_PER_SPECTRUM, fmt="%g")
    _set_weighting(w, True)


def mut_weights_noisy(w):
    """Noise per spectrum sigma_j = 1/(S/N)_j between 20 and 300; weighting on."""
    rng = np.random.default_rng(5)
    for j, p in enumerate(sorted(w.glob("spectram_*.txt"), key=lambda q: int(q.stem.split("_")[1]))):
        d = np.loadtxt(p)
        d[:, 1] += rng.normal(0, 1.0 / SNR_PER_SPECTRUM[j], len(d))
        np.savetxt(p, d, fmt="%.17g", delimiter="\t")
    np.savetxt(w / "snr.dat", SNR_PER_SPECTRUM, fmt="%g")
    _set_weighting(w, True)


def mut_weights_bad_count(w):
    np.savetxt(w / "snr.dat", SNR_PER_SPECTRUM[:-1], fmt="%g")
    _set_weighting(w, True)


def mut_weights_bad_value(w):
    np.savetxt(w / "snr.dat", [0] + SNR_PER_SPECTRUM[1:], fmt="%g")
    _set_weighting(w, True)


def checks_weights_noisy(workdir, outdir, args):
    """Weighted components closer to the truth than unweighted; S/N estimate per spectrum."""
    def rms(od):
        return [component_match(od / f, workdir / t)[1] for f, t in (("SB2_1.txt", "truth_A.txt"),
                                                                      ("SB2_2.txt", "truth_B.txt"))]
    rw = rms(outdir)
    # same data unweighted
    w2 = outdir.parent / "work_unweighted"
    o2 = outdir.parent / "out_unweighted"
    if w2.exists():
        shutil.rmtree(w2)
    shutil.copytree(workdir, w2)
    _set_weighting(w2, False)
    conf = w2 / "SB2_conf.sps"
    conf.write_text(conf.read_text().replace(str(workdir), str(w2)))
    run_adapter("solve", w2, o2, args.timeout)
    ru = rms(o2)
    truth = np.array(SNR_PER_SPECTRUM, dtype=float)
    rel_w = np.loadtxt(outdir / "snr_estimated.dat") / truth - 1
    # recommended procedure without known S/N: estimate from the unweighted solution,
    # solve weighted with it, estimate again
    shutil.copy(o2 / "snr_estimated.dat", w2 / "snr_iter.dat")
    _set_weighting(w2, True, "snr_iter.dat")
    o3 = outdir.parent / "out_iterated"
    run_adapter("solve", w2, o3, args.timeout)
    rel_i = np.loadtxt(o3 / "snr_estimated.dat") / truth - 1
    rel_u = np.loadtxt(o2 / "snr_estimated.dat") / truth - 1
    # "auto": the program iterates itself
    _set_weighting(w2, True, "auto")
    o4 = outdir.parent / "out_auto"
    run_adapter("solve", w2, o4, args.timeout)
    rel_a = np.loadtxt(o4 / "snr_estimated.dat") / truth - 1
    ra = rms(o4)
    return [Check("Weighted closer to the truth than unweighted (A, B)", rw[0] < ru[0] and rw[1] < ru[1],
                  f"RMS weighted {rw[0]:.4f}/{rw[1]:.4f}, unweighted {ru[0]:.4f}/{ru[1]:.4f}"),
            Check("S/N estimate from the weighted solution (within 25 %)", np.abs(rel_w).max() < 0.25,
                  " ".join(f"{v:+.0%}" for v in rel_w)),
            Check("One iteration improves the estimate from the unweighted solution",
                  np.abs(rel_i).max() < np.abs(rel_u).max(),
                  f"max. {np.abs(rel_u).max():.0%} -> {np.abs(rel_i).max():.0%}"),
            Check("S/N 'auto' (3 iterations in the program) within 25 %", np.abs(rel_a).max() < 0.25,
                  " ".join(f"{v:+.0%}" for v in rel_a)),
            Check("Components with 'auto' almost as good as with true weights", ra[0] < 1.2 * rw[0] and ra[1] < 1.2 * rw[1],
                  f"RMS auto {ra[0]:.4f}/{ra[1]:.4f}, true weights {rw[0]:.4f}/{rw[1]:.4f}")]


def mut_collapsed_simplex(w):
    """Final simplex after full convergence: all vertices at the same point (true orbit)."""
    mut_orbit_fit(w)
    truth = np.loadtxt(w / "truth_orbit.txt")
    x = [truth[i] for i in (1, 2, 3, 6)]          # free elements: e, KA, KB, omega
    (w / "optval_SB2.txt").write_text("1\n" * 5)
    (w / "optmat_SB2.txt").write_text("".join(f"{v:.15g}\n" for _ in range(5) for v in x))


def checks_error_collapsed(workdir, outdir):
    # The simplex lies on the true orbit, not at the minimum of the residual (on the
    # coarse grid the interpolation shifts the minimum, e.g. K_A 38.9 instead of 40).
    # Hence we only check that the errors are finite and of the same size as the
    # regular error estimate (there K_A 0.053, K_B 0.074, e 0.00083, omega 0.0013).
    ref = {"K_A": 0.053, "K_B": 0.074, "e": 0.00083, "omega": 0.0013}
    out = []
    truth = np.loadtxt(workdir / "truth_orbit.txt")
    for line in (outdir / "errors.dat").read_text().splitlines():
        parts = line.split("\t")
        if len(parts) == 4 and not line.startswith("#"):
            err = float(parts[2])
            out.append(Check(f"Standard error {parts[0]} of the expected size",
                             math.isfinite(err) and 0.5 < err / ref[parts[0]] < 2,
                             f"{err:.2g} (regular {ref[parts[0]]:.2g})"))
    head = [l for l in (outdir / "errors.dat").read_text().splitlines() if "new simplex" in l]
    out.append(Check("New simplex instead of the degenerate one", len(head) == 1, head[0] if head else ""))
    for line in (outdir / "errors.dat").read_text().splitlines():
        parts = line.split("\t")
        if len(parts) == 4 and not line.startswith("#"):
            i = {"e": 1, "K_A": 2, "K_B": 3, "omega": 6}[parts[0]]
            out.append(Check(f"Best value {parts[0]} is the simplex point",
                             abs(float(parts[1]) - truth[i]) < 1e-9 * max(1, abs(truth[i])), parts[1]))
    return out


def mut_old_sequence(w):
    """Configuration of an earlier version with the removed sequence mode: its settings are
    ignored without an error message."""
    conf = w / "SB2_conf.sps"
    lines = conf.read_text().splitlines()
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    conf.write_text("\n".join(lines + ["checkBox_27=1", "spinBox_9=2", "spinBox_10=5"]) + "\n")


# name: (driver case, data set, modification, expected error message or
#        reference case, reference files)
VARIANTS = {
    "config_old_sequence": ("config", "bin4", mut_old_sequence, None, "config", ["config.txt"]),
    "read_blank_lines": ("read_data", "bin4", mut_blank_lines, None, "read_data",
                         ["C.txt", "W.txt", "RV1.txt", "RV3.txt", "M.txt"]),
    "read_short_spectrum": ("read_data", "bin4", mut_short_spectrum, None, None, []),
    "read_missing_velocities": ("read_data", "bin4", mut_missing_velocities, "Error 2", None, []),
    "read_short_velocities": ("read_data", "bin4", mut_short_velocities, "Error 3", None, []),
    "read_bad_range": ("read_data", "bin4", mut_bad_range, "Error 30", None, []),
    "optimisation_converges": ("optimisation", "synthetic_subpixel", mut_perturb_rvs, None, None, []),
    "autostop": ("optimisation", "synthetic_subpixel", mut_autostop, None, None, []),
    "autostop_reinit": ("optimisation", "synthetic_subpixel", mut_autostop_reinit, None, None, []),
    "reinit_no_autostop": ("optimisation", "synthetic_subpixel", mut_reinit_no_autostop, None, None, []),
    "autostop_snr": ("optimisation", "synthetic_subpixel", mut_autostop_snr, None, None, []),
    # SVD (DaC) instead of LSQR: same results as the LSQR reference
    "divide_conquer_svd": ("divide_conquer", "bin4", lambda w: None, None, "divide_conquer",
                           ["result.txt", "X.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "svd_check_svd": ("svd_check", "bin4", lambda w: None, None, None, []),
    "orbit_fit": ("optimisation", "synthetic_orbit", mut_orbit_fit, None, None, []),
    "error_estimation": ("error_estimation", "synthetic_orbit", mut_orbit_fit, None, None, []),
    "linear_grid_resample": ("read_data", "bin4", mut_linear_grid, None, None, []),
    "linear_grid_auto": ("read_data", "bin4", lambda w: mut_linear_grid(w, False), None, None, []),
    "orbit_fit_lm": ("optimisation", "synthetic_orbit", mut_orbit_fit_lm, None, None, []),
    "orbit_fit_lm_cubic": ("optimisation", "synthetic_orbit", mut_orbit_fit_lm_cubic, None, None, []),
    "orbit_fit_cubic": ("optimisation", "synthetic_orbit", mut_orbit_fit_cubic, None, None, []),
    "orbit_fit_reinit": ("optimisation", "synthetic_orbit", mut_orbit_fit_reinit, None, None, []),
    # wavelength-dependent flux ratio: constant shapes reproduce the constant ratio (the
    # solution vector X differs, it holds the diluted spectra; compared are the outputs)
    "ratio_const": ("divide_conquer", "bin4", mut_ratio_const, None, "ratio_const",
                    ["result.txt", "X.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_planck_equal": ("divide_conquer", "bin4", mut_ratio_planck_equal, None, "ratio_const",
                           ["result.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_table_const": ("divide_conquer", "bin4", mut_ratio_table_const, None, "ratio_const",
                          ["result.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_const_static": ("divide_conquer", "synthetic", mut_ratio_const_static, None, "ratio_const_static", ["result.txt", "X.txt", "SB2_1.txt", "SB2_2.txt", "SB2_tell.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_planck_equal_static": ("divide_conquer", "synthetic", mut_ratio_planck_equal_static, None,
                                  "ratio_const_static", ["result.txt", "SB2_1.txt", "SB2_2.txt", "SB2_tell.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_const_sb1": ("divide_conquer", "synthetic", mut_ratio_const_sb1, None, "ratio_const_sb1", ["result.txt", "X.txt", "SB2_1.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_planck_equal_sb1": ("divide_conquer", "synthetic", mut_ratio_planck_equal_sb1, None,
                               "ratio_const_sb1", ["result.txt", "SB2_1.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "ratio_table_missing": ("read_data", "bin4", mut_ratio_table_missing, "Error 44", None, []),
    "ratio_eclipse_planck": ("divide_conquer", "synthetic", mut_ratio_eclipse_planck, None, None, []),
    "ratio_eclipse_table": ("divide_conquer", "synthetic", mut_ratio_eclipse_table, None, None, []),
    "ratio_eclipse_constant": ("divide_conquer", "synthetic", mut_ratio_eclipse_constant, None, None, []),
    "ratio_plotter": ("plotter_ratio", "synthetic", mut_ratio_plotter, None, None, []),
    "fixed_rvs": ("optimisation", "synthetic_subpixel", mut_fixed_rvs, None, None, []),
    "fixed_rvs_lm": ("optimisation", "synthetic_subpixel", lambda w: mut_fixed_rvs(w, True), None, None, []),
    "fixed_rvs_all": ("optimisation", "synthetic_subpixel",
                      lambda w: (mut_perturb_rvs(w), _conf_v2(w, lineEdit_fixA="0-9", lineEdit_fixB="0-9")),
                      "Error 45", None, []),
    "fixed_rvs_invalid": ("optimisation", "synthetic_subpixel",
                          lambda w: (mut_perturb_rvs(w), _conf_v2(w, lineEdit_fixA="0, 12")), "Error 45", None, []),
    "output_name_missing": ("solve", "synthetic", lambda w: _conf_set(w, 18, ""), "Error 46", None, []),
    "error_mc": ("error_mc", "synthetic_orbit", mut_error_mc, None, None, []),
    "orbit_fit_rvs": ("orbit_fit_rvs", "synthetic_orbit", mut_orbit_fit_rvs, None, None, []),
    "tellcheck": ("tellcheck", "synthetic_orbit", lambda w: _tell_dataset(w, False), None, None, []),
    "tellcheck_corrected": ("tellcheck", "synthetic_orbit", lambda w: _tell_dataset(w, True), None, None, []),
    "tellcheck_reject": ("tellcheck", "synthetic_orbit", lambda w: _tell_dataset(w, True), None, None, []),
    "tellcheck_noband": ("tellcheck", "synthetic_orbit", mut_barycorr_night, "no strong telluric band", None, []),
    "flux_templates": ("flux_templates", "synthetic", mut_flux_templates, None, None, []),
    "barycorr": ("barycorr", "synthetic_orbit", mut_barycorr, None, None, []),
    "barycorr_file": ("barycorr_file", "synthetic_orbit", mut_barycorr_night, None, None, []),
    "barycorr_file_corrected": ("barycorr_file", "synthetic_orbit", mut_barycorr_night, None, None, []),
    "residual_stats": ("residual_stats", "synthetic_orbit", mut_residual_stats, None, None, []),
    "residual_stats_rv": ("residual_stats", "synthetic", lambda w: None, None, None, []),
    "residual_stats_model": ("residual_stats", "synthetic_orbit", mut_residual_stats_model, None, None, []),
    "residual_stats_named": ("residual_stats", "synthetic_orbit", mut_residual_stats_named, None, None, []),
    "custom_file_names": ("optimisation", "synthetic_orbit", mut_custom_names, None, None, []),
    "lm_rv_static": ("optimisation", "synthetic", mut_lm_rv_static, None, None, []),
    "bc_gamma_free": ("optimisation", "synthetic_orbit", mut_bc_gamma_free, None, None, []),
    "bc_tell_nofit": ("optimisation", "synthetic_orbit", mut_bc_tell_nofit, None, None, []),
    "bc_tell_fit": ("optimisation", "synthetic_orbit", mut_bc_tell_fit, None, None, []),
    "bc_tell_file": ("optimisation", "synthetic_orbit", mut_bc_tell_file, None, None, []),
    "bc_tell_misaligned": ("solve", "synthetic_orbit", mut_bc_tell_misaligned, None, None, []),
    "res_static_excl": ("solve", "synthetic_orbit", mut_res_static, None, None, []),
    "res_static_noexcl": ("solve", "synthetic_orbit", lambda w: mut_res_static(w, exclude=False), None, None, []),
    "res_static_excl_file": ("solve", "synthetic_orbit", lambda w: mut_res_static(w, ranges=False), None, None, []),
    "res_static_excl_missing": ("solve", "synthetic_orbit",
                                lambda w: (mut_res_static(w), _conf_v2(w, lineEdit_tellrange="nofile.dat")), "Error 50", None, []),
    "bc_tell_file_count": ("optimisation", "synthetic_orbit", mut_bc_tell_file_count, "Error 49", None, []),
    "bc_tell_groups": ("optimisation", "synthetic_orbit", mut_bc_tell_groups, None, None, []),
    "bc_tell_groups_single": ("optimisation", "synthetic_orbit", mut_bc_tell_groups_single, None, None, []),
    "static_long_damp": ("solve", "synthetic_orbit", lambda w: _long_static_dataset(w, 0.001), None, None, []),
    "static_ranges": ("detect_static", "synthetic_orbit", lambda w: _long_static_dataset(w, 0.001), None, None, []),
    "static_long_nodamp": ("solve", "synthetic_orbit", lambda w: _long_static_dataset(w, 0), None, None, []),
    "bc_obs_static": ("optimisation", "synthetic_orbit", mut_bc_obs_static, None, None, []),
    "bc_obs_static_nobc": ("optimisation", "synthetic_orbit", mut_bc_obs_static_nobc, None, None, []),
    "bc_corrected_static": ("optimisation", "synthetic_orbit", mut_bc_corrected_static, None, None, []),
    "bc_obs_nostatic": ("optimisation", "synthetic_orbit", mut_bc_obs_nostatic, None, None, []),
    "bc_corrected_static_nofile": ("optimisation", "synthetic_orbit", mut_bc_corrected_static_nofile, "Error 48", None, []),
    "bc_wrong_count": ("optimisation", "synthetic_orbit", mut_bc_wrong_count, "Error 48", None, []),
    "ratio_eclipse_optratios": ("opt_ratios", "synthetic", mut_ratio_eclipse_optratios, None, None, []),
    "error_estimation_lm": ("error_estimation", "synthetic_orbit", mut_orbit_fit_lm_cubic, None, None, []),
    "optimisation_converges_svd": ("optimisation", "synthetic_subpixel", mut_perturb_rvs, None, None, []),
    "optimisation_converges_weighted": ("optimisation", "synthetic_subpixel", mut_perturb_rvs_weighted, None, None, []),
    "optimisation_converges_lm": ("optimisation", "synthetic_subpixel", mut_perturb_rvs_lm, None, None, []),
    "lm_reinit": ("optimisation", "synthetic_subpixel", mut_lm_reinit, None, None, []),
    "cores_warning": ("cores_warning", "bin4", lambda w: None, None, None, []),
    "mask_tellurics": ("solve", "synthetic", mut_mask_tellurics, None, None, []),
    "continuum_norm": ("solve", "synthetic", mut_continuum_norm, None, None, []),
    "align_continua": ("solve", "synthetic", mut_align_continua, None, None, []),
    "mask_optimisation": ("optimisation", "synthetic", mut_mask_optimisation, None, None, []),
    "abort_long_spectra": ("optimisation_abort", "synthetic", mut_abort_long, None, None, []),
    "abort_long_spectra_solve": ("solve_abort", "synthetic", mut_abort_long, None, None, []),
    "continue_method_switch": ("optimisation", "synthetic_subpixel", mut_continue_switch, None, None, []),
    "continue_method_switch_orbit": ("optimisation", "synthetic_orbit", mut_orbit_fit, None, None, []),
    "error_collapsed": ("error_only", "synthetic_orbit", mut_collapsed_simplex, None, None, []),
    "weights_exact": ("divide_conquer", "synthetic", mut_weights_exact, None, None, []),
    "weights_noisy": ("solve", "synthetic", mut_weights_noisy, None, None, []),
    "weights_bad_count": ("read_data", "synthetic", mut_weights_bad_count, "Error 40", None, []),
    "weights_bad_value": ("read_data", "synthetic", mut_weights_bad_value, "Error 40", None, []),
    "gui_smoke": ("gui_smoke", "bin8_n10", lambda w: None, None, None, []),
    "edge_coverage": ("edge_coverage", "bin4", lambda w: None, None, None, []),
    "file_menu": ("file_menu", "bin4", lambda w: None, None, None, []),
    "cores": ("cores", "bin4", lambda w: None, None, None, []),
    "edge_coverage_orbit": ("edge_coverage", "synthetic_orbit", lambda w: None, None, None, []),
    "edge_coverage_off": ("edge_coverage", "bin4", mut_edges_off, None, None, []),
    "fits_read": ("read_data", "bin4", mut_fits, None, "read_data",
                  ["C.txt", "W.txt", "RV1.txt", "RV3.txt", "M.txt"]),
    "fits_solve": ("divide_conquer", "bin4", mut_fits, None, "divide_conquer",
                   ["result.txt", "X.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt", "SB2_error.txt"]),
    "fits_float32": ("read_data", "bin4", mut_fits_float32, None, None, []),
    "fits_other_extension": ("read_data", "bin4", mut_fits_other_ext, None, "read_data", ["C.txt", "W.txt"]),
    "fits_empty_extension": ("read_data", "bin4", mut_fits_empty_ext, None, "read_data", ["C.txt", "W.txt"]),
    "fits_uppercase": ("read_data", "bin4", mut_fits_uppercase, None, "read_data", ["C.txt", "W.txt"]),
    "fits_extra_hdus": ("read_data", "bin4", mut_fits_extra_hdus, None, "read_data", ["C.txt", "W.txt"]),
    "fits_no_columns": ("read_data", "bin4", mut_fits_no_columns, "Error 32", None, []),
    "branch_ratio": ("divide_conquer", "synthetic", mut_ratio, None, None, []),
    "opt_ratios": ("opt_ratios", "synthetic", mut_opt_ratios, None, None, []),
    "opt_ratios_autostop": ("opt_ratios", "synthetic", mut_opt_ratios_autostop, None, None, []),
    "branch_static": ("divide_conquer", "synthetic", mut_static, None, None, []),
    "branch_edit_weights": ("divide_conquer", "synthetic", mut_edit_weights, None, None, []),
    "branch_sb1": ("divide_conquer", "synthetic", mut_synth_sb1, None, None, []),
    "branch_few_spectra": ("divide_conquer", "synthetic", mut_few_spectra, None, None, []),
    "branch_static_svd": ("divide_conquer", "synthetic", mut_static, None, None, []),
    "sb1": ("divide_conquer", "bin4", mut_sb1, None, "sb1",
            ["result.txt", "X.txt", "SB2_1.txt", "SB2_2.txt", "SB2_diff.txt"]),
}


SLOW_VARIANTS = {"opt_ratios", "autostop_reinit", "autostop_snr", "ratio_eclipse_optratios",
                 "static_long_damp", "static_long_nodamp", "static_ranges"}   # opt_ratios_autostop: ~8 s, fast suite   # only with --slow (flux ratios: 3000 iterations, ~40 s)


def run_variant(name, args, tmp_root):
    case, dataset, mutate, expect_err, golden_case, files = VARIANTS[name]
    workdir, outdir = tmp_root / name / "work", tmp_root / name / "out"
    prepare(dataset, workdir)
    mutate(workdir)
    checks = []
    try:
        env = {"SPECTEST_SOLVER": "svd"} if name.endswith("_svd") else \
            ({"SPECTANGULAR_CORES": "8"} if name == "cores_warning" else
             ({"SPECTEST_BC_CORRECTED": "1"} if name == "barycorr_file_corrected" else
              TELL_ENV.get(name)))
        secs = run_adapter(case, workdir, outdir, args.timeout, env)
        if expect_err:
            checks.append(Check(f"Message '{expect_err}'", False, "no error message"))
        elif name in ("autostop", "autostop_reinit", "reinit_no_autostop", "autostop_snr"):
            checks += checks_autostop(name, workdir, outdir)
        elif name in ("optimisation_converges", "optimisation_converges_svd"):
            checks += checks_converges(workdir, outdir)
        elif name == "optimisation_converges_weighted":
            out = (outdir / "stdout.log").read_text()
            checks.append(Check("S/N per spectrum estimated (auto)", "S/N per spectrum estimated" in out, ""))
            checks += checks_converges(workdir, outdir)
        elif name == "svd_check_svd":
            checks += checks_svd(outdir, False)
        elif name == "read_short_spectrum":
            # one spectrum 3 pixels shorter on the same grid: common range, values unchanged
            W, C = np.loadtxt(outdir / "W.txt"), np.loadtxt(outdir / "C.txt")
            orig = np.loadtxt(DATA / "bin4" / "spectram_0.txt")
            n = len(orig) - 3
            ok = len(W) == 20 * n and np.abs(W[:n] - orig[:n, 0]).max() < 1e-10 \
                and np.abs(C[:n] - orig[:n, 1]).max() < 1e-6
            checks.append(Check("Shorter spectrum: cut to the common range, values unchanged", ok,
                                f"{len(W) // 20} pixels per spectrum"))
        elif name == "linear_grid_resample":
            checks += checks_linear_grid(workdir, outdir)
        elif name == "linear_grid_auto":
            out = (outdir / "stdout.log").read_text()
            checks.append(Check("Without the option: resampled automatically, with the reason",
                                "not on one common log10(lambda) grid" in out, ""))
            checks += checks_linear_grid(workdir, outdir)
        elif name in ("orbit_fit", "orbit_fit_cubic"):
            checks += checks_orbit_fit(workdir, outdir)
        elif name == "ratio_eclipse_optratios":
            checks += checks_ratio_optratios(workdir, outdir)
        elif name in ("fixed_rvs", "fixed_rvs_lm"):
            checks += checks_fixed_rvs(workdir, outdir)
        elif name == "error_mc":
            checks += checks_error_mc(workdir, outdir)
        elif name == "orbit_fit_rvs":
            checks += checks_orbit_fit_rvs(workdir, outdir)
        elif name == "flux_templates":
            checks += checks_flux_templates(workdir, outdir)
        elif name in ("barycorr_file", "barycorr_file_corrected"):
            checks += checks_barycorr_file(name, workdir, outdir)
        elif name == "barycorr":
            checks += checks_barycorr(workdir, outdir)
        elif name.startswith("tellcheck"):
            checks += checks_tellcheck(name, workdir, outdir)
        elif name in ("residual_stats", "residual_stats_model"):
            checks += checks_residual_stats(workdir, outdir)
        elif name == "residual_stats_rv":
            st = np.loadtxt(outdir / "diffstatistic.dat")
            head = (outdir / "diffstatistic.dat").read_text().splitlines()[1]
            t = np.loadtxt(workdir / "times.dat")
            checks.append(Check("Residual statistics against the times of observation (RV mode)",
                                "time" in head and np.abs(st[:, 1] - t).max() < 1e-6 * np.abs(t).max(),
                                f"{len(st)} spectra; residual RMS max. {st[:, 4].max():.1e}"))
        elif name == "ratio_plotter":
            checks += checks_ratio_plotter(workdir, outdir)
        elif name.startswith("ratio_eclipse"):
            checks += checks_ratio_eclipse(name, workdir, outdir)
        elif name == "orbit_fit_reinit":
            checks += checks_reinit_restart(workdir) + checks_orbit_fit(workdir, outdir)
        elif name in ("orbit_fit_lm", "orbit_fit_lm_cubic"):
            checks += checks_lm_log(workdir, outdir) + checks_orbit_fit(workdir, outdir)
        elif name == "optimisation_converges_lm":
            checks += checks_lm_log(workdir, outdir) + checks_converges(workdir, outdir)
        elif name == "mask_optimisation":
            checks += checks_mask_optimisation(workdir, outdir)
        elif name == "align_continua":
            checks += checks_align_continua(case, workdir, tmp_root / name, args.timeout)
        elif name == "continuum_norm":
            checks += checks_continuum_norm(workdir, outdir)
        elif name == "mask_tellurics":
            checks += checks_mask_tellurics(case, workdir, tmp_root / name, args.timeout)
        elif name == "cores_warning":
            r = {l.split()[0]: float(l.split()[1]) for l in (outdir / "result.txt").read_text().splitlines() if l.strip()}
            note = (outdir / "note.txt").read_text().strip()
            checks += [Check("Default: 2 cores (8 available)", r["default"] == 2 and r["max"] == 8, f"{r['default']:.0f} of {r['max']:.0f}"),
                       Check("Note when the user chooses more than 2 cores, once per crossing; not when set by the program",
                             r["focus"] == 1 and r["modals_program"] == 0 and r["modals_first"] == 1 and r["modals_second"] == 2,
                             f"program {r['modals_program']:.0f}, user 4+5 cores {r['modals_first']:.0f}, again 6 cores {r['modals_second']:.0f}; {note[:70]}")]
        elif name == "abort_long_spectra":
            checks += checks_abort_long(outdir)
        elif name == "abort_long_spectra_solve":
            checks += checks_abort_long(outdir, solve=True)
        elif name == "lm_reinit":
            checks += checks_lm_reinit(workdir, outdir) + checks_lm_log(workdir, outdir) + checks_converges(workdir, outdir)
        elif name == "lm_rv_static":
            checks += checks_lm_rv_static(workdir, outdir)
        elif name == "bc_gamma_free":
            log = (workdir / "SB2_log.txt").read_text()
            el = np.loadtxt(workdir / "orbitelements.dat")
            checks.append(Check("Orbit mode with gamma free: note that gamma is not determined",
                                "the disentangling does not determine it" in log
                                and "the disentangling does not determine it" in (outdir / "stdout.log").read_text(),
                                f"gamma {BC_START[4]} -> {el[4]:.3f} (true {BC_TRUTH[4]})"))
        elif name == "static_ranges":
            checks += checks_static_ranges(case, workdir, tmp_root / name, args.timeout)
        elif name in ("static_long_damp", "static_long_nodamp"):
            checks += checks_static_long(name, outdir)
        elif name in ("bc_tell_groups", "bc_tell_groups_single"):
            checks += checks_bc_groups(name, workdir, outdir)
        elif name.startswith("res_static"):
            checks += checks_res_static(name, workdir, outdir)
        elif name == "bc_tell_misaligned":
            checks += checks_bc_tell_misaligned(workdir, outdir)
        elif name in ("bc_tell_nofit", "bc_tell_fit", "bc_tell_file"):
            checks += checks_bc_tell(name, workdir, outdir)
        elif name.startswith("bc_"):
            checks += checks_bc(name, workdir, outdir)
        elif name == "custom_file_names":
            checks += checks_custom_names(case, workdir, tmp_root / name, args.timeout)
        elif name == "residual_stats_named":
            checks += [Check("Residual statistics written to the name of the Output tab (stat_A.dat), no diffstatistic.dat",
                             (outdir / "stat_A.dat").exists() and (workdir / "stat_A.dat").exists()
                             and not (workdir / "diffstatistic.dat").exists(), "")]
        elif name in ("continue_method_switch", "continue_method_switch_orbit"):
            checks += checks_continue_switch(case, workdir, tmp_root / name, args.timeout, name.endswith("_orbit"))
        elif name in ("error_estimation", "error_estimation_lm"):
            checks += checks_error_estimation(workdir, outdir)
        elif name == "error_collapsed":
            checks += checks_error_collapsed(workdir, outdir)
        elif name == "cores":
            r = scalars(outdir / "result.txt")
            checks += [Check("Field 'Use' (cores) is usable", r["enabled"] == 1, ""),
                       Check("'Use' sets the number of cores", r["threads_1"] == 1 and r["threads_max"] == r["max"],
                             f"1 -> {r['threads_1']:g}, {r['max']:g} -> {r['threads_max']:g}")]
        elif name == "file_menu":
            r = scalars(outdir / "result.txt")
            checks += [Check("Save As writes <name>.sps", r["saved_as"] == 1, ""),
                       Check("Save writes to the same file without dialog, Open loads it",
                             r["iterations_reloaded"] == r["iterations_expected"] and r["save_no_dialog"] == 1,
                             f"iterations {r['iterations_reloaded']:g}, expected {r['iterations_expected']:g}"),
                       Check("Open: Work Path = folder of the configuration", r["workpath_from_file"] == 1, ""),
                       Check("Window title shows the file", r["title"] == 1, "")]
        elif name.startswith("edge_coverage"):
            checks += checks_edge_coverage(outdir, not name.endswith("_off"))
        elif name == "weights_exact":
            checks += checks_branch(workdir, outdir, AB)
        elif name == "weights_noisy":
            checks += checks_weights_noisy(workdir, outdir, args)
        elif name == "gui_smoke":
            checks += checks_gui_smoke(outdir)
        elif name == "fits_float32":
            checks += checks_fits_float32(outdir)
        elif name == "opt_ratios_autostop":
            checks += checks_opt_ratios_autostop(workdir, outdir)
        elif name == "opt_ratios":
            checks += checks_opt_ratios(workdir, outdir)
        elif name.startswith("branch_"):
            comps = {"branch_sb1": AB[:1], "branch_static": AB + [("T", "SB2_tell.txt", "truth_T.txt")],
                     "branch_static_svd": AB + [("T", "SB2_tell.txt", "truth_T.txt")]}.get(name, AB)
            checks += checks_branch(workdir, outdir, comps)
        else:
            # noise-free synthetic data: residuum at the rounding level (~1e-10, depends on the
            # summation order), as in the case "synthetic"
            atol = 1e-9 if dataset == "synthetic" else 1e-10
            if args.record and golden_case == name:
                checks += golden_checks(name, outdir, files, 1e-8, atol, True)
            else:
                checks += golden_checks(golden_case, outdir, files, 1e-8, atol, False)
        head = f"{name}  ({secs:.1f} s)"
    except Exception as exc:  # noqa: BLE001
        msg = str(exc)
        if expect_err:
            checks.append(Check(f"Message '{expect_err}'", expect_err in msg, msg[:140]))
        else:
            checks.append(Check("Run", False, msg))
        head = name
    return head, checks


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("cases", nargs="*")
    ap.add_argument("--record", action="store_true", help="re-record the references")
    ap.add_argument("--slow", action="store_true", help="include slow cases")
    ap.add_argument("--keep", action="store_true", help="keep the work directory")
    ap.add_argument("--timeout", type=int, default=600)
    args = ap.parse_args()

    names = args.cases or (["unit"] + [n for n, c in CASES.items() if args.slow or not c[6]]
                           + [n for n in VARIANTS if args.slow or n not in SLOW_VARIANTS] + ["user_errors", "batch"])
    if not SPECTEST.exists():
        sys.exit(f"Test driver missing: {SPECTEST}")
    tmp_root = Path(tempfile.mkdtemp(prefix="spectest_"))
    counts = {}
    for name in names:
        if name == "batch":
            print("batch")
            for c in run_batch_checks(tmp_root, args.timeout):
                counts[c.status] = counts.get(c.status, 0) + 1
                print(f"  {c.status:5}  {c.name}: {c.detail}")
            continue
        if name == "user_errors":
            print("user_errors")
            for c in run_user_error_checks(tmp_root, args.timeout):
                counts[c.status] = counts.get(c.status, 0) + 1
                print(f"  {c.status:5}  {c.name}: {c.detail}")
            continue
        if name == "unit":
            print("unit")
            p = subprocess.run([str(UNITTESTS)], capture_output=True, text=True, timeout=args.timeout)
            lines = [l for l in p.stdout.splitlines() if l.startswith(("PASS", "FAIL"))]
            for l in lines:
                st, rest = l.split(None, 1)
                counts[st] = counts.get(st, 0) + 1
                print(f"  {st:5}  {rest}")
            if not lines or p.returncode not in (0, 1):
                counts["FAIL"] = counts.get("FAIL", 0) + 1
                print(f"  FAIL   Unit tests: exit code {p.returncode} {p.stderr[-200:]}")
            continue
        if name in VARIANTS:
            head, checks = run_variant(name, args, tmp_root)
            print(head)
            for c in checks:
                counts[c.status] = counts.get(c.status, 0) + 1
                print(f"  {c.status:5}  {c.name}: {c.detail}")
            continue
        case, dataset, files, rtol, atol, soll, _ = CASES[name]
        wd_name = "work dir" if name == "config_path_space" else "work"
        workdir, outdir = tmp_root / name / wd_name, tmp_root / name / "out"
        prepare(dataset, workdir)
        checks = []
        try:
            secs = run_adapter(case, workdir, outdir, args.timeout)
            checks += golden_checks(name, outdir, files, rtol, atol, args.record) if files else []
            if soll:
                checks += soll(workdir, outdir)
            if not checks:
                checks.append(Check("Run without errors", True))
            head = f"{name}  ({secs:.1f} s)"
        except Exception as exc:  # noqa: BLE001
            checks.append(Check("Run", False, str(exc)))
            head = name
        print(head)
        for c in checks:
            counts[c.status] = counts.get(c.status, 0) + 1
            extra = f"  [{c.xfail}]" if c.xfail else ""
            print(f"  {c.status:5}  {c.name}: {c.detail}{extra}")
    if not args.keep:
        shutil.rmtree(tmp_root)
    else:
        print(f"Work directory: {tmp_root}")
    print("Result: " +", ".join(f"{k} {v}" for k, v in sorted(counts.items())))
    sys.exit(1 if counts.get("FAIL") or counts.get("XPASS") else 0)


if __name__ == "__main__":
    main()
