#include "mainwindow.h"
#include "resources.h"
#include <QFileDialog>
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <string>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <cstdlib>
#include <QVector>
#include "plotspec.h"
#include "orbit.h"
#include "disentangle.h"
#include "dataio.h"
#include "continuum.h"
#include "version.h"
#include "neldermead.h"
#include "errorestimate.h"
#include "fluxratio.h"
#include "orbitfit.h"
#include "fluxtemplate.h"
#include "barycorrdialog.h"
#include "perioddialog.h"
#include "linefinder.h"
#include "linewidth.h"
#include <random>
#include <functional>
#include "levmar.h"
#include <limits>
#include <correlation.h>
#include <edit.h>
#include <iostream>
#define ARMA_64BIT_WORD
#include <armadillo>
#include <CCfits/CCfits>
#include <algorithm>
#include <cctype>
#include <valarray>
#include <ctime>
#include <chrono>
//#include <thread>
#include <QFileInfo>
#include <QElapsedTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QTextStream>
//#include <cblas.h>

using namespace std;
using namespace arma;
using namespace arpack;
using namespace blas;
using std::chrono::system_clock;

mat ZZ(2,3);

int reinitiate=0, mini, CDI, maxi, num, checker=0, bso1, bso2, logbin, RV1m, RV3m, RV1a, RV3a, Mn, Mm, elements, bidi=0, error=0, zaehler, abortt=0, eval=0;
int upda;       // live update
int updac;      // save all updates
int inrein=0;   // reinitiate
int abauto=0;   // auto abort
int runow=0;    // start optimisation from terminal without windows; 1 for yes, 0 for no.
int overw=0;    //
int optfratios=0;   // optmize on flux ratios;
int RVCalcCheck=0;
QString g_configFile;                    // most recently loaded or saved configuration
QString g_configName = "star_orb_conf";  // suggestion for "Save Configuration As"
// Auto Stop: best residuum of the previous run (Reinitiate), iterations across all
// restarts, last used sigma^2 (for the plot)
double g_prevRunBestS = std::numeric_limits<double>::quiet_NaN();
int g_itersUsed = 0;
double g_sigma2 = std::numeric_limits<double>::quiet_NaN();
// Best state of the optimisation, written on every improvement while an optimisation runs:
// beststate_rv.dat (RVs of all spectra) and beststate_orbit.dat (7 elements) in the work
// folder. "Continue" and "Reinitiate" start from it, independent of the start values in the
// inputs and of the method (Nelder-Mead / Levenberg-Marquardt) that found it.
int g_trackBest = 0;            // 0: off, 1: RV mode, 2: orbit mode
double g_bestStateS = std::numeric_limits<double>::quiet_NaN();
bool g_contFromBest = false;     // this optimisation starts from the best state
// Excluded wavelength ranges (Model tab, e.g. telluric bands): per row of M (spectrum x pixel)
// 1 = left out of the solution and the residuum
std::vector<char> g_rowMasked;
inline bool rowMasked(arma::uword i){ return i<g_rowMasked.size() && g_rowMasked[i]; }
// With "Exclude edges": rows within the edge margin of an excluded range (same spectrum) see
// component pixels that lost most of their data to the range; like the spectrum edges they
// do not count in the residuum (they stay in the solution)
std::vector<char> g_rowNearMask;
inline bool rowUncounted(arma::uword i){ return rowMasked(i) || (i<g_rowNearMask.size() && g_rowNearMask[i]); }
// Rows in the ranges of the static lines, left out of the residuum only (Model tab, "Exclude the
// ranges of the static lines from the residuum"): residuum, residual statistics, S/N estimate
std::vector<char> g_rowResExcl;
inline bool rowResExcluded(arma::uword i){ return i<g_rowResExcl.size() && g_rowResExcl[i]; }
// entries per column in total and in non-excluded rows (component pixels that lost more than
// half of their data are not written)
std::vector<arma::uword> g_colCountFull, g_colCountReal;
int g_lsqrThreadsLast = 0;        // threads of the last LSQR solve (status line)
bool g_abortableRun = false;     // optimisation / error estimation running: Abort ends a solve
int optimisationFinished=0;  // 1: optimisation finished normally (exit code in batch mode)
int batchOverwrite=0;  // --overwrite: overwrite existing start files
int batchThreads=0;    // --threads N: number of cores in batch mode (0: default)
const unsigned kDefaultCores = 2;   // default number of cores (see the constructor)
unsigned int cores;
string path, eins, zwei, line;
QString qPath, qExtension, qWCol, qICol, qInitval, qInitmat, qOptval, qOptmat;
// File names set in the Output tab (best state, convergence data, residual statistics) and in
// the Data tab (orbital elements); kept in sync with the line edits (MainWindow constructor).
// Empty fields fall back to the defaults.
QString qBestState = "beststate.dat", qOptdata = "optdata.dat", qDiffStat = "diffstatistic.dat",
        qOrbitFile = "orbitelements.dat";

QVector<double> RV1(1), RV2(1), RV3(1), edits(5), Mval1(1), Mval2(1), Mtel(1), otimes(1), orbele(7), dorbele(7);
QVector<double> cpoints(8);
double RV1max, RV3max, dv, residu, RV1min, RV3min, RV1maxi, RV3maxi, difference, RV1amin, RV3amin, r1, Diff, fluxratio, ldiff, Per, T0, ecc, Omega1, gama, K1, K2;
double RVT0, RVt, RVe, RVE, RVP, theta;
const double c0 = 299792.458;
string Extension, WCol, ICol;
std::valarray<double> wave;
std::valarray<double> intens;

double step=1;  //step size for initial points, used: dv
double Gamma=2.0;   //expansion coeff.
double alpha=1.0;   //reflection coeff.
double nmBeta=0.5;  // contraction coeff. (not "beta": std::beta in C++17)
double btot=0.5;    //total contraction coeff.

double dP;
double de;
double dKA;
double dKB;
double dGamma;
double dT0;
double dOmega;

sp_mat M;       //transformation matrix (sparse)
mat Mt(Mm,Mn); //transpose of transformation matrix
vec X(Mm);      //solution vector of SVD
vec C(Mn);      //measurement vector
vec W(Mm);      //wavelength vector of measurement
vec XW1(Mm);    //wavelength vector component 1
vec XW2(Mm);    //wavelength vector component 2
umat loc(2,Mm); //matrix of positions for sparse SVD
vec val(Mm);    //vector with values for sparse SVD
dis::Layout g_layout;   // column layout of the current matrix
double g_dlog = 0.0;    // grid step in log10(lambda)
bool g_matrixValid = false;
int g_lastResBins = 0;   // number of bins of the last residuum
int g_lastResCols = 0;   // unknowns determined by these bins
// Edge per spectrum (pixels) that does not count in the residuum ("Exclude edges").
// -1: determine from the RV span at the next matrix construction. read_data() resets
// it; within one optimisation or error estimate it stays fixed, so that the
// residuum is a continuous function of the parameters.
int g_edgeMargin = -1;
// Barycentric correction BC [km/s] per spectrum (Data tab, read by read_data); empty: none
std::vector<double> g_bc;
// strength of the static lines per spectrum to be fitted at the next solve (Data tab)
bool g_tellFitPending = false;
// static lines only in these ranges (log10 lambda, observatory frame; Data tab); empty: all
std::vector<std::pair<double, double>> g_tellRanges;
// groups of static lines with their own strength (label of the ranges, e.g. O2, H2O): group
// of each range, names, strengths per group and spectrum (only with more than one group)
std::vector<int> g_tellRangeGroup;
std::vector<QString> g_tellGroupNames;
std::vector<std::vector<double>> g_MtelG;
constexpr int kTellFitMaxIter = 10;
// strength of the static lines: lower bound of the fit (relative, before the scaling to a mean
// of 1), and below which a spectrum is reported as not fitting the static lines
constexpr double kMinStrength = 1e-3;
constexpr double kWeakStrength = 0.1;

// Velocity in the observatory frame for a barycentric velocity v and the correction bc:
// lambda_bary = lambda_obs (1 + bc/c), so (1 + v_obs/c) = (1 + v/c) / (1 + bc/c)
static double toObservatory(double v, double bc)
{
    const double c = dis::kSpeedOfLight;
    return c*((1.0+v/c)/(1.0+bc/c)-1.0);
}
static double toBarycentric(double vObs, double bc)
{
    const double c = dis::kSpeedOfLight;
    return c*((1.0+vObs/c)*(1.0+bc/c)-1.0);
}
// Wavelength dependence of the flux ratio: s(lambda) per pixel of the common grid
// (empty: constant ratio, the weights stay per spectrum as before)
std::vector<double> g_ratioShape;
double g_ratioLref = 0.0;   // reference wavelength used (resolved "auto")
fr::Model g_ratioModel;     // model used for g_ratioShape
double g_ratioKref = 1.0;   // reference ratio of the diluted unknowns (buildDesignMatrix)
arma::vec g_colScale;       // x = g_colScale % X for the component output (empty: x = X)
// Weighting by S/N per spectrum ("Weight by S/N from"): weight w_j = sigma_bar/sigma_j with
// sigma_j = 1/(S/N)_j and sigma_bar^2 = 1/mean(1/sigma_j^2), so that the weighted residuum
// stays in flux units and chi^2 = residuum^2/sigma_bar^2. Empty: unweighted.
std::vector<double> g_specW;
double g_sigmaBar2 = 0.0;
// S/N file "auto": weights are estimated at the first solve after reading the data
// (3 iterations solve - estimate - weight) and then kept fixed for the run
bool g_autoWeightsPending = false;
// estimated weights are kept for restarts (Reinitiate) and a following error estimation, so
// that all runs of an optimisation use the same objective; cleared at the start of an
// optimisation, by "Read Data" and by "Solve"
std::vector<double> g_autoSpecW;
double g_autoSigmaBar2 = 0.0;
// LSQR warm start: the last LSQR solution and its column layout. The next solve starts
// from it (transferred to the new layout) and only solves for the correction; the
// inherited continuum offsets are removed afterwards (removeOffsetNullspace), so the
// result is the minimum-norm solution as from a cold start. Cleared by read_data(), after
// a failed solve and by the SVD solver. SPECTANGULAR_COLD_START=1 disables it.
arma::vec g_warmX;
dis::Layout g_warmLayout;
bool g_warmValid = false;
long long g_lsqrIterations = 0;   // LSQR iterations since the last read_data() (statistics)
long long g_lsqrSolves = 0;

// weights w_j = sigma_bar/sigma_j from S/N values (see g_specW); false if a value is not positive
static bool setWeightsFromSnr(const std::vector<double> &snr)
{
    double m2 = 0.0;
    for(double v : snr){
        if(!(v>0) || !std::isfinite(v)) return false;
        m2 += v*v/snr.size();
    }
    g_sigmaBar2 = 1.0/m2;
    g_specW.clear();
    for(double v : snr) g_specW.push_back(v/std::sqrt(m2));
    return true;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Spectangular - Spectral Disentangling");
    {   // build identification (commit), so that results can be traced to a program version
        const QString b = QString("Spectangular build %1").arg(spectangularCommit());
        cout<<b.toStdString()<<endl;
        ui->plainTextEdit_2->appendPlainText(b);
    }

    ui->lineEdit->setText("binaryrv_");
    ui->lineEdit_2->setText("velocities.txt");
    ui->lineEdit_3->setText("rebined_spectra_");
    ui->lineEdit_18->setText("times.dat");
    ui->lineEdit_4->setText(QDir::currentPath());
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    ui->lineEdit_14->setText("initval_star_orb.dat");
    qInitval = ui->lineEdit_14->text();
    ui->lineEdit_15->setText("initmat_star_orb.dat");
    qInitmat = ui->lineEdit_15->text();

    ui->lineEdit_16->setText("optval_star_orb.dat");
    qOptval = ui->lineEdit_16->text();
    ui->lineEdit_17->setText("optmat_star_orb.dat");
    qOptmat = ui->lineEdit_17->text();

    ui->lineEdit_6->setText("0");
    ui->lineEdit_7->setText("0");

    ui->checkBox->setChecked(true);
    ui->checkBox_5->setChecked(true);    // solver: LSQR
    ui->checkBox_7->setChecked(false);
    ui->checkBox_4->setChecked(true);
    ui->checkBox_10->setChecked(true);
    ui->checkBox_11->setChecked(true);
    ui->checkBox_14->setChecked(true);
    ui->checkBox_22->setChecked(false);
    ui->checkBox_40->setChecked(true);   // residuum without edge pixels

    ui->comboBox->addItem("fits");
    ui->comboBox->addItem("ASCII");


    ui->lineEdit_9->setText("DataVector");
    qExtension=ui->lineEdit_9->text();
    Extension=qExtension.toUtf8().constData();

    ui->lineEdit_11->setText("Arg");
    qWCol=ui->lineEdit_11->text();
    WCol=qWCol.toUtf8().constData();

    ui->lineEdit_12->setText("Fun");
    qICol=ui->lineEdit_12->text();
    ICol=qICol.toUtf8().constData();

    ui->spinBox_3->setValue(9999);
    zaehler=ui->spinBox_3->value();

    ui->doubleSpinBox_4->setValue(1);
    fluxratio=ui->doubleSpinBox_4->value();

    ui->lineEdit_26->setText("txt");
    ui->lineEdit_25->setText("star_orb");


    ui->lineEdit_24->setText("errors.dat");
    ui->lineEdit_tellrange->setText("telluric_static.dat");
    // file names of the Output tab and the orbit file: globals follow the fields
    {
        struct F { QLineEdit *e; QString *g; const char *def; };
        const F fs[] = {{ui->lineEdit_best, &qBestState, "beststate.dat"},
                        {ui->lineEdit_optdata, &qOptdata, "optdata.dat"},
                        {ui->lineEdit_stat, &qDiffStat, "diffstatistic.dat"},
                        {ui->lineEdit_orbit, &qOrbitFile, "orbitelements.dat"}};
        for(const F &f : fs){
            QString *g = f.g;
            const QString def = f.def;
            connect(f.e, &QLineEdit::textChanged, this, [g, def](const QString &t){
                *g = t.trimmed().isEmpty() ? def : t.trimmed();
            });
            f.e->setText(f.def);
        }
    }

    ui->checkBox_32->setChecked(true);
    ui->checkBox_23->setChecked(true);
    ui->checkBox_29->setChecked(true);


    // "Bidiagonal Matrix" has been replaced by linear interpolation in the matrix construction.
    ui->checkBox_7->setChecked(false);
    ui->checkBox_7->setEnabled(false);
    ui->checkBox_7->setToolTip("Replaced by interpolation between pixels.");
    ui->checkBox_7->setVisible(false);   // kept only for the config file format
    bidi=0;
    //ui->lineEdit_19->setEnabled(false);

    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis->setTickLength(1);
    //ui->customPlot->setBackground(Qt::transparent);
    ui->customPlot->xAxis->setTickPen(QPen(Qt::black));
    ui->customPlot->xAxis->setLabelColor(Qt::black);
    ui->customPlot->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setTickPen(QPen(Qt::black));
    ui->customPlot->yAxis->setLabelColor(Qt::black);
    ui->customPlot->yAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setLabel("rel. Flux");
    ui->customPlot->xAxis->setLabel("Wavelength");

    ui->customPlot_2->xAxis2->setVisible(true);
    ui->customPlot_2->xAxis2->setTickLabels(false);
    ui->customPlot_2->xAxis2->setTicks(false);
    ui->customPlot_2->yAxis2->setVisible(true);
    ui->customPlot_2->yAxis2->setTickLabels(false);
    ui->customPlot_2->yAxis2->setTicks(false);
    //ui->customPlot_2->setBackground(Qt::transparent);
    ui->customPlot_2->xAxis->setTickPen(QPen(Qt::black));
    ui->customPlot_2->xAxis->setLabelColor(Qt::black);
    ui->customPlot_2->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot_2->yAxis->setTickPen(QPen(Qt::black));
    ui->customPlot_2->yAxis->setLabelColor(Qt::black);
    ui->customPlot_2->yAxis->setTickLabelColor(Qt::black);
    ui->customPlot_2->yAxis->setLabel("rel. Flux");
    ui->customPlot_2->xAxis->setLabel("Wavelength");

    ui->customPlot_2->hide();
    // the differences plot was removed; the offset stays only for old configuration files
    ui->doubleSpinBox_5->hide();
    ui->lbl_68->hide();


    ui->customPlot_4->xAxis2->setVisible(true);
    ui->customPlot_4->xAxis2->setTickLabels(false);
    ui->customPlot_4->xAxis2->setTicks(false);
    ui->customPlot_4->yAxis2->setVisible(true);
    ui->customPlot_4->yAxis2->setTickLabels(false);
    ui->customPlot_4->yAxis2->setTicks(false);
    ui->customPlot_4->yAxis->setLabel("Residuum");
    ui->customPlot_4->xAxis->setLabel("Iterations");

    // residuals per spectrum (x label set in plotResidualStatistics: phase, time or number)
    ui->customPlot_5->xAxis2->setVisible(true);
    ui->customPlot_5->xAxis2->setTickLabels(false);
    ui->customPlot_5->xAxis2->setTicks(false);
    ui->customPlot_5->yAxis2->setVisible(true);
    ui->customPlot_5->yAxis2->setTickLabels(false);
    ui->customPlot_5->yAxis2->setTicks(false);
    ui->customPlot_5->yAxis->setLabel("O-M");
    ui->customPlot_5->xAxis->setLabel("Orbital Phase");
    // the same against the number of the spectrum (outliers labelled)
    ui->customPlot_6->xAxis2->setVisible(true);
    ui->customPlot_6->xAxis2->setTickLabels(false);
    ui->customPlot_6->xAxis2->setTicks(false);
    ui->customPlot_6->yAxis2->setVisible(true);
    ui->customPlot_6->yAxis2->setTickLabels(false);
    ui->customPlot_6->yAxis2->setTicks(false);
    ui->customPlot_6->yAxis->setLabel("O-M");
    ui->customPlot_6->xAxis->setLabel("Spectrum");

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this ,SLOT(showPointToolTip(QMouseEvent*)));

    // Number of cores: the machine's count is displayed, the "Use" setting is applied.
    // Default 2 (if available): only the matrix-vector products of LSQR run in parallel and
    // are limited by the memory bandwidth; in a scaling test on long spectra (14-core
    // machine) 2 cores were fastest, more and fewer took longer. Batch mode: --threads N.
    // SPECTANGULAR_CORES overrides the detected number (tests, containers).
    cores = std::max(1u, thread::hardware_concurrency());
    if(const char *ec = std::getenv("SPECTANGULAR_CORES")){
        const int n = std::atoi(ec);
        if(n>=1) cores = unsigned(n);
    }
    ui->spinBox_7->setValue(int(cores));
    ui->spinBox_8->setMinimum(1);
    ui->spinBox_8->setMaximum(int(cores));
    ui->spinBox_8->setValue(int(std::min(cores, kDefaultCores)));
    cores = unsigned(ui->spinBox_8->value());
    dis::setThreads(int(cores));
    // long solves (LSQR on long spectra take seconds): keep the window responsive and let
    // "Abort" end a solve of an optimisation or error estimation
    // (user input only while the buttons are disabled, i.e. during these runs; otherwise only
    // repaints, so that no second action starts in the middle of a solve)
    dis::setProgressHook([]{
        qApp->processEvents(g_abortableRun ? QEventLoop::AllEvents : QEventLoop::ExcludeUserInputEvents);
        return g_abortableRun && abortt==1;
    });
    if(!dis::openmpAvailable()){
        const char *m = "Note: built without OpenMP; LSQR uses one core whatever the number of cores "
                        "(compiler without OpenMP support, e.g. Clang without libomp-dev; see the qmake output).";
        cout<<m<<endl;
        ui->plainTextEdit_2->appendPlainText(m);
    }
    if(dis::setBlasThreads(int(cores))==0){
        cout<<"Note: no multi-threaded BLAS (OpenBLAS/BLIS/MKL) loaded; SVD (DaC) uses one core."<<endl;
    }

    //cout<<"Run optimization now? 0 no, 1 yes: "<<endl;
    //cin>>runow;
    //runow=1;

    // Batch mode is started via main.cpp (--batch).

    // Layout: subtract tool as a dock (Tools menu), live summary of what is fitted and how
    ui->dockSubtract->hide();
    // "Bidiagonal Matrix" has no effect on the solution (flag unused); kept only so that
    // configuration files keep their format
    ui->checkBox_7->hide();
    for(QCheckBox *c : {ui->checkBox, ui->checkBox_2, ui->checkBox_16})
        connect(c, &QCheckBox::toggled, this, [this]{ updateFitTarget(); });
    for(QCheckBox *c : {ui->checkBox_5, ui->checkBox_6})
        connect(c, &QCheckBox::toggled, this, [this]{ updateRunInfo(); });
    connect(ui->comboBox_interp, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]{ updateRunInfo(); });
    // residuum without the ranges of the static lines: with read data at once (clicked: not
    // while a configuration is loaded)
    connect(ui->checkBox_resstatic, &QCheckBox::clicked, this, [this](bool){
        if(!g_matrixValid) return;
        if(!buildResidualExclusion()) ui->checkBox_resstatic->setChecked(false);
    });
    connect(ui->comboBox_method, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]{ updateRunInfo(); });
    connect(ui->spinBox_8, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]{ updateRunInfo(); });
    connect(ui->comboBox_ratio, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]{ updateRatioControls(); });
    updateFitTarget();
    updateRunInfo();
    updateRatioControls();
}

// Flux-ratio model: only the fields of the chosen model are active
void MainWindow::updateRatioControls()
{
    const int m = ui->comboBox_ratio->currentIndex();
    ui->doubleSpinBox_TA->setEnabled(m==1);
    ui->doubleSpinBox_TB->setEnabled(m==1);
    ui->lineEdit_ratio->setEnabled(m==2);
    ui->doubleSpinBox_lref->setEnabled(m!=0);
}

// "What is fitted" follows the RV source: file or cross-correlation -> individual RVs,
// orbit -> orbital elements (free elements only then)
void MainWindow::updateFitTarget()
{
    const bool orbitMode = ui->checkBox_16->isChecked();
    ui->labelFitTarget->setText(orbitMode
        ? "Orbital elements (RV source: orbit, Data tab)"
        : "Individual RVs (RV source: file or cross-correlation, Data tab)");
    for(QWidget *w : std::initializer_list<QWidget*>{ui->labelFreeElements, ui->checkBox_18, ui->checkBox_17,
                     ui->checkBox_31, ui->checkBox_36, ui->checkBox_37, ui->checkBox_35, ui->checkBox_38}){
        // K_A, K_B, T_peri sit in a row with their rich-text label: disable the row
        QWidget *row = w->parentWidget();
        (row && row->objectName().startsWith("row_") ? row : w)->setEnabled(orbitMode);
    }
    for(QWidget *w : std::initializer_list<QWidget*>{ui->labelFixedRV, ui->lineEdit_fixA, ui->labelFixedRVB,
                     ui->lineEdit_fixB}){
        w->setEnabled(!orbitMode);
    }
}

// Spectrum numbers "0, 3, 7-9" (as in the file names, mini..maxi) -> indices 0..num-1
static bool parseSpectrumList(const QString &text, int first, int last, std::vector<int> &out, QString &err)
{
    out.clear();
    for(QString part : text.split(',', Qt::SkipEmptyParts)){
        part = part.trimmed();
        if(part.isEmpty()) continue;
        bool ok1 = true, ok2 = true;
        int a, b;
        const int dash = part.indexOf('-', 1);
        if(dash>0){
            a = part.left(dash).trimmed().toInt(&ok1);
            b = part.mid(dash+1).trimmed().toInt(&ok2);
        }
        else{
            a = b = part.toInt(&ok1);
        }
        if(!ok1 || !ok2 || a>b){
            err = "'"+part+"' is not a spectrum number or range.";
            return false;
        }
        if(a<first || b>last){
            err = "spectrum "+part+" is outside "+QString::number(first)+"-"+QString::number(last)+".";
            return false;
        }
        for(int k=a; k<=b; k++) out.push_back(k-first);
    }
    std::sort(out.begin(), out.end());
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return true;
}

// Free RVs of the RV optimisation as full indices (A: 0..num-1, B: num..2num-1), from the
// fields "Fixed RVs A / B" of the Optimisation tab.
bool MainWindow::freeRVIndices(bool sb1, std::vector<int> &freeIdx, QString &err) const
{
    std::vector<int> fa, fb;
    if(!parseSpectrumList(ui->lineEdit_fixA->text(), mini, maxi, fa, err)){ err = "Fixed RVs A: "+err; return false; }
    if(!parseSpectrumList(ui->lineEdit_fixB->text(), mini, maxi, fb, err)){ err = "Fixed RVs B: "+err; return false; }
    freeIdx.clear();
    for(int j=0; j<num; j++) if(!std::binary_search(fa.begin(), fa.end(), j)) freeIdx.push_back(j);
    if(!sb1) for(int j=0; j<num; j++) if(!std::binary_search(fb.begin(), fb.end(), j)) freeIdx.push_back(num+j);
    if(freeIdx.empty()){
        err = "All RVs are fixed.";
        return false;
    }
    return true;
}

void MainWindow::updateRunInfo()
{
    QString cores = QString("%1 core%2").arg(ui->spinBox_8->value()).arg(ui->spinBox_8->value()==1 ? "" : "s");
    if(!dis::openmpAvailable()) cores = "LSQR 1 core (built without OpenMP)";
    else if(g_lsqrThreadsLast>0 && !ui->checkBox_6->isChecked())
        cores += QString(", LSQR %1 used").arg(g_lsqrThreadsLast);
    ui->labelRunInfo->setText(QString("%1 · %2 · %3 · %4")
        .arg(ui->checkBox_6->isChecked() ? "SVD (DaC)" : "LSQR")
        .arg(ui->comboBox_interp->currentIndex()==1 ? "cubic" : "linear")
        .arg(ui->comboBox_method->currentIndex()==1 ? "Levenberg–Marquardt" : "Nelder–Mead")
        .arg(cores));
}

// Tools -> Orbit fit: Keplerian orbit fitted to the current RVs (e.g. after an optimisation
// of individual RVs) and the times of observation, or to a file
void MainWindow::on_actionOrbitFit_triggered()
{
    qPath = ui->lineEdit_4->text();
    std::vector<double> t, a, b;
    QString note;
    bool valid = false;
    const bool sb2 = ui->checkBox_4->isChecked();
    if(num>0 && RV1.size()>=num && (!sb2 || RV3.size()>=num)){
        if(int(otimes.size())>=num && ui->checkBox_16->isChecked()){
            t.assign(otimes.begin(), otimes.begin()+num);
        }
        else{
            std::vector<std::vector<double>> rows;
            std::string err;
            if(dataio::readTable((qPath+"/"+ui->lineEdit_18->text()).toStdString(), 1, rows, err)
                    && int(rows.size())==num){
                for(const auto &r : rows) t.push_back(r[0]);
            }
        }
        if(int(t.size())==num){
            a.assign(RV1.begin(), RV1.begin()+num);
            if(sb2) b.assign(RV3.begin(), RV3.begin()+num);
            // RV mode on spectra in the observatory frame: barycentric RVs for the orbit fit
            if(!ui->checkBox_16->isChecked() && int(g_bc.size())>=num && ui->comboBox_bcframe->currentIndex()==0){
                for(int j=0; j<num; j++){
                    a[j] = toBarycentric(a[j], g_bc[j]);
                    if(sb2) b[j] = toBarycentric(b[j], g_bc[j]);
                }
            }
            valid = true;
            note = QString("Main window: %1 spectra, times from %2, current RVs%3.").arg(num)
                   .arg(ui->checkBox_16->isChecked() ? "the orbit mode" : ui->lineEdit_18->text())
                   .arg(ui->checkBox_16->isChecked() ? " (computed from the orbit)" : "");
        }
        else note = "Main window: no times file with one time per spectrum ("+ui->lineEdit_18->text()+").";
    }
    else note = "Main window: no data read.";
    OrbitFit *dlg = new OrbitFit(this);
    dlg->setOrbitFile(qOrbitFile);
    // RVs of the RV mode: zero point of each component arbitrary (also with static lines)
    dlg->setMainData(qPath, t, a, b, sb2, valid, note, !ui->checkBox_16->isChecked());
    dlg->show();
}

// Tools -> Flux ratio from synthetic spectra: flux ratio from a synthetic spectrum per
// component, renormalisation of the disentangled spectra
void MainWindow::on_actionFluxTemplate_triggered()
{
    qPath = ui->lineEdit_4->text();
    fr::Model m;
    std::string err;
    if(!ratioModelFromUi(m, err)){
        ui->plainTextEdit_2->appendPlainText("Flux ratio from synthetic spectra: "+QString::fromStdString(err)+" Constant ratio assumed.");
        m = fr::Model();
    }
    if(!(m.lambdaRef>0) && g_ratioLref>0) m.lambdaRef = g_ratioLref;   // "auto" as resolved when reading
    FluxTemplate *dlg = new FluxTemplate(this);
    dlg->setMainData(qPath, ui->lineEdit_5->text(), ui->lineEdit_8->text(), ui->doubleSpinBox_4->value(), m);
    connect(dlg, &FluxTemplate::ratioAccepted, this, [this](double k){
        ui->doubleSpinBox_4->setValue(k);
        ui->plainTextEdit_2->appendPlainText(QString("Flux ratio set to %1 (from synthetic spectra); "
                                                     "disentangle again to use it.").arg(ui->doubleSpinBox_4->value()));
    });
    dlg->show();
}

static bool readFitsSpectrum(const std::string &file, const std::string &extName, const std::string &wcol,
                             const std::string &icol, std::valarray<double> &w, std::valarray<double> &f,
                             std::string &usedExt, std::string &err);

// Tools -> Barycentric correction: spectra (and start RVs) shifted to the barycentre
// Tools -> Period analysis (residual statistics, fit per spectrum and line windows of the solution)
void MainWindow::on_actionPeriod_triggered()
{
    qPath = ui->lineEdit_4->text();
    PeriodDialog::MainData d;
    d.work = qPath;
    d.statFile = qDiffStat;
    d.fitFile = ui->lineEdit_anaFit->text().trimmed();
    d.lineStatFile = ui->lineEdit_anaLines->text().trimmed();
    d.timesFile = ui->lineEdit_18->text();
    d.first = ui->spinBox->value();
    d.orbitalPeriod = orbele.size()>=1 && orbele[0]>0 ? orbele[0] : 0.0;
    if(!(d.orbitalPeriod>0)){
        std::vector<std::vector<double>> rows;
        std::string err;
        if(dataio::readTable((qPath+"/"+qOrbitFile).toStdString(), 1, rows, err) && !rows.empty() && rows[0][0]>0)
            d.orbitalPeriod = rows[0][0];
    }
    auto *dlg = new PeriodDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setMainData(d);
    connect(dlg, &PeriodDialog::analyseRequested, this, [this, dlg]() {
        this->setCursor(QCursor(Qt::WaitCursor));
        const bool ok = analyseSolution();
        this->setCursor(QCursor(Qt::ArrowCursor));
        dlg->analysisFinished(ok);
    });
    dlg->show();
}

void MainWindow::on_actionBaryCorr_triggered()
{
    qPath = ui->lineEdit_4->text();
    BaryCorrDialog::MainData d;
    d.work = qPath;
    d.prefix = ui->lineEdit_3->text();
    d.ext = ui->lineEdit_19->text();
    d.first = ui->spinBox->value();
    d.last = ui->spinBox_2->value();
    d.fits = ui->comboBox->currentIndex()==0;
    d.timesFile = ui->lineEdit_18->text();
    d.rvFile = ui->checkBox_2->isChecked() ? ui->lineEdit_2->text() : QString();
    d.rvColumns = ui->checkBox_3->isChecked() ? 1 : 2;
    // files with one line per spectrum (adapted when spectra are rejected)
    if(ui->lineEdit_29->text().trimmed().compare("auto", Qt::CaseInsensitive)!=0) d.perSpectrum << ui->lineEdit_29->text().trimmed();
    d.perSpectrum << ui->lineEdit_bc->text().trimmed() << ui->lineEdit_tellw->text().trimmed();
    if(!ui->checkBox_2->isChecked()) d.perSpectrum << ui->lineEdit_2->text().trimmed();
    const std::string ext = ui->lineEdit_9->text().toStdString(), wc = ui->lineEdit_11->text().toStdString(),
                      ic = ui->lineEdit_12->text().toStdString();
    d.read = [d, ext, wc, ic](int g, std::vector<double> &w, std::vector<double> &f, std::string &err){
        const std::string file = (d.work+"/"+d.prefix+QString::number(g)+d.ext).toStdString();
        w.clear(); f.clear();
        if(d.fits){
            std::valarray<double> wv, fv;
            std::string used;
            if(!readFitsSpectrum(file, ext, wc, ic, wv, fv, used, err)){ err = file+": "+err; return false; }
            w.assign(std::begin(wv), std::end(wv));
            f.assign(std::begin(fv), std::end(fv));
            return true;
        }
        std::vector<std::vector<double>> rows;
        if(!dataio::readTable(file, 2, rows, err)) return false;
        for(const auto &r : rows){ w.push_back(r[0]); f.push_back(r[1]); }
        return true;
    };
    BaryCorrDialog *dlg = new BaryCorrDialog(this);
    dlg->setMainData(d);
    connect(dlg, &BaryCorrDialog::useAirmassFile, this, [this](const QString &file){
        ui->lineEdit_tellw->setText(file);
        ui->plainTextEdit_2->appendPlainText("Airmass as start strength of the static lines: "+file+".");
    });
    connect(dlg, &BaryCorrDialog::useBcFile, this, [this](const QString &file, bool corrected){
        ui->lineEdit_bc->setText(file);
        ui->comboBox_bcframe->setCurrentIndex(corrected ? 1 : 0);
        ui->plainTextEdit_2->appendPlainText("Barycentric correction file in use: "+file+" (spectra "
                                             +(corrected ? "barycentrically corrected" : "in the observatory frame")
                                             +"); takes effect with Read data.");
    });
    connect(dlg, &BaryCorrDialog::spectraRejected, this, [this](int last, const QString &note){
        ui->spinBox_2->setValue(last);
        ui->plainTextEdit_2->appendPlainText(note);
    });
    connect(dlg, &BaryCorrDialog::useInMain, this, [this](const QString &prefix, const QString &fext, const QString &rv){
        ui->lineEdit_3->setText(prefix);
        ui->lineEdit_19->setText(fext);
        ui->comboBox->setCurrentIndex(1);                 // ASCII
        if(!rv.isEmpty()) ui->lineEdit_2->setText(rv);
        // the correction itself, for static lines (shifted by BC in corrected spectra)
        ui->lineEdit_bc->setText(prefix+"barycorr.txt");
        ui->comboBox_bcframe->setCurrentIndex(1);
        ui->plainTextEdit_2->appendPlainText("Barycentrically corrected spectra in use: "+prefix+"<index>"+fext
                                             +(rv.isEmpty() ? QString() : ", start RVs "+rv)+".");
    });
    dlg->show();
}

void MainWindow::on_actionSubtract_triggered()
{
    ui->dockSubtract->show();
    ui->dockSubtract->raise();
}

void MainWindow::Input()
{
    string slfile;
    cout<<"Name of config file:"<<endl;
    if(!(cin>>slfile)){
        cerr<<"No config file given."<<endl;
        return;
    }
    MainWindow::runBatch(QString::fromStdString(slfile));
}

//****************************************
// Batch mode: load configuration and optimise, without a window.
// Returns 0 on success, 1 on error.
//****************************************
int MainWindow::runBatch(const QString &configName)
{
    runow=1;
    error=0;
    // configuration from the current folder (name with or without .sps)
    QString file = configName.endsWith(".sps") ? configName : configName+".sps";
    file = QDir::current().absoluteFilePath(file);
    QString err = loadConfiguration(file);
    if(!err.isEmpty()){
        reportError(err);
        return 1;
    }
    if(batchThreads>0){
        ui->spinBox_8->setValue(std::min(batchThreads, ui->spinBox_8->maximum()));
    }
    cout<<"Cores used: "<<dis::threads()<<endl;
    MainWindow::on_pushButton_4_clicked();      // optimise
    if(optimisationFinished!=1){
        cerr<<"Optimisation did not finish."<<endl;
        return 1;
    }
    cout<<"The END."<<endl;
    return 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}



//************************************************
// function to calculate inclination
//************************************************
double MainWindow::inclinations (){
    double xA1 = (cpoints[0]+cpoints[1])/2;
    double xA2 = (cpoints[4]+cpoints[5])/2;
    double xB1 = (cpoints[2]+cpoints[3])/2;
    double xB2 = (cpoints[6]+cpoints[7])/2;
    double m=0.0;

    QString input="frcompa.dat";
    string data1 = input.toUtf8().constData();
    std::ostringstream dat1NameStream(data1);
    dat1NameStream<<path<<"/"<<data1;
    std::string dat1Name = dat1NameStream.str();
    ifstream inA(dat1Name.c_str());

    int npix=0;
    string line, eins, zwei;
    while(std::getline(inA, line))
       ++npix;

    inA.clear();
    inA.seekg(0, ios::beg);
    int nmean1=0, nmean2=0;
    double meanA1=0.0, meanA2=0.0, meanB1=0.0, meanB2=0.0;
    double x=0.0, y=0.0;

    for(int i =0; i<npix; i++){
        inA>> eins >>zwei;
        istringstream ist(eins);
        ist >> x;
        istringstream ist2(zwei);
        ist2 >> y;
        if((x>=cpoints[0]) && (x<=cpoints[1])){
            meanA1+=y;
            ++nmean1;
        }
        else{
            if((x>=cpoints[4]) && (x<=cpoints[5])){
                meanA2+=y;
                ++nmean2;
            }
            else{
                //
            }
        }
    }
    if(nmean1==0 || nmean2==0 || xA2==xA1){
        cerr<<"inclinations: no pixels in the continuum windows of component A."<<endl;
        return orbit::kInvalidPenalty;
    }
    meanA1=meanA1/nmean1;
    meanA2=meanA2/nmean2;

    m = pow(((meanA2-meanA1)/(xA2-xA1)),2);
    cout<<"mA^2: "<<m<<endl;

    QString input2="frcompb.dat";
    string data2 = input2.toUtf8().constData();
    std::ostringstream dat2NameStream(data2);
    dat2NameStream<<path<<"/"<<data2;
    std::string dat2Name = dat2NameStream.str();
    ifstream inB(dat2Name.c_str());

    npix=0;

    while(std::getline(inB, line))
       ++npix;

    inB.clear();
    inB.seekg(0, ios::beg);
    nmean1=0;
    nmean2=0;

    for(int i =0; i<npix; i++){
        inB>> eins >>zwei;
        istringstream ist(eins);
        ist >> x;
        istringstream ist2(zwei);
        ist2 >> y;
        if((x>=cpoints[2]) && (x<=cpoints[3])){
            meanB1+=y;
            ++nmean1;
        }
        else{
            if((x>=cpoints[6]) && (x<=cpoints[7])){
                meanB2+=y;
                ++nmean2;
            }
            else{
                //
            }
        }
    }
    if(nmean1==0 || nmean2==0 || xB2==xB1){
        cerr<<"inclinations: no pixels in the continuum windows of component B."<<endl;
        return orbit::kInvalidPenalty;
    }
    meanB1=meanB1/nmean1;
    meanB2=meanB2/nmean2;
    cout<<"mean: "<<meanB1<<"; "<<meanB2<<"; "<<nmean1<<"; "<<nmean2<<endl;
    m += pow(((meanB2-meanB1)/(xB2-xB1)),2);
    cout<<"mA^2 + mB^2: "<<m<<endl;
    m = sqrt(m)/2;
    cout<<"m: "<<m<<endl;

    return m;
    }

//********************************************************
//show mouse coordinates
//********************************************************
void MainWindow::showPointToolTip(QMouseEvent *event)
{

    double xc = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    double yc = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(xc).arg(yc));
}

//**********************************************************
//read data
//**********************************************************
//****************************************
// Read a spectrum from a FITS binary table. Looks for the extension extName
// (case-insensitive); if the name is empty or not present, the first table
// extension with both columns. Column names are also case-insensitive.
// usedExt: name of the extension that was read.
//****************************************
static bool readFitsSpectrum(const std::string &file, const std::string &extName, const std::string &wcol,
                             const std::string &icol, std::valarray<double> &w, std::valarray<double> &f,
                             std::string &usedExt, std::string &err)
{
    auto lower = [](std::string x){ for(char &c : x) c = char(std::tolower((unsigned char)c)); return x; };
    try{
        CCfits::FITS::setVerboseMode(false);
        CCfits::FITS in(file, CCfits::Read, true);
        // extensions in file order, requested name first
        std::vector<CCfits::ExtHDU*> exts;
        for(const auto &kv : in.extension()) exts.push_back(kv.second);
        std::sort(exts.begin(), exts.end(), [](CCfits::ExtHDU *a, CCfits::ExtHDU *b){ return a->index() < b->index(); });
        if(!extName.empty()){
            std::stable_partition(exts.begin(), exts.end(),
                                  [&](CCfits::ExtHDU *e){ return lower(e->name())==lower(extName); });
        }
        for(CCfits::ExtHDU *e : exts){
            CCfits::Column *cw = nullptr, *ci = nullptr;
            try{
                cw = &e->column(wcol, false);
                ci = &e->column(icol, false);
            }
            catch(CCfits::FitsException&){
                continue;   // image extension or table without these columns
            }
            cw->read(w, 1, cw->rows());
            ci->read(f, 1, ci->rows());
            usedExt = e->name().empty() ? "HDU "+std::to_string(e->index()) : e->name();
            return true;
        }
        err = "no table extension with columns '"+wcol+"' and '"+icol+"'";
        return false;
    }
    catch(CCfits::FitsException &e){
        err = e.message();
        return false;
    }
}

void MainWindow::read_data(){

    error=0;   // discard stale error status from earlier calls (M3)
    // Edge margin of the residuum: fixed for one optimisation. At a reinitiation the data are
    // read again with the best RVs; a margin recomputed from their (smaller) RV range changed
    // the definition of the residuum, so the restarted run started several chi^2 above the
    // best value and the gain against the previous run was wrong.
    if(inrein!=1) g_edgeMargin = -1;
    g_warmValid = false;
    g_lsqrIterations = g_lsqrSolves = 0;
    ui->plainTextEdit->clear();

    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    // output names of the components (Output tab): without them nothing is written, and the
    // plot afterwards reported the work folder itself as an "empty file"
    {
        QStringList missing;
        if(ui->lineEdit_5->text().trimmed().isEmpty()) missing << "Component A";
        if(!ui->checkBox_3->isChecked() && ui->lineEdit_8->text().trimmed().isEmpty()) missing << "Component B";
        if(ui->checkBox_8->isChecked() && ui->lineEdit_10->text().trimmed().isEmpty()) missing << "Static lines";
        if(!missing.isEmpty()){
            reportError("Error 46: No output file name for "+missing.join(", ")
                        +" (Output tab).");
            return;
        }
    }

    std::string ioErr;
    auto fileIn = [&](const QString &name){ return (qPath+"/"+name).toStdString(); };

    // radial velocities
    if(ui->checkBox_34->isChecked()){
        reportError("Error 25: SB3 systems are not supported.");
        return;
    }
    const bool sb2 = ui->checkBox_4->isChecked();
    const bool sb1 = ui->checkBox_3->isChecked();
    if(!sb1 && !sb2){
        reportError("Error 26: Please choose SB1 or SB2.");
        return;
    }
    mini=ui->spinBox->value();
    maxi=ui->spinBox_2->value();
    num=maxi-mini+1;
    if(num<1){
        reportError("Error 30: The last spectrum must not be smaller than the first one.");
        return;
    }
    RV1.fill(0.0, num);
    RV2.fill(0.0, num);
    RV3.fill(0.0, num);
    const int ncol = sb2 ? 2 : 1;

    // If several boxes are checked, the last source wins as before: orbit, then file, then CroCo.
    if(ui->checkBox_16->isChecked()){
        // from orbital elements
        std::vector<std::vector<double>> t;
        if(!dataio::readTable(fileIn(ui->lineEdit_18->text()), 1, t, ioErr)){
            reportError(QString(sb2 ? "Error 4: " : "Error 10: ")+QString::fromStdString(ioErr));
            return;
        }
        if(int(t.size())!=num){
            reportError(QString(sb2 ? "Error 5" : "Error 11")+": Time file "+qPath+"/"+ui->lineEdit_18->text()
                        +" does not match to the number of observations.");
            return;
        }
        otimes.resize(num);
        for(int i=0; i<num; i++) otimes[i]=t[i][0];

        std::vector<double> el;
        if(!dataio::readNumbers(fileIn(qOrbitFile), el, ioErr) || el.size()<7){
            reportError(QString(sb2 ? "Error 6" : "Error 12")+": File "+qPath+"/"+qOrbitFile
                        +" (orbital elements, Data tab) does not exist or is incomplete! Set the orbital elements via the Editor.");
            return;
        }
        for(int i=0; i<7; i++) orbele[i]=el[i];
        {
            QString bad;
            for(int i=0; i<7; i++) if(!std::isfinite(el[i])) bad = "element "+QString::number(i+1)+" is not a number";
            if(bad.isEmpty() && !(el[0]>0)) bad = "the period must be positive";
            if(bad.isEmpty() && !(el[1]>=0 && el[1]<1)) bad = "the eccentricity must be in [0, 1)";
            if(!bad.isEmpty()){
                reportError(QString(sb2 ? "Error 6" : "Error 12")+": Orbital elements in "+qPath+"/"+qOrbitFile+": "+bad+".");
                return;
            }
        }
        MainWindow::VAmplitudeA();
        if(sb2) MainWindow::VAmplitudeB();
    }
    else if(ui->checkBox_2->isChecked()){
        // from the RV file: one line per spectrum (SB2: RV_A RV_B)
        orbele[4]=0;
        std::vector<std::vector<double>> rows;
        if(!dataio::readTable(fileIn(ui->lineEdit_2->text()), ncol, rows, ioErr)){
            reportError(QString(sb2 ? "Error 2: " : "Error 8: ")+QString::fromStdString(ioErr));
            return;
        }
        if(int(rows.size())!=num){
            reportError(QString(sb2 ? "Error 3" : "Error 9")+": RV file "+qPath+"/"+ui->lineEdit_2->text()
                        +" does not match to the number of observations.");
            return;
        }
        for(int i=0; i<num; i++){
            RV1[i]=rows[i][0];
            if(sb2) RV3[i]=rows[i][1];
        }
    }
    else if(ui->checkBox->isChecked()){
        // from CroCo: one file <name><g>.txt per spectrum
        orbele[4]=0;
        for(int g=mini; g<=maxi; g++){
            QString name = ui->lineEdit->text()+QString::number(g)+".txt";
            std::vector<std::vector<double>> rows;
            if(!dataio::readTable(fileIn(name), ncol, rows, ioErr)){
                reportError(QString(sb2 ? "Error 1: " : "Error 7: ")+QString::fromStdString(ioErr));
                return;
            }
            RV1[g-mini]=rows[0][0];
            if(sb2){
                RV2[g-mini]=rows[0][1];
                RV3[g-mini]=RV1[g-mini]+RV2[g-mini];
                if(ui->checkBox_13->isChecked()){
                    RV1[g-mini]=-RV1[g-mini];
                    RV3[g-mini]=-RV3[g-mini];
                }
            }
        }
    }
    else{
        reportError("Error 14: Please choose input data from file or unknown (via global optimization).");
        return;
    }

    for(int i=0; i<num; i++){
        QString line = QString::number(i)+" "+QString::number(RV1[i]);
        if(sb2) line += " "+QString::number(RV3[i]);
        ui->plainTextEdit->appendPlainText(line);
    }

    //read measurements and write to solution vector

    string outM = "matrixconstr.dat";
    std::ostringstream MNameStream(outM);
    MNameStream<<path<<"/"<<outM;
    std::string MName = MNameStream.str();
    ofstream file1(MName.c_str());

    QString fext=ui->lineEdit_19->text();
    this->setCursor(QCursor(Qt::WaitCursor));

    std::vector<std::vector<double>> waves(num), fluxes(num);
    for(int g=mini; g<=maxi; g++){
        QString name = ui->lineEdit_3->text()+QString::number(g)+fext;
        std::string fname = fileIn(name);

        if(!QFile::exists(QString::fromStdString(fname))){
            reportError(QString(ui->comboBox->currentIndex()==0 ? "Error 15" : "Error 17")
                        +": Spectrum "+qPath+"/"+name+" does not exist!");
            return;
        }
        if(ui->comboBox->currentIndex()==0){
            //input as fits-file
            std::string fitsErr, usedExt;
            if(!readFitsSpectrum(fname, Extension, WCol, ICol, wave, intens, usedExt, fitsErr)){
                reportError("Error 32: Cannot read FITS file "+qPath+"/"+name+": "
                            +QString::fromStdString(fitsErr));
                return;
            }
            if(g==mini && !Extension.empty() && usedExt!=Extension){
                cout<<"FITS: extension '"<<Extension<<"' not found, using '"<<usedExt<<"'."<<endl;
            }
            waves[g-mini].assign(std::begin(wave), std::end(wave));
            fluxes[g-mini].assign(std::begin(intens), std::end(intens));
        }
        else{
            //input as ASCII-file
            std::vector<std::vector<double>> rows;
            if(!dataio::readTable(fname, 2, rows, ioErr)){
                reportError("Error 33: "+QString::fromStdString(ioErr));
                return;
            }
            for(const auto &r : rows){
                waves[g-mini].push_back(r[0]);
                fluxes[g-mini].push_back(r[1]);
            }
        }
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);   // long spectra: window stays responsive
    }

    // Resample onto one common log10(lambda) grid: always with "to log grid", otherwise
    // automatically when the spectra are not on one common equidistant log grid (linear
    // Angstrom, or each spectrum rebinned on its own log grid).
    bool resample = ui->checkBox_42->isChecked();
    QString gridReason;
    if(!resample && !dataio::checkCommonGrid(waves, ioErr)){
        resample = true;
        gridReason = QString::fromStdString(ioErr);
    }
    if(resample){
        const double vstep = ui->doubleSpinBox_18->value();
        const double lstep = vstep>0 ? std::log10(1.0+vstep/c0) : 0.0;
        dataio::GridInfo gi;
        if(!dataio::resampleToLogGrid(waves, fluxes, lstep, gi, ioErr)){
            reportError((gridReason.isEmpty() ? QString("Error 43: Resampling to a log grid failed: ")
                                              : "Error 31: "+gridReason+" Resampling to a common log grid failed: ")
                        +QString::fromStdString(ioErr));
            return;
        }
        const double kms = (std::pow(10.0, gi.step)-1.0)*c0;
        QString msg = QString("Resampled %1 spectra (%2 on a linear scale) to a common log grid: "
                              "%3 pixels, %4-%5 A, %6 km/s per pixel.")
                      .arg(num).arg(gi.linearInputs).arg(gi.n)
                      .arg(std::pow(10.0, gi.w0), 0, 'f', 2)
                      .arg(std::pow(10.0, gi.w0+(gi.n-1)*gi.step), 0, 'f', 2)
                      .arg(kms, 0, 'f', 3);
        if(!gridReason.isEmpty()){
            msg = "The spectra are not on one common log10(lambda) grid ("+gridReason+"). "+msg
                  +" Tick \"to log grid\" to choose the step.";
        }
        cout<<msg.toStdString()<<endl;
        ui->plainTextEdit_2->appendPlainText(msg);
    }
    logbin = int(waves[0].size());
    C.set_size(num*logbin);
    W.set_size(num*logbin);
    for(int a=0; a<num; a++){
        for(int i=0; i<logbin; i++){
            W(a*logbin+i)=waves[a][i];
            C(a*logbin+i)=fluxes[a][i];
        }
    }

    // excluded wavelength ranges (Model tab)
    g_rowMasked.clear();
    g_rowNearMask.clear();
    if(ui->checkBox_mask->isChecked()){
        const QString name = ui->lineEdit_mask->text().trimmed();
        if(name.isEmpty()){
            reportError("Error 47: No file of excluded wavelength ranges given (Model tab).");
            return;
        }
        const QString file = qPath+"/"+name;
        if(!QFile::exists(file)){
            if(name!="telluric_mask.dat"){
                reportError("Error 47: File of excluded wavelength ranges "+file+" does not exist.");
                return;
            }
            // default list (strongest telluric bands, strong chromospheric lines), written for editing
            std::ofstream out(file.toStdString().c_str());
            out<<dataio::kTelluricMask;
            const QString m = "Excluded ranges: default list (strongest telluric bands, strong chromospheric lines) written to "+file+".";
            cout<<m.toStdString()<<endl;
            ui->plainTextEdit_2->appendPlainText(m);
        }
        std::vector<std::pair<double, double>> ranges;
        std::string rerr;
        if(!dataio::readRanges(file.toStdString(), ranges, rerr)){
            reportError("Error 47: "+QString::fromStdString(rerr));
            return;
        }
        std::vector<double> lw(W.begin(), W.end());
        const size_t nm = dataio::markRanges(lw, ranges, g_rowMasked);
        if(nm>=size_t(W.n_elem)){
            reportError("Error 47: The excluded ranges cover all pixels.");
            return;
        }
        std::ostringstream m;
        m<<"Excluded ranges ("<<name.toStdString()<<"): "<<ranges.size()<<" ranges, "
         <<nm<<" of "<<W.n_elem<<" pixels ("<<setprecision(3)<<100.0*double(nm)/double(W.n_elem)<<" %).";
        cout<<m.str()<<endl;
        ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
        if(nm==0) g_rowMasked.clear();
    }

    // align the continua of the spectra to their mean (Data tab, Noise): removes differences of
    // the normalisation between the spectra, which the disentangling turns into broad
    // anti-correlated wiggles of A and B
    if(ui->checkBox_align->isChecked() && num>=2 && logbin>=3){
        std::vector<std::vector<double>> fl(num, std::vector<double>(logbin));
        std::vector<std::vector<char>> mk;
        for(int j=0; j<num; j++) for(int i=0; i<logbin; i++) fl[j][i] = C(j*logbin+i);
        if(!g_rowMasked.empty()){
            mk.assign(num, std::vector<char>(logbin, 0));
            for(int j=0; j<num; j++) for(int i=0; i<logbin; i++) mk[j][i] = g_rowMasked[size_t(j)*logbin+i];
        }
        const double dl = W(1)-W(0);
        const double lamMid = std::pow(10.0, W(logbin/2));
        const int widthPx = std::max(3, int(std::lround(ui->doubleSpinBox_alignWidth->value()/(lamMid*std::log(10.0)*dl))));
        std::vector<double> rms;
        std::string aerr;
        if(cont::alignToMean(fl, mk, widthPx, rms, aerr)){
            for(int j=0; j<num; j++) for(int i=0; i<logbin; i++) C(j*logbin+i) = fl[j][i];
            std::ostringstream m;
            m<<setprecision(3)<<"Continua aligned to the mean spectrum ("<<ui->doubleSpinBox_alignWidth->value()
             <<" A, "<<widthPx<<" px); rms of the correction per spectrum:";
            for(double v : rms) m<<" "<<v;
            cout<<m.str()<<endl;
            double mx = 0.0;
            for(double v : rms) mx = std::max(mx, v);
            std::ostringstream m2;
            m2<<setprecision(3)<<"Continua aligned to the mean spectrum: correction up to "<<100.0*mx<<" % rms (per spectrum in the console).";
            ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m2.str()));
        }
        else cerr<<"Aligning the continua failed: "<<aerr<<endl;
    }

    // wavelength-dependent flux ratio F_A/F_B = k_j s(lambda)
    if(!MainWindow::setupRatioShape()){
        return;
    }

    // S/N per spectrum for weighted least squares
    g_specW.clear();
    g_sigmaBar2 = 0.0;
    g_autoWeightsPending = false;
    if(ui->checkBox_41->isChecked() && ui->lineEdit_29->text().trimmed().compare("auto", Qt::CaseInsensitive)==0){
        if(int(g_autoSpecW.size())==num && g_autoSigmaBar2>0){
            g_specW = g_autoSpecW;           // same weights as before in this optimisation
            g_sigmaBar2 = g_autoSigmaBar2;
        }
        else{
            g_autoWeightsPending = true;     // estimated at the first solve (solveSystem)
        }
    }
    else if(ui->checkBox_41->isChecked()){
        std::vector<double> snr;
        if(!dataio::readNumbers(fileIn(ui->lineEdit_29->text()), snr, ioErr)){
            reportError("Error 40: "+QString::fromStdString(ioErr));
            return;
        }
        if(int(snr.size())!=num){
            reportError("Error 40: The S/N file "+ui->lineEdit_29->text()+" has "+QString::number(snr.size())
                        +" values, but "+QString::number(num)+" spectra are used.");
            return;
        }
        if(!setWeightsFromSnr(snr)){
            reportError("Error 40: All S/N values in "+ui->lineEdit_29->text()+" must be positive.");
            return;
        }
        cout<<"Weighted by S/N per spectrum; mean noise sigma = "<<std::sqrt(g_sigmaBar2)<<endl;
    }

    if(ui->checkBox_33->isChecked()){
        double lo = log10(ui->doubleSpinBox_14->value()), hi = log10(ui->doubleSpinBox_15->value());
        if(lo<W(0) || lo>W(logbin-1) || hi<W(0) || hi>W(logbin-1) || !(lo<hi)){
            reportError("Error 16: Please check the wavelength range for residuum computation.");
            return;
        }
    }

    /*ofstream file4("observation.txt");

    for(int i=0; i<W.size(); i++){
    file4<<setprecision(14)<<W(i)<<" "<<C(i)<<endl;
    }*/

    g_dlog = (W(logbin-1)-W(0))/(logbin-1);

    if(!MainWindow::loadComponentWeights()){
        return;
    }
    if(!MainWindow::loadBaryCorr()){
        return;
    }
    if(!MainWindow::loadStaticRanges()){
        return;
    }
    if(g_tellGroupNames.size()>1)        // start strengths: the same for every group
        g_MtelG.assign(g_tellGroupNames.size(), std::vector<double>(Mtel.begin(), Mtel.begin()+num));
    g_tellFitPending = ui->checkBox_8->isChecked() && ui->checkBox_tellfit->isChecked();
    if(g_tellFitPending && !ui->checkBox_mask->isChecked()){
        const QString m = "Note: strength of the static lines fitted per spectrum without excluded ranges: saturated "
                          "telluric bands do not scale linearly; exclude them (Model tab).";
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    }
    if(!MainWindow::buildDesignMatrix(true)){
        return;
    }
    if(!buildResidualExclusion()){
        return;
    }

    file1<<"# Spectrum\tRV_A[km/s]\tRV_B[km/s]\tShift_A[px]\tShift_B[px]"<<endl;
    for(int j=0; j<num; j++){
        bool sb2 = bso2>0;
        file1<<j<<"\t"<<RV1[j]<<"\t"<<(sb2 ? RV3[j] : 0.0)<<"\t"<<dis::pixelShift(RV1[j], g_dlog)
             <<"\t"<<(sb2 ? dis::pixelShift(RV3[j], g_dlog) : 0.0)<<endl;
    }
    file1<<"# M: "<<Mn<<" x "<<Mm<<"; A: "<<g_layout.nA<<" columns from rest pixel "<<g_layout.x0A
         <<"; B: "<<g_layout.nB<<" from "<<g_layout.x0B<<"; static: "<<g_layout.nT<<endl;

    cout<<"bso1 "<<bso1<<endl;
    cout<<"bso2 "<<bso2<<endl;
    cout<<Mm<<" "<<Mn<<endl;

    qApp->processEvents(QEventLoop::AllEvents);

    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_16->setChecked(false);
}

void MainWindow::on_checkBox_2_clicked()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_16->setChecked(false);
}


//**********************************************************
//read data button
//**********************************************************
void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::resetRunState();   // S/N "auto": estimate anew
    MainWindow::read_data();
}

//**********************************************************
// Solve: solve once with the RVs read in and write the output.
// Solver as selected: LSQR (checkBox_5) or SVD with divide and conquer (checkBox_6).
//**********************************************************
void MainWindow::solveOnce()
{
    if(ui->spinBox_2->value()-ui->spinBox->value()<1){
        reportError("Error 19: At least two observations at (hopefully) different phases are necessary.");
        error=0;
        return;
    }

    abortt=0;
    g_abortableRun = true;          // the window takes input during the solve: Stop ends it
    MainWindow::disableButtons();
    MainWindow::resetRunState();   // S/N "auto": estimate anew
    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        g_abortableRun = false;
        MainWindow::enableButtons();
        return;
    }
    this->setCursor(QCursor(Qt::WaitCursor));

    auto time1 = std::chrono::steady_clock::now();

    const bool solved = MainWindow::solveSystem(MainWindow::currentSolver());
    g_abortableRun = false;
    if(!solved){
        this->setCursor(QCursor(Qt::ArrowCursor));
        if(abortt==1){
            abortt = 0;
            const char *m = "Solve stopped.";
            cout<<m<<endl;
            ui->plainTextEdit_2->appendPlainText(m);
        }
        else reportError("Error 29: Solving the linear system failed.");
        error=0;
        MainWindow::enableButtons();
        return;
    }
    vec res = M*X;
    int resBins = 0;
    residu = MainWindow::computeResiduum(res, &resBins);
    MainWindow::writeSolution(res, ui->lineEdit_5->text(), ui->lineEdit_8->text(), ui->lineEdit_10->text(),
                              ui->lineEdit_13->text(), ui->lineEdit_20->text(), "compl.txt");
    MainWindow::writeSnrEstimate(res, resBins);

    cout<<"residuum: "<<residu<<endl;

    double minutes = std::chrono::duration<double>(std::chrono::steady_clock::now()-time1).count()/60.0;
    ui->lineEdit_6->setText(QString::number(minutes));
    ui->lineEdit_7->setText(QString::number(residu));

    MainWindow::on_pushButton_6_clicked();
    MainWindow::enableButtons();
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_pushButton_3_clicked()
{
    MainWindow::solveOnce();
}

void MainWindow::on_actionSpectrum_Plotter_triggered()
{
    pSpec =new PlotSpec(this);
    pSpec->seData(ui->lineEdit_4->text(), ui->lineEdit_5->text(), ui->lineEdit_8->text(), ui->lineEdit_20->text());
    // flux-ratio model for the continuum correction of the components
    fr::Model m;
    std::string err;
    if(!ratioModelFromUi(m, err)){
        ui->plainTextEdit_2->appendPlainText("Spectrum Plotter: "+QString::fromStdString(err)+" Constant ratio used.");
        m = fr::Model();
    }
    if(!(m.lambdaRef>0) && g_ratioLref>0) m.lambdaRef = g_ratioLref;   // "auto" as resolved when reading
    pSpec->setRatioModel(m, ui->doubleSpinBox_4->value());
    pSpec->on_pushButton_3_clicked();
    pSpec->on_pushButton_2_clicked();
    pSpec->show();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "This open-source software was developed at Leibniz-Institute for Astrophysics Potsdam (Germany) by\n\n "
                                            "Daniel P. Sablowski\n\n"
                                            "Version 2.0 (2026)\n\n"
                                            "It makes use of the 'Armadillo' C++ linear algebra library, OpenBLAS, CCfits and libraries therein.\n"
                                            "It uses the spline.h from https://kluge.in-chemnitz.de/opensource/spline/ \n"
                                            "It is provided AS IS WITHOUT WARRANTY of ANY KIND.\n\n"
                                            "Licensed under the Apache 2.0 licence conditions.");
}

void MainWindow::on_actionBug_Report_triggered()
{
    QMessageBox::information(this, "Bug Report", "Please contact: dsablowski@aip.de");
}

//**********************************************************
// Solver: LSQR or SVD (DaC), exactly one option active
//**********************************************************
void MainWindow::on_checkBox_5_clicked()
{
    ui->checkBox_5->setChecked(true);
    ui->checkBox_6->setChecked(false);
}

void MainWindow::on_checkBox_6_clicked()
{
    ui->checkBox_6->setChecked(true);
    ui->checkBox_5->setChecked(false);
}

dis::Solver MainWindow::currentSolver() const
{
    return ui->checkBox_6->isChecked() ? dis::Solver::SvdDc : dis::Solver::Lsqr;
}

void MainWindow::on_checkBox_7_clicked()
{
   if(ui->checkBox_7->isChecked()){
       bidi=1;
   }
   else{
       bidi=0;
   }
}

void MainWindow::on_lineEdit_4_editingFinished()
{
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();
}

void MainWindow::on_actionCorrelation_triggered()
{
    qCorr =new correlation(this);
    qCorr->seData(ui->lineEdit_4->text(), ui->lineEdit_5->text());
    qCorr->show();
}

void MainWindow::on_lineEdit_9_textChanged()
{
    qExtension=ui->lineEdit_9->text();
    Extension=qExtension.toUtf8().constData();
}

void MainWindow::on_lineEdit_11_textChanged()
{
    qWCol=ui->lineEdit_11->text();
    WCol=qWCol.toUtf8().constData();
}

void MainWindow::on_lineEdit_12_textChanged()
{
    qICol=ui->lineEdit_12->text();
    ICol=qICol.toUtf8().constData();
}

void MainWindow::on_comboBox_currentIndexChanged()
{
    if(ui->comboBox->currentIndex()==0){
        ui->lineEdit_9->setEnabled(true);
        ui->lineEdit_11->setEnabled(true);
        ui->lineEdit_12->setEnabled(true);
        ui->lineEdit_19->setText(".fits");
    }
    else if(ui->comboBox->currentIndex()==1){
        ui->lineEdit_9->setEnabled(false);
        ui->lineEdit_11->setEnabled(false);
        ui->lineEdit_12->setEnabled(false);
        ui->lineEdit_19->setText(".txt");
    }
}

//******************************************************
//optimization DSM
//******************************************************
// Steps of the new simplex when the optimisation is reinitiated ("Reinitiate after
// convergence"). The steps of the very first simplex (initialstep.dat or the defaults, e.g.
// P/10 and T/10, or 2*step pixels per RV) are meant to search far from a rough start; around a
// converged point they put the vertices many sigma away, the residuum of the first
// evaluations was orders of magnitude above the best value and the restart spent its
// iterations shrinking back. Here each step is adjusted (errest::probeSimplex) so that chi^2
// rises by about kReinitChi2 per vertex, i.e. about 2 standard deviations per parameter.
// h0: steps of the first construction (start values and sign); residuum(x): residuum at x
// (sets the parameters). Falls back to h0 per parameter where no step can be determined.
constexpr double kReinitChi2 = 4.0;

arma::vec reinitSteps(const arma::rowvec &x0, const arma::vec &h0,
                      const std::function<double(const arma::rowvec &)> &residuum,
                      std::ofstream &log)
{
    arma::vec h = h0;
    const double r0 = residuum(x0);
    if(!std::isfinite(r0) || r0>=orbit::kInvalidPenalty) return h;
    // sigma^2 of the last convergence test; without one (Continue in a new session) estimated
    // from the residuum per degree of freedom
    double s2 = g_sigma2;
    if(!(s2>0)){
        const int dof = g_lastResBins - g_lastResCols - int(x0.n_elem);
        if(dof>0) s2 = r0*r0/dof;
    }
    if(!(s2>0)) return h;
    auto S = [&](const arma::rowvec &x){ const double r = residuum(x); return r*r; };
    errest::ProbeResult pr = errest::probeSimplex(x0, r0*r0, h0, kReinitChi2*s2, S,
                                                  []{ return abortt==1; });
    int kept = 0;
    for(arma::uword i=0; i<h.n_elem; i++){
        if(i<pr.steps.n_elem && std::isfinite(pr.steps(i)) && pr.steps(i)!=0.0) h(i) = pr.steps(i);
        else ++kept;
    }
    log<<"Reinitiation: new simplex around the best point, steps adjusted so that chi^2 rises by about "
       <<kReinitChi2<<" per vertex ("<<pr.evaluations+1<<" evaluations"
       <<(kept>0 ? ", "+std::to_string(kept)+" steps kept from the first construction" : std::string())
       <<")."<<endl;
    return h;
}

// gamma free in the orbit mode: not determined by the disentangling
static const char *kGammaNote = "Note: gamma is free, but the disentangling does not determine it: a common shift "
    "of the RVs is absorbed by the shift of the component spectra (also with static lines). It stays near its start "
    "value and its error is meaningless; lock gamma (Optimisation tab) and take it from elsewhere.";

// Inserts a suffix before the extension: beststate.dat + _rv -> beststate_rv.dat
static QString withSuffix(const QString &name, const QString &suffix)
{
    const int dot = name.lastIndexOf('.');
    if(dot>0 && dot>name.lastIndexOf('/')+1) return name.left(dot)+suffix+name.mid(dot);
    return name+suffix;
}

// Best state (see g_trackBest): file name, write and load. One name in the Output tab, the
// program adds _rv (RVs of all spectra) or _orbit (orbital elements).
static QString bestStateFile(bool rvMode)
{
    return qPath+"/"+withSuffix(qBestState, rvMode ? "_rv" : "_orbit");
}

static void writeBestState(bool rvMode, double residuum)
{
    std::ofstream o(bestStateFile(rvMode).toStdString().c_str());
    if(!o) return;
    o<<setprecision(15);
    if(rvMode){
        o<<"# Spectangular best state (RVs A B per spectrum) residuum "<<residuum
         <<" spectra "<<mini<<" "<<maxi<<"\n";
        for(int j=0; j<num; j++) o<<RV1[j]<<"\t"<<RV3[j]<<"\n";
    }
    else{
        o<<"# Spectangular best state (P e K_A K_B gamma T_peri omega) residuum "<<residuum<<"\n";
        for(int e=0; e<7; e++) o<<orbele[e]<<"\n";
    }
}

// Loads the best state into RV1/RV3 or orbele. false (nothing changed) if there is none or it
// belongs to another set of spectra. residuum: as stored (with the settings of that run).
static bool loadBestState(bool rvMode, double &residuum)
{
    std::ifstream in(bestStateFile(rvMode).toStdString().c_str());
    if(!in) return false;
    std::string head;
    if(!std::getline(in, head) || head.rfind("# Spectangular best state", 0)!=0) return false;
    // the kind must match (the same name could have been given to both)
    if(head.find(rvMode ? "(RVs " : "(P e ")==std::string::npos) return false;
    residuum = std::numeric_limits<double>::quiet_NaN();
    {
        const size_t k = head.find("residuum ");
        if(k!=std::string::npos) residuum = std::atof(head.c_str()+k+9);
    }
    if(rvMode){
        const size_t k = head.find("spectra ");
        if(k==std::string::npos) return false;
        int a=0, b=0;
        std::istringstream hs(head.substr(k+8));
        if(!(hs>>a>>b) || a!=mini || b!=maxi) return false;
        std::vector<double> va(num), vb(num);
        for(int j=0; j<num; j++) if(!(in>>va[j]>>vb[j]) || !std::isfinite(va[j]) || !std::isfinite(vb[j])) return false;
        for(int j=0; j<num; j++){ RV1[j]=va[j]; RV3[j]=vb[j]; }
        return true;
    }
    double el[7];
    for(int e=0; e<7; e++) if(!(in>>el[e]) || !std::isfinite(el[e])) return false;
    if(!orbit::elementsValid(el[0], el[1])) return false;
    for(int e=0; e<7; e++) orbele[e]=el[e];
    return true;
}

// Stored simplex for "Continue": nu+1 residua (optval) and (nu+1)*nu values (optmat).
// false if a file is missing or does not match nu.
static bool readStoredSimplex(int nu, std::vector<double> &y, std::vector<double> &P)
{
    std::vector<double> a, b;
    std::string err;
    if(!dataio::readNumbers((qPath+"/"+qOptval).toStdString(), a, err)) return false;
    if(!dataio::readNumbers((qPath+"/"+qOptmat).toStdString(), b, err)) return false;
    if(int(a.size())!=nu+1 || int(b.size())!=(nu+1)*nu) return false;
    for(double v : a) if(!std::isfinite(v)) return false;
    for(double v : b) if(!std::isfinite(v)) return false;
    y = a; P = b;
    return true;
}

// Continue with Nelder-Mead: the stored simplex is used if it matches the free parameters and
// its best vertex is the best state (cur: free parameters of the best state, loaded before).
// Otherwise (simplex missing, other parameters free, or the best was found later, e.g. by
// Levenberg-Marquardt) a new simplex is built around the current best values. Returns the
// reason for the new simplex, empty if the stored one is used.
static std::string continueNeedsNewSimplex(int nu, const std::vector<double> &cur)
{
    std::vector<double> y, P;
    if(!readStoredSimplex(nu, y, P))
        return "the stored simplex ("+qOptval.toStdString()+", "+qOptmat.toStdString()
               +") is missing or does not match the free parameters";
    if(!g_contFromBest) return "";
    const int b = int(std::min_element(y.begin(), y.end())-y.begin());
    for(int j=0; j<nu; j++){
        if(std::fabs(P[b*nu+j]-cur[j]) > 1e-8*std::max(1.0, std::fabs(cur[j])))
            return "the best state differs from the best vertex of the stored simplex (found later, e.g. by Levenberg-Marquardt)";
    }
    return "";
}

// Called by DivideConquer for every evaluation during an optimisation.
static void trackBest(double r)
{
    if(g_trackBest==0 || optfratios!=0 || !std::isfinite(r) || r>=orbit::kInvalidPenalty) return;
    if(std::isfinite(g_bestStateS) && !(r<g_bestStateS)) return;
    g_bestStateS = r;
    writeBestState(g_trackBest==1, r);
}

void MainWindow::Optimisation()
{
    system_clock::time_point time1 = system_clock::now();

    upda=0;
    updac=0;
    abauto=0;
    optimisationFinished=0;

    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);

    this->setCursor(QCursor(Qt::WaitCursor));
    MainWindow::read_data();
    this->setCursor(QCursor(Qt::WaitCursor));

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;   // error stays set (batch mode: exit code 1)
    }

    qApp->processEvents(QEventLoop::AllEvents);

    int Ph=0, Pl=0, Psh=0, SB1=0;

    eval=0;

    if(ui->checkBox_3->isChecked()){
            SB1=1;
    }

    zaehler=ui->spinBox_3->value();
    double ym=0, ys=0;

    int nu=2*num;
    if(SB1==1){
        nu=num;
    }

    QString qLogFile = ui->lineEdit_21->text();
    QString qlFile = qPath+"/"+qLogFile;
    QFileInfo QLFile(qlFile);
    string lFile = qLogFile.toUtf8().constData();
    std::ostringstream lFileNameStream(lFile);
    lFileNameStream<<path<<"/"<<lFile;
    std::string lFileName = lFileNameStream.str();
    ofstream LogFile;

    struct tm *ts;
    time_t t;

    t = time(NULL);
    ts = localtime(&t);

    if(QLFile.exists()){
        LogFile.open(lFileName.c_str(), std::ios_base::app);
        LogFile<<endl;
        LogFile<<"__________________________________________________"<<endl;
        LogFile<<" "<<asctime(ts)<<endl;
        LogFile<<endl;
    }

    else{
        LogFile.open(lFileName.c_str());
        LogFile<<"******************************"<<endl;
        LogFile<<"  Optimisation LogFile"<<endl;
        LogFile<<"******************************"<<endl;
    }

    if(ui->checkBox_33->isChecked()){
        LogFile<<"Compute Residuum from "<<ui->doubleSpinBox_14->value()<<" to "<<ui->doubleSpinBox_15->value()<<"."<<endl;
    }
    else{
        LogFile<<"Compute Residuum in full range."<<endl;
    }
    if(!g_rowResExcl.empty()){
        LogFile<<"Residuum without the ranges of the static lines ("<<ui->lineEdit_tellrange->text().toStdString()<<")."<<endl;
    }
    LogFile<<endl;

    // weights (light fractions) are set by read_data() via loadComponentWeights()
    if(QFileInfo::exists(qPath+"/edit.dat")){
        LogFile<<"Relative component strength and telluric line strength set via Editor."<<endl;
    }
    else{
        LogFile<<"No relative values set via Editor; flux ratio A/B = "<<ui->doubleSpinBox_4->value()<<endl;
    }

    LogFile<<endl;

    string scoef = "optcoef.dat";
    std::ostringstream coeNameStream(scoef);
    coeNameStream<<path<<"/"<<scoef;
    std::string coeName = coeNameStream.str();

    QFileInfo optcoef(coeName.c_str());

    if(optcoef.exists()){

        LogFile<<"User defined transformation coefficients used."<<endl;

        double optv[4];
        ifstream optc(coeName.c_str());
        string einse;

        for(int i=0; i<4; i++){
            optc >> einse;
            istringstream istr8(einse);
            istr8 >> optv[i];
        }
        alpha=optv[0];
        nmBeta=optv[1];
        btot=optv[2];
        Gamma=optv[3];
    }

    else{
        LogFile<<"Default values for transformation coefficients used."<<endl;
        Gamma=2.0;
        alpha=1.0;
        nmBeta=0.5;
        btot=0.5;
    }

    LogFile<<"Alpha: "<<alpha<<"; Beta: "<<nmBeta<<"; Beta_tot: "<<btot<<"; Gamma: "<<Gamma<<"; Step: "<<step<<endl;


    LogFile<<endl;

    string sinit = "initialstep.dat";
    std::ostringstream iniNameStream(sinit);
    iniNameStream<<path<<"/"<<sinit;
    std::string iniName = iniNameStream.str();

    QFileInfo initstep(iniName.c_str());

    if(initstep.exists()){

        LogFile<<"User defined initial step sizes used."<<endl;

        ifstream initStep(iniName.c_str());
        double IntS[8];
        string einse;

        for(int i =0; i<8; i++){
            initStep >> einse;
            istringstream istr9(einse);
            istr9 >> IntS[i];
        }
        step = IntS[0];
        dP = IntS[1];
        de = IntS[2];
        dKA = IntS[3];
        dKB = IntS[4];
        dGamma = IntS[5];
        dT0 = IntS[6];
        dOmega = IntS[7];

    }

    else{
        LogFile<<"Default initial step size used."<<endl;

        step=3.0;
        dP = orbele[0]/10;
        de = (orbele[1]>0.05 ? orbele[1]/10 : 0.005);   // otherwise degenerate simplex at e=0
        dKA = 3*dv;
        dKB = 3*dv;
        dGamma = 3*dv;
        dT0 = orbele[0]/10;
        dOmega = M_PI/20;
    }
    dorbele[0]=dP;
    dorbele[1]=de;
    dorbele[2]=dKA;
    dorbele[3]=dKB;
    dorbele[4]=dGamma;
    dorbele[5]=dT0;
    dorbele[6]=dOmega;

    double ym2=0, ys2=0;
    int stagnate=0;

    string sopt = qOptdata.toStdString();
    std::ostringstream soptNameStream(sopt);
    soptNameStream<<path<<"/"<<sopt;
    std::string soptName = soptNameStream.str();
    ofstream soptd(soptName.c_str());
    soptd<<setprecision(15);   // full precision (plot in Delta-chi^2)
    LogFile<<setprecision(15);

    LogFile<<"Step: "<<step<<"; dP: "<<dP<<"; de: "<<de<<"; dKA: "<<dKA<<"; dKB: "<<dKB<<"; dGamma: "<<dGamma<<"; dT0: "<<dT0<<"; dOmega: "<<dOmega<<endl;
    LogFile<<endl;

    // Continue and reinitiation start from the best state of the previous optimisation (either
    // method, see g_trackBest), independent of the start values in the inputs.
    const bool rvModeOpt = ui->checkBox->isChecked() || ui->checkBox_2->isChecked();
    g_contFromBest = false;
    if(inrein==1 || ui->checkBox_11->isChecked()){
        double sOld = std::numeric_limits<double>::quiet_NaN();
        g_contFromBest = loadBestState(rvModeOpt, sOld);
        if(g_contFromBest){
            if(!rvModeOpt){            // RVs of the best orbit
                RVCalcCheck = 0;
                MainWindow::VAmplitudeA();
                MainWindow::VAmplitudeB();
            }
            // "Exclude edges": margin from the RV span of the best values, not of the inputs
            // (otherwise the residuum would depend on the start values in the inputs)
            if(inrein==0) g_edgeMargin = -1;
            LogFile<<"Start values from the best state of the previous optimisation ("
                   <<QFileInfo(bestStateFile(rvModeOpt)).fileName().toStdString()<<", residuum "<<sOld<<")."<<endl;
        }
        else if(ui->checkBox_11->isChecked()){
            LogFile<<"No best state of a previous optimisation for these spectra ("
                   <<QFileInfo(bestStateFile(rvModeOpt)).fileName().toStdString()<<"); start values from the simplex or the inputs."<<endl;
        }
    }
    g_trackBest = rvModeOpt ? 1 : 2;
    g_bestStateS = std::numeric_limits<double>::quiet_NaN();

    // Levenberg-Marquardt (alternative to Nelder-Mead) for both parameter sets
    if(ui->comboBox_method->currentIndex()==1){
        MainWindow::optimiseLM(LogFile, soptd);
        LogFile.close();
    }
    // optimisation on individual RVs
    else if(ui->checkBox->isChecked() or ui->checkBox_2->isChecked()){

        LogFile<<"Optimisation on individual RVs."<<endl;

        // free RVs (full index: A 0..num-1, B num..2num-1); fixed ones keep their start value
        std::vector<int> rvIdx;
        {
            QString ferr;
            if(!MainWindow::freeRVIndices(SB1==1, rvIdx, ferr)){
                reportError("Error 45: "+ferr);
                LogFile.close();
                return;
            }
        }
        nu = int(rvIdx.size());
        if(nu < (SB1==1 ? num : 2*num)){
            LogFile<<"Fixed RVs (spectrum: A / B):";
            for(int j=0; j<num; j++){
                const bool fa = std::find(rvIdx.begin(), rvIdx.end(), j)==rvIdx.end();
                const bool fb = SB1==0 && std::find(rvIdx.begin(), rvIdx.end(), num+j)==rvIdx.end();
                if(fa || fb) LogFile<<" "<<(j+mini)<<(fa ? "A" : "")<<(fb ? "B" : "");
            }
            LogFile<<"; "<<nu<<" free."<<endl;
        }
        auto setRV = [&](int k, double v){ if(k<num) RV1[k]=v; else RV3[k-num]=v; };
        auto getRV = [&](int k){ return k<num ? RV1[k] : RV3[k-num]; };
        auto applyRow = [&](const arma::rowvec &x){ for(int k=0; k<nu; k++) setRV(rvIdx[k], x(k)); };
        auto logRVs = [&](std::ostream &o){
            for(int j=0; j<num; j++){
                o<<RV1[j];
                if(SB1==0) o<<"\t"<<RV3[j];
                o<<endl;
            }
        };

        LogFile<<endl;

        std::vector<double> y(nu+1), Z(nu), Co(nu), So(nu), Eo(nu);
        mat P(nu+1,nu), e(nu+1,nu);

    // Continue: stored simplex or a new one around the best values; reinitiation: new one
    qOptval = ui->lineEdit_16->text();
    qOptmat = ui->lineEdit_17->text();
    bool contRebuild = false;
    if(inrein==0 && !ui->checkBox_9->isChecked() && ui->checkBox_11->isChecked()){
        std::vector<double> cur(nu);
        for(int j=0; j<nu; j++) cur[j]=getRV(rvIdx[j]);
        const std::string why = continueNeedsNewSimplex(nu, cur);
        if(!why.empty()){
            contRebuild = true;
            const std::string m = "Continue: new simplex around the current best RVs because "+why+".";
            LogFile<<m<<endl;
            cout<<m<<endl;
            ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m));
        }
    }
    const bool rebuild = inrein==1 || contRebuild;

    //create new initial data
    if(ui->checkBox_9->isChecked() || rebuild){
        r1=0.0;

        LogFile<<"Creating new initial simplex with "<<nu+1<<" points."<<endl;

        qInitval = ui->lineEdit_14->text();
        string file1init = qInitval.toUtf8().constData();
        std::ostringstream file1NameStream(file1init);
        file1NameStream<<path<<"/"<<file1init;
        std::string file1Name = file1NameStream.str();

        qInitmat = ui->lineEdit_15->text();
        string file2init = qInitmat.toUtf8().constData();
        std::ostringstream file2NameStream(file2init);
        file2NameStream<<path<<"/"<<file2init;
        std::string file2Name = file2NameStream.str();
        if(rebuild){   // new simplex around the best values: written as the optimisation files
            file1Name = (qPath+"/"+qOptval).toStdString();
            file2Name = (qPath+"/"+qOptmat).toStdString();
        }

        if(ui->checkBox_9->isChecked() && !rebuild){
            QFile QIn(file1Name.c_str());
            QFile QIn2(file2Name.c_str());

            if((QIn.exists() or QIn2.exists())){
                if(runow==0){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Warning!", "The optimisation files already exists. \n\n Do you want to overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                    qDebug() << "Overwrite initial data.";
                    }
                    else{
                        qDebug()<< "Initiation aborted.";
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        MainWindow::enableButtons();
                        return;
                    }
                }
                else{
                    cout<<"Warning: The optimisation files already exists. Do you want to overwrite it? 0 no, yes 1: "<<endl;
                    if(batchOverwrite==1) overw=1;
                    else if(!(cin >> overw)) overw=0;
                    if(overw==0) cerr<<"Existing optimisation files are not overwritten (use --overwrite)."<<endl;
                    if(overw==0){
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        MainWindow::enableButtons();
                        return;
                    }
                    else{
                        qDebug() << "Overwrite initial data.";
                    }
                }
            }
            else{
                cout<<"Create new initial data."<<endl;
            }

        }
        else{
            cout<<"Create new initial data."<<endl;
            inrein=0;
        }
        
        ofstream init(file1Name.c_str());
        ofstream init2(file2Name.c_str());


    for(int i=0; i<nu; i++) P(0,i)=getRV(rvIdx[i]);

    for(int i=0; i<nu+1; i++){
        for(int j=0; j<nu; j++){

            if((i>0)&(i==j+1)){
                e(i,j)=2*step;
            }
            else{
                e(i,j)=0;
            }
        }
    }

    if(rebuild){
        // reinitiation: steps on the scale of the uncertainty instead of the first construction
        arma::vec h0(nu);
        for(int j=0; j<nu; j++) h0(j) = dv*e(j+1,j);
        auto setRVs = [&](const arma::rowvec &x){
            applyRow(x);
            eval++;
            ui->spinBox_5->setValue(eval);
            qApp->processEvents(QEventLoop::AllEvents);
            MainWindow::ConstructMatrix();
            return MainWindow::DivideConquer();
        };
        const arma::vec h = reinitSteps(P.row(0), h0, setRVs, LogFile);
        for(int j=0; j<nu; j++) e(j+1,j) = h(j)/dv;
        upda = 0;
    }

    for (int i=0; i<nu+1; i++){
        for (int j=0; j<nu; j++){

            P(i,j)=P(0,j)+dv*e(i,j);
            init2<<setprecision(14)<<P(i,j)<<endl;
            setRV(rvIdx[j], P(i,j));
        }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[i]=MainWindow::DivideConquer();
        soptd<<"0\t"<<eval<<"\t"<<y[i]<<"\t"<<sigma2Now(y[i])<<endl;
        qApp->processEvents(QEventLoop::AllEvents);
        if(upda==1){
            upda=0;
            abauto += 1;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<y[i]<<endl;
            MainWindow::plot_functionvalues();
            logRVs(LogFile);
            LogFile<<endl;
        }
        else {
            //
        }

        if(abortt==1){
            LogFile<<"Initiation was aborted."<<endl;
            abortt=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            MainWindow::enableButtons();
            return;
        }
        if(i==0){
            r1 = y[i];
            MainWindow::on_pushButton_6_clicked();
        }
        else if(y[i] < r1){
            r1=y[i];
            if(ui->checkBox_10->isChecked()){
                MainWindow::on_pushButton_6_clicked();
                MainWindow::enableButtons();
            }
            else{
                //
            }
        }

        cout<<y[i]<<endl;
        init<<setprecision(12)<<y[i]<<endl;
        LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;
    }
    init.close();
    init2.close();

    LogFile<<"Initial simplex completed."<<endl;

    }

    //continue with optimization data
    else if(ui->checkBox_11->isChecked()){

        string zeile, one;

        LogFile<<"Continue with optimisation; loading data."<<endl;


        qOptval = ui->lineEdit_16->text();
        string file1opt = qOptval.toUtf8().constData();
        std::ostringstream file1NameStream(file1opt);
        file1NameStream<<path<<"/"<<file1opt;
        std::string file1Name = file1NameStream.str();
        ifstream initiate1(file1Name.c_str());

        qOptmat = ui->lineEdit_17->text();
        string file2opt = qOptmat.toUtf8().constData();
        std::ostringstream file2NameStream(file2opt);
        file2NameStream<<path<<"/"<<file2opt;
        std::string file2Name = file2NameStream.str();
        ifstream initiate2(file2Name.c_str());

        QFile checkfile(file2Name.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The optimisation file "<<checkfile.fileName()<<" does not exist.";
            if(runow==0){
            QMessageBox::information(this, "Error", "Error 23: The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
            }
            else{
                cout<<"Error 23: The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }

        int number_of_lines =0;

        while(std::getline(initiate1, zeile)){
           ++number_of_lines;
           dis::keepAlive();
       }

        initiate1.clear();
        initiate1.seekg(0, ios::beg);

        if(number_of_lines!=nu+1){
            qDebug()<<"Wrong optimisation file.";
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 24: The optimisation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
            }
            else{
                cout<<"Error 24: The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The optimisation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }

        for (int i=0; i<number_of_lines; i++){
            initiate1 >> one;
            istringstream ist(one);
            ist >> y[i];
            if(i==0) r1=y[0];
            else if(y[i]<r1){
                r1=y[i];
            }
            }
        ui->lineEdit_7->setText(QString::number(r1));
        initiate1.close();
        g_bestStateS = r1;   // best state: only real improvements on the stored simplex

        int coun=0;

        for (int i=0; i<number_of_lines*nu; i++){
            initiate2 >> one;
            istringstream ist(one);
            ist >> P(coun,i-coun*nu);
            cout<<"P["<<coun<<"]["<<i-coun*nu<<"]: "<<P(coun,i-coun*nu)<<" ";
            if(i==coun*nu+nu-1){
                cout<<endl;
                ++coun;
            }
        }
        initiate2.close();

        LogFile<<"Data from previous optimisation successfully loaded."<<endl;

    }

    //use initial data.
    else if(ui->checkBox_12->isChecked()){

        LogFile<<"Use available initial data."<<endl;

        string zeile, one;

        qInitval = ui->lineEdit_14->text();
        string file1init = qInitval.toUtf8().constData();
        std::ostringstream file1NameStream(file1init);
        file1NameStream<<path<<"/"<<file1init;
        std::string file1Name = file1NameStream.str();
        ifstream initiate1(file1Name.c_str());

        qInitmat = ui->lineEdit_15->text();
        string file2init = qInitmat.toUtf8().constData();
        std::ostringstream file2NameStream(file2init);
        file2NameStream<<path<<"/"<<file2init;
        std::string file2Name = file2NameStream.str();
        ifstream initiate2(file2Name.c_str());

        QFile checkfile(file2Name.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The initiation file "<<checkfile.fileName()<<" does not exist.";
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 25: The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
            }
            else{
                cout<<"Error 25: The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The initiation file does not exist! When running optimization the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }

        int number_of_lines =0;

        while(std::getline(initiate1, zeile)){
           ++number_of_lines;
           dis::keepAlive();
       }

        initiate1.clear();
        initiate1.seekg(0, ios::beg);

        if(number_of_lines!=nu+1){
            qDebug()<<"Wrong initiate file.";
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 26: The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimisation needs to be initiated first.");
            }
            else{
                cout<<"Error 26: The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }
        else{
            // do nothing
        }

        for (int i=0; i<number_of_lines; i++){
            initiate1 >> one;
            istringstream ist(one);
            ist >> y[i];
            if(i==0) r1=y[0];
                else if(y[i]<r1){
                    r1=y[i];
                }
            }
            initiate1.close();
            ui->lineEdit_7->setText(QString::number(r1));

        int coun=0;

        for (int i=0; i<number_of_lines*nu; i++){
            initiate2 >> one;
            istringstream ist(one);
            ist >> P(coun,i-coun*nu);
            cout<<"P["<<coun<<"]["<<i-coun*nu<<"]: "<<P(coun,i-coun*nu)<<" ";
            if(i==coun*nu+nu-1){
                cout<<endl;
                ++coun;
            }
            else{
                //
            }
        }
        initiate2.close();

        LogFile<<"Initial data successfully loaded."<<endl;
    }


    qOptval = ui->lineEdit_16->text();
    string file1opt = qOptval.toUtf8().constData();
    std::ostringstream file1NameStream(file1opt);
    file1NameStream<<path<<"/"<<file1opt;
    std::string file1Name = file1NameStream.str();

    qOptmat = ui->lineEdit_17->text();
    string file2opt = qOptmat.toUtf8().constData();
    std::ostringstream file2NameStream(file2opt);
    file2NameStream<<path<<"/"<<file2opt;
    std::string file2Name = file2NameStream.str();
    // save simplex (bug report H12: files were emptied beforehand)
    auto saveSimplex = [&](){
        ofstream o1(file1Name.c_str()), o2(file2Name.c_str());
        for(int i=0; i<nu+1;i++){
            o1<<setprecision(14)<<y[i]<<endl;
            for(int j=0; j<nu; j++){
                o2<<setprecision(14)<<P(i,j)<<endl;
            }
        }
    };
        nm::Coefficients nmCoef;
        nmCoef.alpha=alpha;
        nmCoef.gamma=Gamma;
        nmCoef.beta=nmBeta;
        nmCoef.delta=btot;


    LogFile<<endl;
    LogFile<<"*************************"<<endl;
    LogFile<<"Start Optimisation"<<endl;

    if(ui->checkBox_32->isChecked()){
        ui->checkBox_9->setChecked(false);
        ui->checkBox_11->setChecked(true);
        ui->checkBox_12->setChecked(false);
    }


    // progress for Auto Stop; window from the GUI or 10 per parameter
    nm::ProgressMonitor progress(ui->spinBox_11->value()>0 ? ui->spinBox_11->value() : nm::defaultWindow(nu));
    int itersDone = zaehler;

    //start main loop
        for (int t=0; t<zaehler; t++){
            // iteration limit applies across all restarts (Reinitiate)
            if(++g_itersUsed>zaehler){
                LogFile<<"Maximum number of iterations reached (including restarts)."<<endl;
                itersDone = t;
                break;
            }
            ui->spinBox_4->setValue(t+1);

            string optback1 = withSuffix(ui->lineEdit_16->text(), "_backup").toStdString();
            std::ostringstream back1NameStream(optback1);
            back1NameStream<<path<<"/"<<optback1;
            std::string back1Name = back1NameStream.str();
            ofstream back1(back1Name.c_str());


            string optback2 = withSuffix(ui->lineEdit_17->text(), "_backup").toStdString();
            std::ostringstream back2NameStream(optback2);
            back2NameStream<<path<<"/"<<optback2;
            std::string back2Name = back2NameStream.str();
            ofstream back2(back2Name.c_str());

            for(int i=0; i<nu+1;i++){
                //cout<<y[i]<<endl;
                back1<<setprecision(12)<<y[i]<<endl;
                for(int j=0; j<nu; j++){
                    back2<<setprecision(12)<<P(i,j)<<endl;
                }
            }

            LogFile<<endl;
            LogFile<<"Iteration: "<<t+1<<endl;
            MainWindow::plot_functionvalues();
            qApp->processEvents(QEventLoop::AllEvents);

            cout<<endl;
            cout<<endl;
            cout<<"Iteration: "<<t+1<<"\t";

        //initialize next step
        ym=0;
        ys=0;
        for (int i=0; i<nu; i++){
            Z[i]=0;
        }

        nm::order(y, Ph, Psh, Pl);

        //computing mean and sigma
        for (int i=0; i<nu+1; i++){
            ym+=y[i]/(nu+1);
        }
        for (int i=0; i<nu+1; i++){
            ys+=sqrt(pow((y[i]-ym),2));
        }
        ys=ys/(nu);


        // stagnation (exactly equal simplex values) only as a notice now
        if((ys==ys2) && (ym==ym2)) ++stagnate;
        else stagnate=0;

        // Auto Stop / Reinitiate based on the progress in chi^2 (Delta-chi^2 over a window)
        {
            RunAction act = MainWindow::convergenceStep(progress, y[Pl], nu, LogFile);
            if(act==RunAction::Reinit){
                cout<<"Reinitiate optimisation with current best RVs."<<endl;
                ui->plainTextEdit_2->appendPlainText("Reinitiate optimisation with current best RVs.");
                LogFile<<endl<<"Reinitiate optimisation with current best RVs."<<endl;
                // strengths of the static lines again, at the next solve (best point)
                g_tellFitPending = ui->checkBox_8->isChecked() && ui->checkBox_tellfit->isChecked();

                applyRow(P.row(Pl));
                std::ofstream ReVel(withSuffix(bestStateFile(true), "_restart").toStdString().c_str());
                ReVel<<setprecision(14);
                logRVs(ReVel);
                ReVel.close();
                // the restart takes the best state (not the inputs); the settings stay as they are
                writeBestState(true, y[Pl]);
                g_bestStateS = y[Pl];
                saveSimplex();
                reinitiate=1;
                return;
            }
            if(act==RunAction::Done){
                cout<<"Optimisation stopped (Auto Stop)."<<endl;
                LogFile<<"Optimisation stopped (Auto Stop)."<<endl;
                itersDone = t;
                break;                                      // regular completion after the loop
            }
        }

        cout<<"Stag-Index: "<<stagnate<<endl;

        ys2=ys;
        ym2=ym;

        cout<<"Simplex Mean: "<<ym<<"; Simplex STD: "<<ys<<endl;

        LogFile<<setprecision(12)<<"Mean: "<<ym<<"\tSTD: "<<ys<<endl;

        //Nelder-Mead step (neldermead.h)
        {
            auto fRV = [&](const arma::rowvec &x){
                applyRow(x);
                eval++;
                ui->spinBox_5->setValue(eval);
                qApp->processEvents(QEventLoop::AllEvents);
                MainWindow::ConstructMatrix();
                double r=MainWindow::DivideConquer();
                soptd<<t<<"\t"<<eval<<"\t"<<r<<"\t"<<sigma2Now(r)<<endl;
                qApp->processEvents(QEventLoop::AllEvents);
                if(upda==1){
                    upda=0;
                    LogFile<<endl;
                    LogFile<<setprecision(15)<<"New RVs found; Residuum: "<<r<<endl;
                    logRVs(LogFile);
                    LogFile<<endl;
                }
                return r;
            };
            nm::StepResult st = nm::step(P, y, nmCoef, fRV, [&]{ return abortt==1; });
            LogFile<<st.log;
            if(st.aborted){
                nm::order(y, Ph, Psh, Pl);
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                applyRow(P.row(Pl));
                logRVs(LogFile);
                LogFile<<endl;
                saveSimplex();
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }
        }
        LogFile<<endl;
        }//end main loop


        nm::order(y, Ph, Psh, Pl);

        saveSimplex();

        // adopt best RVs (M6)
        applyRow(P.row(Pl));

        LogFile<<ym<<"          "<<ys<<endl;
        LogFile<<" End Optimisation "<<endl;
        LogFile<<" RVs after "<<itersDone<<" Iterations and "<<eval<<" Evaluations of Optimisation: "<<endl;
        logRVs(LogFile);
        LogFile.close();

        cout<<"RV's after "<<itersDone<<" Iterations and "<<eval<<" Evaluations:"<<endl;
        logRVs(cout);
    }

      //optimization on orbit**********************************************************
      else{

              nu=7;
              int add=0;

              QVector<int> pvf(7);

              if(!ui->checkBox_18->isChecked()){ // P
                  pvf[0]=1; // fixed
                  LogFile<<"P locked ";
                  ++add;
              }
              else{
                  pvf[0]=0; // free
              }
              if(!ui->checkBox_17->isChecked()){ // e
                  pvf[1]=1;
                  LogFile<<"e locked ";
                  ++add;
              }
              else{
                  pvf[1]=0;
              }
              if(!ui->checkBox_31->isChecked()){ // KA
                  pvf[2]=1;
                  LogFile<<"KA locked ";
                  ++add;
              }
              else{
                  pvf[2]=0;
              }
              if(!ui->checkBox_36->isChecked()){ // KB
                  pvf[3]=1;
                  LogFile<<"KB locked ";
                  ++add;
              }
              else{
                  pvf[3]=0;
              }
              if(!ui->checkBox_37->isChecked()){ // Gamma
                  pvf[4]=1;
                  LogFile<<"Gamma locked ";
                  ++add;
              }
              else{
                  pvf[4]=0;
                  cout<<kGammaNote<<endl;
                  LogFile<<endl<<kGammaNote<<endl;
                  if(runow==0) ui->plainTextEdit_2->appendPlainText(kGammaNote);
              }
              if(!ui->checkBox_35->isChecked()){ // T0
                  pvf[5]=1;
                  LogFile<<"T0 locked ";
                  ++add;
              }
              else{
                  pvf[5]=0;
              }
              if(!ui->checkBox_38->isChecked()){ // Omega
                  pvf[6]=1;
                  LogFile<<"Omega locked ";
                  ++add;
              }
              else{
                  pvf[6]=0;
              }

              LogFile<<"Optimisation on orbital parameters."<<endl;

              if(add>=7){
                  reportError("Error 37: All orbital elements are locked.");
                  abortt=0;
                  this->setCursor(QCursor(Qt::ArrowCursor));
                  MainWindow::enableButtons();
                  return;
              }
              nu = nu-add;
              //cout<<"add: "<<add<<endl;

              LogFile<<endl;

              std::vector<double> y(nu+1), Z(nu), Co(nu), So(nu), Eo(nu);
              mat P(nu+1,nu), e(nu+1,nu);
              QVector<double> porbele(nu), ddorbele(nu);
              int pcount=0;

              for(int i=0; i<nu+add; i++){
                  if(pvf[i]==0){
                      porbele[pcount]=orbele[i];
                      ddorbele[pcount]=dorbele[i];
                      ++pcount;
                  }
                  else{
                      //
                  }
              }
              pcount=0;

              // Continue: stored simplex or a new one around the best values; reinitiation: new one
              qOptval = ui->lineEdit_16->text();
              qOptmat = ui->lineEdit_17->text();
              bool contRebuild = false;
              if(inrein==0 && !ui->checkBox_9->isChecked() && ui->checkBox_11->isChecked()){
                  std::vector<double> cur(nu);
                  for(int j=0; j<nu; j++) cur[j]=porbele[j];
                  const std::string why = continueNeedsNewSimplex(nu, cur);
                  if(!why.empty()){
                      contRebuild = true;
                      const std::string m = "Continue: new simplex around the current best orbit because "+why+".";
                      LogFile<<m<<endl;
                      cout<<m<<endl;
                      ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m));
                  }
              }
              const bool rebuild = inrein==1 || contRebuild;

              //create new initial data
              if(ui->checkBox_9->isChecked() || rebuild){
                  r1=0.0;

                  qInitval = ui->lineEdit_14->text();
                  string file1init = qInitval.toUtf8().constData();
                  std::ostringstream file1NameStream(file1init);
                  file1NameStream<<path<<"/"<<file1init;
                  std::string file1Name = file1NameStream.str();

                  qInitmat = ui->lineEdit_15->text();
                  string file2init = qInitmat.toUtf8().constData();
                  std::ostringstream file2NameStream(file2init);
                  file2NameStream<<path<<"/"<<file2init;
                  std::string file2Name = file2NameStream.str();
                  if(rebuild){   // new simplex around the best values: written as the optimisation files
                      file1Name = (qPath+"/"+qOptval).toStdString();
                      file2Name = (qPath+"/"+qOptmat).toStdString();
                  }

                  if(ui->checkBox_9->isChecked() && !rebuild){

                    QFile QIn(file1Name.c_str());
                    QFile QIn2(file2Name.c_str());

                    if((QIn.exists() or QIn2.exists())){
                        if(runow==0){
                            QMessageBox::StandardButton reply;
                            reply = QMessageBox::question(this, "Warning!", "The file already exists. \n\n Do you want to overwrite it?",
                                                    QMessageBox::Yes|QMessageBox::No);
                            if (reply == QMessageBox::Yes) {
                                qDebug() << "Overwrite initial data.";
                                LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                                LogFile<<"Elements:"<<endl;
                            }
                            else{
                                qDebug()<< "Initiation aborted.";
                                this->setCursor(QCursor(Qt::ArrowCursor));
                                MainWindow::enableButtons();
                                return;
                            }
                        }
                        else{
                            cout<<"Warning: The optimisation files already exists. Do you want to overwrite it? 0 no, yes 1: "<<endl;
                            if(batchOverwrite==1) overw=1;
                    else if(!(cin >> overw)) overw=0;
                    if(overw==0) cerr<<"Existing optimisation files are not overwritten (use --overwrite)."<<endl;
                            if(overw==0){
                                qDebug()<< "Initiation aborted.";
                                this->setCursor(QCursor(Qt::ArrowCursor));
                                MainWindow::enableButtons();
                                return;
                            }
                            else{
                                qDebug() << "Overwrite initial data.";
                                LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                                LogFile<<"Elements:"<<endl;
                            }
                        }
                    }
                    else{
                        cout<<"Create new initial data."<<endl;
                        LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                        LogFile<<"Elements:"<<endl;
                    }
                  }
                  else{
                      cout<<"Create new initial data."<<endl;
                      LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                      LogFile<<"Elements:"<<endl;
                      inrein=0;
                  }

                  ofstream init(file1Name.c_str());

                  ofstream init2(file2Name.c_str());

                  for(int i=0; i<7; i++){
                      if(pvf[i]==0){
                          P(0,pcount)=porbele[pcount];
                          cout<<P(0,pcount)<<" ";
                          LogFile<<P(0,pcount)<<" ";
                          ++pcount;
                      }
                      else{
                          //
                      }
                  }
                  cout<<endl;
                  LogFile<<endl;
                  pcount=0;

                  for(int i=0; i<nu+1; i++){
                      for(int j=0; j<nu; j++){
                          if((i>0)&(i==j+1)){
                              e(i,j)=ddorbele[i-1];
                          }
                          else e(i,j)=0;
                          cout<<e(i,j)<<" ";
                          LogFile<<e(i,j)<<" ";
                      }
                      cout<<endl;
                      LogFile<<endl;
                  }
                  cout <<endl;
                  LogFile<<endl;

                  if(rebuild){
                      // reinitiation: steps on the scale of the uncertainty (see reinitSteps)
                      arma::vec h0(nu);
                      for(int j=0; j<nu; j++) h0(j) = e(j+1,j);
                      auto setOrbit = [&](const arma::rowvec &x){
                          int pc=0;
                          for(int k=0; k<7; k++) if(pvf[k]==0) orbele[k] = x(pc++);
                          eval++;
                          ui->spinBox_5->setValue(eval);
                          qApp->processEvents(QEventLoop::AllEvents);
                          return MainWindow::evaluateOrbit();
                      };
                      const arma::vec h = reinitSteps(P.row(0), h0, setOrbit, LogFile);
                      for(int j=0; j<nu; j++) e(j+1,j) = h(j);
                      upda = 0;
                  }

                  for(int i=0; i<nu+1; i++){
                      for(int j=0; j<nu; j++){
                          P(i,j)=P(0,j)+e(i,j);
                          init2<<setprecision(14)<<P(i,j)<<endl;
                          cout<<P(i,j)<<" ";
                          LogFile<<P(i,j)<<" ";
                      }
                      LogFile<<endl;

                      for(int e=0; e<7; e++){
                          if(pvf[e]==0){
                              orbele[e]=P(i,pcount);
                              ++pcount;
                          }
                          else{
                              //
                          }
                      }
                      pcount=0;

                      eval++;
                      ui->spinBox_5->setValue(eval);
                      qApp->processEvents(QEventLoop::AllEvents);
                      y[i]=MainWindow::evaluateOrbit();
                      qApp->processEvents(QEventLoop::AllEvents);
                      soptd<<"0\t"<<eval<<"\t"<<y[i]<<"\t"<<sigma2Now(y[i])<<endl;

                      if(upda==1){
                          upda=0;
                          LogFile<<endl;
                          LogFile<<"New Parameters found."<<endl;
                          LogFile<<"Residuum: "<<y[i]<<endl;
                          LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                          LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                          LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                          LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                          LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                          LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                          LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                          LogFile<<endl;
                      }

                      LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;

                      if(abortt==1){
                          abortt=0;
                          LogFile<<"Initiation aborted."<<endl;
                          this->setCursor(QCursor(Qt::ArrowCursor));
                          MainWindow::enableButtons();
                          return;
                      }
                      if(i==0){
                          r1=y[0];
                          MainWindow::on_pushButton_6_clicked();
                      }
                      else if(y[i] < r1){
                        r1 = y[i];
                        if(ui->checkBox_10->isChecked()){
                            MainWindow::on_pushButton_6_clicked();
                        }
                      }

                      cout<<setprecision(12)<<y[i]<<endl;
                      init<<setprecision(14)<<y[i]<<endl;
                  }
              }

              //continue with optimised data
              else if(ui->checkBox_11->isChecked()){
                  string zeile, one;

                  LogFile<<"Load data from previous optimisation."<<endl;

                  qOptval = ui->lineEdit_16->text();
                  string file1opt = qOptval.toUtf8().constData();
                  std::ostringstream file1NameStream(file1opt);
                  file1NameStream<<path<<"/"<<file1opt;
                  std::string file1Name = file1NameStream.str();
                  ifstream initiate1(file1Name.c_str());

                  qOptmat = ui->lineEdit_17->text();
                  string file2opt = qOptmat.toUtf8().constData();
                  std::ostringstream file2NameStream(file2opt);
                  file2NameStream<<path<<"/"<<file2opt;
                  std::string file2Name = file2NameStream.str();
                  ifstream initiate2(file2Name.c_str());

                  QFile checkfile(file2Name.c_str());

                  if(!checkfile.exists()){
                      qDebug()<<"The optimisation file "<<checkfile.fileName()<<" does not exist.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }

                  int number_of_lines =0;

                  while(std::getline(initiate1, zeile)){
                     ++number_of_lines;
                     dis::keepAlive();
                 }

                  initiate1.clear();
                  initiate1.seekg(0, ios::beg);

                  if(number_of_lines!=nu+1){
                      qDebug()<<"Wrong optimisation file.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The optimisation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The optimisation file does match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The optimisation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }

                  for (int i=0; i<number_of_lines; i++){
                    initiate1 >> one;
                    istringstream ist(one);
                    ist >> y[i];
                    if(i==0){
                        r1=y[0];
                    }
                    else if(y[i]<r1){
                        r1=y[i];
                    }
                  }
                  initiate1.close();
                  g_bestStateS = r1;   // best state: only real improvements on the stored simplex

                  int coun=0;

                  for (int i=0; i<number_of_lines*nu; i++){
                    initiate2 >> one;
                    istringstream ist(one);
                    ist >> P(coun,i-coun*nu);
                    cout<<"P("<<coun<<","<<i-coun*nu<<"): "<<P(coun,i-coun*nu)<<" ";
                    if(i==coun*nu+nu-1){
                        cout<<endl;
                        ++coun;
                    }
                  }
                  initiate2.close();

                  LogFile<<"Optimisation data successfully loaded."<<endl;
              }

              // Use initial data
              else if(ui->checkBox_12->isChecked()){

                  string zeile, one;

                  LogFile<<"Load initial data."<<endl;

                  qInitval = ui->lineEdit_14->text();
                  string file1init = qInitval.toUtf8().constData();
                  std::ostringstream file1NameStream(file1init);
                  file1NameStream<<path<<"/"<<file1init;
                  std::string file1Name = file1NameStream.str();
                  ifstream initiate1(file1Name.c_str());

                  qInitmat = ui->lineEdit_15->text();
                  string file2init = qInitmat.toUtf8().constData();
                  std::ostringstream file2NameStream(file2init);
                  file2NameStream<<path<<"/"<<file2init;
                  std::string file2Name = file2NameStream.str();

                  QFile checkfile(file2Name.c_str());

                  if(!checkfile.exists()){
                      qDebug()<<"The initiation file "<<checkfile.fileName()<<" does not exist.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }
                  ifstream initiate2(file2Name.c_str());

                  int number_of_lines =0;

                  while(std::getline(initiate1, zeile)){
                     ++number_of_lines;
                     dis::keepAlive();
                 }

                  initiate1.clear();
                  initiate1.seekg(0, ios::beg);

                  if(number_of_lines!=nu+1){
                      qDebug()<<"Wrong initiate file.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      cout<<number_of_lines<<endl;
                      LogFile<<"The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }

                  for (int i=0; i<number_of_lines; i++){
                    initiate1 >> one;
                    istringstream ist(one);
                    ist >> y[i];
                    if(i==0){
                        r1=y[0];
                    }
                    else if(y[i]<r1){
                        r1=y[i];
                    }
                  }
                  initiate1.close();

                  int coun=0;

                  for (int i=0; i<number_of_lines*nu; i++){
                    initiate2 >> one;
                    istringstream ist(one);
                    ist >> P(coun,i-coun*nu);
                    cout<<"P("<<coun<<","<<i-coun*nu<<"): "<<P(coun,i-coun*nu)<<" ";
                    if(i==coun*nu+nu-1){
                        cout<<endl;
                        ++coun;
                    }
                  }
                  initiate2.close();

                  LogFile<<"Initial data successfully loaded."<<endl;
              }
              qOptval = ui->lineEdit_16->text();
              string file1opt = qOptval.toUtf8().constData();
              std::ostringstream file1NameStream(file1opt);
              file1NameStream<<path<<"/"<<file1opt;
              std::string file1Name = file1NameStream.str();

              qOptmat = ui->lineEdit_17->text();
              string file2opt = qOptmat.toUtf8().constData();
              std::ostringstream file2NameStream(file2opt);
              file2NameStream<<path<<"/"<<file2opt;
              std::string file2Name = file2NameStream.str();
              // save simplex (bug report H12: files were emptied beforehand)
              auto saveSimplex = [&](){
                  ofstream o1(file1Name.c_str()), o2(file2Name.c_str());
                  for(int i=0; i<nu+1;i++){
                      o1<<setprecision(14)<<y[i]<<endl;
                      for(int j=0; j<nu; j++){
                          o2<<setprecision(14)<<P(i,j)<<endl;
                      }
                  }
              };
              auto writeBestOrbit = [&](){
                  ofstream borbit((qPath+"/"+qOrbitFile).toStdString().c_str());
                  int pc=0;
                  for(int e=0; e<7; e++){
                      if(pvf[e]==0){
                          borbit<<setprecision(14)<<P(Pl,pc)<<endl;
                          ++pc;
                      }
                      else{
                          borbit<<setprecision(14)<<orbele[e]<<endl;
                      }
                  }
              };
        nm::Coefficients nmCoef;
        nmCoef.alpha=alpha;
        nmCoef.gamma=Gamma;
        nmCoef.beta=nmBeta;
        nmCoef.delta=btot;

              LogFile<<endl;
              LogFile<<"*************************"<<endl;
              LogFile<<"Start Optimisation"<<endl;

              if(ui->checkBox_32->isChecked()){
                  ui->checkBox_9->setChecked(false);
                  ui->checkBox_11->setChecked(true);
                  ui->checkBox_12->setChecked(false);
              }


                  //start main loop
                  nm::ProgressMonitor progress(ui->spinBox_11->value()>0 ? ui->spinBox_11->value() : nm::defaultWindow(nu));
                  int itersDone = zaehler;
                  for(int t=0; t<zaehler; t++){
                      // iteration limit applies across all restarts (Reinitiate)
                      if(++g_itersUsed>zaehler){
                          LogFile<<"Maximum number of iterations reached (including restarts)."<<endl;
                          itersDone = t;
                          break;
                      }
                      ui->spinBox_4->setValue(t+1);

                      string optback1 = withSuffix(ui->lineEdit_16->text(), "_backup").toStdString();
                      std::ostringstream back1NameStream(optback1);
                      back1NameStream<<path<<"/"<<optback1;
                      std::string back1Name = back1NameStream.str();
                      ofstream back1(back1Name.c_str());


                      string optback2 = withSuffix(ui->lineEdit_17->text(), "_backup").toStdString();
                      std::ostringstream back2NameStream(optback2);
                      back2NameStream<<path<<"/"<<optback2;
                      std::string back2Name = back2NameStream.str();
                      ofstream back2(back2Name.c_str());

                      for(int i=0; i<nu+1;i++){
                          //cout<<y[i]<<endl;
                          back1<<setprecision(12)<<y[i]<<endl;
                          for(int j=0; j<nu; j++){
                              back2<<setprecision(12)<<P(i,j)<<endl;
                          }
                      }

                      LogFile<<endl;

                      LogFile<<"Iteration: "<<t+1<<"; ";
                      MainWindow::plot_functionvalues();
                      qApp->processEvents(QEventLoop::AllEvents);

                      cout<<endl;
                      cout<<endl;
                      cout<<"Iteration: "<<t+1<<endl;

                      //initialize next step
                      ym = 0;
                      ys = 0;

                      for(int i=0; i<nu; i++){
                          Z[i]=0;
                      }

                      nm::order(y, Ph, Psh, Pl);

                      //computing mean and sigma
                      for(int i=0; i<nu+1; i++){
                          ym+=y[i]/(nu+1);
                      }
                      for(int i=0; i<nu+1; i++){
                          ys+=sqrt(pow((y[i]-ym),2));
                      }
                      ys = ys/nu;

                      // stagnation (exactly equal simplex values) only as a notice now
                      if((ys==ys2) && (ym==ym2)) ++stagnate;
                      else stagnate=0;

                      // Auto Stop / Reinitiate based on the progress in chi^2
                      {
                          RunAction act = MainWindow::convergenceStep(progress, y[Pl], nu, LogFile);
                          if(act==RunAction::Reinit){
                              cout<<"Reinitiate optimisation with current best orbit."<<endl;
                              ui->plainTextEdit_2->appendPlainText("Reinitiate optimisation with current best orbit.");
                              LogFile<<endl<<"Reinitiate optimisation with current best orbit."<<endl;
                              // strengths of the static lines again, at the next solve (best point)
                              g_tellFitPending = ui->checkBox_8->isChecked() && ui->checkBox_tellfit->isChecked();
                              writeBestOrbit();
                              {   // the restart takes the best state (not the inputs); settings unchanged
                                  int pc=0;
                                  for(int e=0; e<7; e++) if(pvf[e]==0) orbele[e]=P(Pl,pc++);
                              }
                              writeBestState(false, y[Pl]);
                              g_bestStateS = y[Pl];
                              saveSimplex();
                              reinitiate=1;
                              return;
                          }
                          if(act==RunAction::Done){
                              cout<<"Optimisation stopped (Auto Stop)."<<endl;
                              LogFile<<"Optimisation stopped (Auto Stop)."<<endl;
                              itersDone = t;
                              break;
                          }
                      }

                      cout<<"Stag-Index: "<<stagnate<<endl;

                      ys2=ys;
                      ym2=ym;

                      cout<<"Simplex Mean: "<<ym<<" Simplex STD: "<<ys<<endl;
                      LogFile<<setprecision(12)<<"Mean: "<<ym<<"; STD: "<<ys<<endl;

                      //Nelder-Mead step (neldermead.h)
                      {
                          auto fOrb = [&](const arma::rowvec &x){
                              int pc=0;
                              for(int e=0; e<7; e++){
                                  if(pvf[e]==0){
                                      orbele[e]=x(pc);
                                      ++pc;
                                  }
                              }
                              eval++;
                              ui->spinBox_5->setValue(eval);
                              qApp->processEvents(QEventLoop::AllEvents);
                              double r=MainWindow::evaluateOrbit();
                              soptd<<t<<"\t"<<eval<<"\t"<<r<<"\t"<<sigma2Now(r)<<endl;
                              qApp->processEvents(QEventLoop::AllEvents);
                              if(upda==1){
                                  upda=0;
                                  LogFile<<endl;
                                  LogFile<<"New Parameters found."<<endl;
                                  LogFile<<"Residuum: "<<r<<endl;
                                  LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                                  LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                                  LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                                  LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                                  LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                                  LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                                  LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                                  LogFile<<endl;
                              }
                              return r;
                          };
                          nm::StepResult st = nm::step(P, y, nmCoef, fOrb, [&]{ return abortt==1; });
                          LogFile<<st.log;
                          if(st.aborted){
                              nm::order(y, Ph, Psh, Pl);
                              LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                              for(int i = 0; i<nu;i++){
                                  LogFile<<setprecision(14)<<P(Pl,i)<<endl;
                              }
                              LogFile<<endl;
                              saveSimplex();
                              abortt=0;
                              this->setCursor(QCursor(Qt::ArrowCursor));
                              MainWindow::enableButtons();
                              return;
                          }
                      }

                  }// end main loop


                  nm::order(y, Ph, Psh, Pl);

                  saveSimplex();
                  // best elements to the orbital elements file (as Levenberg-Marquardt does), so
                  // that "Read data", "Solve" and a new run continue from the result
                  writeBestOrbit();

                  {
                      int pc=0;
                      for(int e=0; e<7; e++){
                          if(pvf[e]==0){
                              orbele[e]=P(Pl,pc);
                              ++pc;
                          }
                      }
                  }
                  // RVs for the best elements (M6)
                  MainWindow::VAmplitudeA();
                  MainWindow::VAmplitudeB();

                  cout<<endl;
                  cout<<setprecision(12)<<"Simplex Mean: "<<ym<<" Simplex STD: "<<ys<<endl;
                  cout<<endl;
                  cout<<"Orbitelements after "<<itersDone<<" Iterations and "<<eval<<" Evaluations:"<<endl;
                  cout<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;

                  LogFile<<endl;
                  LogFile<<"Optimisation ends after "<<itersDone<<" Iterations and "<<eval<<" Evaluations:"<<endl;
                  LogFile<<"Mean: "<<ym<<" and STD "<<ys<<endl;
                  LogFile<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;

                      for(int i = 0; i<7;i++){
                          cout<<setprecision(14)<<orbele[i]<<";\t";
                          LogFile<<setprecision(14)<<orbele[i]<<";\t";
                      }

                cout<<endl;
                LogFile<<endl;

          } // end optimization on orbit

    optimisationFinished=1;

    system_clock::time_point time2 = system_clock::now();

    std::time_t tt1, tt2;

      tt1 = system_clock::to_time_t ( time1 );

      tt2 = system_clock::to_time_t ( time2 );

    double CPUtime = std::difftime(tt2, tt1);
    QString CPtime = QString::number(CPUtime/60);
    ui->lineEdit_6->setText(CPtime);
    QString residual=QString::number(r1);
    ui->lineEdit_7->setText(residual);

    ui->checkBox_9->setChecked(false);
    ui->checkBox_11->setChecked(true);
    ui->checkBox_12->setChecked(false);

    r1=1.0;

    this->setCursor(QCursor(Qt::ArrowCursor));
}

//***************************************
// Plot residuum vs evaluations and interations
//***************************************
void MainWindow::plot_functionvalues(){


    string fvalues = qOptdata.toStdString();
    std::ostringstream valuesNameStream(fvalues);
    valuesNameStream<<path<<"/"<<fvalues;
    std::string valuesName = valuesNameStream.str();


    QFile checkfile1(valuesName.c_str());

    if(!checkfile1.exists()){
        qDebug()<<"Error 2: The file "<<checkfile1.fileName()<<" does not exist.";
    }
    else{
        ifstream fvals(valuesName.c_str());

        // columns: iteration, evaluation, residuum[, sigma^2 frozen when written]
        QVector<double> oiter, oeval, ovals, osig;
        string oline;
        while(std::getline(fvals, oline)){
            istringstream ls(oline);
            double a, b, c, d;
            if(!(ls>>a>>b>>c)) continue;
            if(!(ls>>d)) d = std::numeric_limits<double>::quiet_NaN();
            oiter.push_back(a); oeval.push_back(b); ovals.push_back(c); osig.push_back(d);
        }
        fvals.close();
        const int pointsi = ovals.size();

        ui->customPlot_4->clearGraphs();
        // sigma^2 of the current curve: the current one, else the last one in the file
        double sig2 = g_sigma2;
        if(!(sig2>0) && pointsi>0 && std::isfinite(osig.back()) && osig.back()>0) sig2 = osig.back();
        const bool chi2 = ui->checkBox_14->isChecked() && sig2>0 && pointsi>0;
        if(!chi2){
            // without sigma^2 (residuum "Peak" or no estimate yet): residuum on a linear scale
            ui->customPlot_4->yAxis->setScaleType(QCPAxis::stLinear);
            ui->customPlot_4->yAxis->setNumberFormat("g");
            ui->customPlot_4->yAxis->setLabel("Residuum");
            ui->customPlot_4->legend->setVisible(false);
            ui->customPlot_4->addGraph();
            ui->customPlot_4->graph(0)->addData(oiter, ovals);
            ui->customPlot_4->rescaleAxes();
            ui->customPlot_4->replot();
            return;
        }
        // Delta-chi^2 relative to the best value, logarithmic: points = all evaluations,
        // line = distance of the best solution so far from the current best
        const double floorV = 1e-3;
        double best = *std::min_element(ovals.begin(), ovals.end());
        QVector<double> dAll(pointsi), bestIt, dBest;
        double run = std::numeric_limits<double>::infinity();
        for(int i=0; i<pointsi; i++){
            dAll[i] = std::max(floorV, (ovals[i]*ovals[i]-best*best)/sig2);
            run = std::min(run, ovals[i]);
            if(i+1==pointsi || oiter[i+1]!=oiter[i]){
                bestIt.push_back(oiter[i]);
                dBest.push_back(std::max(floorV, (run*run-best*best)/sig2));
            }
        }
        ui->customPlot_4->yAxis->setScaleType(QCPAxis::stLogarithmic);
        ui->customPlot_4->yAxis->setScaleLogBase(10);
        ui->customPlot_4->yAxis->setNumberFormat("eb");
        ui->customPlot_4->yAxis->setNumberPrecision(0);
        ui->customPlot_4->yAxis->setLabel(QString::fromUtf8("\u0394\u03c7\u00b2 to best"));
        ui->customPlot_4->addGraph();
        ui->customPlot_4->graph(0)->setData(oiter, dAll);
        ui->customPlot_4->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot_4->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(150,150,150), 2));
        ui->customPlot_4->addGraph();
        ui->customPlot_4->graph(1)->setData(bestIt, dBest);
        ui->customPlot_4->graph(1)->setPen(QPen(QColor(31,119,180), 2));
        // gain per iteration: decrease of the best chi^2 in this iteration, with the sigma^2
        // frozen when the improvement was written (does not change afterwards; the curve
        // above refers to the current best and moves up when a better solution is found)
        QVector<double> gIt, gVal;
        {
            double runG = ovals[0];
            for(int i=1; i<pointsi; i++){
                if(ovals[i]<runG){
                    const double s2 = (std::isfinite(osig[i]) && osig[i]>0) ? osig[i] : sig2;
                    const double g = (runG*runG-ovals[i]*ovals[i])/s2;
                    runG = ovals[i];
                    if(!gIt.isEmpty() && gIt.back()==oiter[i]) gVal.back() += g;
                    else { gIt.push_back(oiter[i]); gVal.push_back(g); }
                }
            }
            for(double &g : gVal) g = std::max(floorV, g);
        }
        ui->customPlot_4->addGraph();
        ui->customPlot_4->graph(2)->setData(gIt, gVal);
        ui->customPlot_4->graph(2)->setPen(QPen(QColor(214,96,24), 1));
        ui->customPlot_4->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QColor(214,96,24), 5));
        ui->customPlot_4->graph(0)->setName(QString::fromUtf8("evaluations, \u0394\u03c7\u00b2 to current best"));
        ui->customPlot_4->graph(1)->setName("best so far");
        ui->customPlot_4->graph(2)->setName(QString::fromUtf8("gain per iteration (\u0394\u03c7\u00b2, fixed)"));
        if(ui->checkBox_23->isChecked()){
            // Auto Stop compares the gain summed over its window with this threshold
            const double th = ui->doubleSpinBox_17->value();
            QVector<double> tx{oiter.front(), oiter.back()}, ty{th, th};
            ui->customPlot_4->addGraph();
            ui->customPlot_4->graph(3)->setData(tx, ty);
            QPen pen(QColor(214,96,24), 1, Qt::DashLine);
            ui->customPlot_4->graph(3)->setPen(pen);
            ui->customPlot_4->graph(3)->setName(QString::fromUtf8("Auto Stop threshold (sum over the window)"));
        }
        ui->customPlot_4->legend->setVisible(true);
        ui->customPlot_4->legend->setFont(QFont(font().family(), 8));
        ui->customPlot_4->rescaleAxes();
        double ymax = *std::max_element(dAll.begin(), dAll.end());
        for(double g : gVal) ymax = std::max(ymax, g);
        ui->customPlot_4->yAxis->setRange(floorV/2, std::max(1.0, ymax*20));   // room for the legend
        ui->customPlot_4->replot();
    }


}

//****************************************
// Error message: dialog in the GUI, text output in batch mode. Sets error=1.
//****************************************
void MainWindow::reportError(const QString &msg)
{
    if(runow==0){
        QMessageBox::information(this, "Error", msg);
    }
    else{
        cerr<<msg.toStdString()<<endl;
    }
    error=1;
    this->setCursor(QCursor(Qt::ArrowCursor));
}

//****************************************
// Flux weights per spectrum: from edit.dat (editor) or from the flux ratio.
// Component A: fr/(1+fr), B: 1/(1+fr), static lines: 1.
//****************************************
// Flux-ratio model from the Data tab (constant / Planck / table) on the common grid.
// Constant: g_ratioShape stays empty, so the matrix is built exactly as before.
// Flux-ratio model as set in the Data tab (table read from the work folder)
bool MainWindow::ratioModelFromUi(fr::Model &m, std::string &err) const
{
    m = fr::Model();
    m.mode = fr::Mode(ui->comboBox_ratio->currentIndex());
    m.tA = ui->doubleSpinBox_TA->value();
    m.tB = ui->doubleSpinBox_TB->value();
    m.lambdaRef = ui->doubleSpinBox_lref->value();
    if(m.mode==fr::Mode::Table)
        return fr::loadTable((ui->lineEdit_4->text()+"/"+ui->lineEdit_ratio->text()).toStdString(), m, err);
    return true;
}

bool MainWindow::setupRatioShape()
{
    g_ratioShape.clear();
    g_ratioLref = 0.0;
    fr::Model m;
    std::string err;
    if(!ratioModelFromUi(m, err)){
        reportError("Error 44: "+QString::fromStdString(err));
        return false;
    }
    if(m.mode==fr::Mode::Constant) return true;
    std::vector<double> lam(logbin);
    for(int i=0; i<logbin; i++) lam[i] = std::pow(10.0, W(i));
    double lref = 0.0;
    std::vector<double> sh;
    if(!fr::shapeOnGrid(m, lam, sh, lref, err)){
        reportError("Error 44: "+QString::fromStdString(err));
        return false;
    }
    g_ratioShape = sh;
    g_ratioLref = lref;
    g_ratioModel = m;
    const auto mm = std::minmax_element(sh.begin(), sh.end());
    QString what = m.mode==fr::Mode::Planck
        ? QString("Planck, T_A = %1 K, T_B = %2 K").arg(m.tA).arg(m.tB)
        : "table "+ui->lineEdit_ratio->text();
    QString msg = QString("Flux ratio depends on wavelength (%1): F_A/F_B = k * s(lambda), s = 1 at %2 A, "
                          "%3 ... %4 over %5-%6 A.")
                  .arg(what).arg(lref, 0, 'f', 1).arg(*mm.first, 0, 'g', 4).arg(*mm.second, 0, 'g', 4)
                  .arg(lam.front(), 0, 'f', 1).arg(lam.back(), 0, 'f', 1);
    if(m.mode==fr::Mode::Table){
        msg += QString(" Table value at the reference wavelength: %1 (set it as flux ratio if the table "
                       "holds the measured ratios).").arg(fr::interpLinear(m.tabLambda, m.tabRatio, lref), 0, 'g', 5);
    }
    cout<<msg.toStdString()<<endl;
    ui->plainTextEdit_2->appendPlainText(msg);
    return true;
}

// Barycentric correction per spectrum (Data tab). Spectra in the observatory frame: the RVs of
// the orbit (barycentric) are converted to the observatory frame in buildDesignMatrix; static
// lines stay at rest. Barycentrically corrected spectra: the static lines (observatory frame)
// are shifted by BC. Without a file nothing is converted (as before).
bool MainWindow::loadBaryCorr()
{
    g_bc.clear();
    const QString f = ui->lineEdit_bc->text().trimmed();
    const bool corrected = ui->comboBox_bcframe->currentIndex()==1;
    const bool stat = ui->checkBox_8->isChecked();
    const bool orbitMode = ui->checkBox_16->isChecked();
    auto note = [&](const QString &m){
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    };
    if(f.isEmpty()){
        if(stat && corrected){
            reportError("Error 48: Static lines in barycentrically corrected spectra need the barycentric correction "
                        "per spectrum (Data tab, Barycentric correction).");
            return false;
        }
        if(stat && orbitMode)
            note("Note: orbit mode with static lines but without barycentric correction: the RVs of the orbit are "
                 "used as RVs in the observatory frame. Set the barycentric correction in the Data tab.");
        return true;
    }
    std::vector<double> v;
    std::string err;
    if(!dataio::readLastColumn((qPath+"/"+f).toStdString(), v, err)){
        reportError("Error 48: Barycentric correction: "+QString::fromStdString(err));
        return false;
    }
    if(int(v.size())!=num){
        reportError(QString("Error 48: Barycentric correction file %1 has %2 values, but there are %3 spectra.")
                    .arg(qPath+"/"+f).arg(v.size()).arg(num));
        return false;
    }
    for(double x : v){
        if(!(std::fabs(x)<1000.0)){
            reportError("Error 48: Barycentric correction file "+qPath+"/"+f+": value out of range ("+QString::number(x)+" km/s).");
            return false;
        }
    }
    g_bc = v;
    const auto mm = std::minmax_element(v.begin(), v.end());
    QString use;
    if(corrected) use = stat ? "barycentrically corrected spectra, static lines shifted by BC"
                             : "barycentrically corrected spectra, no static lines: not needed";
    else use = orbitMode ? "spectra in the observatory frame, RVs of the orbit converted to the observatory frame"
                         : "spectra in the observatory frame, fitted RVs in the observatory frame (barycentric: v + BC)";
    note(QString("Barycentric correction (%1): %2 spectra, BC %3 to %4 km/s; %5.")
         .arg(f).arg(num).arg(*mm.first, 0, 'f', 3).arg(*mm.second, 0, 'f', 3).arg(use));
    return true;
}

// Ranges of the static lines (Data tab, "Static lines only in"): read, or the default list
// written to the work folder if the file does not exist
bool MainWindow::readStaticRangeFile(std::vector<std::pair<double, double>> &logRanges, std::vector<std::string> *labels,
                                     QString &name)
{
    logRanges.clear();
    name = ui->lineEdit_tellrange->text().trimmed();
    if(name.isEmpty()){
        name = "telluric_static.dat";
        ui->lineEdit_tellrange->setText(name);
    }
    const QString file = qPath+"/"+name;
    if(!QFileInfo::exists(file)){
        if(name!="telluric_static.dat"){
            reportError("Error 50: File of the ranges of the static lines "+file+" does not exist.");
            return false;
        }
        std::ofstream out(file.toStdString().c_str());
        out<<dataio::kStaticRanges;
        if(!out){
            reportError("Error 50: Cannot write the ranges of the static lines to "+file+".");
            return false;
        }
        const QString m = "Ranges of the static lines: default list (telluric O2 and H2O, weak systems included) written to "+name+".";
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    }
    std::vector<std::pair<double, double>> r;
    std::string err;
    if(!dataio::readRanges(file.toStdString(), r, err, labels) || r.empty()){
        reportError("Error 50: Ranges of the static lines ("+name+"): "+(err.empty() ? QString("no ranges.") : QString::fromStdString(err)));
        return false;
    }
    for(const auto &x : r) logRanges.emplace_back(std::log10(x.first), std::log10(x.second));
    {   // at least one pixel of the data in the ranges
        const double lo = W(0), hi = W(logbin-1);
        bool any = false;
        for(const auto &x : logRanges) if(x.second>=lo && x.first<=hi) any = true;
        if(!any){
            reportError(QString("Error 50: The ranges of the static lines (%1) do not overlap with the data (%2-%3 A).")
                        .arg(name).arg(std::pow(10.0, lo), 0, 'f', 1).arg(std::pow(10.0, hi), 0, 'f', 1));
            logRanges.clear();
            return false;
        }
    }
    return true;
}

bool MainWindow::loadStaticRanges()
{
    g_tellRanges.clear();
    g_tellRangeGroup.clear();
    g_tellGroupNames.clear();
    g_MtelG.clear();
    if(!ui->checkBox_8->isChecked() || !ui->checkBox_tellrange->isChecked()) return true;
    auto note = [&](const QString &m){
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    };
    std::vector<std::string> labels;
    QString name;
    std::vector<std::pair<double, double>> lr;
    if(!readStaticRangeFile(lr, &labels, name)) return false;
    g_tellRanges = lr;
    const size_t nRanges = lr.size();
    // groups by the first word after the numbers (O2, H2O, ...); without a word: "static"
    std::vector<int> count;
    for(const std::string &l : labels){
        const QString g = l.empty() ? QString("static") : QString::fromStdString(l);
        int gi = -1;
        for(size_t i=0; i<g_tellGroupNames.size(); i++) if(g_tellGroupNames[i]==g) gi = int(i);
        if(gi<0){ gi = int(g_tellGroupNames.size()); g_tellGroupNames.push_back(g); count.push_back(0); }
        g_tellRangeGroup.push_back(gi);
        count[size_t(gi)]++;
    }
    QString groups;
    if(g_tellGroupNames.size()>1){
        QStringList gl;
        for(size_t i=0; i<g_tellGroupNames.size(); i++) gl << QString("%1 (%2)").arg(g_tellGroupNames[i]).arg(count[i]);
        groups = "; groups with their own strength: "+gl.join(", ");
    }
    else{
        g_tellRangeGroup.clear();
        g_tellGroupNames.clear();
    }
    note(QString("Static lines only in %1 ranges (%2)%3.").arg(nRanges).arg(name).arg(groups));
    return true;
}

// Rows of the ranges of the static lines (file of the Data tab) for the residuum: the ranges are
// in the observatory frame, in corrected spectra at lambda (1 + BC/c).
bool MainWindow::buildResidualExclusion()
{
    g_rowResExcl.clear();
    if(!ui->checkBox_resstatic->isChecked() || num<1 || logbin<1 || int(W.n_elem)<num*logbin) return true;
    std::vector<std::pair<double, double>> r = g_tellRanges;
    QString name = ui->lineEdit_tellrange->text().trimmed();
    if(r.empty() && !readStaticRangeFile(r, nullptr, name)) return false;
    const bool corrected = ui->comboBox_bcframe->currentIndex()==1 && int(g_bc.size())>=num;
    g_rowResExcl.assign(size_t(num)*size_t(logbin), 0);
    long n = 0, nAll = 0;
    for(int a=0; a<num; a++){
        const double sh = corrected ? std::log10(1.0+g_bc[a]/299792.458) : 0.0;
        for(int px=0; px<logbin; px++){
            const arma::uword i = arma::uword(a)*logbin + px;
            const double w = W(i) - sh;
            for(const auto &x : r)
                if(w>=x.first && w<=x.second){ g_rowResExcl[i] = 1; break; }
            n += g_rowResExcl[i];
            ++nAll;
        }
    }
    if(n>=nAll){
        reportError("Error 53: The ranges of the static lines ("+name+") cover all pixels: nothing left for the residuum.");
        g_rowResExcl.clear();
        return false;
    }
    const QString m = QString("Residuum without the ranges of the static lines (%1): %2 % of the pixels left out%3.")
                      .arg(name).arg(100.0*n/nAll, 0, 'f', 1).arg(corrected ? ", shifted by BC" : "");
    cout<<m.toStdString()<<endl;
    if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    return true;
}

bool MainWindow::loadComponentWeights()
{
    Mval1.resize(num);
    Mval2.resize(num);
    Mtel.resize(num);

    QString ediName = qPath+"/edit.dat";
    if(QFileInfo::exists(ediName)){
        ifstream edit(ediName.toStdString().c_str());
        std::vector<double> v;
        double x;
        while(edit >> x){
            v.push_back(x);
        }
        if(int(v.size())!=3*num){
            reportError("Error 18: Edit file does not match to specified number of observations.");
            return false;
        }
        for(int i=0; i<num; i++){
            Mtel[i]=v[i];
            Mval1[i]=v[num+i];
            Mval2[i]=v[2*num+i];
            if(!std::isfinite(v[i]) || !std::isfinite(v[num+i]) || !std::isfinite(v[2*num+i])
                    || v[i]<0 || v[num+i]<0 || v[2*num+i]<0 || !(v[num+i]+v[2*num+i]>0)){
                reportError(QString("Error 18: Edit file %1: the strengths of spectrum %2 must not be negative, and "
                                    "those of A and B must not both be 0.").arg(ediName).arg(i));
                return false;
            }
        }
    }
    else{
        double fr = ui->doubleSpinBox_4->value();
        if(!(fr>0) || !std::isfinite(fr)){
            reportError("Error 51: The flux ratio F_A/F_B (Data tab) must be positive.");
            return false;
        }
        for(int i=0; i<num; i++){
            Mtel[i]=1.0;
            Mval2[i]=1.0/(1.0+fr);
            Mval1[i]=fr/(1.0+fr);
        }
        cout<<"Elements A:\t"<<Mval1[0]<<endl;
        cout<<"Elements B:\t"<<Mval2[0]<<endl;
        cout<<"Elements static:\t"<<Mtel[0]<<endl;
    }
    // start strengths of the static lines from a file (e.g. the airmass), mean 1
    const QString tw = ui->lineEdit_tellw->text().trimmed();
    if(ui->checkBox_8->isChecked() && !tw.isEmpty()){
        std::vector<std::vector<double>> rows;
        std::string err;
        if(!dataio::readTable((qPath+"/"+tw).toStdString(), 1, rows, err)){
            reportError("Error 49: Strength of the static lines: "+QString::fromStdString(err));
            return false;
        }
        if(int(rows.size())!=num){
            reportError(QString("Error 49: Strength of the static lines: %1 has %2 values, but there are %3 spectra.")
                        .arg(qPath+"/"+tw).arg(rows.size()).arg(num));
            return false;
        }
        double sum = 0.0;
        for(const auto &r : rows){
            if(!(r[0]>0) || !std::isfinite(r[0])){
                reportError("Error 49: Strength of the static lines: "+qPath+"/"+tw+" contains a value <= 0.");
                return false;
            }
            sum += r[0];
        }
        for(int i=0; i<num; i++) Mtel[i] = rows[i][0]*num/sum;
        const auto mm = std::minmax_element(Mtel.begin(), Mtel.begin()+num);
        const QString m = QString("Strength of the static lines from %1 (mean 1): %2 to %3.").arg(tw)
                          .arg(*mm.first, 0, 'f', 3).arg(*mm.second, 0, 'f', 3);
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    }
    return true;
}

// Strengths of the static lines on a sub-problem: only the rows (pixels of all spectra) in
// windows around the ranges of the static lines, widened by the span of the shifts of A and B
// and the largest shift of the static lines, so that the columns of A and B seen by the rows in
// the ranges are covered by all spectra. Alternates LSQR on the sub-problem (warm start) and
// the scale per spectrum (and group), as fitStaticStrengths; the weights of the static columns
// are updated in place. Sets Mtel / g_MtelG. iters, change: as in the full loop.
bool MainWindow::fitStrengthsWindowed(int &iters, double &change, QString &note)
{
    const bool grouped = g_MtelG.size()>1 && g_layout.tGroup.size()==size_t(g_layout.nT);
    const int G = grouped ? int(g_MtelG.size()) : 1;
    // margin in pixels
    double tShift = 0.0;
    if(int(g_bc.size())>=num && ui->comboBox_bcframe->currentIndex()==1)
        for(int a=0; a<num; a++) tShift = std::max(tShift, std::fabs(dis::pixelShift(g_bc[a], g_dlog)));
    const int spanA = std::max(0, g_layout.nA-logbin), spanB = std::max(0, g_layout.nB-logbin);
    const int margin = std::max(spanA, spanB) + int(std::ceil(tShift)) + 4;
    std::vector<char> keepPx(logbin, 0);
    const double w0 = W(0);
    for(const auto &r : g_tellRanges){
        const int lo = std::max(0, int(std::ceil((r.first-w0)/g_dlog)) - margin);
        const int hi = std::min(logbin-1, int(std::floor((r.second-w0)/g_dlog)) + margin);
        for(int i=lo; i<=hi; i++) keepPx[i] = 1;
    }
    const int nPx = int(std::count(keepPx.begin(), keepPx.end(), 1));
    if(nPx==0) return true;
    // sub-matrix: kept rows, columns with entries there
    std::vector<arma::sword> newRow(M.n_rows, -1);
    std::vector<arma::uword> oldRow;
    for(int a=0; a<num; a++)
        for(int i=0; i<logbin; i++)
            if(keepPx[i]){ newRow[arma::uword(a)*logbin+i] = arma::sword(oldRow.size()); oldRow.push_back(arma::uword(a)*logbin+i); }
    M.sync();
    std::vector<arma::uword> ri, ci, oldCol;
    std::vector<double> vv;
    for(arma::uword c=0; c<M.n_cols; c++){
        bool used = false;
        for(arma::uword q=M.col_ptrs[c]; q<M.col_ptrs[c+1]; q++){
            const arma::sword nr = newRow[M.row_indices[q]];
            if(nr<0) continue;
            if(!used){ used = true; oldCol.push_back(c); }
            ri.push_back(arma::uword(nr));
            ci.push_back(oldCol.size()-1);
            vv.push_back(M.values[q]);
        }
    }
    const arma::uword nr = oldRow.size(), nc = oldCol.size();
    // static entries: index into vv with spectrum and group, for the in-place update
    std::vector<size_t> tEnt;
    std::vector<int> tEntSpec, tEntGroup;
    std::vector<int> colT(nc, -1);                     // sub column -> static index k
    for(arma::uword c=0; c<nc; c++){
        const long k = long(oldCol[c]) - g_layout.startT();
        if(k>=0 && k<g_layout.nT) colT[c] = int(k);
    }
    for(size_t e=0; e<vv.size(); e++){
        const int k = colT[ci[e]];
        if(k<0) continue;
        tEnt.push_back(e);
        tEntSpec.push_back(int(oldRow[ri[e]]/arma::uword(logbin)));
        tEntGroup.push_back(grouped ? g_layout.tGroup[size_t(k)] : 0);
    }
    const bool wtd = !g_specW.empty() && int(g_specW.size())==num;
    auto build = [&](bool weighted){
        arma::umat loc(2, vv.size());
        arma::vec val(vv.size());
        for(size_t e=0; e<vv.size(); e++){
            loc(0, e) = ri[e]; loc(1, e) = ci[e];
            val(e) = vv[e]*(weighted ? g_specW[oldRow[ri[e]]/arma::uword(logbin)] : 1.0);
        }
        return arma::sp_mat(loc, val, nr, nc);
    };
    arma::vec Cs(nr), Csw(nr);
    for(arma::uword r=0; r<nr; r++){
        Cs(r) = C(oldRow[r]);
        Csw(r) = Cs(r)*(wtd ? g_specW[oldRow[r]/arma::uword(logbin)] : 1.0);
    }
    const double staticDamp = ui->doubleSpinBox_tdamp->value();
    std::vector<std::vector<double>> cur(G);
    for(int g=0; g<G; g++){
        const double *src = grouped ? g_MtelG[g].data() : Mtel.data();
        cur[g].assign(src, src+num);
    }
    const int edge = std::max(0, g_edgeMargin);
    arma::vec Xs, x0;
    iters = 0;
    change = 1.0;
    for(; iters<kTellFitMaxIter && change>1e-4; iters++){
        const arma::sp_mat Ms = build(false), Msw = wtd ? build(true) : Ms;
        arma::vec damp;
        if(staticDamp>0){
            damp.zeros(nc);
            for(arma::uword c=0; c<nc; c++){
                if(colT[c]<0) continue;
                double n2 = 0.0;
                for(auto it=Msw.begin_col(c); it!=Msw.end_col(c); ++it) n2 += (*it)*(*it);
                damp(c) = std::sqrt(staticDamp*n2);
            }
        }
        const dis::SolveResult r = dis::solveLsqr(Msw, Csw, Xs, 1e-12, 0, x0.n_elem==nc ? &x0 : nullptr,
                                                  damp.n_elem ? &damp : nullptr);
        if(!r.ok){
            cerr<<"Solving the windows failed: "<<r.message<<endl;
            return false;
        }
        x0 = Xs;
        cout<<"LSQR (static windows): "<<r.iterations<<" iterations ("<<nr<<" rows)"<<endl;
        // static spectrum without its continuum level
        std::vector<arma::uword> tc;
        for(arma::uword c=0; c<nc; c++) if(colT[c]>=0) tc.push_back(c);
        arma::vec Tv(tc.size());
        for(size_t q=0; q<tc.size(); q++) Tv(q) = Xs(tc[q]);
        const double lev = dis::continuumLevel(Tv);
        const arma::vec model = Ms*Xs;
        std::vector<std::vector<double>> w(G, std::vector<double>(num, 0.0));
        change = 0.0;
        for(int g=0; g<G; g++){
            arma::vec XT(nc, arma::fill::zeros);
            for(size_t q=0; q<tc.size(); q++){
                const int k = colT[tc[q]];
                if(!grouped || g_layout.tGroup[size_t(k)]==g) XT(tc[q]) = Tv(q)-lev;
            }
            const arma::vec t = Ms*XT;
            std::vector<double> tt(num, 0.0), td(num, 0.0);
            for(arma::uword rr=0; rr<nr; rr++){
                const arma::uword i = oldRow[rr];
                const int a = int(i/arma::uword(logbin)), px = int(i%arma::uword(logbin));
                if(px<edge || px>=logbin-edge || rowUncounted(i)) continue;
                tt[a] += t(rr)*t(rr);
                td[a] += t(rr)*(Cs(rr)-model(rr)+t(rr));
            }
            double sum = 0.0;
            for(int a=0; a<num; a++){
                w[g][a] = tt[a]>0 ? std::max(kMinStrength, cur[g][a]*td[a]/tt[a]) : std::max(kMinStrength, cur[g][a]);
                sum += w[g][a];
            }
            if(!(sum>0)) return true;
            for(int a=0; a<num; a++){ w[g][a] *= num/sum; change = std::max(change, std::fabs(w[g][a]-cur[g][a])); }
        }
        // static weights in place: value *= new / old
        for(size_t q=0; q<tEnt.size(); q++){
            const double o = cur[tEntGroup[q]][tEntSpec[q]];
            vv[tEnt[q]] *= o>0 ? w[tEntGroup[q]][tEntSpec[q]]/o : 0.0;
        }
        cur = w;
    }
    for(int g=0; g<G; g++){
        double *dst = grouped ? g_MtelG[g].data() : Mtel.data();
        for(int a=0; a<num; a++) dst[a] = cur[g][a];
    }
    note = QString(", in windows: %1 of %2 pixels per spectrum").arg(nPx).arg(logbin);
    return true;
}

// Strength of the static lines per spectrum (least squares) for the current solution X:
// with t = contribution of the static lines (continuum level removed) to spectrum j and
// d = data - model + t, the factor is <t, d> / <t, t>; counted pixels only. With groups (O2,
// H2O, ...) one factor per group and spectrum, t from the columns of the group only. Each group
// scaled to a mean of 1 (the amplitude of the static spectrum takes the rest).
// w[g][a]; false if there are no static lines in the fit.
bool MainWindow::fitStaticStrengths(std::vector<std::vector<double>> &w)
{
    if(g_layout.nT<=0 || X.n_elem!=arma::uword(g_layout.cols()) || int(Mtel.size())<num) return false;
    const bool grouped = g_MtelG.size()>1 && g_layout.tGroup.size()==size_t(g_layout.nT);
    const int G = grouped ? int(g_MtelG.size()) : 1;
    // the static spectrum without its continuum level: with equal strengths a constant in T is
    // a null direction (taken up by A and B), and it would dominate the regression
    arma::vec T = X.subvec(g_layout.startT(), g_layout.startT()+g_layout.nT-1);
    T -= dis::continuumLevel(T);
    const arma::vec model = M*X;
    const int edge = std::max(0, g_edgeMargin);
    w.assign(G, std::vector<double>(num, 0.0));
    for(int g=0; g<G; g++){
        arma::vec XT(X.n_elem, arma::fill::zeros);
        for(int k=0; k<g_layout.nT; k++)
            if(!grouped || g_layout.tGroup[size_t(k)]==g) XT(g_layout.startT()+k) = T(k);
        const arma::vec t = M*XT;
        double sum = 0.0;
        for(int a=0; a<num; a++){
            const double cur = grouped ? g_MtelG[size_t(g)][a] : Mtel[a];
            double tt = 0.0, td = 0.0;
            for(int px=edge; px<logbin-edge; px++){
                const arma::uword i = arma::uword(a)*logbin + px;
                if(rowUncounted(i)) continue;
                tt += t(i)*t(i);
                td += t(i)*(C(i)-model(i)+t(i));
            }
            // (lower bound > 0: the update is multiplicative, a strength of exactly 0 would
            // remove the static columns of the spectrum and could never recover)
            w[g][a] = tt>0 ? std::max(kMinStrength, cur*td/tt) : std::max(kMinStrength, cur);
            sum += w[g][a];
        }
        if(!(sum>0)) return false;
        for(double &v : w[g]) v *= num/sum;
    }
    return true;
}

//****************************************
// Weighted residual vector over the counted pixels (same selection and weights as
// computeResiduum with "Mean": |r|^2 = residuum^2).
//****************************************
arma::vec MainWindow::residualVector(const arma::vec &res)
{
    const bool range = ui->checkBox_33->isChecked();
    const double lo = range ? log10(ui->doubleSpinBox_14->value()) : 0.0;
    const double hi = range ? log10(ui->doubleSpinBox_15->value()) : 0.0;
    const int edge = std::max(0, g_edgeMargin);
    std::vector<double> r;
    r.reserve(res.n_elem);
    for(arma::uword i=0; i<res.n_elem; i++){
        if(range && !((W(i)>lo) && (W(i)<hi))) continue;
        if(rowUncounted(i) || rowResExcluded(i)) continue;
        if(edge>0 && logbin>0){
            int px = int(i % arma::uword(logbin));
            if(px<edge || px>=logbin-edge) continue;
        }
        double d = res(i)-C(i);
        if(!g_specW.empty() && logbin>0) d *= g_specW[i/arma::uword(logbin)];
        r.push_back(d);
    }
    return arma::vec(r);
}

//****************************************
// Optimisation with Levenberg-Marquardt (levmar.h) on the individual RVs or on the free
// orbital elements, starting from the current values (RV file / orbit, orbitelements).
// The component spectra are solved for at every evaluation (variable projection); the
// Jacobian of the residual vector comes from forward differences (steps 1e-3 pixel for
// RVs, 1e-4 of the initial Nelder-Mead step for elements), each a warm-started LSQR solve.
// Afterwards a simplex around the best point (vertices one standard error away) is written
// to the optimisation files, so that Error Estimation and a Nelder-Mead "Continue" work.
//****************************************
bool MainWindow::optimiseLM(std::ofstream &LogFile, std::ofstream &soptd)
{
    if(!ui->checkBox_14->isChecked()){
        reportError("Error 41: Levenberg-Marquardt needs the residuum mode 'Mean' (sum of squares).");
        return false;
    }
    const bool rvMode = ui->checkBox->isChecked() || ui->checkBox_2->isChecked();
    const bool sb1 = ui->checkBox_3->isChecked();
    const char *elName[7] = {"P", "e", "K_A", "K_B", "gamma", "T_peri", "omega"};
    int pvf[7] = {!ui->checkBox_18->isChecked(), !ui->checkBox_17->isChecked(), !ui->checkBox_31->isChecked(),
                  !ui->checkBox_36->isChecked(), !ui->checkBox_37->isChecked(), !ui->checkBox_35->isChecked(),
                  !ui->checkBox_38->isChecked()};
    std::vector<std::string> names;
    std::vector<int> elIdx;
    arma::vec x0, h;
    if(rvMode){
        const int n = sb1 ? num : 2*num;
        x0.set_size(n);
        h.set_size(n);
        for(int j=0; j<num; j++){
            x0(j) = RV1[j];
            names.push_back("RV_A_"+std::to_string(j));
            if(!sb1){
                x0(num+j) = RV3[j];
            }
        }
        if(!sb1) for(int j=0; j<num; j++) names.push_back("RV_B_"+std::to_string(j));
        h.fill(1e-3*dv);      // 1e-3 pixel
        LogFile<<"Levenberg-Marquardt on individual RVs ("<<n<<" parameters)."<<endl;
    }
    else{
        for(int e=0; e<7; e++){
            if(pvf[e]) LogFile<<elName[e]<<" locked ";
            else elIdx.push_back(e);
        }
        if(elIdx.empty()){
            reportError("Error 37: All orbital elements are locked.");
            return false;
        }
        x0.set_size(elIdx.size());
        h.set_size(elIdx.size());
        for(size_t i=0; i<elIdx.size(); i++){
            x0(i) = orbele[elIdx[i]];
            h(i) = 1e-4*dorbele[elIdx[i]];
            if(!(std::fabs(h(i))>0)) h(i) = 1e-8*std::max(1.0, std::fabs(x0(i)));
            names.push_back(elName[elIdx[i]]);
        }
        LogFile<<endl<<"Levenberg-Marquardt on orbital parameters ("<<elIdx.size()<<" free)."<<endl;
        if(!pvf[4]){
            cout<<kGammaNote<<endl;
            LogFile<<kGammaNote<<endl;
            if(runow==0) ui->plainTextEdit_2->appendPlainText(kGammaNote);
        }
    }
    const int n = int(x0.n_elem);
    // RV mode: a common shift of all RVs of a component only shifts its reconstructed spectrum
    // (the zero point is not determined by disentangling, also not with static lines, which
    // are an independent unknown spectrum). Along this
    // direction S changes only through the interpolation smoothing, and the optimisation
    // drifts slowly without physical meaning. The mean RV of each component is therefore kept
    // at its start value: x = x0 + Q y with Q an orthonormal basis of the complement.
    // Fixed RVs (Optimisation tab) are constraints as well: x = x0 + Q y with Q an orthonormal
    // basis of the directions that keep fixed RVs and, where needed, the mean RVs. A component
    // with at least one fixed RV has its zero point determined by it.
    arma::mat Q;
    std::vector<int> freeIdx;
    if(rvMode){
        QString ferr;
        if(!MainWindow::freeRVIndices(sb1, freeIdx, ferr)){
            reportError("Error 45: "+ferr);
            return false;
        }
    }
    std::vector<arma::vec> cons;
    bool meanFixed = false;
    if(rvMode){
        std::vector<char> isFree(n, 0);
        for(int k : freeIdx) isFree[k] = 1;
        for(int k=0; k<n; k++){
            if(isFree[k]) continue;
            arma::vec c(n, arma::fill::zeros);
            c(k) = 1.0;
            cons.push_back(c);
        }
        if(num>1){
            for(int comp=0; comp<(sb1 ? 1 : 2); comp++){
                bool anyFixed = false;
                for(int j=0; j<num; j++) if(!isFree[comp*num+j]) anyFixed = true;
                if(anyFixed) continue;
                arma::vec c(n, arma::fill::zeros);
                c.subvec(comp*num, comp*num+num-1).ones();
                cons.push_back(c);
                meanFixed = true;
            }
        }
        if(int(freeIdx.size())<n) LogFile<<"Fixed RVs: "<<(n-int(freeIdx.size()))<<" of "<<n<<"."<<endl;
    }
    const bool fixMeans = !cons.empty();   // constrained parametrisation
    if(fixMeans){
        const int nc = int(cons.size());
        arma::mat Bm(n, nc);
        for(int c=0; c<nc; c++) Bm.col(c) = cons[c];
        arma::mat Qf, R;
        arma::qr(Qf, R, Bm);
        Q = Qf.cols(nc, n-1);
        if(meanFixed) LogFile<<"Mean RV of a component without fixed RVs kept at its start value (zero point not determined by disentangling)."<<endl;
    }
    // Continue without a best state (e.g. a simplex of an older version): start from the best
    // vertex of the stored simplex if it matches the free parameters
    if(ui->checkBox_11->isChecked() && !g_contFromBest && inrein==0){
        qOptval = ui->lineEdit_16->text();
        qOptmat = ui->lineEdit_17->text();
        std::vector<int> axes;
        if(rvMode) axes = freeIdx;
        else for(int i=0; i<n; i++) axes.push_back(i);
        const int na = int(axes.size());
        std::vector<double> sy, sP;
        if(readStoredSimplex(na, sy, sP)){
            const int b = int(std::min_element(sy.begin(), sy.end())-sy.begin());
            for(int a=0; a<na; a++) x0(axes[a]) = sP[b*na+a];
            LogFile<<"Continue: start from the best vertex of the stored simplex."<<endl;
        }
        else LogFile<<"Continue: no stored best values for these parameters; start values from the inputs."<<endl;
    }
    const int ny = fixMeans ? int(Q.n_cols) : n;
    auto toX = [&](const arma::vec &y){ return fixMeans ? arma::vec(x0 + Q*y) : y; };
    LogFile<<"Interpolation: "<<(ui->comboBox_interp->currentIndex()==1 ? "cubic" : "linear")<<endl;

    auto setParams = [&](const arma::vec &x){
        if(rvMode){
            for(int j=0; j<num; j++){
                RV1[j] = x(j);
                if(!sb1) RV3[j] = x(num+j);
            }
            return true;
        }
        for(size_t i=0; i<elIdx.size(); i++) orbele[elIdx[i]] = x(i);
        if(!orbit::elementsValid(orbele[0], orbele[1])) return false;
        RVCalcCheck = 0;
        MainWindow::VAmplitudeA();
        MainWindow::VAmplitudeB();
        return RVCalcCheck==0;
    };
    int lmIt = 0;
    bool wroteStart = false;
    auto resid = [&](const arma::vec &y, arma::vec &r, bool trial){
        if(!setParams(toX(y))) return false;
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        if(trial){
            MainWindow::ConstructMatrix();
            const double s = MainWindow::DivideConquer();
            if(!(s<orbit::kInvalidPenalty)) return false;
            if(!wroteStart){   // start value; afterwards one line per accepted step
                soptd<<lmIt<<"\t"<<eval<<"\t"<<s<<"\t"<<sigma2Now(s)<<endl;
                wroteStart = true;
            }
            upda = 0;
        }
        else{
            if(!MainWindow::buildDesignMatrix(false)) return false;
            if(!MainWindow::solveSystem(MainWindow::currentSolver())) return false;
        }
        r = MainWindow::residualVector(M*X);
        return r.is_finite();
    };

    lm::Options opt;
    opt.h = fixMeans ? arma::vec(ny, arma::fill::value(h(0))) : h;   // Q orthonormal: same step length
    opt.maxIter = std::max(1, zaehler);
    // converged when the model predicts less than 0.002 in chi^2 still to gain (the model
    // curvature can be several times too large, the true remainder is then still < ~0.01;
    // MC tests: 0.002-0.009 above the Nelder-Mead minimum); sigma^2 from S/N or S/bins
    const double known = knownSigma2();
    double tolSigma2 = known;
    if(!(tolSigma2>0)){
        arma::vec r0;
        if(resid(fixMeans ? arma::vec(ny, arma::fill::zeros) : x0, r0, false) && r0.n_elem>0)
            tolSigma2 = arma::dot(r0, r0)/double(r0.n_elem);
    }
    opt.tolS = 0.002*tolSigma2;
    opt.relTolS = 1e-12;
    r1 = 0.0;      // DivideConquer: every improvement is written
    LogFile<<setprecision(15);
    // Reinitiate: after convergence a new run from the best point (new damping, new Jacobian,
    // new secant updates). Ends when a run brings no improvement, i.e. less than the
    // convergence tolerance tolS in S (a restart from a converged point always finds a
    // negligible remainder), or at the iteration limit across all runs.
    const bool reinitLM = ui->checkBox_32->isChecked();
    const int maxIterAll = std::max(1, zaehler);
    int itOffset = 0;            // iterations of the previous runs (plot, counter)
    lm::Result res;
    double prevS = std::numeric_limits<double>::quiet_NaN();
    for(int run=0;; run++){
        const arma::vec y0 = fixMeans ? arma::vec(ny, arma::fill::zeros) : x0;
        opt.maxIter = std::max(1, maxIterAll-itOffset);
        res = lm::minimize(y0, resid, opt, [&]{ return abortt==1; },
            [&](int it, const arma::vec &, double S){
                lmIt = itOffset+it;
                ui->spinBox_4->setValue(lmIt);
                // convergence plot: accepted steps only (trial steps of the line search and
                // rejected steps would appear as spikes)
                soptd<<lmIt<<"\t"<<eval<<"\t"<<setprecision(15)<<std::sqrt(S)<<"\t"<<sigma2Now(std::sqrt(S))<<endl;
                LogFile<<"Iteration "<<lmIt<<": Residuum "<<std::sqrt(S)<<endl;
                cout<<"LM iteration "<<lmIt<<": residuum "<<std::sqrt(S)<<endl;
                MainWindow::plot_functionvalues();
                qApp->processEvents(QEventLoop::AllEvents);
            });
        itOffset += res.iterations;
        LogFile<<res.log;
        if(!res.message.empty()){
            LogFile<<res.message<<endl;
            cerr<<res.message<<endl;
            ui->plainTextEdit_2->appendPlainText(QString::fromStdString(res.message));
        }
        if(res.aborted) abortt = 0;
        if(!res.ok){
            reportError("Error 42: "+QString::fromStdString(res.message));
            return false;
        }
        if(!reinitLM || res.aborted || !res.converged) break;
        std::ostringstream m;
        m<<setprecision(6);
        if(run>0){
            const double gain = prevS-res.S;
            const bool improved = gain > opt.tolS;
            m<<"Levenberg-Marquardt run "<<run+1<<": gain against the previous run "<<gain/tolSigma2
             <<" in chi^2"<<(improved ? " (improved: restart from the new best)." : " (no improvement: stop).");
            LogFile<<m.str()<<endl;
            cout<<m.str()<<endl;
            ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
            if(!improved) break;
            m.str("");
        }
        if(itOffset>=maxIterAll){
            LogFile<<"Maximum number of iterations reached (all runs)."<<endl;
            break;
        }
        prevS = res.S;
        x0 = toX(res.x);         // new start: the best point (constraints keep their values)
        if(ui->checkBox_8->isChecked() && ui->checkBox_tellfit->isChecked()){
            // strengths of the static lines again for the best point (fitted at the start)
            setParams(x0);
            MainWindow::ConstructMatrix();
            g_tellFitPending = true;
            MainWindow::DivideConquer();     // the next run gains what the new strengths bring
        }
        m<<"Reinitiate optimisation with current best "<<(rvMode ? "RVs" : "orbit")<<" (Levenberg-Marquardt).";
        LogFile<<endl<<m.str()<<endl;
        cout<<m.str()<<endl;
        ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
    }
    res.iterations = itOffset;

    // back to the parameters (RVs / elements)
    const arma::vec xbest = toX(res.x);
    // best point: state (RVs, X, residuum bins) and files belong to it
    setParams(xbest);
    MainWindow::ConstructMatrix();
    const double rbest = MainWindow::DivideConquer();
    // Standard errors are not taken from the curvature model: J^T J of the residual vector
    // overestimates the true curvature of S by a factor 1.5-3.5 at S/N ~ 65 (the component
    // spectra absorb part of the noise), and the secant correction is only approximate.
    // The simplex written below lets "Error Estimation" measure the real curvature.
    arma::vec err(n, arma::fill::zeros);
    {
        const double s2 = known>0 ? known : res.S/std::max(1, g_lastResBins - g_lastResCols - ny);
        arma::mat cov = lm::covariance(res.JtJ, s2);                       // in y (= x without fixed means)
        if(fixMeans && cov.n_rows==arma::uword(ny)) cov = Q*cov*Q.t();
        if(cov.n_rows==arma::uword(n)) err = arma::sqrt(arma::abs(cov.diag()));
    }
    LogFile<<endl<<"Levenberg-Marquardt result after "<<res.iterations<<" steps ("<<res.jacobians
           <<" Jacobians, "<<res.evaluations<<" evaluations): residuum "<<rbest
           <<(res.converged ? ", converged" : "")<<endl;
    for(int i=0; i<n; i++) LogFile<<names[i]<<"\t"<<xbest(i)<<endl;
    LogFile<<"Standard errors: run Error Estimation (the simplex around this point has been saved)."<<endl;
    cout<<"Levenberg-Marquardt: residuum "<<rbest<<" after "<<res.iterations<<" steps, "
        <<res.evaluations<<" evaluations"<<(res.converged ? " (converged)" : "")<<endl;
    for(int i=0; i<n; i++) cout<<names[i]<<"\t"<<setprecision(14)<<xbest(i)<<endl;

    // simplex around the best point for Error Estimation / Nelder-Mead Continue:
    // vertex 0 = best, vertex i = best + error from the curvature model along axis i
    // (initial steps for the error estimation; S of the vertices from the model)
    {
        std::ofstream o1((qPath+"/"+ui->lineEdit_16->text()).toStdString().c_str());
        std::ofstream o2((qPath+"/"+ui->lineEdit_17->text()).toStdString().c_str());
        o1<<setprecision(14);
        o2<<setprecision(14);
        // curvature along axis i of x: (J^T J)_x = Q (J^T J)_y Q^T with the means fixed
        arma::mat Hx = res.JtJ;
        if(fixMeans && Hx.n_rows==arma::uword(ny)) Hx = Q*Hx*Q.t();
        // simplex over the free parameters (RV mode: without the fixed RVs, as Nelder-Mead)
        std::vector<int> axes;
        if(rvMode) axes = freeIdx;
        else for(int i=0; i<n; i++) axes.push_back(i);
        const int na = int(axes.size());
        for(int v=0; v<=na; v++){
            arma::vec x = xbest;
            double S = res.S;
            if(v>0){
                const int i = axes[v-1];
                const double d = err(i)>0 ? err(i) : 10.0*h(i);
                x(i) += d;
                if(Hx.n_rows==arma::uword(n)) S += d*d*Hx(i,i);
            }
            o1<<std::sqrt(S)<<endl;
            for(int a=0; a<na; a++) o2<<x(axes[a])<<endl;
        }
    }
    // restore the best point (the simplex vertices were not evaluated)
    setParams(xbest);
    // orbit mode: best elements to the orbital elements file, as in the Nelder-Mead branch, so that
    // "Read data" and "Solve" continue from the result
    if(!rvMode){
        std::ofstream borbit((qPath+"/"+qOrbitFile).toStdString().c_str());
        borbit<<setprecision(14);
        for(int e=0; e<7; e++) borbit<<orbele[e]<<(e<6 ? "\n" : "");
    }
    ui->spinBox_4->setValue(res.iterations);
    return true;
}

//****************************************
// Transformation matrix from the current RVs (RV1, RV3) and weights.
// Sets M, g_layout, bso1, bso2, Mn, Mm, dv and sets X to 0.
// interactive=false: no dialogs (called from the optimisation).
//****************************************
bool MainWindow::buildDesignMatrix(bool interactive)
{
    g_matrixValid = false;
    auto fail = [&](const QString &msg){
        if(interactive) reportError(msg);
        else cerr<<msg.toStdString()<<endl;
        return false;
    };

    if(ui->checkBox_34->isChecked()){
        return fail("Error 25: SB3 systems are not supported.");
    }
    bool sb2 = ui->checkBox_4->isChecked();
    if(!sb2 && !ui->checkBox_3->isChecked()){
        return fail("Error 26: Please choose SB1 or SB2.");
    }
    if(num<1 || logbin<2 || !(g_dlog>0) || RV1.size()<num || (sb2 && RV3.size()<num)
            || Mval1.size()<num || Mval2.size()<num || Mtel.size()<num){
        return fail("Error 27: Data not read or inconsistent; please read data first.");
    }

    dis::MatrixSpec spec;
    spec.num = num;
    spec.logbin = logbin;
    spec.dlog = g_dlog;
    spec.sb2 = sb2;
    spec.telluric = ui->checkBox_8->isChecked();
    const bool cubic = ui->comboBox_interp->currentIndex()==1;
    spec.interp = cubic ? dis::Interpolation::Cubic : dis::Interpolation::Linear;
    spec.rvA.assign(RV1.begin(), RV1.begin()+num);
    if(sb2) spec.rvB.assign(RV3.begin(), RV3.begin()+num);
    spec.wA.assign(Mval1.begin(), Mval1.begin()+num);
    spec.wB.assign(Mval2.begin(), Mval2.begin()+num);
    spec.wT.assign(Mtel.begin(), Mtel.begin()+num);
    // barycentric correction (loadBaryCorr): orbit RVs to the observatory frame, or the static
    // lines shifted by BC in corrected spectra
    if(int(g_bc.size())>=num){
        const bool corrected = ui->comboBox_bcframe->currentIndex()==1;
        if(!corrected && ui->checkBox_16->isChecked()){
            for(int j=0; j<num; j++){
                spec.rvA[j] = toObservatory(RV1[j], g_bc[j]);
                if(sb2) spec.rvB[j] = toObservatory(RV3[j], g_bc[j]);
            }
        }
        if(corrected && spec.telluric) spec.rvT.assign(g_bc.begin(), g_bc.begin()+num);
    }
    if(spec.telluric && !g_tellRanges.empty()){
        spec.tRanges = g_tellRanges;
        spec.w0 = W(0);
        if(g_MtelG.size()>1 && g_tellRangeGroup.size()==g_tellRanges.size()){
            spec.tRangeGroup = g_tellRangeGroup;
            spec.wTgroup = g_MtelG;
        }
    }
    g_colScale.reset();
    if(!g_ratioShape.empty()){
        // F_A/F_B = k_j s(lambda), k_j = wA_j / wB_j (field, Editor or flux-ratio optimisation),
        // sum wA_j + wB_j kept per spectrum. The unknowns are the diluted spectra
        // y_A = rho_A x_A, y_B = rho_B x_B with the light fractions rho of a reference ratio
        // k_ref s(lambda) (k_ref: median of the k_j). The rows then carry h = w / rho, which is
        // exactly 1 for every spectrum with k_j = k_ref: the matrix has the exact null space of
        // the constant-ratio case (offsets of y_A and y_B), instead of directions that are
        // nearly null only, which LSQR would fill with amplified noise. The error of using
        // rho at the observed instead of the rest wavelength is (d ln rho/d ln lambda) v/c,
        // ~1e-4 relative. The output x = y / rho (componentSpectra).
        if(int(g_ratioShape.size())!=logbin){
            return fail("Error 27: Data not read or inconsistent; please read data first.");
        }
        std::vector<double> ks;
        for(int a=0; a<num; a++) if(Mval2[a]>0 && Mval1[a]>=0) ks.push_back(Mval1[a]/Mval2[a]);
        double kref = 1.0;
        if(!ks.empty()){
            std::nth_element(ks.begin(), ks.begin()+ks.size()/2, ks.end());
            kref = ks[ks.size()/2];
        }
        g_ratioKref = kref;
        spec.wArow.resize(size_t(num)*logbin);
        spec.wBrow.resize(size_t(num)*logbin);
        for(int i=0; i<logbin; i++){
            double rhoA, rhoB;
            fr::weights(kref, g_ratioShape[i], 1.0, rhoA, rhoB);
            for(int a=0; a<num; a++){
                double wa = Mval1[a], wb = Mval2[a];
                if(Mval2[a]>0 && Mval1[a]>=0) fr::weights(Mval1[a]/Mval2[a], g_ratioShape[i], Mval1[a]+Mval2[a], wa, wb);
                spec.wArow[size_t(a)*logbin+i] = wa/rhoA;
                spec.wBrow[size_t(a)*logbin+i] = wb/rhoB;
            }
        }
    }

    for(int j=0; j<num; j++){
        double sA = dis::pixelShift(spec.rvA[j], g_dlog);
        double sB = sb2 ? dis::pixelShift(spec.rvB[j], g_dlog) : 0.0;
        double sT = spec.rvT.empty() ? 0.0 : dis::pixelShift(spec.rvT[j], g_dlog);
        if(!std::isfinite(sA) || !std::isfinite(sB) || std::fabs(sA)>=logbin || std::fabs(sB)>=logbin
                || !std::isfinite(sT) || std::fabs(sT)>=logbin){
            return fail("Error 28: Radial velocity out of range for spectrum "+QString::number(j)+".");
        }
    }

    g_layout = dis::buildMatrix(spec, M);
    if(!g_rowMasked.empty()){
        if(g_rowMasked.size()==size_t(M.n_rows)){                 // excluded ranges: rows removed
            M.sync();
            g_colCountFull.assign(M.n_cols, 0);
            g_colCountReal.assign(M.n_cols, 0);
            for(arma::uword c=0; c<M.n_cols; c++){
                g_colCountFull[c] = M.col_ptrs[c+1]-M.col_ptrs[c];
                for(arma::uword k=M.col_ptrs[c]; k<M.col_ptrs[c+1]; k++)
                    if(!g_rowMasked[M.row_indices[k]]) g_colCountReal[c]++;
            }
            dis::dropRows(M, g_rowMasked);
        }
        else g_rowMasked.clear();
    }
    if(!g_ratioShape.empty()){
        // x = y / rho at the rest wavelength of each column (static lines: unchanged)
        g_colScale.ones(g_layout.cols());
        const double w0 = W(0);
        auto rho = [&](char comp, int k){
            const double lam = std::pow(10.0, dis::columnLog10Lambda(g_layout, comp, k, w0, g_dlog));
            double rA, rB;
            fr::weights(g_ratioKref, fr::shape(g_ratioModel, lam, g_ratioLref), 1.0, rA, rB);
            return comp=='A' ? rA : rB;
        };
        for(int k=0; k<g_layout.nA; k++) g_colScale(k) = 1.0/rho('A', k);
        for(int k=0; k<g_layout.nB; k++) g_colScale(g_layout.startB()+k) = 1.0/rho('B', k);
    }

    if(g_edgeMargin<0){
        g_edgeMargin = 0;
        if(ui->checkBox_40->isChecked()){
            // Rows at a distance >= span+1 (cubic: span+2) from the edge only see columns
            // covered by all spectra. 20 % and 2 pixels of margin for the change of the RVs
            // during the optimisation.
            double span = 0;
            auto spanOf = [&](const std::vector<double> &rv){
                double lo=1e300, hi=-1e300;
                for(double v : rv){ double s=dis::pixelShift(v, g_dlog); lo=std::min(lo,s); hi=std::max(hi,s); }
                return rv.empty() ? 0.0 : hi-lo;
            };
            span = std::max(std::max(spanOf(spec.rvA), spanOf(spec.rvB)), spanOf(spec.rvT));
            int m = int(std::ceil(1.2*span)) + (cubic ? 3 : 2);   // cubic stencil: 1 pixel wider
            if(logbin-2*m < 10){
                cerr<<"Exclude edges: spectra too short for an edge of "<<m<<" pixels; full residuum used."<<endl;
                m = 0;
            }
            g_edgeMargin = m;
            cout<<"Residuum without "<<m<<" edge pixels per spectrum (RV range "<<span<<" px)."<<endl;
        }
    }
    if(!g_rowMasked.empty() && g_rowNearMask.size()!=g_rowMasked.size()){
        g_rowNearMask.assign(g_rowMasked.size(), 0);
        const int m = std::max(0, g_edgeMargin);
        if(m>0){
            for(int j=0; j<num; j++){
                const size_t o = size_t(j)*size_t(logbin);
                int last = -1000000;                       // last excluded pixel to the left
                for(int i=0; i<logbin; i++){
                    if(g_rowMasked[o+i]){ last = i; continue; }
                    if(i-last<=m) g_rowNearMask[o+i] = 1;
                }
                last = 1000000;                            // ... to the right
                for(int i=logbin-1; i>=0; i--){
                    if(g_rowMasked[o+i]){ last = i; continue; }
                    if(last-i<=m) g_rowNearMask[o+i] = 1;
                }
            }
        }
    }
    bso1 = g_layout.nA;
    bso2 = g_layout.nB;
    Mn = g_layout.rows();
    Mm = g_layout.cols();
    X.zeros(Mm);

    // key figures for display
    RV1max = RV1[0];
    RV1min = RV1maxi = RV1[0];
    for(int i=0; i<num; i++){
        if(std::fabs(RV1[i])>std::fabs(RV1max)) RV1max=RV1[i];
        RV1min = std::min(RV1min, RV1[i]);
        RV1maxi = std::max(RV1maxi, RV1[i]);
    }
    if(sb2){
        RV3max = RV3[0];
        RV3min = RV3maxi = RV3[0];
        for(int i=0; i<num; i++){
            if(std::fabs(RV3[i])>std::fabs(RV3max)) RV3max=RV3[i];
            RV3min = std::min(RV3min, RV3[i]);
            RV3maxi = std::max(RV3maxi, RV3[i]);
        }
    }
    ui->doubleSpinBox->setValue(std::fabs(RV1max));
    if(sb2) ui->doubleSpinBox_2->setValue(std::fabs(RV3max));

    dv = (pow(10, g_dlog)-1)*c0;
    difference = g_dlog;
    ldiff = pow(10, W(1))-pow(10, W(0));
    ui->doubleSpinBox_3->setValue(dv);

    g_matrixValid = true;
    return true;
}

//****************************************
// Solves M X = C (least squares, minimum norm) with LSQR or SVD (DaC).
//****************************************
bool MainWindow::solveSystem(dis::Solver solver)
{
    if(!g_matrixValid){
        return false;
    }
    if(g_autoWeightsPending){
        // S/N per spectrum "auto": solve, estimate S/N from the residuals, weight; three
        // iterations reach the fixed point (tests: within the statistical scatter)
        g_autoWeightsPending = false;
        g_specW.clear();
        g_sigmaBar2 = 0.0;
        for(int it=0; it<3; it++){
            if(!solveSystem(solver)) return false;
            std::vector<double> est = estimateSnr(M*X);
            if(est.empty() || !setWeightsFromSnr(est)){
                g_specW.clear();
                g_sigmaBar2 = 0.0;
                cerr<<"S/N estimate failed; unweighted."<<endl;
                break;
            }
        }
        g_autoSpecW = g_specW;
        g_autoSigmaBar2 = g_sigmaBar2;
        if(!g_specW.empty()){
            std::ofstream out((qPath+"/snr_estimated.dat").toStdString().c_str());
            out<<setprecision(6);
            for(double w : g_specW) out<<w/std::sqrt(g_sigmaBar2)<<"\n";
            cout<<"S/N per spectrum estimated (auto) and written to snr_estimated.dat; mean noise sigma = "
                <<std::sqrt(g_sigmaBar2)<<endl;
        }
    }
    if(g_tellFitPending){
        // strength of the static lines per spectrum: alternate solving and the scale per
        // spectrum (each step lowers the sum of squares) until the strengths change by less
        // than 1e-4; kept afterwards (optimisation)
        g_tellFitPending = false;
        int it = 0;
        double change = 1.0;
        const bool grouped = g_MtelG.size()>1;
        QString windowNote;
        if(solver==dis::Solver::Lsqr && !g_tellRanges.empty() && g_layout.nT>0){
            // only in windows around the ranges of the static lines (sub-problem), then one
            // full solve with the final strengths (below)
            if(!fitStrengthsWindowed(it, change, windowNote)) return false;
            if(!buildDesignMatrix(false)) return false;
        }
        else for(; it<kTellFitMaxIter && change>1e-4; it++){
            if(!solveSystem(solver)) return false;
            std::vector<std::vector<double>> w;
            if(!fitStaticStrengths(w)) break;
            change = 0.0;
            for(size_t g=0; g<w.size(); g++){
                double *cur = grouped ? g_MtelG[g].data() : Mtel.data();
                for(int a=0; a<num; a++){ change = std::max(change, std::fabs(w[g][a]-cur[a])); cur[a] = w[g][a]; }
            }
            if(!buildDesignMatrix(false)) return false;
        }
        const QString name = withSuffix(ui->lineEdit_10->text().trimmed().isEmpty() ? QString("static.txt")
                                                                                   : ui->lineEdit_10->text().trimmed(), "_strength");
        QStringList spans;
        {
            std::ofstream out((qPath+"/"+name).toStdString().c_str());
            out<<"# strength of the static lines per spectrum (fitted, mean 1)";
            if(grouped){
                out<<"; columns:";
                for(const QString &g : g_tellGroupNames) out<<" "<<g.toStdString();
            }
            out<<"\n"<<setprecision(8);
            for(int a=0; a<num; a++){
                if(grouped){ for(size_t g=0; g<g_MtelG.size(); g++) out<<(g ? "\t" : "")<<g_MtelG[g][a]; }
                else out<<Mtel[a];
                out<<"\n";
            }
            for(size_t g=0; g<(grouped ? g_MtelG.size() : 1); g++){
                const double *v = grouped ? g_MtelG[g].data() : Mtel.data();
                const auto mm = std::minmax_element(v, v+num);
                spans << QString("%1%2 to %3").arg(grouped ? g_tellGroupNames[g]+" " : QString())
                                              .arg(*mm.first, 0, 'f', 3).arg(*mm.second, 0, 'f', 3);
            }
        }
        const QString m = QString("Strength of the static lines fitted per spectrum (%1 iterations%2%5): %3, written to %4.")
                          .arg(it).arg(change>1e-4 ? ", not converged" : "").arg(spans.join("; ")).arg(name).arg(windowNote);
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
        // spectra whose static lines do not fit (strength near 0): usually a wrong barycentric
        // correction, time or wavelength scale of the spectrum, which shifts its telluric lines
        QStringList weak;
        for(int a=0; a<num; a++){
            QStringList gs;
            for(size_t g=0; g<(grouped ? g_MtelG.size() : 1); g++){
                const double v = grouped ? g_MtelG[g][a] : Mtel[a];
                if(v<kWeakStrength) gs << (grouped ? g_tellGroupNames[g]+" " : QString())+QString::number(v, 'f', 3);
            }
            if(!gs.isEmpty()) weak << QString("%1 (%2)").arg(ui->spinBox->value()+a).arg(gs.join(", "));
        }
        if(!weak.isEmpty()){
            const QString wm = QString("Warning: the static lines do not fit spectrum %1 (strength below %2). Check the "
                                       "barycentric correction, the time and the wavelength scale of these spectra "
                                       "(Tools -> Barycentric correction -> Check telluric lines).")
                               .arg(weak.join(", ")).arg(kWeakStrength);
            cout<<wm.toStdString()<<endl;
            if(runow==0) ui->plainTextEdit_2->appendPlainText(wm);
        }
    }
    // warm start (LSQR only): previous solution on the current column layout
    static const bool coldOnly = [](){ const char *e = std::getenv("SPECTANGULAR_COLD_START");
                                       return e && *e && std::string(e)!="0"; }();
    arma::vec x0;
    const bool warm = solver==dis::Solver::Lsqr && g_warmValid && !coldOnly
                      && g_warmLayout.num==g_layout.num && g_warmLayout.logbin==g_layout.logbin
                      && g_warmLayout.nT==g_layout.nT && (g_warmLayout.nB>0)==(g_layout.nB>0);
    if(warm) x0 = dis::transferSolution(g_warmLayout, g_warmX, g_layout);
    const arma::vec *px0 = warm ? &x0 : nullptr;
    g_warmValid = false;

    // static lines: their broad structure is nearly undetermined (only the relative motion of
    // the stars separates it from A and B), and LSQR spends most iterations on these directions
    // (ι Tri A, 47 x 70 000 pixels: 11 720 instead of 1 325 iterations). A weak damping of the
    // static columns, d_k^2 = damp * |column k|^2 (Model tab, default 1e-3), fixes them (the
    // lines shrink by this fraction; broad structure stays in A and B).
    arma::vec damp;
    const double staticDamp = ui->doubleSpinBox_tdamp->value();   // Model tab, default 1e-3
    if(solver==dis::Solver::Lsqr && g_layout.nT>0 && staticDamp>0){
        damp.zeros(M.n_cols);
        M.sync();
        const bool wtd = !g_specW.empty() && int(g_specW.size())==num;
        for(int k=0; k<g_layout.nT; k++){
            const arma::uword c = arma::uword(g_layout.startT()+k);
            double n2 = 0.0;
            for(arma::uword q=M.col_ptrs[c]; q<M.col_ptrs[c+1]; q++){
                const double w = wtd ? g_specW[M.row_indices[q]/arma::uword(logbin)] : 1.0;
                n2 += w*w*M.values[q]*M.values[q];
            }
            damp(c) = std::sqrt(staticDamp*n2);
        }
    }
    const arma::vec *pd = damp.n_elem ? &damp : nullptr;
    dis::SolveResult r;
    if(g_specW.empty() || int(g_specW.size())!=num){
        r = dis::solve(M, C, X, solver, px0, pd);
    }
    else{
        // weighted least squares: rows of spectrum j (matrix and data) times w_j
        arma::sp_mat Mw = M;
        Mw.sync();
        double *val = arma::access::rwp(Mw.values);
        for(arma::uword k=0; k<Mw.n_nonzero; k++){
            val[k] *= g_specW[Mw.row_indices[k]/arma::uword(logbin)];
        }
        arma::vec Cw = C;
        for(arma::uword i=0; i<Cw.n_elem; i++) Cw(i) *= g_specW[i/arma::uword(logbin)];
        r = dis::solve(Mw, Cw, X, solver, px0, pd);
    }
    if(!r.ok){
        cerr<<"Solving failed: "<<r.message<<endl;
        return false;
    }
    if(!g_rowMasked.empty()){
        // columns only in excluded ranges are undetermined: 0 (a warm start would keep its
        // start value there)
        const std::vector<char> empty = dis::emptyColumns(M);
        for(arma::uword c=0; c<X.n_elem && c<empty.size(); c++) if(empty[c]) X(c) = 0.0;
    }
    if(solver==dis::Solver::Lsqr){
        // the null space does not depend on the row weights: unweighted M suffices
        if(warm) dis::removeOffsetNullspace(g_layout, M, X);
        g_warmX = X;
        g_warmLayout = g_layout;
        g_warmValid = X.is_finite();
        g_lsqrIterations += r.iterations;
        g_lsqrSolves++;
        cout<<"LSQR: "<<r.iterations<<" iterations"<<(warm ? " (warm start)" : "")
            <<", "<<r.threads<<" thread"<<(r.threads==1 ? "" : "s")<<" ("<<M.n_rows<<" rows)"<<endl;
        if(g_lsqrThreadsLast!=r.threads){
            g_lsqrThreadsLast = r.threads;
            updateRunInfo();
        }
    }
    else{
        cout<<"SVD (DaC): tsh "<<r.threshold<<"; # singular values: "<<r.rank<<endl;
    }
    if(!r.message.empty()){
        cerr<<r.message<<endl;
        ui->plainTextEdit_2->appendPlainText(QString::fromStdString(r.message));
    }
    return true;
}

//****************************************
// S/N per spectrum from the residuals of the counted pixels, written to snr_estimated.dat
// (one value per spectrum, usable as S/N file for "Weight by S/N").
// sigma_j^2 = sum r^2 / sum (1 - h_i) over the pixels of spectrum j, with the leverage
// h_i of each pixel approximated as sum_c w_i^2 M_ic^2 / sum_k w_k^2 M_kc^2 (exact for
// non-overlapping columns): a spectrum with a high weight determines the component
// spectra strongly and therefore has small residuals. Assumes continuum-normalised
// spectra (sigma = 1/(S/N)). With very different S/N, an estimate from an unweighted
// solution is biased (noise of poor spectra enters the components); iterate: solve,
// use the estimate as weights, solve again.
//****************************************
std::vector<double> MainWindow::estimateSnr(const arma::vec &res)
{
    std::vector<double> est;
    if(num<1 || logbin<1 || M.n_rows!=res.n_elem) return est;
    const bool weighted = !g_specW.empty() && int(g_specW.size())==num;
    auto w2 = [&](arma::uword row){
        const double w = weighted ? g_specW[row/arma::uword(logbin)] : 1.0;
        return w*w;
    };
    // weighted column norms and leverage per row
    arma::vec colNorm(M.n_cols, arma::fill::zeros), lev(M.n_rows, arma::fill::zeros);
    for(arma::uword c=0; c<M.n_cols; c++)
        for(auto it=M.begin_col(c); it!=M.end_col(c); ++it)
            colNorm(c) += w2(it.row())*(*it)*(*it);
    for(arma::uword c=0; c<M.n_cols; c++){
        if(!(colNorm(c)>0)) continue;
        for(auto it=M.begin_col(c); it!=M.end_col(c); ++it)
            lev(it.row()) += w2(it.row())*(*it)*(*it)/colNorm(c);
    }
    const int edge = std::max(0, g_edgeMargin);
    const bool range = ui->checkBox_33->isChecked();
    const double lo = range ? log10(ui->doubleSpinBox_14->value()) : 0.0;
    const double hi = range ? log10(ui->doubleSpinBox_15->value()) : 0.0;
    for(int a=0; a<num; a++){
        double s = 0.0, d = 0.0;
        for(int px=edge; px<logbin-edge; px++){
            const arma::uword i = arma::uword(a)*logbin + px;
            if(range && !((W(i)>lo) && (W(i)<hi))) continue;
            if(rowUncounted(i) || rowResExcluded(i)) continue;
            s += (res(i)-C(i))*(res(i)-C(i));
            d += std::max(0.0, 1.0-lev(i));
        }
        const double snr = (d>0 && s>0) ? 1.0/std::sqrt(s/d) : 0.0;
        est.push_back(snr);
    }
    return est;
}

void MainWindow::writeSnrEstimate(const arma::vec &res, int bins)
{
    if(bins<1) return;
    std::vector<double> est = estimateSnr(res);
    if(est.empty()) return;
    std::ofstream out((qPath+"/snr_estimated.dat").toStdString().c_str());
    out<<setprecision(6);
    for(double v : est) out<<v<<"\n";
    std::vector<double> sorted = est;
    std::sort(sorted.begin(), sorted.end());
    ui->plainTextEdit_2->appendPlainText("S/N per spectrum estimated from the residuals (median "
                                         +QString::number(sorted[sorted.size()/2], 'g', 4)
                                         +"), written to snr_estimated.dat");
}

//****************************************
// Residuum from res = M*X.
// Mean (checkBox_14): sqrt(sum of squares); Peak: largest square < 1.
// Optionally only within the wavelength range (checkBox_33).
//****************************************
double MainWindow::computeResiduum(const arma::vec &res, int *nbins)
{
    bool range = ui->checkBox_33->isChecked();
    bool mean = ui->checkBox_14->isChecked();
    double lo = range ? log10(ui->doubleSpinBox_14->value()) : 0.0;
    double hi = range ? log10(ui->doubleSpinBox_15->value()) : 0.0;
    double r = 0.0;
    int n = 0;
    const int edge = std::max(0, g_edgeMargin);
    std::vector<char> used(res.n_elem, 0);
    for(arma::uword i=0; i<res.n_elem; i++){
        if(range && !((W(i)>lo) && (W(i)<hi))) continue;
        if(rowUncounted(i) || rowResExcluded(i)) continue;
        if(edge>0 && logbin>0){
            int px = int(i % arma::uword(logbin));
            if(px<edge || px>=logbin-edge) continue;
        }
        used[i] = 1;
        double d2 = pow(res(i)-C(i), 2);
        if(!g_specW.empty() && logbin>0){
            const double w = g_specW[i/arma::uword(logbin)];
            d2 *= w*w;   // weighted residuum (chi^2 = residuum^2 / sigma_bar^2)
        }
        ++n;
        if(mean){
            r += d2;
        }
        else if((d2>r) && (d2<1)){
            r = d2;
        }
    }
    if(mean) r = sqrt(r);
    if(nbins) *nbins = n;
    // unknowns occurring in the counted bins (for the degrees of freedom)
    int cols = 0;
    if(M.n_rows==res.n_elem){
        for(arma::uword c=0; c<M.n_cols; c++){
            for(auto it=M.begin_col(c); it!=M.end_col(c); ++it){
                if(used[it.row()]){ ++cols; break; }
            }
        }
    }
    g_lastResCols = cols;
    return r;
}

//****************************************
// Writes components (A, B, static), differences, mean difference per pixel
// and optionally the solution vector. File names relative to the work folder;
// empty name = do not write. Wavelengths in the rest frame of the component.
//****************************************
// Component spectra from the solution X. Constant flux ratio: X itself. Wavelength-dependent
// ratio: X holds the diluted spectra y = rho x (see buildDesignMatrix); x = y / rho, and from x
// the offsets along the null directions are removed (minimum norm in x, as for a constant
// ratio; for s = 1 this gives exactly the constant-ratio result).
// Then the continuum levels of A and B are made equal along the exact offset null direction
// (dis::levelComponents; the model M X and the fit do not change). The minimum-norm choice
// alone puts the two continua at different levels if the flux ratio is not 1, and it is
// dominated by the poorly determined edge pixels (Mizar, RV mode: A at 0.8, B at 1.2, and
// further apart during long optimisations).
arma::vec MainWindow::componentSpectra() const
{
    const bool scaled = g_colScale.n_elem>0 && g_colScale.n_elem==X.n_elem;
    if(M.n_cols!=X.n_elem) return scaled ? arma::vec(g_colScale % X) : X;
    arma::vec x = scaled ? arma::vec(g_colScale % X) : X;
    arma::mat N = dis::offsetNullspace(g_layout, M);
    if(N.n_cols>0){
        if(scaled) N.each_col() %= g_colScale;     // null directions in x: dx = dy / rho
        dis::levelComponents(g_layout, M, N, x);
    }
    return x;
}

// Name of the continuum-normalised component file: "<base>_norm.<ext>" (Output tab option)
static QString normName(const QString &name)
{
    const int slash = std::max(name.lastIndexOf('/'), name.lastIndexOf('\\'));
    const int dot = name.lastIndexOf('.');
    if(dot>slash+1) return name.left(dot)+"_norm"+name.mid(dot);
    return name+"_norm";
}

// Continuum-normalised copies of A and B (Output tab option): spline continuum per piece
// between excluded ranges through the upper points (module continuum)
static void writeNormalised(const QString &file, const std::vector<double> &w, const std::vector<double> &f,
                            double knotA, double low)
{
    cont::Options o;
    o.knotA = knotA;
    o.low = low;
    std::vector<double> c;
    std::string err;
    if(!cont::fit(w, f, o, c, err)){
        cerr<<"Continuum normalisation of "<<file.toStdString()<<" failed: "<<err<<endl;
        return;
    }
    std::ofstream out(file.toStdString().c_str());
    if(!out){ cerr<<"Cannot write "<<file.toStdString()<<endl; return; }
    out<<setprecision(14);
    // a continuum near zero (component without continuum, e.g. badly separated) would blow up
    // the quotient: such points are left out
    std::vector<double> ac(c.size());
    for(size_t i=0; i<c.size(); i++) ac[i] = std::fabs(c[i]);
    std::nth_element(ac.begin(), ac.begin()+ac.size()/2, ac.end());
    const double floorC = 0.05*ac[ac.size()/2];
    size_t skipped = 0;
    for(size_t i=0; i<w.size(); i++){
        if(!(c[i]>floorC)){ ++skipped; continue; }
        out<<w[i]<<"\t"<<f[i]/c[i]<<"\n";
    }
    if(skipped>0)
        cerr<<"Continuum normalisation of "<<file.toStdString()<<": "<<skipped
            <<" points with a continuum near or below zero left out."<<endl;
}

// Continuum-normalised A and B that leave the fit unchanged (Output tab option; idea of the
// user): A' = A - e, B' = B + r e with r = w_A / w_B, so that w_A A' + w_B B' = w_A A + w_B B at
// every wavelength. e(lambda) is smooth (from the continuum fits), so the model of the observed
// spectra hardly changes (shifts of e between the components only); the residuum with A', B'
// is computed and reported. e minimises g_A (c_A - e - 1)^2 + g_B (c_B + r e - 1)^2: the
// continua c_A, c_B (spline per piece through the upper points) are brought as close to 1 as
// possible, weighted by g = 1 / scatter^2 of their continuum points (a line-rich component,
// whose continuum is uncertain, follows the other). Without an offset null direction (light
// ratio varying between the spectra) A and B are divided by their continua instead.
void MainWindow::writeRenormalised(const arma::vec &res, const arma::vec &Xc, const std::vector<double> wc[2],
                                   const std::vector<double> vc[2], const std::vector<int> cc[2],
                                   const QString &fileA, const QString &fileB, double knotA, double lowSigma)
{
    cont::Options o;
    o.knotA = knotA;
    o.low = lowSigma;
    std::vector<double> c[2];
    double g[2];
    for(int p=0; p<2; p++){
        std::string err;
        if(!cont::fit(wc[p], vc[p], o, c[p], err)){
            cerr<<"Continuum fit failed: "<<err<<endl;
            return;
        }
        double s2 = 0.0;
        size_t n = 0;
        for(size_t i=0; i<c[p].size(); i++){
            if(!(c[p][i]>0)) continue;
            const double r = vc[p][i]/c[p][i]-1.0;
            if(r>0){ s2 += r*r; ++n; }
        }
        const double sig = n>0 ? std::sqrt(s2/n) : 1.0;
        g[p] = 1.0/std::max(1e-12, sig*sig);
    }
    // light ratio r = w_A / w_B from the constant offset null direction (b/a = -w_A/w_B)
    const bool scaled = g_colScale.n_elem>0 && g_colScale.n_elem==X.n_elem;
    arma::mat N = dis::offsetNullspace(g_layout, M);
    if(scaled && N.n_cols>0) N.each_col() %= g_colScale;
    double ratio = std::numeric_limits<double>::quiet_NaN();
    for(arma::uword j=0; j<N.n_cols && !std::isfinite(ratio); j++){
        double a = 0.0, b = 0.0;
        int na = 0, nb = 0;
        for(int k : cc[0]) if(N(k, j)!=0.0){ a += N(k, j); ++na; }
        for(int k : cc[1]) if(N(k, j)!=0.0){ b += N(k, j); ++nb; }
        if(na>0 && nb>0 && a!=0.0){
            const double r = -(b/nb)/(a/na);
            if(r>0 && std::isfinite(r)) ratio = r;
        }
    }
    auto interp = [](const std::vector<double> &x, const std::vector<double> &y, double t){
        if(t<=x.front()) return y.front();
        if(t>=x.back()) return y.back();
        const size_t k = size_t(std::upper_bound(x.begin(), x.end(), t)-x.begin());
        const double u = (t-x[k-1])/(x[k]-x[k-1]);
        return y[k-1]+u*(y[k]-y[k-1]);
    };
    std::vector<double> out[2] = {vc[0], vc[1]};
    const bool conserving = std::isfinite(ratio);
    if(conserving){
        std::vector<double> e(wc[0].size());
        for(size_t i=0; i<wc[0].size(); i++){
            const double cB = interp(wc[1], c[1], wc[0][i]);
            e[i] = (g[0]*(c[0][i]-1.0) - g[1]*ratio*(cB-1.0))/(g[0]+g[1]*ratio*ratio);
            out[0][i] -= e[i];
        }
        for(size_t i=0; i<wc[1].size(); i++) out[1][i] += ratio*interp(wc[0], e, wc[1][i]);
    }
    else{
        for(int p=0; p<2; p++)
            for(size_t i=0; i<out[p].size(); i++) out[p][i] = c[p][i]>0 ? vc[p][i]/c[p][i] : vc[p][i];
    }
    // residuum with the renormalised components (the condition of the renormalisation)
    arma::vec x = Xc;
    for(int p=0; p<2; p++) for(size_t i=0; i<out[p].size(); i++) x(cc[p][i]) = out[p][i];
    if(scaled) x /= g_colScale;
    const double r0 = computeResiduum(res), r1 = computeResiduum(arma::vec(M*x));
    for(int p=0; p<2; p++){
        std::ofstream f((p==0 ? fileA : fileB).toStdString().c_str());
        if(!f){ cerr<<"Cannot write "<<(p==0 ? fileA : fileB).toStdString()<<endl; continue; }
        f<<setprecision(14);
        for(size_t i=0; i<out[p].size(); i++) f<<wc[p][i]<<"\t"<<out[p][i]<<"\n";
    }
    std::ostringstream m;
    m<<setprecision(5);
    if(conserving)
        m<<"Renormalised A and B (A - e, B + "<<ratio<<" e: composite unchanged): residuum "<<r0<<" -> "<<r1
         <<" (x "<<setprecision(4)<<r1/r0<<").";
    else
        m<<"Renormalised A and B by their continua (light ratio varies between the spectra, no conserving "
           "renormalisation): residuum "<<r0<<" -> "<<r1<<" (x "<<setprecision(4)<<r1/r0<<").";
    cout<<m.str()<<endl;
    if(runow==0) ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
}

void MainWindow::writeSolution(const arma::vec &res, const QString &fA, const QString &fB,
                               const QString &fT, const QString &fDiff, const QString &fErr,
                               const QString &fCompl)
{
    QStringList unwritable;
    auto openOut = [&](const QString &name, std::ofstream &f){
        if(name.isEmpty()) return false;
        f.open((qPath+"/"+name).toStdString().c_str());
        if(!f){
            cerr<<"Cannot write "<<(qPath+"/"+name).toStdString()<<endl;
            unwritable << name;
            return false;
        }
        f<<setprecision(14);
        return true;
    };
    const double w0 = W(0);
    const arma::vec Xc = componentSpectra();
    // excluded ranges: undetermined component pixels and excluded data pixels are not written
    // (gaps in the files)
    std::vector<char> emptyCol;
    if(!g_rowMasked.empty() && g_colCountFull.size()==M.n_cols && g_colCountReal.size()==M.n_cols){
        emptyCol.assign(M.n_cols, 0);
        for(arma::uword c=0; c<M.n_cols; c++)
            emptyCol[c] = 2*g_colCountReal[c] < g_colCountFull[c];   // lost more than half
    }
    auto skipCol = [&](int c){ return !emptyCol.empty() && c<int(emptyCol.size()) && emptyCol[c]; };
    dis::keepAlive();
    const bool norm = ui->checkBox_norm->isChecked();
    const double knotA = ui->doubleSpinBox_normKnot->value();
    const double lowSigma = ui->doubleSpinBox_normLow->value();
    std::vector<double> wc[2], vc[2];          // written points of A and B
    std::vector<int> cc[2];                     // their matrix columns
    for(int part=0; part<2; part++){
        const QString &name = part==0 ? fA : fB;
        const int n = part==0 ? g_layout.nA : g_layout.nB;
        const int start = part==0 ? 0 : g_layout.startB();
        for(int k=0; k<n; k++){
            if(skipCol(start+k)) continue;
            wc[part].push_back(pow(10, dis::columnLog10Lambda(g_layout, part==0 ? 'A' : 'B', k, w0, g_dlog)));
            vc[part].push_back(Xc(start+k));
            cc[part].push_back(start+k);
        }
        std::ofstream f;
        if(openOut(name, f)){
            for(size_t i=0; i<wc[part].size(); i++) f<<wc[part][i]<<"\t"<<vc[part][i]<<"\n";
        }
        dis::keepAlive();
    }
    if(norm && !fA.isEmpty() && !fB.isEmpty() && wc[0].size()>=3 && wc[1].size()>=3)
        writeRenormalised(res, Xc, wc, vc, cc, qPath+"/"+normName(fA), qPath+"/"+normName(fB), knotA, lowSigma);
    else if(norm && !fA.isEmpty() && wc[0].size()>=3)          // SB1: A alone
        writeNormalised(qPath+"/"+normName(fA), wc[0], vc[0], knotA, lowSigma);
    {
        std::ofstream f;
        if(openOut(fT, f)){
            for(int k=0; k<g_layout.nT; k++)
                if(skipCol(g_layout.startT()+k)) continue; else
                f<<pow(10, dis::columnLog10Lambda(g_layout, 'T', k, w0, g_dlog))<<"\t"<<Xc(g_layout.startT()+k)<<"\n";
        }
    }
    {
        std::ofstream f;
        if(openOut(fDiff, f)){
            for(arma::uword i=0; i<res.n_elem; i++){
                if(!rowMasked(i)) f<<pow(10, W(i))<<" "<<(res(i)-C(i))<<"\n";
                if((i&4095)==0) dis::keepAlive();
            }
        }
    }
    dis::keepAlive();
    {
        std::ofstream f;
        if(openOut(fErr, f)){
            for(int i=0; i<logbin; i++){
                double m = 0.0;
                int cnt = 0;
                for(int n=0; n<num; n++){
                    if(rowMasked(arma::uword(i+n*logbin))) continue;
                    m += res(i+n*logbin)-C(i+n*logbin);
                    ++cnt;
                }
                if(cnt>0) f<<pow(10, W(i))<<"\t"<<m/cnt<<"\n";
            }
        }
    }
    {
        std::ofstream f;
        if(openOut(fCompl, f)){
            // RVs the solution belongs to (for Subtract)
            for(int j=0; j<num; j++)
                f<<"# rv "<<j<<" "<<RV1[j]<<" "<<(bso2>0 ? RV3[j] : 0.0)<<"\n";
            for(arma::uword n=0; n<X.n_elem; n++)
                f<<n<<" "<<X(n)<<"\n";
        }
    }
    if(!unwritable.isEmpty())
        reportError("Error 52: Cannot write "+unwritable.join(", ")+" in "+qPath
                    +" (folder missing or not writable, or the name is a folder; Output tab).");
}

//****************************************
// Matrix construction for DSM
//****************************************
void MainWindow::ConstructMatrix(){
    MainWindow::buildDesignMatrix(false);
}

//*******************************************
//ECON for optimization
//*******************************************
double MainWindow::DivideConquer(){

    residu=0;

    if(!MainWindow::solveSystem(MainWindow::currentSolver())){
        residu = orbit::kInvalidPenalty;
        return residu;
    }
    vec res = M*X;

    int resbins=0;
    residu = MainWindow::computeResiduum(res, &resbins);
    g_lastResBins = resbins;

    cout<<setprecision(15)<<"Residuum: "<<residu<<endl;
    cout<<"  # Bins: "<<resbins<<endl;
    if(resbins>0) cout<<"  r/Bins: "<<residu/resbins<<endl;

    trackBest(residu);

    // better value found, write data to files.
    if(((residu<r1) or (r1==0.0)) and (optfratios==0)){

        r1=residu;
        upda=1;

        for(int i=0; i<num; i++){
            QString index= QString::number(i);
            if(ui->checkBox_3->isChecked()){
                QString velocity=QString::number(RV1[i]);
                ui->plainTextEdit->appendPlainText(index+" "+velocity);
            }
            else if(ui->checkBox_4->isChecked()){
                QString velocity=QString::number(RV1[i]);
                QString sum=QString::number(RV3[i]);
                ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+sum);
            }
        }

        if(ui->checkBox_16->isChecked()){
            cout<<"New Parameters found:"<<endl;
            cout<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;
            for(int i=0; i<7;i++){
                cout<<setprecision(14)<<orbele[i]<<" ";
            }
            cout<<endl;

            ui->plainTextEdit_2->appendPlainText("New Parameters found:\nPeriod; Eccentricity; Amplitude A; Amplitude B; Systemic V; T_peri; Omega A;");
            QString Peri = QString::number(orbele[0]);
            QString Ecce = QString::number(orbele[1]);
            QString AA = QString::number(orbele[2]);
            QString AB = QString::number(orbele[3]);
            QString SV = QString::number(orbele[4]);
            QString Tper = QString::number(orbele[5]);
            QString wper = QString::number(orbele[6]);
            ui->plainTextEdit_2->appendPlainText(Peri+" "+Ecce+" "+AA+" "+AB+" "+SV+" "+Tper+" "+wper);
        }
        else{
            cout<<"Found better RVs."<<endl;
            cout<<"Residuum: "<<r1<<endl;
        }

        ui->spinBox_6->setValue(eval);

        QString residual=QString::number(residu);
        ui->lineEdit_7->setText(residual);

        MainWindow::writeSolution(res, ui->lineEdit_5->text(), ui->lineEdit_8->text(), ui->lineEdit_10->text(),
                                  ui->lineEdit_13->text(), ui->lineEdit_20->text(), "compl.txt");

        // Save all Updates
        if(ui->checkBox_19->isChecked()){
            QString n = QString::number(updac);
            MainWindow::writeSolution(res, "compa_"+n+".dat", "compb_"+n+".dat", "tellur_"+n+".dat",
                                      "differences_"+n+".dat", "error_"+n+".dat", "");
        }

        if(ui->checkBox_10->isChecked()){
            MainWindow::on_pushButton_6_clicked();
        }

        if(ui->checkBox_19->isChecked()){
            QString qEval = QString::number(updac);
            ui->customPlot->savePng(qPath+"/Sol_"+qEval+".png");
            ui->customPlot_5->savePng(qPath+"/Residuals_"+qEval+".png");
        }

        ++updac;
    }
    // write to files to determine inclinations
    else if(optfratios==1){

        MainWindow::writeSolution(res, "frcompa.dat", "frcompb.dat", "frtellur.dat",
                                  "frdifferences.dat", "frerror.dat", "");

        // use function slope to optimise
        if(ui->checkBox_30->isChecked()){
            residu=MainWindow::inclinations();
        }

        if(residu<r1){
            r1=residu;
            upda=1;
            cout<<"Better flux ratios found."<<endl;
            for(int i=0; i<num; i++){
                cout<<Mval1[i]<<"\t"<<Mval2[i]<<endl;
            }
            ui->spinBox_6->setValue(eval);

            QString residual=QString::number(residu);
            ui->lineEdit_7->setText(residual);
        }
        if(r1==0.0){
            r1=residu;
            upda=1;
        }
    }
    qApp->processEvents(QEventLoop::AllEvents);

    return residu;
}

//*******************************************************
//plot results in main window
//*******************************************************
namespace {
// y range of plotted data without the outermost pixels of each segment (a new segment begins
// where x decreases: the differences hold one segment per spectrum). The edge columns of the
// component spectra are determined by single spectra only and can be strong outliers that
// would squeeze the plot; they stay in the data, only the range ignores them.
void innerYRange(const QVector<double> &x, const QVector<double> &y, int trim, double &lo, double &hi)
{
    int start = 0;
    const int n = std::min(x.size(), y.size());
    for(int i=1; i<=n; i++){
        if(i==n || x[i]<x[i-1]){
            const int len = i-start, t = std::min(trim, len/4);
            for(int k=start+t; k<i-t; k++){
                if(std::isfinite(y[k])){ lo = std::min(lo, y[k]); hi = std::max(hi, y[k]); }
            }
            start = i;
        }
    }
}

// pixels left out per segment end: at least 3 and 3 %, and the "Exclude edges" margin
int plotTrim(int n, int edgeMargin)
{
    return std::max({3, int(std::ceil(0.03*n)), edgeMargin});
}

// differences: one graph per spectrum (a new spectrum begins where x decreases). In one
// graph the spectra share the keys, and the adaptive sampling of QCustomPlot then drops the
// points of the middle spectra for long spectra (gaps of hundreds of Angstrom at 86 000 pixels).
void plotSegments(QCustomPlot *p, const QVector<double> &x, const QVector<double> &y)
{
    const int n = std::min(x.size(), y.size());
    int start = 0;
    for(int i=1; i<=n; i++){
        if(i==n || x[i]<x[i-1]){
            QCPGraph *g = p->addGraph();
            g->setData(x.mid(start, i-start), y.mid(start, i-start));
            g->setPen(QPen(Qt::red));
            g->setLineStyle(QCPGraph::lsNone);
            g->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
            start = i;
        }
    }
}

void setYRange(QCustomPlot *p, double lo, double hi)
{
    if(!(lo<=hi)) return;           // no data: keep the axis
    double pad = 0.05*(hi-lo);
    if(!(pad>0)) pad = 0.05*std::max(1e-3, std::fabs(hi));
    p->yAxis->setRange(lo-pad, hi+pad);
}
}

// Residual statistics per spectrum: observation - model, from the current solution, in the
// wavelength range of the Convergence & residuals tab (auto: whole spectrum) without the edge
// pixels. Written to diffstatistic.dat and plotted against the orbital phase (orbit mode),
// the time of observation (times file present) or the spectrum number. Shows variability of
// lines from spectrum to spectrum, which the disentangled (mean) spectra do not contain.
void MainWindow::plotResidualStatistics()
{
    if(!g_matrixValid || num<1 || logbin<1 || int(C.n_elem)!=num*logbin || M.n_rows!=C.n_elem
            || X.n_elem!=M.n_cols){
        return;
    }
    const arma::vec model = M*X;
    // window: the values given here; "auto" takes the residuum range of the Model tab
    // ("Only from ... to"), if it is switched on, otherwise the whole spectrum
    double l1 = ui->doubleSpinBox_st1->value(), l2 = ui->doubleSpinBox_st2->value();
    const bool modelRange = ui->checkBox_33->isChecked();
    if(!(l1>0)) l1 = (modelRange && ui->doubleSpinBox_14->value()>0) ? ui->doubleSpinBox_14->value() : 0.0;
    if(!(l2>0)) l2 = (modelRange && ui->doubleSpinBox_15->value()>0) ? ui->doubleSpinBox_15->value()
                                                                      : std::numeric_limits<double>::infinity();
    const int edge = std::max(0, g_edgeMargin);

    // abscissa
    std::vector<double> x(num);
    QString xlabel = "Spectrum";
    std::vector<double> t;
    if(int(otimes.size())>=num && ui->checkBox_16->isChecked()){
        t.assign(otimes.begin(), otimes.begin()+num);
    }
    else{
        std::vector<std::vector<double>> rows;
        std::string err;
        if(dataio::readTable((qPath+"/"+ui->lineEdit_18->text()).toStdString(), 1, rows, err) && int(rows.size())==num){
            for(const auto &r : rows) t.push_back(r[0]);
        }
    }
    const bool phase = ui->checkBox_16->isChecked() && int(t.size())==num && orbele.size()>=6 && orbele[0]>0;
    for(int j=0; j<num; j++){
        if(phase){
            double ph = std::fmod((t[j]-orbele[5])/orbele[0], 1.0);
            x[j] = ph<0 ? ph+1.0 : ph;
        }
        else x[j] = int(t.size())==num ? t[j] : double(j+mini);
    }
    if(phase) xlabel = "Orbital Phase";
    else if(int(t.size())==num) xlabel = "Time";

    QVector<double> qx(num), qmean(num), qrms(num);
    double allS2 = 0.0;
    long allN = 0;
    std::ofstream f((qPath+"/"+qDiffStat).toStdString().c_str());
    f<<"# Residuals per spectrum, observation - model";
    if(std::isfinite(l2) || l1>0) f<<", "<<l1<<" - "<<l2<<" A";
    f<<", without "<<edge<<" edge pixels\n";
    const bool timeCol = phase;     // with the phase as abscissa the time is added (period analysis)
    f<<"# spectrum\t"<<(phase ? "phase" : (int(t.size())==num ? "time" : "spectrum"))
     <<"\tpixels\tmean\trms\tstd"<<(timeCol ? "\ttime" : "")<<"\n";
    f<<setprecision(10);
    for(int j=0; j<num; j++){
        double s1 = 0.0, s2 = 0.0;
        int n = 0;
        for(int i=edge; i<logbin-edge; i++){
            const double lam = std::pow(10.0, W(j*logbin+i));
            if(lam<l1 || lam>l2) continue;
            if(rowUncounted(arma::uword(j*logbin+i)) || rowResExcluded(arma::uword(j*logbin+i))) continue;
            const double d = C(j*logbin+i) - model(j*logbin+i);
            s1 += d;
            s2 += d*d;
            ++n;
        }
        allS2 += s2;
        allN += n;
        const double mean = n>0 ? s1/n : 0.0;
        const double rms = n>0 ? std::sqrt(s2/n) : 0.0;
        const double sd = n>1 ? std::sqrt(std::max(0.0, (s2 - n*mean*mean)/(n-1))) : 0.0;
        f<<(j+mini)<<"\t"<<x[j]<<"\t"<<n<<"\t"<<mean<<"\t"<<rms<<"\t"<<sd;
        if(timeCol) f<<"\t"<<t[j];
        f<<"\n";
        qx[j] = x[j]; qmean[j] = mean; qrms[j] = rms;
    }
    f.close();

    if(runow!=0) return;     // batch mode: file only
    // against the phase (orbit mode): a second plot against the number of the spectrum; otherwise
    // (time or number) one plot against the number only (the file keeps the time)
    const bool twoPlots = phase;
    if(!twoPlots){
        for(int j=0; j<num; j++) qx[j] = j+mini;
        xlabel = "Spectrum";
    }
    ui->customPlot_6->setVisible(twoPlots);
    QCustomPlot *p = ui->customPlot_5;
    p->xAxis->setAutoTickStep(twoPlots);
    p->xAxis->setAutoSubTicks(twoPlots);
    if(!twoPlots){     // integer ticks (1, 2, 5, 10, ... spectra)
        double st = 1.0;
        while(num/st > 12) st *= (QString::number(st).startsWith('2') ? 2.5 : 2.0);
        p->xAxis->setTickStep(st);
        p->xAxis->setSubTickCount(0);
    }
    p->clearGraphs();
    p->addGraph();
    p->graph(0)->setData(qx, qrms);
    p->graph(0)->setLineStyle(QCPGraph::lsNone);
    p->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(31,119,180), 6));
    p->graph(0)->setName("RMS");
    p->addGraph();
    p->graph(1)->setData(qx, qmean);
    p->graph(1)->setLineStyle(QCPGraph::lsNone);
    p->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QColor(214,39,40), 6));
    p->graph(1)->setName("mean");
    double ylo = 0.0, yhi = 0.0;
    for(int j=0; j<num; j++){ ylo = std::min({ylo, qmean[j], qrms[j]}); yhi = std::max({yhi, qmean[j], qrms[j]}); }
    double xlo = *std::min_element(qx.begin(), qx.end()), xhi = *std::max_element(qx.begin(), qx.end());
    if(phase){ xlo = 0.0; xhi = 1.0; }
    auto hline = [&](double y, Qt::PenStyle st, const QString &name){
        p->addGraph();
        p->graph(p->graphCount()-1)->setData(QVector<double>{xlo, xhi}, QVector<double>{y, y});
        p->graph(p->graphCount()-1)->setPen(QPen(Qt::gray, 1, st));
        p->graph(p->graphCount()-1)->setName(name);
        yhi = std::max(yhi, y);
    };
    if(allN>0) hline(std::sqrt(allS2/allN), Qt::DotLine, "RMS of all spectra");
    const double sig2 = knownSigma2();
    if(sig2>0) hline(std::sqrt(sig2), Qt::DashLine, "noise 1/(S/N)");
    p->xAxis->setLabel(xlabel);
    p->yAxis->setLabel("O-M");
    p->legend->setVisible(true);
    p->rescaleAxes();
    if(phase) p->xAxis->setRange(0.0, 1.0);
    else{
        const double xr = p->xAxis->range().size();
        if(!twoPlots) p->xAxis->setRange(mini-0.7, mini+num-0.3);
        else p->xAxis->setRange(p->xAxis->range().lower - 0.03*xr - 0.5*(xr==0), p->xAxis->range().upper + 0.03*xr + 0.5*(xr==0));
    }
    setYRange(p, ylo, yhi + 0.8*(yhi-ylo));   // room for the legend above the points

    // against the number of the spectrum; outliers (RMS > median + 5 x 1.4826 MAD) labelled in
    // both plots
    std::vector<double> rv(qrms.begin(), qrms.end()), ad;
    std::vector<double> tmp = rv;
    std::nth_element(tmp.begin(), tmp.begin()+tmp.size()/2, tmp.end());
    const double med = tmp[tmp.size()/2];
    for(double v : rv) ad.push_back(std::fabs(v-med));
    std::nth_element(ad.begin(), ad.begin()+ad.size()/2, ad.end());
    const double sig = 1.4826*ad[ad.size()/2];
    QCustomPlot *q = ui->customPlot_6;
    q->clearGraphs();
    q->clearItems();
    p->clearItems();
    QVector<double> qn(num);
    for(int j=0; j<num; j++) qn[j] = j+mini;
    q->addGraph();
    q->graph(0)->setData(qn, qrms);
    q->graph(0)->setLineStyle(QCPGraph::lsNone);
    q->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, QColor(31,119,180), 6));
    q->graph(0)->setName("RMS");
    q->addGraph();
    q->graph(1)->setData(qn, qmean);
    q->graph(1)->setLineStyle(QCPGraph::lsNone);
    q->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QColor(214,39,40), 6));
    q->graph(1)->setName("mean");
    QStringList outl;
    for(int j=0; j<num; j++){
        if(!(sig>0) || qrms[j] <= med + 5.0*sig) continue;
        outl << QString::number(j+mini);
        for(auto pr : {std::make_pair(q, qn[j]), std::make_pair(p, qx[j])}){
            auto *lab = new QCPItemText(pr.first);
            lab->position->setCoords(pr.second, qrms[j]);
            lab->setPositionAlignment(Qt::AlignLeft|Qt::AlignBottom);
            lab->setText(" "+QString::number(j+mini));
            lab->setColor(QColor(214,39,40));
        }
    }
    q->setProperty("outliers", outl.join(","));
    q->legend->setVisible(false);
    q->rescaleAxes();
    // integer ticks (1, 2, 5, 10, ... spectra)
    q->xAxis->setAutoTickStep(false);
    {
        double st = 1.0;
        while(num/st > 12) st *= (QString::number(st).startsWith('2') ? 2.5 : 2.0);
        q->xAxis->setTickStep(st);
        q->xAxis->setAutoSubTicks(false);
        q->xAxis->setSubTickCount(0);
    }
    q->xAxis->setRange(mini-0.7, mini+num-0.3);
    setYRange(q, ylo, yhi + 0.15*(yhi-ylo));
    p->replot();
    q->replot();
}

//**************************************************
// Analysis of the solution (Output tab)
//**************************************************
void MainWindow::spectrumTimes(std::vector<double> &t, std::vector<double> &phase)
{
    t.clear();
    phase.clear();
    if(int(otimes.size())>=num && ui->checkBox_16->isChecked()) t.assign(otimes.begin(), otimes.begin()+num);
    else{
        std::vector<std::vector<double>> rows;
        std::string err;
        if(dataio::readTable((qPath+"/"+ui->lineEdit_18->text()).toStdString(), 1, rows, err) && int(rows.size())==num)
            for(const auto &r : rows) t.push_back(r[0]);
    }
    if(ui->checkBox_16->isChecked() && int(t.size())==num && orbele.size()>=6 && orbele[0]>0)
        for(int j=0; j<num; j++){
            double ph = std::fmod((t[j]-orbele[5])/orbele[0], 1.0);
            phase.push_back(ph<0 ? ph+1.0 : ph);
        }
}

namespace {
// Catmull-Rom value and derivative of v at the fractional position x (pixels); false if the
// stencil leaves the vector or touches a bad pixel
bool cubicAt(const std::vector<double> &v, const std::vector<char> &bad, double x, double &y, double &dy)
{
    const int i = int(std::floor(x));
    if(i<1 || i+2>=int(v.size())) return false;
    for(int k=i-1; k<=i+2; k++) if(bad[size_t(k)]) return false;
    const double t = x-i, p0 = v[size_t(i-1)], p1 = v[size_t(i)], p2 = v[size_t(i+1)], p3 = v[size_t(i+2)];
    const double a = -0.5*p0 + 1.5*p1 - 1.5*p2 + 0.5*p3, b = p0 - 2.5*p1 + 2*p2 - 0.5*p3, c = -0.5*p0 + 0.5*p2;
    y = ((a*t + b)*t + c)*t + p1;
    dy = (3*a*t + 2*b)*t + c;
    return true;
}

// solves the symmetric system A x = b (n <= 4) by Gauss elimination; false if singular
bool solveSmall(std::vector<std::vector<double>> A, std::vector<double> b, std::vector<double> &x)
{
    const int n = int(b.size());
    for(int c=0; c<n; c++){
        int p = c;
        for(int r=c+1; r<n; r++) if(std::fabs(A[r][c])>std::fabs(A[p][c])) p = r;
        if(!(std::fabs(A[p][c])>1e-300)) return false;
        std::swap(A[p], A[c]);
        std::swap(b[p], b[c]);
        for(int r=c+1; r<n; r++){
            const double q = A[r][c]/A[c][c];
            for(int k=c; k<n; k++) A[r][k] -= q*A[c][k];
            b[r] -= q*b[c];
        }
    }
    x.assign(n, 0.0);
    for(int c=n-1; c>=0; c--){
        double v = b[c];
        for(int k=c+1; k<n; k++) v -= A[c][k]*x[k];
        x[c] = v/A[c][c];
    }
    return true;
}

// inverse of a small symmetric positive definite matrix (Gauss-Jordan); false if singular
bool invertSmall(std::vector<std::vector<double>> A, std::vector<std::vector<double>> &inv)
{
    const int n = int(A.size());
    inv.assign(n, std::vector<double>(n, 0.0));
    for(int i=0; i<n; i++) inv[i][i] = 1.0;
    for(int c=0; c<n; c++){
        int p = c;
        for(int r=c+1; r<n; r++) if(std::fabs(A[r][c])>std::fabs(A[p][c])) p = r;
        if(!(std::fabs(A[p][c])>1e-300)) return false;
        std::swap(A[p], A[c]);
        std::swap(inv[p], inv[c]);
        const double d = A[c][c];
        for(int k=0; k<n; k++){ A[c][k] /= d; inv[c][k] /= d; }
        for(int r=0; r<n; r++){
            if(r==c) continue;
            const double q = A[r][c];
            for(int k=0; k<n; k++){ A[r][k] -= q*A[c][k]; inv[r][k] -= q*inv[c][k]; }
        }
    }
    return true;
}
} // namespace

bool MainWindow::analyseSolution()
{
    qPath = ui->lineEdit_4->text();
    if(!g_matrixValid || num<1 || logbin<8 || int(C.n_elem)!=num*logbin || M.n_rows!=C.n_elem || X.n_elem!=M.n_cols
            || g_layout.cols()!=int(M.n_cols)){
        reportError("Error 54: Analysis of the solution: there is no solution. Read data and Solve (or optimise) first.");
        return false;
    }
    const bool doFit = ui->checkBox_anaFit->isChecked(), doLines = ui->checkBox_anaLines->isChecked();
    if(!doFit && !doLines){
        reportError("Error 54: Analysis of the solution: nothing selected (Output tab, Analysis of the solution).");
        return false;
    }
    std::vector<double> t, phase;
    spectrumTimes(t, phase);
    const bool sb2 = g_layout.nB>0 && RV3.size()>=num;
    const arma::vec model = M*X;
    const int edge = std::max(0, g_edgeMargin);
    QStringList notes;
    const double c = dis::kSpeedOfLight;

    // ---- RV O-C and line strength per spectrum (components fixed)
    if(doFit){
        const QString name = ui->lineEdit_anaFit->text().trimmed();
        if(name.isEmpty()){ reportError("Error 54: Analysis: no file name for the RV O-C."); return false; }
        const dis::Layout &L = g_layout;
        const bool scaled = g_colScale.n_elem==X.n_elem;
        const arma::vec cs = scaled ? g_colScale : arma::vec(X.n_elem, arma::fill::ones);
        const arma::vec x = cs % X;                    // component spectra
        // contributions of A and B per row and their weights (sum of the interpolation entries)
        arma::vec aPart(M.n_rows, arma::fill::zeros), bPart(M.n_rows, arma::fill::zeros),
                  wA(M.n_rows, arma::fill::zeros), wB(M.n_rows, arma::fill::zeros);
        for(arma::sp_mat::const_iterator it = M.begin(); it!=M.end(); ++it){
            const arma::uword col = it.col(), row = it.row();
            const double v = *it;
            if(int(col)<L.nA){ aPart(row) += v*X(col); wA(row) += v*cs(col); }
            else if(int(col)<L.nA+L.nB){ bPart(row) += v*X(col); wB(row) += v*cs(col); }
        }
        const double levA = L.nA>0 ? dis::continuumLevel(x.subvec(0, L.nA-1)) : 0.0;
        const double levB = (sb2 && L.nB>0) ? dis::continuumLevel(x.subvec(L.nA, L.nA+L.nB-1)) : 0.0;
        const bool range = ui->checkBox_33->isChecked();
        const double lo = range ? std::log10(ui->doubleSpinBox_14->value()) : -1e300;
        const double hi = range ? std::log10(ui->doubleSpinBox_15->value()) : 1e300;
        const int np = sb2 ? 4 : 2;     // shift A, strength A (, shift B, strength B)
        std::ofstream out((qPath+"/"+name).toStdString().c_str());
        if(!out){ reportError("Error 52: Cannot write "+qPath+"/"+name+"."); return false; }
        out<<"# RV O-C and line strength per spectrum: each spectrum fitted with the component spectra fixed, "
           <<"own shift and own strength of the lines (1 = as in the component spectra) of A"<<(sb2 ? " and B" : "")
           <<"; errors from the scatter of the fit\n";
        out<<"# spectrum\ttime\tphase\tRV_A\tdRV_A\terr\tS_A\terr";
        if(sb2) out<<"\tRV_B\tdRV_B\terr\tS_B\terr";
        out<<"\tpixels\trms\n"<<setprecision(10);
        int nOut = 0;
        QStringList big;
        std::vector<double> ocA, ocB;
        std::vector<int> ocSpec;
        for(int j=0; j<num; j++){
            dis::keepAlive();
            const size_t r0 = size_t(j)*logbin;
            std::vector<double> la(logbin), lb(logbin), base(logbin), obs(logbin);
            std::vector<char> bad(logbin, 0), use(logbin, 0);
            for(int i=0; i<logbin; i++){
                const arma::uword r = r0+i;
                la[i] = aPart(r) - wA(r)*levA;
                lb[i] = bPart(r) - wB(r)*levB;
                base[i] = model(r) - aPart(r) - bPart(r) + wA(r)*levA + wB(r)*levB;
                obs[i] = C(r);
                bad[i] = rowMasked(r) || !std::isfinite(C(r)) || wA(r)==0.0;
            }
            for(int i=edge; i<logbin-edge; i++){
                const arma::uword r = r0+i;
                if(bad[i] || rowUncounted(r) || rowResExcluded(r)) continue;
                if(range && !((W(r)>lo) && (W(r)<hi))) continue;
                use[i] = 1;
            }
            std::vector<double> p(np, 0.0);
            p[1] = 1.0;
            if(sb2) p[3] = 1.0;
            std::vector<std::vector<double>> JtJ;
            double s2 = 0.0;
            int n = 0;
            bool ok = true;
            for(int iter=0; iter<15; iter++){
                JtJ.assign(np, std::vector<double>(np, 0.0));
                std::vector<double> Jtr(np, 0.0);
                s2 = 0.0;
                n = 0;
                for(int i=0; i<logbin; i++){
                    if(!use[i]) continue;
                    double ya, dya, yb = 0, dyb = 0;
                    if(!cubicAt(la, bad, i-p[0], ya, dya)) continue;
                    if(sb2 && !cubicAt(lb, bad, i-p[2], yb, dyb)) continue;
                    const double m = base[i] + p[1]*ya + (sb2 ? p[3]*yb : 0.0);
                    const double r = obs[i]-m;
                    double J[4] = {-p[1]*dya, ya, sb2 ? -p[3]*dyb : 0.0, yb};
                    for(int a=0; a<np; a++){
                        Jtr[a] += J[a]*r;
                        for(int b=0; b<np; b++) JtJ[a][b] += J[a]*J[b];
                    }
                    s2 += r*r;
                    ++n;
                }
                std::vector<double> d;
                if(n<=np+2 || !solveSmall(JtJ, Jtr, d)){ ok = false; break; }
                double step = 0.0;
                for(int a=0; a<np; a++){
                    // shifts limited to 2 pixels per step
                    if(a%2==0) d[a] = std::max(-2.0, std::min(2.0, d[a]));
                    p[a] += d[a];
                    step = std::max(step, std::fabs(d[a]));
                }
                if(step<1e-7) break;
            }
            std::vector<std::vector<double>> cov;
            if(ok) ok = invertSmall(JtJ, cov);
            const double sig2 = ok && n>np ? s2/(n-np) : 0.0;
            auto dv = [&](double px, double rv){ return (c+rv)*(std::pow(10.0, px*g_dlog)-1.0); };
            auto ev = [&](int a, double rv){ return ok ? std::fabs(dv(std::sqrt(std::max(0.0, sig2*cov[a][a])), rv)) : 0.0; };
            out<<(j+mini)<<"\t"<<(int(t.size())==num ? t[j] : 0.0)<<"\t"<<(int(phase.size())==num ? phase[j] : 0.0);
            const double eA = ev(0, RV1[j]);
            out<<"\t"<<RV1[j]<<"\t"<<(ok ? dv(p[0], RV1[j]) : 0.0)<<"\t"<<eA<<"\t"<<(ok ? p[1] : 0.0)<<"\t"
               <<(ok ? std::sqrt(std::max(0.0, sig2*cov[1][1])) : 0.0);
            if(sb2){
                out<<"\t"<<RV3[j]<<"\t"<<(ok ? dv(p[2], RV3[j]) : 0.0)<<"\t"<<ev(2, RV3[j])<<"\t"<<(ok ? p[3] : 0.0)<<"\t"
                   <<(ok ? std::sqrt(std::max(0.0, sig2*cov[3][3])) : 0.0);
            }
            out<<"\t"<<n<<"\t"<<std::sqrt(n>0 ? s2/n : 0.0)<<"\n";
            if(ok){
                ++nOut;
                ocA.push_back(dv(p[0], RV1[j]));
                ocB.push_back(sb2 ? dv(p[2], RV3[j]) : 0.0);
                ocSpec.push_back(j+mini);
            }
        }
        out.close();
        // robust scatter of the O-C (1.4826 MAD): the formal errors hold the noise of the spectrum
        // only, not that of the (fixed) component spectra
        auto rscat = [](std::vector<double> v){
            if(v.size()<3) return 0.0;
            std::nth_element(v.begin(), v.begin()+v.size()/2, v.end());
            const double med = v[v.size()/2];
            for(double &x : v) x = std::fabs(x-med);
            std::nth_element(v.begin(), v.begin()+v.size()/2, v.end());
            return 1.4826*v[v.size()/2];
        };
        // outliers: more than 5 robust scatters from the median O-C
        auto med = [](std::vector<double> v){
            if(v.empty()) return 0.0;
            std::nth_element(v.begin(), v.begin()+v.size()/2, v.end());
            return v[v.size()/2];
        };
        const double sA = rscat(ocA), sB = rscat(ocB), mA = med(ocA), mB = med(ocB);
        for(size_t k=0; k<ocA.size(); k++)
            if((sA>0 && std::fabs(ocA[k]-mA)>5*sA) || (sb2 && sB>0 && std::fabs(ocB[k]-mB)>5*sB))
                big << QString::number(ocSpec[k]);
        notes << QString("RV O-C and line strength of %1 spectra written to %2 (scatter of the O-C: A %3 km/s%4)")
                 .arg(nOut).arg(name).arg(rscat(ocA), 0, 'f', 3).arg(sb2 ? QString(", B %1 km/s").arg(rscat(ocB), 0, 'f', 3) : QString())
                 + (big.isEmpty() ? QString() : QString(" (O-C more than 5 times the scatter from the median: spectrum %1)").arg(big.join(", ")));
    }

    // ---- statistics per line window
    if(doLines){
        QString lname = ui->lineEdit_lines->text().trimmed();
        const QString oname = ui->lineEdit_anaLines->text().trimmed();
        if(lname.isEmpty() || oname.isEmpty()){ reportError("Error 54: Analysis: file name of the line windows or of the statistics missing."); return false; }
        const QString lfile = qPath+"/"+lname;
        if(!QFileInfo::exists(lfile)){
            if(lname!="lines.dat"){
                reportError("Error 54: Analysis: file of the line windows "+lfile+" does not exist.");
                return false;
            }
            std::ofstream d(lfile.toStdString().c_str());
            d<<dataio::kLineWindows;
            notes << "Default line windows written to lines.dat";
        }
        std::vector<std::pair<double, double>> win;
        std::vector<std::string> lab;
        std::string err;
        if(!dataio::readRanges(lfile.toStdString(), win, err, &lab) || win.empty()){
            reportError("Error 54: Analysis: line windows ("+lname+"): "+(err.empty() ? QString("no windows.") : QString::fromStdString(err)));
            return false;
        }
        std::ofstream out((qPath+"/"+oname).toStdString().c_str());
        if(!out){ reportError("Error 52: Cannot write "+qPath+"/"+oname+"."); return false; }
        out<<"# Statistics of observation - model per spectrum and line window ("<<lname.toStdString()
           <<"); EW = sum of (O-M) x pixel width [A], negative: more absorption than the model\n";
        out<<"# spectrum\ttime\tphase\tline\tfrom\tto\tpixels\tmean\trms\tEW\n"<<setprecision(10);
        int used = 0;
        QStringList outside;
        for(size_t w=0; w<win.size(); w++){
            const double l1 = std::log10(win[w].first), l2 = std::log10(win[w].second);
            const std::string label = lab.size()>w && !lab[w].empty() ? lab[w] : ("line_"+std::to_string(w+1));
            bool any = false;
            for(int j=0; j<num; j++){
                double s1 = 0, s2 = 0, ew = 0;
                int n = 0;
                for(int i=edge; i<logbin-edge; i++){
                    const arma::uword r = arma::uword(j)*logbin+i;
                    if(W(r)<l1 || W(r)>l2 || rowUncounted(r)) continue;
                    const double d = C(r)-model(r);
                    s1 += d;
                    s2 += d*d;
                    ew += d*std::pow(10.0, W(r))*std::log(10.0)*g_dlog;
                    ++n;
                }
                if(n==0) continue;
                any = true;
                out<<(j+mini)<<"\t"<<(int(t.size())==num ? t[j] : 0.0)<<"\t"<<(int(phase.size())==num ? phase[j] : 0.0)<<"\t"
                   <<label<<"\t"<<win[w].first<<"\t"<<win[w].second<<"\t"<<n<<"\t"<<s1/n<<"\t"<<std::sqrt(s2/n)<<"\t"<<ew<<"\n";
            }
            if(any) ++used; else outside << QString::fromStdString(label);
        }
        out.close();
        notes << QString("Statistics of %1 line windows written to %2").arg(used).arg(oname)
                 + (outside.isEmpty() ? QString() : QString(" (outside the data: %1)").arg(outside.join(", ")));
    }
    const QString m = "Analysis of the solution: "+notes.join("; ")+".";
    cout<<m.toStdString()<<endl;
    if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    return true;
}

void MainWindow::on_pushButton_stat_clicked()
{
    MainWindow::plotResidualStatistics();
}

void MainWindow::on_pushButton_6_clicked()
{
    MainWindow::plotResidualStatistics();
    if((runow==0) & (optfratios==0)){       //plot only of in GUI-mode
        ui->customPlot->clearGraphs();
        ui->customPlot_2->clearGraphs();

        double xs1, xs2, ys1, ys2;
        string zeile, one, two;
        // with "continuum-normalised" (Output tab) the normalised files are plotted
        auto plotFile = [&](const QString &name){
            if(ui->checkBox_norm->isChecked() && !name.trimmed().isEmpty()
               && QFile::exists(qPath+"/"+normName(name))) return normName(name);
            return name;
        };
        QString plot1=plotFile(ui->lineEdit_5->text());
        if(plot1.trimmed().isEmpty() || (!ui->checkBox_3->isChecked() && ui->lineEdit_8->text().trimmed().isEmpty())){
            QMessageBox::information(this, "Error", "No output file name for the components (Output tab).");
            return;
        }
        string plot11 = plot1.toUtf8().constData();
        std::ostringstream datNameStream(plot11);
        datNameStream<<path<<"/"<<plot11;
        std::string datName = datNameStream.str();

        QFile checkfile(datName.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+plot1+" does not exist!");
            return;
        }

        ifstream toplot1(datName.c_str());
        if(toplot1.peek() == std::ifstream::traits_type::eof()){
            qDebug()<<"The file "<<checkfile.fileName()<<" is empty.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+plot1+" is empty.");
            return;
        }
        QString plot2=plotFile(ui->lineEdit_8->text());
        string plot12 = plot2.toUtf8().constData();
        std::ostringstream dat2NameStream(plot12);
        dat2NameStream<<path<<"/"<<plot12;
        std::string dat2Name = dat2NameStream.str();

        QFile checkfile2(dat2Name.c_str());

        if(!checkfile2.exists()){
            qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+plot2+" does not exist!");
            return;
        }
        ifstream toplot2(dat2Name.c_str());

        QString qError=ui->lineEdit_20->text();
        string errorf=qError.toUtf8().constData();
        std::ostringstream dat4NameStream(errorf);
        dat4NameStream<<path<<"/"<<errorf;
        std::string dat4Name = dat4NameStream.str();  


        QFile checkfile4(dat4Name.c_str());

        if(!checkfile4.exists()){
            qDebug()<<"The file "<<checkfile4.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+qError+" does not exist!");
            return;
        }
        ifstream errors(dat4Name.c_str());

        int number_of_lines=0;

        while(std::getline(toplot1, zeile)){
            ++number_of_lines;
            dis::keepAlive();
        }

        toplot1.clear();
        toplot1.seekg(0, ios::beg);

        QVector<double> a(number_of_lines), b(number_of_lines);

        for (int i=0; i<number_of_lines; i++){
            dis::keepAlive();
            toplot1 >> one >>two;
            istringstream ist(one);
            ist >> a[i];
            istringstream ist2(two);
            ist2 >> b[i];
        }
        toplot1.close();

        xs1=a[0];
        xs2=a[number_of_lines-1];
        for(int i=0; i<number_of_lines; i++){
            if(a[i]<xs1){
                xs1=a[i];
            }
        }

        for(int i=0; i<number_of_lines; i++){
            if(a[i]>xs2){
                xs2=a[i];
            }
        }


        ys1=b[0];
        ys2=b[number_of_lines-1];
        for(int i=0; i<number_of_lines; i++){
            if(b[i]<ys1){
                ys1=b[i];
            }
        }
        for(int i=0; i<number_of_lines; i++){
            if(b[i]>ys2){
                ys2=b[i];
            }
        }


        number_of_lines=0;

        while(std::getline(toplot2, zeile)){
            ++number_of_lines;
            dis::keepAlive();
        }

        toplot2.clear();
        toplot2.seekg(0, ios::beg);

        QVector<double> c(number_of_lines), d(number_of_lines);

        for (int i=0; i<number_of_lines; i++){
            dis::keepAlive();
            toplot2 >> one >>two;
            istringstream ist(one);
            ist >> c[i];
            istringstream ist2(two);
            ist2 >> d[i];
        }
        toplot2.close();

        for(int i=0; i<number_of_lines; i++){
            if(c[i]<xs1){
                xs1=c[i];
            }
        }

        for(int i=0; i<number_of_lines; i++){
            if(c[i]>xs2){
                xs2=c[i];
            }
        }

        for(int i=0; i<number_of_lines; i++){
            if(d[i]<ys1){
                ys1=d[i];
            }
        }
        for(int i=0; i<number_of_lines; i++){
            if(d[i]>ys2){
                ys2=d[i];
            }
        }

        number_of_lines=0;

        while(std::getline(errors, zeile)){
            ++number_of_lines;
            dis::keepAlive();
        }

        errors.clear();
        errors.seekg(0, ios::beg);

        QVector<double> e(number_of_lines), f(number_of_lines);

        for (int i=0; i<number_of_lines; i++){
            dis::keepAlive();
            errors >> one >>two;
            istringstream ist(one);
            ist >> e[i];
            istringstream ist2(two);
            ist2 >> f[i];
        }
        errors.close();

        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(a, b);
        ui->customPlot->graph(0)->rescaleAxes();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setData(c, d);
        ui->customPlot->graph(1)->rescaleAxes(true);
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setData(e, f);
        ui->customPlot->graph(2)->setPen(QPen(Qt::black));
        {
            double lo = 1e300, hi = -1e300;
            innerYRange(a, b, plotTrim(a.size(), g_edgeMargin), lo, hi);
            innerYRange(c, d, plotTrim(c.size(), g_edgeMargin), lo, hi);
            innerYRange(e, f, plotTrim(e.size(), g_edgeMargin), lo, hi);
            setYRange(ui->customPlot, lo, hi);
        }
        ui->customPlot->replot();

        //telluric lines
        if(ui->checkBox_8->isChecked()){

          QString plot5=ui->lineEdit_10->text();
          string plot15 = plot5.toUtf8().constData();
          std::ostringstream dat5NameStream(plot15);
          dat5NameStream<<path<<"/"<<plot15;
          std::string dat5Name = dat5NameStream.str();
          ifstream toplot5(dat5Name.c_str());

          QFile checkfile5(dat5Name.c_str());

          if(!checkfile5.exists()){
              qDebug()<<"The file "<<checkfile5.fileName()<<" does not exist.";
              QMessageBox::information(this, "Error", "File "+qPath+"/"+plot5+" does not exist!");
             return;
          }

          number_of_lines=0;

          while(std::getline(toplot5, zeile)){
             ++number_of_lines;
             dis::keepAlive();
         }

          toplot5.clear();
          toplot5.seekg(0, ios::beg);

          a.resize(number_of_lines);
          b.resize(number_of_lines);

          for (int i=0; i<number_of_lines; i++){
            dis::keepAlive();
            toplot5 >> one >>two;
            istringstream ist(one);
            ist >> a[i];
            istringstream ist2(two);
            ist2 >> b[i];
          }
          toplot5.close();

          ui->customPlot_2->addGraph();
          ui->customPlot_2->graph(0)->setData(a, b);
          ui->customPlot_2->graph(0)->setPen(QPen(Qt::red));
          ui->customPlot_2->addGraph();
          ui->customPlot_2->xAxis->setRange(xs1, xs2);
          {
              double lo = 1e300, hi = -1e300;
              innerYRange(a, b, plotTrim(a.size(), g_edgeMargin), lo, hi);
              setYRange(ui->customPlot_2, lo, hi);
          }
          ui->customPlot_2->replot();

      }
    }

    // plot files flux optimisation
    else{
        if((runow==0) & (optfratios==1)){
            ui->customPlot->clearGraphs();
            ui->customPlot_2->clearGraphs();

            double xs1, xs2, ys1, ys2;
            string zeile, one, two;
            QString plot1="frcompa.dat";
            string plot11 = plot1.toUtf8().constData();
            std::ostringstream datNameStream(plot11);
            datNameStream<<path<<"/"<<plot11;
            std::string datName = datNameStream.str();

            QFile checkfile(datName.c_str());

            if(!checkfile.exists()){
                qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+plot1+" does not exist!");
                return;
            }
            ifstream toplot1(datName.c_str());

            QString plot2="frcompb.dat";
            string plot12 = plot2.toUtf8().constData();
            std::ostringstream dat2NameStream(plot12);
            dat2NameStream<<path<<"/"<<plot12;
            std::string dat2Name = dat2NameStream.str();

            QFile checkfile2(dat2Name.c_str());

            if(!checkfile2.exists()){
                qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+plot2+" does not exist!");
                return;
            }
            ifstream toplot2(dat2Name.c_str());

            QString qError="frerror.dat";
            string errorf=qError.toUtf8().constData();
            std::ostringstream dat4NameStream(errorf);
            dat4NameStream<<path<<"/"<<errorf;
            std::string dat4Name = dat4NameStream.str();

            QFile checkfile4(dat4Name.c_str());

            if(!checkfile4.exists()){
                qDebug()<<"The file "<<checkfile4.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+qError+" does not exist!");
                return;
            }
            ifstream errors(dat4Name.c_str());

            int number_of_lines=0;

            while(std::getline(toplot1, zeile)){
                ++number_of_lines;
                dis::keepAlive();
            }

            toplot1.clear();
            toplot1.seekg(0, ios::beg);

            QVector<double> a(number_of_lines), b(number_of_lines);

            for (int i=0; i<number_of_lines; i++){
                dis::keepAlive();
                toplot1 >> one >>two;
                istringstream ist(one);
                ist >> a[i];
                istringstream ist2(two);
                ist2 >> b[i];
            }
            toplot1.close();

            xs1=a[0];
            xs2=a[number_of_lines-1];
            for(int i=0; i<number_of_lines; i++){
                if(a[i]<xs1){
                    xs1=a[i];
                }
            }

            for(int i=0; i<number_of_lines; i++){
                if(a[i]>xs2){
                    xs2=a[i];
                }
            }


            ys1=b[0];
            ys2=b[number_of_lines-1];
            for(int i=0; i<number_of_lines; i++){
                if(b[i]<ys1){
                    ys1=b[i];
                }
            }
            for(int i=0; i<number_of_lines; i++){
                if(b[i]>ys2){
                    ys2=b[i];
                }
            }


            number_of_lines=0;

            while(std::getline(toplot2, zeile)){
                ++number_of_lines;
                dis::keepAlive();
            }

            toplot2.clear();
            toplot2.seekg(0, ios::beg);

            QVector<double> c(number_of_lines), d(number_of_lines);

            for (int i=0; i<number_of_lines; i++){
                dis::keepAlive();
                toplot2 >> one >>two;
                istringstream ist(one);
                ist >> c[i];
                istringstream ist2(two);
                ist2 >> d[i];
            }
            toplot2.close();

            for(int i=0; i<number_of_lines; i++){
                if(c[i]<xs1){
                    xs1=c[i];
                }
            }

            for(int i=0; i<number_of_lines; i++){
                if(c[i]>xs2){
                    xs2=c[i];
                }
            }

            for(int i=0; i<number_of_lines; i++){
                if(d[i]<ys1){
                    ys1=d[i];
                }
            }
            for(int i=0; i<number_of_lines; i++){
                if(d[i]>ys2){
                    ys2=d[i];
                }
            }

            number_of_lines=0;

            while(std::getline(errors, zeile)){
           ++number_of_lines;
           dis::keepAlive();
       }

            errors.clear();
            errors.seekg(0, ios::beg);

            QVector<double> e(number_of_lines), f(number_of_lines);

            for (int i=0; i<number_of_lines; i++){
                dis::keepAlive();
                errors >> one >>two;
                istringstream ist(one);
                ist >> e[i];
                istringstream ist2(two);
                ist2 >> f[i];
            }
            errors.close();

            ui->customPlot->addGraph();
            ui->customPlot->graph(0)->setData(a, b);
            ui->customPlot->graph(0)->rescaleAxes();
            ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
            ui->customPlot->addGraph();
            ui->customPlot->graph(1)->setData(c, d);
            ui->customPlot->graph(1)->rescaleAxes(true);
            ui->customPlot->graph(1)->setPen(QPen(Qt::red));
            ui->customPlot->addGraph();
            ui->customPlot->graph(2)->setData(e, f);
            ui->customPlot->graph(2)->setPen(QPen(Qt::black));
            {
                double lo = 1e300, hi = -1e300;
                innerYRange(a, b, plotTrim(a.size(), g_edgeMargin), lo, hi);
                innerYRange(c, d, plotTrim(c.size(), g_edgeMargin), lo, hi);
                innerYRange(e, f, plotTrim(e.size(), g_edgeMargin), lo, hi);
                setYRange(ui->customPlot, lo, hi);
            }
            ui->customPlot->replot();

        }
        else{
            //
        }
    }

}

void MainWindow::on_spinBox_3_valueChanged()
{
    zaehler=ui->spinBox_3->value();
}

void MainWindow::on_checkBox_9_clicked()
{
    if(ui->checkBox_9->isChecked()){
        ui->checkBox_11->setChecked(false);
        ui->checkBox_12->setChecked(false);
    }
    else{
        ui->checkBox_11->setChecked(true);
        ui->checkBox_12->setChecked(false);
    }
}

void MainWindow::on_checkBox_11_clicked()
{
    if(ui->checkBox_11->isChecked()){
        ui->checkBox_9->setChecked(false);
        ui->checkBox_12->setChecked(false);
    }
    else{
        ui->checkBox_9->setChecked(true);
        ui->checkBox_12->setChecked(false);
    }
}

void MainWindow::on_doubleSpinBox_4_valueChanged()
{
    fluxratio=ui->doubleSpinBox_4->value();
}

void MainWindow::on_pushButton_7_clicked()
{
    abortt=1;
}

void MainWindow::on_checkBox_12_clicked()
{
    if(ui->checkBox_12->isChecked()){
        ui->checkBox_11->setChecked(false);
        ui->checkBox_9->setChecked(false);
    }
    else{
        ui->checkBox_11->setChecked(true);
        ui->checkBox_9->setChecked(false);
    }
}

void MainWindow::on_checkBox_4_clicked()
{
    ui->checkBox_3->setChecked(false);
    ui->checkBox_34->setChecked(false);
    ui->checkBox_8->setEnabled(true);
}

void MainWindow::on_checkBox_3_clicked()
{
    ui->checkBox_4->setChecked(false);
    ui->checkBox_34->setChecked(false);
    ui->checkBox_8->setEnabled(true);
}

void MainWindow::on_checkBox_14_clicked()
{
    ui->checkBox_15->setChecked(false);
}

void MainWindow::on_checkBox_15_clicked()
{
    ui->checkBox_14->setChecked(false);
}

void MainWindow::on_actionEditor_triggered()
{
    int hgh = ui->spinBox_2->value() - ui->spinBox->value() + 1;
    qEdit =new Edit(this);
    qEdit->seData(ui->lineEdit_4->text(), hgh, ui->doubleSpinBox_4->value());
    qEdit->setOrbitFile(qOrbitFile);
    qEdit->show();
}


void MainWindow::on_checkBox_16_clicked()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
}

//***********************************
// RVs for A
//**********************************
void MainWindow::VAmplitudeA()
{
    RVT0 = orbele[5];
    RVP = orbele[0];
    RVe = orbele[1];

    for(int m=0; m<num; m++){
        bool ok = true;
        RV1[m] = orbit::radialVelocity(otimes[m], orbele[0], orbele[1], orbele[2], orbele[4],
                                       orbele[5], orbele[6], &ok);
        if(!ok) RVCalcCheck=1;
    }
}

//**********************************
// Auto Stop and Reinitiate based on the progress in chi^2.
// sigma = 1/(S/N) from the GUI, otherwise sigma^2 = residuum^2 / degrees of freedom.
// A run has converged when chi^2 = S^2/sigma^2 has dropped by less than Delta-chi^2
// over the window. Then:
//   Reinitiate on:  restart; with Auto Stop, stop if the run gained less than
//                   Delta-chi^2 over the previous run (restart n vs. n-1)
//   Reinitiate off: with Auto Stop, stop; otherwise continue up to the iteration limit
// Only for the residuum "Mean" (sum of squares); "Peak" has no chi^2.
//**********************************
// sigma^2 for the convergence data (4th column of optdata.dat), frozen when the line is
// written: the known one (S/N), else the current estimate, else residuum^2 / degrees of freedom
double MainWindow::sigma2Now(double residuum) const
{
    const double known = knownSigma2();
    if(known>0) return known;
    if(std::isfinite(g_sigma2) && g_sigma2>0) return g_sigma2;
    return residuum*residuum/std::max(1, g_lastResBins - g_lastResCols);
}

double MainWindow::knownSigma2() const
{
    if(!g_specW.empty() && g_sigmaBar2>0) return g_sigmaBar2;   // from the S/N file
    const double snr = ui->doubleSpinBox_16->value();
    return snr>0 ? 1.0/(snr*snr) : 0.0;                          // 0: estimate from the residuum
}

void MainWindow::resetRunState()
{
    g_autoSpecW.clear();      // S/N "auto": estimate anew for this optimisation
    g_autoSigmaBar2 = 0.0;
    g_prevRunBestS = std::numeric_limits<double>::quiet_NaN();
    g_itersUsed = 0;
}

RunAction MainWindow::convergenceStep(nm::ProgressMonitor &mon, double bestS, int nParams, std::ofstream &log,
                                      bool allowReinit)
{
    if(!ui->checkBox_14->isChecked() || !std::isfinite(bestS) || bestS>=orbit::kInvalidPenalty){
        return RunAction::Continue;
    }
    const double known = knownSigma2();
    const int dof = g_lastResBins - g_lastResCols - nParams;
    const double s2 = known>0 ? known : (dof>0 ? bestS*bestS/dof : 0.0);
    if(!(s2>0)) return RunAction::Continue;
    g_sigma2 = s2;
    mon.add(bestS*bestS);
    const double dchi = ui->doubleSpinBox_17->value();
    if(!mon.converged(dchi*s2)) return RunAction::Continue;

    std::ostringstream m;
    m<<setprecision(4)<<"Converged: chi^2 improved by "<<mon.gain()/s2<<" < "<<dchi<<" over the last "
     <<mon.window()<<" iterations (sigma = "<<std::sqrt(s2)<<(known>0 ? ", from S/N" : ", estimated")<<").";

    if(allowReinit && ui->checkBox_32->isChecked()){      // Reinitiate
        // Each restart starts from the best point of the previous run. The optimisation ends
        // when a run converges without having improved on that point (as in version 1.11:
        // best value still that of the first evaluation); any improvement starts another run.
        if(ui->checkBox_23->isChecked() && std::isfinite(g_prevRunBestS)){
            const double runGain = (g_prevRunBestS*g_prevRunBestS - bestS*bestS)/s2;
            const bool improved = bestS < g_prevRunBestS*(1.0-1e-12);
            m<<" Gain of this run against the previous one: "<<runGain
             <<(improved ? " (improved: restart from the new best)." : " (no improvement: stop).");
            if(!improved){
                log<<m.str()<<endl;
                cout<<m.str()<<endl;
                ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
                return RunAction::Done;
            }
        }
        g_prevRunBestS = bestS;
        log<<m.str()<<endl;
        cout<<m.str()<<endl;
        ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
        return RunAction::Reinit;
    }
    if(ui->checkBox_23->isChecked()){      // Auto Stop without Reinitiate
        log<<m.str()<<endl;
        cout<<m.str()<<endl;
        ui->plainTextEdit_2->appendPlainText(QString::fromStdString(m.str()));
        return RunAction::Done;
    }
    return RunAction::Continue;            // without Auto Stop up to the iteration limit
}

//**********************************
// RVs for B (argument of periastron omega + pi)
//**********************************
void MainWindow::VAmplitudeB()
{
    RVT0 = orbele[5];
    RVP = orbele[0];
    RVe = orbele[1];

    for(int m=0; m<num; m++){
        bool ok = true;
        RV3[m] = orbit::radialVelocity(otimes[m], orbele[0], orbele[1], orbele[3], orbele[4],
                                       orbele[5], orbele[6]+M_PI, &ok);
        if(!ok) RVCalcCheck=1;
    }
}

//**********************************
// Residuum for orbital elements: invalid elements (P<=0, e outside [0,1))
// get a penalty residuum instead of NaN.
//**********************************
double MainWindow::evaluateOrbit()
{
    if(!orbit::elementsValid(orbele[0], orbele[1])){
        return orbit::kInvalidPenalty;
    }
    MainWindow::VAmplitudeA();
    MainWindow::VAmplitudeB();
    MainWindow::ConstructMatrix();
    return MainWindow::DivideConquer();
}

void MainWindow::on_actionArithmetic_triggered()
{
    qArith =new Arithmetic(this);
    qArith->seData(ui->lineEdit_4->text(), ui->lineEdit_13->text(), ui->lineEdit_26->text());
    qArith->show();
}

void MainWindow::on_actionManual_triggered()
{
    QString manual = findResource("Spectangular_Manual.pdf");
    if(manual.isEmpty()) manual = findResource("Spectangular_Manual_2020.pdf");
    if(manual.isEmpty()) manual = findResource("Manual.pdf");
    if(manual.isEmpty()){
        reportError("Manual not found. Searched: "+resourceSearchPath());
        error=0;
        return;
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(manual));
}

//*******************************************************
// Estimate error
//*******************************************************
void MainWindow::ErrorEstimation()
{
    // Error estimate for the orbital elements from the final simplex
    // (errorestimate.h; Nelder & Mead 1965, appendix).
    if(!ui->checkBox_16->isChecked()){
        reportError("Error 34: Error estimation is available for the optimisation of orbital elements only.");
        error=0;
        return;
    }
    if(!ui->checkBox_14->isChecked()){
        reportError("Error 35: Error estimation needs the residuum mode 'Mean' (sum of squares).");
        error=0;
        return;
    }

    const char *names[7] = {"P", "e", "K_A", "K_B", "gamma", "T_peri", "omega"};
    int pvf[7] = {!ui->checkBox_18->isChecked(), !ui->checkBox_17->isChecked(), !ui->checkBox_31->isChecked(),
                  !ui->checkBox_36->isChecked(), !ui->checkBox_37->isChecked(), !ui->checkBox_35->isChecked(),
                  !ui->checkBox_38->isChecked()};
    int n=0;
    for(int e=0; e<7; e++) if(pvf[e]==0) ++n;
    if(n<1){
        reportError("Error 37: All orbital elements are locked.");
        error=0;
        return;
    }

    std::vector<std::vector<double>> yv, pv;
    std::string ioErr;
    QString fVal = qPath+"/"+ui->lineEdit_16->text(), fMat = qPath+"/"+ui->lineEdit_17->text();
    if(!dataio::readTable(fVal.toStdString(), 1, yv, ioErr) || !dataio::readTable(fMat.toStdString(), 1, pv, ioErr)){
        reportError("Error 36: "+QString::fromStdString(ioErr));
        error=0;
        return;
    }
    if(int(yv.size())!=n+1 || int(pv.size())!=n*(n+1)){
        reportError("Error 37: The optimisation data "+fVal+" / "+fMat
                    +" does not match the orbital elements selected in the main window.");
        error=0;
        return;
    }

    MainWindow::read_data();   // spectra, times and fixed elements
    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        return;
    }

    arma::mat P(n+1, n);
    std::vector<double> S(n+1);
    for(int i=0; i<n+1; i++){
        S[i] = yv[i][0]*yv[i][0];
        for(int j=0; j<n; j++) P(i,j) = pv[i*n+j][0];
    }
    auto setElements = [&](const arma::rowvec &x){
        int pc=0;
        for(int e=0; e<7; e++){
            if(pvf[e]==0){
                orbele[e]=x(pc);
                ++pc;
            }
        }
    };
    auto f = [&](const arma::rowvec &x){
        setElements(x);
        double r = MainWindow::evaluateOrbit();
        qApp->processEvents(QEventLoop::AllEvents);
        return r*r;
    };

    int best = int(std::min_element(S.begin(), S.end()) - S.begin());
    const arma::rowvec x0 = P.row(best);
    r1 = sqrt(S[best]);   // output only on a real improvement
    auto restoreBest = [&](){
        setElements(x0);
        MainWindow::VAmplitudeA();
        MainWindow::VAmplitudeB();
    };
    auto aborted = [&](){
        abortt=0;
        restoreBest();
        if(runow==0) QMessageBox::information(this, "Abort", "Error estimation aborted.");
    };

    // sigma^2 from the residuum: degrees of freedom = bins - unknowns of the spectra - elements
    setElements(x0);
    double rb = MainWindow::evaluateOrbit();
    int dof = g_lastResBins - g_lastResCols - n;
    // sigma^2: from the given S/N (sigma = 1/(S/N)), otherwise from the residuum
    const double known = knownSigma2();
    double sigma2 = known>0 ? known : (dof>0 ? rb*rb/dof : std::numeric_limits<double>::quiet_NaN());
    if(!(sigma2>0)){
        restoreBest();
        reportError("Error 38: Not enough data for an error estimation (degrees of freedom: "+QString::number(dof)+").");
        error=0;
        return;
    }

    // Monte Carlo (parametric bootstrap): noise with the scatter of the residuals of each
    // spectrum is added to the best model, and the free elements are refitted with
    // Levenberg-Marquardt from the best point for every realisation. The errors are the
    // scatter of the fits; they include the second-order effects that the curvature misses
    // at moderate S/N.
    if(ui->comboBox_err->currentIndex()==1){
        const int nReal = ui->spinBox_mc->value();
        setElements(x0);
        rb = MainWindow::evaluateOrbit();
        const arma::vec model = M*X;
        const arma::vec C0 = C;
        // noise per spectrum: from the S/N (file or field) or from the residuals of the
        // spectrum, corrected for the degrees of freedom of the fit
        std::vector<double> sig(num, 0.0);
        const int edge = std::max(0, g_edgeMargin);
        const double dofFac = (g_lastResBins>g_lastResCols+n) ? double(g_lastResBins)/double(g_lastResBins-g_lastResCols-n) : 1.0;
        for(int j=0; j<num; j++){
            if(known>0){
                const double sb = std::sqrt(known);
                sig[j] = g_specW.empty() ? sb : sb/g_specW[j];
                continue;
            }
            double q = 0.0;
            int cnt = 0;
            for(int i=edge; i<logbin-edge; i++){
                const double d = C0(j*logbin+i) - model(j*logbin+i);
                q += d*d;
                ++cnt;
            }
            sig[j] = cnt>0 ? std::sqrt(q/cnt*dofFac) : 0.0;
        }
        std::vector<int> elIdx;
        for(int e=0; e<7; e++) if(pvf[e]==0) elIdx.push_back(e);
        // finite-difference steps as in the Levenberg-Marquardt optimisation (1e-4 of the
        // default initial simplex steps)
        arma::vec h(n);
        for(int k=0; k<n; k++){
            const int e = elIdx[k];
            const double P0 = orbele[0];
            const double d[7] = {P0/10, std::max(orbele[1]/10, 0.005), 3*dv, 3*dv, 3*dv, P0/10, M_PI/20};
            h(k) = 1e-4*d[e];
        }
        auto resid = [&](const arma::vec &x, arma::vec &r, bool){
            for(int k=0; k<n; k++) orbele[elIdx[k]] = x(k);
            if(!orbit::elementsValid(orbele[0], orbele[1])) return false;
            RVCalcCheck = 0;
            MainWindow::VAmplitudeA();
            MainWindow::VAmplitudeB();
            if(RVCalcCheck!=0) return false;
            if(!MainWindow::buildDesignMatrix(false)) return false;
            if(!MainWindow::solveSystem(MainWindow::currentSolver())) return false;
            r = MainWindow::residualVector(M*X);
            eval++;
            ui->spinBox_5->setValue(eval);
            qApp->processEvents(QEventLoop::AllEvents);
            return r.is_finite();
        };
        lm::Options opt;
        opt.h = h;
        opt.maxIter = 50;
        opt.tolS = 0.002*sigma2;
        opt.relTolS = 1e-12;
        arma::vec xs = x0.t();
        arma::mat fits(0, n);
        ui->plainTextEdit_2->appendPlainText(QString("Monte Carlo error estimation: %1 realisations").arg(nReal));
        std::mt19937_64 gen(20261101);
        std::normal_distribution<double> norm(0.0, 1.0);
        int failed = 0;
        for(int k=0; k<nReal; k++){
            for(int j=0; j<num; j++)
                for(int i=0; i<logbin; i++)
                    C(j*logbin+i) = model(j*logbin+i) + sig[j]*norm(gen);
            lm::Result rk = lm::minimize(xs, resid, opt, [&]{ return abortt==1; });
            if(rk.aborted || abortt==1){
                C = C0;
                aborted();
                return;
            }
            if(rk.ok && rk.converged) fits.insert_rows(fits.n_rows, rk.x.t());
            else ++failed;
            ui->spinBox_4->setValue(k+1);
            qApp->processEvents(QEventLoop::AllEvents);
        }
        C = C0;
        restoreBest();
        MainWindow::evaluateOrbit();
        if(fits.n_rows<3){
            reportError("Error 38: Monte Carlo: fewer than three realisations converged.");
            error=0;
            return;
        }
        const arma::rowvec mean = arma::mean(fits, 0);
        const arma::mat cov = arma::cov(fits);
        const arma::vec err = arma::sqrt(cov.diag());
        arma::mat corr = cov;
        for(arma::uword a=0; a<corr.n_rows; a++)
            for(arma::uword b=0; b<corr.n_cols; b++)
                corr(a,b) = cov(a,b)/(err(a)*err(b));
        QString qErr = ui->lineEdit_24->text();
        ofstream Err((qPath+"/"+qErr).toStdString().c_str());
        Err<<setprecision(10);
        Err<<"# Error estimation by Monte Carlo: "<<fits.n_rows<<" of "<<nReal<<" realisations converged"
           <<(failed>0 ? " ("+std::to_string(failed)+" not)" : std::string())<<endl;
        if(pvf[4]==0) Err<<"# "<<kGammaNote<<endl;
        Err<<"# noise per spectrum "<<(known>0 ? "from the S/N" : "from the residuals")<<": sigma ";
        for(int j=0; j<num; j++) Err<<sig[j]<<(j+1<num ? " " : "\n");
        Err<<"# Element\tbest value\tstandard error\tmean of the realisations"<<endl;
        int pc=0;
        for(int e=0; e<7; e++){
            if(pvf[e]==0){
                Err<<names[e]<<"\t"<<x0(pc)<<"\t"<<err(pc)<<"\t"<<mean(pc)<<endl;
                ui->plainTextEdit_2->appendPlainText(QString(names[e])+" = "+QString::number(x0(pc), 'g', 10)
                                                     +" +/- "+QString::number(err(pc), 'g', 3)+" (Monte Carlo)");
                ++pc;
            }
            else{
                Err<<names[e]<<"\t"<<orbele[e]<<"\tfixed"<<endl;
            }
        }
        Err<<endl<<"Covariance:"<<endl<<cov<<endl;
        Err<<"Correlation:"<<endl<<corr<<endl;
        Err<<"Realisations (free elements):"<<endl<<fits<<endl;
        Err.close();
        ui->plainTextEdit_2->appendPlainText("Data written to file "+qErr);
        return;
    }

    // After full convergence the final simplex is degenerate. Therefore a new simplex is
    // spanned around the best point whose vertices increase S by about sigma^2
    // (Nelder & Mead 1965, appendix). Initial steps: extent of the final simplex.
    arma::vec h0 = (arma::max(P, 0) - arma::min(P, 0)).t();
    errest::ProbeResult probe = errest::probeSimplex(x0, rb*rb, h0, sigma2, f, [&]{ return abortt==1; },
                                                     0.5*orbit::kInvalidPenalty*orbit::kInvalidPenalty);
    if(probe.aborted){
        aborted();
        return;
    }
    if(!probe.ok){
        restoreBest();
        QString undetermined;
        if(!probe.undetermined.empty()){
            int pc=0;
            for(int e=0; e<7; e++){
                if(pvf[e]==0){
                    if(std::find(probe.undetermined.begin(), probe.undetermined.end(), pc)!=probe.undetermined.end())
                        undetermined += QString(" ")+names[e];
                    ++pc;
                }
            }
            reportError("Error 38: The residuum does not depend measurably on"+undetermined
                        +". Lock these elements for the error estimation.");
        }
        else{
            reportError("Error 38: "+QString::fromStdString(probe.message));
        }
        error=0;
        return;
    }

    errest::Result res1 = errest::quadraticFit(probe.P, probe.S, f, [&]{ return abortt==1; });
    if(res1.aborted){
        aborted();
        return;
    }
    if(res1.B.n_rows!=arma::uword(n)){
        restoreBest();
        reportError("Error 38: "+QString::fromStdString(res1.message));
        error=0;
        return;
    }
    // Second pass along the principal axes of the first model: measures the curvature
    // also along strongly correlated valleys on the scale of one standard deviation.
    errest::ProbeResult probe2 = errest::probePrincipal(x0, rb*rb, res1, sigma2, f, [&]{ return abortt==1; },
                                                        0.5*orbit::kInvalidPenalty*orbit::kInvalidPenalty);
    if(probe2.aborted){
        aborted();
        return;
    }
    errest::Result res;
    QString passNote;
    if(probe2.ok){
        res = errest::quadraticFit(probe2.P, probe2.S, f, [&]{ return abortt==1; });
        if(res.aborted){
            aborted();
            return;
        }
    }
    if(!res.ok){
        if(!res1.ok){
            restoreBest();
            reportError("Error 38: "+QString::fromStdString(res.message.empty() ? res1.message : res.message));
            error=0;
            return;
        }
        passNote = "second pass failed ("+QString::fromStdString(probe2.message+res.message)
                   +"); result of the axis-parallel pass";
        res = res1;   // principal-axes pass failed: result of the first pass
        ui->plainTextEdit_2->appendPlainText("Error estimation: "+passNote);
    }

    arma::mat cov = sigma2*res.covUnit;
    arma::vec err = arma::sqrt(arma::abs(cov.diag()));
    arma::mat corr = cov / (err*err.t());

    QString qErr = ui->lineEdit_24->text();
    ofstream Err((qPath+"/"+qErr).toStdString().c_str());
    Err<<setprecision(10);
    Err<<"# Error estimation of the orbital elements (Nelder & Mead 1965, appendix)"<<endl;
    if(pvf[4]==0) Err<<"# "<<kGammaNote<<endl;
    if(known>0){
        Err<<"# sigma^2 from S/N"<<(g_specW.empty() ? "" : " (per spectrum, weighted)")<<" = "<<sigma2
           <<" (from the residuum it would be "<<rb*rb<<" / "<<dof<<" = "<<(dof>0 ? rb*rb/dof : 0.0)<<")"<<endl;
    }
    else{
        Err<<"# sigma^2 = residuum^2 / degrees of freedom = "<<rb*rb<<" / "<<dof<<" = "<<sigma2<<endl;
    }
    Err<<"# new simplex around the best point, steps:";
    for(arma::uword k=0; k<probe.steps.n_elem; k++) Err<<" "<<probe.steps(k);
    Err<<endl<<"# second pass along the principal axes, multiples of the model direction:";
    for(arma::uword k=0; k<probe2.steps.n_elem; k++) Err<<" "<<probe2.steps(k);
    Err<<" ("<<probe.evaluations+res1.evaluations+probe2.evaluations+(passNote.isEmpty() ? res.evaluations : 0)
       <<" evaluations)"<<endl;
    if(!passNote.isEmpty()) Err<<"# Note: "<<passNote.toStdString()<<endl;
    Err<<"# Element\tbest value\tstandard error\tminimum of the quadratic model"<<endl;
    int pc=0;
    for(int e=0; e<7; e++){
        if(pvf[e]==0){
            Err<<names[e]<<"\t"<<x0(pc)<<"\t"<<err(pc)<<"\t"<<res.minimum(pc)<<endl;
            ui->plainTextEdit_2->appendPlainText(QString(names[e])+" = "+QString::number(x0(pc), 'g', 10)
                                                 +" +/- "+QString::number(err(pc), 'g', 3));
            ++pc;
        }
        else{
            Err<<names[e]<<"\t"<<orbele[e]<<"\tfixed"<<endl;
        }
    }
    Err<<endl<<"Covariance:"<<endl<<cov<<endl;
    Err<<"Correlation:"<<endl<<corr<<endl;
    Err<<"Matrix Q:"<<endl<<res.Q<<endl;
    Err<<"Matrix B:"<<endl<<res.B<<endl;
    Err.close();

    restoreBest();
    ui->plainTextEdit_2->appendPlainText("Data written to file "+qErr);
}



void MainWindow::on_pushButton_8_clicked()
{
    this->setCursor(QCursor(Qt::WaitCursor));

    ui->plainTextEdit_2->appendPlainText("Error Estimation");
    ui->tabPlots->setCurrentWidget(ui->tabConv);   // results appear there

    abortt=0;
    g_abortableRun = true;
    MainWindow::disableButtons();
    MainWindow::ErrorEstimation();
    g_abortableRun = false;
    MainWindow::enableButtons();

    this->setCursor(QCursor(Qt::ArrowCursor));
}


//*************************************
// Optimisation
//*************************************
void MainWindow::on_pushButton_4_clicked()
{
    abortt=0;
    g_abortableRun = true;
    MainWindow::disableButtons();
    MainWindow::resetRunState();
    // Restarts (reinitialisation) in a loop instead of
    // recursively from within the running optimisation (bug report K5).
    for(;;){
        MainWindow::Optimisation();
        if(reinitiate==1){
            reinitiate=0;
            inrein=1;
            continue;
        }
        break;
    }
    inrein=0;
    g_trackBest=0;
    g_abortableRun = false;
    MainWindow::enableButtons();

    this->setCursor(QCursor(Qt::ArrowCursor));

}

void MainWindow::on_checkBox_34_clicked()
{
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_8->setEnabled(false);
}

//************************************************
// Save settings
//************************************************

//****************************************
// Settings (.sps)
//
// Lines 1-58: old position-based format (kSettingsLegacy), one line per
// value; older program versions read exactly these lines.
// After that: "# Spectangular settings v2" and further settings as name=value.
// Read line by line so that paths with spaces and empty fields
// are preserved (bug report H5).
//****************************************
namespace {

const char *const kSettingsLegacy[] = {
    "spinBox", "spinBox_2", "spinBox_3", "spinBox_4", "spinBox_5", "spinBox_6",
    "doubleSpinBox", "doubleSpinBox_2", "doubleSpinBox_3", "doubleSpinBox_4", "doubleSpinBox_5",
    "doubleSpinBox_14", "doubleSpinBox_15",
    "comboBox",
    "lineEdit", "lineEdit_2", "lineEdit_3", "lineEdit_4", "lineEdit_5", "lineEdit_6", "lineEdit_7",
    "lineEdit_8", "lineEdit_9", "lineEdit_10", "lineEdit_11", "lineEdit_12", "lineEdit_13",
    "lineEdit_14", "lineEdit_15", "lineEdit_16", "lineEdit_17", "lineEdit_18", "lineEdit_19",
    "lineEdit_20", "lineEdit_21", "lineEdit_24",
    "checkBox", "checkBox_2", "checkBox_3", "checkBox_4", "checkBox_5", "checkBox_6", "checkBox_7",
    "checkBox_8", "checkBox_9", "checkBox_10", "checkBox_11", "checkBox_12", "checkBox_13",
    "checkBox_14", "checkBox_15", "checkBox_16", "checkBox_17", "checkBox_18",
    "checkBox_32", "checkBox_33", "checkBox_34", "checkBox_35"};

// Previously not saved. Not included: spinBox_7/8 (cores, machine-dependent).
const char *const kSettingsExtra[] = {
    "spinBox_11", "doubleSpinBox_16", "doubleSpinBox_17", "checkBox_41", "lineEdit_29",
    "checkBox_19", "checkBox_20", "checkBox_21", "checkBox_22", "checkBox_23", "checkBox_24",
    "checkBox_25", "checkBox_26", "checkBox_28", "checkBox_29", "checkBox_30",
    "checkBox_31", "checkBox_36", "checkBox_37", "checkBox_38", "checkBox_40",
    "lineEdit_25", "lineEdit_26", "lineEdit_27", "lineEdit_28", "comboBox_interp", "comboBox_method",
    "checkBox_42", "doubleSpinBox_18", "comboBox_ratio", "doubleSpinBox_lref", "doubleSpinBox_TA",
    "doubleSpinBox_TB", "lineEdit_ratio", "doubleSpinBox_st1", "doubleSpinBox_st2", "lineEdit_fixA", "lineEdit_fixB",
    "comboBox_err", "spinBox_mc", "checkBox_mask", "lineEdit_mask", "checkBox_norm", "doubleSpinBox_normKnot", "doubleSpinBox_normLow",
    "checkBox_align", "doubleSpinBox_alignWidth",
    "lineEdit_best", "lineEdit_optdata", "lineEdit_stat", "lineEdit_orbit", "lineEdit_bc", "comboBox_bcframe",
    "lineEdit_tellw", "checkBox_tellfit", "doubleSpinBox_tdamp", "checkBox_tellrange", "lineEdit_tellrange",
    "doubleSpinBox_telldet", "checkBox_resstatic",
    "checkBox_anaFit", "lineEdit_anaFit", "checkBox_anaLines", "lineEdit_lines", "lineEdit_anaLines",
    "lineEdit_lsGrid", "lineEdit_lsLines", "lineEdit_lsOut", "checkBox_lsSave"};

const char *const kSettingsHeaderV2 = "# Spectangular settings v2";

} // namespace

namespace {
// Orbital element checkboxes show "free"; configuration files keep storing "locked".
bool isLockKey(const QString &key)
{
    static const QStringList keys = {"checkBox_17", "checkBox_18", "checkBox_31", "checkBox_35",
                                     "checkBox_36", "checkBox_37", "checkBox_38"};
    return keys.contains(key);
}
}

QString MainWindow::settingValue(const QString &key) const
{
    QObject *w = findChild<QObject*>(key);
    if(isLockKey(key)){
        if(auto *c = qobject_cast<QCheckBox*>(w)) return c->isChecked() ? "0" : "1";
    }
    if(auto *s = qobject_cast<QSpinBox*>(w)) return QString::number(s->value());
    if(auto *d = qobject_cast<QDoubleSpinBox*>(w)) return QString::number(d->value(), 'g', 17);
    if(auto *c = qobject_cast<QCheckBox*>(w)) return c->isChecked() ? "1" : "0";
    if(auto *b = qobject_cast<QComboBox*>(w)) return QString::number(b->currentIndex());
    if(auto *l = qobject_cast<QLineEdit*>(w)) return l->text();
    return QString();
}

bool MainWindow::setSettingValue(const QString &key, const QString &value)
{
    QObject *w = findChild<QObject*>(key);
    bool ok = true;
    if(isLockKey(key)){
        if(auto *c = qobject_cast<QCheckBox*>(w)){
            c->setChecked(value.trimmed().toInt(&ok) == 0);
            return ok;
        }
    }
    if(auto *s = qobject_cast<QSpinBox*>(w)) s->setValue(int(std::lround(value.toDouble(&ok))));
    else if(auto *d = qobject_cast<QDoubleSpinBox*>(w)) d->setValue(value.toDouble(&ok));
    else if(auto *c = qobject_cast<QCheckBox*>(w)) c->setChecked(value.trimmed().toInt(&ok) != 0);
    else if(auto *b = qobject_cast<QComboBox*>(w)) b->setCurrentIndex(value.toInt(&ok));
    else if(auto *l = qobject_cast<QLineEdit*>(w)) l->setText(value);
    else return false;
    return ok;
}

bool MainWindow::writeSettings(const QString &fileName)
{
    QFile f(fileName);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        return false;
    }
    QTextStream out(&f);
    out.setCodec("UTF-8");
    for(const char *key : kSettingsLegacy){
        out << settingValue(key) << "\n";
    }
    out << kSettingsHeaderV2 << "\n";
    for(const char *key : kSettingsExtra){
        out << key << "=" << settingValue(key) << "\n";
    }
    out << "solver=" << (currentSolver()==dis::Solver::SvdDc ? "svd_dc" : "lsqr") << "\n";
    return f.error() == QFile::NoError;
}

QString MainWindow::readSettings(const QString &fileName)
{
    QFile f(fileName);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        return "File " + fileName + " cannot be opened.";
    }
    QTextStream in(&f);
    in.setCodec("UTF-8");
    QStringList lines;
    while(!in.atEnd()){
        QString l = in.readLine();
        if(l.endsWith('\r')) l.chop(1);
        lines << l;
    }
    const int nLegacy = int(sizeof(kSettingsLegacy)/sizeof(kSettingsLegacy[0]));
    if(lines.size() < nLegacy){
        return QString("File %1 has %2 lines, expected at least %3.").arg(fileName).arg(lines.size()).arg(nLegacy);
    }
    QStringList bad, unknown;
    // file names added later: defaults for configurations without them (not the names of the
    // configuration loaded before)
    ui->lineEdit_best->setText("beststate.dat");
    ui->lineEdit_optdata->setText("optdata.dat");
    ui->lineEdit_stat->setText("diffstatistic.dat");
    ui->lineEdit_orbit->setText("orbitelements.dat");
    ui->lineEdit_bc->setText("");
    ui->comboBox_bcframe->setCurrentIndex(0);
    ui->lineEdit_tellw->setText("");
    ui->checkBox_tellfit->setChecked(false);
    ui->doubleSpinBox_tdamp->setValue(1e-3);
    ui->checkBox_tellrange->setChecked(false);
    ui->lineEdit_tellrange->setText("telluric_static.dat");
    ui->doubleSpinBox_telldet->setValue(5.0);
    ui->checkBox_resstatic->setChecked(false);
    ui->checkBox_anaFit->setChecked(true);
    ui->lineEdit_anaFit->setText("rv_oc.dat");
    ui->checkBox_anaLines->setChecked(true);
    ui->lineEdit_lines->setText("lines.dat");
    ui->lineEdit_anaLines->setText("linestat.dat");
    ui->checkBox_lsSave->setChecked(false);
    ui->lineEdit_lsGrid->setText("orbitgrid.dat");
    ui->lineEdit_lsLines->setText("scanlines.dat");
    ui->lineEdit_lsOut->setText("linescan.dat");
    for(int i=0; i<nLegacy; i++){
        if(!setSettingValue(kSettingsLegacy[i], lines[i])) bad << kSettingsLegacy[i];
    }
    // Solver: in old files checkBox_5/6 meant "Standard"/"DaC" (both SVD);
    // without the key "solver" the default LSQR applies.
    QString solver = "lsqr";
    for(int i=nLegacy; i<lines.size(); i++){
        const QString &l = lines[i];
        int eq = l.indexOf('=');
        if(l.startsWith('#') || eq <= 0) continue;
        QString key = l.left(eq).trimmed();
        if(key=="solver"){
            solver = l.mid(eq+1).trimmed();
            continue;
        }
        if(key=="checkBox_39") continue;   // Trunc. SVD dropped
        if(key=="checkBox_27" || key=="spinBox_9" || key=="spinBox_10") continue;   // sequence mode dropped
        if(!findChild<QObject*>(key)){ unknown << key; continue; }   // e.g. from a newer version
        if(!setSettingValue(key, l.mid(eq+1))) bad << key;
    }
    if(solver!="lsqr" && solver!="svd_dc") bad << "solver";
    ui->checkBox_6->setChecked(solver=="svd_dc");
    ui->checkBox_5->setChecked(solver!="svd_dc");

    // side effects otherwise triggered only by the click handlers
    qPath = ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();
    ui->checkBox_7->setChecked(false);   // replaced by interpolation
    bidi = 0;
    ui->checkBox_8->setEnabled(!ui->checkBox_34->isChecked());
    ui->customPlot_2->setVisible(ui->checkBox_22->isChecked());

    QStringList msg;
    if(!bad.isEmpty()) msg << "Invalid values in " + fileName + ": " + bad.join(", ") + ".";
    if(!unknown.isEmpty()) msg << "Unknown settings (ignored) in " + fileName + ": " + unknown.join(", ") + ".";
    return msg.join(" ");
}

//****************************************
// Load/save configuration (File menu, batch mode, tests).
// On loading, the folder of the configuration file becomes the work folder (Work Path),
// regardless of the path stored in the file.
//****************************************
QString MainWindow::loadConfiguration(const QString &file)
{
    if(!QFile::exists(file)){
        return "File "+file+" does not exist.";
    }
    QString err = readSettings(file);
    if(!err.isEmpty()){
        return err;
    }
    const QFileInfo fi(file);
    ui->lineEdit_4->setText(fi.absolutePath());
    qPath = fi.absolutePath();
    path = qPath.toStdString();
    g_configFile = fi.absoluteFilePath();
    g_configName = fi.completeBaseName();
    this->setWindowTitle("Spectangular - "+fi.fileName());
    return QString();
}

bool MainWindow::saveConfiguration(const QString &file)
{
    if(!writeSettings(file)){
        return false;
    }
    const QFileInfo fi(file);
    g_configFile = fi.absoluteFilePath();
    g_configName = fi.completeBaseName();
    this->setWindowTitle("Spectangular - "+fi.fileName());
    return true;
}

//****************************************
// File menu: load/save configuration via file dialog.
//****************************************

// File dialog for the File menu; replaceable for tests (without a display).
QString (*g_pickConfigFile)(QWidget *parent, bool save, const QString &start) =
    [](QWidget *parent, bool save, const QString &start) -> QString {
        const QString filter = "Spectangular configuration (*.sps);;All files (*)";
        return save ? QFileDialog::getSaveFileName(parent, "Save Configuration As", start, filter)
                    : QFileDialog::getOpenFileName(parent, "Open Configuration", start, filter);
    };

void MainWindow::on_actionOpen_Configuration_triggered()
{
    QString dir = g_configFile.isEmpty() ? ui->lineEdit_4->text() : QFileInfo(g_configFile).absolutePath();
    QString file = g_pickConfigFile(this, false, dir);
    if(file.isEmpty()) return;
    QString err = loadConfiguration(file);
    if(!err.isEmpty()){
        reportError(err);
        error=0;
        return;
    }
    ui->plainTextEdit_2->appendPlainText("Configuration loaded: "+file+"; work path: "+qPath);
}

void MainWindow::on_actionSave_Configuration_triggered()
{
    if(g_configFile.isEmpty()){
        MainWindow::on_actionSave_Configuration_As_triggered();
        return;
    }
    if(!saveConfiguration(g_configFile)){
        reportError("File "+g_configFile+" could not be written.");
        error=0;
        return;
    }
    ui->plainTextEdit_2->appendPlainText("Configuration saved: "+g_configFile);
}

void MainWindow::on_actionSave_Configuration_As_triggered()
{
    QString dir = g_configFile.isEmpty() ? ui->lineEdit_4->text()+"/"+g_configName+".sps" : g_configFile;
    QString file = g_pickConfigFile(this, true, dir);
    if(file.isEmpty()) return;
    if(!file.endsWith(".sps")) file += ".sps";   // the dialog asks before overwriting
    if(!saveConfiguration(file)){
        reportError("File "+file+" could not be written.");
        error=0;
        return;
    }
    ui->plainTextEdit_2->appendPlainText("Configuration saved: "+file);
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

//**************************************
// root finding
//**************************************
void MainWindow::findroot(){
    bool ok = true;
    RVE = orbit::eccentricAnomaly(2*M_PI*(RVt-RVT0)/RVP, RVe, &ok);
    if(!ok) RVCalcCheck=1;
}

//*************************
// apply file names
//*************************
void MainWindow::on_pushButton_11_clicked()
{
    QString cName = ui->lineEdit_25->text();
    QString cExt = ui->lineEdit_26->text();
    ui->lineEdit_5->setText(cName+"_1."+cExt);
    ui->lineEdit_8->setText(cName+"_2."+cExt);
    ui->lineEdit_10->setText(cName+"_tell."+cExt);
    ui->lineEdit_13->setText(cName+"_diff."+cExt);
    ui->lineEdit_14->setText("initval_"+cName+"."+cExt);
    ui->lineEdit_15->setText("initmat_"+cName+"."+cExt);
    ui->lineEdit_16->setText("optval_"+cName+"."+cExt);
    ui->lineEdit_17->setText("optmat_"+cName+"."+cExt);
    ui->lineEdit_20->setText(cName+"_error."+cExt);
    ui->lineEdit_21->setText(cName+"_log."+cExt);
    ui->lineEdit_24->setText(cName+"_errors."+cExt);
    ui->lineEdit_stat->setText(cName+"_diffstat."+cExt);
    ui->lineEdit_best->setText(cName+"_beststate."+cExt);
    ui->lineEdit_optdata->setText(cName+"_optdata."+cExt);
    ui->lineEdit_anaFit->setText(cName+"_rv_oc."+cExt);
    ui->lineEdit_anaLines->setText(cName+"_linestat."+cExt);
    ui->lineEdit_lsLines->setText(cName+"_scanlines."+cExt);
    ui->lineEdit_lsOut->setText(cName+"_linescan."+cExt);
    g_configName = cName+"_conf";
}

//*******************************************
// subtract disentangled from observations
//*******************************************
void MainWindow::on_pushButton_12_clicked()
{
    // Subtract: subtract the contribution of one component (from compl.txt) from the
    // observations and write the residual spectra. checkBox_20: subtract A,
    // otherwise subtract B. Optionally wavelengths in the rest frame (checkBox_24/25)
    // and addition/multiplication of a value (checkBox_26/28).
    this->setCursor(QCursor(Qt::WaitCursor));

    MainWindow::read_data();
    if(error==1){
        error=0;
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;
    }

    double ofs=0.0;
    if((ui->checkBox_26->isChecked()) or (ui->checkBox_28->isChecked())){
        bool ok=false;
        ofs = ui->lineEdit_28->text().toDouble(&ok);
        if(!ok){
            reportError("No valid value set to add or multiply.");
            error=0;
            return;
        }
    }

    // build the matrix with the RVs that compl.txt belongs to
    {
        ifstream cf((qPath+"/compl.txt").toStdString().c_str());
        std::string line;
        int nrv=0;
        while(std::getline(cf, line)){
            std::istringstream ls(line);
            std::string hash, key;
            int j;
            double v1, v3;
            if((ls>>hash>>key>>j>>v1>>v3) && hash=="#" && key=="rv" && j>=0 && j<num){
                RV1[j]=v1;
                if(bso2>0) RV3[j]=v3;
                ++nrv;
            }
        }
        if(nrv==num){
            if(!MainWindow::buildDesignMatrix(true)){
                error=0;
                return;
            }
        }
        else if(nrv>0){
            reportError("Error 39: "+qPath+"/compl.txt belongs to a different number of spectra.");
            error=0;
            return;
        }
    }

    std::vector<std::vector<double>> rows;
    std::string ioErr;
    if(!dataio::readTable((qPath+"/compl.txt").toStdString(), 2, rows, ioErr)){
        reportError("Error 39: "+QString::fromStdString(ioErr)+" Run Solve or the optimisation first.");
        error=0;
        return;
    }
    if(int(rows.size())!=Mm){
        reportError("Error 39: "+qPath+"/compl.txt does not match the current data ("
                    +QString::number(rows.size())+" instead of "+QString::number(Mm)
                    +" entries). Run Solve or the optimisation first.");
        error=0;
        return;
    }

    const bool subtractA = ui->checkBox_20->isChecked();
    vec sol2(Mm, fill::zeros);
    for(int k=0; k<Mm; k++){
        bool inA = k<bso1;
        bool inB = (k>=bso1) && (k<bso1+bso2);
        if((subtractA && inA) || (!subtractA && inB)){
            sol2(k) = rows[k][1];
        }
    }
    vec diff = C - M*sol2;

    QString fext=ui->lineEdit_19->text();
    int smin= ui->spinBox->value();
    for(int e = 0; e<num; e++){
        QString name = qPath+"/"+ui->lineEdit_27->text()+"_"+QString::number(e+smin)+fext;
        ofstream file2(name.toStdString().c_str());
        if(!file2){
            reportError("Cannot write "+name);
            error=0;
            return;
        }
        for(int i=e*logbin; i< (e+1)*logbin; i++){
            if(ui->checkBox_26->isChecked()){
                diff[i]=diff[i]+ofs;
            }
            else if(ui->checkBox_28->isChecked()){
                diff[i]=diff[i]*ofs;
            }
            double lambda = pow(10,(W[i]));
            if(ui->checkBox_24->isChecked() && bso2>0){
                lambda /= 1+RV3[e]/c0;     // rest frame of B
            }
            else if(ui->checkBox_25->isChecked()){
                lambda /= 1+RV1[e]/c0;     // rest frame of A
            }
            file2<<setprecision(14)<<lambda<<"\t"<<diff[i]<<endl;
        }
    }

    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_spinBox_8_valueChanged(int)
{
    // number of cores for LSQR (OpenMP) and the SVD (BLAS library, if OpenBLAS/BLIS/MKL)
    const unsigned before = cores;
    cores = unsigned(ui->spinBox_8->value());
    dis::setThreads(ui->spinBox_8->value());
    // Note when the user (field has the focus: not set by the program, a configuration or
    // --threads) goes above 2 cores; once per crossing, not for every further step
    if(runow==0 && cores>kDefaultCores && before<=kDefaultCores && ui->spinBox_8->hasFocus()){
        QMessageBox::information(this, "Number of cores",
            QString("With more than %1 cores the computation will probably take longer, not less.\n\n"
                    "Only the matrix-vector products of the LSQR solver run in parallel, and they are "
                    "limited by the memory bandwidth; matrix construction, residuum and the optimiser "
                    "run on one core. In a scaling test on long spectra %1 cores were fastest.\n\n"
                    "To check on your data: compare the time of a Solve with %1 and with %2 cores.")
                .arg(kDefaultCores).arg(cores));
    }
}

void MainWindow::on_checkBox_22_clicked()
{
    if(ui->checkBox_22->isChecked()){
        ui->customPlot_2->show();
    }
    else{
        ui->customPlot_2->hide();
    }
}

void MainWindow::on_checkBox_20_clicked()
{
    if(ui->checkBox_20->isChecked()){
        ui->checkBox_21->setChecked(false);
    }
    else{
        ui->checkBox_21->setChecked(true);
    }
}

void MainWindow::on_checkBox_21_clicked()
{
    if(ui->checkBox_21->isChecked()){
        ui->checkBox_20->setChecked(false);
    }
    else{
        ui->checkBox_20->setChecked(true);
    }
}

void MainWindow::on_checkBox_24_clicked()
{
    if(ui->checkBox_24->isChecked()){
        ui->checkBox_25->setChecked(false);
    }
    else{
        //
    }
}

void MainWindow::on_checkBox_25_clicked()
{
    if(ui->checkBox_25->isChecked()){
        ui->checkBox_24->setChecked(false);
    }
    else{
        //
    }
}

void MainWindow::on_checkBox_26_clicked()
{
    if(ui->checkBox_26->isChecked()){
        ui->checkBox_28->setChecked(false);
    }
    else{
        //
    }
}

void MainWindow::on_checkBox_28_clicked()
{
    if(ui->checkBox_28->isChecked()){
        ui->checkBox_26->setChecked(false);
    }
}

void MainWindow::disableButtons()
{
   ui->pushButton_2->setEnabled(false);
   ui->pushButton_3->setEnabled(false);
   ui->pushButton_4->setEnabled(false);
   ui->pushButton_8->setEnabled(false);
   ui->pushButton_11->setEnabled(false);
   ui->pushButton_12->setEnabled(false);
   ui->pushButton_13->setEnabled(false);
}

void MainWindow::enableButtons()
{
   ui->pushButton_2->setEnabled(true);
   ui->pushButton_3->setEnabled(true);
   ui->pushButton_4->setEnabled(true);
   ui->pushButton_8->setEnabled(true);
   ui->pushButton_11->setEnabled(true);
   ui->pushButton_12->setEnabled(true);
   ui->pushButton_13->setEnabled(true);
}


//*******************************************
// optimize on flux ratios
//*******************************************
void MainWindow::on_pushButton_13_clicked()
{
    abortt=0;
    if(!(ui->doubleSpinBox_4->value()>0)){
        reportError("Error 51: The flux ratio F_A/F_B (Data tab) must be positive.");
        return;
    }
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    QString input=qPath+"/continuumpoints.dat";
    string data1 = input.toUtf8().constData();
    std::ostringstream dat1NameStream(data1);
    dat1NameStream<<data1;
    std::string dat1Name = dat1NameStream.str();

    QFile checkfile1(dat1Name.c_str());

    if(ui->checkBox_30->isChecked()){

        if(!checkfile1.exists()){
            QMessageBox::information(this, "Error", "File to define continuum does not exist. Please set the points via the Editor.");
            return;
        }
        else{
            string line, eins;
            int nlines=0;
            ifstream points(dat1Name.c_str());
            while(std::getline(points, line))
                ++nlines;

            points.clear();
            points.seekg(0, ios::beg);

            if(nlines!=8){
                QMessageBox::information(this, "Error", "Number of points in continuumpoints.dat is not 8.");
                return;
            }
            else{
                for(int i =0;i <8; i++){
                    points >> eins;
                    istringstream ist(eins);
                    ist>>cpoints[i];
                }
            }
        }
    }
    else{
        //
    }

    optfratios=1;
    MainWindow::disableButtons();

    // optmisation on flux ratios
    //system_clock::time_point time1 = system_clock::now();

    upda=0;
    updac=0;
    abauto=0;

    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);

    this->setCursor(QCursor(Qt::WaitCursor));
    MainWindow::read_data();
    this->setCursor(QCursor(Qt::WaitCursor));

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        optfratios=0;
        error=0;
        MainWindow::enableButtons();
        return;
    }

    qApp->processEvents(QEventLoop::AllEvents);

    int Ph=0, Pl=0, Psh=0;

    eval=0;


    zaehler=ui->spinBox_3->value();
    double ym=0, ys=0;

    int nu=num;
    QVector<double> fratios(num);

    QString qLogFile = ui->lineEdit_21->text();
    QString qlFile = qPath+"/"+qLogFile;
    QFileInfo QLFile(qlFile);
    string lFile = qLogFile.toUtf8().constData();
    std::ostringstream lFileNameStream(lFile);
    lFileNameStream<<path<<"/"<<lFile;
    std::string lFileName = lFileNameStream.str();
    ofstream LogFile;

    struct tm *ts;
    time_t t;

    t = time(NULL);
    ts = localtime(&t);

    if(QLFile.exists()){
        LogFile.open(lFileName.c_str(), std::ios_base::app);
        LogFile<<endl;
        LogFile<<"__________________________________________________"<<endl;
        LogFile<<" "<<asctime(ts)<<endl;
        LogFile<<endl;
    }

    else{
        LogFile.open(lFileName.c_str());
        LogFile<<"******************************"<<endl;
        LogFile<<"  Optimisation LogFile"<<endl;
        LogFile<<"******************************"<<endl;
    }

    LogFile<<"Optimisation of flux ratios."<<endl;

    LogFile<<endl;

    if(ui->checkBox_29->isChecked()){
        LogFile<<"Using residuum as criterion for optimisation."<<endl;
    }
    else{
        LogFile<<"Using function slope as criterion for optimisation."<<endl;
    }

    string sstrength = "edit.dat";
    std::ostringstream ediNameStream(sstrength);
    ediNameStream<<path<<"/"<<sstrength;
    std::string ediName = ediNameStream.str();

    QFileInfo lstrength(ediName.c_str());

    if(lstrength.exists()){

        ifstream edit(ediName.c_str());
        string lines, einse;

        int num_lines1=0;

        while(std::getline(edit, lines))
                   ++ num_lines1;

                edit.clear();
                edit.seekg(0, ios::beg);

                if(num_lines1!=3*num){
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 22: Edit file does not match to specified number of observations.");
                    }
                    else{
                        cout<<"Error 22: Edit file does not match to specified number of observations."<<endl;
                    }
                    error=1;
                    optfratios=0;
                    MainWindow::enableButtons();
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    return;
                }
                LogFile<<"Relative component strength and telluric line strength set via Editor."<<endl;
                LogFile<<"The strength of component B and the ratios are calculated via given strength of component A."<<endl;

                for(int i=0; i<num_lines1; i++){
                    edit >> einse;
                    istringstream istr7(einse);

                    if(i<num){
                        istr7>>Mtel[i];
                    }
                    if((i>=num) & (i<2*num)){
                        istr7>>Mval1[i-num];
                        fratios[i-num]=Mval1[i-num]/(1-Mval1[i-num]);   // F_A/F_B
                        Mval2[i-num]=1/(1+fratios[i-num]);
                    }
                    if(i>=2*num){
                        //istr7>>Mval2[i-2*num];
                        //Mval2[i]=fratios[i]/(1+fratios[i]);
                    }
                }
            }

            else {
                LogFile<<"No relative values set via Editor. Using flux ratio from GUI for all spectra."<<endl;

                for(int i=0; i<num; i++){
                    Mtel[i]=1.0;
                    Mval1[i]=ui->doubleSpinBox_4->value()/(1+ui->doubleSpinBox_4->value());
                    Mval2[i]=1/(1+ui->doubleSpinBox_4->value());
                    fratios[i]=ui->doubleSpinBox_4->value();
                }
            }

    LogFile<<endl;

    string scoef = "optcoef.dat";
    std::ostringstream coeNameStream(scoef);
    coeNameStream<<path<<"/"<<scoef;
    std::string coeName = coeNameStream.str();

    QFileInfo optcoef(coeName.c_str());

    if(optcoef.exists()){

        LogFile<<"User defined transformation coefficients used."<<endl;

        double optv[4];
        ifstream optc(coeName.c_str());
        string einse;

        for(int i=0; i<4; i++){
            optc >> einse;
            istringstream istr8(einse);
            istr8 >> optv[i];
        }
        alpha=optv[0];
        nmBeta=optv[1];
        btot=optv[2];
        Gamma=optv[3];

    }

    else{
        LogFile<<"Default values for transformation coefficients used."<<endl;

        Gamma=2.0;
        alpha=1.0;
        nmBeta=0.5;
        btot=0.5;
    }

    string sinit = "initialstep.dat";
    std::ostringstream iniNameStream(sinit);
    iniNameStream<<path<<"/"<<sinit;
    std::string iniName = iniNameStream.str();

    QFileInfo initstep(iniName.c_str());

    if(initstep.exists()){

        ifstream initStep(iniName.c_str());
        double IntS[8];
        string einse;

        for(int i =0; i<8; i++){
            initStep >> einse;
            istringstream istr9(einse);
            istr9 >> IntS[i];
        }
        step = IntS[0]*0.1;
        dP = IntS[1];
        de = IntS[2];
        dKA = IntS[3];
        dKB = IntS[4];
        dGamma = IntS[5];
        dT0 = IntS[6];
        dOmega = IntS[7];

        LogFile<<"User defined initial step size of "<<step<<" used."<<endl;
    }

    else{

        LogFile<<"Default initial step size of 0.1 used."<<endl;

        step=0.1;
        dP = orbele[0]/10;
        de = (orbele[1]>0.05 ? orbele[1]/10 : 0.005);   // otherwise degenerate simplex at e=0
        dKA = 3*dv;
        dKB = 3*dv;
        dGamma = 3*dv;
        dT0 = orbele[0]/10;
        dOmega = M_PI/20;
    }

    LogFile<<"Alpha: "<<alpha<<"; Beta: "<<nmBeta<<"; Beta_tot: "<<btot<<"; Gamma: "<<Gamma<<"; Step: "<<step<<endl;

    LogFile<<endl;

    std::vector<double> y(nu+1), Z(nu), Co(nu), So(nu), Eo(nu);
    mat P(nu+1,nu), e(nu+1,nu);

    r1=0.0;

    LogFile<<"Creating new initial simplex with "<<nu+1<<" points."<<endl;

    qInitval = "fr_opt_val.dat";
    string file1init = qInitval.toUtf8().constData();
    std::ostringstream file1NameStream(file1init);
    file1NameStream<<path<<"/"<<file1init;
    std::string file1Name = file1NameStream.str();

    qInitmat = "fr_opt_mat.dat";
    string file2init = qInitmat.toUtf8().constData();
    std::ostringstream file2NameStream(file2init);
    file2NameStream<<path<<"/"<<file2init;
    std::string file2Name = file2NameStream.str();

    if(ui->checkBox_9->isChecked()){
        QFile QIn(file1Name.c_str());
        QFile QIn2(file2Name.c_str());

        if((inrein==0) & (QIn.exists() or QIn2.exists())){
            if(runow==0){
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Warning!", "The optimisation files "+QIn.fileName()+" and "+QIn2.fileName()+" already exists. \n\n Do you want to overwrite it?",
                                      QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                qDebug() << "Overwrite initial data.";
                }
                else{
                    qDebug()<< "Initiation aborted.";
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    MainWindow::enableButtons();
                    optfratios=0;
                    return;
                }
            }
            else{
                cout<<"Warning: The optimisation files already exists. Do you want to overwrite it? 0 no, yes 1: "<<endl;
                if(batchOverwrite==1) overw=1;
                    else if(!(cin >> overw)) overw=0;
                    if(overw==0) cerr<<"Existing optimisation files are not overwritten (use --overwrite)."<<endl;
                if(overw==0){
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    MainWindow::enableButtons();
                    optfratios=0;
                    return;
                }
                else{
                    qDebug() << "Overwrite initial data.";
                }
            }
        }
        else{
            cout<<"Create new initial data."<<endl;
        }

    }
    else{
        cout<<"Create new initial data."<<endl;
        inrein=0;
    }

    ofstream init(file1Name.c_str());
    ofstream init2(file2Name.c_str());

        for(int i=0; i<nu; i++){
            P(0,i)=fratios[i];
        }

    for(int i=0; i<nu+1; i++){
        for(int j=0; j<nu; j++){

            if((i>0)&(i==j+1)){
                e(i,j)=step;
            }
            else{
                e(i,j)=0;
            }
        }
    }

    for (int i=0; i<nu+1; i++){
        for (int j=0; j<nu; j++){

            P(i,j)=P(0,j)+e(i,j);
            init2<<setprecision(14)<<P(i,j)<<endl;
            //cout<<P(i,j)<<" ";

            Mval1[j]=P(i,j)/(1+P(i,j));
            Mval2[j]=1/(1+P(i,j));
        }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[i]=MainWindow::DivideConquer();
        //soptd<<"0\t"<<eval<<"\t"<<y[i]<<endl;
        qApp->processEvents(QEventLoop::AllEvents);
        if(upda==1){
            upda=0;
            abauto += 1;
            LogFile<<endl;
            LogFile<<"New ratios found; Residuum: "<<y[i]<<endl;
            MainWindow::on_pushButton_6_clicked();
            //MainWindow::plot_functionvalues();
                for(int i = 0; i<nu;i++){
                     LogFile<<Mval1[i]<<"\t"<<Mval2[i]<<endl;
                }
            LogFile<<endl;
        }

        if(abortt==1){
            LogFile<<"Initiation was aborted."<<endl;
            abortt=0;
            optfratios=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            MainWindow::enableButtons();
            return;
        }

        cout<<y[i]<<endl;
        init<<setprecision(12)<<y[i]<<endl;
        LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;
    }

    init.close();
    init2.close();

    LogFile<<"Initial simplex completed. Start of optimisation."<<endl;

    double ys2=0.0, ym2=0.0;
    int stagnate=0;

    //start optimisation

    qOptval = "optval_flux.dat";
    string file1opt = qOptval.toUtf8().constData();
    std::ostringstream file3NameStream(file1opt);
    file3NameStream<<path<<"/"<<file1opt;
    std::string file3Name = file3NameStream.str();

    qOptmat = "optmat_flux.dat";
    string file2opt = qOptmat.toUtf8().constData();
    std::ostringstream file4NameStream(file2opt);
    file4NameStream<<path<<"/"<<file2opt;
    std::string file4Name = file4NameStream.str();
    // save simplex (bug report H12: files were emptied beforehand)
    auto saveSimplex = [&](){
        ofstream o1(file3Name.c_str()), o2(file4Name.c_str());
        for(int i=0; i<nu+1;i++){
            o1<<setprecision(14)<<y[i]<<endl;
            for(int j=0; j<nu; j++){
                o2<<setprecision(14)<<P(i,j)<<endl;
            }
        }
    };
        nm::Coefficients nmCoef;
        nmCoef.alpha=alpha;
        nmCoef.gamma=Gamma;
        nmCoef.beta=nmBeta;
        nmCoef.delta=btot;

    // Auto Stop as for RVs and orbit (chi^2 progress over a window); only with the
    // residuum criterion, the slope criterion has no chi^2. No restarts here.
    nm::ProgressMonitor progress(ui->spinBox_11->value()>0 ? ui->spinBox_11->value() : nm::defaultWindow(nu));
    int itersDone = zaehler;
    for (int t=0; t<zaehler; t++){
        ui->spinBox_4->setValue(t+1);

        string optback1 = "optval_flux_backup.dat";
        std::ostringstream back1NameStream(optback1);
        back1NameStream<<path<<"/"<<optback1;
        std::string back1Name = back1NameStream.str();
        ofstream back1(back1Name.c_str());


        string optback2 = "optmat_flux_backup.dat";
        std::ostringstream back2NameStream(optback2);
        back2NameStream<<path<<"/"<<optback2;
        std::string back2Name = back2NameStream.str();
        ofstream back2(back2Name.c_str());

        for(int i=0; i<nu+1;i++){
            //cout<<y[i]<<endl;
            back1<<setprecision(12)<<y[i]<<endl;
            for(int j=0; j<nu; j++){
                back2<<setprecision(12)<<P(i,j)<<endl;
            }
        }

        LogFile<<endl;
        LogFile<<"Iteration: "<<t+1<<endl;
        //MainWindow::plot_functionvalues();
        qApp->processEvents(QEventLoop::AllEvents);

        cout<<endl;
        cout<<endl;
        cout<<"Iteration: "<<t+1<<"\t";

        //initialize next step
        ym=0;
        ys=0;
        for (int i=0; i<nu; i++){
            Z[i]=0;
        }

        nm::order(y, Ph, Psh, Pl);

        //computing mean and sigma
        for (int i=0; i<nu+1; i++){
            ym+=y[i]/(nu+1);
        }
        for (int i=0; i<nu+1; i++){
            ys+=sqrt(pow((y[i]-ym),2));
        }
        ys=ys/(nu);

        if((ys==ys2) && (ym==ym2)){
            ++stagnate;
            cout<<"stagnating ("<<stagnate<<" iterations)."<<endl;
        }
        else{
            //
        }
        ys2=ys;
        ym2=ym;

        if(ui->checkBox_29->isChecked()
                && MainWindow::convergenceStep(progress, y[Pl], nu, LogFile, false)==RunAction::Done){
            cout<<"Optimisation stopped (Auto Stop)."<<endl;
            LogFile<<"Optimisation stopped (Auto Stop)."<<endl;
            itersDone = t;
            break;
        }

        cout<<"Simplex Mean: "<<ym<<"; Simplex STD: "<<ys<<endl;

        LogFile<<setprecision(12)<<"Mean: "<<ym<<"\tSTD: "<<ys<<endl;

        //Nelder-Mead step (neldermead.h)
        {
            auto fRatio = [&](const arma::rowvec &x){
                for(int i=0; i<nu; i++){
                    if(!(x(i)>0)) return orbit::kInvalidPenalty;   // ratios must be positive
                }
                for(int i=0; i<nu; i++){
                    Mval1[i]=x(i)/(1+x(i));   // x = F_A/F_B as in "Flux Ratio A/B"
                    Mval2[i]=1/(1+x(i));
                }
                eval++;
                ui->spinBox_5->setValue(eval);
                qApp->processEvents(QEventLoop::AllEvents);
                MainWindow::ConstructMatrix();
                double r=MainWindow::DivideConquer();
                qApp->processEvents(QEventLoop::AllEvents);
                if(upda==1){
                    upda=0;
                    LogFile<<endl;
                    LogFile<<"New ratios found; Residuum: "<<r<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<x(i)<<endl;
                    }
                    LogFile<<endl;
                    MainWindow::on_pushButton_6_clicked();
                    qApp->processEvents(QEventLoop::AllEvents);
                }
                return r;
            };
            nm::StepResult st = nm::step(P, y, nmCoef, fRatio, [&]{ return abortt==1; });
            LogFile<<st.log;
            if(st.aborted){
                nm::order(y, Ph, Psh, Pl);
                LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
                }
                LogFile<<endl;
                saveSimplex();
                abortt=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }
        }
        LogFile<<endl;
        }//end main loop


        nm::order(y, Ph, Psh, Pl);

        saveSimplex();

        // adopt best ratios and write the output with them (M6)
        for(int i=0; i<nu; i++){
            Mval1[i]=P(Pl,i)/(1+P(Pl,i));
            Mval2[i]=1/(1+P(Pl,i));
        }
        MainWindow::ConstructMatrix();
        MainWindow::DivideConquer();

        LogFile<<ym<<"          "<<ys<<endl;
        LogFile<<" End Optimisation "<<endl;
        LogFile<<" ratios after "<<itersDone<<" Iterations and "<<eval<<" Evaluations of Optimisation: "<<endl;
            for(int i = 0; i<nu;i++){
                LogFile<<P(Pl,i)<<endl;
            }

        LogFile.close();

        cout<<"Ratios after "<<itersDone<<" Iterations and "<<eval<<" Evaluations:"<<endl;
        for(int i=0; i<nu;i++){
            cout<<P(Pl,i)<<endl;
        }

    optfratios=0;
    MainWindow::enableButtons();
    this->setCursor(QCursor(Qt::ArrowCursor));

}


void MainWindow::on_checkBox_29_clicked()
{
    if(ui->checkBox_29->isChecked()){
        ui->checkBox_30->setChecked(false);
    }
    else{
        ui->checkBox_30->setChecked(true);
    }
}

void MainWindow::on_checkBox_30_clicked()
{
    if(ui->checkBox_30->isChecked()){
        ui->checkBox_29->setChecked(false);
    }
    else{
        ui->checkBox_29->setChecked(true);
    }
}

void MainWindow::on_pushButton_15_clicked()
{

    checker=2;
    mini=ui->spinBox->value();
    maxi=ui->spinBox_2->value();
    num=maxi-mini+1;

    RV1.resize(num);
    RV2.resize(num);
    RV3.resize(num);
    otimes.resize(num);

    QString inputt=ui->lineEdit_18->text();
    string times = inputt.toUtf8().constData();
    std::ostringstream intNameStream(times);
    intNameStream<<path<<"/"<<times;
    std::string intName = intNameStream.str();

    QFile checkfile2(intName.c_str());

    if(!checkfile2.exists()){
        qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
        if(runow==0){
            QMessageBox::information(this, "Error", "Error 4: File"+qPath+"/"+inputt+" does not exist!");
        }
        else{
            cout<<"Error 4: File "<<path<<"/"<<times<<" does not exist!"<<endl;
        }
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=1;
       return;
    }
    else{

        ifstream dat(intName.c_str());

        int entries=0;
        string liner;
        while(std::getline(dat, liner))
           ++entries;

        dat.clear();
        dat.seekg(0, ios::beg);

        if(entries!=num){
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 5: Time file "+qPath+"/"+inputt+" does not match to the number of observations.");
            }
            else{
                cout<<"Error 5: File "<<path<<"/"<<times<<" does not match to the number of observations."<<endl;
            }
            error=1;
            this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }


        for(int g=0; g<num; g++){
            dat >> eins;
            istringstream istr(eins);
            istr >> otimes[g];
        }
        dat.close();
    }

    string orbit = qOrbitFile.toStdString();
    std::ostringstream orbNameStream(orbit);
    orbNameStream<<path<<"/"<<orbit;
    std::string orbName = orbNameStream.str();

    QFile checkorb(orbName.c_str());

    if(!checkorb.exists()){
        qDebug()<<"The file "<<checkorb.fileName()<<" does not exist.";
        if(runow==0){
            QMessageBox::information(this, "Error", "Error 6: File "+checkorb.fileName()+" does not exist! Set the orbital elements via the Editor.");
        }
        else{
            cout<<"Error 6: File "<<path<<"/"<<orbit<<" does not exist! Set the orbital elements via the Editor."<<endl;
        }
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=1;
       return;
    }
    else{

        ifstream initorbit(orbName.c_str());

        for(int i=0; i<7; i++){
            initorbit >> eins;
            istringstream istr1(eins);
            istr1 >> orbele[i];
        }
        initorbit.close();
    }

    MainWindow::VAmplitudeA();
    MainWindow::VAmplitudeB();

    for(int i=0; i<num; i++){
        QString index= QString::number(i);
        QString velocity=QString::number(RV1[i]);
        QString sum=QString::number(RV3[i]);
        ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+sum);
        //ui->plainTextEdit->appendPlainText(QString::number(RVE)+" "+QString::number(theta));
        //cout<<RV1[i]<<" "<<RV3[i]<<endl;

    }
    if(RVCalcCheck==1){
        RVCalcCheck=0;
        QMessageBox::information(this, "Warning", "Possible uncertainty in RV calculation. Please check.");
    }


//looking for maximum RV absolute
RV1max=abs(RV1[0]);
RV3max=abs(RV3[0]);
for (int i=0; i<num; i++){
    if (abs(RV1[i])>abs(RV1max)){
        RV1max=RV1[i];
    }
    else{
        //
    }
    if (abs(RV3[i])>abs(RV3max)){
        RV3max=RV3[i];
    }
    else{
        //
    }
}

//looking for minimum and maximum RV
RV1min=RV1max;
RV3min=RV3max;
RV1maxi=RV1[0];
RV3maxi=RV3[0];
for(int i=0; i<num; i++){
    if(RV1[i]<RV1min){
        RV1min=RV1[i];
    }
    else{
        //
    }
    if(RV3[i]<RV3min){
        RV3min=RV3[i];
    }
    else{
        //
    }
    if(RV1[i]>RV1maxi){
        RV1maxi=RV1[i];
    }
    else{
        //
    }
    if(RV3[i]>RV3maxi){
        RV3maxi=RV3[i];
    }
    else{
        //
    }
}

RV1amin=RV1[0];
for(int i=0; i<num; i++){
    if(abs(RV1[i])<RV1amin){
        RV1amin=RV1[i];
    }
    else{
        //
    }
}

}

//****************************************
// Data tab, "Detect": ranges of the static lines from the static spectrum of the last Solve.
// High-pass (minus a running median over 5 A), noise from the median absolute deviation,
// pixels beyond the threshold (absorption and emission), widened by 0.5 A, gaps below 1 A
// closed. Written to <file>_detected.<ext> and set in the Data tab.
//****************************************
// Spectra of the components A and B of the current solution on the wavelength grid of A (B is
// interpolated onto it); false with Error 55 (if report) if there is no solution
bool MainWindow::lineScanSpectra(std::vector<double> &lam, std::vector<double> &fa, std::vector<double> &fb, bool report)
{
    if(!g_matrixValid || num<1 || logbin<8 || M.n_rows!=C.n_elem || X.n_elem!=M.n_cols || g_layout.cols()!=int(M.n_cols)
            || W.n_elem<2 || g_layout.nA<10 || g_layout.nB<10 || !(arma::abs(X).max()>0)){
        if(report) reportError("Error 55: Line width: there is no solution of two components. Read data and Solve (or optimise) first.");
        return false;
    }
    const arma::vec Xc = componentSpectra();
    const double w0 = W(0);
    std::vector<double> lb(g_layout.nB), vb(g_layout.nB);
    lam.assign(g_layout.nA, 0.0);
    fa.assign(g_layout.nA, 0.0);
    for(int k=0; k<g_layout.nA; k++){
        lam[k] = std::pow(10.0, dis::columnLog10Lambda(g_layout, 'A', k, w0, g_dlog));
        fa[k] = Xc(k);
    }
    for(int k=0; k<g_layout.nB; k++){
        lb[k] = std::pow(10.0, dis::columnLog10Lambda(g_layout, 'B', k, w0, g_dlog));
        vb[k] = Xc(g_layout.startB()+k);
    }
    fb = linewidth::resample(lb, vb, lam);
    return true;
}

// Component spectra for Find Lines: the files of A and B of the Output tab (the continuum-normalised
// ones "<name>_norm.<ext>" if they exist, else the plain ones), otherwise the current solution
bool MainWindow::lineFinderSpectra(std::vector<double> &lam, std::vector<double> &fa, std::vector<double> &fb, QString &source)
{
    QStringList used;
    std::vector<double> w[2], v[2];
    bool ok = true;
    for(int part=0; part<2 && ok; part++){
        const QString name = (part==0 ? ui->lineEdit_5 : ui->lineEdit_8)->text().trimmed();
        QString file;
        for(const QString &cand : {normName(name), name})
            if(!name.isEmpty() && QFileInfo::exists(qPath+"/"+cand)){ file = cand; break; }
        std::vector<std::vector<double>> rows;
        std::string err;
        if(file.isEmpty() || !dataio::readTable((qPath+"/"+file).toStdString(), 2, rows, err) || rows.size()<10){
            ok = false;
            break;
        }
        for(const auto &r : rows){ w[part].push_back(r[0]); v[part].push_back(r[1]); }
        used << file;
    }
    if(ok){
        lam = w[0];
        fa = v[0];
        fb = linewidth::resample(w[1], v[1], lam);
        source = "files "+used.join(", ");
        return true;
    }
    if(lineScanSpectra(lam, fa, fb, false)){
        source = "current solution";
        return true;
    }
    reportError("Error 55: Find Lines: no component spectra. The files of A and B (Output tab) do not exist and there is no solution. Solve (or optimise) first or set the file names.");
    return false;
}

// Optimisation tab, Line width scan: Find Lines
void MainWindow::on_pushButton_lsFind_clicked()
{
    qPath = ui->lineEdit_4->text();
    LineFinderDialog::Data d;
    QString source;
    if(!lineFinderSpectra(d.lambda, d.fluxA, d.fluxB, source)) return;
    const QString name = ui->lineEdit_lsLines->text().trimmed();
    if(name.isEmpty()){
        reportError("Error 56: Line width: no file name for the line list (Optimisation tab).");
        return;
    }
    d.file = qPath+"/"+name;
    // an existing list is shown and its settings are used
    if(QFileInfo::exists(d.file)){
        std::string err;
        linewidth::readLineList(d.file.toStdString(), d.params, d.linesA, d.linesB, err);
    }
    auto *dlg = new LineFinderDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setData(d);
    dlg->setWindowTitle("Find Lines ("+source+")");
    connect(dlg, &LineFinderDialog::applied, this, [this](const QString &file){
        const QString m = "Line width: line list written to "+file+".";
        cout<<m.toStdString()<<endl;
        if(runow==0) ui->plainTextEdit_2->appendPlainText(m);
    });
    dlg->show();
}

// Optimisation tab, Line width scan: residuum and line width for every orbit of the grid
void MainWindow::on_pushButton_lsScan_clicked()
{
    qPath = ui->lineEdit_4->text();
    const QString gridName = ui->lineEdit_lsGrid->text().trimmed(), lineName = ui->lineEdit_lsLines->text().trimmed(),
                  outName = ui->lineEdit_lsOut->text().trimmed();
    if(gridName.isEmpty() || lineName.isEmpty() || outName.isEmpty()){
        reportError("Error 56: Line width scan: file name of the orbit grid, the line list or the result missing (Optimisation tab).");
        return;
    }
    if(!ui->checkBox_4->isChecked() || ui->checkBox_2->isChecked() || ui->checkBox->isChecked()){
        reportError("Error 56: Line width scan: needs two components with the orbit from orbital elements (Data tab).");
        return;
    }
    std::vector<std::array<double, 7>> orbits;
    std::string err;
    if(!linewidth::readOrbitGrid((qPath+"/"+gridName).toStdString(), orbits, err) || orbits.empty()){
        reportError("Error 56: Line width scan: "+(err.empty() ? QString("the orbit grid is empty.") : QString::fromStdString(err)));
        return;
    }
    linewidth::Params prm;
    std::vector<double> linesA, linesB;
    if(!linewidth::readLineList((qPath+"/"+lineName).toStdString(), prm, linesA, linesB, err)){
        reportError("Error 56: Line width scan: line list "+qPath+"/"+lineName+": "+QString::fromStdString(err)+" Create it with Find Lines.");
        return;
    }
    std::ofstream out((qPath+"/"+outName).toStdString().c_str());
    if(!out){
        reportError("Error 52: Cannot write "+qPath+"/"+outName+".");
        return;
    }
    abortt = 0;
    this->setCursor(QCursor(Qt::WaitCursor));
    MainWindow::disableButtons();
    ui->pushButton_lsFind->setEnabled(false);
    ui->pushButton_lsScan->setEnabled(false);
    auto finish = [this](){
        MainWindow::enableButtons();
        ui->pushButton_lsFind->setEnabled(true);
        ui->pushButton_lsScan->setEnabled(true);
        this->setCursor(QCursor(Qt::ArrowCursor));
    };
    MainWindow::read_data();
    if(error==1){
        finish();
        error = 0;
        return;
    }
    const std::vector<double> keep(orbele.begin(), orbele.end());
    std::vector<linewidth::ScanRow> rows;
    const int n = int(orbits.size());
    const bool saveSpectra = ui->checkBox_lsSave->isChecked();
    const int dot = outName.lastIndexOf('.');
    const QString spectraExt = dot>0 ? outName.mid(dot) : QString(".dat");
    const QString spectraBase = qPath+"/"+(dot>0 ? outName.left(dot) : outName);
    ui->progressBar_lsScan->setMaximum(n);
    ui->progressBar_lsScan->setValue(0);
    for(int k=0; k<n; k++){
        linewidth::ScanRow row;
        for(int e=0; e<7; e++){ orbele[e] = orbits[k][e]; row.el[e] = orbits[k][e]; }
        if(orbit::elementsValid(orbele[0], orbele[1])){
            row.residuum = MainWindow::evaluateOrbit();
            std::vector<double> lam, fa, fb;
            if(error==0 && std::isfinite(row.residuum) && lineScanSpectra(lam, fa, fb, true)){
                row.m = linewidth::measure(lam, fa, fb, linesA, linesB, prm);
                row.ok = row.m.ok;
                if(saveSpectra){
                    // continuum-normalised A and B of this orbit (as the Output tab option)
                    const arma::vec Xc = componentSpectra();
                    std::vector<double> wc[2], vc[2];
                    std::vector<int> cc[2];
                    for(int part=0; part<2; part++){
                        const int nn = part==0 ? g_layout.nA : g_layout.nB;
                        const int start = part==0 ? 0 : g_layout.startB();
                        for(int i=0; i<nn; i++){
                            wc[part].push_back(std::pow(10.0, dis::columnLog10Lambda(g_layout, part==0 ? 'A' : 'B', i, W(0), g_dlog)));
                            vc[part].push_back(Xc(start+i));
                            cc[part].push_back(start+i);
                        }
                    }
                    if(wc[0].size()>=3 && wc[1].size()>=3){
                        const QString tag = QString("_%1").arg(k+1, 3, 10, QChar('0'));
                        const arma::vec res = M*X;
                        writeRenormalised(res, Xc, wc, vc, cc, spectraBase+tag+"_A"+spectraExt, spectraBase+tag+"_B"+spectraExt,
                                          ui->doubleSpinBox_normKnot->value(), ui->doubleSpinBox_normLow->value());
                    }
                }
            }
        }
        error = 0;
        rows.push_back(row);
        ui->spinBox_4->setValue(k+1);
        ui->progressBar_lsScan->setValue(k+1);
        qApp->processEvents(QEventLoop::AllEvents);
        if(abortt==1){
            abortt = 0;
            break;
        }
    }
    for(int e=0; e<7; e++) orbele[e] = keep[e];
    MainWindow::evaluateOrbit();     // the solution of the elements before the scan
    finish();

    out.close();
    linewidth::ScanInfo info;
    info.linesFile = lineName.toStdString();
    const QString profName = spectraBase+"_profiles"+spectraExt;
    if(!linewidth::writeScan((qPath+"/"+outName).toStdString(), info, rows, err)
            || !linewidth::writeProfiles(profName.toStdString(), rows, err))
        reportError("Error 52: "+QString::fromStdString(err));
    const QString m = QString("Line width scan: %1 of %2 orbits, %3 lines in A, %4 in B, written to %5.")
                          .arg(rows.size()).arg(n).arg(linesA.size()).arg(linesB.size()).arg(qPath+"/"+outName);
    cout<<m.toStdString()<<endl;
    if(runow==0){
        ui->plainTextEdit_2->appendPlainText(m);
        auto *dlg = new LineScanDialog(this);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->setRows(rows, qPath+"/"+outName);
        dlg->show();
    }
}

// Optimisation tab, Line width scan: Show results (of an earlier scan, from the files)
void MainWindow::on_pushButton_lsShow_clicked()
{
    qPath = ui->lineEdit_4->text();
    const QString outName = ui->lineEdit_lsOut->text().trimmed();
    if(outName.isEmpty()){
        reportError("Error 56: Line width scan: no file name for the result (Optimisation tab).");
        return;
    }
    linewidth::ScanInfo info;
    std::vector<linewidth::ScanRow> rows;
    std::string err;
    if(!linewidth::readScan((qPath+"/"+outName).toStdString(), info, rows, err)){
        reportError("Error 56: Line width scan: "+QString::fromStdString(err)+" Run Scan orbits first.");
        return;
    }
    const int dot = outName.lastIndexOf('.');
    const QString profName = qPath+"/"+(dot>0 ? outName.left(dot)+"_profiles"+outName.mid(dot) : outName+"_profiles");
    std::string perr;
    const bool haveProfiles = linewidth::readProfiles(profName.toStdString(), rows, perr);
    if(!haveProfiles){
        cout<<("Line width scan: no profiles ("+perr+")")<<endl;
        for(auto &r : rows){ r.m.dv.clear(); r.m.a.prof.clear(); r.m.a.err.clear(); r.m.b.prof.clear(); r.m.b.err.clear(); }
    }
    auto *dlg = new LineScanDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setRows(rows, qPath+"/"+outName);
    dlg->show();
}

void MainWindow::on_pushButton_telldetect_clicked()
{
    if(!g_matrixValid || g_layout.nT<3 || X.n_elem!=arma::uword(g_layout.cols()) || W.n_elem<2){
        reportError("Error 50: Detect needs a Solve with static lines first.");
        return;
    }
    const arma::vec Xc = componentSpectra();
    const double w0 = W(0);
    const int n = g_layout.nT;
    std::vector<double> lam(n), t(n);
    for(int k=0; k<n; k++){
        lam[k] = std::pow(10.0, dis::columnLog10Lambda(g_layout, 'T', k, w0, g_dlog));
        t[k] = Xc(g_layout.startT()+k);
    }
    // running median over 5 A
    const double pxA = lam[n/2]*(std::pow(10.0, g_dlog)-1.0);
    const int h = std::max(2, int(std::ceil(2.5/pxA)));
    std::vector<double> hp(n), buf;
    for(int k=0; k<n; k++){
        const int a = std::max(0, k-h), b = std::min(n, k+h+1);
        buf.assign(t.begin()+a, t.begin()+b);
        std::nth_element(buf.begin(), buf.begin()+buf.size()/2, buf.end());
        hp[k] = t[k]-buf[buf.size()/2];
    }
    std::vector<double> ab(n);
    for(int k=0; k<n; k++) ab[k] = std::fabs(hp[k]);
    std::nth_element(ab.begin(), ab.begin()+n/2, ab.end());
    const double sigma = 1.4826*ab[n/2];
    const double thr = ui->doubleSpinBox_telldet->value()*sigma;
    std::vector<std::pair<double, double>> r;
    for(int k=0; k<n; k++){
        if(!(std::fabs(hp[k])>thr)) continue;
        const double lo = lam[k]-0.5, hi = lam[k]+0.5;
        if(!r.empty() && lo<=r.back().second+1.0) r.back().second = std::max(r.back().second, hi);
        else r.emplace_back(lo, hi);
    }
    QString name = ui->lineEdit_tellrange->text().trimmed();
    if(name.isEmpty()) name = "telluric_static.dat";
    const QString out = withSuffix(name.endsWith("_detected.dat") ? name.left(name.size()-13)+".dat" : name, "_detected");
    std::ofstream f((qPath+"/"+out).toStdString().c_str());
    f<<"# Ranges of the static lines detected in the static spectrum of the last Solve (Spectangular):\n"
     <<"# |static - running median (5 A)| > "<<ui->doubleSpinBox_telldet->value()<<" sigma, sigma = "<<sigma
     <<" (MAD), widened by 0.5 A, gaps < 1 A closed. Observatory frame, air.\n";
    f<<setprecision(8);
    double cover = 0.0;
    for(const auto &x : r){ f<<x.first<<"  "<<x.second<<"\n"; cover += x.second-x.first; }
    if(!f){
        reportError("Error 50: Cannot write "+qPath+"/"+out+".");
        return;
    }
    ui->lineEdit_tellrange->setText(out);
    const QString m = QString("Static lines detected: %1 ranges, %2 A of %3 A (threshold %4 sigma, sigma %5), written to %6 "
                              "and set in the Data tab; tick \"Static lines only in\" and Read data to use them.")
                      .arg(r.size()).arg(cover, 0, 'f', 1).arg(lam[n-1]-lam[0], 0, 'f', 1)
                      .arg(ui->doubleSpinBox_telldet->value()).arg(sigma, 0, 'g', 3).arg(out);
    cout<<m.toStdString()<<endl;
    ui->plainTextEdit_2->appendPlainText(m);
}
