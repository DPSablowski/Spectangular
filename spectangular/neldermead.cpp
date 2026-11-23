#include "neldermead.h"

namespace nm {

void order(const std::vector<double> &y, int &h, int &sh, int &l)
{
    const int m = int(y.size());
    h = 0;
    l = 0;
    for (int i = 1; i < m; ++i) {
        if (y[i] > y[h]) h = i;
        if (y[i] < y[l]) l = i;
    }
    sh = (h == 0 && m > 1) ? 1 : 0;
    for (int i = 0; i < m; ++i) {
        if (i != h && y[i] > y[sh]) sh = i;
    }
    if (m == 1) sh = h;
}

StepResult step(arma::mat &P, std::vector<double> &y, const Coefficients &c,
                const std::function<double(const arma::rowvec &)> &f,
                const std::function<bool()> &abort)
{
    StepResult r;
    const int n = int(P.n_cols);
    int h, sh, l;
    order(y, h, sh, l);
    const double yl = y[l], ysh = y[sh];
    double yh = y[h];

    auto eval = [&](const arma::rowvec &x) {
        double v = f(x);
        ++r.evaluations;
        if (abort()) r.aborted = true;
        return v;
    };

    // centroid without the worst point
    arma::rowvec Z(n, arma::fill::zeros);
    for (int i = 0; i < int(P.n_rows); ++i)
        if (i != h) Z += P.row(i);
    Z /= double(n);

    // reflection
    arma::rowvec Co = Z + c.alpha * (Z - P.row(h));
    double yi = eval(Co);
    if (r.aborted) return r;

    if (yi < yl) {
        // better than the best point: expansion in the direction of the reflection
        r.log += "yi<yl; ";
        arma::rowvec Eo = Z + c.gamma * (Co - Z);
        double yt = eval(Eo);
        if (r.aborted) return r;
        if (yt < yl) {
            r.log += "yt<yl, expansion; ";
            P.row(h) = Eo;
            y[h] = yt;
        } else {
            r.log += "yt>=yl, reflection; ";
            P.row(h) = Co;
            y[h] = yi;
        }
        return r;
    }

    r.log += "yi>=yl; ";
    if (yi <= ysh) {
        r.log += "yi<=ysh, reflection 2; ";
        P.row(h) = Co;
        y[h] = yi;
        return r;
    }

    r.log += "yi>ysh; ";
    if (yi <= yh) {
        r.log += "yi<=yh, reflection 3; ";
        P.row(h) = Co;
        y[h] = yi;
        yh = yi;   // contraction refers to the new worst point
    }

    // contraction
    arma::rowvec So = Z + c.beta * (P.row(h) - Z);
    double yt = eval(So);
    if (r.aborted) return r;
    if (yt > yh) {
        // contraction worse: shrink all points towards the best one
        r.log += "yt>yh, total contraction; ";
        for (int j = 0; j < int(P.n_rows); ++j) {
            if (j == l) continue;
            P.row(j) = P.row(l) + c.delta * (P.row(j) - P.row(l));
            y[j] = eval(P.row(j));
            if (r.aborted) return r;
        }
    } else {
        r.log += "yt<=yh, single contraction; ";
        P.row(h) = So;
        y[h] = yt;
    }
    return r;
}

} // namespace nm
