#ifndef FLUXTEMPLATE_H
#define FLUXTEMPLATE_H

// Dialog Tools -> Flux ratio from synthetic spectra: fits the flux ratio of the components
// with a synthetic spectrum for each (module templatefit) and renormalises the disentangled
// spectra to their own continua.

#include <QDialog>
#include <QString>

#include "fluxratio.h"
#include "templatefit.h"

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;
class QCheckBox;
class QLabel;
class QPushButton;
class QCustomPlot;

class FluxTemplate : public QDialog
{
    Q_OBJECT

public:
    explicit FluxTemplate(QWidget *parent = nullptr);

    // work folder, component files (relative to it), ratio of the disentangling and its
    // wavelength dependence
    void setMainData(const QString &workPath, const QString &compA, const QString &compB,
                     double k0, const fr::Model &m);

    const tfit::Result &result() const { return res; }

signals:
    void ratioAccepted(double k);

public slots:
    void on_pushButton_fit_clicked();
    void on_pushButton_write_clicked();
    void on_pushButton_use_clicked();

private:
    QString path(const QString &f) const;
    QLineEdit *fileRow(class QGridLayout *g, int row, const QString &label, const QString &name,
                       const QString &tip);
    void plot();

    QString work;
    fr::Model model;
    tfit::Spectrum A, B;
    tfit::Result res;

    QLabel *labelWork = nullptr, *labelModel = nullptr, *labelResult = nullptr;
    QLineEdit *eCompA = nullptr, *eCompB = nullptr, *eSynA = nullptr, *eSynB = nullptr;
    QDoubleSpinBox *k0 = nullptr, *l1 = nullptr, *l2 = nullptr, *vA = nullptr, *vB = nullptr,
                   *vmax = nullptr, *vsA = nullptr, *vsB = nullptr, *resol = nullptr;
    QCheckBox *fitA = nullptr, *fitB = nullptr;
    QSpinBox *edge = nullptr, *deg = nullptr;
    QPushButton *bFit = nullptr, *bWrite = nullptr, *bUse = nullptr;
    QCustomPlot *plotA = nullptr, *plotB = nullptr;
};

#endif // FLUXTEMPLATE_H
