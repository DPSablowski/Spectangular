#include "linefinder.h"

#include "qcustomplot.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>

namespace {
QDoubleSpinBox *dspin(QWidget *p, const QString &name, double lo, double hi, int dec, double val)
{
    auto *s = new QDoubleSpinBox(p);
    s->setObjectName(name);
    s->setRange(lo, hi);
    s->setDecimals(dec);
    s->setValue(val);
    return s;
}
}  // namespace

LineFinderDialog::LineFinderDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Find Lines");
    setObjectName("LineFinderDialog");
    resize(1000, 640);
    auto *top = new QVBoxLayout(this);

    auto *gP = new QGroupBox("Search", this);
    auto *g = new QGridLayout(gP);
    cFind = new QComboBox(gP);
    cFind->setObjectName("comboBox_findin");
    cFind->addItems({"A", "B"});
    cFind->setToolTip("Component whose spectrum is shown; its lines are searched and edited here, the width of its lines is measured in it");
    sEnv = new QSpinBox(gP);
    sEnv->setObjectName("spinBox_env");
    sEnv->setRange(50, 100000);
    sEnv->setToolTip("Window of the continuum estimate [pixels]");
    sPRef = dspin(gP, "doubleSpinBox_pref", 1, 99.9, 1, 90.0);
    sPRef->setSuffix(" %");
    sPRef->setToolTip("Percentile of the flux in the window that is taken as the continuum of this component (lower: for a component whose continuum is less well defined)");
    sDepth = dspin(gP, "doubleSpinBox_mindepth", 0.001, 1, 3, 0.12);
    sDepth->setToolTip("Minimum depth of a line (1 - flux / continuum), smoothed over 5 pixels");
    sSep = new QSpinBox(gP);
    sSep->setObjectName("spinBox_sep");
    sSep->setRange(1, 100000);
    sSep->setToolTip("Minimum distance of two lines [pixels]; of two close ones the deeper is taken");
    sLamLo = dspin(gP, "doubleSpinBox_laml", 0, 100000, 1, 4700);
    sLamHi = dspin(gP, "doubleSpinBox_lamh", 0, 100000, 1, 6500);
    bFind = new QPushButton("Find Lines", gP);
    bFind->setObjectName("pushButton_find");
    bFind->setToolTip("Searches the lines of the selected component with these settings; its list so far is replaced. Then select the other component and search its lines");
    g->addWidget(new QLabel("Component", gP), 0, 0);
    g->addWidget(cFind, 0, 1);
    g->addWidget(new QLabel("window [px]", gP), 0, 2);
    g->addWidget(sEnv, 0, 3);
    g->addWidget(new QLabel("percentile", gP), 0, 4);
    g->addWidget(sPRef, 0, 5);
    g->addWidget(new QLabel("min. depth", gP), 1, 0);
    g->addWidget(sDepth, 1, 1);
    g->addWidget(new QLabel("min. distance [px]", gP), 1, 2);
    g->addWidget(sSep, 1, 3);
    g->addWidget(new QLabel("search from [A]", gP), 1, 4);
    g->addWidget(sLamLo, 1, 5);
    g->addWidget(new QLabel("to", gP), 1, 6);
    g->addWidget(sLamHi, 1, 7);
    g->addWidget(bFind, 0, 8, 2, 1);
    g->setColumnStretch(9, 1);
    top->addWidget(gP);

    plot = new QCustomPlot(this);
    plot->setObjectName("customPlot_lines");
    plot->setMinimumHeight(360);
    plot->xAxis->setLabel("Wavelength [A]");
    plot->yAxis->setLabel("Flux");
    plot->xAxis2->setVisible(true);   // box around the plot, no ticks on the unused axes
    plot->xAxis2->setTickLabels(false);
    plot->xAxis2->setTicks(false);
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(false);
    plot->yAxis2->setTicks(false);
    plot->setInteractions(QCP::iRangeZoom);
    gSpec = plot->addGraph();
    gSpec->setPen(QPen(Qt::black));
    gLines = plot->addGraph();
    gLines->setLineStyle(QCPGraph::lsImpulse);
    gLines->setPen(QPen(QColor(200, 30, 30), 1));
    gLines->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(200, 30, 30), 6));
    top->addWidget(plot, 1);
    connect(plot, &QCustomPlot::mousePress, this, [this](QMouseEvent *e) { plotClicked(e); });
    connect(plot->xAxis, QOverload<const QCPRange &>::of(&QCPAxis::rangeChanged), this, [this](const QCPRange &) { spinsFromRange(); });
    connect(plot->yAxis, QOverload<const QCPRange &>::of(&QCPAxis::rangeChanged), this, [this](const QCPRange &) { spinsFromRange(); });

    auto *r = new QHBoxLayout();
    sX0 = dspin(this, "doubleSpinBox_x0", 0, 100000, 1, 0);
    sX1 = dspin(this, "doubleSpinBox_x1", 0, 100000, 1, 1);
    sY0 = dspin(this, "doubleSpinBox_y0", -100, 100, 3, 0);
    sY1 = dspin(this, "doubleSpinBox_y1", -100, 100, 3, 1.2);
    sY0->setSingleStep(0.1);
    sY1->setSingleStep(0.1);
    bFull = new QPushButton("Full range", this);
    bFull->setObjectName("pushButton_full");
    r->addWidget(new QLabel("X from", this));
    r->addWidget(sX0);
    r->addWidget(new QLabel("to", this));
    r->addWidget(sX1);
    r->addWidget(new QLabel("Y from", this));
    r->addWidget(sY0);
    r->addWidget(new QLabel("to", this));
    r->addWidget(sY1);
    r->addWidget(bFull);
    lCount = new QLabel(this);
    lCount->setObjectName("label_count");
    r->addWidget(lCount, 1);
    top->addLayout(r);
    for (auto *s : {sX0, sX1, sY0, sY1})
        connect(s, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double) { rangeFromSpins(); });
    connect(bFull, &QPushButton::clicked, this, [this]() {
        const std::vector<double> &f = refFlux();
        if (d.lambda.empty() || f.empty()) return;
        plot->xAxis->setRange(d.lambda.front(), d.lambda.back());
        double mx = *std::max_element(f.begin(), f.end());
        double mn = *std::min_element(f.begin(), f.end());
        plot->yAxis->setRange(std::min(0.0, mn), std::max(1.2, std::min(mx, 3.0)));
        plot->replot();
    });

    auto *b = new QHBoxLayout();
    lFile = new QLabel(this);
    lFile->setObjectName("label_file");
    bApply = new QPushButton("Apply", this);
    bApply->setObjectName("pushButton_apply");
    bApply->setToolTip("Writes the line list (with these settings) and closes the window");
    auto *bCancel = new QPushButton("Cancel", this);
    bCancel->setObjectName("pushButton_cancel");
    b->addWidget(new QLabel("Click on a line: remove it; click elsewhere: add a line at the minimum of the flux nearby. Wheel: zoom.", this));
    b->addStretch(1);
    b->addWidget(lFile);
    b->addWidget(bApply);
    b->addWidget(bCancel);
    top->addLayout(b);
    connect(bFind, &QPushButton::clicked, this, &LineFinderDialog::on_pushButton_find_clicked);
    connect(bApply, &QPushButton::clicked, this, &LineFinderDialog::on_pushButton_apply_clicked);
    connect(bCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(cFind, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        percent[curComp] = sPRef->value();
        curComp = idx;
        sPRef->setValue(percent[curComp]);
        showSpectrum();
    });
}

