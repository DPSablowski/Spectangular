#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fluxratio.h"
#include <armadillo>
#include "disentangle.h"
#include "neldermead.h"
#include <fstream>

// Result of the per-iteration convergence check (Auto Stop, Reinitiate)
enum class RunAction { Continue, Reinit, Done };
#include <string>
#include <plotspec.h>
#include <correlation.h>
#include <edit.h>
#include <arithmetic.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString loadConfiguration(const QString &file);   // error text or empty
    bool saveConfiguration(const QString &file);
    int runBatch(const QString &configName);

private slots:
    void on_actionSubtract_triggered();
    void on_actionOrbitFit_triggered();
    void on_actionFluxTemplate_triggered();
    void on_actionBaryCorr_triggered();
    void on_actionPeriod_triggered();
    void on_actionOpen_Configuration_triggered();
    void on_actionSave_Configuration_triggered();
    void on_actionSave_Configuration_As_triggered();
    void on_actionQuit_triggered();
    double evaluateOrbit();

    void Input();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_2_clicked();

    void read_data();

    void on_pushButton_3_clicked();

    void on_actionSpectrum_Plotter_triggered();

    void on_actionAbout_triggered();

    void on_actionBug_Report_triggered();


    void on_checkBox_5_clicked();

    void on_checkBox_6_clicked();

    void on_checkBox_7_clicked();

    void on_lineEdit_4_editingFinished();

    void on_actionCorrelation_triggered();

    void on_lineEdit_9_textChanged();

    void on_lineEdit_11_textChanged();

    void on_lineEdit_12_textChanged();

    void on_comboBox_currentIndexChanged();

    void ConstructMatrix();

    double DivideConquer();

    void on_pushButton_6_clicked();
    void on_pushButton_stat_clicked();

    void on_spinBox_3_valueChanged();

    void on_checkBox_9_clicked();

    void on_checkBox_11_clicked();

    void on_doubleSpinBox_4_valueChanged();

    void on_pushButton_7_clicked();

    void on_checkBox_12_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_14_clicked();

    void on_checkBox_15_clicked();

    void on_actionEditor_triggered();

    void on_checkBox_16_clicked();

    void VAmplitudeA();

    void VAmplitudeB();

    void on_actionArithmetic_triggered();

    void on_actionManual_triggered();

    void ErrorEstimation();

    void on_pushButton_8_clicked();


    void Optimisation();

    void on_pushButton_4_clicked();

    void on_checkBox_34_clicked();



    void findroot();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_spinBox_8_valueChanged(int);

    void on_checkBox_22_clicked();

    void on_checkBox_20_clicked();

    void on_checkBox_21_clicked();

    void on_checkBox_24_clicked();

    void on_checkBox_25_clicked();

    void showPointToolTip(QMouseEvent *event);



    void plot_functionvalues();

    void on_checkBox_26_clicked();

    void on_checkBox_28_clicked();

    void disableButtons();

    void enableButtons();

    void on_pushButton_13_clicked();

    double inclinations();

    void on_checkBox_29_clicked();

    void on_checkBox_30_clicked();


    void on_pushButton_15_clicked();
    void on_pushButton_telldetect_clicked();
    void on_pushButton_lsFind_clicked();
    void on_pushButton_lsScan_clicked();
    void on_pushButton_lsShow_clicked();

private:
    void reportError(const QString &msg);
    bool loadComponentWeights();
    bool loadBaryCorr();
    bool loadStaticRanges();
    // file of the ranges of the static lines (Data tab), log10 ranges; default list written for
    // telluric_static.dat if missing; Error 50 otherwise
    bool readStaticRangeFile(std::vector<std::pair<double, double>> &logRanges, std::vector<std::string> *labels,
                             QString &name);
    // rows left out of the residuum only (Model tab, "Exclude the ranges of the static lines")
    bool buildResidualExclusion();
    // Output tab, "Analysis of the solution": RV O-C and line strength per spectrum, statistics
    // per line window; false with a message if there is no solution or a file cannot be used
    bool analyseSolution();
    // time and orbital phase of every spectrum (empty if unknown)
    void spectrumTimes(std::vector<double> &t, std::vector<double> &phase);
    bool fitStaticStrengths(std::vector<std::vector<double>> &w);
    bool fitStrengthsWindowed(int &iters, double &change, QString &note);
    bool setupRatioShape();
    bool freeRVIndices(bool sb1, std::vector<int> &freeIdx, QString &err) const;
    arma::vec componentSpectra() const;
    bool lineScanSpectra(std::vector<double> &lam, std::vector<double> &fa, std::vector<double> &fb, bool report);
    bool lineFinderSpectra(std::vector<double> &lam, std::vector<double> &fa, std::vector<double> &fb, QString &source);
    void plotResidualStatistics();
    bool ratioModelFromUi(fr::Model &m, std::string &err) const;
    void updateRatioControls();
    bool buildDesignMatrix(bool interactive);
    bool solveSystem(dis::Solver solver);
    void resetRunState();
    double knownSigma2() const;
    double sigma2Now(double residuum) const;
    void writeSnrEstimate(const arma::vec &res, int bins);
    std::vector<double> estimateSnr(const arma::vec &res);
    RunAction convergenceStep(nm::ProgressMonitor &mon, double bestS, int nParams, std::ofstream &log,
                              bool allowReinit = true);
    dis::Solver currentSolver() const;
    double computeResiduum(const arma::vec &res, int *nbins = nullptr);
    void writeRenormalised(const arma::vec &res, const arma::vec &Xc, const std::vector<double> wc[2],
                           const std::vector<double> vc[2], const std::vector<int> cc[2],
                           const QString &fileA, const QString &fileB, double knotA, double lowSigma);
    arma::vec residualVector(const arma::vec &res);
    void updateFitTarget();
    void updateRunInfo();
    bool optimiseLM(std::ofstream &LogFile, std::ofstream &soptd);
    void writeSolution(const arma::vec &res, const QString &fA, const QString &fB, const QString &fT,
                       const QString &fDiff, const QString &fErr, const QString &fCompl);
    void solveOnce();
    QString settingValue(const QString &key) const;
    bool setSettingValue(const QString &key, const QString &value);
    bool writeSettings(const QString &fileName);
    QString readSettings(const QString &fileName);
    Ui::MainWindow *ui;
    PlotSpec *pSpec;
    correlation *qCorr;
    Edit *qEdit;
    Arithmetic *qArith;
};

#endif // MAINWINDOW_H
