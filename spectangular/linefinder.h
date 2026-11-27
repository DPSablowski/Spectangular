#ifndef LINEFINDER_H
#define LINEFINDER_H

// Windows of the line width scan (Optimisation tab): "Find Line" finds absorption lines in one
// component of the current solution and lets the user add or remove them by a click in the plot;
// "Apply" writes the line list. The result window shows the width of the lines of the other
// component for every orbit of the scan.

#include "linewidth.h"

#include <QDialog>

#include <array>
#include <string>
#include <vector>

class QCPGraph;
class QCustomPlot;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QMouseEvent;
class QPushButton;
class QSpinBox;
class QTableWidget;

class LineFinderDialog : public QDialog
{
    Q_OBJECT

public:
    struct Data {
        std::vector<double> lambda;      // wavelengths of the grid [A]
        std::vector<double> fluxA, fluxB;   // the component spectra
        QString file;                    // line list written by Apply
        linewidth::Params params;
        std::vector<double> linesA, linesB;   // lines of an existing file
    };
    explicit LineFinderDialog(QWidget *parent = nullptr);
    void setData(const Data &d);
    std::vector<double> lines(int comp) const { return lineLists[comp ? 1 : 0]; }   // 0: A, 1: B

    // add a line at a wavelength (snapped to the minimum of the flux nearby) / remove the line
    // closest to it within tolerance (A); both are what a click in the plot does
    Q_INVOKABLE bool addLineAt(double lambda);
    Q_INVOKABLE bool removeLineNear(double lambda, double tolerance);

signals:
    void applied(const QString &file);

public slots:
    void on_pushButton_find_clicked();
    void on_pushButton_apply_clicked();

private:
    void collectParams();
    void showSpectrum();
    void showLines();
    void rangeFromSpins();
    void spinsFromRange();
    void plotClicked(QMouseEvent *e);
    const std::vector<double> &refFlux() const;

    Data d;
    std::vector<double> lineLists[2];
    std::vector<double> &cur() { return lineLists[curComp]; }
    double percent[2] = {90.0, 90.0};
    int curComp = 0;
    QComboBox *cFind;
    QSpinBox *sEnv, *sSep;
    QDoubleSpinBox *sPRef, *sDepth, *sLamLo, *sLamHi;
    QDoubleSpinBox *sX0, *sX1, *sY0, *sY1;
    QPushButton *bFind, *bApply, *bFull;
    QLabel *lCount, *lFile;
    QCustomPlot *plot;
    QCPGraph *gSpec, *gLines;
    bool updatingSpins = false;
};

class LineScanDialog : public QDialog
{
    Q_OBJECT

public:
    typedef linewidth::ScanRow Row;
    explicit LineScanDialog(QWidget *parent = nullptr);
    void setRows(const std::vector<Row> &rows, const QString &file);
    // mean line profile and Gaussian fit of both components for one orbit (index from 0)
    Q_INVOKABLE void selectOrbit(int k);

private:
    void showPlot();
    void showProfile(int k);
    std::vector<Row> rows;
    QComboBox *cX;
    QCustomPlot *plot, *plotProf;
    QTableWidget *table;
    QLabel *lInfo, *lProf;
};

#endif
