#!/usr/bin/env python3
"""Synthetic eclipsing SB2 over a wide wavelength range, with a wavelength-dependent
flux ratio (test of the flux-ratio model, manual section "Flux ratio over a wide range").

Model (independent of the program; the line profiles are evaluated analytically at the
Doppler-shifted wavelengths, no interpolation):

  component spectra   x_A, x_B: continuum 1, Gaussian absorption lines (product of
                      1 - d exp(-(lambda - l0)^2 / 2 s^2)); A hot with few broad lines,
                      B cool with many narrower lines; widths given in km/s
  continuum fluxes    F_A(lambda) = r_ref P(lambda) / P(lambda_ref), F_B = 1,
                      P = B_lambda(T_A) / B_lambda(T_B)  (Planck, no limb darkening)
  orbit               circular, RV_A = K_A sin(2 pi phi), RV_B = -K_B sin(2 pi phi)
  eclipses            fraction eps_A(phi) of the disc of A covered around phi = 0, eps_B
                      of B around phi = 0.5 (triangular in phase, wavelength-independent,
                      i.e. uniform discs)
  observation j       o_j = [(1-eps_A) F_A x_A(lambda/(1+v_A/c)) + (1-eps_B) F_B x_B(...)]
                            / [(1-eps_A) F_A + (1-eps_B) F_B]  + Gaussian noise 1/(S/N)

Hence F_A/F_B of spectrum j is k_j s(lambda) with k_j = r_ref (1-eps_A,j)/(1-eps_B,j) at
lambda_ref and s = P/P(lambda_ref): exactly the program's model "Planck".

Written into the target folder:
  spectram_<j>.txt   log10(lambda), flux on one equidistant log10 grid
  velocities.txt     true RVs (RV_A RV_B)
  times.dat          phases (one per spectrum)
  edit.dat           true light fractions at lambda_ref (static 1, A = k/(1+k), B = 1/(1+k))
  truth_A.txt, truth_B.txt   component spectra (Angstrom, flux) on the rest grid, extended
  truth_k.txt        k_j, eps_A, eps_B per spectrum
  eclipse_info.txt   parameters
If --conf is given, that configuration is copied and set up (spectra 0..n-1, SB2, RVs from
velocities.txt, cubic interpolation, flux ratio r_ref, model Planck with T_A, T_B, lambda_ref).

Usage: make_eclipse.py <target> [--conf SB2_conf.sps] [--snr 200] [--seed 1] [--dv 30]
                       [--l1 4500] [--l2 6500]
"""
import argparse
import json
import shutil
from pathlib import Path

import numpy as np

C_KMS = 299792.458
C2 = 1.438776877e8     # h c / k in A K

DEFAULTS = dict(l1=4500.0, l2=6500.0, dv=30.0, snr=200.0, seed=1, ta=15000.0, tb=6000.0,
                r_ref=1.5, lref=5500.0, ka=90.0, kb=120.0,
                phases=[0.0, 0.015, 0.03, 0.47, 0.5, 0.53, 0.1, 0.2, 0.3, 0.38, 0.62, 0.72, 0.8, 0.9],
                eps_a=0.6, eps_b=0.45, width=0.045)


def planck_ratio(lam, ta, tb):
    """B_lambda(T_A) / B_lambda(T_B)."""
    return np.expm1(C2 / (lam * tb)) / np.expm1(C2 / (lam * ta))


def line_list(rng, n, l1, l2, smin, smax, dmin, dmax):
    return [(rng.uniform(l1, l2), rng.uniform(smin, smax), rng.uniform(dmin, dmax)) for _ in range(n)]


def spectrum(lam, lines):
    f = np.ones_like(lam)
    for l0, s_kms, d in lines:
        s = l0 * s_kms / C_KMS
        f *= 1.0 - d * np.exp(-0.5 * ((lam - l0) / s) ** 2)
    return f


def eclipse(phi, centre, depth, width):
    d = np.abs((phi - centre + 0.5) % 1.0 - 0.5)
    return depth * np.clip(1.0 - d / width, 0.0, None)