const std::vector<double> &LineFinderDialog::refFlux() const
{
    return curComp == 0 ? d.fluxA : d.fluxB;
}

void LineFinderDialog::setData(const Data &data)
{
    d = data;
    lineLists[0] = d.linesA;
    lineLists[1] = d.linesB;
    for (auto &l : lineLists) std::sort(l.begin(), l.end());
    const linewidth::Params &p = d.params;
    percent[0] = p.percentA;
    percent[1] = p.percentB;
    curComp = 0;
    cFind->setCurrentIndex(0);
    sPRef->setValue(percent[0]);
    sEnv->setValue(p.envelopeSize);
    sDepth->setValue(p.minDepth);
    sSep->setValue(p.minSeparation);
    sLamLo->setValue(p.lamMin);
    sLamHi->setValue(p.lamMax);
    lFile->setText(d.file);
    showSpectrum();
    bFull->click();
    showLines();
}

void LineFinderDialog::collectParams()
{
    d.params.envelopeSize = sEnv->value();
    percent[curComp] = sPRef->value();
    d.params.percentA = percent[0];
    d.params.percentB = percent[1];
    d.params.minDepth = sDepth->value();
    d.params.minSeparation = sSep->value();
    d.params.lamMin = sLamLo->value();
    d.params.lamMax = sLamHi->value();
}

void LineFinderDialog::showSpectrum()
{
    const std::vector<double> &f = refFlux();
    QVector<double> x(d.lambda.size()), y(f.size());
    for (size_t i = 0; i < d.lambda.size() && i < f.size(); i++) { x[i] = d.lambda[i]; y[i] = f[i]; }
    gSpec->setData(x, y);
    showLines();
}

