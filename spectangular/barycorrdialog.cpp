#include "barycorrdialog.h"

#include "barycorr.h"
#include "dataio.h"

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDoubleSpinBox>
#include <QFileInfo>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QTableWidget>
#include <QVBoxLayout>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace {

struct Observatory { const char *name; double lat, lon, h; };
// geodetic coordinates (lon east positive); 0.01 deg changes the correction by < 1 m/s
const Observatory kSites[] = {
    {"STELLA / Teide Observatory, Tenerife", 28.3010, -16.5100, 2390},
    {"Roque de los Muchachos, La Palma", 28.7606, -17.8816, 2396},
    {"Calar Alto", 37.2236, -2.5463, 2168},
    {"Haute-Provence", 43.9317, 5.7122, 650},
    {"Ondřejov", 49.9106, 14.7836, 528},
    {"Tautenburg", 50.9800, 11.7111, 341},
    {"La Silla", -29.2563, -70.7380, 2400},
    {"Paranal", -24.6275, -70.4044, 2635},
    {"McDonald", 30.6714, -104.0225, 2075},
    {"Mauna Kea", 19.8238, -155.4690, 4205},
};
const int kNSites = int(sizeof(kSites) / sizeof(kSites[0]));
const double C_KMS = 299792.458;

QDoubleSpinBox *spin(QWidget *p, const QString &name, double lo, double hi, int dec, const QString &suffix)
{
    auto *s = new QDoubleSpinBox(p);
    s->setObjectName(name);
    s->setRange(lo, hi);
    s->setDecimals(dec);
    s->setSuffix(suffix);
    return s;
}

} // namespace

