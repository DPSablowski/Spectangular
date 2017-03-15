#include "orbitfit.h"
#include "ui_orbitfit.h"
#include <QFile>
#include <sstream>
#include <fstream>
#include <QMessageBox>
using namespace std;

QString qOFPath;
string OFPath;

OrbitFit::OrbitFit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OrbitFit)
{
    ui->setupUi(this);
    this->setWindowTitle("Fit Orbital Paramters");

    ui->lineEdit_2->setText("/home/daniels/Disentangling/Artificial/ErrorTest/SN100");
    qOFPath=ui->lineEdit_2->text();
    OFPath = qOFPath.toUtf8().constData();
}

OrbitFit::~OrbitFit()
{
    delete ui;
}

void OrbitFit::on_pushButton_2_clicked()
{
    qOFPath=ui->lineEdit_2->text();
    OFPath = qOFPath.toUtf8().constData();

    // SB1 data
    if(ui->checkBox->isChecked()){

        QString input=ui->lineEdit->text();
        string in = input.toUtf8().constData();
        std::ostringstream datNameStream(in);
        datNameStream<<OFPath<<"/"<<in;
        std::string datName = datNameStream.str();
        ifstream inp(datName.c_str());

        QFile checkfile(datName.c_str());

        if(!checkfile.exists()){
            QMessageBox::information(this, "Error", "File"+qOFPath+"/"+input+" does not exist!");
            this->setCursor(QCursor(Qt::ArrowCursor));
           return;
        }

        int numpix=0;

        string line, eins, zwei;

        while(std::getline(inp, line))
           ++numpix;

        inp.clear();
        inp.seekg(0, ios::beg);

        QVector<double> t(numpix), RV(numpix);

        for(int g=0; g<numpix; g++){
        inp >> eins >>zwei;
        istringstream ist(eins);
        ist >> t[g];
        istringstream ist2(zwei);
        ist2 >> RV[g];
        }

        inp.close();

    }

    // SB2 data
    if(ui->checkBox_2->isChecked()){

        QString input=ui->lineEdit->text();
        string in = input.toUtf8().constData();
        std::ostringstream datNameStream(in);
        datNameStream<<OFPath<<"/"<<in;
        std::string datName = datNameStream.str();
        ifstream inp(datName.c_str());

        QFile checkfile(datName.c_str());

        if(!checkfile.exists()){
            QMessageBox::information(this, "Error", "File"+qOFPath+"/"+input+" does not exist!");
            this->setCursor(QCursor(Qt::ArrowCursor));
           return;
        }

        int numpix=0;

        string line, eins, zwei, drei;

        while(std::getline(inp, line))
           ++numpix;

        inp.clear();
        inp.seekg(0, ios::beg);

        QVector<double> t(numpix), RV1(numpix), RV2(numpix);

        for(int g=0; g<numpix; g++){
        inp >> eins >>zwei>>drei;
        istringstream ist(eins);
        ist >> t[g];
        istringstream ist2(zwei);
        ist2 >> RV1[g];
        istringstream ist3(drei);
        ist3>>RV2[g];
        }

        inp.close();

    }
}



void OrbitFit::on_checkBox_2_clicked()
{
    ui->checkBox->setChecked(false);
}


void OrbitFit::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(false);
}


void OrbitFit::on_lineEdit_2_editingFinished()
{
    qOFPath=ui->lineEdit_2->text();
    OFPath = qOFPath.toUtf8().constData();
}