void LineFinderDialog::showLines()
{
    const std::vector<double> &f = refFlux();
    QVector<double> x, y;
    for (double l : cur()) {
        auto it = std::lower_bound(d.lambda.begin(), d.lambda.end(), l);
        size_t i = std::min<size_t>(it - d.lambda.begin(), f.size() ? f.size() - 1 : 0);
        x << l;
        y << (f.empty() ? 0.0 : f[i]);
    }
    gLines->setData(x, y);
    lCount->setText(QString("A: %1 lines, B: %2 lines").arg(lineLists[0].size()).arg(lineLists[1].size()));
    plot->replot();
}

void LineFinderDialog::on_pushButton_find_clicked()
{
    collectParams();
    const std::vector<double> &f = refFlux();
    if (f.size() < 10 || f.size() != d.lambda.size()) return;
    std::vector<double> dep = linewidth::depth(f, percent[curComp], d.params.envelopeSize);
    cur() = linewidth::findLines(d.lambda, dep, d.params);
    showLines();
}

bool LineFinderDialog::addLineAt(double lambda)
{
    const std::vector<double> &f = refFlux();
    if (d.lambda.size() < 3 || f.size() != d.lambda.size()) return false;
    if (lambda < d.lambda.front() || lambda > d.lambda.back()) return false;
    size_t i = std::lower_bound(d.lambda.begin(), d.lambda.end(), lambda) - d.lambda.begin();
    if (i >= d.lambda.size()) i = d.lambda.size() - 1;
    // snap to the flux minimum within +-5 pixels
    size_t lo = i > 5 ? i - 5 : 0, hi = std::min(d.lambda.size() - 1, i + 5), best = i;
    for (size_t k = lo; k <= hi; k++) if (f[k] < f[best]) best = k;
    cur().push_back(d.lambda[best]);
    std::sort(cur().begin(), cur().end());
    showLines();
    return true;
}

bool LineFinderDialog::removeLineNear(double lambda, double tol)
{
    int best = -1;
    double bd = tol;
    for (size_t k = 0; k < cur().size(); k++)
        if (std::fabs(cur()[k] - lambda) <= bd) { bd = std::fabs(cur()[k] - lambda); best = (int)k; }
    if (best < 0) return false;
    cur().erase(cur().begin() + best);
    showLines();
    return true;
}

void LineFinderDialog::plotClicked(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) return;
    if (!plot->axisRect()->rect().contains(e->pos())) return;
    const double x = plot->xAxis->pixelToCoord(e->pos().x());
    // 6 pixels on the screen
    const double tol = std::fabs(plot->xAxis->pixelToCoord(e->pos().x() + 6) - x);
    if (!removeLineNear(x, tol)) addLineAt(x);
}

void LineFinderDialog::rangeFromSpins()
{
    if (updatingSpins) return;
    if (sX1->value() > sX0->value()) plot->xAxis->setRange(sX0->value(), sX1->value());
    if (sY1->value() > sY0->value()) plot->yAxis->setRange(sY0->value(), sY1->value());
    plot->replot();
}

void LineFinderDialog::spinsFromRange()
{
    updatingSpins = true;
    sX0->setValue(plot->xAxis->range().lower);
    sX1->setValue(plot->xAxis->range().upper);
    sY0->setValue(plot->yAxis->range().lower);
    sY1->setValue(plot->yAxis->range().upper);
    updatingSpins = false;
}

void LineFinderDialog::on_pushButton_apply_clicked()
{
    collectParams();
    if (lineLists[0].empty() && lineLists[1].empty()) {
        QMessageBox::information(this, "Find Lines", "There are no lines. Press Find Lines or click in the plot to add some (for each component).");
        return;
    }
    std::string err;
    if (!linewidth::writeLineList(d.file.toStdString(), d.params, lineLists[0], lineLists[1], err)) {
        QMessageBox::information(this, "Find Lines", QString::fromStdString(err));
        return;
    }
    emit applied(d.file);
    accept();
}

// ---------------------------------------------------------------------------------------------

