#include "simplesubtraction.h"
#include "ui_simplesubtraction.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QMessageBox>
#include <QFile>
#include <iostream>
#include <CCfits/CCfits>
#include <stdio.h>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <QVector>

using namespace std;

string ext, sExtension, sWcol, sIcol, sPath;
QVector<double> w1(1), w2(1), i1(1), i2(1), CA(1), CB(1);
std::valarray<double> swave;
std::valarray<double> sintens;
QString qsExtension, qsWCol, qsICol, qsPath;
double RVA1, RVB1, RVA2, RVB2;
int length=0;

SimpleSubtraction::SimpleSubtraction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleSubtraction)
{
    ui->setupUi(this);

    this->setWindowTitle("Decomposition via Subtraction");

    ui->comboBox->addItem("fits");
    ui->comboBox->addItem("txt");
    ui->comboBox->addItem("dat");

    ui->lineEdit_8->setText("/home/daniels/Disentangling/");
}

SimpleSubtraction::~SimpleSubtraction()
{
    delete ui;
}

//***************************************
//subtract spectra
//***************************************
void SimpleSubtraction::on_pushButton_2_clicked()
{
    //read data from fits table
    if(ui->comboBox->currentIndex()==0){
        QString input=ui->lineEdit->text();
        string data = input.toUtf8().constData();
        std::ostringstream dat1NameStream(data);
        dat1NameStream<<sPath<<"/"<<data<<".fits";
        std::string dat1Name = dat1NameStream.str();

        QFile checkfile1(dat1Name.c_str());

        if(!checkfile1.exists()){
            qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "Error 5: Spectrum "+qsPath+"/"+input+".fits does not exist!");
           return;
        }

        QString input2=ui->lineEdit_2->text();
        string data2 = input2.toUtf8().constData();
        std::ostringstream dat2NameStream(data2);
        dat2NameStream<<sPath<<"/"<<data2<<".fits";
        std::string dat2Name = dat2NameStream.str();

        QFile checkfile2(dat2Name.c_str());

        if(!checkfile1.exists()){
            qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "Error 5: Spectrum "+qsPath+"/"+input2+".fits does not exist!");
           return;
        }

        CCfits::FITS::setVerboseMode(true);

        try
        {

            //open file for reading
            shared_ptr<CCfits::FITS> input_file(new CCfits::FITS(dat1Name.c_str(),CCfits::Read,true));

            // Create pointer to extension
                CCfits::ExtHDU& datavector = input_file->extension(sExtension);

              // Read rows
              CCfits::Column& column = datavector.column(sWcol);
              column.read(swave, 1, column.rows());

              // Read rows
              CCfits::Column& column2 = datavector.column(sIcol);
              column2.read(sintens, 1, column2.rows());

              w1.resize(swave.size());
              i1.resize(swave.size());

              for(int i=0; i<swave.size(); i++){
                w1[i]=swave[i];
                i1[i]=sintens[i];
              }


        }
            catch (CCfits::FitsException&)

             {
              std::cerr << " Fits Exception Thrown by test function \n";
              }

        try
        {

            //open file for reading
            shared_ptr<CCfits::FITS> input_file(new CCfits::FITS(dat2Name.c_str(),CCfits::Read,true));

            // Create pointer to extension
                CCfits::ExtHDU& datavector = input_file->extension(sExtension);

              // Read rows
              CCfits::Column& column = datavector.column(sWcol);
              column.read(swave, 1, column.rows());

              // Read rows
              CCfits::Column& column2 = datavector.column(sIcol);
              column2.read(sintens, 1, column2.rows());

              w2.resize(swave.size());
              i2.resize(swave.size());

              for(int i=0; i<swave.size(); i++){
                w2[i]=swave[i];
                i2[i]=sintens[i];
              }


        }
            catch (CCfits::FitsException&)

             {
              std::cerr << " Fits Exception Thrown by test function \n";
              }


           return;
    }
    //read data from txt file
    if(ui->comboBox->currentIndex()!=0){

        string line, eins, zwei;

        if(ui->comboBox->currentIndex()==1){
            ext=".txt";
        }
        else ext=".dat";

        QString input=ui->lineEdit->text();
        string data = input.toUtf8().constData();
        std::ostringstream dat1NameStream(data);
        dat1NameStream<<sPath<<"/"<<data<<ext;
        std::string dat1Name = dat1NameStream.str();
        ifstream dat(dat1Name.c_str());

        QFile checkfile1(dat1Name.c_str());

        if(!checkfile1.exists()){
            qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "Error 5: Spectrum "+qsPath+"/"+input+".fits does not exist!");
           return;
        }

        QString input2=ui->lineEdit_2->text();
        string data2 = input2.toUtf8().constData();
        std::ostringstream dat2NameStream(data2);
        dat2NameStream<<sPath<<"/"<<data2<<ext;
        std::string dat2Name = dat2NameStream.str();
        ifstream dat2(dat2Name.c_str());

        QFile checkfile2(dat2Name.c_str());

        if(!checkfile2.exists()){
            qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "Error 5: Spectrum "+qsPath+"/"+input2+".fits does not exist!");
           return;
        }

        length=0;

        while(std::getline(dat, line))
           ++length;

        dat.clear();
        dat.seekg(0, ios::beg);

        w1.resize(length);
        i1.resize(length);

        for(int i=0; i<length; i++){
        dat >> eins >>zwei;
        istringstream ist(eins);
        ist >> w1[i];
        istringstream ist2(zwei);
        ist2 >> i1[i];
        }

        length=0;

        while(std::getline(dat2, line))
           ++length;

        dat2.clear();
        dat2.seekg(0, ios::beg);

        w2.resize(length);
        i2.resize(length);

        for(int i=0; i<length; i++){
            dat >> eins >>zwei;
            istringstream ist3(eins);
            ist3 >> w2[i];
            istringstream ist4(zwei);
            ist4 >> i2[i];
        }

    }

    CA.resize(length);
    CB.resize(length);

    RVA1 = ui->doubleSpinBox->value();
    RVB1 = ui->doubleSpinBox_2->value();
    RVA2 = ui->doubleSpinBox_3->value();
    RVB2 = ui->doubleSpinBox_4->value();

    /*
    for(int i=0; i<length; i++){
        CA[i]=CA[i-RVA1+RVB1+RVA2+RVB2]-w1[i+RVB1+RVA2-RVB2]+w2[i+RVB1];
        CB[i]=CB[i-RVA1+RVB1+RVA2+RVB2]-w2[i-RVA1+RVB1+RVA2]+w2[i+RVA2];
    }

    */
}

void SimpleSubtraction::on_comboBox_currentIndexChanged()
{
    if(ui->comboBox->currentIndex()==0){
        ui->lineEdit_3->setEnabled(true);
        ui->lineEdit_4->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
    }
    else{
        ui->lineEdit_3->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
    }
}
