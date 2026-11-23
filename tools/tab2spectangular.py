#!/usr/bin/env python3
"""Convert spectra in three-column tables (wavelength [A], flux, error) into the ASCII input
of Spectangular.

Spectangular reads per spectrum a file <prefix><index><ext> with two columns,
log10(lambda) and flux, all spectra on one common grid that is equidistant in log10(lambda)
(constant step in km/s). This script

  * reads the tables (e.g. STELLA/SES "...botzfxsEcdtab.txt"),
  * cuts them to a common wavelength range (optionally --wmin/--wmax),
  * puts all of them on one common log10(lambda) grid: cubic spline for steps up to 1.5
    times the original pixel, flux-conserving averaging for coarser steps (--step-kms),
  * writes <prefix>0<ext>, <prefix>1<ext>, ... (index from --first),
  * writes <prefix>snr.txt: S/N per output pixel for each spectrum (median of flux/error in
    the range), usable as S/N file ("Weight by S/N", Model tab),
  * writes <prefix>files.txt: index, source file and S/N, as the order for the times file
    and the RV file.

Spectangular settings afterwards (Data tab): Path = output folder, Prefix = --prefix,
Extension = --ext, format ASCII, first/last spectrum = the indices written. The times of
observation (times file, one line per spectrum in the same order) are not in the tables
and must be taken from the FITS headers or the observing log. Barycentric correction:
Tools -> Barycentric correction in Spectangular (on the files written here).

Example:
  python3 tab2spectangular.py "raw/*_botzfxsEcdtab.txt" -o itri_6400 --prefix itri_ \\
          --wmin 6400 --wmax 6500

Only numpy is needed; scipy is used for the spline if it is installed.
"""

import argparse
import glob
import os
import sys

import numpy as np

C_KMS = 299792.458


def read_table(path):
    """Columns wavelength, flux, error (error optional). Lines with '#' and non-finite values
    are skipped; the wavelengths are sorted and duplicates removed."""
    d = np.genfromtxt(path, comments="#", invalid_raise=False)
    if d.ndim != 2 or d.shape[1] < 2:
        raise ValueError(f"{path}: need at least two columns (wavelength, flux)")
    w, f = d[:, 0], d[:, 1]
    e = d[:, 2] if d.shape[1] >= 3 else np.full_like(f, np.nan)
    ok = np.isfinite(w) & np.isfinite(f) & (w > 0)
    w, f, e = w[ok], f[ok], e[ok]
    order = np.argsort(w, kind="stable")
    w, f, e = w[order], f[order], e[order]
    keep = np.concatenate(([True], np.diff(w) > 0))
    w, f, e = w[keep], f[keep], e[keep]
    if len(w) < 10:
        raise ValueError(f"{path}: fewer than 10 valid pixels")
    if w[0] < 100.0:
        raise ValueError(f"{path}: first column must be the wavelength in Angstrom")
    return w, f, e


def spline(x, y, xi):
    try:
        from scipy.interpolate import CubicSpline
        return CubicSpline(x, y, bc_type="natural")(xi)
    except ImportError:
        return natural_spline(x, y, xi)


def natural_spline(x, y, xi):
    """Natural cubic spline (as in Spectangular), without scipy."""
    n = len(x)
    h = np.diff(x)
    a = h[:-1] / 6.0
    b = (h[:-1] + h[1:]) / 3.0
    c = h[1:] / 6.0
    r = np.diff(y[1:]) / h[1:] - np.diff(y[:-1]) / h[:-1]
    m = np.zeros(n)
    cp = np.zeros(n - 2)
    dp = np.zeros(n - 2)
    for i in range(n - 2):                       # Thomas algorithm
        den = b[i] - (a[i] * cp[i - 1] if i > 0 else 0.0)
        cp[i] = c[i] / den
        dp[i] = (r[i] - (a[i] * dp[i - 1] if i > 0 else 0.0)) / den
    for i in range(n - 3, -1, -1):
        m[i + 1] = dp[i] - (cp[i] * m[i + 2] if i < n - 3 else 0.0)
    k = np.clip(np.searchsorted(x, xi) - 1, 0, n - 2)
    hk = x[k + 1] - x[k]
    A = (x[k + 1] - xi) / hk
    B = (xi - x[k]) / hk
    return A * y[k] + B * y[k + 1] + ((A ** 3 - A) * m[k] + (B ** 3 - B) * m[k + 1]) * hk ** 2 / 6.0


def rebin(x, y, edges):
    """Flux-conserving mean over the bins [edges[i], edges[i+1]] of the piecewise linear
    function through (x, y)."""
    cum = np.concatenate(([0.0], np.cumsum(0.5 * (y[1:] + y[:-1]) * np.diff(x))))

    def integral(t):
        k = np.clip(np.searchsorted(x, t) - 1, 0, len(x) - 2)
        dt = t - x[k]
        slope = (y[k + 1] - y[k]) / (x[k + 1] - x[k])
        return cum[k] + y[k] * dt + 0.5 * slope * dt ** 2

    I = integral(edges)
    return np.diff(I) / np.diff(edges)