LineScanDialog::LineScanDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Line width scan");
    setObjectName("LineScanDialog");
    resize(900, 640);
    auto *top = new QVBoxLayout(this);
    auto *h = new QHBoxLayout();
    cX = new QComboBox(this);
    cX->setObjectName("comboBox_scanx");
    cX->addItems({"orbit number", "P", "e", "K_A", "K_B", "gamma", "T_peri", "omega"});
    h->addWidget(new QLabel("x axis", this));
    h->addWidget(cX);
    lInfo = new QLabel(this);
    lInfo->setObjectName("label_scaninfo");
    lInfo->setWordWrap(true);
    h->addWidget(lInfo, 1);
    top->addLayout(h);
    auto *pl = new QHBoxLayout();
    plot = new QCustomPlot(this);
    plot->setObjectName("customPlot_scan");
    plot->setMinimumHeight(300);
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plot->xAxis2->setVisible(true);
    plot->xAxis2->setTickLabels(false);
    plot->xAxis2->setTicks(false);
    plotProf = new QCustomPlot(this);
    plotProf->setObjectName("customPlot_profile");
    plotProf->setMinimumHeight(300);
    plotProf->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    for (QCPAxis *a : {plotProf->xAxis2, plotProf->yAxis2}) { a->setVisible(true); a->setTickLabels(false); a->setTicks(false); }
    plotProf->xAxis->setLabel("Velocity [km/s]");
    plotProf->yAxis->setLabel("Depth");
    pl->addWidget(plot, 1);
    pl->addWidget(plotProf, 1);
    top->addLayout(pl, 1);
    lProf = new QLabel(this);
    lProf->setObjectName("label_profile");
    top->addWidget(lProf);
    table = new QTableWidget(0, 12, this);
    table->setObjectName("tableWidget_scan");
    table->setHorizontalHeaderLabels({"#", "P", "e", "K_A", "K_B", "gamma", "T_peri", "omega", "Residuum", "FWHM A", "FWHM B", "depth ratio"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setMaximumHeight(200);
    top->addWidget(table);
    connect(cX, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { showPlot(); });
    connect(table, &QTableWidget::currentCellChanged, this, [this](int r, int, int, int) { if (r >= 0) showProfile(r); });
}

void LineScanDialog::selectOrbit(int k)
{
    if (k >= 0 && k < table->rowCount()) table->setCurrentCell(k, 0);
}

void LineScanDialog::showProfile(int k)
{
    plotProf->clearGraphs();
    plotProf->clearItems();
    if (k < 0 || k >= (int)rows.size()) return;
    const Row &r = rows[k];
    const linewidth::Profile *pr[2] = {&r.m.a, &r.m.b};
    bool any = false;
    for (const auto *p : pr) any = any || (p->ok && p->prof.size() == r.m.dv.size() && !r.m.dv.empty());
    if (!r.ok || !any) {
        lProf->setText(QString("Orbit %1: no profile (no measurement, or the profile file is missing).").arg(k + 1));
        plotProf->replot();
        return;
    }
    QVector<double> x;
    for (double v : r.m.dv) x << v;
    const QColor col[2] = {QColor(30, 90, 200), QColor(200, 70, 30)};
    QString text = QString("Orbit %1:").arg(k + 1);
    for (int c = 0; c < 2; c++) {
        const linewidth::Profile &p = *pr[c];
        const QString name = c == 0 ? "A" : "B";
        if (!p.ok || p.prof.size() != r.m.dv.size()) { text += QString(" %1: not measured;").arg(name); continue; }
        QVector<double> y, e, f;
        for (size_t i = 0; i < r.m.dv.size(); i++) {
            y << p.prof[i]; e << p.err[i];
            f << linewidth::gaussValue(p.fit, r.m.dv[i]);
        }
        QCPGraph *g = plotProf->addGraph();
        g->setDataValueError(x, y, e);
        g->setErrorType(QCPGraph::etValue);
        g->setLineStyle(QCPGraph::lsNone);
        g->setPen(QPen(col[c]));
        g->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, col[c], 5));
        QCPGraph *gf = plotProf->addGraph();
        gf->setData(x, f);
        gf->setPen(QPen(col[c], 2));
        text += QString(" %1 (%2, %3 lines) FWHM %4 +- %5 km/s, v0 %6 km/s, depth %7;")
                    .arg(name, c == 0 ? "blue" : "orange").arg(p.nLines).arg(p.fit.fwhm(), 0, 'f', 1).arg(p.fit.efwhm(), 0, 'f', 1)
                    .arg(p.fit.x0, 0, 'f', 1).arg(p.fit.a, 0, 'f', 3);
    }
    plotProf->rescaleAxes();
    plotProf->replot();
    lProf->setText(text);
}

