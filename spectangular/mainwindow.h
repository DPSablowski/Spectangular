#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void Input();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_2_clicked();

    void read_data();

    void on_pushButton_3_clicked();

    void on_actionSpectrum_Plotter_triggered();

    void on_actionAbout_triggered();

    void on_actionBug_Report_triggered();

    void on_pushButton_5_clicked();

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

    void ComputeDifferences();

    void Optimisation();

    void on_pushButton_4_clicked();

    void on_checkBox_34_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void findroot();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_spinBox_8_valueChanged();

    void on_checkBox_22_clicked();

    void on_checkBox_20_clicked();

    void on_checkBox_21_clicked();

    void on_checkBox_24_clicked();

    void on_checkBox_25_clicked();

    void showPointToolTip(QMouseEvent *event);

    void showPointToolTip_3(QMouseEvent *event);

private:
    Ui::MainWindow *ui;
    PlotSpec *pSpec;
    correlation *qCorr;
    Edit *qEdit;
    Arithmetic *qArith;
};

#endif // MAINWINDOW_H