BaryCorrDialog::BaryCorrDialog(QWidget *parent) : QDialog(parent)
{
    setObjectName("BaryCorr");
    setWindowTitle("Barycentric correction");
    auto *top = new QVBoxLayout(this);

    auto *gT = new QGroupBox("Star and observatory", this);
    auto *g = new QGridLayout(gT);
    eRa = new QLineEdit(gT);
    eRa->setObjectName("lineEdit_ra");
    eRa->setToolTip("Right ascension (ICRS / J2000): \"02 12 22.28\" or \"02:12:22.28\" (hours) or degrees");
    eDec = new QLineEdit(gT);
    eDec->setObjectName("lineEdit_dec");
    eDec->setToolTip("Declination (ICRS / J2000): \"+30 18 11.1\" or degrees");
    g->addWidget(new QLabel("RA", gT), 0, 0);
    g->addWidget(eRa, 0, 1);
    g->addWidget(new QLabel("Dec", gT), 0, 2);
    g->addWidget(eDec, 0, 3);
    site = new QComboBox(gT);
    site->setObjectName("comboBox_site");
    for (int i = 0; i < kNSites; ++i) site->addItem(QString::fromUtf8(kSites[i].name));
    site->addItem("Other (enter coordinates)");
    lat = spin(gT, "doubleSpinBox_lat", -90, 90, 4, "°");
    lon = spin(gT, "doubleSpinBox_lon", -180, 360, 4, "°");
    height = spin(gT, "doubleSpinBox_height", -500, 6000, 0, " m");
    lat->setToolTip("Geodetic latitude, north positive");
    lon->setToolTip("Longitude, east positive (west negative)");
    g->addWidget(new QLabel("Observatory", gT), 1, 0);
    g->addWidget(site, 1, 1, 1, 3);
    g->addWidget(new QLabel("Latitude", gT), 2, 0);
    g->addWidget(lat, 2, 1);
    g->addWidget(new QLabel("Longitude", gT), 2, 2);
    g->addWidget(lon, 2, 3);
    g->addWidget(new QLabel("Height", gT), 3, 0);
    g->addWidget(height, 3, 1);
    connect(site, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BaryCorrDialog::siteChanged);
    top->addWidget(gT);

    auto *gS = new QGroupBox("Spectra and times", this);
    auto *gs = new QGridLayout(gS);
    labelSpectra = new QLabel(gS);
    labelSpectra->setWordWrap(true);
    gs->addWidget(labelSpectra, 0, 0, 1, 4);
    eTimes = new QLineEdit(gS);
    eTimes->setObjectName("lineEdit_times");
    eTimes->setToolTip("Times of observation (mid-exposure), one line per spectrum, in the work folder");
    scale = new QComboBox(gS);
    scale->setObjectName("comboBox_scale");
    scale->addItems({"JD (UTC)", "MJD (UTC)", "HJD (UTC)", "BJD (TDB)"});
    scale->setCurrentIndex(2);
    gs->addWidget(new QLabel("Times file", gS), 1, 0);
    gs->addWidget(eTimes, 1, 1);
    gs->addWidget(new QLabel("Time scale", gS), 1, 2);
    gs->addWidget(scale, 1, 3);
    eOut = new QLineEdit(gS);
    eOut->setObjectName("lineEdit_out");
    eOut->setToolTip("File name of the corrected spectra: <prefix><index>.txt in the work folder (ASCII)");
    gs->addWidget(new QLabel("Output prefix", gS), 2, 0);
    gs->addWidget(eOut, 2, 1);
    cRv = new QCheckBox("Correct the start RVs as well", gS);
    cRv->setObjectName("checkBox_rv");
    cRv->setToolTip("Writes the RV file of the Data tab with RV + BC per spectrum as <name>_bc.<ext>; "
                    "RVs measured on the uncorrected spectra then fit the corrected ones.");
    gs->addWidget(cRv, 2, 2, 1, 2);
    eBc = new QLineEdit(gS);
    eBc->setObjectName("lineEdit_bcfile");
    eBc->setToolTip("Table of the corrections (spectrum, time, JD_UTC, BC [km/s]) in the work folder, written by "
                    "\"Write BC file\"; the Data tab reads the last number of each line");
    gs->addWidget(new QLabel("BC file", gS), 3, 0);
    gs->addWidget(eBc, 3, 1);
    cCorrected = new QCheckBox("Spectra already corrected", gS);
    cCorrected->setObjectName("checkBox_corrected");
    cCorrected->setToolTip("The spectra of the main window are already barycentrically corrected (e.g. by the "
                           "reduction): the Data tab then shifts the static lines by BC. Unticked: spectra in the "
                           "observatory frame, the RVs of the orbit are converted.");
    gs->addWidget(cCorrected, 3, 2, 1, 2);
    eAm = new QLineEdit(gS);
    eAm->setObjectName("lineEdit_airmass");
    eAm->setToolTip("Airmass of every spectrum (one value per line), written with the BC file: start values of the "
                    "strength of the static (telluric) lines, Data tab");
    gs->addWidget(new QLabel("Airmass file", gS), 4, 0);
    gs->addWidget(eAm, 4, 1);
    top->addWidget(gS);

    auto *buttons = new QHBoxLayout;
    bCompute = new QPushButton("Compute", this);
    bCompute->setObjectName("pushButton_compute");
    bWrite = new QPushButton("Write corrected spectra", this);
    bWrite->setObjectName("pushButton_write");
    bUse = new QPushButton("Use in main window", this);
    bUse->setObjectName("pushButton_use");
    bUse->setToolTip("Sets file name, format and (if written) RV file of the Data tab to the corrected files");
    bWriteBc = new QPushButton("Write BC file", this);
    bWriteBc->setObjectName("pushButton_writebc");
    bWriteBc->setToolTip("Writes only the table of the corrections (BC file) and sets it in the Data tab, with the "
                         "frame of the spectra (observatory, or corrected if ticked); the spectra are not changed");
    auto *bClose = new QPushButton("Close", this);
    buttons->addWidget(bCompute);
    buttons->addWidget(bWriteBc);
    buttons->addWidget(bWrite);
    buttons->addWidget(bUse);
    buttons->addStretch();
    buttons->addWidget(bClose);
    top->addLayout(buttons);
    bWrite->setEnabled(false);
    bUse->setEnabled(false);
    connect(bCompute, &QPushButton::clicked, this, &BaryCorrDialog::on_pushButton_compute_clicked);
    connect(bWrite, &QPushButton::clicked, this, &BaryCorrDialog::on_pushButton_write_clicked);
    connect(bUse, &QPushButton::clicked, this, &BaryCorrDialog::on_pushButton_use_clicked);
    connect(bWriteBc, &QPushButton::clicked, this, &BaryCorrDialog::on_pushButton_writebc_clicked);
    connect(bClose, &QPushButton::clicked, this, &QDialog::close);

    // check of the telluric lines: their position in every spectrum against the correction
    auto *gC = new QGroupBox("Check of the telluric lines", this);
    auto *gc = new QGridLayout(gC);
    auto *lc = new QLabel("Position of the strong telluric bands (O2 B, O2 A, H2O 7200, 8200, 9300 Å) in every "
                          "spectrum, from the cross-correlation with the first one, against the shift expected "
                          "from the correction (BC_i − BC_0 for corrected spectra, 0 in the observatory frame; "
                          "\"Spectra already corrected\"). A deviation means a wrong correction, time or "
                          "wavelength scale of the spectrum; its stellar lines are shifted as well.", gC);
    lc->setWordWrap(true);
    gc->addWidget(lc, 0, 0, 1, 5);
    tellLimit = spin(gC, "doubleSpinBox_telllimit", 0.1, 100, 1, " km/s");
    tellLimit->setValue(2.0);
    tellLimit->setToolTip("Spectra whose deviation exceeds this (or without correlation, r < 0.3) are marked for "
                          "rejection; the marks can be changed in the column Reject");
    bTellCheck = new QPushButton("Check telluric lines", gC);
    bTellCheck->setObjectName("pushButton_tellcheck");
    bReject = new QPushButton("Reject marked spectra", gC);
    bReject->setObjectName("pushButton_reject");
    bReject->setToolTip("Renames the marked spectra to <prefix>bc_rejected_<index><ext>, numbers the others "
                        "consecutively and removes their lines from the times file, the RV file, the per-spectrum "
                        "files of the Data tab and edit.dat (copies of the old files: *.before_bc_reject); "
                        "log in bc_rejected.txt");
    bReject->setEnabled(false);
    gc->addWidget(new QLabel("Limit", gC), 1, 0);
    gc->addWidget(tellLimit, 1, 1);
    gc->addWidget(bTellCheck, 1, 2);
    gc->addWidget(bReject, 1, 3);
    gc->setColumnStretch(4, 1);
    top->addWidget(gC);
    connect(bTellCheck, &QPushButton::clicked, this, &BaryCorrDialog::on_pushButton_tellcheck_clicked);
    connect(bReject, &QPushButton::clicked, this, &BaryCorrDialog::on_pushButton_reject_clicked);
    connect(tellLimit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double) {
        if (!tellDev.empty()) markRejects();
    });

    labelResult = new QLabel(this);
    labelResult->setObjectName("labelResult");
    labelResult->setWordWrap(true);
    top->addWidget(labelResult);
    table = new QTableWidget(0, 9, this);
    table->setObjectName("tableWidget_bc");
    table->setHorizontalHeaderLabels({"Spectrum", "Time", "JD (UTC)", "BC [km/s]", "Airmass",
                                      "Tell. shift [km/s]", "Deviation [km/s]", "r", "Reject"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeaderItem(5)->setToolTip("Shift of the telluric lines against the first spectrum");
    table->horizontalHeaderItem(6)->setToolTip("Shift minus the expected one (BC_i - BC_0 or 0), minus the median");
    table->horizontalHeaderItem(7)->setToolTip("Correlation coefficient at the peak");
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    top->addWidget(table, 1);

    // last values
    QSettings st("Spectangular", "barycorr");
    eRa->setText(st.value("ra").toString());
    eDec->setText(st.value("dec").toString());
    const int si = st.value("site", 0).toInt();
    lat->setValue(st.value("lat", kSites[0].lat).toDouble());
    lon->setValue(st.value("lon", kSites[0].lon).toDouble());
    height->setValue(st.value("height", kSites[0].h).toDouble());
    site->setCurrentIndex(std::max(0, std::min(si, kNSites)));
    siteChanged(site->currentIndex());
    scale->setCurrentIndex(st.value("scale", 2).toInt());
    resize(900, 820);
}

void BaryCorrDialog::siteChanged(int i)
{
    const bool other = i >= kNSites;
    if (!other) {
        lat->setValue(kSites[i].lat);
        lon->setValue(kSites[i].lon);
        height->setValue(kSites[i].h);
    }
    lat->setEnabled(other);
    lon->setEnabled(other);
    height->setEnabled(other);
}

void BaryCorrDialog::setMainData(const MainData &d)
{
    md = d;
    labelSpectra->setText(QString("Spectra %1%2%3 to %1%4%3 (%5) in %6")
                          .arg(d.prefix).arg(d.first).arg(d.ext).arg(d.last)
                          .arg(d.fits ? "FITS; written as ASCII" : "ASCII").arg(d.work));
    eTimes->setText(d.timesFile);
    // time scale from the file name, if it says so (e.g. 6TriA_HJD_times.dat)
    const QString tl = QFileInfo(d.timesFile).fileName().toLower();
    if (tl.contains("bjd")) scale->setCurrentIndex(3);
    else if (tl.contains("hjd")) scale->setCurrentIndex(2);
    else if (tl.contains("mjd")) scale->setCurrentIndex(1);
    eOut->setText("bc_" + d.prefix);
    eBc->setText(d.prefix + "barycorr.txt");
    eAm->setText(d.prefix + "airmass.txt");
    cRv->setEnabled(!d.rvFile.isEmpty());
    cRv->setChecked(!d.rvFile.isEmpty() && QFileInfo::exists(QDir(d.work).filePath(d.rvFile)));
    if (!d.rvFile.isEmpty()) cRv->setText("Correct the start RVs as well (" + d.rvFile + ")");
}

bool BaryCorrDialog::compute(QString &err)
{
    double ra, dec;
    if (!bc::parseRa(eRa->text().trimmed().toStdString(), ra)) { err = "Invalid right ascension."; return false; }
    if (!bc::parseDec(eDec->text().trimmed().toStdString(), dec)) { err = "Invalid declination."; return false; }
    std::vector<std::vector<double>> rows;
    std::string e;
    if (!dataio::readTable(QDir(md.work).filePath(eTimes->text().trimmed()).toStdString(), 1, rows, e)) {
        err = QString::fromStdString(e);
        return false;
    }
    const int n = md.last - md.first + 1;
    if (int(rows.size()) != n) {
        err = QString("The times file has %1 lines, but there are %2 spectra (%3 to %4).")
              .arg(rows.size()).arg(n).arg(md.first).arg(md.last);
        return false;
    }
    bc::Site s;
    s.latDeg = lat->value();
    s.lonDeg = lon->value();
    s.heightM = height->value();
    times.assign(n, 0);
    jdUtc.assign(n, 0);
    bcs.assign(n, 0);
    airmasses.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        times[i] = rows[i][0];
        double alt = 0;
        const bool ok = bc::correction(ra, dec, s, times[i], bc::TimeScale(scale->currentIndex()), bcs[i], &jdUtc[i], e, &alt);
        airmasses[i] = bc::airmass(alt);
        if (!ok) {
            err = QString("Spectrum %1, time %2: %3").arg(md.first + i).arg(times[i], 0, 'f', 6).arg(QString::fromStdString(e));
            return false;
        }
    }
    QSettings st("Spectangular", "barycorr");
    st.setValue("ra", eRa->text());
    st.setValue("dec", eDec->text());
    st.setValue("site", site->currentIndex());
    st.setValue("lat", lat->value());
    st.setValue("lon", lon->value());
    st.setValue("height", height->value());
    st.setValue("scale", scale->currentIndex());
    return true;
}

