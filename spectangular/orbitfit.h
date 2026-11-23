#ifndef ORBITFIT_H
#define ORBITFIT_H

// Dialog Tools -> Orbit fit: Keplerian orbit fitted to radial velocities (module rvfit),
// e.g. to the RVs of an optimisation of individual RVs.

#include <QDialog>
#include <QString>
#include <vector>

#include "rvfit.h"

namespace Ui {
class OrbitFit;
}

class OrbitFit : public QDialog
{
    Q_OBJECT

public:
    explicit OrbitFit(QWidget *parent = nullptr);
    ~OrbitFit();

    // work folder and the data of the main window (times, current RVs); valid = false if
    // the main window has no times or RVs
    void setMainData(const QString &workPath, const std::vector<double> &t, const std::vector<double> &rvA,
                     const std::vector<double> &rvB, bool sb2, bool valid, const QString &note,
                     bool offsetB = false);

    // name of the orbital elements file (Data tab of the main window); before setMainData
    void setOrbitFile(const QString &f){ orbitFile = f.isEmpty() ? QString("orbitelements.dat") : f; }

    const rvfit::Result &result() const { return res; }

public slots:
    void on_pushButton_2_clicked();          // Fit
    void on_pushButton_load_clicked();       // start values from the orbital elements file
    void on_pushButton_write_clicked();      // result to the orbital elements file

private slots:
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_main_clicked();
    void on_checkBox_file_clicked();

private:
    QString orbitFile = "orbitelements.dat";
    bool collectData(rvfit::Data &d, QString &err);
    void plotResult(const rvfit::Data &d);
    void writeResultFile(const rvfit::Data &d);

    Ui::OrbitFit *ui;
    std::vector<double> mainT, mainA, mainB;
    bool mainSb2 = true, mainValid = false;
    rvfit::Result res;
};

#endif // ORBITFIT_H
