#include "fluxtemplate.h"

#include "qcustomplot.h"

#include <QApplication>
#include <QCheckBox>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include <cmath>
#include <fstream>
#include <iomanip>

namespace {

QDoubleSpinBox *spin(QWidget *parent, const QString &name, double lo, double hi, double val, int dec,
                     const QString &suffix, const QString &tip)
{
    auto *s = new QDoubleSpinBox(parent);
    s->setObjectName(name);
    s->setRange(lo, hi);
    s->setDecimals(dec);
    s->setValue(val);
    if (!suffix.isEmpty()) s->setSuffix(suffix);
    s->setToolTip(tip);
    return s;
}

} // namespace

FluxTemplate::FluxTemplate(QWidget *parent) : QDialog(parent)
{
    setObjectName("FluxTemplate");
    setWindowTitle("Flux ratio from synthetic spectra");
    auto *top = new QVBoxLayout(this);

    // files
    auto *gFiles = new QGroupBox("Spectra", this);
    auto *gf = new QGridLayout(gFiles);
    labelWork = new QLabel(gFiles);
    gf->addWidget(new QLabel("Work folder", gFiles), 0, 0);
    gf->addWidget(labelWork, 0, 1, 1, 2);
    eCompA = fileRow(gf, 1, "Disentangled A", "lineEdit_compA", "Component spectrum A (output of the disentangling)");
    eCompB = fileRow(gf, 2, "Disentangled B", "lineEdit_compB", "Component spectrum B (output of the disentangling)");
    eSynA = fileRow(gf, 3, "Synthetic A", "lineEdit_synA",
                    "Normalised synthetic spectrum of A (wavelength [A], flux), sampled finer than the components; "
                    "same wavelength scale (air or vacuum) as the observations");
    eSynB = fileRow(gf, 4, "Synthetic B", "lineEdit_synB", "Normalised synthetic spectrum of B");
    top->addWidget(gFiles);

    // disentangling and fit options
    auto *gOpt = new QGroupBox("Fit", this);
    auto *go = new QGridLayout(gOpt);
    k0 = spin(gOpt, "doubleSpinBox_k0", 1e-4, 1e4, 1.0, 5, "",
              "Flux ratio F_A/F_B (at the reference wavelength) with which the components were disentangled");
    labelModel = new QLabel(gOpt);
    go->addWidget(new QLabel("Flux ratio of the disentangling", gOpt), 0, 0);
    go->addWidget(k0, 0, 1);
    go->addWidget(labelModel, 0, 2, 1, 4);
    l1 = spin(gOpt, "doubleSpinBox_l1", 0, 1e6, 0, 2, " Å", "Fit range: lower wavelength (0: all)");
    l2 = spin(gOpt, "doubleSpinBox_l2", 0, 1e6, 0, 2, " Å", "Fit range: upper wavelength (0: all)");
    edge = new QSpinBox(gOpt);
    edge->setObjectName("spinBox_edge");
    edge->setRange(0, 100000);
    edge->setValue(10);
    edge->setToolTip("Pixels left out at both ends of each component (poorly determined edges)");
    go->addWidget(new QLabel("Range from", gOpt), 1, 0);
    go->addWidget(l1, 1, 1);
    go->addWidget(new QLabel("to", gOpt), 1, 2);
    go->addWidget(l2, 1, 3);
    go->addWidget(new QLabel("Edge pixels left out", gOpt), 1, 4);
    go->addWidget(edge, 1, 5);
    fitA = new QCheckBox("fit", gOpt);
    fitA->setObjectName("checkBox_fitA");
    fitA->setChecked(true);
    fitB = new QCheckBox("fit", gOpt);
    fitB->setObjectName("checkBox_fitB");
    fitB->setChecked(true);
    const QString tipV = "RV shift of the synthetic spectrum (km/s); with 'fit': start value of the search. "
                         "Covers a systemic velocity left in the components and air/vacuum differences.";
    vA = spin(gOpt, "doubleSpinBox_vA", -2000, 2000, 0, 2, " km/s", tipV);
    vB = spin(gOpt, "doubleSpinBox_vB", -2000, 2000, 0, 2, " km/s", tipV);
    vmax = spin(gOpt, "doubleSpinBox_vmax", 0, 2000, 100, 1, " km/s", "Search range of the shift (±)");
    go->addWidget(new QLabel("RV shift A", gOpt), 2, 0);
    go->addWidget(vA, 2, 1);
    go->addWidget(fitA, 2, 2);
    go->addWidget(new QLabel("RV shift B", gOpt), 2, 3);
    go->addWidget(vB, 2, 4);
    go->addWidget(fitB, 2, 5);
    go->addWidget(new QLabel("Search range ±", gOpt), 3, 0);
    go->addWidget(vmax, 3, 1);
    deg = new QSpinBox(gOpt);
    deg->setObjectName("spinBox_deg");
    deg->setRange(0, 10);
    deg->setValue(2);
    deg->setToolTip("Degree of the polynomial continuum of the components in the fit (0: constant). Slowly "
                    "varying functions are nearly undetermined by the disentangling, so the continua can be "
                    "wavy over a wide range. Keep it low compared with the number of broad lines.");
    go->addWidget(new QLabel("Continuum degree", gOpt), 3, 2);
    go->addWidget(deg, 3, 3);
    vsA = spin(gOpt, "doubleSpinBox_vsiniA", 0, 1000, 0, 1, " km/s", "Rotational broadening of the synthetic spectrum A (0: none)");
    vsB = spin(gOpt, "doubleSpinBox_vsiniB", 0, 1000, 0, 1, " km/s", "Rotational broadening of the synthetic spectrum B (0: none)");
    resol = spin(gOpt, "doubleSpinBox_R", 0, 1e7, 0, 0, "", "Resolving power λ/Δλ of the observations: Gaussian instrumental profile (0: none)");
    go->addWidget(new QLabel("v sin i A", gOpt), 4, 0);
    go->addWidget(vsA, 4, 1);
    go->addWidget(new QLabel("v sin i B", gOpt), 4, 2);
    go->addWidget(vsB, 4, 3);
    go->addWidget(new QLabel("Resolving power", gOpt), 4, 4);
    go->addWidget(resol, 4, 5);
    top->addWidget(gOpt);

    auto *buttons = new QHBoxLayout;
    bFit = new QPushButton("Fit", this);
    bFit->setObjectName("pushButton_fit");
    bWrite = new QPushButton("Write renormalised spectra", this);
    bWrite->setObjectName("pushButton_write");
    bWrite->setToolTip("Writes <component>_renorm.<ext> for A and B and fluxratio_templates.txt to the work folder");
    bUse = new QPushButton("Use ratio in main window", this);
    bUse->setObjectName("pushButton_use");
    bUse->setToolTip("Sets the field 'Flux ratio' of the main window to the fitted ratio (disentangle again)");
    auto *bClose = new QPushButton("Close", this);
    buttons->addWidget(bFit);
    buttons->addWidget(bWrite);
    buttons->addWidget(bUse);
    buttons->addStretch();
    buttons->addWidget(bClose);
    top->addLayout(buttons);
    bWrite->setEnabled(false);
    bUse->setEnabled(false);
    connect(bFit, &QPushButton::clicked, this, &FluxTemplate::on_pushButton_fit_clicked);
    connect(bWrite, &QPushButton::clicked, this, &FluxTemplate::on_pushButton_write_clicked);
    connect(bUse, &QPushButton::clicked, this, &FluxTemplate::on_pushButton_use_clicked);
    connect(bClose, &QPushButton::clicked, this, &QDialog::close);

    labelResult = new QLabel(this);
    labelResult->setObjectName("labelResult");
    labelResult->setWordWrap(true);
    labelResult->setTextInteractionFlags(Qt::TextSelectableByMouse);
    labelResult->setMinimumHeight(4 * labelResult->fontMetrics().lineSpacing() + 4);
    labelResult->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    top->addWidget(labelResult);

    for (QCustomPlot **p : {&plotA, &plotB}) {
        *p = new QCustomPlot(this);
        (*p)->setMinimumHeight(200);
        (*p)->xAxis2->setVisible(true);
        (*p)->xAxis2->setTickLabels(false);
        (*p)->xAxis2->setTicks(false);
        (*p)->yAxis2->setVisible(true);
        (*p)->yAxis2->setTickLabels(false);
        (*p)->yAxis2->setTicks(false);
        (*p)->xAxis->setLabel("Wavelength [Å]");
        (*p)->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        top->addWidget(*p, 1);
    }
    plotA->setObjectName("customPlot_A");
    plotB->setObjectName("customPlot_B");
    plotA->yAxis->setLabel("A");
    plotB->yAxis->setLabel("B");
    resize(1000, 860);
}

