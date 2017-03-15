#ifndef PLOTSPEC_H
#define PLOTSPEC_H

#include <QDialog>

namespace Ui {
class PlotSpec;
}

class PlotSpec : public QDialog
{
    Q_OBJECT

public:
    explicit PlotSpec(QWidget *parent = 0);
    ~PlotSpec();

public slots:
    void seData(QString str, QString str2, QString str3, QString str4);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private slots:

    void on_doubleSpinBox_valueChanged();

    void on_doubleSpinBox_2_valueChanged();

    void on_doubleSpinBox_3_valueChanged();

    void on_doubleSpinBox_4_valueChanged();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_pushButton_4_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_lineEdit_5_textEdited();

    void on_spinBox_valueChanged();

    void on_doubleSpinBox_10_valueChanged();

    void on_doubleSpinBox_12_valueChanged();

    void on_doubleSpinBox_9_valueChanged();

    void on_doubleSpinBox_11_valueChanged();

    void on_pushButton_5_clicked();

    void SNR();

    void EW();

    void showPointToolTip(QMouseEvent *event);

    void writeCoords(QMouseEvent *event);

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::PlotSpec *ui;
};

#endif // PLOTSPEC_H