def native_step_kms(w):
    return float(np.median(np.diff(w) / w[:-1]) * C_KMS)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("inputs", nargs="+", help="table files or glob patterns (sorted by name)")
    ap.add_argument("-o", "--outdir", default="spectangular_input", help="output folder")
    ap.add_argument("--prefix", default="spec_", help="file name prefix (Data tab: Prefix)")
    ap.add_argument("--ext", default=".txt", help="extension (Data tab: Extension)")
    ap.add_argument("--first", type=int, default=0, help="index of the first spectrum")
    ap.add_argument("--wmin", type=float, help="lower wavelength limit [A]")
    ap.add_argument("--wmax", type=float, help="upper wavelength limit [A]")
    ap.add_argument("--step-kms", type=float, default=0.0,
                    help="grid step in km/s (default: the finest original pixel step in the range)")
    ap.add_argument("--sort", choices=["name", "none"], default="name",
                    help="order of the spectra: by file name (STELLA: chronological) or as given")
    ap.add_argument("--min-pixels", type=int, default=10)
    args = ap.parse_args()

    files = []
    for p in args.inputs:
        hits = glob.glob(p)
        files += hits if hits else [p]
    if args.sort == "name":
        files = sorted(dict.fromkeys(files), key=os.path.basename)
    if not files:
        sys.exit("No input files.")

    spectra = []
    for p in files:
        try:
            spectra.append((p,) + read_table(p))
        except (OSError, ValueError) as exc:
            sys.exit(f"Error: {exc}")

    # common range
    lo = max(s[1][0] for s in spectra)
    hi = min(s[1][-1] for s in spectra)
    if args.wmin is not None:
        lo = max(lo, args.wmin)
    if args.wmax is not None:
        hi = min(hi, args.wmax)
    if not hi > lo:
        sys.exit(f"Error: no common wavelength range ({lo:.3f} >= {hi:.3f} A).")

    # original step in the range per spectrum
    steps = []
    for p, w, f, e in spectra:
        m = (w >= lo) & (w <= hi)
        if m.sum() < args.min_pixels:
            sys.exit(f"Error: {p}: fewer than {args.min_pixels} pixels in {lo:.2f}-{hi:.2f} A.")
        steps.append(native_step_kms(w[m]))
    step_kms = args.step_kms if args.step_kms > 0 else min(steps)
    dlog = np.log10(1.0 + step_kms / C_KMS)

    l0, l1 = np.log10(lo), np.log10(hi)
    n = int(np.floor((l1 - l0) / dlog * (1 + 1e-12))) + 1
    if n < args.min_pixels:
        sys.exit(f"Error: only {n} pixels on the grid.")
    grid = l0 + dlog * np.arange(n)
    lam = 10.0 ** grid

    os.makedirs(args.outdir, exist_ok=True)
    rows, snr = [], []
    for k, ((p, w, f, e), st) in enumerate(zip(spectra, steps)):
        idx = args.first + k
        ratio = step_kms / st
        if ratio <= 1.5:
            flux = spline(w, f, lam)
            method = "spline"
        else:
            half = 0.5 * dlog
            edges = 10.0 ** np.concatenate(([grid[0] - half], grid + half))
            edges = np.clip(edges, w[0], w[-1])
            flux = rebin(w, f, edges)
            method = "rebinned"
        # S/N per output pixel: median of flux/error in the range (pixels with error <= 0,
        # e.g. saturated telluric lines, ignored); averaging over 'ratio' pixels raises it
        m = (w >= lo) & (w <= hi) & np.isfinite(e) & (e > 0)
        s = float(np.median(f[m] / e[m])) * np.sqrt(max(1.0, ratio)) if m.sum() > 0 else float("nan")
        snr.append(s)
        out = os.path.join(args.outdir, f"{args.prefix}{idx}{args.ext}")
        np.savetxt(out, np.column_stack([grid, flux]), fmt=["%.12f", "%.8g"], delimiter="\t")
        rows.append((idx, os.path.basename(p), s, method))
        print(f"{idx:4d}  {os.path.basename(p)}  S/N {s:7.1f}  ({method}, original {st:.3f} km/s)")

    with open(os.path.join(args.outdir, f"{args.prefix}files.txt"), "w") as fh:
        fh.write("# index\tsource file\tS/N per pixel\tmethod\n")
        for r in rows:
            fh.write(f"{r[0]}\t{r[1]}\t{r[2]:.1f}\t{r[3]}\n")
    if all(np.isfinite(snr)):
        np.savetxt(os.path.join(args.outdir, f"{args.prefix}snr.txt"), snr, fmt="%.2f")
    else:
        print("No (valid) error column in some files: no S/N file written (use S/N 'auto').")

    print(f"\n{len(spectra)} spectra, {n} pixels each, {lo:.3f}-{hi:.3f} A, {step_kms:.4f} km/s per pixel.")
    print(f"Spectangular, Data tab: Path {os.path.abspath(args.outdir)}, Prefix '{args.prefix}', "
          f"Extension '{args.ext}', format ASCII, spectra {args.first} to {args.first + len(spectra) - 1}.")
    print("Times file (one line per spectrum, same order): from the FITS headers / observing log.")


if __name__ == "__main__":
    main()
