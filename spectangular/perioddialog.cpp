#include "perioddialog.h"

#include "dataio.h"
#include "qcustomplot.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QSplitter>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace {

QDoubleSpinBox *spin(QWidget *p, const QString &name, double lo, double hi, int dec, double val)
{
    auto *s = new QDoubleSpinBox(p);
    s->setObjectName(name);
    s->setRange(lo, hi);
    s->setDecimals(dec);
    s->setValue(val);
    return s;
}

double median(std::vector<double> v)
{
    if (v.empty()) return 0.0;
    std::nth_element(v.begin(), v.begin() + long(v.size() / 2), v.end());
    return v[v.size() / 2];
}

} // namespace

PeriodDialog::PeriodDialog(QWidget *parent) : QDialog(parent)
{
    setObjectName("Period");
    setWindowTitle("Period analysis");
    auto *top = new QVBoxLayout(this);

    auto *gD = new QGroupBox("Data", this);
    auto *g = new QGridLayout(gD);
    cSource = new QComboBox(gD);
    cSource->setObjectName("comboBox_source");
    cSource->addItems({"Residual statistics of O-M", "Fit per spectrum: RV O-C, line strength",
                       "Statistics per line window"});
    cSource->setToolTip("Residual statistics: diffstatistic.dat (Spectra tab). Fit and line windows: files of the analysis "
                        "of the current solution (Output tab)");
    eStat = new QLineEdit(gD);
    eStat->setObjectName("lineEdit_stat");
    eStat->setToolTip("File with the values per spectrum (name in the Output tab for the selected source)");
    eTimes = new QLineEdit(gD);
    eTimes->setObjectName("lineEdit_times");
    eTimes->setToolTip("Times of the spectra (one per line), used if the file has no times");
    cColumn = new QComboBox(gD);
    cColumn->setObjectName("comboBox_column");
    cColumn->setMinimumContentsLength(24);
    cOutliers = new QCheckBox("Leave out outliers: more than", gD);
    cOutliers->setObjectName("checkBox_outliers");
    cOutliers->setChecked(true);
    cOutliers->setToolTip("Residual statistics: spectra whose RMS lies above the median by more than this many robust "
                          "standard deviations (e.g. a spectrum with a wrong correction). Other sources: values that "
                          "differ from the median by more than that, in either direction.");
    sClip = spin(gD, "doubleSpinBox_clip", 1, 100, 1, 5.0);
    sClip->setSuffix(" × 1.48 MAD");
    bAnalyse = new QPushButton("Analyse the current solution", gD);
    bAnalyse->setObjectName("pushButton_analyse");
    bAnalyse->setToolTip("Writes the files of the fit per spectrum and of the line windows for the current solution "
                         "(settings: Output tab, Analysis of the solution; needs a solution), then reads them again");
    g->addWidget(new QLabel("Source", gD), 0, 0);
    g->addWidget(cSource, 0, 1, 1, 2);
    g->addWidget(bAnalyse, 0, 3);
    g->addWidget(new QLabel("File", gD), 1, 0);
    g->addWidget(eStat, 1, 1);
    g->addWidget(new QLabel("Times", gD), 1, 2);
    g->addWidget(eTimes, 1, 3);
    g->addWidget(new QLabel("Values", gD), 2, 0);
    g->addWidget(cColumn, 2, 1);
    auto *ro = new QHBoxLayout();
    ro->addWidget(cOutliers);
    ro->addWidget(sClip);
    ro->addStretch(1);
    g->addLayout(ro, 2, 2, 1, 2);
    top->addWidget(gD);
    connect(cSource, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int) { sourceChanged(); });
    connect(bAnalyse, &QPushButton::clicked, this, [this]() { emit analyseRequested(); });

    auto *gF = new QGroupBox("Frequencies [1 / time unit of the times, e.g. c/d]", this);
    auto *f = new QGridLayout(gF);
    sFmin = spin(gF, "doubleSpinBox_fmin", 0, 1000, 4, 0.0);
    sFmax = spin(gF, "doubleSpinBox_fmax", 0.0001, 1000, 4, 1.0);
    sOver = spin(gF, "doubleSpinBox_over", 1, 1000, 0, 10);
    sOver->setToolTip("Frequency step 1 / (oversampling × time span)");
    cWindow = new QCheckBox("Spectral window", gF);
    cWindow->setObjectName("checkBox_window");
    cWindow->setChecked(true);
    cWindow->setToolTip("Periodogram of the sampling alone (grey): its peaks (e.g. 1 c/d, 1/year) give aliases");
    bCompute = new QPushButton("Compute", gF);
    bCompute->setObjectName("pushButton_compute");
    bWrite = new QPushButton("Write periodogram", gF);
    bWrite->setObjectName("pushButton_write");
    bWrite->setEnabled(false);
    f->addWidget(new QLabel("from", gF), 0, 0);
    f->addWidget(sFmin, 0, 1);
    f->addWidget(new QLabel("to", gF), 0, 2);
    f->addWidget(sFmax, 0, 3);
    f->addWidget(new QLabel("oversampling", gF), 0, 4);
    f->addWidget(sOver, 0, 5);
    f->addWidget(cWindow, 0, 6);
    f->addWidget(bCompute, 0, 7);
    f->addWidget(bWrite, 0, 8);
    f->setColumnStretch(9, 1);
    top->addWidget(gF);
    connect(bCompute, &QPushButton::clicked, this, &PeriodDialog::on_pushButton_compute_clicked);
    connect(bWrite, &QPushButton::clicked, this, &PeriodDialog::on_pushButton_write_clicked);

    plotP = new QCustomPlot(this);
    plotP->setObjectName("customPlot_periodogram");
    plotP->setMinimumHeight(240);
    plotP->xAxis->setLabel("Frequency");
    plotP->yAxis->setLabel("Power (GLS)");
    plotP->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    plotP->axisRect()->setRangeZoom(Qt::Horizontal);
    plotP->axisRect()->setRangeDrag(Qt::Horizontal);
    plotPhase = new QCustomPlot(this);
    plotPhase->setObjectName("customPlot_phase");
    plotPhase->setMinimumHeight(200);
    plotPhase->xAxis->setLabel("Phase");
    for (QCustomPlot *bp : {plotP, plotPhase}) {   // box around the plot, no ticks on the unused axes
        bp->xAxis2->setVisible(true);
        bp->xAxis2->setTickLabels(false);
        bp->xAxis2->setTicks(false);
        bp->yAxis2->setVisible(true);
        bp->yAxis2->setTickLabels(false);
        bp->yAxis2->setTicks(false);
    }
    table = new QTableWidget(0, 5, this);
    table->setObjectName("tableWidget_peaks");
    table->setHorizontalHeaderLabels({"Frequency", "Period", "Power", "FAP", "Amplitude"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setMaximumHeight(200);
    table->horizontalHeaderItem(4)->setToolTip("Semi-amplitude of the sine fit at the frequency");
    table->setToolTip("Highest peaks; select one to fold the data with its period");
    connect(table, &QTableWidget::currentCellChanged, this, [this](int r, int, int, int) { showPhase(r); });
    top->addWidget(plotP, 3);
    auto *low = new QHBoxLayout();
    low->addWidget(table, 1);
    low->addWidget(plotPhase, 1);
    top->addLayout(low, 2);
    labelResult = new QLabel(this);
    labelResult->setObjectName("labelResult");
    labelResult->setWordWrap(true);
    labelResult->setTextInteractionFlags(Qt::TextSelectableByMouse);
    top->addWidget(labelResult);
    resize(980, 820);
}

void PeriodDialog::setMainData(const MainData &d)
{
    md = d;
    eTimes->setText(d.timesFile);
    sourceChanged();
}

QString PeriodDialog::activeFile() const
{
    switch (cSource->currentIndex()) {
    case 1: return md.fitFile;
    case 2: return md.lineStatFile;
    default: return md.statFile;
    }
}

void PeriodDialog::sourceChanged()
{
    eStat->setText(activeFile());
    // list the quantities of the file (quietly: the file may not exist yet)
    QString keep = cColumn->currentText(), err;
    if (!loadData(err)) series.clear();
    fillValues(keep);
    if (!err.isEmpty()) labelResult->setText(err);
    else labelResult->clear();
}

void PeriodDialog::fillValues(const QString &keep)
{
    cColumn->clear();
    for (const Series &s : series) cColumn->addItem(s.name);
    int k = cColumn->findText(keep);
    if (k < 0) k = (cSource->currentIndex() == 0 && cColumn->count() > 1) ? 1 : 0;   // RMS of O-M by default
    if (cColumn->count() > 0) cColumn->setCurrentIndex(k);
}

void PeriodDialog::analysisFinished(bool ok)
{
    if (!ok) return;     // the main window has reported the error
    if (cSource->currentIndex() == 0) cSource->setCurrentIndex(1);   // show the new data
    else sourceChanged();
    if (series.empty()) return;
    labelResult->setText("Analysis written: " + md.fitFile + ", " + md.lineStatFile + ". Select the quantity and press Compute.");
}

// lines of a file: the last comment "# spectrum ..." is the header; data rows are split at tabs, or at white space
bool PeriodDialog::readRows(const QString &file, QStringList &head, std::vector<QStringList> &rows, QString &err)
{
    head.clear();
    rows.clear();
    QFile q(md.work + "/" + file);
    if (file.trimmed().isEmpty() || !q.open(QIODevice::ReadOnly | QIODevice::Text)) {
        err = cSource->currentIndex() == 0
              ? "cannot read the residual statistics " + q.fileName() + " (Spectra tab, \"Statistics\")."
              : "cannot read " + q.fileName() + " (\"Analyse the current solution\" writes it).";
        return false;
    }
    for (const QString &line : QString::fromUtf8(q.readAll()).split('\n')) {
        const QString l = line.trimmed();
        if (l.isEmpty()) continue;
        if (l.startsWith('#')) {
            const QStringList h = l.mid(1).trimmed().split(QRegularExpression("\\s+"));
            if (!h.isEmpty() && h[0] == "spectrum") head = h;
            continue;
        }
        rows.push_back(l.contains('\t') ? l.split('\t') : l.split(QRegularExpression("\\s+")));
    }
    if (rows.size() < 4) { err = file + ": fewer than 4 spectra."; return false; }
    return true;
}

// if a file has no times (zeros), they come from the times file (row = spectrum - first index)
bool PeriodDialog::timesFromFile(std::vector<Series> &ss, QString &err)
{
    std::vector<std::vector<double>> tr;
    std::string e;
    if (!dataio::readTable((md.work + "/" + eTimes->text().trimmed()).toStdString(), 1, tr, e)) {
        err = "the file has no times, and the times file " + eTimes->text() + " cannot be read: " + QString::fromStdString(e);
        return false;
    }
    for (Series &s : ss)
        for (size_t i = 0; i < s.spec.size(); ++i) {
            const int k = s.spec[i] - md.first;
            if (k < 0 || k >= int(tr.size())) {
                err = QString("no time for spectrum %1 in %2 (%3 lines, first index %4).")
                      .arg(s.spec[i]).arg(eTimes->text()).arg(tr.size()).arg(md.first);
                return false;
            }
            s.t[i] = tr[size_t(k)][0];
        }
    return true;
}

bool PeriodDialog::loadData(QString &err)
{
    series.clear();
    switch (cSource->currentIndex()) {
    case 1: return loadFit(err);
    case 2: return loadLines(err);
    default: return loadStatistics(err);
    }
}

// statistics file: "# spectrum <x> pixels mean rms std [time]"; time from the column "time", the
// abscissa if it is the time, or the times file
bool PeriodDialog::loadStatistics(QString &err)
{
    QStringList head;
    std::vector<QStringList> rows;
    if (!readRows(md.statFile, head, rows, err)) return false;
    int timeCol = -1;
    for (int c = 0; c < head.size(); ++c)
        if (head[c] == "time" && c != 1) timeCol = c;
    const bool xIsTime = head.size() > 1 && head[1] == "time";
    Series m, r, d;
    m.name = "mean of O-M";
    r.name = "RMS of O-M";
    d.name = "std of O-M";
    for (const QStringList &p : rows) {
        std::vector<double> v;
        for (const QString &x : p) {
            bool ok = false;
            v.push_back(x.toDouble(&ok));
            if (!ok) { err = "line \"" + p.join(' ') + "\" of " + md.statFile + " is not numeric."; return false; }
        }
        if (v.size() < 6) { err = "line \"" + p.join(' ') + "\" of " + md.statFile + " has fewer than 6 columns."; return false; }
        const int sp = int(std::lround(v[0]));
        const double tt = timeCol >= 0 && size_t(timeCol) < v.size() ? v[size_t(timeCol)] : (xIsTime ? v[1] : 0.0);
        for (Series *s : {&m, &r, &d}) { s->spec.push_back(sp); s->t.push_back(tt); }
        m.v.push_back(v[3]);
        r.v.push_back(v[4]);
        d.v.push_back(v[5]);
    }
    std::vector<Series> ss{m, r, d};
    if (timeCol < 0 && !xIsTime && !timesFromFile(ss, err)) return false;
    for (Series &s : ss) s.clip = ss[1].v;
    series = ss;
    return true;
}

// fit per spectrum (rv_oc.dat): O-C of the RV and strength with standard errors; weights 1/err^2
bool PeriodDialog::loadFit(QString &err)
{
    QStringList head;
    std::vector<QStringList> rows;
    if (!readRows(md.fitFile, head, rows, err)) return false;
    struct Col { const char *key, *name; };
    const Col cols[] = {{"dRV_A", "RV O-C of A [km/s]"}, {"S_A", "Strength of the lines of A"},
                        {"dRV_B", "RV O-C of B [km/s]"}, {"S_B", "Strength of the lines of B"}};
    const int tc = head.indexOf("time");
    if (head.isEmpty() || tc < 0) { err = md.fitFile + ": no header line \"# spectrum time ...\"."; return false; }
    std::vector<Series> ss;
    for (const Col &c : cols) {
        const int k = head.indexOf(c.key);
        if (k < 0 || k + 1 >= head.size()) continue;      // no B in an SB1
        Series s;
        s.name = c.name;
        for (const QStringList &p : rows) {
            if (p.size() <= k + 1) continue;
            bool o1 = false, o2 = false, o3 = false, o4 = false;
            const double v = p[k].toDouble(&o1), e = p[k + 1].toDouble(&o2);
            const double tt = p[tc].toDouble(&o3), sp = p[0].toDouble(&o4);
            if (!(o1 && o2 && o3 && o4) || !std::isfinite(v)) continue;     // failed fits are left out
            s.spec.push_back(int(std::lround(sp)));
            s.t.push_back(tt);
            s.v.push_back(v);
            s.sigma.push_back(e);
        }
        ss.push_back(s);
    }
    if (ss.empty() || ss[0].v.size() < 4) { err = md.fitFile + ": fewer than 4 spectra with a fit."; return false; }
    bool noTimes = true;
    for (double tt : ss[0].t) if (tt != 0.0) noTimes = false;
    if (noTimes && !timesFromFile(ss, err)) return false;
    series = ss;
    return true;
}

// line windows (linestat.dat): spectrum time phase line from to pixels mean rms EW
bool PeriodDialog::loadLines(QString &err)
{
    QStringList head;
    std::vector<QStringList> rows;
    if (!readRows(md.lineStatFile, head, rows, err)) return false;
    const int ti = head.indexOf("time"), li = head.indexOf("line"), mi = head.indexOf("mean"),
              ri = head.indexOf("rms"), ei = head.indexOf("EW");
    if (ti < 0 || li < 0 || mi < 0 || ri < 0 || ei < 0) { err = md.lineStatFile + ": unexpected header."; return false; }
    QStringList labels;
    std::vector<Series> ss;       // per label: mean, rms, EW
    for (const QStringList &p : rows) {
        if (p.size() <= std::max({ti, li, mi, ri, ei})) continue;
        const QString lab = p[li].trimmed();
        int k = labels.indexOf(lab);
        if (k < 0) {
            labels << lab;
            for (const char *q : {"mean", "RMS", "EW [A]"}) {
                Series s;
                s.name = lab + ": " + q;
                ss.push_back(s);
            }
            k = labels.size() - 1;
        }
        bool o[5];
        const int sp = int(std::lround(p[0].toDouble(&o[0])));
        const double tt = p[ti].toDouble(&o[1]), me = p[mi].toDouble(&o[2]), rm = p[ri].toDouble(&o[3]), ew = p[ei].toDouble(&o[4]);
        if (!(o[0] && o[1] && o[2] && o[3] && o[4])) continue;
        const double vals[3] = {me, rm, ew};
        for (int q = 0; q < 3; ++q) {
            Series &s = ss[size_t(3 * k + q)];
            s.spec.push_back(sp);
            s.t.push_back(tt);
            s.v.push_back(vals[q]);
        }
    }
    bool noTimes = true;
    for (const Series &s : ss)
        for (double tt : s.t) if (tt != 0.0) noTimes = false;
    if (noTimes && !timesFromFile(ss, err)) return false;
    // lines with fewer than 4 spectra cannot be analysed
    for (const Series &s : ss) if (s.v.size() >= 4) series.push_back(s);
    if (series.empty()) { err = md.lineStatFile + ": no line with at least 4 spectra."; return false; }
    return true;
}

void PeriodDialog::on_pushButton_compute_clicked()
{
    QString err;
    const QString keep = cColumn->currentText();
    if (!loadData(err)) {
        QMessageBox::information(this, "Error", "Period analysis: " + err);
        return;
    }
    fillValues(keep);
    const int col = cColumn->currentIndex();
    if (col < 0 || col >= int(series.size())) {
        QMessageBox::information(this, "Error", "Period analysis: no quantity selected.");
        return;
    }
    const Series &S = series[size_t(col)];
    // outliers: statistics by the RMS above the median, other sources by the distance from the median
    std::vector<char> out(S.v.size(), 0);
    QStringList left;
    if (cOutliers->isChecked()) {
        const bool byRms = !S.clip.empty();
        const std::vector<double> &x = byRms ? S.clip : S.v;
        const double med = median(x);
        std::vector<double> ad;
        for (double v : x) ad.push_back(std::fabs(v - med));
        const double sig = 1.4826 * median(ad);
        for (size_t i = 0; i < x.size(); ++i) {
            const double dev = byRms ? x[i] - med : std::fabs(x[i] - med);
            if (sig > 0 && dev > sClip->value() * sig) {
                out[i] = 1;
                left << QString::number(S.spec[i]);
            }
        }
    }
    t.clear();
    y.clear();
    wgt.clear();
    usedSpec.clear();
    bool weighted = !S.sigma.empty();
    for (size_t i = 0; i < S.v.size(); ++i)
        if (!out[i] && weighted && !(S.sigma[i] > 0)) weighted = false;
    for (size_t i = 0; i < S.v.size(); ++i)
        if (!out[i]) {
            t.push_back(S.t[i]);
            y.push_back(S.v[i]);
            usedSpec.push_back(S.spec[i]);
            if (weighted) wgt.push_back(1.0 / (S.sigma[i] * S.sigma[i]));
        }
    std::string e;
    if (!period::gls(t, y, wgt, sFmin->value(), sFmax->value(), sOver->value(), pg, e)) {
        QMessageBox::information(this, "Error", "Period analysis: " + QString::fromStdString(e));
        return;
    }
    const int n = int(t.size());
    peakIdx = period::peaks(pg, 5);

    // periodogram, window, FAP levels, orbital frequency and harmonics
    plotP->clearGraphs();
    plotP->clearItems();
    QVector<double> qf(int(pg.freq.size())), qp(qf.size()), qw(qf.size());
    for (int k = 0; k < qf.size(); ++k) { qf[k] = pg.freq[size_t(k)]; qp[k] = pg.power[size_t(k)]; qw[k] = pg.window[size_t(k)]; }
    const double fLo = qf.front(), fHi = qf.back();
    if (cWindow->isChecked()) {
        plotP->addGraph();
        plotP->graph()->setData(qf, qw);
        plotP->graph()->setPen(QPen(QColor(170, 170, 170), 1));
        plotP->graph()->setName("spectral window");
    }
    plotP->addGraph();
    plotP->graph()->setData(qf, qp);
    plotP->graph()->setPen(QPen(QColor(31, 119, 180), 1.3));
    plotP->graph()->setName("GLS power");
    for (double lev : {0.01, 0.001}) {
        const double p = period::powerAtFap(lev, n, pg.nIndependent);
        plotP->addGraph();
        plotP->graph()->setData(QVector<double>{fLo, fHi}, QVector<double>{p, p});
        plotP->graph()->setPen(QPen(Qt::gray, 1, lev > 0.005 ? Qt::DashLine : Qt::DotLine));
        plotP->graph()->setName(QString("FAP %1 %").arg(100 * lev));
    }
    if (md.orbitalPeriod > 0) {
        bool named = false;
        for (int h = 1; h * (1.0 / md.orbitalPeriod) <= fHi && h <= 50; ++h) {
            const double fo = h / md.orbitalPeriod;
            if (fo < fLo) continue;
            plotP->addGraph();
            plotP->graph()->setData(QVector<double>{fo, fo}, QVector<double>{0, 1});
            plotP->graph()->setPen(QPen(QColor(214, 39, 40, 140), 1, Qt::DashLine));
            if (!named) { plotP->graph()->setName("orbital frequency × n"); named = true; }
            else plotP->graph()->removeFromLegend();
        }
    }
    plotP->legend->setVisible(true);
    plotP->legend->setFont(QFont(font().family(), font().pointSize() - 1));
    plotP->xAxis->setRange(fLo, fHi);
    double top = 0.0;
    for (double p : pg.power) top = std::max(top, p);
    // room for the legend above the peaks (the window, up to 1 near f = 0, is cut)
    plotP->yAxis->setRange(0.0, std::min(1.05, std::max(top, period::powerAtFap(0.001, n, pg.nIndependent)) * 1.6));
    plotP->replot();

    // peaks
    table->setRowCount(int(peakIdx.size()));
    for (int r = 0; r < int(peakIdx.size()); ++r) {
        const size_t k = peakIdx[size_t(r)];
        const double fq = pg.freq[k];
        const period::Sine s = period::sineFit(t, y, wgt, fq);
        const double fa = period::fap(pg.power[k], n, pg.nIndependent);
        table->setItem(r, 0, new QTableWidgetItem(QString::number(fq, 'f', 5)));
        table->setItem(r, 1, new QTableWidgetItem(fq > 0 ? QString::number(1.0 / fq, 'g', 6) : QString("-")));
        table->setItem(r, 2, new QTableWidgetItem(QString::number(pg.power[k], 'f', 3)));
        table->setItem(r, 3, new QTableWidgetItem(fa < 1e-4 ? QString::number(fa, 'e', 1) : QString::number(fa, 'f', 4)));
        table->setItem(r, 4, new QTableWidgetItem(QString::number(s.amplitude, 'g', 4)));
    }
    if (!peakIdx.empty()) table->setCurrentCell(0, 0);
    bWrite->setEnabled(true);
    QString res = QString("%1 spectra, time span %2, %3 frequencies (%4 independent), values: %5")
                  .arg(n).arg(pg.tspan, 0, 'g', 6).arg(pg.freq.size()).arg(pg.nIndependent)
                  .arg(S.name) + (weighted ? " (weighted with the errors)" : "");
    if (!left.isEmpty()) res += ". Left out (outliers): spectrum " + left.join(", ");
    if (!peakIdx.empty()) {
        const size_t k = peakIdx[0];
        const double fa = period::fap(pg.power[k], n, pg.nIndependent);
        res += QString(". Highest peak: f = %1 (P = %2), power %3, FAP %4%5")
               .arg(pg.freq[k], 0, 'f', 5).arg(pg.freq[k] > 0 ? 1.0 / pg.freq[k] : 0.0, 0, 'g', 6)
               .arg(pg.power[k], 0, 'f', 3).arg(fa, 0, 'g', 2)
               .arg(fa > 0.01 ? " – not significant" : "");
        if (md.orbitalPeriod > 0) {
            const double h = pg.freq[k] * md.orbitalPeriod;
            if (std::lround(h) >= 1 && std::fabs(h - std::lround(h)) * pg.tspan / md.orbitalPeriod < 1.0)
                res += QString(" (= %1 × orbital frequency)").arg(std::lround(h));
        }
    }
    labelResult->setText(res + ".");
}

void PeriodDialog::showPhase(int r)
{
    plotPhase->clearGraphs();
    if (r < 0 || r >= int(peakIdx.size()) || t.empty()) { plotPhase->replot(); return; }
    const double fq = pg.freq[peakIdx[size_t(r)]];
    if (!(fq > 0)) { plotPhase->replot(); return; }
    const period::Sine s = period::sineFit(t, y, wgt, fq);
    QVector<double> px, py;
    for (size_t i = 0; i < t.size(); ++i) {
        double ph = std::fmod((t[i] - s.tMax) * fq, 1.0);
        if (ph < 0) ph += 1.0;
        for (double o : {0.0, 1.0}) { px << ph + o; py << y[i]; }   // two cycles
    }
    plotPhase->addGraph();
    plotPhase->graph()->setData(px, py);
    plotPhase->graph()->setLineStyle(QCPGraph::lsNone);
    plotPhase->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(31, 119, 180), 6));
    QVector<double> sx, sy;
    for (int k = 0; k <= 200; ++k) {
        const double ph = 2.0 * k / 200.0;
        sx << ph;
        sy << s.offset + s.amplitude * std::cos(6.283185307179586 * ph);
    }
    plotPhase->addGraph();
    plotPhase->graph()->setData(sx, sy);
    plotPhase->graph()->setPen(QPen(QColor(214, 39, 40), 1.3));
    plotPhase->xAxis->setLabel(QString("Phase (P = %1, phase 0 at the maximum)").arg(1.0 / fq, 0, 'g', 6));
    plotPhase->yAxis->setLabel(cColumn->currentText());
    plotPhase->rescaleAxes();
    plotPhase->xAxis->setRange(0.0, 2.0);
    const double yr = plotPhase->yAxis->range().size();
    plotPhase->yAxis->setRange(plotPhase->yAxis->range().lower - 0.08 * yr, plotPhase->yAxis->range().upper + 0.08 * yr);
    plotPhase->replot();
}

