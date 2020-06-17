#ifndef SIMPLESUBTRACTION_H
#define SIMPLESUBTRACTION_H

#include <QDialog>

namespace Ui {
class SimpleSubtraction;
}

class SimpleSubtraction : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleSubtraction(QWidget *parent = 0);
    ~SimpleSubtraction();

private slots:
    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged();

private:
    Ui::SimpleSubtraction *ui;
};

#endif // SIMPLESUBTRACTION_H
