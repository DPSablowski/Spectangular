#!/usr/bin/env python3
"""Creates reduced test data sets from Spectangular/TestData.

The spectra lie on an equidistant log10(lambda) grid. Binning by a factor k
averages k neighbouring pixels each (wavelength and flux). The grid stays
equidistant in log10(lambda), dv grows by the factor k. RVs (km/s) and
observation times stay unchanged. A remainder at the end of a spectrum is discarded.

Usage:  make_testdata.py <TestData_dir> <target_dir>
"""
import shutil
import sys
from pathlib import Path

import numpy as np

# name: (bin factor, list of spectrum indices from TestData)
DATASETS = {
    "bin4": (4, list(range(20))),        # all 20 spectra, 354 pixels
    "bin8_n10": (8, list(range(0, 20, 2))),  # every second spectrum, 177 pixels
}

PREFIX = "spectram_"


def bin_spectrum(src: Path, k: int) -> np.ndarray:
    d = np.loadtxt(src)
    n = (len(d) // k) * k
    return d[:n].reshape(-1, k, 2).mean(axis=1)


def write_config(template: Path, dst: Path, n_spectra: int) -> None:
    lines = template.read_text().splitlines()
    assert len(lines) == 58, f"{template}: {len(lines)} lines instead of 58"
    lines[1] = str(n_spectra - 1)   # spinBox_2: last spectrum
    lines[2] = "3"                  # spinBox_3: iterations
    lines[17] = "@DATADIR@"         # lineEdit_4: path, set by the test run
    lines[14 + 22 + 8] = "1"        # checkBox_9: create a new start simplex
    dst.write_text("\n".join(lines) + "\n")


def main() -> None:
    src_dir, out_dir = Path(sys.argv[1]), Path(sys.argv[2])
    vel = np.loadtxt(src_dir / "velocities.txt")
    times = np.loadtxt(src_dir / "times.dat")
    for name, (k, idx) in DATASETS.items():
        dst = out_dir / name
        if dst.exists():
            shutil.rmtree(dst)
        dst.mkdir(parents=True)
        for new_i, old_i in enumerate(idx):
            b = bin_spectrum(src_dir / f"{PREFIX}{old_i}.txt", k)
            np.savetxt(dst / f"{PREFIX}{new_i}.txt", b, fmt="%.14g", delimiter="\t")
        np.savetxt(dst / "velocities.txt", vel[idx], fmt="%.10g", delimiter="\t")
        np.savetxt(dst / "times.dat", times[idx], fmt="%.10f")
        shutil.copy(src_dir / "orbitelements.dat", dst / "orbitelements.dat")
        write_config(src_dir / "SB2_conf.sps", dst / "SB2_conf.sps", len(idx))
        w = b[:, 0]
        dv = (10 ** (w[5] - w[4]) - 1) * 299792.458
        print(f"{name}: {len(idx)} spectra, {len(w)} pixels, dv = {dv:.3f} km/s")


if __name__ == "__main__":
    main()
