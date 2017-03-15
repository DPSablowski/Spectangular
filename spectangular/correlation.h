#ifndef CORRELATION_H
#define CORRELATION_H

#include <QDialog>

namespace Ui {
class correlation;
}

class correlation : public QDialog
{
    Q_OBJECT

public:
    explicit correlation(QWidget *parent = 0);
    ~correlation();

public slots:
    void seData(QString str, QString str1);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_4_textEdited();

private:
    Ui::correlation *ui;
};

#endif // CORRELATION_H
