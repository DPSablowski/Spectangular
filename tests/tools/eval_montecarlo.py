#!/usr/bin/env python3
"""Evaluation of the Monte Carlo runs (make_montecarlo.py).

Per element, over all realisations:
  Bias       = mean(best value) - true, with error of the mean
  Spread     = standard deviation of the best values
  <Error>    = mean of the reported standard errors
  Spr/<E>    = ratio Spread / <Error>  (expected 1)
  Pull       = (best value - true) / error: mean ~0, std ~1, ~68 % with |Pull| < 1

Usage: eval_montecarlo.py <target_dir> [--plot file.png]
"""
import argparse
import math
from pathlib import Path

import numpy as np

IDX = {"P": 0, "e": 1, "K_A": 2, "K_B": 3, "gamma": 4, "T_peri": 5, "omega": 6}


def read_errors(path):
    rows = {}
    for line in path.read_text().splitlines():
        p = line.split("\t")
        if len(p) == 4 and not line.startswith("#"):
            rows[p[0]] = (float(p[1]), float(p[2]))
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dest")
    ap.add_argument("--plot")
    a = ap.parse_args()
    runs, failed = [], []
    for d in sorted(Path(a.dest).glob("r[0-9]*")):
        f = d / "errout" / "errors.dat"
        if f.exists():
            runs.append((d.name, read_errors(f), np.loadtxt(d / "truth_orbit.txt")))
        else:
            failed.append(d.name)
    names = [n for n in IDX if n in runs[0][1]]
    print(f"{len(runs)} realisations evaluated, {len(failed)} without result {failed}")
    print(f"{'Element':8} {'true':>12} {'Bias':>11} {'+/-':>9} {'Bias/s':>7} {'Spread':>10} "
          f"{'<Error>':>10} {'Spr/<E>':>8} {'Pull-Avg':>8} {'Pull-Std':>8} {'|Pull|<1':>8}")
    pulls = {}
    summary = {}
    for n in names:
        truth = runs[0][2][IDX[n]]
        v = np.array([r[1][n][0] for r in runs])
        e = np.array([r[1][n][1] for r in runs])
        bias, spread, merr = v.mean() - truth, v.std(ddof=1), e.mean()
        bias_err = spread / math.sqrt(len(v))
        pull = (v - truth) / e
        pulls[n] = pull
        frac = np.mean(np.abs(pull) < 1)
        summary[n] = dict(truth=truth, bias=bias, bias_err=bias_err, spread=spread, merr=merr,
                          ratio=spread / merr, pull_mean=pull.mean(), pull_std=pull.std(ddof=1),
                          frac=frac)
        print(f"{n:8} {truth:12.6g} {bias:11.3g} {bias_err:9.2g} {bias / bias_err:7.2f} {spread:10.3g} "
              f"{merr:10.3g} {spread / merr:8.2f} {pull.mean():8.2f} {pull.std(ddof=1):8.2f} {frac:8.0%}")
    # Uncertainty of the ratio Spread/<Error> for N realisations: ~1/sqrt(2(N-1))
    print(f"(Uncertainty of Spr/<E> and Pull-Std for N = {len(runs)}: about "
          f"{1 / math.sqrt(2 * (len(runs) - 1)):.2f})")
    if a.plot:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
        fig, axes = plt.subplots(1, len(names), figsize=(2.3 * len(names), 3.0), sharey=True)
        x = np.linspace(-4, 4, 200)
        for ax, n in zip(axes, names):
            ax.hist(pulls[n], bins=np.linspace(-4, 4, 17), density=True, color="#4C78A8", alpha=0.8)
            ax.plot(x, np.exp(-0.5 * x * x) / math.sqrt(2 * math.pi), color="#222", lw=1.2)
            s = summary[n]
            ax.set_title(f"{n}\nMean {s['pull_mean']:+.2f}, Std {s['pull_std']:.2f}", fontsize=9)
            ax.set_xlabel("Pull")
        axes[0].set_ylabel("Density")
        fig.suptitle(f"(fit - true) / standard error, {len(runs)} noise realisations; line: N(0,1)",
                     fontsize=10, x=0.01, ha="left")
        fig.tight_layout()
        fig.savefig(a.plot, dpi=130)


if __name__ == "__main__":
    main()
