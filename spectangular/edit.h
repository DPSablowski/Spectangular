#ifndef EDIT_H
#define EDIT_H

#include <QDialog>

namespace Ui {
class Edit;
}

class Edit : public QDialog
{
    Q_OBJECT

public:
    explicit Edit(QWidget *parent = 0);
    ~Edit();

public slots:
    void seData(QString str, int g, double k);
    // name of the orbital elements file (Data tab of the main window)
    // (the fields show its elements if it exists, so that "Apply Orbit" does not overwrite
    // them with the defaults)
    void setOrbitFile(const QString &f);

private slots:
    void on_spinBox_valueChanged();

    void on_lineEdit_3_textChanged();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

private:
    QString orbitFile = "orbitelements.dat";
    Ui::Edit *ui;
};

#endif // EDIT_H