void BaryCorrDialog::on_pushButton_compute_clicked()
{
    QString err;
    if (!compute(err)) {
        QMessageBox::information(this, "Error", "Barycentric correction: " + err);
        bWrite->setEnabled(false);
        return;
    }
    table->setRowCount(int(bcs.size()));
    double lo = 1e9, hi = -1e9;
    for (int i = 0; i < int(bcs.size()); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(md.first + i)));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(times[i], 'f', 6)));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(jdUtc[i], 'f', 6)));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(bcs[i], 'f', 4)));
        table->setItem(i, 4, new QTableWidgetItem(std::isfinite(airmasses[i]) ? QString::number(airmasses[i], 'f', 3)
                                                                              : QString("below horizon")));
        lo = std::min(lo, bcs[i]);
        hi = std::max(hi, bcs[i]);
    }
    tellShift.clear();
    tellDev.clear();
    tellCorr.clear();
    bReject->setEnabled(false);
    labelResult->setText(QString("Barycentric correction of %1 spectra: %2 to %3 km/s. Corrected: "
                                 "λ_bary = λ_obs (1 + BC/c), v_bary = v + BC.")
                         .arg(bcs.size()).arg(lo, 0, 'f', 3).arg(hi, 0, 'f', 3));
    bWrite->setEnabled(true);
}