void PeriodDialog::on_pushButton_write_clicked()
{
    if (pg.freq.empty()) return;
    QString base = QFileInfo(activeFile().trimmed()).completeBaseName();
    if (base.isEmpty()) base = "diffstatistic";
    if (cSource->currentIndex() == 2) base += "_" + cColumn->currentText().replace(QRegularExpression("[^A-Za-z0-9]+"), "_");
    const QString name = base + "_periodogram.dat";
    std::ofstream o((md.work + "/" + name).toStdString());
    o << "# GLS periodogram of " << cColumn->currentText().toStdString() << " per spectrum ("
      << t.size() << " spectra";
    if (!usedSpec.empty()) {
        o << ": ";
        for (size_t i = 0; i < usedSpec.size(); ++i) o << (i ? " " : "") << usedSpec[i];
    }
    o << ")\n# frequency\tperiod\tpower\twindow\n" << std::setprecision(10);
    for (size_t k = 0; k < pg.freq.size(); ++k)
        o << pg.freq[k] << "\t" << (pg.freq[k] > 0 ? 1.0 / pg.freq[k] : 0.0) << "\t" << pg.power[k] << "\t" << pg.window[k] << "\n";
    o.close();
    if (!o) {
        QMessageBox::information(this, "Error", "Period analysis: cannot write " + md.work + "/" + name + ".");
        return;
    }
    labelResult->setText(labelResult->text() + " Written to " + name + ".");
}
