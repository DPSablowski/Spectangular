#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <QDialog>

namespace Ui {
class Arithmetic;
}

class Arithmetic : public QDialog
{
    Q_OBJECT

public:
    explicit Arithmetic(QWidget *parent = 0);
    ~Arithmetic();

private slots:
    void on_pushButton_2_clicked();

    void on_lineEdit_4_textChanged();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

    void on_checkBox_6_clicked();

private:
    Ui::Arithmetic *ui;
};

#endif // ARITHMETIC_H