void LineScanDialog::setRows(const std::vector<Row> &r, const QString &file)
{
    rows = r;
    // x axis: the first element that differs between the orbits
    int xi = 0;
    for (int e = 0; e < 7 && xi == 0; e++)
        for (const Row &w : rows)
            if (w.el[e] != rows[0].el[e]) { xi = e + 1; break; }
    cX->blockSignals(true);
    cX->setCurrentIndex(xi);
    cX->blockSignals(false);
    table->setRowCount((int)rows.size());
    auto fw = [](const linewidth::Profile &p) { return p.ok ? QString::number(p.fit.fwhm(), 'f', 1) : QString("-"); };
    for (size_t k = 0; k < rows.size(); k++) {
        const Row &w = rows[k];
        table->setItem((int)k, 0, new QTableWidgetItem(QString::number(k + 1)));
        for (int e = 0; e < 7; e++) table->setItem((int)k, e + 1, new QTableWidgetItem(QString::number(w.el[e], 'g', 10)));
        table->setItem((int)k, 8, new QTableWidgetItem(QString::number(w.residuum, 'g', 6)));
        table->setItem((int)k, 9, new QTableWidgetItem(fw(w.m.a)));
        table->setItem((int)k, 10, new QTableWidgetItem(fw(w.m.b)));
        table->setItem((int)k, 11, new QTableWidgetItem(w.ok ? QString::number(w.m.ratio, 'f', 3) : QString("-")));
    }
    QString info = QString("FWHM of the mean line profile of component A in its lines and of B in its lines (km/s, left axis), residuum (right axis). Written to %1.").arg(file);
    int bw[2] = {-1, -1}, br = -1;
    for (size_t k = 0; k < rows.size(); k++) {
        for (int c = 0; c < 2; c++) {
            const linewidth::Profile &p = c == 0 ? rows[k].m.a : rows[k].m.b;
            if (!p.ok) continue;
            const linewidth::Profile &q = c == 0 ? rows[bw[c] < 0 ? k : bw[c]].m.a : rows[bw[c] < 0 ? k : bw[c]].m.b;
            if (bw[c] < 0 || p.fit.fwhm() < q.fit.fwhm()) bw[c] = (int)k;
        }
        if (rows[k].residuum > 0 && (br < 0 || rows[k].residuum < rows[br].residuum)) br = (int)k;
    }
    for (int c = 0; c < 2; c++)
        if (bw[c] >= 0) {
            const linewidth::Profile &p = c == 0 ? rows[bw[c]].m.a : rows[bw[c]].m.b;
            info += QString(" Narrowest lines of %1: orbit %2 (FWHM %3).").arg(c == 0 ? "A" : "B").arg(bw[c] + 1).arg(p.fit.fwhm(), 0, 'f', 1);
        }
    if (br >= 0) info += QString(" Lowest residuum: orbit %1.").arg(br + 1);
    lInfo->setText(info);
    showPlot();
    selectOrbit(bw[1] >= 0 ? bw[1] : (bw[0] >= 0 ? bw[0] : 0));
}

void LineScanDialog::showPlot()
{
    plot->clearGraphs();
    QVector<double> xa, xb, xr, fa, fb, res;
    const int xi = cX->currentIndex();
    for (size_t k = 0; k < rows.size(); k++) {
        const double xv = xi == 0 ? double(k + 1) : rows[k].el[xi - 1];
        if (rows[k].m.a.ok) { xa << xv; fa << rows[k].m.a.fit.fwhm(); }
        if (rows[k].m.b.ok) { xb << xv; fb << rows[k].m.b.fit.fwhm(); }
        if (rows[k].ok) { xr << xv; res << rows[k].residuum; }
    }
    plot->xAxis->setLabel(cX->currentText());
    plot->yAxis->setLabel("FWHM [km/s]");
    plot->yAxis2->setVisible(true);
    plot->yAxis2->setTickLabels(true);
    plot->yAxis2->setTicks(true);
    plot->yAxis2->setLabel("Residuum");
    auto add = [&](const QVector<double> &x, const QVector<double> &y, QCPAxis *ya, const QColor &c, Qt::PenStyle st, QCPScatterStyle::ScatterShape sh) {
        QCPGraph *g = plot->addGraph(plot->xAxis, ya);
        g->setData(x, y);
        g->setPen(QPen(c, 2, st));
        g->setScatterStyle(QCPScatterStyle(sh, 6));
    };
    add(xa, fa, plot->yAxis, QColor(30, 90, 200), Qt::SolidLine, QCPScatterStyle::ssDisc);
    add(xb, fb, plot->yAxis, QColor(200, 70, 30), Qt::SolidLine, QCPScatterStyle::ssDisc);
    add(xr, res, plot->yAxis2, QColor(40, 140, 60), Qt::DashLine, QCPScatterStyle::ssSquare);
    plot->rescaleAxes();
    plot->replot();
}