QLineEdit *FluxTemplate::fileRow(QGridLayout *g, int row, const QString &label, const QString &name,
                                 const QString &tip)
{
    QWidget *parent = g->parentWidget();
    auto *e = new QLineEdit(parent);
    e->setObjectName(name);
    e->setToolTip(tip);
    auto *b = new QPushButton("…", parent);
    b->setMaximumWidth(40);
    connect(b, &QPushButton::clicked, this, [this, e]() {
        const QString f = QFileDialog::getOpenFileName(this, "Spectrum", path(e->text()));
        if (f.isEmpty()) return;
        const QString rel = QDir(work).relativeFilePath(f);
        e->setText(rel.startsWith("..") ? f : rel);
    });
    g->addWidget(new QLabel(label, parent), row, 0);
    g->addWidget(e, row, 1);
    g->addWidget(b, row, 2);
    return e;
}

QString FluxTemplate::path(const QString &f) const
{
    if (f.isEmpty()) return work;
    return QFileInfo(f).isAbsolute() ? f : QDir(work).filePath(f);
}

void FluxTemplate::setMainData(const QString &workPath, const QString &compA, const QString &compB,
                               double kUsed, const fr::Model &m)
{
    work = workPath;
    labelWork->setText(workPath);
    eCompA->setText(compA);
    eCompB->setText(compB);
    k0->setValue(kUsed);
    model = m;
    QString desc;
    switch (m.mode) {
    case fr::Mode::Constant: desc = "constant over the wavelengths"; break;
    case fr::Mode::Planck:
        desc = QString("Planck shape, T_A %1 K, T_B %2 K").arg(m.tA).arg(m.tB);
        break;
    case fr::Mode::Table: desc = "shape from the table"; break;
    }
    if (m.mode != fr::Mode::Constant)
        desc += m.lambdaRef > 0 ? QString(", λ_ref %1 Å").arg(m.lambdaRef) : QString(", λ_ref: centre");
    labelModel->setText(desc + " (Data tab; the fit assumes the same shape)");
}

