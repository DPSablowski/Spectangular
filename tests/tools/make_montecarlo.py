#!/usr/bin/env python3
"""Monte Carlo data sets for the orbit fit with error estimation.

Known eccentric orbit on the full grid of the test data (20 spectra,
1417 pixels), component spectra from a disentangling of the real test data
(i.e. realistic line shapes). The observations are generated independently of
the program: Doppler shift with a cubic spline in log10(lambda), not with the
program's linear interpolation. Plus Gaussian noise with fixed S/N, a different
random stream for each realisation.

Free in the fit: P, e, K_A, K_B, T_peri, omega; gamma fixed (not determinable
by disentangling). Start is the same for all realisations and perturbed.

Usage:
  make_montecarlo.py <data_dir> <dir with SB2_1.txt/SB2_2.txt> <target> <count>
                     [--snr 65] [--iterations 600] [--seed 1]
data_dir: spectram_*.txt, times.dat, SB2_conf.sps (test data, full resolution).
"""
import argparse
import shutil
from pathlib import Path

import numpy as np
from scipy.interpolate import CubicSpline

C_KMS = 299792.458
#            P      e    KA    KB    gamma   T_peri      omega
TRUTH = [104.0, 0.3, 24.0, 30.0, 29.94, 2448180.0, 1.2]
START = [104.6, 0.25, 22.0, 32.0, 29.94, 2448181.5, 1.0]


def kepler_rv(el, t):
    P, e, ka, kb, gam, tp, om = el
    M = np.mod(2 * np.pi * (t - tp) / P, 2 * np.pi)
    E = M.copy()
    for _ in range(100):
        E = E - (E - e * np.sin(E) - M) / (1 - e * np.cos(E))
    nu = 2 * np.arctan2(np.sqrt(1 + e) * np.sin(E / 2), np.sqrt(1 - e) * np.cos(E / 2))
    return (gam + ka * (np.cos(nu + om) + e * np.cos(om)),
            gam + kb * (np.cos(nu + om + np.pi) + e * np.cos(om + np.pi)))


def template(path, pad=80):
    """Spline of the component spectrum; extended at both ends with continuum 1
    (smooth transition over 10 pixels) so that larger RVs are covered too."""
    d = np.loadtxt(path)
    lw = np.log10(d[:, 0])
    f = d[:, 1] - np.median(d[:, 1]) + 1.0     # continuum ~1 (offset is undetermined)
    taper = np.ones_like(f)
    ramp = 0.5 - 0.5 * np.cos(np.linspace(0, np.pi, 10))
    taper[:10], taper[-10:] = ramp, ramp[::-1]
    f = 1.0 + (f - 1.0) * taper
    dl = (lw[-1] - lw[0]) / (len(lw) - 1)
    lo = lw[0] - dl * np.arange(pad, 0, -1)
    hi = lw[-1] + dl * np.arange(1, pad + 1)
    lw = np.concatenate([lo, lw, hi])
    f = np.concatenate([np.ones(pad), f, np.ones(pad)])
    return CubicSpline(lw, f), lw[0], lw[-1]