def make(dst, conf=None, **kw):
    p = dict(DEFAULTS, **kw)
    dst = Path(dst)
    dst.mkdir(parents=True, exist_ok=True)
    rng = np.random.default_rng(p["seed"])
    dlog = np.log10(1.0 + p["dv"] / C_KMS)
    w = np.arange(np.log10(p["l1"]), np.log10(p["l2"]), dlog)
    lam = 10 ** w
    margin = 40.0     # lines not too close to the ends
    la = line_list(rng, 30, p["l1"] + margin, p["l2"] - margin, 120.0, 260.0, 0.05, 0.35)   # hot, broad
    lb = line_list(rng, 140, p["l1"] + margin, p["l2"] - margin, 45.0, 80.0, 0.05, 0.6)     # cool, narrow
    phi = np.asarray(p["phases"], float)
    va = p["ka"] * np.sin(2 * np.pi * phi)
    vb = -p["kb"] * np.sin(2 * np.pi * phi)
    ea = eclipse(phi, 0.0, p["eps_a"], p["width"])
    eb = eclipse(phi, 0.5, p["eps_b"], p["width"])
    fa = p["r_ref"] * planck_ratio(lam, p["ta"], p["tb"]) / planck_ratio(p["lref"], p["ta"], p["tb"])
    k = p["r_ref"] * (1 - ea) / (1 - eb)
    noise = np.random.default_rng(p["seed"] + 1000)
    for j in range(len(phi)):
        xa = spectrum(lam / (1 + va[j] / C_KMS), la)
        xb = spectrum(lam / (1 + vb[j] / C_KMS), lb)
        FA, FB = (1 - ea[j]) * fa, (1 - eb[j]) * np.ones_like(lam)
        o = (FA * xa + FB * xb) / (FA + FB)
        if p["snr"] > 0:
            o = o + noise.normal(0.0, 1.0 / p["snr"], len(o))
        np.savetxt(dst / f"spectram_{j}.txt", np.column_stack([w, o]), fmt="%.15g", delimiter="\t")
    np.savetxt(dst / "velocities.txt", np.column_stack([va, vb]), fmt="%.12g", delimiter="\t")
    np.savetxt(dst / "times.dat", phi, fmt="%.6f")
    # edit.dat: static weights, then A, then B (light fractions at lambda_ref)
    np.savetxt(dst / "edit.dat", np.concatenate([np.ones(len(phi)), k / (1 + k), 1 / (1 + k)]), fmt="%.15g")
    ext = np.arange(w[0] - 300 * dlog, w[-1] + 300 * dlog, dlog)
    np.savetxt(dst / "truth_A.txt", np.column_stack([10 ** ext, spectrum(10 ** ext, la)]), fmt="%.12g")
    np.savetxt(dst / "truth_B.txt", np.column_stack([10 ** ext, spectrum(10 ** ext, lb)]), fmt="%.12g")
    np.savetxt(dst / "truth_k.txt", np.column_stack([k, ea, eb]), fmt="%.12g",
               header="k_j = F_A/F_B at lambda_ref, eps_A, eps_B")
    info = {key: p[key] for key in ("l1", "l2", "dv", "snr", "seed", "ta", "tb", "r_ref", "lref", "ka", "kb",
                                     "eps_a", "eps_b", "width")}
    info.update(n_spectra=len(phi), pixels=len(w), dlog=dlog,
                ratio_range=[float(fa.min()), float(fa.max())])
    (dst / "eclipse_info.txt").write_text(json.dumps(info, indent=1) + "\n")
    if conf is not None:
        setup_conf(Path(conf), dst / "SB2_conf.sps", len(phi), p)
    return info


def setup_conf(src, dst_conf, n, p):
    lines = Path(src).read_text().splitlines()
    lines[0], lines[1] = "0", str(n - 1)     # spectra 0 .. n-1
    lines[9] = str(p["r_ref"])               # flux ratio (at lambda_ref)
    lines[16] = "spectram_"
    lines[17] = str(dst_conf.parent)
    cb = 14 + 22
    lines[cb + 0] = "0"                      # cross-correlation off
    lines[cb + 1] = "1"                      # RVs from velocities.txt
    lines[cb + 15] = "0"                     # orbit off
    lines = [l for l in lines if not l.startswith(("comboBox_interp", "comboBox_ratio", "doubleSpinBox_T",
                                                   "doubleSpinBox_lref"))]
    if "# Spectangular settings v2" not in lines:
        lines.append("# Spectangular settings v2")
    lines += ["comboBox_interp=1", "comboBox_ratio=1", f"doubleSpinBox_TA={p['ta']}",
              f"doubleSpinBox_TB={p['tb']}", f"doubleSpinBox_lref={p['lref']}"]
    dst_conf.write_text("\n".join(lines) + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("target")
    ap.add_argument("--conf")
    ap.add_argument("--snr", type=float, default=DEFAULTS["snr"])
    ap.add_argument("--seed", type=int, default=DEFAULTS["seed"])
    ap.add_argument("--dv", type=float, default=DEFAULTS["dv"])
    ap.add_argument("--l1", type=float, default=DEFAULTS["l1"])
    ap.add_argument("--l2", type=float, default=DEFAULTS["l2"])
    a = ap.parse_args()
    info = make(a.target, a.conf, snr=a.snr, seed=a.seed, dv=a.dv, l1=a.l1, l2=a.l2)
    print(json.dumps(info))


if __name__ == "__main__":
    main()