void FluxTemplate::on_pushButton_fit_clicked()
{
    std::string err;
    tfit::Spectrum TA, TB;
    struct F { QLineEdit *e; tfit::Spectrum *s; } files[4] = {{eCompA, &A}, {eCompB, &B}, {eSynA, &TA}, {eSynB, &TB}};
    for (const F &f : files) {
        if (f.e->text().trimmed().isEmpty()) {
            QMessageBox::information(this, "Error", "Flux ratio from synthetic spectra: all four files are needed.");
            return;
        }
        if (!tfit::readSpectrum(path(f.e->text().trimmed()).toStdString(), *f.s, err)) {
            QMessageBox::information(this, "Error", "Flux ratio from synthetic spectra: " + QString::fromStdString(err));
            return;
        }
    }
    tfit::Options o;
    o.l1 = l1->value();
    o.l2 = l2->value();
    o.edgeSkip = edge->value();
    o.fitShift[0] = fitA->isChecked();
    o.fitShift[1] = fitB->isChecked();
    o.shift[0] = vA->value();
    o.shift[1] = vB->value();
    o.vmax = vmax->value();
    o.vsini[0] = vsA->value();
    o.vsini[1] = vsB->value();
    o.resolvingPower = resol->value();
    o.contDegree = deg->value();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    res = tfit::fit(A, B, TA, TB, model, k0->value(), o);
    QApplication::restoreOverrideCursor();
    if (!res.ok) {
        labelResult->setText("Fit failed: " + QString::fromStdString(res.message));
        bWrite->setEnabled(false);
        bUse->setEnabled(false);
        return;
    }
    if (o.fitShift[0]) vA->setValue(res.shift[0]);
    if (o.fitShift[1]) vB->setValue(res.shift[1]);
    const double sum = res.lInd[0] + res.lInd[1];
    QString txt = QString("F_A/F_B = %1 ± %2 (jackknife over %8 wavelength blocks; formal ± %9) at %3 Å "
                          "(light fractions %4 / %5); continuum level of A in the components %6 ± %7.\n")
                  .arg(res.k, 0, 'f', 4).arg(res.errKJack > 0 ? res.errKJack : res.errK, 0, 'g', 2)
                  .arg(res.lambdaRef, 0, 'f', 1)
                  .arg(res.lA, 0, 'f', 4).arg(res.lB, 0, 'f', 4).arg(res.cA, 0, 'f', 4).arg(res.errCA, 0, 'g', 2)
                  .arg(10).arg(res.errK, 0, 'g', 2);
    txt += QString("RV shifts of the synthetic spectra: A %1, B %2 km/s. O−C RMS A %3, B %4 (%5 and %6 pixels).\n")
           .arg(res.shift[0], 0, 'f', 2).arg(res.shift[1], 0, 'f', 2).arg(res.rms[0], 0, 'g', 3)
           .arg(res.rms[1], 0, 'g', 3).arg(res.npix[0]).arg(res.npix[1]);
    txt += QString("Check, separate fits: light fractions %1 + %2 = %3").arg(res.lInd[0], 0, 'f', 4)
           .arg(res.lInd[1], 0, 'f', 4).arg(sum, 0, 'f', 4);
    txt += std::fabs(sum - 1) < 0.03 ? QString(" (consistent).")
           : QString(" — clearly different from 1: the line strengths of the synthetic spectra do not match "
                     "(temperature, gravity, metallicity, broadening); the ratio is not reliable.");
    labelResult->setText(txt);
    bWrite->setEnabled(true);
    bUse->setEnabled(true);
    plot();
}

