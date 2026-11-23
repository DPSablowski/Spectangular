#!/usr/bin/env python3
"""Synthetic SB2 data set with known solution.

Two component spectra made of Gaussian lines (continuum 1) on the grid of
bin8_n10. Each observation is 0.5*A + 0.5*B, both Doppler-shifted in a
physically correct way (positive RV = redshift = larger pixel index) by
whole pixels. Without noise, disentangling at the true RVs must yield a
residual of practically 0.

The RVs correspond exactly to the shift s: v = c (10^(s*dlog) - 1).

Second data set (<target_dir>_subpixel): shifts by fractions of a pixel
and broader lines, for testing the interpolation.

Usage:  make_synthetic.py <bin8_n10_dir> <target_dir>
"""
import shutil
import sys
from pathlib import Path

import numpy as np

C_KMS = 299792.458


def lines(rng, n, npix, smin=1.5, smax=3.0):
    return [(rng.uniform(25, npix - 25), rng.uniform(smin, smax), rng.uniform(0.05, 0.4))
            for _ in range(n)]


def spectrum(x, line_list):
    f = np.ones_like(x)
    for x0, s, d in line_list:
        f -= d * np.exp(-0.5 * ((x - x0) / s) ** 2)
    return f


ORBIT_TRUTH = [20.54, 0.3, 40.0, 45.0, 5.0, 2448150.0, 1.2]   # P e KA KB gamma T_peri omega


def kepler_rv(el, t):
    P, e, ka, kb, gam, tp, om = el
    M = np.mod(2 * np.pi * (t - tp) / P, 2 * np.pi)
    E = M.copy()
    for _ in range(100):
        E = E - (E - e * np.sin(E) - M) / (1 - e * np.cos(E))
    nu = 2 * np.arctan2(np.sqrt(1 + e) * np.sin(E / 2), np.sqrt(1 - e) * np.cos(E / 2))
    return (gam + ka * (np.cos(nu + om) + e * np.cos(om)),
            gam + kb * (np.cos(nu + om + np.pi) + e * np.cos(om + np.pi)))


def make_orbit(src, dst):
    """RVs from a known orbit; start elements perturbed, P, T and gamma fixed."""
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(src, dst)
    w = np.loadtxt(src / "spectram_0.txt")[:, 0]
    npix = len(w)
    dlog = (w[-1] - w[0]) / (npix - 1)
    t = np.loadtxt(src / "times.dat")
    va, vb = kepler_rv(ORBIT_TRUTH, t)
    sa = np.log10(1 + va / C_KMS) / dlog
    sb = np.log10(1 + vb / C_KMS) / dlog
    rng = np.random.default_rng(42)
    la, lb = lines(rng, 12, npix, 2.5, 4.0), lines(rng, 10, npix, 2.5, 4.0)
    x = np.arange(npix, dtype=float)
    for j in range(len(t)):
        obs = 0.5 * spectrum(x - sa[j], la) + 0.5 * spectrum(x - sb[j], lb)
        np.savetxt(dst / f"spectram_{j}.txt", np.column_stack([w, obs]), fmt="%.17g", delimiter="\t")
    np.savetxt(dst / "truth_orbit.txt", np.array(ORBIT_TRUTH), fmt="%.14g")
    start = list(ORBIT_TRUTH)
    start[1], start[2], start[3], start[6] = 0.25, 35.0, 50.0, 1.0
    np.savetxt(dst / "orbitelements.dat", np.array(start), fmt="%.14g")
    conf = dst / "SB2_conf.sps"
    lines_ = conf.read_text().splitlines()
    cb = 14 + 22
    lines_[cb + 0] = "0"     # checkBox: CroCo
    lines_[cb + 1] = "0"     # checkBox_2: RV file
    lines_[cb + 15] = "1"    # checkBox_16: orbital elements
    lines_[cb + 16] = "0"    # checkBox_17: e free
    lines_[cb + 17] = "1"    # checkBox_18: P fixed
    lines_[cb + 21] = "1"    # checkBox_35: T fixed
    lines_ += ["# Spectangular settings v2", "checkBox_31=0", "checkBox_36=0", "checkBox_37=1",
               "checkBox_38=0"]
    conf.write_text("\n".join(lines_) + "\n")
    print(f"{dst.name}: orbit {ORBIT_TRUTH}, RV A {np.round(va, 1).tolist()}")


def make_correlation(dst):
    """Spectrum = template shifted by +3 pixels, same grid."""
    dst.mkdir(parents=True, exist_ok=True)
    rng = np.random.default_rng(5)
    x = np.arange(300, dtype=float)
    w = 3.8 + x * 1e-5
    ll = lines(rng, 15, 300, 2.0, 3.5)
    np.savetxt(dst / "tmpl.txt", np.column_stack([w, spectrum(x, ll)]), fmt="%.17g")
    np.savetxt(dst / "spec.txt", np.column_stack([w, spectrum(x - 3.0, ll)]), fmt="%.17g")
    print(f"{dst.name}: shift +3 px")


def make(src, dst, subpixel):
    if dst.exists():
        shutil.rmtree(dst)
    shutil.copytree(src, dst)
    w = np.loadtxt(src / "spectram_0.txt")[:, 0]
    npix = len(w)
    dlog = (w[-1] - w[0]) / (npix - 1)
    dv = (10 ** dlog - 1) * C_KMS
    nspec = len(np.loadtxt(src / "times.dat"))

    rng = np.random.default_rng(20261029)
    phase = 2 * np.pi * np.arange(nspec) / nspec
    if subpixel:
        la, lb = lines(rng, 12, npix, 2.5, 4.0), lines(rng, 10, npix, 2.5, 4.0)
        ka = 4.3 * np.sin(phase)
        kb = -5.2 * np.sin(phase + 0.3)
    else:
        la, lb = lines(rng, 12, npix), lines(rng, 10, npix)
        ka = np.rint(4 * np.sin(phase)).astype(int)
        kb = -np.rint(5 * np.sin(phase + 0.3)).astype(int)

    x = np.arange(npix, dtype=float)
    for j in range(nspec):
        obs = 0.5 * spectrum(x - ka[j], la) + 0.5 * spectrum(x - kb[j], lb)
        np.savetxt(dst / f"spectram_{j}.txt", np.column_stack([w, obs]), fmt="%.17g",
                   delimiter="\t")
    vel = C_KMS * (10 ** (np.column_stack([ka, kb]) * dlog) - 1)
    np.savetxt(dst / "velocities.txt", vel, fmt="%.17g", delimiter="\t")
    lam = 10 ** w   # truth in Angstrom, like the program output
    np.savetxt(dst / "truth_A.txt", np.column_stack([lam, spectrum(x, la)]), fmt="%.17g")
    np.savetxt(dst / "truth_B.txt", np.column_stack([lam, spectrum(x, lb)]), fmt="%.17g")
    np.savetxt(dst / "truth_shifts_px.txt", np.column_stack([ka, kb]), fmt="%.10g")
    print(f"{dst.name}: {nspec} spectra, {npix} pixels, dv = {dv:.4f} km/s, "
          f"shifts A{np.round(ka, 2).tolist()}, B {np.round(kb, 2).tolist()}")


def main() -> None:
    src, dst = Path(sys.argv[1]), Path(sys.argv[2])
    make(src, dst, False)
    make(src, dst.parent / (dst.name + "_subpixel"), True)
    make_orbit(src, dst.parent / (dst.name + "_orbit"))
    make_correlation(dst.parent / "correlation")


if __name__ == "__main__":
    main()