void BaryCorrDialog::on_pushButton_write_clicked()
{
    QString err;
    if (!compute(err)) {
        QMessageBox::information(this, "Error", "Barycentric correction: " + err);
        return;
    }
    const QString out = eOut->text().trimmed();
    if (out.isEmpty() || (out == md.prefix && !md.fits && md.ext == ".txt")) {
        QMessageBox::information(this, "Error", "Barycentric correction: choose an output prefix different from the input.");
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    const int n = int(bcs.size());
    for (int i = 0; i < n; ++i) {
        std::vector<double> w, f;
        std::string e;
        if (!md.read || !md.read(md.first + i, w, f, e)) {
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "Error", "Barycentric correction: " + QString::fromStdString(e));
            return;
        }
        const double fac = 1.0 + bcs[i] / C_KMS;
        // log10(lambda) if all wavelengths <= 100 (as when reading the data), else Angstrom
        bool logScale = !w.empty();
        for (double x : w) if (x > 100) { logScale = false; break; }
        std::ofstream o(QDir(md.work).filePath(out + QString::number(md.first + i) + ".txt").toStdString());
        o << std::setprecision(15);
        for (size_t k = 0; k < w.size(); ++k)
            o << (logScale ? w[k] + std::log10(fac) : w[k] * fac) << "\t" << f[k] << "\n";
        if (!o) {
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "Error", "Barycentric correction: cannot write the corrected spectra.");
            return;
        }
    }
    // table of the corrections
    {
        QString e2;
        if (!writeTable(out + "barycorr.txt", e2)) {
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "Error", "Barycentric correction: " + e2);
            return;
        }
    }
    writtenRv.clear();
    if (cRv->isEnabled() && cRv->isChecked()) {
        std::vector<std::vector<double>> rows;
        std::string e;
        const QString in = QDir(md.work).filePath(md.rvFile);
        if (!dataio::readTable(in.toStdString(), md.rvColumns, rows, e) || int(rows.size()) != n) {
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "Error", "Barycentric correction: RV file " + md.rvFile
                                     + (e.empty() ? QString(" must have one line per spectrum.") : ": " + QString::fromStdString(e)));
            return;
        }
        QFileInfo fi(md.rvFile);
        writtenRv = fi.completeBaseName() + "_bc" + (fi.suffix().isEmpty() ? QString() : "." + fi.suffix());
        std::ofstream o(QDir(md.work).filePath(writtenRv).toStdString());
        o << std::setprecision(12);
        for (int i = 0; i < n; ++i) {
            for (size_t k = 0; k < rows[i].size(); ++k) o << (k ? " " : "") << rows[i][k] + bcs[i];
            o << "\n";
        }
    }
    QApplication::restoreOverrideCursor();
    writtenPrefix = out;
    writtenExt = ".txt";
    labelResult->setText(labelResult->text() + QString("\nWritten: %1%2.txt to %1%3.txt, %1barycorr.txt%4.")
                         .arg(out).arg(md.first).arg(md.last).arg(writtenRv.isEmpty() ? "" : ", " + writtenRv));
    bUse->setEnabled(true);
}

