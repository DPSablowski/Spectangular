#ifndef PERIODDIALOG_H
#define PERIODDIALOG_H

// Tools -> Period analysis: generalised Lomb-Scargle periodogram of a quantity per spectrum against
// the time of observation, with spectral window, false-alarm levels, marks of the orbital frequency
// and its harmonics, the highest peaks and the data folded with a selected period. Sources:
//  - residual statistics (diffstatistic.dat): mean, RMS or std of O-M,
//  - analysis of the solution, fit per spectrum (rv_oc.dat): O-C of the RV and strength of the
//    lines of A and B (weighted with the errors),
//  - analysis of the solution, line windows (linestat.dat): mean, RMS and equivalent width of O-M
//    per line.
// The button "Analyse the current solution" asks the main window to write the last two files.

#include <QDialog>
#include <QString>
#include <QStringList>

#include <vector>

#include "periodogram.h"

class QCheckBox;
class QComboBox;
class QCPItemText;
class QCustomPlot;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class PeriodDialog : public QDialog
{
    Q_OBJECT

public:
    struct MainData {
        QString work;                 // work folder
        QString statFile;             // residual statistics (Output tab)
        QString fitFile;              // RV O-C and line strength per spectrum (Output tab)
        QString lineStatFile;         // statistics per line window (Output tab)
        QString timesFile;            // times of the spectra (Data tab), if the file has no time
        int first = 0;                // index of the first spectrum
        double orbitalPeriod = 0.0;   // > 0: marks at f_orb and harmonics
    };
    explicit PeriodDialog(QWidget *parent = nullptr);
    void setMainData(const MainData &d);
    // the main window reports the result of the analysis it was asked for; the files are read again
    void analysisFinished(bool ok);

signals:
    void analyseRequested();

public slots:
    void on_pushButton_compute_clicked();
    void on_pushButton_write_clicked();

private:
    // one quantity per spectrum
    struct Series {
        QString name;                 // text of the "Values" list
        std::vector<int> spec;
        std::vector<double> t, v;
        std::vector<double> sigma;    // standard errors; empty: equal weights
        std::vector<double> clip;     // outliers: values above median + k MAD (the RMS); empty: both sides of v
    };
    bool loadData(QString &err);
    bool loadStatistics(QString &err);
    bool loadFit(QString &err);
    bool loadLines(QString &err);
    bool readRows(const QString &file, QStringList &head, std::vector<QStringList> &rows, QString &err);
    bool timesFromFile(std::vector<Series> &s, QString &err);
    void fillValues(const QString &keep);
    void sourceChanged();
    QString activeFile() const;
    void showPhase(int peakRow);

    MainData md;
    std::vector<Series> series;
    // used points (after leaving out outliers)
    std::vector<double> t, y, wgt;
    std::vector<int> usedSpec;
    period::Periodogram pg;
    std::vector<size_t> peakIdx;

    QLineEdit *eStat = nullptr, *eTimes = nullptr;
    QComboBox *cColumn = nullptr, *cSource = nullptr;
    QCheckBox *cOutliers = nullptr, *cWindow = nullptr;
    QDoubleSpinBox *sClip = nullptr, *sFmin = nullptr, *sFmax = nullptr, *sOver = nullptr;
    QPushButton *bCompute = nullptr, *bWrite = nullptr, *bAnalyse = nullptr;
    QCustomPlot *plotP = nullptr, *plotPhase = nullptr;
    QTableWidget *table = nullptr;
    QLabel *labelResult = nullptr;
};

#endif
