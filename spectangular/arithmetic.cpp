#include "arithmetic.h"
#include "ui_arithmetic.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QMessageBox>
#include <QFile>
#include <iostream>

using namespace std;

QString qAPath;
string aPath;

Arithmetic::Arithmetic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Arithmetic)
{
    ui->setupUi(this);

    this->setWindowTitle("Arithmetic");

    ui->lineEdit_4->setText("/home/daniels/Observations/CTIO/BAT99_32");
    qAPath=ui->lineEdit_4->text();
    aPath = qAPath.toUtf8().constData();


}

Arithmetic::~Arithmetic()
{
    delete ui;
}


// A + B
void Arithmetic::on_pushButton_2_clicked()
{
    this->setCursor(QCursor(Qt::WaitCursor));

    QString inputA=ui->lineEdit->text();
    string dataA = inputA.toUtf8().constData();
    std::ostringstream datANameStream(dataA);
    datANameStream<<aPath<<"/"<<dataA;
    std::string datAName = datANameStream.str();
    ifstream inA(datAName.c_str());

    QFile checkfile1(datAName.c_str());

    if(!checkfile1.exists()){
        QMessageBox::information(this, "Error", "File"+qAPath+"/"+inputA+" does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
       return;
    }

    QString inputB=ui->lineEdit_2->text();
    string dataB = inputB.toUtf8().constData();
    std::ostringstream datBNameStream(dataB);
    datBNameStream<<aPath<<"/"<<dataB;
    std::string datBName = datBNameStream.str();
    ifstream inB(datBName.c_str());

    QFile checkfile2(datBName.c_str());

    if(!checkfile1.exists()){
        QMessageBox::information(this, "Error", "File"+qAPath+"/"+inputB+" does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
       return;
    }

    int numpixA=0;
    int numpixB=0;
    string line, eins, zwei;

    while(std::getline(inA, line))
       ++numpixA;

    inA.clear();
    inA.seekg(0, ios::beg);

    while(std::getline(inB, line))
       ++numpixB;

    inB.clear();
    inB.seekg(0, ios::beg);

    /*
    if(numpixA!=numpixB){
        QMessageBox::information(this, "Error", "Unequal length of data.");
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;
    }*/

    QVector<double> a(numpixA), b(numpixA), c(numpixB), d(numpixB), e(numpixA), f(numpixA);

    for(int g=0; g<numpixA; g++){
    inA >> eins >>zwei;
    istringstream ist(eins);
    ist >> a[g];
    e[g]=a[g];
    istringstream ist2(zwei);
    ist2 >> b[g];
    }

    inA.close();

    for(int g=0; g<numpixB; g++){
    inB >> eins >>zwei;
    istringstream ist3(eins);
    ist3 >> c[g];
    istringstream ist4(zwei);
    ist4 >> d[g];
    }

    QString output=ui->lineEdit_3->text();
    string out = output.toUtf8().constData();
    std::ostringstream outNameStream(out);
    outNameStream<<aPath<<"/"<<out;
    std::string outName = outNameStream.str();
    ofstream outp(outName.c_str());

    for(int g=0; g<numpixA; g++){
        if(ui->checkBox->isChecked()){
        f[g]=b[g]+d[g];
        }
        if(ui->checkBox_2->isChecked()){
        f[g]=b[g]-d[g];
        }
        if(ui->checkBox_3->isChecked()){
        f[g]=b[g]*d[g];
        }
        if(ui->checkBox_4->isChecked()){
        f[g]=b[g]/d[g];
        }
        if(ui->checkBox_5->isChecked()){
            QString qValue = ui->lineEdit_5->text();
            bool ok = false;
            double value = qValue.toDouble(&ok);
        f[g]=b[g]+value;
        }
        if(ui->checkBox_6->isChecked()){
            QString qValue = ui->lineEdit_5->text();
            bool ok = false;
            double value = qValue.toDouble(&ok);
        f[g]=b[g]*value;
        }
        outp<<scientific<<e[g]<<" "<<f[g]<<endl;
    }


    this->setCursor(QCursor(Qt::ArrowCursor));
}


//work path
void Arithmetic::on_lineEdit_4_textChanged()
{
    qAPath=ui->lineEdit_4->text();
    aPath = qAPath.toUtf8().constData();
}

void Arithmetic::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
}

void Arithmetic::on_checkBox_2_clicked()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
}

void Arithmetic::on_checkBox_3_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
}

void Arithmetic::on_checkBox_4_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_6->setChecked(false);
}

void Arithmetic::on_checkBox_5_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_6->setChecked(false);
}

void Arithmetic::on_checkBox_6_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
}
