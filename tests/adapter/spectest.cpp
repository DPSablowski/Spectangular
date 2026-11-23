// Test driver for Spectangular.
//
// Calls individual steps of the program (load configuration, read data,
// build the matrix, one SVD evaluation, RV from orbital elements, short optimisation)
// and writes the results as text files. The comparison with the reference values
// happens in run_tests.py.
//
// This driver is the only place that knows the internals (slot names, global
// variables, widget names). When refactoring, only it is adapted; test cases and
// reference files stay the same.
//
// Usage: spectest <case> <work_dir> <output_dir>

#include "mainwindow.h"
#include "disentangle.h"
#include "qcustomplot.h"
#include "correlation.h"

#include <QApplication>
#include <QCheckBox>
#include <QFileInfo>
#include <QFileDialog>
#include <QDialog>
#include <QDockWidget>
#include <QScrollArea>
#include <QAction>
#include <QPushButton>
#include <QComboBox>
#include <QTabWidget>
#include <QDir>
#include <QDoubleSpinBox>
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableWidget>
#include <QAbstractButton>
#include <QSpinBox>
#include <QTimer>
#include <QElapsedTimer>

#include <armadillo>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <set>
#include <tuple>

// Global variables from mainwindow.cpp
extern int g_edgeMargin, g_lastResBins, g_lastResCols;
extern QString qDiffStat;   // file name of the residual statistics (Output tab)
extern QString (*g_pickConfigFile)(QWidget *parent, bool save, const QString &start);
extern int num, logbin, bso1, bso2, Mn, Mm, error, eval, optfratios, RVCalcCheck, runow, bidi;
extern double dv, residu, r1;
extern QVector<double> RV1, RV3, Mval1, Mval2, otimes, orbele;
extern arma::sp_mat M;
extern arma::vec X, C, W;

