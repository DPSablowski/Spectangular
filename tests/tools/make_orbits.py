#!/usr/bin/env python3
"""Inputs for the test of the RV calculation from orbital elements.

orbits.txt: per line P, e, K_A, K_B, gamma, T_peri, omega[rad] (order as in orbele).
rv_times.txt: 200 epochs over two periods.
Based on the elements from TestData/orbitelements.dat, varied in e.

Usage:  make_orbits.py <TestData_dir> <target_dir>
"""
import sys
from pathlib import Path

import numpy as np

ECCENTRICITIES = [0.0, 0.3, 0.537, 0.8, 0.95]


def main() -> None:
    src, dst = Path(sys.argv[1]), Path(sys.argv[2])
    dst.mkdir(parents=True, exist_ok=True)
    base = np.loadtxt(src / "orbitelements.dat")
    times = np.loadtxt(src / "times.dat")
    P = base[0]
    t = times[0] + np.linspace(0.0, 2.0 * P, 200, endpoint=False)
    np.savetxt(dst / "rv_times.txt", t, fmt="%.10f")
    rows = []
    for e in ECCENTRICITIES:
        r = base.copy()
        r[1] = e
        rows.append(r)
    np.savetxt(dst / "orbits.txt", np.array(rows), fmt="%.14g")
    print(f"orbit: {len(rows)} orbits, {len(t)} epochs")


if __name__ == "__main__":
    main()