bool BaryCorrDialog::writeTable(const QString &file, QString &err) const
{
    std::ofstream o(QDir(md.work).filePath(file).toStdString());
    o << std::setprecision(12);
    o << "# Barycentric correction (Spectangular, ERFA): RA " << eRa->text().toStdString() << ", Dec "
      << eDec->text().toStdString() << "; " << site->currentText().toStdString() << ", lat " << lat->value()
      << ", lon " << lon->value() << ", h " << height->value() << " m; times " << scale->currentText().toStdString() << "\n";
    o << "# lambda_bary = lambda_obs * (1 + BC/c); v_bary = v + BC\n# spectrum\ttime\tJD_UTC\tBC [km/s]\n";
    for (size_t i = 0; i < bcs.size(); ++i) o << md.first + int(i) << "\t" << times[i] << "\t" << jdUtc[i] << "\t" << bcs[i] << "\n";
    if (!o) {
        err = "cannot write " + QDir(md.work).filePath(file) + ".";
        return false;
    }
    return true;
}

bool BaryCorrDialog::writeAirmass(const QString &file, QString &err) const
{
    for (size_t i = 0; i < airmasses.size(); ++i) {
        if (!std::isfinite(airmasses[i])) {
            err = QString("spectrum %1: star below the horizon at %2 (wrong time, coordinates or site?).")
                  .arg(md.first + int(i)).arg(times[i], 0, 'f', 6);
            return false;
        }
    }
    std::ofstream o(QDir(md.work).filePath(file).toStdString());
    o << std::setprecision(8);
    o << "# Airmass (Pickering 2002) at the times of the spectra; RA " << eRa->text().toStdString() << ", Dec "
      << eDec->text().toStdString() << "; " << site->currentText().toStdString() << "\n# airmass\t# spectrum\n";
    for (size_t i = 0; i < airmasses.size(); ++i) o << airmasses[i] << "\t# " << md.first + int(i) << "\n";
    if (!o) {
        err = "cannot write " + QDir(md.work).filePath(file) + ".";
        return false;
    }
    return true;
}

// Only the table of the corrections, for the Data tab (spectra unchanged)
void BaryCorrDialog::on_pushButton_writebc_clicked()
{
    QString err;
    if (!compute(err)) {
        QMessageBox::information(this, "Error", "Barycentric correction: " + err);
        return;
    }
    const QString f = eBc->text().trimmed();
    if (f.isEmpty()) {
        QMessageBox::information(this, "Error", "Barycentric correction: enter a name for the BC file.");
        return;
    }
    if (!writeTable(f, err)) {
        QMessageBox::information(this, "Error", "Barycentric correction: " + err);
        return;
    }
    const QString fa = eAm->text().trimmed();
    QString amNote;
    if (!fa.isEmpty()) {
        if (writeAirmass(fa, err)) amNote = ", " + fa;
        else amNote = " (no airmass file: " + err + ")";
    }
    labelResult->setText(labelResult->text() + QString("\nWritten: %1%4 (%2 spectra); set in the Data tab, spectra %3.")
                         .arg(f).arg(bcs.size()).arg(cCorrected->isChecked() ? "barycentrically corrected"
                                                                              : "in the observatory frame").arg(amNote));
    emit useBcFile(f, cCorrected->isChecked());
    if (amNote.startsWith(", ")) emit useAirmassFile(fa);
}