def set_conf(src, dst, folder, iterations):
    lines = src.read_text().splitlines()
    head = lines[:58]
    head[2] = str(iterations)       # iterations
    head[17] = str(folder)          # data directory
    cb = 36
    head[cb + 0] = "0"              # checkBox: CroCo
    head[cb + 1] = "0"              # checkBox_2: RV file
    head[cb + 8] = "1"              # checkBox_9: new optimisation (New)
    head[cb + 10] = "0"             # checkBox_11: do not continue (Continue)
    head[cb + 13] = "1"             # checkBox_14: residual "Mean" (for the error estimation)
    head[cb + 15] = "1"             # checkBox_16: orbital elements
    head[cb + 16] = "0"             # checkBox_17: e free
    head[cb + 17] = "0"             # checkBox_18: P free
    head[cb + 18] = "0"             # checkBox_32: no Reinitiate
    head[cb + 21] = "0"             # checkBox_35: T free
    extra = ["# Spectangular settings v2", "solver=lsqr", "checkBox_31=0", "checkBox_36=0",
             "checkBox_37=1", "checkBox_38=0", "checkBox_23=0"]
    dst.write_text("\n".join(head + extra) + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("data")
    ap.add_argument("templates")
    ap.add_argument("dest")
    ap.add_argument("n", type=int)
    ap.add_argument("--snr", type=float, default=65.0)
    ap.add_argument("--iterations", type=int, default=600)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--linear", action="store_true",
                    help="shift as in the program: components on the pixel grid, "
                         "linearly interpolated (tests the error estimation without model error)")
    ap.add_argument("--window", type=int, default=0,
                    help="residual without the outer N pixels of each spectrum ('Compute Residuum from')")
    a = ap.parse_args()
    data, dest = Path(a.data), Path(a.dest)

    nspec = len(list(data.glob("spectram_*.txt")))
    w = np.loadtxt(data / "spectram_0.txt")[:, 0]          # log10(lambda)
    t = np.loadtxt(data / "times.dat")[:nspec]
    va, vb = kepler_rv(TRUTH, t)
    A, a0, a1 = template(Path(a.templates) / "SB2_1.txt")
    B, b0, b1 = template(Path(a.templates) / "SB2_2.txt")
    sa, sb = np.log10(1 + va / C_KMS), np.log10(1 + vb / C_KMS)
    assert (w[0] - sa).min() >= a0 and (w[-1] - sa).max() <= a1, "template A too short"
    assert (w[0] - sb).min() >= b0 and (w[-1] - sb).max() <= b1, "template B too short"
    if a.linear:
        dlog = (w[-1] - w[0]) / (len(w) - 1)
        k = np.arange(-120, len(w) + 120)
        grid = w[0] + k * dlog
        Ag, Bg = A(grid), B(grid)
        clean = [0.5 * np.interp(w - sa[j], grid, Ag) + 0.5 * np.interp(w - sb[j], grid, Bg)
                 for j in range(nspec)]
    else:
        clean = [0.5 * A(w - sa[j]) + 0.5 * B(w - sb[j]) for j in range(nspec)]

    dest.mkdir(parents=True, exist_ok=True)
    for r in range(a.n):
        d = dest / f"r{r:03d}"
        if d.exists():
            shutil.rmtree(d)
        d.mkdir()
        rng = np.random.default_rng([a.seed, r])
        for j in range(nspec):
            obs = clean[j] + (rng.normal(0, 1 / a.snr, len(w)) if a.snr > 0 else 0.0)
            np.savetxt(d / f"spectram_{j}.txt", np.column_stack([w, obs]), fmt="%.15g", delimiter="\t")
        np.savetxt(d / "times.dat", t, fmt="%.10f")
        np.savetxt(d / "velocities.txt", np.column_stack([va, vb]), fmt="%.12g", delimiter="\t")
        np.savetxt(d / "truth_orbit.txt", np.array(TRUTH), fmt="%.14g")
        np.savetxt(d / "orbitelements.dat", np.array(START), fmt="%.14g")
        set_conf(data / "SB2_conf.sps", d / "SB2_conf.sps", d.resolve(), a.iterations)
        if a.window:
            lam = 10 ** w
            l = (d / "SB2_conf.sps").read_text().splitlines()
            l[11] = f"{0.5 * (lam[a.window - 1] + lam[a.window]):.6f}"
            l[12] = f"{0.5 * (lam[-a.window - 1] + lam[-a.window]):.6f}"
            l[55] = "1"                     # checkBox_33: residual only within the range
            (d / "SB2_conf.sps").write_text("\n".join(l) + "\n")
    print(f"{a.n} realisations in {dest}: {nspec} x {len(w)} pixels, S/N {a.snr}, "
          f"RV A {va.min():.1f}..{va.max():.1f}, B {vb.min():.1f}..{vb.max():.1f} km/s")


if __name__ == "__main__":
    main()
