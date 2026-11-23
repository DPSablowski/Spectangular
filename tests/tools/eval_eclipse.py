#!/usr/bin/env python3
"""Compares disentangled components with the truth of make_eclipse.py.

For each component: RMS of (output - truth) over the inner pixels (without the
outermost 5 % of the range, which are determined by single spectra), and the same after
removing a smooth trend (cubic polynomial in lambda) from the difference, i.e. the error
of the lines alone without a continuum offset or tilt.

Usage: eval_eclipse.py <work dir with truth_A/B.txt> <output dir with SB2_1.txt, SB2_2.txt>
       prints a JSON dictionary
"""
import json
import sys
from pathlib import Path

import numpy as np


def compare(outfile, truthfile, trim=0.05):
    out = np.loadtxt(outfile)
    tr = np.loadtxt(truthfile)
    lam, f = out[:, 0], out[:, 1]
    n = len(lam)
    inner = slice(int(trim * n), n - int(trim * n))
    lam, f = lam[inner], f[inner]
    ref = np.interp(np.log10(lam), np.log10(tr[:, 0]), tr[:, 1])
    d = f - ref
    x = (lam - lam.mean()) / (lam.max() - lam.min())
    trend = np.polyval(np.polyfit(x, d, 3), x)
    return {"rms": float(np.sqrt(np.mean(d ** 2))), "rms_lines": float(np.sqrt(np.mean((d - trend) ** 2))),
            "mean": float(d.mean()), "trend_range": float(trend.max() - trend.min())}


def evaluate(work, out):
    work, out = Path(work), Path(out)
    return {"A": compare(out / "SB2_1.txt", work / "truth_A.txt"),
            "B": compare(out / "SB2_2.txt", work / "truth_B.txt")}


if __name__ == "__main__":
    print(json.dumps(evaluate(sys.argv[1], sys.argv[2]), indent=1))