namespace {

QString g_out;
MainWindow *g_win = nullptr;

void fail(const std::string &msg)
{
    std::cerr << "ERROR: " << msg << std::endl;
    std::ofstream(QDir(g_out).filePath("FAILED.txt").toStdString()) << msg << "\n";
    _exit(2);
}

// Any modal box (error message, confirmation prompt) ends the test with its text.
extern bool g_closeModals;
bool g_acceptQuestions = false;   // answer confirmation prompts (Yes/No) with Yes

void startModalWatchdog()
{
    auto *t = new QTimer(qApp);
    QObject::connect(t, &QTimer::timeout, [] {
        QWidget *mw = QApplication::activeModalWidget();
        if (!mw)
            return;
        QString text = mw->windowTitle();
        if (auto *mb = qobject_cast<QMessageBox *>(mw))
            text += ": " + mb->text();
        if (g_acceptQuestions) {
            if (auto *mb = qobject_cast<QMessageBox *>(mw))
                if (QAbstractButton *yes = mb->button(QMessageBox::Yes)) {
                    std::ofstream(QDir(g_out).filePath("QUESTIONS.txt").toStdString(), std::ios::app)
                        << text.simplified().toStdString() << "\n";
                    yes->click();
                    return;
                }
        }
        if (g_closeModals) {
            std::ofstream(QDir(g_out).filePath("MODALS.txt").toStdString(), std::ios::app)
                << text.simplified().toStdString() << "\n";
            if (auto *d = qobject_cast<QDialog *>(mw))
                d->reject();
            else
                mw->close();
            return;
        }
        std::ofstream(QDir(g_out).filePath("MODAL.txt").toStdString())
            << text.toStdString() << "\n";
        std::cerr << "Modal box: " << text.toStdString() << std::endl;
        _exit(3);
    });
    t->start(20);
}

template <class T> T *widget(const char *name)
{
    T *w = g_win->findChild<T *>(name);
    if (!w)
        fail(std::string("Widget missing: ") + name);
    return w;
}

void call(const char *slot)
{
    if (!QMetaObject::invokeMethod(g_win, slot, Qt::DirectConnection))
        fail(std::string("Slot not callable: ") + slot);
}

double callDouble(const char *slot)
{
    double r = 0;
    if (!QMetaObject::invokeMethod(g_win, slot, Qt::DirectConnection, Q_RETURN_ARG(double, r)))
        fail(std::string("Slot not callable: ") + slot);
    return r;
}

std::ofstream outFile(const QString &name)
{
    std::ofstream f(QDir(g_out).filePath(name).toStdString());
    f << std::setprecision(17);
    return f;
}

template <class V> void dumpVec(const QString &name, const V &v, int n)
{
    auto f = outFile(name);
    for (int i = 0; i < n; ++i)
        f << v[i] << "\n";
}

void dumpScalars(const QString &name, const std::map<std::string, double> &s)
{
    auto f = outFile(name);
    for (const auto &kv : s)
        f << kv.first << " " << kv.second << "\n";
}

void dumpMatrix(const QString &name)
{
    auto f = outFile(name);
    f << "# " << M.n_rows << " " << M.n_cols << "\n";
    // sorted by row as before (sp_mat iterates by column)
    std::vector<std::tuple<arma::uword, arma::uword, double>> e;
    for (auto it = M.begin(); it != M.end(); ++it)
        if (*it != 0.0)
            e.emplace_back(it.row(), it.col(), *it);
    std::sort(e.begin(), e.end());
    for (const auto &t : e)
        f << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << "\n";
}

std::map<std::string, double> dataScalars()
{
    return {{"num", double(num)}, {"logbin", double(logbin)}, {"bso1", double(bso1)},
            {"bso2", double(bso2)}, {"Mn", double(Mn)}, {"Mm", double(Mm)},
            {"dv", dv}, {"M_rows", double(M.n_rows)}, {"M_cols", double(M.n_cols)}};
}

void dumpConfigWidgets(const QString &name)
{
    std::map<QString, QString> v;
    for (auto *w : g_win->findChildren<QSpinBox *>())
        v[w->objectName()] = QString::number(w->value());
    for (auto *w : g_win->findChildren<QDoubleSpinBox *>())
        v[w->objectName()] = QString::number(w->value(), 'g', 17);
    for (auto *w : g_win->findChildren<QLineEdit *>())
        if (!w->objectName().startsWith("qt_"))
            v[w->objectName()] = w->text();
    for (auto *w : g_win->findChildren<QCheckBox *>())
        v[w->objectName()] = w->isChecked() ? "1" : "0";
    for (auto *w : g_win->findChildren<QComboBox *>())
        v[w->objectName()] = QString::number(w->currentIndex());
    auto f = outFile(name);
    for (const auto &kv : v)
        f << kv.first.toStdString() << " = " << kv.second.toStdString() << "\n";
}

void copyOutput(const QString &fileName)
{
    QString src = QDir::current().filePath(fileName);
    QString dst = QDir(g_out).filePath(fileName);
    QFile::remove(dst);
    if (!QFile::copy(src, dst))
        fail("Output file missing: " + fileName.toStdString());
}

void loadConfig(const char *name = "SB2_conf")
{
    // Configuration from the work directory; its directory becomes the Work Path
    QString err = g_win->loadConfiguration(QDir::current().absoluteFilePath(QString(name) + ".sps"));
    if (!err.isEmpty())
        fail("Configuration: " + err.toStdString());
    if (qgetenv("SPECTEST_SOLVER") == "svd") {
        widget<QCheckBox>("checkBox_6")->setChecked(true);
        widget<QCheckBox>("checkBox_5")->setChecked(false);
    }
    if (qgetenv("SPECTEST_EDGES") == "0")
        widget<QCheckBox>("checkBox_40")->setChecked(false);   // residual including edge pixels
}

void readData()
{
    error = 0;
    call("read_data");
    if (error != 0)
        fail("read_data reports error=" + std::to_string(error));
}

double evaluate()
{
    call("ConstructMatrix");
    return callDouble("DivideConquer");
}

// ---------------------------------------------------------------- Test cases

void caseConfig()
{
    loadConfig();
    dumpConfigWidgets("config.txt");
}

void caseConfigRoundtrip()
{
    loadConfig();
    // Values the old format lost: decimal places, widgets that were not saved,
    // spaces
    widget<QDoubleSpinBox>("doubleSpinBox_14")->setValue(6562.8123);
    widget<QDoubleSpinBox>("doubleSpinBox_15")->setValue(6599.4567);
    widget<QCheckBox>("checkBox_31")->setChecked(!widget<QCheckBox>("checkBox_31")->isChecked());
    widget<QLineEdit>("lineEdit_25")->setText("name mit leerzeichen");
    widget<QLineEdit>("lineEdit_13")->setText("");
    dumpConfigWidgets("config_loaded.txt");
    if (!g_win->saveConfiguration(QDir::current().absoluteFilePath("roundtrip_conf.sps")))
        fail("roundtrip_conf.sps not written");
    loadConfig();   // other values in between, so that nothing sticks
    loadConfig("roundtrip_conf");
    dumpConfigWidgets("config_reloaded.txt");

    dumpScalars("state.txt", {{"bidi", double(bidi)}});
    copyOutput("roundtrip_conf.sps");
}

void caseReadData()
{
    loadConfig();
    readData();
    dumpScalars("scalars.txt", dataScalars());
    dumpVec("C.txt", C, C.n_elem);
    dumpVec("W.txt", W, W.n_elem);
    dumpVec("RV1.txt", RV1, num);
    dumpVec("RV3.txt", RV3, num);
    dumpMatrix("M.txt");
}

void caseConstructMatrix()
{
    loadConfig();
    readData();
    for (int i = 0; i < num; ++i) {   // non-integer shift
        RV1[i] += 3.3;
        RV3[i] -= 2.1;
    }
    call("ConstructMatrix");
    dumpScalars("scalars.txt", dataScalars());
    dumpMatrix("M.txt");
    dumpVec("W.txt", W, W.n_elem);   // wavelengths of the rows (flux-ratio checks)
}

// Solve, then the continuum correction of the Spectrum Plotter with the flux-ratio model:
// continuum points of A at the wavelengths in cont_lambda.txt (mean of the output A within
// +-2 pixels), "Flux-ratio model", Apply, Save to A_norm.txt / B_norm.txt.
void casePlotterRatio()
{
    loadConfig();
    readData();
    r1 = 0;
    evaluate();
    const QString outA = widget<QLineEdit>("lineEdit_5")->text();
    const QString outB = widget<QLineEdit>("lineEdit_8")->text();
    copyOutput(outA);
    copyOutput(outB);
    arma::mat a;
    a.load(QDir::current().filePath(outA).toStdString(), arma::raw_ascii);
    arma::vec cl;
    cl.load("cont_lambda.txt", arma::raw_ascii);
    {
        std::ofstream sp("spline.dat");
        sp << std::setprecision(15);
        for (double l : cl) {
            arma::uword k = arma::index_min(arma::abs(a.col(0) - l));
            arma::uword lo = k >= 2 ? k - 2 : 0, hi = std::min<arma::uword>(a.n_rows - 1, k + 2);
            sp << l << " " << arma::mean(a.col(1).subvec(lo, hi)) << "\n";
        }
    }
    QFile::remove("A_norm.txt");     // no overwrite question
    QFile::remove("B_norm.txt");
    call("on_actionSpectrum_Plotter_triggered");
    QDialog *dlg = nullptr;
    for (QDialog *d : g_win->findChildren<QDialog *>())
        if (d->objectName() == "PlotSpec") dlg = d;
    if (!dlg) fail("Spectrum Plotter not opened");
    auto set = [&](const char *n, bool v) {
        auto *c = dlg->findChild<QCheckBox *>(n);
        if (!c) fail(std::string("Plotter widget missing: ") + n);
        c->setChecked(v);
    };
    dlg->findChild<QLineEdit *>("lineEdit")->setText(outA);
    dlg->findChild<QLineEdit *>("lineEdit_7")->setText(outB);
    dlg->findChild<QLineEdit *>("lineEdit_8")->setText("A_norm.txt");
    dlg->findChild<QLineEdit *>("lineEdit_9")->setText("B_norm.txt");
    set("checkBox_13", false);
    set("checkBox_17", true);
    set("checkBox_rmodel", true);
    if (!QMetaObject::invokeMethod(dlg, "on_pushButton_6_clicked", Qt::DirectConnection)
        || !QMetaObject::invokeMethod(dlg, "on_pushButton_9_clicked", Qt::DirectConnection))
        fail("Plotter slots not callable");
    copyOutput("A_norm.txt");
    copyOutput("B_norm.txt");
}

// Solve, then the residual statistics per spectrum (diffstatistic.dat or the name in the Output tab).
void caseResidualStats()
{
    loadConfig();
    readData();
    r1 = 0;
    evaluate();
    call("on_pushButton_stat_clicked");
    copyOutput(qDiffStat);
    // plot of the differences: one graph per spectrum, each with all its pixels
    call("on_pushButton_6_clicked");
    QCustomPlot *p = widget<QCustomPlot>("customPlot_3");
    std::ofstream o(QDir(g_out).filePath("plot_differences.txt").toStdString());
    for (int i = 0; i < p->graphCount(); ++i) o << i << "\t" << p->graph(i)->data()->size() << "\n";
}

// Orbit fit dialog (Tools -> Orbit fit) on the RVs of the main window: Fit, Write.
void caseOrbitFitRvs()
{
    loadConfig();
    readData();
    call("on_actionOrbitFit_triggered");
    QDialog *dlg = nullptr;
    for (QDialog *d : g_win->findChildren<QDialog *>())
        if (d->objectName() == "OrbitFit") dlg = d;
    if (!dlg) fail("Orbit fit dialog not opened");
    // gamma fixed as in the main window's orbit mode (lineEdit_21: start gamma)
    dlg->findChild<QCheckBox *>("checkBox_7")->setChecked(true);
    if (!QMetaObject::invokeMethod(dlg, "on_pushButton_2_clicked", Qt::DirectConnection)
        || !QMetaObject::invokeMethod(dlg, "on_pushButton_write_clicked", Qt::DirectConnection))
        fail("Orbit fit slots not callable");
    copyOutput("orbitfit_result.txt");
    copyOutput("orbitelements.dat");
}

// Solve, then Tools -> Flux ratio from synthetic spectra with the true component spectra
// (truth_A.txt, truth_B.txt) as synthetic spectra; fit, write, use the ratio.
void caseFluxTemplates()
{
    loadConfig();
    readData();
    r1 = 0;
    evaluate();
    call("on_actionFluxTemplate_triggered");
    QDialog *dlg = nullptr;
    for (QDialog *d : g_win->findChildren<QDialog *>())
        if (d->objectName() == "FluxTemplate") dlg = d;
    if (!dlg) fail("Dialog 'Flux ratio from synthetic spectra' not opened");
    dlg->findChild<QLineEdit *>("lineEdit_synA")->setText("truth_A.txt");
    dlg->findChild<QLineEdit *>("lineEdit_synB")->setText("truth_B.txt");
    // 2000 A: the continua of the components are wavy (slowly varying functions are nearly
    // undetermined), degree 4 for the continuum
    dlg->findChild<QSpinBox *>("spinBox_deg")->setValue(4);
    if (!QMetaObject::invokeMethod(dlg, "on_pushButton_fit_clicked", Qt::DirectConnection)
        || !QMetaObject::invokeMethod(dlg, "on_pushButton_write_clicked", Qt::DirectConnection)
        || !QMetaObject::invokeMethod(dlg, "on_pushButton_use_clicked", Qt::DirectConnection))
        fail("Slots of the dialog not callable");
    const QString a = widget<QLineEdit>("lineEdit_5")->text(), b = widget<QLineEdit>("lineEdit_8")->text();
    copyOutput("fluxratio_templates.txt");
    copyOutput(QFileInfo(a).completeBaseName() + "_renorm." + QFileInfo(a).suffix());
    copyOutput(QFileInfo(b).completeBaseName() + "_renorm." + QFileInfo(b).suffix());
    dumpScalars("result.txt", {{"field_ratio", widget<QDoubleSpinBox>("doubleSpinBox_4")->value()}});
}

// Tools -> Barycentric correction: iota Tri from STELLA, times as JD (UTC); compute, write the
// corrected spectra and start RVs, use them in the main window.
void caseBaryCorr()
{
    loadConfig();
    call("on_actionBaryCorr_triggered");
    QDialog *dlg = nullptr;
    for (QDialog *d : g_win->findChildren<QDialog *>())
        if (d->objectName() == "BaryCorr") dlg = d;
    if (!dlg) fail("Dialog 'Barycentric correction' not opened");
    dlg->findChild<QLineEdit *>("lineEdit_ra")->setText("02 12 22.28");
    dlg->findChild<QLineEdit *>("lineEdit_dec")->setText("+30 18 11.1");
    dlg->findChild<QComboBox *>("comboBox_site")->setCurrentIndex(0);
    dlg->findChild<QComboBox *>("comboBox_scale")->setCurrentIndex(0);
    dlg->findChild<QCheckBox *>("checkBox_rv")->setChecked(true);
    if (!QMetaObject::invokeMethod(dlg, "on_pushButton_compute_clicked", Qt::DirectConnection)
        || !QMetaObject::invokeMethod(dlg, "on_pushButton_write_clicked", Qt::DirectConnection)
        || !QMetaObject::invokeMethod(dlg, "on_pushButton_use_clicked", Qt::DirectConnection))
        fail("Slots of the dialog not callable");
    const QString pre = widget<QLineEdit>("lineEdit_3")->text();
    copyOutput(pre + "barycorr.txt");
    for (int g = widget<QSpinBox>("spinBox")->value(); g <= widget<QSpinBox>("spinBox_2")->value(); ++g)
        copyOutput(pre + QString::number(g) + ".txt");
    copyOutput(widget<QLineEdit>("lineEdit_2")->text());
    std::ofstream o(QDir(g_out).filePath("main.txt").toStdString());
    o << "prefix\t" << pre.toStdString() << "\nrvfile\t" << widget<QLineEdit>("lineEdit_2")->text().toStdString()
      << "\nformat\t" << widget<QComboBox>("comboBox")->currentIndex() << "\n";
}

// Tools -> Barycentric correction, only the BC file (spectra unchanged): written, set in the
// Data tab with the frame. SPECTEST_BC_CORRECTED=1: "Spectra already corrected" ticked.
void caseBaryCorrFile()
{
    loadConfig();
    call("on_actionBaryCorr_triggered");
    QDialog *dlg = nullptr;
    for (QDialog *d : g_win->findChildren<QDialog *>())
        if (d->objectName() == "BaryCorr") dlg = d;
    if (!dlg) fail("Dialog 'Barycentric correction' not opened");
    dlg->findChild<QLineEdit *>("lineEdit_ra")->setText("02 12 22.28");
    dlg->findChild<QLineEdit *>("lineEdit_dec")->setText("+30 18 11.1");
    dlg->findChild<QComboBox *>("comboBox_site")->setCurrentIndex(0);
    dlg->findChild<QComboBox *>("comboBox_scale")->setCurrentIndex(0);
    dlg->findChild<QCheckBox *>("checkBox_corrected")->setChecked(qEnvironmentVariableIsSet("SPECTEST_BC_CORRECTED"));
    const QString f = dlg->findChild<QLineEdit *>("lineEdit_bcfile")->text();
    const QString fa = dlg->findChild<QLineEdit *>("lineEdit_airmass")->text();
    if (!QMetaObject::invokeMethod(dlg, "on_pushButton_writebc_clicked", Qt::DirectConnection))
        fail("Slot of the dialog not callable");
    copyOutput(f);
    copyOutput(fa);
    std::ofstream o(QDir(g_out).filePath("main.txt").toStdString());
    o << "bcfile\t" << widget<QLineEdit>("lineEdit_bc")->text().toStdString() << "\nframe\t"
      << widget<QComboBox>("comboBox_bcframe")->currentIndex() << "\nprefix\t"
      << widget<QLineEdit>("lineEdit_3")->text().toStdString() << "\ntellw\t"
      << widget<QLineEdit>("lineEdit_tellw")->text().toStdString() << "\n";
}

// Tools -> Barycentric correction -> Check telluric lines (and, with SPECTEST_REJECT, reject the
// marked spectra; the confirmation is answered with Yes). Env: SPECTEST_RADEC "ra;dec",
// SPECTEST_BC_SCALE (index of the time scale, default 0 = JD), SPECTEST_BC_CORRECTED.
void caseTellCheck()
{
    loadConfig();
    call("on_actionBaryCorr_triggered");
    QDialog *dlg = nullptr;
    for (QDialog *d : g_win->findChildren<QDialog *>())
        if (d->objectName() == "BaryCorr") dlg = d;
    if (!dlg) fail("Dialog 'Barycentric correction' not opened");
    const QStringList rd = qEnvironmentVariable("SPECTEST_RADEC", "02 12 22.28;+30 18 11.1").split(';');
    dlg->findChild<QLineEdit *>("lineEdit_ra")->setText(rd.value(0));
    dlg->findChild<QLineEdit *>("lineEdit_dec")->setText(rd.value(1));
    dlg->findChild<QComboBox *>("comboBox_site")->setCurrentIndex(0);
    dlg->findChild<QComboBox *>("comboBox_scale")->setCurrentIndex(qEnvironmentVariableIntValue("SPECTEST_BC_SCALE"));
    dlg->findChild<QCheckBox *>("checkBox_corrected")->setChecked(qEnvironmentVariableIsSet("SPECTEST_BC_CORRECTED"));
    if (!QMetaObject::invokeMethod(dlg, "on_pushButton_tellcheck_clicked", Qt::DirectConnection))
        fail("Slot of the dialog not callable");
    auto *t = dlg->findChild<QTableWidget *>("tableWidget_bc");
    std::ofstream o(QDir(g_out).filePath("tellcheck.txt").toStdString());
    o << "# spectrum\tBC\tshift\tdeviation\treject\n";
    for (int i = 0; i < t->rowCount(); ++i) {
        if (!t->item(i, 5)) fail("No result of the check");
        o << t->item(i, 0)->text().toStdString() << "\t" << t->item(i, 3)->text().toStdString() << "\t"
          << t->item(i, 5)->text().toStdString() << "\t" << t->item(i, 6)->text().toStdString()
          << "\t" << (t->item(i, 8)->checkState() == Qt::Checked) << "\n";
    }
    o.close();
    if (qEnvironmentVariableIsSet("SPECTEST_REJECT")) {
        g_acceptQuestions = true;
        if (!QMetaObject::invokeMethod(dlg, "on_pushButton_reject_clicked", Qt::DirectConnection))
            fail("Slot of the dialog not callable");
        std::ofstream m(QDir(g_out).filePath("main.txt").toStdString());
        m << "last\t" << widget<QSpinBox>("spinBox_2")->value() << "\n";
    }
}

// Orbit optimisation, error estimation with the curvature, then with Monte Carlo.
void caseErrorMc()
{
    loadConfig();
    r1 = 0;
    call("on_pushButton_4_clicked");
    widget<QComboBox>("comboBox_err")->setCurrentIndex(0);
    call("on_pushButton_8_clicked");
    const QString f = widget<QLineEdit>("lineEdit_24")->text();
    QFile::remove(QDir(g_out).filePath("errors_curvature.dat"));
    QFile::copy(f, QDir(g_out).filePath("errors_curvature.dat"));
    widget<QComboBox>("comboBox_err")->setCurrentIndex(1);
    call("on_pushButton_8_clicked");
    copyOutput(f);
}

void caseDivideConquer()
{
    loadConfig();
    readData();
    r1 = 0;
    double r = evaluate();
    dumpScalars("result.txt", {{"residuum", r}, {"residu_global", residu}});
    dumpVec("X.txt", X, X.n_elem);
    auto *le = widget<QLineEdit>("lineEdit_5");
    copyOutput(le->text());
    copyOutput(widget<QLineEdit>("lineEdit_8")->text());
    copyOutput(widget<QLineEdit>("lineEdit_13")->text());
    copyOutput(widget<QLineEdit>("lineEdit_20")->text());
    if (widget<QCheckBox>("checkBox_8")->isChecked())
        copyOutput(widget<QLineEdit>("lineEdit_10")->text());   // static component
}

// The same evaluation twice: the result must be identical.
void caseDivideConquerRepeat()
{
    loadConfig();
    readData();
    double ra = evaluate();
    arma::vec xa = X;
    double rb = evaluate();
    arma::vec xb = X;
    double dx = arma::abs(xa - xb).max();
    dumpScalars("result.txt", {{"residuum_1", ra}, {"residuum_2", rb}, {"max_abs_dX", dx}});
}

// Compares the program's SVD solution with the minimum-norm solution pinv(M)*C.
// sign = -1 negates the RVs beforehand (for the synthetic data set).
void svdCheck(double sign)
{
    loadConfig();
    readData();
    for (int i = 0; i < num; ++i) {
        RV1[i] *= sign;
        RV3[i] *= sign;
    }
    r1 = 0;
    double r = evaluate();
    const arma::mat Md(M);
    arma::vec s = arma::svd(Md);
    double tsh = 0.5 * std::sqrt(double(Mm + Mn + 1)) * s(0) * arma::datum::eps;
    arma::uword nSmall = arma::accu(s <= tsh);
    arma::vec xp = arma::pinv(Md, tsh) * C;   // same threshold as the SVD
    arma::vec d = arma::abs(X - xp);
    arma::uvec bad = arma::find(d > 1e-8);
    {
        auto f = outFile("differing_unknowns.txt");
        f << "# index of the unknown, X_program, X_pinv   (bso1=" << bso1
          << ", bso2=" << bso2 << ", Mm=" << Mm << ")\n";
        for (arma::uword k : bad)
            f << k << " " << X(k) << " " << xp(k) << "\n";
    }
    dumpScalars("result.txt", {{"residuum", r},
                               {"n_singular_below_tsh", double(nSmall)},
                               {"n_unknowns_differing", double(bad.n_elem)},
                               {"max_abs_dX", d.max()},
                               {"norm_resid_program", arma::norm(M * X - C)},
                               {"norm_resid_pinv", arma::norm(M * xp - C)},
                               {"n_zero_columns", double(arma::accu(arma::sum(arma::abs(Md), 0) == 0))}});
}

void caseSvdCheck() { svdCheck(1); }
void caseSvdCheckNegated() { svdCheck(-1); }

// RV from orbital elements. Input: orbits.txt (7 elements per line) and rv_times.txt.
void caseOrbitRv()
{
    std::vector<double> t;
    {
        std::ifstream in("rv_times.txt");
        double x;
        while (in >> x)
            t.push_back(x);
    }
    if (t.empty())
        fail("rv_times.txt empty");
    num = int(t.size());
    otimes.resize(num);
    RV1.resize(num);
    RV3.resize(num);
    for (int i = 0; i < num; ++i)
        otimes[i] = t[i];

    std::ifstream in("orbits.txt");
    std::string lineStr;
    int k = 0;
    while (std::getline(in, lineStr)) {
        std::istringstream ls(lineStr);
        for (int i = 0; i < 7; ++i)
            ls >> orbele[i];
        if (!ls)
            continue;
        RVCalcCheck = 0;
        call("VAmplitudeA");
        call("VAmplitudeB");
        auto f = outFile(QString("rv_%1.txt").arg(k));
        f << "# RVCalcCheck " << RVCalcCheck << "\n";
        for (int i = 0; i < num; ++i)
            f << otimes[i] << " " << RV1[i] << " " << RV3[i] << "\n";
        ++k;
    }
}

// Invalid orbital elements must yield a penalty residual (no NaN).
void caseOrbitInvalid()
{
    loadConfig();
    readData();
    const double base[7] = {20.54, 0.5, 68.6, 67.6, -5.6, 2436997, 1.85};
    struct V { const char *name; int idx; double val; };
    std::map<std::string, double> res;
    for (V v : {V{"e_1.2", 1, 1.2}, V{"e_1.0", 1, 1.0}, V{"e_-0.1", 1, -0.1}, V{"P_0", 0, 0.0},
                V{"P_-5", 0, -5.0}}) {
        for (int i = 0; i < 7; ++i)
            orbele[i] = base[i];
        orbele[v.idx] = v.val;
        res[v.name] = callDouble("evaluateOrbit");
    }
    dumpScalars("result.txt", res);
}

void caseOptimisation()
{
    loadConfig();
    r1 = 0;
    call("on_pushButton_4_clicked");
    // Levenberg-Marquardt writes no initial simplex (lineEdit_14/15)
    const bool lm = widget<QComboBox>("comboBox_method")->currentIndex() == 1;
    for (const char *n : {"lineEdit_14", "lineEdit_15", "lineEdit_16", "lineEdit_17"})
        if (!lm || (std::string(n) != "lineEdit_14" && std::string(n) != "lineEdit_15"))
            copyOutput(widget<QLineEdit>(n)->text());
    copyOutput(widget<QLineEdit>("lineEdit_5")->text());
    copyOutput(widget<QLineEdit>("lineEdit_8")->text());
    dumpScalars("result.txt", {{"best_residuum", r1}, {"evaluations", double(eval)}});
    if (auto *plot = g_win->findChild<QCustomPlot *>("customPlot_4"))
        plot->savePng(QDir(g_out).filePath("objective.png"), 700, 350);   // plot of residual vs. iteration
}

// Optimisation on long spectra, "Abort" pressed after SPECTEST_ABORT_MS (default 1000 ms, during the first solve)
// via the event loop: the window stays responsive during the solves (largest gap between
// two ticks of a 50 ms timer) and the abort ends the running solve.
void runWithAbort(const char *slot, int defaultMs);
void caseOptimisationAbort() { runWithAbort("on_pushButton_4_clicked", 1000); }
// Solve on long spectra, "Stop" after SPECTEST_ABORT_MS (default 800 ms, during the LSQR solve
// after reading the data and building the matrix): the solve ends at once, no error box
void caseSolveAbort() { runWithAbort("on_pushButton_3_clicked", 800); }

void runWithAbort(const char *slot, int defaultMs)
{
    loadConfig();
    r1 = 0;
    const char *e = std::getenv("SPECTEST_ABORT_MS");
    const int abortMs = e ? std::atoi(e) : defaultMs;
    QElapsedTimer clock;
    qint64 last = 0, maxGap = 0, abortAt = -1;
    int ticks = 0;
    QTimer tick;
    QObject::connect(&tick, &QTimer::timeout, [&]{
        const qint64 now = clock.elapsed();
        if (ticks > 0) maxGap = std::max(maxGap, now - last);
        last = now;
        ++ticks;
    });
    QTimer::singleShot(abortMs, [&]{ abortAt = clock.elapsed(); call("on_pushButton_7_clicked"); });
    clock.start();
    tick.start(50);
    g_closeModals = true;                      // an error box would be recorded in MODALS.txt
    call(slot);
    g_closeModals = false;
    const qint64 end = clock.elapsed();
    tick.stop();
    dumpScalars("result.txt", {{"abort_ms", double(abortAt)}, {"end_ms", double(end)},
                               {"max_gap_ms", double(maxGap)}, {"ticks", double(ticks)},
                               {"evaluations", double(eval)}});
}

// SVD solution, then subtract component A (button "Subtract").
void subtractCase(bool staticLines)
{
    loadConfig();
    widget<QCheckBox>("checkBox_8")->setChecked(staticLines);
    call("on_pushButton_3_clicked");
    widget<QCheckBox>("checkBox_20")->setChecked(true);
    widget<QCheckBox>("checkBox_21")->setChecked(false);
    widget<QCheckBox>("checkBox_24")->setChecked(false);
    widget<QCheckBox>("checkBox_25")->setChecked(false);
    widget<QCheckBox>("checkBox_26")->setChecked(false);
    widget<QCheckBox>("checkBox_28")->setChecked(false);
    widget<QLineEdit>("lineEdit_27")->setText("residB");
    widget<QLineEdit>("lineEdit_19")->setText(".txt");
    call("on_pushButton_12_clicked");
    copyOutput("residB_0.txt");
    copyOutput("residB_3.txt");
}
void caseSubtract() { subtractCase(false); }
void caseSubtractStatic() { subtractCase(true); }

// Main window as image (layout check; not part of the suite)
void caseScreenshot()
{
    loadConfig();
    g_win->resize(1440, 920);
    g_win->show();
    qApp->processEvents();
    g_win->grab().save(QDir(g_out).filePath("main.png"));
    if (qEnvironmentVariableIsSet("SPECTEST_SHOT_SIZES"))   // layout check: minimum widths
        for (QWidget *w : g_win->findChildren<QWidget *>())
            if (w->isVisible() && w->minimumSizeHint().width() > 300)
                std::cout << "MINW " << w->metaObject()->className() << " " << w->objectName().toStdString()
                          << " " << w->minimumSizeHint().width() << " " << w->width() << std::endl;
    // after Solve and Plot, and each tab of both tab widgets
    call("on_pushButton_3_clicked");
    if (qEnvironmentVariableIsSet("SPECTEST_SHOT_RUN")) {
        // optional: optimisation and error estimation, so that the Convergence and
        // Messages tabs have content (images for the manual)
        r1 = 0;
        call("on_pushButton_4_clicked");
        call("on_pushButton_8_clicked");
    }
    call("on_pushButton_6_clicked");
    if (QFile::exists("optdata.dat")) call("plot_functionvalues");   // convergence of an earlier run
    if (auto *plots = g_win->findChild<QTabWidget *>("tabPlots"))
        plots->setCurrentIndex(0);   // the spectra
    qApp->processEvents();
    g_win->grab().save(QDir(g_out).filePath("main_solved.png"));
    auto *left = g_win->findChild<QTabWidget *>("tabSettings");
    auto *right = g_win->findChild<QTabWidget *>("tabPlots");
    for (int i = 0; left && i < left->count(); ++i) {
        left->setCurrentIndex(i);
        qApp->processEvents();
        left->grab().save(QDir(g_out).filePath(QString("tab_left_%1.png").arg(i)));
        // the whole content of the tab (without scrolling), for the manual
        if (auto *sa = qobject_cast<QScrollArea *>(left->widget(i)->findChild<QScrollArea *>()
                                                   ? left->widget(i)->findChild<QScrollArea *>()
                                                   : qobject_cast<QScrollArea *>(left->widget(i)))) {
            if (QWidget *w = sa->widget()) {
                w->resize(sa->viewport()->width(), w->sizeHint().height());
                qApp->processEvents();
                w->grab().save(QDir(g_out).filePath(QString("tab_full_%1.png").arg(i)));
                sa->setWidgetResizable(sa->widgetResizable());
            }
        }
    }
    for (int i = 0; right && i < right->count(); ++i) {
        right->setCurrentIndex(i);
        qApp->processEvents();
        right->grab().save(QDir(g_out).filePath(QString("tab_right_%1.png").arg(i)));
    }
    // auxiliary windows of the Tools menu (child dialogs of the main window)
    struct Shot { const char *slot, *file; int w, h; };
    const std::vector<Shot> dialogs = {
        {"on_actionEditor_triggered", "dlg_editor.png", 1500, 900},
        {"on_actionSpectrum_Plotter_triggered", "dlg_plotter.png", 1500, 820},
        {"on_actionCorrelation_triggered", "dlg_correlation.png", 760, 380},
        {"on_actionArithmetic_triggered", "dlg_arithmetic.png", 760, 260},
        {"on_actionOrbitFit_triggered", "dlg_orbitfit.png", 1150, 820},
        {"on_actionFluxTemplate_triggered", "dlg_fluxtemplate.png", 1000, 900},
        {"on_actionBaryCorr_triggered", "dlg_barycorr.png", 1000, 900},
    };
    for (const auto &d : dialogs) {
        const QList<QDialog *> before = g_win->findChildren<QDialog *>();
        call(d.slot);
        qApp->processEvents();
        for (QDialog *dlg : g_win->findChildren<QDialog *>()) {
            if (before.contains(dlg) || !dlg->isVisible())
                continue;
            if (d.w > 0)   // large enough that the scroll area shows everything
                dlg->resize(d.w, d.h);
            if (dlg->objectName() == "FluxTemplate" && QFile::exists("truth_A.txt")) {   // test data with truth
                dlg->findChild<QLineEdit *>("lineEdit_synA")->setText("truth_A.txt");
                dlg->findChild<QLineEdit *>("lineEdit_synB")->setText("truth_B.txt");
                dlg->findChild<QSpinBox *>("spinBox_deg")->setValue(4);
                QMetaObject::invokeMethod(dlg, "on_pushButton_fit_clicked", Qt::DirectConnection);
            }
            if (dlg->objectName() == "BaryCorr" && qEnvironmentVariableIsSet("SPECTEST_SHOT_RADEC")) {
                const QStringList rd = qEnvironmentVariable("SPECTEST_SHOT_RADEC").split(';');
                dlg->findChild<QLineEdit *>("lineEdit_ra")->setText(rd.value(0));
                dlg->findChild<QLineEdit *>("lineEdit_dec")->setText(rd.value(1));
                dlg->findChild<QComboBox *>("comboBox_site")->setCurrentIndex(0);
                if (qEnvironmentVariableIsSet("SPECTEST_SHOT_TELLCHECK")) {   // corrected spectra, check
                    dlg->findChild<QCheckBox *>("checkBox_corrected")->setChecked(true);
                    QMetaObject::invokeMethod(dlg, "on_pushButton_tellcheck_clicked", Qt::DirectConnection);
                } else
                    QMetaObject::invokeMethod(dlg, "on_pushButton_compute_clicked", Qt::DirectConnection);
            }
            if (dlg->objectName() == "OrbitFit") {   // with a fit to the RVs of the main window
                if (auto *g = dlg->findChild<QCheckBox *>("checkBox_7")) g->setChecked(true);
                QMetaObject::invokeMethod(dlg, "on_pushButton_2_clicked", Qt::DirectConnection);
            }
            qApp->processEvents();
            dlg->grab().save(QDir(g_out).filePath(d.file));
            dlg->close();
        }
    }
    if (auto *dock = g_win->findChild<QDockWidget *>("dockSubtract")) {
        dock->setFloating(true);
        dock->show();
        qApp->processEvents();
        dock->grab().save(QDir(g_out).filePath("dock_subtract.png"));
    }
}

// Editor: opened, it shows the elements of the orbital elements file of the Data tab
void caseEditorOrbit()
{
    g_closeModals = true;    // no BinaryData.dat in the test data: message, list empty
    loadConfig();
    const QList<QDialog *> before = g_win->findChildren<QDialog *>();
    call("on_actionEditor_triggered");
    for (QDialog *dlg : g_win->findChildren<QDialog *>()) {
        if (before.contains(dlg) || dlg->objectName() != "Edit")
            continue;
        auto v = [&](const char *n) { return dlg->findChild<QDoubleSpinBox *>(n)->value(); };
        dumpScalars("result.txt", {{"P", v("doubleSpinBox_6")}, {"e", v("doubleSpinBox_7")},
                                   {"K_A", v("doubleSpinBox_8")}, {"K_B", v("doubleSpinBox_9")},
                                   {"gamma", v("doubleSpinBox_10")}, {"T_peri", v("doubleSpinBox_11")},
                                   {"omega_deg", v("doubleSpinBox_12")}});
        dlg->close();
        return;
    }
    fail("Editor not opened");
}

// Cross-correlation (dialog "Correlation"): spec.txt against tmpl.txt in the work directory.
void caseCorrelation()
{
    correlation dlg;
    QMetaObject::invokeMethod(&dlg, "seData", Qt::DirectConnection,
                              Q_ARG(QString, QDir::currentPath()), Q_ARG(QString, QString("spec.txt")));
    auto *t = dlg.findChild<QLineEdit *>("lineEdit_2");
    auto *sh = dlg.findChild<QSpinBox *>("spinBox");
    auto *c1 = dlg.findChild<QDoubleSpinBox *>("doubleSpinBox_2");
    auto *c2 = dlg.findChild<QDoubleSpinBox *>("doubleSpinBox_3");
    auto *res = dlg.findChild<QDoubleSpinBox *>("doubleSpinBox");
    if (!t || !sh || !c1 || !c2 || !res)
        fail("Widgets of the correlation dialog missing");
    t->setText("tmpl.txt");
    sh->setValue(10);
    c1->setValue(1.0);
    c2->setValue(1.0);
    if (!QMetaObject::invokeMethod(&dlg, "on_pushButton_2_clicked", Qt::DirectConnection))
        fail("Slot not callable");
    dumpScalars("result.txt", {{"shift_px", res->value()}});
}

// Orbit optimisation, then error estimation (button "Error").
void caseErrorEstimation()
{
    loadConfig();
    r1 = 0;
    call("on_pushButton_4_clicked");
    call("on_pushButton_8_clicked");
    copyOutput(widget<QLineEdit>("lineEdit_24")->text());
    copyOutput(widget<QLineEdit>("lineEdit_16")->text());
    copyOutput(widget<QLineEdit>("lineEdit_17")->text());
}

// Click test of the GUI: all buttons and menu items of the main window, then
// all buttons of the opened windows. Modal dialogs are logged and closed with
// "Cancel/No" instead of ending the run. Every action is written to the log before
// it is executed, so that a crash can be attributed.
bool g_closeModals = false;

void caseGuiSmoke()
{
    g_closeModals = true;
    loadConfig();
    std::ofstream log(QDir(g_out).filePath("clicked.txt").toStdString());
    auto note = [&](const QString &what) {
        log << what.toStdString() << std::endl;
        log.flush();
    };
    auto clickAll = [&](QWidget *root, const QString &prefix, const QStringList &skip) {
        QList<QPushButton *> buttons = root->findChildren<QPushButton *>();
        std::sort(buttons.begin(), buttons.end(),
                  [](QPushButton *a, QPushButton *b) { return a->objectName() < b->objectName(); });
        for (QPushButton *b : buttons) {
            if (b->objectName().isEmpty() || skip.contains(b->objectName()))
                continue;
            if (!b->isEnabled()) {
                note(prefix + b->objectName() + " (disabled)");
                continue;
            }
            note(prefix + b->objectName() + " \"" + b->text() + "\"");
            b->click();
            QApplication::processEvents();
        }
    };
    // main window; Abort last
    clickAll(g_win, "main/", {"pushButton_15"});
    for (QAction *a : g_win->findChildren<QAction *>()) {
        if (a->objectName().isEmpty() || a->objectName() == "actionManual" || a->objectName() == "actionQuit"
            || a->menu())
            continue;
        note("action/" + a->objectName());
        a->trigger();
        QApplication::processEvents();
    }
    // opened windows
    for (QWidget *w : QApplication::topLevelWidgets()) {
        if (w == g_win || !w->isVisible())
            continue;
        QString name = w->metaObject()->className();
        note("window/" + name);
        clickAll(w, name + "/", {});
        w->close();
        QApplication::processEvents();
    }
    note("main/pushButton_15 (Abort)");
    if (auto *b = g_win->findChild<QPushButton *>("pushButton_15"))
        b->click();
    note("END");
}

// File menu: instead of the dialog, g_pickConfigFile returns the name from g_dialogFile.
QString g_dialogFile;
int g_dialogCalls = 0;

void caseFileMenu()
{
    g_closeModals = false;
    loadConfig();
    g_pickConfigFile = [](QWidget *, bool, const QString &) {
        ++g_dialogCalls;
        return g_dialogFile;
    };
    auto act = [](const char *name) {
        QAction *a = g_win->findChild<QAction *>(name);
        if (!a)
            fail(std::string("Action missing: ") + name);
        a->trigger();
        QApplication::processEvents();
    };
    auto *iter = widget<QSpinBox>("spinBox_3");
    const int orig = iter->value();
    // Save As to a new name without extension (.sps is appended)
    QDir().mkpath("anderer Ordner");
    const QString sub = QDir::current().absoluteFilePath("anderer Ordner");
    g_dialogFile = sub + "/menu_test";
    act("actionSave_Configuration_As");
    bool saved = QFile::exists(sub + "/menu_test.sps");
    // Change, Save (without dialog) to the same file, change again, Open
    iter->setValue(orig + 7);
    const int callsBeforeSave = g_dialogCalls;
    act("actionSave_Configuration");
    const bool saveNoDialog = g_dialogCalls == callsBeforeSave;
    iter->setValue(orig + 99);
    widget<QLineEdit>("lineEdit_4")->setText("/anderswo");   // Work Path comes from the file's directory
    g_dialogFile = sub + "/menu_test.sps";
    act("actionOpen_Configuration");
    dumpScalars("result.txt", {{"saved_as", saved ? 1.0 : 0.0},
                               {"iterations_reloaded", double(iter->value())},
                               {"iterations_expected", double(orig + 7)},
                               {"workpath_from_file", widget<QLineEdit>("lineEdit_4")->text() == sub ? 1.0 : 0.0},
                               {"title", g_win->windowTitle().contains("menu_test.sps") ? 1.0 : 0.0},
                               {"save_no_dialog", saveNoDialog ? 1.0 : 0.0}});
}

// Button "Solve": solve once with the given RVs; writes components and snr_estimated.dat
void caseSolve()
{
    loadConfig();
    call("on_pushButton_3_clicked");
    copyOutput(widget<QLineEdit>("lineEdit_5")->text());
    copyOutput(widget<QLineEdit>("lineEdit_8")->text());
    copyOutput("snr_estimated.dat");
    dumpScalars("result.txt", {{"residuum", residu}});
}

// Solve, then Data tab "Detect" (ranges of the static lines from the static spectrum)
void caseDetectStatic()
{
    loadConfig();
    call("on_pushButton_3_clicked");
    call("on_pushButton_telldetect_clicked");
    const QString f = widget<QLineEdit>("lineEdit_tellrange")->text();
    copyOutput(f);
    copyOutput(widget<QLineEdit>("lineEdit_10")->text());
    std::ofstream o(QDir(g_out).filePath("main.txt").toStdString());
    o << "tellrange\t" << f.toStdString() << "\n";
    dumpScalars("result.txt", {{"residuum", residu}});
}

// Generic: load the configuration, call the slots of SPECTEST_SLOTS (comma separated) in
// turn. SPECTEST_CLOSE_MODALS=1: message boxes are logged (MODALS.txt) and closed, the run goes
// on (state after an error). For the test campaign (tests/tools/campaign.py).
void caseSlots()
{
    g_closeModals = qEnvironmentVariableIsSet("SPECTEST_CLOSE_MODALS");
    loadConfig();
    const QStringList slotList = qEnvironmentVariable("SPECTEST_SLOTS").split(',', Qt::SkipEmptyParts);
    std::ofstream log(QDir(g_out).filePath("slots.txt").toStdString());
    for (const QString &sl : slotList) {
        log << sl.toStdString() << std::endl;
        if (sl.startsWith("set:")) {          // set:widget=value (main window), between the slots
            const QString nv = sl.mid(4);
            const int eq = nv.indexOf('=');
            const QString name = nv.left(eq), val = nv.mid(eq + 1);
            QObject *o = g_win->findChild<QObject *>(name);
            if (auto *e = qobject_cast<QLineEdit *>(o)) e->setText(val);
            else if (auto *b = qobject_cast<QSpinBox *>(o)) b->setValue(val.toInt());
            else if (auto *d = qobject_cast<QDoubleSpinBox *>(o)) d->setValue(val.toDouble());
            else if (auto *c = qobject_cast<QCheckBox *>(o)) c->setChecked(val.toInt() != 0);
            else if (auto *cb = qobject_cast<QComboBox *>(o)) cb->setCurrentIndex(val.toInt());
            else fail("set: widget missing: " + name.toStdString());
            continue;
        }
        call(sl.trimmed().toUtf8().constData());
        QApplication::processEvents();
    }
    log << "END" << std::endl;
    dumpScalars("result.txt", {{"residuum", residu}});
}

// Input fuzzing of a dialog or the main window (test campaign): SPECTEST_PRE slots of the main
// window first (e.g. Read data), then SPECTEST_OPEN (slot that opens the dialog; empty: main
// window). Every line edit, spin box, combo box and check box of the window gets a value of
// the set SPECTEST_FUZZ (0..7), then all enabled buttons are clicked (main window: only
// SPECTEST_BUTTONS). Message boxes are logged (MODALS.txt) and closed; every step is written
// to fuzz.txt before it runs, so that a crash can be attributed.
void caseFuzz()
{
    g_closeModals = true;
    loadConfig();
    std::ofstream log(QDir(g_out).filePath("fuzz.txt").toStdString());
    auto note = [&](const QString &w) { log << w.toStdString() << std::endl; log.flush(); };
    for (const QString &sl : qEnvironmentVariable("SPECTEST_PRE").split(',', Qt::SkipEmptyParts)) {
        note("pre " + sl);
        call(sl.toUtf8().constData());
        QApplication::processEvents();
    }
    const QString open = qEnvironmentVariable("SPECTEST_OPEN");
    QWidget *win = g_win;
    if (!open.isEmpty()) {
        const QList<QWidget *> before = QApplication::topLevelWidgets();
        note("open " + open);
        call(open.toUtf8().constData());
        QApplication::processEvents();
        win = nullptr;
        for (QWidget *w : QApplication::topLevelWidgets())
            if (w != g_win && w->isVisible() && !before.contains(w)) win = w;
        if (!win) for (QWidget *w : QApplication::topLevelWidgets())
            if (w != g_win && w->isVisible() && qobject_cast<QDialog *>(w)) win = w;
        if (!win) fail("No window opened by " + open.toStdString());
    }
    const int mode = qEnvironmentVariableIntValue("SPECTEST_FUZZ");
    const QStringList vals = {"", "abc", "-1", "nan", "0", "1e308", "nofile.txt", "/"};
    const QString v = vals[mode % vals.size()];
    const QStringList keep = qEnvironmentVariable("SPECTEST_KEEP").split(',', Qt::SkipEmptyParts);
    for (QLineEdit *e : win->findChildren<QLineEdit *>()) {
        if (e->objectName().isEmpty() || keep.contains(e->objectName()) || e->isReadOnly()
            || qobject_cast<QAbstractSpinBox *>(e->parentWidget())) continue;
        note("set " + e->objectName() + " = '" + v + "'");
        e->setText(v);
    }
    for (QSpinBox *b : win->findChildren<QSpinBox *>()) {
        if (b->objectName().isEmpty() || keep.contains(b->objectName())) continue;
        const int x = (mode % 2) ? b->minimum() : b->maximum();
        note("set " + b->objectName() + " = " + QString::number(x));
        b->setValue(x);
    }
    for (QDoubleSpinBox *b : win->findChildren<QDoubleSpinBox *>()) {
        if (b->objectName().isEmpty() || keep.contains(b->objectName())) continue;
        const double x = (mode % 2) ? b->minimum() : b->maximum();
        note("set " + b->objectName() + " = " + QString::number(x));
        b->setValue(x);
    }
    for (QComboBox *c : win->findChildren<QComboBox *>()) {
        if (c->objectName().isEmpty() || keep.contains(c->objectName()) || c->count() == 0) continue;
        note("set " + c->objectName() + " = index " + QString::number(mode % c->count()));
        c->setCurrentIndex(mode % c->count());
    }
    for (QCheckBox *c : win->findChildren<QCheckBox *>()) {
        if (c->objectName().isEmpty() || keep.contains(c->objectName())) continue;
        note("set " + c->objectName() + " = " + QString::number(mode % 2));
        c->setChecked(mode % 2);
    }
    QApplication::processEvents();
    QStringList buttons;
    if (win == g_win) buttons = qEnvironmentVariable("SPECTEST_BUTTONS").split(',', Qt::SkipEmptyParts);
    QList<QPushButton *> bl = win->findChildren<QPushButton *>();
    std::sort(bl.begin(), bl.end(), [](QPushButton *a, QPushButton *b) { return a->objectName() < b->objectName(); });
    for (QPushButton *b : bl) {
        if (b->objectName().isEmpty()) continue;
        if (win == g_win && !buttons.contains(b->objectName())) continue;
        const QString t = b->text().toLower();
        if (t.contains("close") || t.contains("quit") || t.contains("stop") || t.contains("abort")) continue;
        if (!b->isEnabled()) { note("skip disabled " + b->objectName()); continue; }
        note("click " + b->objectName() + " \"" + b->text() + "\"");
        b->click();
        QApplication::processEvents();
    }
    note("END");
}

// Field "Use" (cores): usable and effective
void caseCores()
{
    auto *use = widget<QSpinBox>("spinBox_8");
    const bool enabled = use->isEnabled();
    const int maxCores = use->maximum();
    use->setValue(1);
    const int t1 = dis::threads();
    use->setValue(maxCores);
    const int tm = dis::threads();
    dumpScalars("result.txt", {{"enabled", enabled ? 1.0 : 0.0}, {"max", double(maxCores)},
                               {"threads_1", double(t1)}, {"threads_max", double(tm)}});
}

// Default of "Use" (cores) and the note above 2 cores: shown when the user (field with the
// focus) goes above 2, once per crossing; not when the program sets the value.
// Run with SPECTANGULAR_CORES=8 (more cores than the test machine may have).
void caseCoresWarning()
{
    auto *use = widget<QSpinBox>("spinBox_8");
    const int def = use->value();
    const int maxCores = use->maximum();
    g_closeModals = true;
    auto modals = []{
        QFile f(QDir(g_out).filePath("MODALS.txt"));
        if (!f.open(QIODevice::ReadOnly)) return 0;
        return int(f.readAll().count('\n'));
    };
    // set by the program: no note
    use->clearFocus();
    use->setValue(std::min(4, maxCores));
    use->setValue(1);
    const int noFocus = modals();
    // by the user
    g_win->show();
    QApplication::setActiveWindow(g_win);   // offscreen: no window manager activates it
    use->setFocus();
    QApplication::processEvents();
    const bool focus = use->hasFocus();
    use->setValue(std::min(4, maxCores));      // above 2: note
    use->setValue(std::min(5, maxCores));      // further step: no note
    const int first = modals();
    use->setValue(2);
    QApplication::setActiveWindow(g_win);      // focus back after the closed box (offscreen)
    use->setFocus();
    use->setValue(std::min(6, maxCores));      // above 2 again: note
    const int second = modals();
    g_closeModals = false;
    QFile f(QDir(g_out).filePath("MODALS.txt"));
    QString text;
    if (f.open(QIODevice::ReadOnly)) text = QString::fromUtf8(f.readAll()).split('\n').value(0);
    dumpScalars("result.txt", {{"default", double(def)}, {"max", double(maxCores)}, {"focus", focus ? 1.0 : 0.0},
                               {"modals_program", double(noFocus)}, {"modals_first", double(first)},
                               {"modals_second", double(second)}, {"threads", double(dis::threads())}});
    std::ofstream(QDir(g_out).filePath("note.txt").toStdString()) << text.toStdString() << "\n";
}

// Optimisation of the flux ratios ("Opt. Ratios"), criterion: residual
void caseOptRatios()
{
    loadConfig();
    widget<QCheckBox>("checkBox_29")->setChecked(true);
    widget<QCheckBox>("checkBox_30")->setChecked(false);
    r1 = 0;
    call("on_pushButton_13_clicked");
    copyOutput("optval_flux.dat");
    copyOutput("optmat_flux.dat");
}

// Error estimation only, on existing simplex files (optval/optmat)
void caseErrorOnly()
{
    loadConfig();
    r1 = 0;
    call("on_pushButton_8_clicked");
    copyOutput(widget<QLineEdit>("lineEdit_24")->text());
}

// Synthetic SB2 with known solution (tools/make_synthetic.py).
// velocities.txt contains physical RVs (positive = redshift).
// Evaluated with these RVs, with negated RVs, with a single spectrum perturbed
// by 1 pixel, and at the end once more with the correct RVs.
void caseSynthetic()
{
    loadConfig();
    readData();
    std::vector<double> a(RV1.begin(), RV1.begin() + num), b(RV3.begin(), RV3.begin() + num);
    auto setRv = [&](double sign) {
        for (int i = 0; i < num; ++i) {
            RV1[i] = sign * a[i];
            RV3[i] = sign * b[i];
        }
    };
    r1 = 0;
    setRv(1);
    double rPhys = evaluate();
    copyOutput(widget<QLineEdit>("lineEdit_5")->text());
    copyOutput(widget<QLineEdit>("lineEdit_8")->text());

    setRv(-1);
    double rNeg = evaluate();

    setRv(1);
    RV1[3] += dv;   // perturb one spectrum of A by 1 pixel
    double rPert = evaluate();

    setRv(1);
    double rPhys2 = evaluate();

    dumpScalars("result.txt", {{"residuum_physical", rPhys}, {"residuum_negated", rNeg},
                               {"residuum_perturbed", rPert},
                               {"residuum_physical_again", rPhys2},
                               {"bso1", double(bso1)}, {"bso2", double(bso2)},
                               {"Mval1", Mval1[0]}, {"Mval2", Mval2[0]}});
}

// Interpolation: residual when the RV of one spectrum is shifted by fractions of a pixel
// relative to the truth (data set synthetic_subpixel).
void caseInterpolation()
{
    const double c = 299792.458;
    loadConfig();
    readData();
    double dlog = (W(logbin - 1) - W(0)) / (logbin - 1);
    double v0 = RV1[3];
    double s0 = std::log10(1 + v0 / c) / dlog;
    std::map<std::string, double> out;
    for (double d : {-0.5, -0.25, -0.1, 0.0, 0.1, 0.25, 0.5}) {
        RV1[3] = c * (std::pow(10.0, (s0 + d) * dlog) - 1);
        std::ostringstream k;
        k << "r_" << std::showpos << d;
        out[k.str()] = evaluate();
    }
    RV1[3] = v0;
    dumpScalars("result.txt", out);
}

// Edge exclusion in the residual: every counted row may only touch columns that are
// seen by all spectra. Then no column in the counted rows is determined by
// only one spectrum, and there are no jumps when an RV crosses a pixel boundary.
void caseEdgeCoverage()
{
    loadConfig();
    readData();
    double r = evaluate();
    const int m = std::max(0, g_edgeMargin);
    // spectra per column
    std::vector<std::set<int>> seen(M.n_cols);
    for (arma::uword c = 0; c < M.n_cols; ++c)
        for (auto it = M.begin_col(c); it != M.end_col(c); ++it)
            seen[c].insert(int(it.row() / logbin));
    int bad = 0, counted = 0;
    for (arma::uword c = 0; c < M.n_cols; ++c) {
        if (int(seen[c].size()) == num)
            continue;
        for (auto it = M.begin_col(c); it != M.end_col(c); ++it) {
            int px = int(it.row() % logbin);
            if (px >= m && px < logbin - m)
                ++bad;
        }
    }
    for (int a = 0; a < num; ++a)
        for (int px = m; px < logbin - m; ++px)
            ++counted;
    dumpScalars("result.txt", {{"margin", double(m)}, {"bad_entries", double(bad)},
                               {"counted_rows", double(counted)}, {"bins", double(g_lastResBins)},
                               {"cols", double(g_lastResCols)}, {"residuum", r}});
}

const std::map<std::string, std::function<void()>> kCases = {
    {"config", caseConfig},
    {"config_roundtrip", caseConfigRoundtrip},
    {"read_data", caseReadData},
    {"construct_matrix", caseConstructMatrix},
    {"divide_conquer", caseDivideConquer},
    {"divide_conquer_repeat", caseDivideConquerRepeat},
    {"svd_check", caseSvdCheck},
    {"svd_check_negated", caseSvdCheckNegated},
    {"orbit_rv", caseOrbitRv},
    {"orbit_invalid", caseOrbitInvalid},
    {"optimisation", caseOptimisation},
    {"optimisation_abort", caseOptimisationAbort},
    {"solve_abort", caseSolveAbort},
    {"synthetic", caseSynthetic},
    {"interpolation", caseInterpolation},
    {"error_estimation", caseErrorEstimation},
    {"error_only", caseErrorOnly},
    {"opt_ratios", caseOptRatios},
    {"plotter_ratio", casePlotterRatio},
    {"residual_stats", caseResidualStats},
    {"orbit_fit_rvs", caseOrbitFitRvs},
    {"flux_templates", caseFluxTemplates},
    {"barycorr", caseBaryCorr},
    {"barycorr_file", caseBaryCorrFile},
    {"tellcheck", caseTellCheck},
    {"detect_static", caseDetectStatic},
    {"slots", caseSlots},
    {"fuzz", caseFuzz},
    {"error_mc", caseErrorMc},
    {"gui_smoke", caseGuiSmoke},
    {"screenshot", caseScreenshot},
    {"editor_orbit", caseEditorOrbit},
    {"edge_coverage", caseEdgeCoverage},
    {"file_menu", caseFileMenu},
    {"cores", caseCores},
    {"cores_warning", caseCoresWarning},
    {"solve", caseSolve},
    {"subtract", caseSubtract},
    {"correlation", caseCorrelation},
    {"subtract_static", caseSubtractStatic},
};

} // namespace

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Usage: spectest <case> <work_dir> <output_dir>\nCases:";
        for (const auto &kv : kCases)
            std::cerr << " " << kv.first;
        std::cerr << std::endl;
        return 1;
    }
    auto it = kCases.find(argv[1]);
    if (it == kCases.end()) {
        std::cerr << "Unknown case: " << argv[1] << std::endl;
        return 1;
    }
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);
    g_out = QDir(argv[3]).absolutePath();
    QDir().mkpath(g_out);
    if (!QDir::setCurrent(argv[2])) {
        std::cerr << "Work directory missing: " << argv[2] << std::endl;
        return 1;
    }
    startModalWatchdog();
    runow = 0;
    MainWindow w;
    g_win = &w;
    it->second();
    std::cout.flush();
    _exit(0);   // skip destructors of the globals; results have been written
}
