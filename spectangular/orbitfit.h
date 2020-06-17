#ifndef ORBITFIT_H
#define ORBITFIT_H

#include <QDialog>

namespace Ui {
class OrbitFit;
}

class OrbitFit : public QDialog
{
    Q_OBJECT

public:
    explicit OrbitFit(QWidget *parent = 0);
    ~OrbitFit();

private slots:
    void on_pushButton_2_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_clicked();

    void on_lineEdit_2_editingFinished();

private:
    Ui::OrbitFit *ui;
};

#endif // ORBITFIT_H