void FluxTemplate::plot()
{
    struct P { QCustomPlot *p; const tfit::Spectrum *s; const std::vector<double> *m; const std::vector<char> *u; } ps[2] =
        {{plotA, &A, &res.modelA, &res.usedA}, {plotB, &B, &res.modelB, &res.usedB}};
    for (const P &q : ps) {
        q.p->clearGraphs();
        QVector<double> x, y, xm, ym;
        double lo = 1e300, hi = -1e300, ylo = 1e300, yhi = -1e300;
        for (size_t i = 0; i < q.s->lam.size(); ++i) {
            if (!(*q.u)[i]) continue;
            x << q.s->lam[i];
            y << q.s->f[i];
            lo = std::min(lo, q.s->lam[i]);
            hi = std::max(hi, q.s->lam[i]);
            ylo = std::min(ylo, q.s->f[i]);
            yhi = std::max(yhi, q.s->f[i]);
            if (std::isfinite((*q.m)[i])) { xm << q.s->lam[i]; ym << (*q.m)[i]; }
        }
        q.p->addGraph();
        q.p->graph(0)->setData(x, y);
        q.p->graph(0)->setPen(QPen(q.p == plotA ? Qt::blue : Qt::red));
        q.p->graph(0)->setName("disentangled");
        q.p->addGraph();
        q.p->graph(1)->setData(xm, ym);
        q.p->graph(1)->setPen(QPen(Qt::black));
        q.p->graph(1)->setName("synthetic, fitted");
        q.p->legend->setVisible(true);
        if (hi > lo) q.p->xAxis->setRange(lo, hi);
        if (yhi > ylo) q.p->yAxis->setRange(ylo - 0.05 * (yhi - ylo), yhi + 0.25 * (yhi - ylo));
        q.p->replot();
    }
}

void FluxTemplate::on_pushButton_write_clicked()
{
    if (!res.ok) return;
    auto out = [&](const QString &comp) {
        QFileInfo fi(comp);
        const QString name = fi.completeBaseName() + "_renorm" + (fi.suffix().isEmpty() ? QString() : "." + fi.suffix());
        return QFileInfo(path(comp)).dir().filePath(name);
    };
    const QString fa = out(eCompA->text().trimmed()), fb = out(eCompB->text().trimmed());
    for (int q = 0; q < 2; ++q) {
        const tfit::Spectrum &s = q ? B : A;
        const std::vector<double> &r = q ? res.renB : res.renA;
        std::ofstream o((q ? fb : fa).toStdString());
        o << std::setprecision(14);
        for (size_t i = 0; i < s.lam.size(); ++i) o << s.lam[i] << "\t" << r[i] << "\n";
    }
    std::ofstream o(QDir(work).filePath("fluxratio_templates.txt").toStdString());
    o << std::setprecision(10);
    o << "# Flux ratio from synthetic spectra\n";
    o << "# components: " << eCompA->text().toStdString() << ", " << eCompB->text().toStdString()
      << "; synthetic: " << eSynA->text().toStdString() << ", " << eSynB->text().toStdString() << "\n";
    o << "# disentangled with F_A/F_B = " << k0->value() << " (same wavelength dependence assumed)\n";
    o << "# k: value, error (block jackknife), formal error (chi2 curvature)\n";
    o << "k\t" << res.k << "\t" << res.errKJack << "\t" << res.errK << "\n";
    o << "lambda_ref\t" << res.lambdaRef << "\n";
    o << "light_fraction_A\t" << res.lA << "\n";
    o << "light_fraction_B\t" << res.lB << "\n";
    o << "continuum_A\t" << res.cA << "\t" << res.errCA << "\n";
    o << "shift_A\t" << res.shift[0] << "\n";
    o << "shift_B\t" << res.shift[1] << "\n";
    o << "rms_A\t" << res.rms[0] << "\n";
    o << "rms_B\t" << res.rms[1] << "\n";
    o << "pixels_A\t" << res.npix[0] << "\n";
    o << "pixels_B\t" << res.npix[1] << "\n";
    o << "separate_light_fraction_A\t" << res.lInd[0] << "\n";
    o << "separate_light_fraction_B\t" << res.lInd[1] << "\n";
    o << "# renormalised: " << QFileInfo(fa).fileName().toStdString() << ", " << QFileInfo(fb).fileName().toStdString() << "\n";
    labelResult->setText(labelResult->text() + "\nWritten: " + QFileInfo(fa).fileName() + ", " + QFileInfo(fb).fileName()
                         + ", fluxratio_templates.txt.");
}

void FluxTemplate::on_pushButton_use_clicked()
{
    if (res.ok) emit ratioAccepted(res.k);
}