void BaryCorrDialog::on_pushButton_use_clicked()
{
    if (!writtenPrefix.isEmpty()) emit useInMain(writtenPrefix, writtenExt, writtenRv);
}

// Telluric lines of every spectrum against the correction (bc::telluricShifts)
void BaryCorrDialog::on_pushButton_tellcheck_clicked()
{
    on_pushButton_compute_clicked();
    if (bcs.empty()) return;
    const int n = int(bcs.size());
    QApplication::setOverrideCursor(Qt::WaitCursor);
    std::vector<std::vector<double>> lw(static_cast<size_t>(n)), fl(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        std::string e;
        if (!md.read || !md.read(md.first + i, lw[size_t(i)], fl[size_t(i)], e)) {
            QApplication::restoreOverrideCursor();
            QMessageBox::information(this, "Error", "Check of the telluric lines: " + QString::fromStdString(e));
            return;
        }
        bool logScale = true;
        for (double x : lw[size_t(i)]) if (x > 100) { logScale = false; break; }
        if (!logScale) for (double &x : lw[size_t(i)]) x = std::log10(x);
    }
    const bool corrected = cCorrected->isChecked();
    double maxExp = 0.0;
    std::vector<double> expd(size_t(n), 0.0);
    for (int i = 0; i < n; ++i) {
        expd[size_t(i)] = corrected ? bcs[size_t(i)] - bcs[0] : 0.0;
        maxExp = std::max(maxExp, std::fabs(expd[size_t(i)]));
    }
    bc::TelluricShifts ts;
    std::string e;
    const bool ok = bc::telluricShifts(lw, fl, maxExp + 50.0, ts, e);
    QApplication::restoreOverrideCursor();
    if (!ok) {
        QMessageBox::information(this, "Error", "Check of the telluric lines: " + QString::fromStdString(e));
        return;
    }
    // deviation from the expected shift, minus its median (the first spectrum may be off itself)
    tellShift = ts.shiftKms;
    tellCorr = ts.corr;
    tellDev.assign(size_t(n), 0.0);
    std::vector<double> d(static_cast<size_t>(n), 0.0);
    for (int i = 0; i < n; ++i) d[size_t(i)] = tellShift[size_t(i)] - expd[size_t(i)];
    std::vector<double> sd = d;
    std::nth_element(sd.begin(), sd.begin() + n / 2, sd.end());
    const double med = sd[size_t(n / 2)];
    for (int i = 0; i < n; ++i) tellDev[size_t(i)] = d[size_t(i)] - med;
    for (int i = 0; i < n; ++i) {
        table->setItem(i, 5, new QTableWidgetItem(QString::number(tellShift[size_t(i)], 'f', 2)));
        table->setItem(i, 6, new QTableWidgetItem(QString::number(tellDev[size_t(i)], 'f', 2)));
        table->setItem(i, 7, new QTableWidgetItem(QString::number(tellCorr[size_t(i)], 'f', 2)));
        auto *rj = new QTableWidgetItem();
        rj->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        table->setItem(i, 8, rj);
    }
    markRejects();
    QStringList bad;
    for (int i = 0; i < n; ++i)
        if (table->item(i, 8)->checkState() == Qt::Checked)
            bad << QString("%1 (%2 km/s)").arg(md.first + i).arg(tellDev[size_t(i)], 0, 'f', 1);
    std::vector<double> ad;
    for (int i = 0; i < n; ++i) if (std::fabs(tellDev[size_t(i)]) <= tellLimit->value()) ad.push_back(std::fabs(tellDev[size_t(i)]));
    std::sort(ad.begin(), ad.end());
    const double typ = ad.empty() ? 0.0 : ad[ad.size() / 2];
    labelResult->setText(labelResult->text() + QString("\nTelluric lines (bands %1 Å, %2): typical deviation %3 km/s; ")
                         .arg(QString::fromStdString(ts.bands)).arg(corrected ? "expected BC_i − BC_0" : "expected 0")
                         .arg(typ, 0, 'f', 2)
                         + (bad.isEmpty() ? QString("all spectra within %1 km/s.").arg(tellLimit->value())
                                          : QString("beyond %1 km/s or without correlation: %2.").arg(tellLimit->value()).arg(bad.join(", "))));
    bReject->setEnabled(true);
}

void BaryCorrDialog::markRejects()
{
    for (int i = 0; i < table->rowCount() && i < int(tellDev.size()); ++i) {
        const bool bad = std::fabs(tellDev[size_t(i)]) > tellLimit->value() || tellCorr[size_t(i)] < 0.3;
        if (auto *it = table->item(i, 8)) it->setCheckState(bad ? Qt::Checked : Qt::Unchecked);
        for (int c = 5; c <= 7; ++c)
            if (auto *it = table->item(i, c)) it->setForeground(bad ? QBrush(Qt::red) : QBrush());
    }
}

void BaryCorrDialog::on_pushButton_reject_clicked()
{
    std::vector<int> rows;
    QStringList names;
    for (int i = 0; i < table->rowCount(); ++i)
        if (table->item(i, 8) && table->item(i, 8)->checkState() == Qt::Checked) {
            rows.push_back(i);
            names << QString::number(md.first + i);
        }
    if (rows.empty()) {
        QMessageBox::information(this, "Error", "Reject spectra: no spectrum is marked (column Reject).");
        return;
    }
    if (int(rows.size()) >= table->rowCount() - 1) {
        QMessageBox::information(this, "Error", "Reject spectra: at least two spectra must remain.");
        return;
    }
    if (QMessageBox::question(this, "Reject spectra",
                              QString("Reject spectrum %1? They are renamed to %2bc_rejected_<index>%3, the others are "
                                      "numbered %4 to %5, and their lines are removed from the times file, the RV file, "
                                      "the per-spectrum files of the Data tab and edit.dat (copies: *.before_bc_reject).")
                              .arg(names.join(", ")).arg(md.prefix).arg(md.ext).arg(md.first)
                              .arg(md.last - int(rows.size())),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;
    QString report, err;
    if (!rejectSpectra(rows, report, err)) {
        QMessageBox::information(this, "Error", "Reject spectra: " + err);
        return;
    }
    md.last -= int(rows.size());
    setMainData(md);
    table->setRowCount(0);
    tellShift.clear();
    tellDev.clear();
    tellCorr.clear();
    bReject->setEnabled(false);
    labelResult->setText(report);
    emit spectraRejected(md.last, report);
}

namespace {
// lines with data (not empty, not starting with '#')
bool isDataLine(const QString &l)
{
    const QString t = l.trimmed();
    return !t.isEmpty() && !t.startsWith('#');
}
} // namespace

// Renames the rejected spectra, renumbers the others and removes the rejected lines from the
// files with one line per spectrum (and the blocks of edit.dat). Checks everything first, so
// that nothing is changed when a file does not fit.
bool BaryCorrDialog::rejectSpectra(const std::vector<int> &rows, QString &report, QString &err)
{
    const QDir dir(md.work);
    const int n = md.last - md.first + 1;
    std::vector<char> rej(size_t(n), 0);
    for (int r : rows) rej[size_t(r)] = 1;
    auto spec = [&](int idx) { return dir.filePath(md.prefix + QString::number(idx) + md.ext); };
    for (int i = 0; i < n; ++i)
        if (!QFileInfo::exists(spec(md.first + i))) { err = "spectrum " + spec(md.first + i) + " not found."; return false; }
    // files with one line per spectrum
    QStringList files;
    files << md.timesFile << md.rvFile << md.perSpectrum << eBc->text().trimmed() << eAm->text().trimmed();
    files.removeAll(QString());
    files.removeDuplicates();
    struct Lines { QString name; QStringList lines; };
    std::vector<Lines> adapt;
    QStringList skipped;
    for (const QString &f : files) {
        QFile q(dir.filePath(f));
        if (!q.exists()) continue;
        if (!q.open(QIODevice::ReadOnly | QIODevice::Text)) { err = "cannot read " + q.fileName() + "."; return false; }
        const QStringList lines = QString::fromUtf8(q.readAll()).split('\n');
        int data = 0;
        for (const QString &l : lines) if (isDataLine(l)) ++data;
        if (data != n) {
            if (f == md.timesFile) {
                err = QString("the times file %1 has %2 lines with data, but there are %3 spectra.").arg(f).arg(data).arg(n);
                return false;
            }
            skipped << QString("%1 (%2 lines, not %3)").arg(f).arg(data).arg(n);
            continue;
        }
        adapt.push_back({f, lines});
    }
    // edit.dat: three blocks of n numbers
    std::vector<double> edit;
    const QString editName = dir.filePath("edit.dat");
    if (QFileInfo::exists(editName)) {
        std::ifstream in(editName.toStdString());
        double x;
        while (in >> x) edit.push_back(x);
        if (int(edit.size()) != 3 * n) { skipped << QString("edit.dat (%1 values, not 3 x %2)").arg(edit.size()).arg(n); edit.clear(); }
    }
    // names of the rejected spectra (not overwriting earlier ones)
    std::vector<QString> rejName(static_cast<size_t>(n));
    for (int i = 0; i < n; ++i) {
        if (!rej[size_t(i)]) continue;
        QString base = md.prefix + "bc_rejected_" + QString::number(md.first + i);
        QString nm = base + md.ext;
        for (int k = 2; QFileInfo::exists(dir.filePath(nm)); ++k) nm = base + "_" + QString::number(k) + md.ext;
        rejName[size_t(i)] = nm;
    }
    // copies of the files that change
    auto backup = [&](const QString &f) {
        const QString b = dir.filePath(f + ".before_bc_reject");
        QFile::remove(b);
        return QFile::copy(dir.filePath(f), b);
    };
    for (const auto &a : adapt) if (!backup(a.name)) { err = "cannot copy " + a.name + "."; return false; }
    if (!edit.empty() && !backup("edit.dat")) { err = "cannot copy edit.dat."; return false; }
    // spectra: rejected ones out of the way, then the others down (targets are free)
    QStringList log;
    for (int i = 0; i < n; ++i) {
        if (!rej[size_t(i)]) continue;
        if (!QFile::rename(spec(md.first + i), dir.filePath(rejName[size_t(i)]))) {
            err = "cannot rename " + spec(md.first + i) + "."; return false;
        }
    }
    std::vector<int> newIdx(size_t(n), -1);
    int next = md.first;
    for (int i = 0; i < n; ++i) {
        if (rej[size_t(i)]) continue;
        newIdx[size_t(i)] = next;
        if (next != md.first + i && !QFile::rename(spec(md.first + i), spec(next))) {
            err = "cannot rename " + spec(md.first + i) + " to " + spec(next) + "."; return false;
        }
        ++next;
    }
    // list files without the rejected lines
    for (const auto &a : adapt) {
        QStringList out;
        int k = 0;
        for (const QString &l : a.lines) {
            if (isDataLine(l)) { if (!rej[size_t(k++)]) out << l; }
            else out << l;
        }
        QFile q(dir.filePath(a.name));
        if (!q.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) { err = "cannot write " + a.name + "."; return false; }
        q.write(out.join('\n').toUtf8());
    }
    if (!edit.empty()) {
        std::ofstream o(editName.toStdString());
        o << std::setprecision(12);
        for (int b = 0; b < 3; ++b)
            for (int i = 0; i < n; ++i) if (!rej[size_t(i)]) o << edit[size_t(b * n + i)] << "\n";
    }
    // log
    {
        std::ofstream o(dir.filePath("bc_rejected.txt").toStdString(), std::ios::app);
        o << "# " << QDateTime::currentDateTime().toString(Qt::ISODate).toStdString() << ": spectra "
          << (md.prefix + "<index>" + md.ext).toStdString() << ", " << n << " -> " << n - int(rows.size()) << "\n";
        o << "# old index\tnew index or rejected file\tdeviation of the telluric lines [km/s]\ttime\n";
        for (int i = 0; i < n; ++i) {
            o << md.first + i << "\t" << (rej[size_t(i)] ? rejName[size_t(i)].toStdString() : std::to_string(newIdx[size_t(i)])) << "\t";
            if (size_t(i) < tellDev.size()) o << tellDev[size_t(i)]; else o << "-";
            o << "\t" << (size_t(i) < times.size() ? QString::number(times[size_t(i)], 'f', 6).toStdString() : "-") << "\n";
        }
    }
    QStringList changed;
    for (const auto &a : adapt) changed << a.name;
    if (!edit.empty()) changed << "edit.dat";
    QStringList rn;
    for (int i = 0; i < n; ++i) if (rej[size_t(i)]) rn << rejName[size_t(i)];
    report = QString("Rejected: %1. The other spectra are numbered %2 to %3; adapted: %4%5. Log: bc_rejected.txt. "
                     "Read data again; start values from an earlier optimisation refer to the old numbering.")
             .arg(rn.join(", ")).arg(md.first).arg(md.last - int(rows.size()))
             .arg(changed.isEmpty() ? QString("no other file") : changed.join(", "))
             .arg(skipped.isEmpty() ? QString() : "; not adapted (number of lines does not fit): " + skipped.join(", "));
    return true;
}
