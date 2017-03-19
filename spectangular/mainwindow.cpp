#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <string>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <QVector>
#include "plotspec.h"
#include <correlation.h>
#include <edit.h>
#include <iostream>
#define ARMA_64BIT_WORD
#include <armadillo>
#include <CCfits/CCfits>
#include <ctime>
#include <chrono>
#include <thread>
#include <QFileInfo>
//#include <openblas/cblas.h>

using namespace std;
using namespace arma;
using namespace arpack;
using namespace blas;
using std::chrono::system_clock;

int reinitiate=0, mini, CDI, maxi, num, checker=0, bso1, bso2, logbin, RV1m, RV3m, RV1a, RV3a, Mn, Mm, elements, bidi=0, error=0, zaehler, abortt=0, eval=0;
int upda, updac, inrein=0;
unsigned int cores;
string path, eins, zwei, line;
QString qPath, qExtension, qWCol, qICol, qInitval, qInitmat, qOptval, qOptmat;
QVector<double> RV1(1), RV2(1), RV3(1), edits(5), Mval1(1), Mval2(1), Mtel(1), otimes(1), orbele(7);
double RV1max, RV3max, dv, residu, RV1min, RV3min, RV1maxi, RV3maxi, difference, RV1amin, RV3amin, r1, Diff, fluxratio, ldiff, Per, T0, ecc, Omega1, gama, K1, K2;
double RVT0, RVt, RVe, RVE, RVP;
const double c0 = 299792.458;
string Extension, WCol, ICol;
std::valarray<double> wave;
std::valarray<double> intens;

double step=1;  //step size for initial points, used: dv
double Gamma=2.0;   //expansion coeff.
double alpha=1.0;   //reflection coeff.
double beta=0.5;    //contraction coeff.
double btot=0.5;    //total contraction coeff.

double dP;
double de;
double dKA;
double dKB;
double dGamma;
double dT0;
double dOmega;

mat M(Mn,Mm);   //transformation matrix
mat Mt(Mm,Mn);  //transpose of transformation matrix
vec X(Mm);      //solution vector of SVD
vec C(Mn);      //measurement vector
vec W(Mm);      //wavelength vector of measurement
vec XW1(Mm);    //wavelength vector component 1
vec XW2(Mm);    //wavelength vector component 2
umat loc(2,Mm); //matrix of positions for sparse SVD
vec val(Mm);    //vector with values for sparse SVD

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Spectangular - Spectral Disentangling via Singular Value Decomposition");

    ui->lineEdit->setText("binaryrv_");
    ui->lineEdit_2->setText("velocities.txt");
    ui->lineEdit_3->setText("spectra2m_");
    ui->lineEdit_18->setText("times.dat");
    ui->lineEdit_4->setText(QDir::currentPath());
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    ui->lineEdit_14->setText("initval_2orb.dat");
    qInitval = ui->lineEdit_14->text();
    ui->lineEdit_15->setText("initmat_2orb.dat");
    qInitmat = ui->lineEdit_15->text();

    ui->lineEdit_16->setText("optval_2orb.dat");
    qOptval = ui->lineEdit_16->text();
    ui->lineEdit_17->setText("optmat_2orb.dat");
    qOptmat = ui->lineEdit_17->text();

    ui->lineEdit_6->setText("0");
    ui->lineEdit_7->setText("0");

    ui->checkBox->setChecked(true);
    ui->checkBox_6->setChecked(true);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_4->setChecked(true);
    ui->checkBox_10->setChecked(true);
    ui->checkBox_11->setChecked(true);
    ui->checkBox_14->setChecked(true);
    ui->checkBox_22->setChecked(false);

    ui->comboBox->addItem("fits");
    ui->comboBox->addItem("ASCII");

    ui->doubleSpinBox->setEnabled(false);
    ui->doubleSpinBox_2->setEnabled(false);

    ui->lineEdit_9->setText("DataVector");
    qExtension=ui->lineEdit_9->text();
    Extension=qExtension.toUtf8().constData();

    ui->lineEdit_11->setText("Arg");
    qWCol=ui->lineEdit_11->text();
    WCol=qWCol.toUtf8().constData();

    ui->lineEdit_12->setText("Fun");
    qICol=ui->lineEdit_12->text();
    ICol=qICol.toUtf8().constData();

    ui->spinBox_3->setValue(3);
    zaehler=ui->spinBox_3->value();

    ui->doubleSpinBox_4->setValue(1);
    fluxratio=ui->doubleSpinBox_4->value();

    ui->lineEdit_5->setText("SN100_2orb_1.txt");
    ui->lineEdit_8->setText("SN100_2orb_2.txt");
    ui->lineEdit_10->setText("tell.txt");
    ui->lineEdit_13->setText("SN100_2orb_diff.txt");
    ui->lineEdit_20->setText("SN100_2orb_error.txt");
    ui->lineEdit_21->setText("logfile_2orb.dat");
    ui->lineEdit_22->setText("Ha_vel_conf");
    ui->lineEdit_23->setText("Ha_vel_conf");
    ui->lineEdit_24->setText("errors.dat");

    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->xAxis->setTickLength(1);
    //ui->customPlot->setBackground(Qt::transparent);
    ui->customPlot->xAxis->setTickPen(QPen(Qt::black));
    ui->customPlot->xAxis->setLabelColor(Qt::black);
    ui->customPlot->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setTickPen(QPen(Qt::black));
    ui->customPlot->yAxis->setLabelColor(Qt::black);
    ui->customPlot->yAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setLabel("rel. Flux");
    ui->customPlot->xAxis->setLabel("Wavelength");

    ui->customPlot_2->xAxis2->setVisible(true);
    ui->customPlot_2->xAxis2->setTickLabels(false);
    ui->customPlot_2->yAxis2->setVisible(true);
    ui->customPlot_2->yAxis2->setTickLabels(false);
    //ui->customPlot_2->setBackground(Qt::transparent);
    ui->customPlot_2->xAxis->setTickPen(QPen(Qt::black));
    ui->customPlot_2->xAxis->setLabelColor(Qt::black);
    ui->customPlot_2->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot_2->yAxis->setTickPen(QPen(Qt::black));
    ui->customPlot_2->yAxis->setLabelColor(Qt::black);
    ui->customPlot_2->yAxis->setTickLabelColor(Qt::black);
    ui->customPlot_2->yAxis->setLabel("rel. Flux");
    ui->customPlot_2->xAxis->setLabel("Wavelength");

    ui->customPlot_2->hide();

    ui->customPlot_3->xAxis2->setVisible(true);
    ui->customPlot_3->xAxis2->setTickLabels(false);
    ui->customPlot_3->yAxis2->setVisible(true);
    ui->customPlot_3->yAxis2->setTickLabels(false);
    //ui->customPlot_3->setBackground(Qt::transparent);
    ui->customPlot_3->xAxis->setTickPen(QPen(Qt::black));
    ui->customPlot_3->xAxis->setLabelColor(Qt::black);
    ui->customPlot_3->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot_3->yAxis->setTickPen(QPen(Qt::black));
    ui->customPlot_3->yAxis->setLabelColor(Qt::black);
    ui->customPlot_3->yAxis->setTickLabelColor(Qt::black);
    ui->customPlot_3->yAxis->setLabel("Difference");
    ui->customPlot_3->xAxis->setLabel("Wavelength");

    ui->pushButton->setStyleSheet("QPushButton{background: transparent; border: 1px solid black;}");
    ui->pushButton_2->setStyleSheet("QPushButton{background: transparent; border: 1px solid black;}");
    ui->pushButton_3->setStyleSheet("QPushButton{background: transparent; border: 1px solid black;}");
    ui->pushButton_4->setStyleSheet("QPushButton{background: transparent; border: 1px solid black;}");
    ui->pushButton_5->setStyleSheet("QPushButton{background: transparent; border: 1px solid black;}");
    ui->pushButton_6->setStyleSheet("QPushButton{background: transparent; color: black;  border: 1px solid black;}");
    ui->pushButton_7->setStyleSheet("QPushButton{background: transparent; border: 1px solid black;}");
    ui->pushButton_8->setStyleSheet("QPushButton{background: transparent; color: black;  border: 1px solid black;}");

    ui->doubleSpinBox->setStyleSheet("QDoubleSpinBox{background: transparent; border: 1px solid black;}");
    ui->doubleSpinBox_2->setStyleSheet("QDoubleSpinBox{background: transparent; border: 1px solid black;}");
    ui->doubleSpinBox_3->setStyleSheet("QDoubleSpinBox{background: transparent; border: 1px solid black;}");
    ui->doubleSpinBox_4->setStyleSheet("QDoubleSpinBox{background: transparent; border: 1px solid black;}");
    ui->doubleSpinBox_5->setStyleSheet("QDoubleSpinBox{background: transparent; color:black; border: 1px solid black;}");
    ui->doubleSpinBox_14->setStyleSheet("QDoubleSpinBox{background: transparent; color:black; border: 1px solid black;}");
    ui->doubleSpinBox_15->setStyleSheet("QDoubleSpinBox{background: transparent; color:black; border: 1px solid black;}");

    ui->comboBox->setStyleSheet("QComboBox{background: transparent; color: black;  border: 1px solid black;}");

    ui->spinBox->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_2->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_3->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_4->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_5->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_6->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_7->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_8->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");

    ui->plainTextEdit->setStyleSheet("QPlainTextEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->plainTextEdit_2->setStyleSheet("QPlainTextEdit{background: transparent; color: black; border: 1px solid black;}");

    ui->lineEdit->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_2->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_3->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_4->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_5->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_6->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_7->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_8->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_9->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_10->setStyleSheet("QLineEdit{background: transparent; color: black;   border: 1px solid black;}");
    ui->lineEdit_11->setStyleSheet("QLineEdit{background: transparent; color: black;  border: 1px solid black;}");
    ui->lineEdit_12->setStyleSheet("QLineEdit{background: transparent; color: black;   border: 1px solid black;}");
    ui->lineEdit_13->setStyleSheet("QLineEdit{background: transparent; color: black;   border: 1px solid black;}");
    ui->lineEdit_14->setStyleSheet("QLineEdit{background: transparent; color: black;   border: 1px solid black;}");
    ui->lineEdit_15->setStyleSheet("QLineEdit{background: transparent; color: black;   border: 1px solid black;}");
    ui->lineEdit_16->setStyleSheet("QLineEdit{background: transparent; color: black;   border: 1px solid black;}");
    ui->lineEdit_17->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_18->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_19->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_20->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_21->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");

    ui->label->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_2->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_3->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_4->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_5->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_6->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_7->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_8->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_10->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_11->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_12->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_13->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_14->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_15->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_16->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_17->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_18->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_19->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_20->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_21->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_22->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_23->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_24->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_25->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_26->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_27->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_28->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_29->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_30->setStyleSheet("QLabel{background: transparent; color: black;}");
    ui->label_31->setStyleSheet("QLabel{background: transparent; color: black;}");

    ui->checkBox->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_2->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_3->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_4->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_5->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_6->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_7->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_8->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_9->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_10->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_11->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_12->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_13->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_14->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_15->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_16->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_17->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_18->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_32->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_33->setStyleSheet("QCheckBox{background: transparent; color: black;}");
    ui->checkBox_35->setStyleSheet("QCheckBox{background: transparent; color: black;}");    

    cores = thread::hardware_concurrency();
    ui->spinBox_7->setValue(cores);
    ui->spinBox_8->setValue(cores);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//**********************************************************
//read data
//**********************************************************
void MainWindow::read_data(){

    ui->plainTextEdit->clear();

    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    int entries=0;
    string liner;

    // Read SB2 RV Data
        if(ui->checkBox_4->isChecked()){
        //from CroCo
        if(ui->checkBox->isChecked()){
            checker=2;
            orbele[4]=0;
            mini=ui->spinBox->value();
            maxi=ui->spinBox_2->value();
            num=maxi-mini+1;

            RV1.resize(num);
            RV2.resize(num);
            RV3.resize(num);

            for(int g=mini; g<=maxi; g++){

            QString input=ui->lineEdit->text();
            string data1 = input.toUtf8().constData();
            std::ostringstream dat1NameStream(data1);
            dat1NameStream<<path<<"/"<<data1<<g<<".txt";
            std::string dat1Name = dat1NameStream.str();

            QFile checkfile1(dat1Name.c_str());

            if(!checkfile1.exists()){
                qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
                QString fError= QString::number(g);
                QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+fError+".txt does not exist!");
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
                return;
            }

            ifstream dat1(dat1Name.c_str());

            dat1 >> eins >>zwei;
            istringstream ist(eins);
            ist >> RV1[g-mini];
            istringstream ist2(zwei);
            ist2 >> RV2[g-mini];
            RV3[g-mini]=RV1[g-mini]+RV2[g-mini];
            if(ui->checkBox_13->isChecked()){
            RV1[g-mini]=-RV1[g-mini];
            RV3[g-mini]=-RV3[g-mini];
            }

            QString index= QString::number(g);
            QString velocity=QString::number(RV1[g-mini]);
            QString doppler=QString::number(RV2[g-mini]);
            QString sum=QString::number(RV3[g-mini]);
            ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+doppler+" "+sum);

            }
        }
        //from your file
        if(ui->checkBox_2->isChecked()){
        checker=2;
        orbele[4]=0;
        mini=ui->spinBox->value();
        maxi=ui->spinBox_2->value();
        num=maxi-mini+1;

        RV1.resize(num);
        RV2.resize(num);
        RV3.resize(num);

        QString input2=ui->lineEdit_2->text();
        string data2 = input2.toUtf8().constData();
        std::ostringstream dat2NameStream(data2);
        dat2NameStream<<path<<"/"<<data2;
        std::string dat2Name = dat2NameStream.str();

        QFile checkfile2(dat2Name.c_str());

        if(!checkfile2.exists()){
            qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "Error 2: File "+qPath+"/"+input2+" does not exist!");
            this->setCursor(QCursor(Qt::ArrowCursor));
            error=1;
           return;
        }

        ifstream dat2(dat2Name.c_str());

        entries=0;
        while(std::getline(dat2, liner))
           ++entries;

        dat2.clear();
        dat2.seekg(0, ios::beg);

        if(entries!=num){
            QMessageBox::information(this, "Error", "Time file "+qPath+"/"+input2+" does not match to the number of observations.");
            error=1;
            this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }


        for(int g=mini; g<=maxi; g++){
        dat2 >> eins >>zwei;
        istringstream ist(eins);
        ist >> RV1[g-mini];
        istringstream ist2(zwei);
        ist2 >> RV3[g-mini];
        QString index= QString::number(g-mini);
        QString velocity=QString::number(RV1[g-mini]);
        //QString doppler=QString::number(RV2[g]);
        QString sum=QString::number(RV3[g-mini]);
        ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+sum);

                    }
        }

        //unknown RVs
        if(ui->checkBox_16->isChecked()){
            checker=2;
            mini=ui->spinBox->value();
            maxi=ui->spinBox_2->value();
            num=maxi-mini+1;

            RV1.resize(num);
            RV2.resize(num);
            RV3.resize(num);
            otimes.resize(num);

            QString inputt=ui->lineEdit_18->text();
            string times = inputt.toUtf8().constData();
            std::ostringstream intNameStream(times);
            intNameStream<<path<<"/"<<times;
            std::string intName = intNameStream.str();

            QFile checkfile2(intName.c_str());

            if(!checkfile2.exists()){
                qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "Error 2: File"+qPath+"/"+inputt+" does not exist!");
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
               return;
            }

            ifstream dat(intName.c_str());

            entries=0;
            while(std::getline(dat, liner))
               ++entries;

            dat.clear();
            dat.seekg(0, ios::beg);

            if(entries!=num){
                QMessageBox::information(this, "Error", "Error 3: Time file "+qPath+"/"+inputt+" does not match to the number of observations.");
                error=1;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }


            for(int g=0; g<num; g++){
                dat >> eins;
                istringstream istr(eins);
                istr >> otimes[g];
            }

            string orbit = "orbitelements.dat";
            std::ostringstream orbNameStream(orbit);
            orbNameStream<<path<<"/"<<orbit;
            std::string orbName = orbNameStream.str();

            QFile checkorb(orbName.c_str());

            if(!checkorb.exists()){
                qDebug()<<"The file "<<checkorb.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "Error 3: File "+checkorb.fileName()+" does not exist! You need to set the orbital elements via Editor.");
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
               return;
            }

            ifstream initorbit(orbName.c_str());

            for(int i=0; i<7; i++){
                initorbit >> eins;
                istringstream istr1(eins);
                istr1 >> orbele[i];
            }

            MainWindow::VAmplitudeA();
            MainWindow::VAmplitudeB();

            for(int i=0; i<num; i++){
                QString index= QString::number(i);
                QString velocity=QString::number(RV1[i]);
                QString sum=QString::number(RV3[i]);
                ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+sum);
                cout<<RV1[i]<<" "<<RV3[i]<<endl;
            }
        }

        //looking for maximum RV absolute
        RV1max=abs(RV1[0]);
        RV3max=abs(RV3[0]);
        for (int i=0; i<num; i++){
        if (abs(RV1[i])>abs(RV1max)){
        RV1max=RV1[i];
        }
        if (abs(RV3[i])>abs(RV3max)){
        RV3max=RV3[i];
        }
        }

        //looking for minimum and maximum RV
        RV1min=RV1max;
        RV3min=RV3max;
        RV1maxi=RV1[0];
        RV3maxi=RV3[0];
        for(int i=0; i<num; i++){
            if(RV1[i]<RV1min){
                RV1min=RV1[i];
            }
            if(RV3[i]<RV3min){
                RV3min=RV3[i];
            }
            if(RV1[i]>RV1maxi){
                RV1maxi=RV1[i];
            }
            if(RV3[i]>RV3maxi){
                RV3maxi=RV3[i];
            }
        }

        RV1amin=RV1[0];
        for(int i=0; i<num; i++){
            if(abs(RV1[i])<RV1amin){
            RV1amin=RV1[i];
            }
        }

}

        // Read SB1 RV Data
        if(ui->checkBox_3->isChecked()){

            //from CroCo
            if(ui->checkBox->isChecked()){
                checker=2;
                orbele[4]=0;
                mini=ui->spinBox->value();
                maxi=ui->spinBox_2->value();
                num=maxi-mini+1;

                RV1.resize(num);

                for(int g=mini; g<=maxi; g++){

                QString input=ui->lineEdit->text();
                string data1 = input.toUtf8().constData();
                std::ostringstream dat1NameStream(data1);
                dat1NameStream<<path<<"/"<<data1<<g<<".txt";
                std::string dat1Name = dat1NameStream.str();

                QFile checkfile1(dat1Name.c_str());

                if(!checkfile1.exists()){
                    qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
                    QString fError= QString::number(g);
                    QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+fError+".txt does not exist!");
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                    return;
                }

                ifstream dat1(dat1Name.c_str());

                dat1 >> eins;
                istringstream ist(eins);
                ist >> RV1[g-mini];

                QString index= QString::number(g);
                QString velocity=QString::number(RV1[g-mini]);

                ui->plainTextEdit->appendPlainText(index+" "+velocity);

                }
            }
            //from your file
            if(ui->checkBox_2->isChecked()){
            checker=2;
            orbele[4]=0;
            mini=ui->spinBox->value();
            maxi=ui->spinBox_2->value();
            num=maxi-mini+1;

            RV1.resize(num);

            QString input2=ui->lineEdit_2->text();
            string data2 = input2.toUtf8().constData();
            std::ostringstream dat2NameStream(data2);
            dat2NameStream<<path<<"/"<<data2;
            std::string dat2Name = dat2NameStream.str();

            QFile checkfile2(dat2Name.c_str());

            if(!checkfile2.exists()){
                qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "Error 2: File"+qPath+"/"+input2+" does not exist!");
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
               return;
            }

            ifstream dat2(dat2Name.c_str());

            entries=0;
            while(std::getline(dat2, liner))
               ++entries;

            dat2.clear();
            dat2.seekg(0, ios::beg);

            if(entries!=num){
                QMessageBox::information(this, "Error", "Time file "+qPath+"/"+input2+" does not match to the number of observations.");
                error=1;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }


            for(int g=0; g<=maxi; g++){
            dat2 >> eins;
            istringstream ist(eins);

            if(g>=mini){
                ist >> RV1[g-mini];
                QString index= QString::number(g);
                QString velocity=QString::number(RV1[g]);
                ui->plainTextEdit->appendPlainText(index+" "+velocity);
                    }
                }
            }

            //unknown RVs
            if(ui->checkBox_16->isChecked()){
                checker=2;
                mini=ui->spinBox->value();
                maxi=ui->spinBox_2->value();
                num=maxi-mini+1;

                RV1.resize(num);
                RV2.resize(num);
                //RV3.resize(num);
                otimes.resize(num);

                QString inputt=ui->lineEdit_18->text();
                string times = inputt.toUtf8().constData();
                std::ostringstream intNameStream(times);
                intNameStream<<path<<"/"<<times;
                std::string intName = intNameStream.str();

                QFile checkfile2(intName.c_str());

                if(!checkfile2.exists()){
                    qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
                    QMessageBox::information(this, "Error", "Error 2: File"+qPath+"/"+inputt+" does not exist!");
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                   return;
                }

                ifstream dat(intName.c_str());

                entries=0;
                while(std::getline(dat, liner))
                   ++entries;

                dat.clear();
                dat.seekg(0, ios::beg);

                if(entries!=num){
                    QMessageBox::information(this, "Error", "Time file "+qPath+"/"+inputt+" does not match to the number of observations.");
                    error=1;
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    return;
                }

                for(int g=0; g<num; g++){
                    dat >> eins;
                    istringstream istr(eins);
                    istr >> otimes[g];
                }

                string orbit = "orbitelements.dat";
                std::ostringstream orbNameStream(orbit);
                orbNameStream<<path<<"/"<<orbit;
                std::string orbName = orbNameStream.str();

                QFile checkorb2(orbName.c_str());

                if(!checkorb2.exists()){
                    qDebug()<<"The file "<<checkorb2.fileName()<<" does not exist.";
                    QMessageBox::information(this, "Error", "Error 3: File "+checkorb2.fileName()+" does not exist! You need to set the orbital elements via Editor.");
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                   return;
                }

                ifstream initorbit(orbName.c_str());

                for(int i=0; i<7; i++){
                    initorbit >> eins;
                    istringstream istr1(eins);
                    istr1 >> orbele[i];
                }

                MainWindow::VAmplitudeA();
                //MainWindow::VAmplitudeB();

                for(int i=0; i<num; i++){
                    QString index= QString::number(i);
                    QString velocity=QString::number(RV1[i]);
                    //QString sum=QString::number(RV3[i]);
                    ui->plainTextEdit->appendPlainText(index+" "+velocity);
                    cout<<RV1[i]<<endl;
                }
            }

            //looking for maximum RV absolute
            RV1max=abs(RV1[0]);

            for (int i=0; i<num; i++){
            if (abs(RV1[i])>abs(RV1max)){
            RV1max=RV1[i];
            }

        }
            RV1amin=RV1[0];
            RV1min=RV1[0];
            for(int i=0; i<num; i++){
                if(abs(RV1[i])<RV1amin){
                RV1amin=RV1[i];
                }
                if(RV1[i]<RV1min){
                    RV1min=RV1[i];
                }
            }
    }

        ui->doubleSpinBox->setValue(RV1max);
        ui->doubleSpinBox_2->setValue(RV3max);

        if(checker==1){
            QMessageBox::information(this,"Choose Input", "Error 3: Please choose input file.");
            this->setCursor(QCursor(Qt::ArrowCursor));
            error=1;
            return;
        }


    if(checker==0){
        QMessageBox::information(this,"Choose Input", "Error 4: Please choose input data from file or unknown (via global optimization).");
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=1;
        return;
    }
    checker=0;

    //read measurements and write to solution vector

    ofstream file1("output.txt");

    QString fext=ui->lineEdit_19->text();
    string sfext = fext.toUtf8().constData();

    //input as fits-file
    if(ui->comboBox->currentIndex()==0){
        this->setCursor(QCursor(Qt::WaitCursor));

        for(int g=mini; g<=maxi; g++){
        QString input3=ui->lineEdit_3->text();
        string data3 = input3.toUtf8().constData();
        std::ostringstream dat3NameStream(data3);
        dat3NameStream<<path<<"/"<<data3<<g<<sfext;
        std::string dat3Name = dat3NameStream.str();
        ifstream dat3(dat3Name.c_str());

        QFile checkfile3(dat3Name.c_str());

        if(!checkfile3.exists()){
            qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
            QString fError= QString::number(g);
            QMessageBox::information(this, "Error", "Error 5: Spectrum "+qPath+"/"+input3+fError+fext+" does not exist!");
            this->setCursor(QCursor(Qt::ArrowCursor));
            error=1;
           return;
        }

        CCfits::FITS::setVerboseMode(true);

        try
        {

            //open file for reading
            auto_ptr<CCfits::FITS> input_file(new CCfits::FITS(dat3Name.c_str(),CCfits::Read,true));

            // Create pointer to extension
                CCfits::ExtHDU& datavector = input_file->extension(Extension);

              // Read rows
              CCfits::Column& column = datavector.column(WCol);
              column.read(wave, 1, column.rows());

              // Read rows
              CCfits::Column& column2 = datavector.column(ICol);
              column2.read(intens, 1, column2.rows());

              logbin=wave.size();

              C.resize(logbin*num);
              W.resize(logbin*num);
              int grar =0;

              for(int i=(g-mini)*logbin; i<(g-mini+1)*logbin; i++){
              W[i]=wave[grar];
              C[i]=intens[grar];
              ++grar;
              }

        }
            catch (CCfits::FitsException&)

             {
              std::cerr << " Fits Exception Thrown by test function \n";
              }

    }

    }

    //input as ASCII-file
    if(ui->comboBox->currentIndex()==1){
    for(int g=mini; g<=maxi; g++){
    QString input3=ui->lineEdit_3->text();
    string data3 = input3.toUtf8().constData();
    std::ostringstream dat3NameStream(data3);
    dat3NameStream<<path<<"/"<<data3<<g<<sfext;
    std::string dat3Name = dat3NameStream.str();
    ifstream dat3(dat3Name.c_str());

    QFile checkfile3(dat3Name.c_str());

    if(!checkfile3.exists()){
        qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
        QString fError= QString::number(g);
        QMessageBox::information(this, "Error", "Error 6: Spectrum "+qPath+"/"+input3+fError+fext+" does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=1;
       return;
    }

    this->setCursor(QCursor(Qt::WaitCursor));

    logbin=0;

    while(std::getline(dat3, line))
       ++logbin;

    dat3.clear();
    dat3.seekg(0, ios::beg);

    C.resize(logbin*num);
    W.resize(logbin*num);

    for(int i=(g-mini)*logbin; i<(g-mini+1)*logbin; i++){
    dat3 >> eins >>zwei;
    istringstream ist3(eins);
    ist3 >> W(i);
    istringstream ist4(zwei);
    ist4 >> C(i);

    }
    }
    }

    /*ofstream file4("observation.txt");

    for(int i=0; i<W.size(); i++){
    file4<<setprecision(14)<<W(i)<<" "<<C(i)<<endl;
    }*/

    dv=(pow(10,(W(5)-W(4)))-1)*c0;
    difference=W(5)-W(4);
    ldiff=pow(10,W(5))-pow(10,W(4));
    ui->doubleSpinBox_3->setValue(dv);

    elements=0;
    int a=0;

    double RV1b[num], RV3b[num];
    int RV1c[num], RV3c[num], RV1cmin, RV1cmax, RV2cmin, RV2cmax;

    for(int j=0; j<num; j++){
        RV1a=RV1[j]/dv;
        RV3a=RV3[j]/dv;
        RV1b[j]=abs(RV1[j])/dv-abs(RV1a);
        RV3b[j]=abs(RV3[j])/dv-abs(RV3a);
        //file1<<RV1[j]<<"\t"<<RV3[j]<<"\t"<<RV1b[j]<<"\t"<<RV3b[j]<<"\t";

        if(RV1b[j]>=0.5){
            if(RV1[j]>=0){
                RV1c[j]=RV1[j]/dv+1;
            }
            if(RV1[j]<0){
                RV1c[j]=RV1[j]/dv-1;
            }
        }
        else{
            RV1c[j]=RV1[j]/dv;
        }

        if(RV3b[j]>=0.5){
            if(RV3[j]>=0){
                RV3c[j]=RV3[j]/dv+1;
            }
            if(RV3[j]<0){
                RV3c[j]=RV3[j]/dv-1;
            }
        }
        else{
            RV3c[j]=RV3[j]/dv;
        }
        if(j==0){
            RV1cmin=RV1c[j];
            RV1cmax=RV1c[j];
            RV2cmin=RV3c[j];
            RV2cmax=RV3c[j];
            RV1m=abs(RV1c[j]);
            RV3m=abs(RV3c[j]);
        }
        else{
            if(RV1c[j]<RV1cmin){
                RV1cmin=RV1c[j];
            }
            if(RV1c[j]>RV1cmax){
                RV1cmax=RV1c[j];
            }
            if(RV3c[j]<RV2cmin){
                RV2cmin=RV3c[j];
            }
            if(RV3c[j]>RV2cmax){
                RV2cmax=RV3c[j];
            }
            if(abs(RV1c[j])>RV1m){
                RV1m = abs(RV1c[j]);
            }
            if(abs(RV3c[j])>RV3m){
                RV3m = abs(RV3c[j]);
            }
        }
    }

    RV1a=RV1c[0];
    RV3a=RV3c[0];
    file1<<endl;
    file1<<dv<<" "<<RV1m<<" "<<RV3m<<" "<<RV1a<<" "<<RV3a<<" "<<logbin<<endl;
    file1<<"M is a "<<num*logbin<<" x "<<bso1+bso2<<" Matrix."<<endl;
    file1<<endl;

    if(RV1cmin<0){
        if(RV1cmax<0){
        bso1=logbin+abs(RV1cmin);
        }
        else{
            bso1=logbin+abs(RV1cmin)+RV1cmax;
        }
    }
    else{
        bso1=logbin+RV1cmax;
    }
    if(RV2cmin<0){
        if(RV2cmax<0){
        bso2=logbin+abs(RV2cmin);
        }
        else{
            bso2=logbin+abs(RV2cmin)+RV2cmax;
        }
    }
    else{
        bso2=logbin+RV2cmax;
    }

    if(ui->checkBox_3->isChecked()){
        bso2=0;
    }

    cout<<"bso1 "<<bso1<<endl;
    cout<<"bso2 "<<bso2<<endl;

    Mn=num*logbin;
    if(ui->checkBox_8->isChecked()){
    Mm=bso1+bso2+logbin;    //telluric +logbin
    }
    else{
        Mm=bso1+bso2;
    }
    M.resize(Mn,Mm);
    Mt.resize(Mm,Mn);
    X.resize(Mm);
    cout<<Mm<<" "<<Mn<<endl;

    Mval1.resize(num);
    Mval2.resize(num);
    Mtel.resize(num);

    string sstrength = "edit.dat";
    std::ostringstream ediNameStream(sstrength);
    ediNameStream<<path<<"/"<<sstrength;
    std::string ediName = ediNameStream.str();

    QFileInfo lstrength(ediName.c_str());

    if(lstrength.exists()){

        ifstream edit(ediName.c_str());

        string lines, einse;

        int num_lines1=0;

        while(std::getline(edit, lines))
                   ++ num_lines1;

                edit.clear();
                edit.seekg(0, ios::beg);

                if(num_lines1!=3*num){
                    QMessageBox::information(this, "Error", "Edit file does not match to specified number of observations.");
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                    return;
                }

        for(int i=0; i<num_lines1; i++){
            edit >> einse;
            istringstream istr7(einse);

            if(i<num){
                istr7>>Mtel[i];
            }
            if((i>=num) & (i<2*num)){
                istr7>>Mval1[i-num];
            }
            if((i>=2*num) & (i<3*num)){
                istr7>>Mval2[i-2*num];
            }
        }
    }

    else {
        for(int i=0; i<num; i++){
            Mtel[i]=1.0;
            Mval1[i]=1.0/(1+ui->doubleSpinBox_4->value());
            Mval2[i]=ui->doubleSpinBox_4->value()/(1+ui->doubleSpinBox_4->value());
        }
         cout<<"Elements A:\t"<<Mval1[0]<<endl;
         cout<<"Elements B:\t"<<Mval2[0]<<endl;
         cout<<"Elements staic:\t"<<Mtel[0]<<endl;
    }


    // matrix for SB2 system
    if(ui->checkBox_4->isChecked()){

        //constructing initial transformation matrix with telluric lines
        if(ui->checkBox_8->isChecked()){

            for (int n=0; n<num*logbin; n++){//num*logbin

                if(n==0){
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                        RV1a=abs(RV1cmin)+RV1c[a];
                    }

                    if(RV2cmin>0){
                        RV3a=RV3c[a];
                    }
                    else{
                        RV3a=abs(RV2cmin)+RV3c[a];
                    }
                }

            if(n==logbin+a*logbin){
                a+=1;
                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                    RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }

            for (int m=0; m<Mm; m++){
                if((n-a*logbin+RV1a)==m or (n+bso1-1-a*logbin+RV3a==m) or (m==n-a*logbin+bso1+bso2)){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                    if(bidi==1){
                        if(m==n-a*logbin+bso1+bso2){
                            M(n,m)=Mtel[a];
                        }
                        if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        if(m<Mm-1) {
                            if(m==n-a*logbin+bso1+bso2){
                                M(n,m+1)=Mtel[a];
                            }
                            if(n+bso1-1-a*logbin+RV3a==m){
                                M(n,m+1)=Mval2[a];
                            }
                            else M(n,m+1)=Mval1[a];
                            m=m+1;
                            ++elements;
                        }
                        ++elements;
                    }
                    if(bidi==0){
                        if(m==n-a*logbin+bso1+bso2){
                            M(n,m)=Mtel[a];
                        }
                        if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        ++elements;
                    }
                }
                else{
                    M(n,m)=0;
                }
            }
        }
    }

    //without telluric lines
    else{
        for (int n=0; n<num*logbin; n++){//num*logbin

            if(n==0){
                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                        RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                    RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }
            if(n==logbin+a*logbin){
                a+=1;

                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                        RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                    RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }

            for (int m=0; m<Mm; m++){
                if((n-a*logbin+RV1a)==m or (n+bso1-1-a*logbin+RV3a==m)){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                    if(bidi==1){
                        if((n+bso1-1-a*logbin+RV3a==m) & (m>=bso1)){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        if(m<Mm-1) {
                            if((n+bso1-1-a*logbin+RV3a==m) & (m>=bso1)){
                                M(n,m+1)=Mval2[a];
                            }
                            else M(n,m+1)=Mval1[a];
                            m=m+1;
                            ++elements;
                        }
                        ++elements;
                    }
                    if(bidi==0){
                        if(n-a*logbin+RV1a==m){

                            M(n,m)=Mval1[a];
                        }
                        else M(n,m)=Mval2[a];
                        ++elements;
                    }
                }
                else{
                    M(n,m)=0;
                }
            }
        }
    }
}

    //file1<<M<<endl;

    //matrix for SB1 system
    if(ui->checkBox_3->isChecked()){

        //constructing initial transformation matrix with tellric lines
        if(ui->checkBox_8->isChecked()){

            for (int n=0; n<num*logbin; n++){//num*logbin

                if(n==0){
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }

                if(n==logbin+a*logbin){
                    a+=1;
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }
                for (int m=0; m<Mm; m++){
                    if((n-a*logbin+RV1a)==m or (m==n-a*logbin+bso1)){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                        if(bidi==1){
                            if(m==n-a*logbin+bso1){
                                M(n,m)=Mtel[a];
                            }
                            else M(n,m)=Mval1[a];
                            if(m<Mm-1) {
                                if(m==n-a*logbin+bso1){
                                    M(n,m+1)=Mtel[a];
                                }

                                else M(n,m+1)=Mval1[a];
                                m=m+1;
                                ++elements;
                            }
                            ++elements;
                        }
                        if(bidi==0){
                            if(m==n-a*logbin+bso1){
                                M(n,m)=Mtel[a];
                            }
                            else M(n,m)=Mval1[a];
                            ++elements;
                        }
                    }
                    else{
                        M(n,m)=0;
                    }
                }
            }
        }

        //without telluric lines
        else{
            for (int n=0; n<num*logbin; n++){//num*logbin

                if(n==0){
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }

                if(n==logbin+a*logbin){
                    a+=1;
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }
                    for (int m=0; m<Mm; m++){
                        if((n-a*logbin+RV1a)==m){
                            if(bidi==1){
                                M(n,m)=Mval1[a];
                                if(m<Mm-1) {
                                    M(n,m+1)=Mval1[a];
                                    m=m+1;
                                    ++elements;
                                }
                                ++elements;
                            }
                            if(bidi==0){
                                M(n,m)=Mval1[a];
                                ++elements;
                            }
                        }
                        else{
                            M(n,m)=0;
                        }
                    }
                }
            }
            //file1<<M<<endl;
        }

    //matrix for SB3 systems without telluric

    if(ui->checkBox_34->isChecked()){

        // with tellurics
        if(ui->checkBox_8->isChecked()){

        }

        // without tellurics
        else{

        }

    }


    a=0;

    loc.resize(2,elements);
    val.resize(elements);
    int elcount=0;

    for(int i=0; i<elements; i++){
        val(i)=Mval1[a];
    }

    for(int n=0; n<Mn;n++){
        for(int m=0; m<Mm;m++){
            if(M(n,m)>0){
            loc(0,elcount)=n;
            loc(1,elcount)=m;
            ++elcount;
            }
    }}


    qApp->processEvents(QEventLoop::AllEvents);

    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_16->setChecked(false);
}

void MainWindow::on_checkBox_2_clicked()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_16->setChecked(false);
}


//**********************************************************
//read data button
//**********************************************************
void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::read_data();
}

//**********************************************************
//SVD
//**********************************************************
void MainWindow::on_pushButton_3_clicked()
{

    this->setCursor(QCursor(Qt::WaitCursor));
    if(ui->spinBox_2->value()-ui->spinBox->value()<1){
        QMessageBox::information(this, "Error", "Error 7: Please read RV data first. At least two observations at (hopefully) different phases are necessary.");
        return;
    }

    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        return;
    }

    residu=0;

    QString output=ui->lineEdit_5->text();
    string out = output.toUtf8().constData();
    std::ostringstream file1NameStream(out);
    file1NameStream<<path<<"/"<<out;
    std::string file1Name = file1NameStream.str();
    ofstream file1(file1Name.c_str());

    QString output2=ui->lineEdit_8->text();
    string out2 = output2.toUtf8().constData();
    std::ostringstream file2NameStream(out2);
    file2NameStream<<path<<"/"<<out2;
    std::string file2Name = file2NameStream.str();
    ofstream file2(file2Name.c_str());

    QString tel=ui->lineEdit_10->text();
    string telout = tel.toUtf8().constData();
    std::ostringstream telNameStream(telout);
    telNameStream<<path<<"/"<<telout;
    std::string telName = telNameStream.str();
    ofstream tell(telName.c_str());

    QString output3=ui->lineEdit_13->text();
    string out3 = output3.toUtf8().constData();
    std::ostringstream file3NameStream(out3);
    file3NameStream<<path<<"/"<<out3;
    std::string file3Name = file3NameStream.str();
    ofstream file5(file3Name.c_str());

    QString QError=ui->lineEdit_20->text();
    string errout = QError.toUtf8().constData();
    std::ostringstream file6NameStream(errout);
    file6NameStream<<path<<"/"<<errout;
    std::string file6Name = file6NameStream.str();
    ofstream file8(file6Name.c_str());

    system_clock::time_point time1 = system_clock::now();

    mat U, V;
    vec w;

    if(Mm>Mn){
        Mm=Mn;
    }

    vec res(Mm);
    svd(U,w,V,M);

    int i,j,jj;
        double s;
        double thresh =-1;
        double eps = numeric_limits<double>::epsilon();
        vec tmp(Mm);
        double tsh = (thresh >= 0. ? thresh : 0.5*sqrt(Mm+Mn+1.)*w(0)*eps);
        for (j=0;j<Mm;j++) {
            s=0.0;
            if (w(j) > tsh) {
                for (i=0;i<Mn;i++) s += U(i,j)*C(i);
                s /= w(j);
            }
            tmp(j)=s;
        }

        int index1=0;
        int index2=0;
        int index3=0;

        for (j=0;j<Mm;j++) {
            s=0.0;
            for (jj=0;jj<Mm;jj++) s += V(j,jj)*tmp(jj);
                X(j)=s;
                if(X(j)>10*tsh){

                    if(j<bso1){
                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(fluxratio/(fluxratio+1)-0.5);
                        //}
                        file1<<pow(10,(W(0)+(index1+RV1min/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index1;
                    }
                    if((j>=bso1) & (j<bso1+bso2)){
                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(1.0/(fluxratio+1)-0.5);
                        //}
                        file2<<pow(10,(W(0)+(index2+RV3min/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index2;
                    }
                    if(j>=bso1+bso2){
                        tell<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
            }
            index1=0;
            index2=0;
            index3=0;

        if(ui->checkBox_7->isChecked()){
            index1=0;
            index2=0;
            std::ostringstream file1NameStream("rcomp1.txt");
            file1NameStream<<path<<"/rcomp1.txt";
            std::string file1Name = file1NameStream.str();
            ofstream file3(file1Name.c_str());

            std::ostringstream file2NameStream("rcomp2.txt");
            file2NameStream<<path<<"/rcomp2.txt";
            std::string file2Name = file2NameStream.str();
            ofstream file4(file2Name.c_str());

            for(int i=0; i<Mm-1; i++){
            X(i)=(X(i)+X(i+1))/2;
            if(i<bso1){

            file3<<pow(10,(W(0)+(2*index1-RV1maxi/dv)*difference))<<"\t"<<X(i)<<endl;
            ++index1;
            }
            if((i>=bso1) & (i<bso1+bso2)){

                file4<<pow(10,(W(0)+(index2*2+-RV3maxi/dv)*difference))<<"\t"<<X(i)<<endl;
                ++index2;
            }
            if(i>=bso1+bso2){
                tell<<pow(10,(W(0)+2*index3*difference))<<"\t"<<X(i)<<endl;
                ++index3;
            }
            i=i+2;
            }
        }
        index1=0;
        index2=0;
        index3=0;

        res=M*X;

        for(int i=0; i<Mn; i++){

            if(res(i)>10*tsh){
                file5<<pow(10,W(i))<<" "<<res(i)-C(i)<<endl;
                residu+=pow((res(i)-C(i)),2);
            }
        }

        residu=sqrt(residu);

        QVector<double> errmean(logbin);

        for(int i =0; i<logbin;i++){
            for(int n = 0; n<num; n++){
                errmean[i]+=(res(i+n*logbin)-C(i+n*logbin))/num;

            }
            file8<<pow(10,W(i))<<"\t"<<errmean[i]<<endl;
            errmean[i]=0;
        }


        cout<<"residuum of SVD: "<<residu<<endl;

        system_clock::time_point time2 = system_clock::now();

        std::time_t tt1, tt2;

          tt1 = system_clock::to_time_t ( time1 );

          tt2 = system_clock::to_time_t ( time2 );

        double CPUtime = std::difftime(tt2, tt1);
        QString CPtime = QString::number(CPUtime/60);
        ui->lineEdit_6->setText(CPtime);

        QString residual=QString::number(residu);
        ui->lineEdit_7->setText(residual);

         this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_actionSpectrum_Plotter_triggered()
{
    pSpec =new PlotSpec(this);
    pSpec->seData(ui->lineEdit_4->text(), ui->lineEdit_5->text(), ui->lineEdit_8->text(), ui->lineEdit_20->text());
    pSpec->on_pushButton_3_clicked();
    pSpec->on_pushButton_2_clicked();
    pSpec->show();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "This open-source software was developed at Leibniz-Institute for Astrophysics Potsdam (Germany) by\n\n "
                                            "D.P. Sablowski\n\n"
                                            "It makes use of the 'Armadillo' C++ linear algebra library, CCfits and libraries therein. It is provided AS IS WITHOUT WARRANTY of ANY KIND.\n\n"
                                            "Licensed under the Apache 2.0 licence conditions");
}

void MainWindow::on_actionBug_Report_triggered()
{
    QMessageBox::information(this, "Bug Rebort", "Please contact: dsablowski@aip.de");
}

//**********************************************************
//ECON
//**********************************************************
void MainWindow::on_pushButton_5_clicked()
{

    if(ui->spinBox_2->value()-ui->spinBox->value()<1){
        QMessageBox::information(this, "Error", "Error 8: Please read RV data first. At least two observations at (hopefully) different phases are necessary.");
        return;
    }

    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        return;
    }

    residu=0;
    this->setCursor(QCursor(Qt::WaitCursor));

    QString output=ui->lineEdit_5->text();
    string out = output.toUtf8().constData();
    std::ostringstream file1NameStream(out);
    file1NameStream<<path<<"/"<<out;
    std::string file1Name = file1NameStream.str();
    ofstream file1(file1Name.c_str());

    QString output2=ui->lineEdit_8->text();
    string out2 = output2.toUtf8().constData();
    std::ostringstream file2NameStream(out2);
    file2NameStream<<path<<"/"<<out2;
    std::string file2Name = file2NameStream.str();
    ofstream file2(file2Name.c_str());

    QString tel=ui->lineEdit_10->text();
    string telout = tel.toUtf8().constData();
    std::ostringstream telNameStream(telout);
    telNameStream<<path<<"/"<<telout;
    std::string telName = telNameStream.str();
    ofstream tell(telName.c_str());

    QString output3=ui->lineEdit_13->text();
    string out3 = output3.toUtf8().constData();
    std::ostringstream file3NameStream(out3);
    file3NameStream<<path<<"/"<<out3;
    std::string file3Name = file3NameStream.str();
    ofstream file5(file3Name.c_str());

    std::ostringstream file5NameStream("compl.txt");
    file5NameStream<<path<<"/compl.txt";
    std::string file5Name = file5NameStream.str();
    ofstream file7(file5Name.c_str());

    QString QError=ui->lineEdit_20->text();
    string errout = QError.toUtf8().constData();
    std::ostringstream file6NameStream(errout);
    file6NameStream<<path<<"/"<<errout;
    std::string file6Name = file6NameStream.str();
    ofstream file8(file6Name.c_str());

    string CA = "CompmA.dat";
    std::ostringstream file7NameStream(CA);
    file7NameStream<<path<<"/"<<CA;
    std::string file7Name = file7NameStream.str();
    ofstream file9(file7Name.c_str());

    string CB = "CompmB.dat";
    std::ostringstream file8NameStream(CB);
    file8NameStream<<path<<"/"<<CB;
    std::string file8Name = file8NameStream.str();
    ofstream file10(file8Name.c_str());

    system_clock::time_point time1 = system_clock::now();

    mat U, V;
    vec w;

    if(Mm>Mn){
        Mm=Mn;
    }

    vec res(Mm);

    int cBS;

    if(ui->checkBox_5->isChecked()){
        svd_econ(U,w,V,M, "both", "std");
    }
    else ++cBS;

    if(ui->checkBox_6->isChecked()){
        svd_econ(U,w,V,M, "both", "dc");
    }
    else ++cBS;

    if(cBS == 0){
        QMessageBox::information(this, "Error", "Select method for ECON.");
        return;
    }


    int i,j,jj, svalues=0;
        double s;
        double thresh =-1;
        double eps = numeric_limits<double>::epsilon();
        vec tmp(Mm);
        double tsh = (thresh >= 0. ? thresh : 0.5*sqrt(Mm+Mn+1.)*w(0)*eps);
        cout <<"tsh "<<tsh<<endl;
        for (j=0;j<Mm;j++) {
            s=0.0;
            if (w(j) > tsh) {
                for (i=0;i<Mn;i++) s += U(i,j)*C(i);
                s /= w(j);

            }
            tmp(j)=s;
        }

        int index1=0;
        int index2=0;
        int index3=0;
        for (j=0;j<Mm;j++) {
            s=0.0;
            if(w(j)>tsh){
                for (jj=0;jj<Mm;jj++) s += V(j,jj)*tmp(jj);
                    X(j)=s;
                ++svalues;
                }
            }

        cout<<"# singular values: "<<svalues<<endl;

        res=M*X;

        for(int i=0; i<Mn; i++){

            if(res(i)>10*tsh){
                file5<<setprecision(14)<<pow(10,W(i))<<" "<<res(i)-C(i)<<endl;
                if(ui->checkBox_33->isChecked()){//(abs(C(i)-res(i))<C(i)/3) &
                    if((W(i)>log10(ui->doubleSpinBox_14->value())) & (W(i)<log10(ui->doubleSpinBox_15->value()))){
                        residu+=pow((res(i)-C(i)),2);
                    }
                }

                else{
                        residu+=pow((res(i)-C(i)),2);
                }

            }
        }

        residu=sqrt(residu);

        QVector<double> errmean(logbin);

        for(int i =0; i<logbin;i++){
            for(int n = 0; n<num; n++){
                errmean[i]+=(res(i+n*logbin)-C(i+n*logbin))/num;
            }
            file8<<setprecision(14)<<pow(10,W(i))<<"\t"<<errmean[i]<<endl;
            errmean[i]=0;
        }

            for(j=0; j<Mm;j++){
                if(abs(X(j))>10*tsh){

                if(j<bso1){

                    file9<<setprecision(14)<<W(j)<<"\t"<<X(j)<<endl;

                    //if(ui->checkBox_4->isChecked()){
                    //    X(j)=X(j)+(fluxratio/(fluxratio+1)-0.5);
                    //}

                    file1<<setprecision(14)<<pow(10,(W(0)+(index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<(X(j))<<endl;
                    ++index1;
                }
                if((j>=bso1)&(j<bso1+bso2)){

                    file10<<setprecision(14)<<W(j)<<"\t"<<X(j)<<endl;

                    //if(ui->checkBox_4->isChecked()){
                    //    X(j)=X(j)+(1.0/(fluxratio+1)-0.5);
                    //}
                    file2<<setprecision(14)<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<(X(j))<<endl;
                    ++index2;
                }
                if(j>=bso1+bso2){

                    tell<<setprecision(14)<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                    ++index3;
                }
            }
         }

        for(int n=0; n<X.size(); n++){
            file7<<n<<" "<<X(n)<<endl;
        }
        index1=0;
        index2=0;
        index3=0;


        if(ui->checkBox_7->isChecked()){
            index1=0;
            index2=0;


            std::ostringstream file1NameStream("rcomp1.txt");
            file1NameStream<<path<<"/rcomp1.txt";
            std::string file1Name = file1NameStream.str();
            ofstream file3(file1Name.c_str());

            std::ostringstream file2NameStream("rcomp2.txt");
            file2NameStream<<path<<"/rcomp2.txt";
            std::string file2Name = file2NameStream.str();
            ofstream file4(file2Name.c_str());

            for(int i=0; i<Mm-1; i++){
                X(i)=(X(i)+X(i+1))/2;

                if(i<bso1){
                    //X(i)=X(i)-0.5*(1/fluxratio+1)+1/(fluxratio+1);
                    file3<<setprecision(14)<<pow(10,(W(0)+(2*index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<X(i)<<endl;
                    ++index1;
                }
                if((i>=bso1) & (i<bso1+bso2)){
                    //X(i)=X(i)-0.5*(1/fluxratio+1)+fluxratio/(fluxratio+1);
                    file4<<setprecision(14)<<pow(10,(W(0)+(2*index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<X(i)<<endl;
                    ++index2;
                }
                ++i;
            }
        }


        cout<<"residuum of SVD: "<<residu<<endl;

        system_clock::time_point time2 = system_clock::now();

        std::time_t tt1, tt2;

          tt1 = system_clock::to_time_t ( time1 );

          tt2 = system_clock::to_time_t ( time2 );

        double CPUtime = std::difftime(tt2, tt1);
        QString CPtime = QString::number(CPUtime/60);
        ui->lineEdit_6->setText(CPtime);
        QString residual=QString::number(residu);
        ui->lineEdit_7->setText(residual);

        MainWindow::on_pushButton_6_clicked();

         this->setCursor(QCursor(Qt::ArrowCursor));

}

void MainWindow::on_checkBox_5_clicked()
{
    ui->checkBox_6->setChecked(false);
}

void MainWindow::on_checkBox_6_clicked()
{
     ui->checkBox_5->setChecked(false);
}

void MainWindow::on_checkBox_7_clicked()
{
   if(ui->checkBox_7->isChecked()){
       bidi=1;
   }
   else{
       bidi=0;
   }
}

void MainWindow::on_lineEdit_4_editingFinished()
{
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();
}

void MainWindow::on_actionCorrelation_triggered()
{
    qCorr =new correlation(this);
    qCorr->seData(ui->lineEdit_4->text(), ui->lineEdit_5->text());
    qCorr->show();
}

void MainWindow::on_lineEdit_9_textChanged()
{
    qExtension=ui->lineEdit_9->text();
    Extension=qExtension.toUtf8().constData();
}

void MainWindow::on_lineEdit_11_textChanged()
{
    qWCol=ui->lineEdit_11->text();
    WCol=qWCol.toUtf8().constData();
}

void MainWindow::on_lineEdit_12_textChanged()
{
    qICol=ui->lineEdit_12->text();
    ICol=qICol.toUtf8().constData();
}

void MainWindow::on_comboBox_currentIndexChanged()
{
    if(ui->comboBox->currentIndex()==0){
        ui->lineEdit_9->setEnabled(true);
        ui->lineEdit_11->setEnabled(true);
        ui->lineEdit_12->setEnabled(true);
        ui->lineEdit_19->setText(".fits");
    }
    if(ui->comboBox->currentIndex()==1){
        ui->lineEdit_9->setEnabled(false);
        ui->lineEdit_11->setEnabled(false);
        ui->lineEdit_12->setEnabled(false);
        ui->lineEdit_19->setText(".txt");
    }
}

//******************************************************
//optimization DSM
//******************************************************
void MainWindow::Optimisation()
{
    system_clock::time_point time1 = system_clock::now();

    upda=0;
    updac=0;

    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);

    this->setCursor(QCursor(Qt::WaitCursor));
    MainWindow::read_data();
    this->setCursor(QCursor(Qt::WaitCursor));

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        return;
    }

    qApp->processEvents(QEventLoop::AllEvents);

    int Ph=0, Pl=0, Psh=0, SB1=0;

    eval=0;

    if(ui->checkBox_3->isChecked()){
            SB1=1;
    }

    zaehler=ui->spinBox_3->value();
    double yh, ysh, yl, ym, yi, ys, yt;

    int nu=2*num;
    if(SB1==1){
        nu=num;
    }

    QString qLogFile = ui->lineEdit_21->text();
    QString qlFile = qPath+"/"+qLogFile;
    QFileInfo QLFile(qlFile);
    string lFile = qLogFile.toUtf8().constData();
    std::ostringstream lFileNameStream(lFile);
    lFileNameStream<<path<<"/"<<lFile;
    std::string lFileName = lFileNameStream.str();
    ofstream LogFile;

    struct tm *ts;
    time_t t;

    t = time(NULL);
    ts = localtime(&t);

    if(QLFile.exists()){
        LogFile.open(lFileName.c_str(), std::ios_base::app);
        LogFile<<endl;
        LogFile<<"__________________________________________________"<<endl;
        LogFile<<" "<<asctime(ts)<<endl;
        LogFile<<endl;
    }

    else{
        LogFile.open(lFileName.c_str());
        LogFile<<"******************************"<<endl;
        LogFile<<"  Optimisation LogFile"<<endl;
        LogFile<<"******************************"<<endl;
    }

    string sstrength = "edit.dat";
    std::ostringstream ediNameStream(sstrength);
    ediNameStream<<path<<"/"<<sstrength;
    std::string ediName = ediNameStream.str();

        QFileInfo lstrength(ediName.c_str());

    if(lstrength.exists()){

        LogFile<<"Relative component strength and telluric line strength set via Editor."<<endl;

        ifstream edit(ediName.c_str());
        string lines, einse;

        int num_lines1=0;

        while(std::getline(edit, lines))
                   ++ num_lines1;

                edit.clear();
                edit.seekg(0, ios::beg);

                if(num_lines1!=3*num){
                    QMessageBox::information(this, "Error", "Edit file does not match to specified number of observations.");
                    error=1;
                    return;
                }

        for(int i=0; i<num_lines1; i++){
            edit >> einse;
            istringstream istr7(einse);

            if(i<num){
                istr7>>Mtel[i];
            }
            if((i>=num) & (i<2*num)){
                istr7>>Mval1[i-num];
            }
            if(i>=2*num){
                istr7>>Mval2[i-2*num];
            }
        }
    }

    else {

        LogFile<<"No relative values set via Editor."<<endl;

        for(int i=0; i<num; i++){
            Mtel[i]=1.0;
            Mval1[i]=1/(1+ui->doubleSpinBox_4->value());
            Mval2[i]=ui->doubleSpinBox_4->value()/(1+ui->doubleSpinBox_4->value());
        }
    }

    LogFile<<endl;

    string scoef = "optcoef.dat";
    std::ostringstream coeNameStream(scoef);
    coeNameStream<<path<<"/"<<scoef;
    std::string coeName = coeNameStream.str();

    QFileInfo optcoef(coeName.c_str());

    if(optcoef.exists()){

        LogFile<<"User defined transformation coefficients used."<<endl;

        double optv[4];
        ifstream optc(coeName.c_str());
        string einse;

        for(int i=0; i<4; i++){
            optc >> einse;
            istringstream istr8(einse);
            istr8 >> optv[i];
        }
        alpha=optv[0];
        beta=optv[1];
        btot=optv[2];
        Gamma=optv[3];

    }

    else{
        LogFile<<"Default values for transformation coefficients used."<<endl;

        step=1.0;
        Gamma=2.0;
        alpha=1.0;
        beta=0.5;
        btot=0.5;
    }

    LogFile<<"Alpha: "<<alpha<<"; Beta: "<<beta<<"; Beta_tot: "<<btot<<"; Gamma: "<<Gamma<<"; Step: "<<step<<endl;


    LogFile<<endl;

    string sinit = "initialstep.dat";
    std::ostringstream iniNameStream(sinit);
    iniNameStream<<path<<"/"<<sinit;
    std::string iniName = iniNameStream.str();

    QFileInfo initstep(iniName.c_str());

    if(initstep.exists()){

        LogFile<<"User defined initial step sizes used."<<endl;

        ifstream initStep(iniName.c_str());
        double IntS[8];
        string einse;

        for(int i =0; i<8; i++){
            initStep >> einse;
            istringstream istr9(einse);
            istr9 >> IntS[i];
        }
        step = IntS[0];
        dP = IntS[1];
        de = IntS[2];
        dKA = IntS[3];
        dKB = IntS[4];
        dGamma = IntS[5];
        dT0 = IntS[6];
        dOmega = IntS[7];

    }

    else{

        LogFile<<"Default initial step size used."<<endl;

        step=3.0;
        dP = orbele[0]/10;
        de = orbele[1]/10;
        dKA = 3*dv;
        dKB = 3*dv;
        dGamma = 3*dv;
        dT0 = orbele[0]/10;
        dOmega = M_PI/20;
    }

    double ym2=0, ys2=0;
    int stagnate=0;

    LogFile<<"Step: "<<step<<"; dP: "<<dP<<"; de: "<<de<<"; dKA: "<<dKA<<"; dKB: "<<dKB<<"; dGamma: "<<dGamma<<"; dT0: "<<dT0<<"; dOmega: "<<dOmega<<endl;
    LogFile<<endl;

    if(ui->checkBox->isChecked() or ui->checkBox_2->isChecked()){

        LogFile<<"Optimisation on individual RVs."<<endl;

        LogFile<<endl;

        double y[nu+1], Z[nu], Co[nu], So[nu], Eo[nu];
        mat P(nu+1,nu), e(nu+1,nu);

    //create new initial data
    if(ui->checkBox_9->isChecked()){
        r1=100.0;

        LogFile<<"Creating new initial simplex with "<<nu+1<<" points."<<endl;

        qInitval = ui->lineEdit_14->text();
        string file1init = qInitval.toUtf8().constData();
        std::ostringstream file1NameStream(file1init);
        file1NameStream<<path<<"/"<<file1init;
        std::string file1Name = file1NameStream.str();

        qInitmat = ui->lineEdit_15->text();
        string file2init = qInitmat.toUtf8().constData();
        std::ostringstream file2NameStream(file2init);
        file2NameStream<<path<<"/"<<file2init;
        std::string file2Name = file2NameStream.str();

        if(ui->checkBox_9->isChecked()){
            QFile QIn(file1Name.c_str());
            QFile QIn2(file2Name.c_str());

            if(inrein==0 & (QIn.exists() or QIn2.exists())){
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Warning!", "The file already exists. \n\n Do you want to overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                qDebug() << "Overwrite initial data.";
                }
                else{
                    qDebug()<< "Initiation aborted.";
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    return;
                }
            }
            else{
                cout<<"Create new initial data."<<endl;
            }

        }
        else{
            cout<<"Create new initial data."<<endl;
            inrein=0;
        }
        
        ofstream init(file1Name.c_str());

        ofstream init2(file2Name.c_str());


    for(int i=0; i<nu; i++){
        if(SB1==0){
        if(i<nu/2){
        P(0,i)=RV1[i];
        }
        else{
        P(0,i)=RV3[i-nu/2];
        }
        }
        if(SB1==1){
            P(0,i)=RV1[i];
        }
    }

    for(int i=0; i<nu+1; i++){
        for(int j=0; j<nu; j++){

            if((i>0)&(i==j+1)){
                e(i,j)=2*step;
            }
            else{
                e(i,j)=0;
            }
        }
    }

    for (int i=0; i<nu+1; i++){
        for (int j=0; j<nu; j++){

            P(i,j)=P(0,j)+dv*e(i,j);
            init2<<setprecision(14)<<P(i,j)<<endl;
            //cout<<P[i][j]<<" ";
            if(SB1==0){
                if(j<nu/2){
                    RV1[j]=P(i,j);
                }
                else{
                    RV3[j-nu/2]=P(i,j);
                }
            }
            if(SB1==1){
                RV1[j]=P(i,j);
            }
        }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[i]=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);
        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<yi<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<RV1[i]<<"\t"<<RV3[i]<<endl;
                }
            }
            LogFile<<endl;
        }

        if(abortt==1){
            LogFile<<"Initiation was aborted."<<endl;
            abortt=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }
        if(y[i]<r1){
            r1=y[i];
            if(ui->checkBox_10->isChecked()){
                MainWindow::on_pushButton_6_clicked();
            }
        }

        cout<<y[i]<<endl;
        init<<setprecision(12)<<y[i]<<endl;
        LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;
    }
    init.close();
    init2.close();

    LogFile<<"Initial simplex completed."<<endl;

    }

    //continue with optimization data
    if(ui->checkBox_11->isChecked()){

        string zeile, one;

        LogFile<<"Continue with optimisation; loading data."<<endl;


        qOptval = ui->lineEdit_16->text();
        string file1opt = qOptval.toUtf8().constData();
        std::ostringstream file1NameStream(file1opt);
        file1NameStream<<path<<"/"<<file1opt;
        std::string file1Name = file1NameStream.str();
        ifstream initiate1(file1Name.c_str());

        qOptmat = ui->lineEdit_17->text();
        string file2opt = qOptmat.toUtf8().constData();
        std::ostringstream file2NameStream(file2opt);
        file2NameStream<<path<<"/"<<file2opt;
        std::string file2Name = file2NameStream.str();
        ifstream initiate2(file2Name.c_str());

        QFile checkfile(file2Name.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The optimisation file "<<checkfile.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            return;
        }

        int number_of_lines =0;

        while(std::getline(initiate1, zeile))
           ++ number_of_lines;

        initiate1.clear();
        initiate1.seekg(0, ios::beg);

        if(number_of_lines!=nu+1){
            qDebug()<<"Wrong optimisation file.";
            QMessageBox::information(this, "Error", "The optimisation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The optimisation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            return;
        }

        for (int i=0; i<number_of_lines; i++){
        initiate1 >> one;
        istringstream ist(one);
        ist >> y[i];
        if(i==0) r1=y[0];
        if(y[i]<r1){
            r1=y[i];
        }
        }
        initiate1.close();

        int coun=0;

        for (int i=0; i<number_of_lines*nu; i++){
        initiate2 >> one;
        istringstream ist(one);
        ist >> P(coun,i-coun*nu);
        cout<<"P["<<coun<<"]["<<i-coun*nu<<"]: "<<P(coun,i-coun*nu)<<" ";
        if(i==coun*nu+nu-1){
            cout<<endl;
            ++coun;
        }
        }
        initiate2.close();

        LogFile<<"Data from previous optimisation successfully loaded."<<endl;

    }

    //use initial data.
    if(ui->checkBox_12->isChecked()){

        LogFile<<"Use available initial data."<<endl;

        string zeile, one;

        qInitval = ui->lineEdit_14->text();
        string file1init = qInitval.toUtf8().constData();
        std::ostringstream file1NameStream(file1init);
        file1NameStream<<path<<"/"<<file1init;
        std::string file1Name = file1NameStream.str();
        ifstream initiate1(file1Name.c_str());

        qInitmat = ui->lineEdit_15->text();
        string file2init = qInitmat.toUtf8().constData();
        std::ostringstream file2NameStream(file2init);
        file2NameStream<<path<<"/"<<file2init;
        std::string file2Name = file2NameStream.str();
        ifstream initiate2(file2Name.c_str());

        QFile checkfile(file2Name.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The initiation file "<<checkfile.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "The initiation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The initiation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            return;
        }

        int number_of_lines =0;

        while(std::getline(initiate1, zeile))
           ++ number_of_lines;

        initiate1.clear();
        initiate1.seekg(0, ios::beg);

        if(number_of_lines!=nu+1){
            qDebug()<<"Wrong initiate file.";
            QMessageBox::information(this, "Error", "The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            return;
        }

        for (int i=0; i<number_of_lines; i++){
        initiate1 >> one;
        istringstream ist(one);
        ist >> y[i];
        if(i==0) r1=y[0];
        if(y[i]<r1){
            r1=y[i];
        }
        }
        initiate1.close();

        int coun=0;

        for (int i=0; i<number_of_lines*nu; i++){
        initiate2 >> one;
        istringstream ist(one);
        ist >> P(coun,i-coun*nu);
        cout<<"P["<<coun<<"]["<<i-coun*nu<<"]: "<<P(coun,i-coun*nu)<<" ";
        if(i==coun*nu+nu-1){
            cout<<endl;
            ++coun;
        }
        }
        initiate2.close();

        LogFile<<"Initial data successfully loaded."<<endl;
    }
    qOptval = ui->lineEdit_16->text();
    string file1opt = qOptval.toUtf8().constData();
    std::ostringstream file1NameStream(file1opt);
    file1NameStream<<path<<"/"<<file1opt;
    std::string file1Name = file1NameStream.str();
    ofstream opt1(file1Name.c_str());

    qOptmat = ui->lineEdit_17->text();
    string file2opt = qOptmat.toUtf8().constData();
    std::ostringstream file2NameStream(file2opt);
    file2NameStream<<path<<"/"<<file2opt;
    std::string file2Name = file2NameStream.str();
    ofstream opt2(file2Name.c_str());

    LogFile<<endl;
    LogFile<<"*************************"<<endl;
    LogFile<<"Start Optimisation"<<endl;

    if(ui->checkBox_32->isChecked()){
        ui->checkBox_9->setChecked(false);
        ui->checkBox_11->setChecked(true);
        ui->checkBox_12->setChecked(false);
    }


    //start main loop
        for (int t=0; t<zaehler; t++){
            ui->spinBox_4->setValue(t+1);

            LogFile<<endl;
            LogFile<<"Iteration: "<<t+1<<endl;
            qApp->processEvents(QEventLoop::AllEvents);

            cout<<endl;
            cout<<endl;
            cout<<"Iteration: "<<t+1<<"\t";

        //initialize next step
        ym=0;
        ys=0;
        for (int i=0; i<nu; i++){
            Z[i]=0;
        }

        //looking for highest value
        yh=y[0];
        Ph = 0;
        for (int j=0; j<nu+1; j++){
            if(y[j]>yh){
                yh = y[j];
                Ph = j;
            }
        }

        //looking for smallest value
        yl=yh;
        for (int j=0; j<nu+1; j++){
            if(y[j]<yl){
                yl=y[j];
                Pl = j;
            }
        }

        //looking for second highest value
        ysh=yl;
        for (int j=0; j<nu+1; j++){
            if((y[j]>ysh) & (y[j]<yh) & (y[j]>yl) & (j !=Pl)){
                ysh=y[j];
                Psh=j;
            }
        }

        yh=y[Ph];
        yl=y[Pl];
        ysh=y[Psh];

        //computing mean and sigma
        for (int i=0; i<nu+1; i++){
            ym+=y[i]/(nu+1);
        }
        for (int i=0; i<nu+1; i++){
            ys+=sqrt(pow((y[i]-ym),2));
        }
        ys=ys/(nu);


        if((ys==ys2) & (ym==ym2)){
            ++stagnate;

            if(ui->checkBox_32->isChecked()){
                cout<<"Reinitiate optimisation with current best RVs."<<endl;
                ui->plainTextEdit_2->appendPlainText("Reinitiate optimisation with current best RVs.");
                LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                LogFile<<endl;
                LogFile<<"Reinitiate optimisation with current best RV."<<endl;

                string ReinV = ("rein_vel.dat");
                std::ostringstream ReNameStream(ReinV);
                ReNameStream<<path<<"/"<<ReinV;
                std::string ReName = ReNameStream.str();
                ofstream ReVel(ReName.c_str());

                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        ReVel<<P(Pl,i)<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        ReVel<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                    }
                }

                ui->lineEdit_2->setText("rein_vel.dat");

                ui->checkBox->setChecked(false);
                ui->checkBox_2->setChecked(true);
                ui->checkBox_16->setChecked(false);

                ui->checkBox_9->setChecked(true);
                ui->checkBox_11->setChecked(false);
                ui->checkBox_12->setChecked(false);

                reinitiate=1;

                return;

            }
            else{
                QString stagn = QString::number(stagnate);
                LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                cout<<"Optimisation may stagnate. No change in mean and STD of simplex points since "<<stagnate<<" iterations."<<endl;
                ui->plainTextEdit_2->appendPlainText("Optimisation may stagnate. No change in mean and STD of simplex points since "+stagn+" iterations.");
            }

            }

        else {
            stagnate =0;
        }

        cout<<"Stag-Index: "<<stagnate<<endl;

        ys2=ys;
        ym2=ym;

        cout<<"Simplex Mean: "<<ym<<" Simplex STD: "<<ys<<endl;

        LogFile<<setprecision(12)<<"Mean: "<<ym<<"\tSTD: "<<ys<<endl;

        //compute centroid
        for (int j=0; j<nu; j++){
            for (int i=0; i<nu+1; i++){
                if (i!=Ph){
                    Z[j]+=P(i,j)/nu;
                }
            }
        }
        cout<<t<<" centroid ";

        //reflect highest value at centroid
        cout<<"reflection at centroid..."<<endl;
        for (int i=0; i<nu; i++){
            Co[i]=Z[i]+alpha*(Z[i]-P(Ph,i));
            if(SB1==0){
                if(i<nu/2){
                    RV1[i]=Co[i];
                }
                else{
                    RV3[i-nu/2]=Co[i];
                }
            }
            if(SB1==1){
                RV1[i]=Co[i];
            }
        }
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        yi=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);

        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<yi<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<RV1[i]<<"\t"<<RV3[i]<<endl;
                }
            }
            LogFile<<endl;
        }

        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                }
            }
            LogFile<<endl;
            for(int i=0; i<nu+1;i++){
                cout<<y[i]<<endl;
                opt1<<setprecision(12)<<y[i]<<endl;
                for(int j=0; j<nu; j++){
                    opt2<<setprecision(12)<<P(i,j)<<endl;
                }
            }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
        }

        //Alpha Branch, better than best point
        if(yi<yl){
           cout<<"expansion 1 ";
            LogFile<<"yi<yl; ";
            for (int i=0; i<nu; i++){
                Eo[i]=Z[i]+Gamma*(Z[i]-P(Ph,i));  //Expansion over worst point
                if(SB1==0){
                    if(i<nu/2){
                        RV1[i]=Eo[i];
                    }
                    else{
                        RV3[i-nu/2]=Eo[i];
                    }
                }
                if(SB1==1){
                    RV1[i]=Eo[i];
                }
            }
            eval++;
            ui->spinBox_5->setValue(eval);
            qApp->processEvents(QEventLoop::AllEvents);
            MainWindow::ConstructMatrix();
            yt=MainWindow::DivideConquer();
            qApp->processEvents(QEventLoop::AllEvents);

            if(upda==1){
                upda=0;
                LogFile<<endl;
                LogFile<<"New RVs found; Residuum: "<<yt<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<RV1[i]<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        LogFile<<RV1[i]<<"\t"<<RV3[i]<<endl;
                    }
                }
                LogFile<<endl;
            }

            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                    }
                }
                LogFile<<endl;
                for(int i=0; i<nu+1;i++){
                    cout<<y[i]<<endl;
                    opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            if(yt<yl){
                cout<<"exp ";
                for (int i=0; i<nu; i++){
                    P(Ph,i)=Eo[i];
                }
        /*
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        */
            y[Ph]=yt;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                    }
                }
                LogFile<<endl;
                for(int i=0; i<nu+1;i++){
                    cout<<y[i]<<endl;
                    opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);

        }
        if (yt>=yl){
            cout<<"ref1 ";
            LogFile<<"yt<=yl, reflection; ";
            for (int i=0; i<nu; i++){
                P(Ph,i)=Co[i];
                if(SB1==0){
                    if(i<nu/2){
                        RV1[i]=Co[i];
                    }
                    else{
                        RV3[i-nu/2]=Co[i];
                    }
                }
                if(SB1==1){
                    RV1[i]=Co[i];
                }
            }
        /*
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        */
            y[Ph]=yi;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                    }
                }
                LogFile<<endl;
                for(int i=0; i<nu+1;i++){
                    cout<<y[i]<<endl;
                    opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);
            }
        }//Alpha Branch end

        //Beta Branch, worse than best point
        if(yi>=yl){
            cout<<"yi>=yl ";
            LogFile<<"yi>=yl; ";
            //Beta_2 Branch
            if(yi<=ysh){
                cout<<"ref2 ";
                for(int i=0; i<nu; i++){
                    P(Ph,i)=Co[i];
                    if(SB1==0){
                        if(i<nu/2){
                            RV1[i]=Co[i];
                        }
                        else{
                            RV3[i-nu/2]=Co[i];
                        }
                    }
                    if(SB1==1){
                        RV1[i]=Co[i];
                    }
                }
        /*
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        */
            y[Ph]=yi;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                    }
                }
                LogFile<<endl;
                for(int i=0; i<nu+1;i++){
                    cout<<y[i]<<endl;
                    opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);
        }//Beta_2 Branch End

        //Beta_1 Branch
        if(yi>ysh){
            cout<<"yi>ysh ";
            LogFile<<"yi>ysh; ";
            if(yi<=yh){
                cout<<"yi<=yh ref3 ";
                LogFile<<"yi<=yh, reflection 3; ";
                for(int i=0; i<nu; i++){
                    P(Ph,i)=Co[i];
                    if(SB1==0){
                        if(i<nu/2){
                            RV1[i]=Co[i];
                        }
                        else{
                            RV3[i-nu/2]=Co[i];
                        }
                    }
                    if(SB1==1){
                        RV1[i]=Co[i];
                    }
                }
        /*
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        */
            y[Ph]=yi;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                if(ui->checkBox_4->isChecked()){
                    for(int i = 0; i<nu/2;i++){
                        LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                    }
                }
                LogFile<<endl;
                for(int i=0; i<nu+1;i++){
                    cout<<y[i]<<endl;
                    opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);
        //yh=y[Ph];
        }

        for(int i=0; i<nu; i++){
            So[i]=Z[i]+beta*(P(Ph,i)-Z[i]); //simple contraction
            if(SB1==0){
                if(i<nu/2){
                    RV1[i]=So[i];
                }
                else{
                    RV3[i-nu/2]=So[i];
                }
            }
            if(SB1==1){
                RV1[i]=So[i];
            }
        }
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        yt=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);

        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<yt<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<RV1[i]<<"\t"<<RV3[i]<<endl;
                }
            }
            LogFile<<endl;
        }

        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                }
            }
            LogFile<<endl;
            for(int i=0; i<nu+1;i++){
                cout<<y[i]<<endl;
                opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }

        //Beta_1_1 Branch
        if(yt>yh){
            cout<<"yt>yh tot ";
            LogFile<<"yt>yh, total contraction, ";
            for (int j=0; j<nu+1; j++){
                for (int i=0; i<nu; i++){

                    P(j,i)=P(Pl,i)+btot*(P(j,i)-P(Pl,i)); //total contraction
                    if(SB1==0){
                        if(i<nu/2){
                            RV1[i]=P(j,i);
                        }
                        else{
                            RV3[i-nu/2]=P(j,i);
                        }
                    }
                    if(SB1==1){
                        RV1[i]=P(j,i);
                    }
                }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[j]=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);
        cout<<j<<" from "<<nu+1<<" points contracted."<<endl;

        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<yt<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<RV1[i]<<"\t"<<RV3[i]<<endl;
                }
            }
            LogFile<<endl;
        }

        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                }
            }
            LogFile<<endl;
            for(int i=0; i<nu+1;i++){
                cout<<y[i]<<endl;
                opt1<<setprecision(12)<<y[i]<<endl;
                for(int j=0; j<nu; j++){
                    opt2<<setprecision(12)<<P(i,j)<<endl;
                }
            }
            abortt=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            return;
         }
        }
        }//Beta_1_1 Branch End

        //Beta_1_2 Branch
        if(yt<=yh){
            cout<<"yt<=yh, sco ";
            LogFile<<"yt<=yh, single contraction ";
            for(int i=0; i<nu; i++){
                P(Ph,i)=So[i];
                if(SB1==0){
                    if(i<nu/2){
                        RV1[i]=So[i];
                    }
                    else{
                        RV3[i-nu/2]=So[i];
                    }
                }
                if(SB1==1){
                    RV1[i]=So[i];
                }
            }
        /*
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        */
        y[Ph]=yt;//MainWindow::DivideConquer();
        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
                }
            }
            if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<P(Pl,i)<<"\t"<<P(Pl,i+nu/2)<<endl;
                }
            }
            LogFile<<endl;
            for(int i=0; i<nu+1;i++){
                cout<<y[i]<<endl;
                opt1<<setprecision(12)<<y[i]<<endl;
                    for(int j=0; j<nu; j++){
                        opt2<<setprecision(12)<<P(i,j)<<endl;
                    }
                }
                abortt=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);
           }
        }

        }
        LogFile<<endl;
        }//end main loop


        //looking for highest value
        yh=y[0];
        for (int j=0; j<nu+1; j++){
            if(y[j]>yh){
            yh = y[j];
            Ph = j;
            }
        }

        //looking for smallest value
        yl=yh;
        for (int j=0; j<nu+1; j++){
            if(y[j]<yl){
                yl=y[j];
                Pl = j;
            }
        }

        //looking for second highest value
        ysh=yl;
        for (int j=0; j<nu+1; j++){
            if((y[j]>ysh)&(y[j]<yh)){
                ysh=y[j];
                Psh=j;
            }
        }

        //output results
        for(int i=0; i<nu+1;i++){
            cout<<y[i]<<endl;
            opt1<<setprecision(12)<<y[i]<<endl;
            for(int j=0; j<nu; j++){
                opt2<<setprecision(12)<<P(i,j)<<endl;
            }
        }

        LogFile<<ym<<"          "<<ys<<endl;
        LogFile<<" End Optimisation "<<endl;
        LogFile<<" RVs after "<<zaehler<<" Iterations and "<<eval<<" Evaluations of Optimisation: "<<endl;
        if(ui->checkBox_3->isChecked()){
            for(int i = 0; i<nu;i++){
                LogFile<<P(Pl,i)<<endl;
            }
        }
        if(ui->checkBox_4->isChecked()){
            for(int i = 0; i<nu/2;i++){
                LogFile<<P(Pl,i)<<" "<<P(Pl,i+nu/2)<<endl;
            }
        }
        LogFile.close();

        cout<<"RV's after "<<zaehler<<" Iterations and "<<eval<<" Evaluations:"<<endl;
        for(int i=0; i<nu/2;i++){
            cout<<P(Pl,i)<<" "<<P(Pl,i+nu/2)<<endl;
        }
    }

      //optimization on orbit
          if(ui->checkBox_16->isChecked()){

              int Plock=0, elock=0, T0lock=0;
              nu=7;

              LogFile<<"Optimisation on orbital parameters."<<endl;

              int add=0;

              if(ui->checkBox_18->isChecked()){ // lock P
                  ++add;
                  Plock = 1;
                  LogFile<<"P locked ";
              }

              if(ui->checkBox_17->isChecked()){ // lock e
                  ++add;
                  elock = 1;
                  LogFile<<"e locked ";
              }

              if(ui->checkBox_35->isChecked()){ // lock T0
                  ++add;
                  T0lock = 1;
                  LogFile<<"T0 locked"<<endl;
              }

              nu = nu-add;
              cout<<"add: "<<add<<endl;

              LogFile<<endl;

              double y[nu+1], P[nu+1][nu], Z[nu], Co[nu], So[nu], Eo[nu], e[nu+1][nu];

              //create new initial data
              if(ui->checkBox_9->isChecked()){
                  r1=100.0;

                  qInitval = ui->lineEdit_14->text();
                  string file1init = qInitval.toUtf8().constData();
                  std::ostringstream file1NameStream(file1init);
                  file1NameStream<<path<<"/"<<file1init;
                  std::string file1Name = file1NameStream.str();

                  qInitmat = ui->lineEdit_15->text();
                  string file2init = qInitmat.toUtf8().constData();
                  std::ostringstream file2NameStream(file2init);
                  file2NameStream<<path<<"/"<<file2init;
                  std::string file2Name = file2NameStream.str();
                  
                  if(ui->checkBox_9->isChecked()){

                    QFile QIn(file1Name.c_str());
                    QFile QIn2(file2Name.c_str());

                    if(inrein==0 & (QIn.exists() or QIn2.exists())){
                        QMessageBox::StandardButton reply;
                        reply = QMessageBox::question(this, "Warning!", "The file already exists. \n\n Do you want to overwrite it?",
                                                    QMessageBox::Yes|QMessageBox::No);
                        if (reply == QMessageBox::Yes) {
                            qDebug() << "Overwrite initial data.";
                            LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                            LogFile<<"Elements:"<<endl;
                        }
                        else{
                            qDebug()<< "Initiation aborted.";
                            this->setCursor(QCursor(Qt::ArrowCursor));
                            return;
                        }
                    }
                    else{
                        cout<<"Create new initial data."<<endl;
                        LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                        LogFile<<"Elements:"<<endl;
                    }
                  }
                  else{
                      cout<<"Create new initial data."<<endl;
                      LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                      LogFile<<"Elements:"<<endl;
                      inrein=0;
                  }

                  ofstream init(file1Name.c_str());

                  ofstream init2(file2Name.c_str());

                  for(int i=0; i<nu; i++){
                      if(add==0){
                      P[0][i]=orbele[i];
                      }
                      if(add==1){
                          if(Plock==1){
                              P[0][i]=orbele[i+1];
                          }
                          if(elock==1){
                              if(i==0){
                                  P[0][i]=orbele[0];
                              }
                              if(i>0){
                                  P[0][i]=orbele[i+1];
                              }
                          }
                          if(T0lock==1){
                              if(i==5){
                                  P[0][i]=orbele[6];
                              }
                              else P[0][i]=orbele[i];
                          }
                      }
                      if(add==2){
                          if((Plock==1) & (elock==1)){
                              P[0][i]=orbele[i+2];
                          }
                          if((Plock==1) & (T0lock==1)){
                              if(i==4){
                                  P[0][i]=orbele[6];
                              }
                              else P[0][i]=orbele[i+1];
                          }
                          if((elock==1) & (T0lock==1)){

                              if(i==0){
                                  P[0][i]=orbele[0];
                              }
                              if(i==1) P[0][i]=orbele[2];
                              if(i==2) P[0][i]=orbele[3];
                              if(i==3) P[0][i]=orbele[4];
                              if(i==4){
                                  P[0][i]=orbele[6];
                              }
                          }
                      }
                      if(add==3){
                          if(i==0) P[0][i]=orbele[2];
                          if(i==1) P[0][i]=orbele[3];
                          if(i==2) P[0][i]=orbele[4];
                          if(i==3) P[0][i]=orbele[6];
                      }
                      cout<<P[0][i]<<" ";
                      LogFile<<P[0][i]<<" ";
                  }
                  cout<<endl;
                  LogFile<<endl;

                  for(int i=0; i<nu+1; i++){
                      for(int j=0; j<nu; j++){
                          if((i>0)&(i==j+1)){
                              if(add == 0){  // nothing locked
                                  if(j==0){
                                      e[i][j]=dP;
                                  }
                                  if(j==1){
                                      e[i][j]=de;
                                  }
                                  if(j==2){
                                      e[i][j]=dKA;
                                  }
                                  if(j==3){
                                      e[i][j]=dKB;
                                  }
                                  if(j==4){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==5){
                                      e[i][j]=dT0;
                                  }
                                  if(j==6){
                                      e[i][j]=dOmega;
                                  }
                              }
                              if((add == 1) & (Plock == 1)){ // only P locked

                              if(j==0){     //Eccentricity
                                  e[i][j]=de;
                              }
                              if(j==1){       //Velocities
                                  e[i][j]=dKA;
                              }
                              if(j==2){
                                  e[i][j]=dKB;
                              }
                              if(j==3){
                                  e[i][j]=dGamma;
                              }
                              if(j==4){     //periastron passage
                                  e[i][j]=dT0;
                              }
                              if(j==5){     //length periastron
                                  e[i][j]=dOmega;
                              }
                              }
                              if((add == 1) & (elock==1)){ // only e locked
                                  if(j==0){
                                      e[i][j]=dP; // Period
                                  }
                                  if(j==1){ // Amplitudes
                                      e[i][j]=dKA;
                                  }
                                  if(j==2){
                                      e[i][j]=dKB;
                                  }
                                  if(j==3){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==4){
                                      e[i][j]=dT0; // T0
                                  }
                                  if(j==5){
                                      e[i][j]=dOmega; // periastron length
                                  }
                              }
                              if((add==1) & (T0lock==1)){    // only T0 locked
                                  if(j==0){
                                      e[i][j]=dP; // Period
                                  }if(j==1){
                                      e[i][j]=de; // Eccentricity
                                  }
                                  if(j==2){ // Amplitudes
                                      e[i][j]=dKA;
                                  }
                                  if(j==3){
                                      e[i][j]=dKB;
                                  }
                                  if(j==4){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==5){
                                      e[i][j]=dOmega; // periastron length
                                  }
                              }
                              if((add==2) & (Plock == 1) & (elock ==1)){ // P and e locked
                                  if(j==0){ // Amplitudes
                                      e[i][j]=dKA;
                                  }
                                  if(j==1){
                                      e[i][j]=dKB;
                                  }
                                  if(j==2){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==3){
                                      e[i][j]=dT0;
                                  }
                                  if(j==4){
                                      e[i][j]=dOmega; // periastron length
                                  }
                              }
                              if((add==2) & (Plock==1) & (T0lock==1)){ // P and T0 locked
                                  if(j==0){
                                      e[i][j]=de;
                                  }
                                  if(j==1){ // Amplitudes
                                      e[i][j]=dKA;
                                  }
                                  if(j==2){
                                      e[i][j]=dKB;
                                  }
                                  if(j==3){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==4){
                                      e[i][j]=dOmega; // periastron length
                                  }
                              }
                              if((add==2) & (elock==1) & (T0lock==1)){ // e and T0 locked
                                  if(j==0){
                                      e[i][j]=dP;
                                  }
                                  if(j==1){ // Amplitudes
                                      e[i][j]=dKA;
                                  }
                                  if(j==2){
                                      e[i][j]=dKB;
                                  }
                                  if(j==3){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==4){
                                      e[i][j]=dOmega; // periastron length
                                  }
                              }
                              if(add==3){ // P, e and T0 locked
                                  if(j==0){ // Amplitudes
                                      e[i][j]=dKA;
                                    }
                                  if(j==1){
                                      e[i][j]=dKB;
                                  }
                                  if(j==2){
                                      e[i][j]=dGamma;
                                  }
                                  if(j==3){
                                      e[i][j]=dOmega; // periastron length
                                  }
                          }}
                          else e[i][j]=0;
                          cout<<e[i][j]<<" ";
                          LogFile<<e[i][j]<<" ";
                      }
                      cout<<endl;
                      LogFile<<endl;
                  }
                  cout <<endl;
                  LogFile<<endl;

                  for(int i=0; i<nu+1; i++){
                      for(int j=0; j<nu; j++){
                          P[i][j]=P[0][j]+e[i][j];
                          init2<<setprecision(14)<<P[i][j]<<endl;
                          cout<<P[i][j]<<" ";
                          LogFile<<P[i][j]<<" ";
                          if(add==0){
                              orbele[j]=P[i][j];    // no parameter locked
                          }
                          if(add==1){   // one parameter locked
                              if(Plock==1){
                                  orbele[j+1]=P[i][j];
                              }
                              if(elock==1){
                                  if(j==0){
                                      orbele[0]=P[i][0];
                                  }
                                  if(j>0){
                                      orbele[j+1]=P[i][j];
                                  }
                              }
                              if(T0lock==1){
                                  if(j==5){
                                      orbele[6]=P[i][j];
                                  }
                                  else orbele[j]=P[i][j];
                              }
                          }
                          if(add==2){   // two parameters locked
                              if((Plock==1) & (elock==1)){
                                  orbele[j+2]=P[i][j];
                              }
                              if((Plock==1) & (T0lock==1)){
                                  if(j==4){
                                      orbele[6]=P[i][j];
                                  }
                                  else{
                                      orbele[j+1]=P[i][j];
                                  }
                              }
                              if((elock==1) & (T0lock==1)){
                                  if(j==0){
                                      orbele[0]=P[i][0];
                                  }
                                  if(j==1) orbele[2]=P[i][1];
                                  if(j==2) orbele[3]=P[i][2];
                                  if(j==3) orbele[4]=P[i][3];
                                  if(j==4){
                                      orbele[6]=P[i][4];
                                  }
                              }
                          }
                          if(add==3){      // three parameters locked
                              if(j==0){
                                  orbele[2]=P[i][j];
                              }
                              if(j==1){
                                  orbele[3]=P[i][j];
                              }
                              if(j==2){
                                  orbele[4]=P[i][j];
                              }
                              if(j==3){
                                  orbele[6]=P[i][j];
                              }

                          }

                      }
                      LogFile<<endl;

                      eval++;
                      ui->spinBox_5->setValue(eval);
                      qApp->processEvents(QEventLoop::AllEvents);
                      MainWindow::VAmplitudeA();
                      MainWindow::VAmplitudeB();
                      MainWindow::ConstructMatrix();
                      y[i]=MainWindow::DivideConquer();
                      qApp->processEvents(QEventLoop::AllEvents);

                      if(upda==1){
                          upda=0;
                          LogFile<<"New Parameters found."<<endl;
                          LogFile<<"Residuum: "<<r1<<endl;
                          LogFile<<scientific<<"P: "<<orbele[0]<<endl;
                          LogFile<<scientific<<"e: "<<orbele[1]<<endl;
                          LogFile<<scientific<<"KA: "<<orbele[2]<<endl;
                          LogFile<<scientific<<"KB: "<<orbele[3]<<endl;
                          LogFile<<scientific<<"Gamma: "<<orbele[4]<<endl;
                          LogFile<<scientific<<"T0: "<<orbele[5]<<endl;
                          LogFile<<scientific<<"Omega: "<<orbele[6]<<endl;
                          LogFile<<endl;
                      }

                      LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;

                      if(abortt==1){
                          abortt=0;
                          LogFile<<"Initiation aborted."<<endl;
                          this->setCursor(QCursor(Qt::ArrowCursor));
                          return;
                      }
                      if(i==0) r1=y[0];
                      if(y[i]<r1) r1 = y[i];

                      if(ui->checkBox_10->isChecked()){
                          MainWindow::on_pushButton_6_clicked();
                      }

                      cout<<setprecision(12)<<y[i]<<endl;
                      init<<setprecision(14)<<y[i]<<endl;
                  }}

              //continue with optimised data
              if(ui->checkBox_11->isChecked()){
                  string zeile, one;

                  LogFile<<"Load data from previous optimisation."<<endl;

                  qOptval = ui->lineEdit_16->text();
                  string file1opt = qOptval.toUtf8().constData();
                  std::ostringstream file1NameStream(file1opt);
                  file1NameStream<<path<<"/"<<file1opt;
                  std::string file1Name = file1NameStream.str();
                  ifstream initiate1(file1Name.c_str());

                  qOptmat = ui->lineEdit_17->text();
                  string file2opt = qOptmat.toUtf8().constData();
                  std::ostringstream file2NameStream(file2opt);
                  file2NameStream<<path<<"/"<<file2opt;
                  std::string file2Name = file2NameStream.str();
                  ifstream initiate2(file2Name.c_str());

                  QFile checkfile(file2Name.c_str());

                  if(!checkfile.exists()){
                      qDebug()<<"The optimisation file "<<checkfile.fileName()<<" does not exist.";
                      QMessageBox::information(this, "Error", "The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimization needs to be initiated first.");
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimization needs to be initiated first."<<endl;
                      return;
                  }

                  int number_of_lines =0;

                  while(std::getline(initiate1, zeile))
                     ++ number_of_lines;

                  initiate1.clear();
                  initiate1.seekg(0, ios::beg);

                  if(number_of_lines!=nu+1){
                      qDebug()<<"Wrong optimisation file.";
                      QMessageBox::information(this, "Error", "The optimisation file does not match to the data! When running optimisation the first time for a given set of data, the optimization needs to be initiated first.");
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The optimisation file does not match to the data! When running optimisation the first time for a given set of data, the optimization needs to be initiated first."<<endl;
                      return;
                  }

                  for (int i=0; i<number_of_lines; i++){
                  initiate1 >> one;
                  istringstream ist(one);
                  ist >> y[i];
                  if(i==0) r1=y[0];
                  if(y[i]<r1){
                      r1=y[i];
                  }
                  }
                  initiate1.close();

                  int coun=0;

                  for (int i=0; i<number_of_lines*nu; i++){
                  initiate2 >> one;
                  istringstream ist(one);
                  ist >> P[coun][i-coun*nu];
                  cout<<"P["<<coun<<"]["<<i-coun*nu<<"]: "<<P[coun][i-coun*nu]<<" ";
                  if(i==coun*nu+nu-1){
                      cout<<endl;
                      ++coun;
                  }
                  }
                  initiate2.close();

                  LogFile<<"Optimisation data successfully loaded."<<endl;
              }

              // Use initial data
              if(ui->checkBox_12->isChecked()){

                  string zeile, one;

                  LogFile<<"Load initial data."<<endl;

                  qInitval = ui->lineEdit_14->text();
                  string file1init = qInitval.toUtf8().constData();
                  std::ostringstream file1NameStream(file1init);
                  file1NameStream<<path<<"/"<<file1init;
                  std::string file1Name = file1NameStream.str();
                  ifstream initiate1(file1Name.c_str());

                  qInitmat = ui->lineEdit_15->text();
                  string file2init = qInitmat.toUtf8().constData();
                  std::ostringstream file2NameStream(file2init);
                  file2NameStream<<path<<"/"<<file2init;
                  std::string file2Name = file2NameStream.str();
                  ifstream initiate2(file2Name.c_str());

                  QFile checkfile(file2Name.c_str());

                  if(!checkfile.exists()){
                      qDebug()<<"The initiation file "<<checkfile.fileName()<<" does not exist.";
                      QMessageBox::information(this, "Error", "The initiation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The initiation file does not exist! When running optimisation the first time for a given set of data, the optimization needs to be initiated first."<<endl;
                      return;
                  }

                  int number_of_lines =0;

                  while(std::getline(initiate1, zeile))
                     ++ number_of_lines;

                  initiate1.clear();
                  initiate1.seekg(0, ios::beg);

                  if(number_of_lines!=nu+1){
                      qDebug()<<"Wrong initiate file.";
                      QMessageBox::information(this, "Error", "The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      cout<<number_of_lines<<endl;
                      LogFile<<"The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimization needs to be initiated first."<<endl;
                      return;
                  }

                  for (int i=0; i<number_of_lines; i++){
                  initiate1 >> one;
                  istringstream ist(one);
                  ist >> y[i];
                  if(i==0) r1=y[0];
                  if(y[i]<r1){
                      r1=y[i];
                  }
                  }
                  initiate1.close();

                  int coun=0;

                  for (int i=0; i<number_of_lines*nu; i++){
                  initiate2 >> one;
                  istringstream ist(one);
                  ist >> P[coun][i-coun*nu];
                  cout<<"P["<<coun<<"]["<<i-coun*nu<<"]: "<<P[coun][i-coun*nu]<<" ";
                  if(i==coun*nu+nu-1){
                      cout<<endl;
                      ++coun;
                  }
                  }
                  initiate2.close();

                  LogFile<<"Initial data successfully loaded."<<endl;
              }
              qOptval = ui->lineEdit_16->text();
              string file1opt = qOptval.toUtf8().constData();
              std::ostringstream file1NameStream(file1opt);
              file1NameStream<<path<<"/"<<file1opt;
              std::string file1Name = file1NameStream.str();
              ofstream opt1(file1Name.c_str());

              qOptmat = ui->lineEdit_17->text();
              string file2opt = qOptmat.toUtf8().constData();
              std::ostringstream file2NameStream(file2opt);
              file2NameStream<<path<<"/"<<file2opt;
              std::string file2Name = file2NameStream.str();
              ofstream opt2(file2Name.c_str());

              LogFile<<endl;
              LogFile<<"*************************"<<endl;
              LogFile<<"Start Optimisation"<<endl;

              if(ui->checkBox_32->isChecked()){
                  ui->checkBox_9->setChecked(false);
                  ui->checkBox_11->setChecked(true);
                  ui->checkBox_12->setChecked(false);
              }


                  //start main loop
                  for(int t=0; t<zaehler; t++){
                      ui->spinBox_4->setValue(t+1);

                      LogFile<<endl;

                      LogFile<<"Iteration: "<<t+1<<"; ";
                      qApp->processEvents(QEventLoop::AllEvents);

                      cout<<endl;
                      cout<<endl;
                      cout<<"Iteration: "<<t+1<<endl;

                      //initialize next step
                      ym = 0;
                      ys = 0;

                      for(int i=0; i<nu; i++){
                          Z[i]=0;
                      }

                      //looking for highest value
                      yh=y[0];
                      for(int i=0; i<nu+1; i++){
                          if(y[i]>=yh){
                              yh = y[i];
                              Ph = i;
                          }
                      }

                      //looking for smallest value
                      yl = yh;
                      for(int i=0; i<nu+1; i++){
                          if(y[i]<yl){
                              yl = y[i];
                              Pl = i;
                          }
                      }

                      //looking for second highest value
                      ysh = yl;
                      for(int i=0; i<nu+1; i++){
                          if((y[i]>ysh) &(y[i]<yh) & (y[i]>yl) & (i!=Pl)){
                              for(int i=0; i<nu+1; i++){
                                 ysh = y[i];
                                 Psh = i;
                                }
                            }
                      }

                      yh=y[Ph];
                      yl=y[Pl];
                      ysh=y[Psh];

                      //computing mean and sigma
                      for(int i=0; i<nu+1; i++){
                          ym+=y[i]/(nu+1);
                      }
                      for(int i=0; i<nu+1; i++){
                          ys+=sqrt(pow((y[i]-ym),2));
                      }
                      ys = ys/nu;

                      if((ys==ys2) & (ym==ym2)){
                          ++stagnate;

                          if(ui->checkBox_32->isChecked()){
                              cout<<"Reinitiate optimisation with current best orbit."<<endl;
                              ui->plainTextEdit_2->appendPlainText("Reinitiate optimisation with current best orbit.");
                              LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                              LogFile<<endl;
                              LogFile<<"Reinitiate optimisation with current best orbit."<<endl;

                              ofstream borbit("orbitelements.dat");

                              if(add==0){
                              for(int k=0; k<7; k++){
                              borbit<<setprecision(14)<<P[Pl][k]<<endl;
                              }
                              }
                              if(add==1){
                                  if(Plock==1){
                                  borbit<<setprecision(14)<<orbele[0]<<endl;
                                  for(int k=0; k<6; k++){
                                  borbit<<setprecision(14)<<P[Pl][k]<<endl;
                                  }
                                  }
                                  if(elock==1){
                                      borbit<<setprecision(14)<<P[Pl][0]<<endl;
                                      borbit<<setprecision(14)<<orbele[1]<<endl;
                                      for(int k=0; k<5; k++){
                                      borbit<<setprecision(14)<<P[Pl][k+1]<<endl;
                                      }
                                  }
                                  if(T0lock==1){
                                      for(int k=0; k<5; k++){
                                      borbit<<setprecision(14)<<P[Pl][k]<<endl;
                                      }
                                      borbit<<setprecision(14)<<orbele[5]<<endl;
                                      borbit<<setprecision(14)<<P[Pl][5]<<endl;
                                  }
                              }
                              if(add==2){
                                  if((Plock==1) & (elock==1)){
                                      borbit<<setprecision(14)<<orbele[0]<<endl;
                                      borbit<<setprecision(14)<<orbele[1]<<endl;
                                      for(int k=0; k<5; k++){
                                      borbit<<setprecision(14)<<P[Pl][k]<<endl;
                                      }
                                  }
                                  if((Plock==1) & (T0lock==1)){
                                      borbit<<setprecision(14)<<orbele[0]<<endl;
                                      for(int k=0; k<4; k++){
                                      borbit<<setprecision(14)<<P[Pl][k]<<endl;
                                      }
                                      borbit<<setprecision(14)<<orbele[5]<<endl;
                                      borbit<<setprecision(14)<<P[Pl][4]<<endl;
                                  }
                                  if((elock==1) & (T0lock==1)){
                                      borbit<<setprecision(14)<<P[Pl][0]<<endl;
                                      borbit<<setprecision(14)<<orbele[1]<<endl;
                                      borbit<<setprecision(14)<<P[Pl][1]<<endl;
                                      borbit<<setprecision(14)<<P[Pl][2]<<endl;
                                      borbit<<setprecision(14)<<P[Pl][3]<<endl;
                                      borbit<<setprecision(14)<<orbele[5]<<endl;
                                      borbit<<setprecision(14)<<P[Pl][4]<<endl;
                                  }
                              }
                              if(add==3){
                                  borbit<<setprecision(14)<<orbele[0]<<endl;
                                  borbit<<setprecision(14)<<orbele[1]<<endl;
                                  borbit<<setprecision(14)<<P[Pl][0]<<endl;
                                  borbit<<setprecision(14)<<P[Pl][1]<<endl;
                                  borbit<<setprecision(14)<<P[Pl][2]<<endl;
                                  borbit<<setprecision(14)<<orbele[5]<<endl;
                                  borbit<<setprecision(14)<<P[Pl][3]<<endl;
                              }

                              ui->checkBox_9->setChecked(true);
                              ui->checkBox_11->setChecked(false);
                              ui->checkBox_12->setChecked(false);

                              reinitiate=1;

                              return;

                          }
                          else{
                              QString stagn = QString::number(stagnate);
                              LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                              cout<<"Optimisation may stagnate. No change in mean and STD of simplex points since "<<stagnate<<" iterations."<<endl;
                              ui->plainTextEdit_2->appendPlainText("Optimisation may stagnate. No change in mean and STD of simplex points since "+stagn+" iterations.");
                          }

                          }

                      else {
                          stagnate =0;
                      }

                      cout<<"Stag-Index: "<<stagnate<<endl;

                      ys2=ys;
                      ym2=ym;

                      cout<<"Simplex Mean: "<<ym<<" Simplex STD: "<<ys<<endl;
                      LogFile<<setprecision(12)<<"Mean: "<<ym<<"; STD: "<<ys<<endl;

                      //compute centroid
                      for(int j=0; j<nu; j++){
                          for(int i=0; i<nu+1; i++){
                              if(i!=Ph){
                                  Z[j]+=P[i][j]/nu;
                              }
                          }
                      }



                      //reflect highest value at centroid
                      for(int i=0; i<nu; i++){
                          Co[i]=Z[i]+alpha*(Z[i]-P[Ph][i]);
                          if(add==0){
                              orbele[i]=Co[i];    // no parameter locked
                          }
                          if(add==1){   // one parameter locked
                              if(Plock==1){
                                  orbele[i+1]=Co[i];
                              }
                              if(elock==1){
                                  if(i==0){
                                      orbele[0]=Co[i];
                                  }
                                  if(i>0){
                                      orbele[i+1]=Co[i];
                                  }
                              }
                              if(T0lock==1){
                                  if(i==5){
                                      orbele[6]=Co[i];
                                  }
                                  else orbele[i]=Co[i];
                              }
                          }
                          if(add==2){   // two parameters locked
                              if((Plock==1) & (elock==1)){
                                  orbele[i+2]=Co[i];
                              }
                              if((Plock==1) & (T0lock==1)){
                                  if(i==4){
                                      orbele[6]=Co[i];
                                  }
                                  else{
                                      orbele[i+1]=Co[i];
                                  }
                              }
                              if((elock==1) & (T0lock==1)){
                                  if(i==0){
                                      orbele[0]=Co[i];
                                  }
                                  if(i==1) orbele[2]=Co[i];
                                  if(i==2) orbele[3]=Co[i];
                                  if(i==3) orbele[4]=Co[i];
                                  if(i==4){
                                      orbele[6]=Co[i];
                                  }
                              }
                          }
                          if(add==3){      // three parameters locked
                              if(i==0){
                                  orbele[2]=Co[i];
                              }
                              if(i==1){
                                  orbele[3]=Co[i];
                              }
                              if(i==2){
                                  orbele[4]=Co[i];
                              }
                              if(i==3){
                                  orbele[6]=Co[i];
                              }

                          }
                      }
                      eval++;
                      ui->spinBox_5->setValue(eval);
                      qApp->processEvents(QEventLoop::AllEvents);
                      MainWindow::VAmplitudeA();
                      MainWindow::VAmplitudeB();
                      MainWindow::ConstructMatrix();
                      yi=MainWindow::DivideConquer();
                      qApp->processEvents(QEventLoop::AllEvents);

                      if(upda==1){
                          upda=0;
                          LogFile<<"New Parameters found."<<endl;
                          LogFile<<"Residuum: "<<r1<<endl;
                          LogFile<<scientific<<"P: "<<orbele[0]<<endl;
                          LogFile<<scientific<<"e: "<<orbele[1]<<endl;
                          LogFile<<scientific<<"KA: "<<orbele[2]<<endl;
                          LogFile<<scientific<<"KB: "<<orbele[3]<<endl;
                          LogFile<<scientific<<"Gamma: "<<orbele[4]<<endl;
                          LogFile<<scientific<<"T0: "<<orbele[5]<<endl;
                          LogFile<<scientific<<"Omega: "<<orbele[6]<<endl;
                          LogFile<<endl;
                      }

                      if(abortt==1){
                          //output results
                          LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                              for(int i = 0; i<nu;i++){
                                  LogFile<<P[Pl][i]<<endl;
                              }
                          LogFile<<endl;
                          for(int i=0; i<nu+1; i++){
                              cout<<y[i]<<endl;
                              opt1<<setprecision(14)<<y[i]<<endl;
                              for(int j =0; j<nu; j++){
                                  opt2<<setprecision(14)<<P[i][j]<<endl;
                              }
                          }
                          opt1.close();
                          opt2.close();
                          abortt=0;
                          this->setCursor(QCursor(Qt::ArrowCursor));
                          return;
                      }
                      cout<<"reflection 1"<<endl;

                      //Alpha Branch, better than best point=So[i]=So[i]
                      if(yi<yl){
                          cout<<"expansion"<<endl;
                          LogFile<<"yi<yl, expansion; ";
                          for(int i=0; i<nu; i++){
                              Eo[i]=Z[i]+Gamma*(Co[i]-Z[i]);
                              if(add==0){
                                  orbele[i]=Eo[i];    // no parameter locked
                              }
                              if(add==1){   // one parameter locked
                                  if(Plock==1){
                                      orbele[i+1]=Eo[i];
                                  }
                                  if(elock==1){
                                      if(i==0){
                                          orbele[0]=Eo[i];
                                      }
                                      if(i>0){
                                          orbele[i+1]=Eo[i];
                                      }
                                  }
                                  if(T0lock==1){
                                      if(i==5){
                                          orbele[6]=Eo[i];
                                      }
                                      else orbele[i]=Eo[i];
                                  }
                              }
                              if(add==2){   // two parameters locked
                                  if((Plock==1) & (elock==1)){
                                      orbele[i+2]=Eo[i];
                                  }
                                  if((Plock==1) & (T0lock==1)){
                                      if(i==4){
                                          orbele[6]=Eo[i];
                                      }
                                      else{
                                          orbele[i+1]=Eo[i];
                                      }
                                  }
                                  if((elock==1) & (T0lock==1)){
                                      if(i==0){
                                          orbele[0]=Eo[i];
                                      }
                                      if(i==1) orbele[2]=Eo[i];
                                      if(i==2) orbele[3]=Eo[i];
                                      if(i==3) orbele[4]=Eo[i];
                                      if(i==4){
                                          orbele[6]=Eo[i];
                                      }
                                  }
                              }
                              if(add==3){      // three parameters locked
                                  if(i==0){
                                      orbele[2]=Eo[i];
                                  }
                                  if(i==1){
                                      orbele[3]=Eo[i];
                                  }
                                  if(i==2){
                                      orbele[4]=Eo[i];
                                  }
                                  if(i==3){
                                      orbele[6]=Eo[i];
                                  }

                              }
                          }
                          eval++;
                          ui->spinBox_5->setValue(eval);
                          qApp->processEvents(QEventLoop::AllEvents);
                          MainWindow::VAmplitudeA();
                          MainWindow::VAmplitudeB();
                          MainWindow::ConstructMatrix();
                          yt=MainWindow::DivideConquer();
                          qApp->processEvents(QEventLoop::AllEvents);

                          if(upda==1){
                              upda=0;
                              LogFile<<"New Parameters found."<<endl;
                              LogFile<<"Residuum: "<<r1<<endl;
                              LogFile<<scientific<<"P: "<<orbele[0]<<endl;
                              LogFile<<scientific<<"e: "<<orbele[1]<<endl;
                              LogFile<<scientific<<"KA: "<<orbele[2]<<endl;
                              LogFile<<scientific<<"KB: "<<orbele[3]<<endl;
                              LogFile<<scientific<<"Gamma: "<<orbele[4]<<endl;
                              LogFile<<scientific<<"T0: "<<orbele[5]<<endl;
                              LogFile<<scientific<<"Omega: "<<orbele[6]<<endl;
                              LogFile<<endl;
                          }

                          if(abortt==1){
                              //output results
                              LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                  for(int i = 0; i<nu;i++){
                                      LogFile<<P[Pl][i]<<endl;
                                  }
                              for(int i=0; i<nu+1; i++){
                                  cout<<y[i]<<endl;
                                  opt1<<setprecision(14)<<y[i]<<endl;
                                  for(int j =0; j<nu; j++){
                                      opt2<<setprecision(14)<<P[i][j]<<endl;
                                  }
                              }
                              opt1.close();
                              opt2.close();
                              abortt=0;
                              this->setCursor(QCursor(Qt::ArrowCursor));
                              return;
                          }
                          if(yt<yl){
                              cout<<"exp ";
                              for(int i=0; i<nu; i++){
                                  P[Ph][i]=Eo[i];
                              }
                              y[Ph]=yt;

                          }
                          if(yt>=yl){
                              cout<<"ref1 ";
                              for(int i=0; i<nu; i++){
                                  P[Ph][i]=Co[i];
                                  if(add==0){
                                      orbele[i]=Co[i];    // no parameter locked
                                  }
                                  if(add==1){   // one parameter locked
                                      if(Plock==1){
                                          orbele[i+1]=Co[i];
                                      }
                                      if(elock==1){
                                          if(i==0){
                                              orbele[0]=Co[i];
                                          }
                                          if(i>0){
                                              orbele[i+1]=Co[i];
                                          }
                                      }
                                      if(T0lock==1){
                                          if(i==5){
                                              orbele[6]=Co[i];
                                          }
                                          else orbele[i]=Co[i];
                                      }
                                  }
                                  if(add==2){   // two parameters locked
                                      if((Plock==1) & (elock==1)){
                                          orbele[i+2]=Co[i];
                                      }
                                      if((Plock==1) & (T0lock==1)){
                                          if(i==4){
                                              orbele[6]=Co[i];
                                          }
                                          else{
                                              orbele[i+1]=Co[i];
                                          }
                                      }
                                      if((elock==1) & (T0lock==1)){
                                          if(i==0){
                                              orbele[0]=Co[i];
                                          }
                                          if(i==1) orbele[2]=Co[i];
                                          if(i==2) orbele[3]=Co[i];
                                          if(i==3) orbele[4]=Co[i];
                                          if(i==4){
                                              orbele[6]=Co[i];
                                          }
                                      }
                                  }
                                  if(add==3){      // three parameters locked
                                      if(i==0){
                                          orbele[2]=Co[i];
                                      }
                                      if(i==1){
                                          orbele[3]=Co[i];
                                      }
                                      if(i==2){
                                          orbele[4]=Co[i];
                                      }
                                      if(i==3){
                                          orbele[6]=Co[i];
                                      }

                                  }
                              }
                              /*
                              eval++;
                              ui->spinBox_5->setValue(eval);
                              qApp->processEvents(QEventLoop::AllEvents);
                              MainWindow::VAmplitudeA();
                              MainWindow::VAmplitudeB();
                              MainWindow::ConstructMatrix();
                              */
                              y[Ph]=yi;//MainWindow::DivideConquer();
                              if(abortt==1){
                                  //output results
                                  LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                      for(int i = 0; i<nu;i++){
                                          LogFile<<P[Pl][i]<<endl;
                                      }
                                  for(int i=0; i<nu+1; i++){
                                      cout<<y[i]<<endl;
                                      opt1<<setprecision(14)<<y[i]<<endl;
                                      for(int j =0; j<nu; j++){
                                          opt2<<setprecision(14)<<P[i][j]<<endl;
                                      }
                                  }
                                  opt1.close();
                                  opt2.close();
                                  abortt=0;
                                  this->setCursor(QCursor(Qt::ArrowCursor));
                                  return;
                              }
                              qApp->processEvents(QEventLoop::AllEvents);
                          }
                      }
                      //Alpha Branch End

                      //Beta Brnach, worse than best point
                      if(yi>=yl){
                          //Beta_2 branch
                          cout<<"yi>=yl ";
                          LogFile<<"yi>=yl ";
                          if(yi<=ysh){
                              cout<<"& <=ysh, ref2 ";
                              LogFile<<"& <=ysh, reflection2; ";
                              for(int i=0; i<nu; i++){
                                  P[Ph][i]=Co[i];
                              }
                              /*
                              eval++;
                              ui->spinBox_5->setValue(eval);
                              qApp->processEvents(QEventLoop::AllEvents);
                              MainWindow::VAmplitudeA();
                              MainWindow::VAmplitudeB();
                              MainWindow::ConstructMatrix();
                              */
                              y[Ph]=yi;//MainWindow::DivideConquer();
                              if(abortt==1){
                                  //output results
                                  LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                      for(int i = 0; i<nu;i++){
                                          LogFile<<P[Pl][i]<<endl;
                                      }
                                  for(int i=0; i<nu+1; i++){
                                      cout<<y[i]<<endl;
                                      opt1<<setprecision(14)<<y[i]<<endl;
                                      for(int j =0; j<nu; j++){
                                          opt2<<setprecision(14)<<P[i][j]<<endl;
                                      }
                                  }
                                  opt1.close();
                                  opt2.close();
                                  abortt=0;
                                  this->setCursor(QCursor(Qt::ArrowCursor));
                                  return;
                              }
                              qApp->processEvents(QEventLoop::AllEvents);
                          }//Beta_2 branch end

                          //Beta_1 branch
                          if(yi>ysh){
                              cout<<"yi>ysh ";
                              LogFile<<"yi>ysh ";
                              if(yi<=yh){
                                  cout<<"yi<= yh ref3 ";
                                  LogFile<<"yi <=yh, relfection3; ";
                                  for(int i=0; i<nu; i++){
                                      P[Ph][i]=Co[i];
                                  }
                                  /*
                                  eval++;
                                  ui->spinBox_5->setValue(eval);
                                  qApp->processEvents(QEventLoop::AllEvents);
                                  MainWindow::VAmplitudeA();
                                  MainWindow::VAmplitudeB();
                                  MainWindow::ConstructMatrix();
                                  */
                                  y[Ph]=yi;//MainWindow::DivideConquer();
                                  if(abortt==1){
                                      //output results
                                      LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                          for(int i = 0; i<nu;i++){
                                              LogFile<<P[Pl][i]<<endl;
                                          }
                                      for(int i=0; i<nu+1; i++){
                                          cout<<y[i]<<endl;
                                          opt1<<setprecision(14)<<y[i]<<endl;
                                          for(int j =0; j<nu; j++){
                                              opt2<<setprecision(14)<<P[i][j]<<endl;
                                          }
                                      }
                                      opt1.close();
                                      opt2.close();
                                      abortt=0;
                                      this->setCursor(QCursor(Qt::ArrowCursor));
                                      return;
                                  }
                                  qApp->processEvents(QEventLoop::AllEvents);
                                  yh=y[Ph];
                              }
                              for(int i=0; i<nu; i++){
                                  So[i]=Z[i]+beta*(P[Ph][i]-Z[i]);
                                  if(add==0){
                                      orbele[i]=So[i];    // no parameter locked
                                  }
                                  if(add==1){   // one parameter locked
                                      if(Plock==1){
                                          orbele[i+1]=So[i];
                                      }
                                      if(elock==1){
                                          if(i==0){
                                              orbele[0]=So[i];
                                          }
                                          if(i>0){
                                              orbele[i+1]=So[i];
                                          }
                                      }
                                      if(T0lock==1){
                                          if(i==5){
                                              orbele[6]=So[i];
                                          }
                                          else orbele[i]=So[i];
                                      }
                                  }
                                  if(add==2){   // two parameters locked
                                      if((Plock==1) & (elock==1)){
                                          orbele[i+2]=So[i];
                                      }
                                      if((Plock==1) & (T0lock==1)){
                                          if(i==4){
                                              orbele[6]=So[i];
                                          }
                                          else{
                                              orbele[i+1]=So[i];
                                          }
                                      }
                                      if((elock==1) & (T0lock==1)){
                                          if(i==0){
                                              orbele[0]=So[i];
                                          }
                                          if(i==1) orbele[2]=So[i];
                                          if(i==2) orbele[3]=So[i];
                                          if(i==3) orbele[4]=So[i];
                                          if(i==4){
                                              orbele[6]=So[i];
                                          }
                                      }
                                  }
                                  if(add==3){      // three parameters locked
                                      if(i==0){
                                          orbele[2]=So[i];
                                      }
                                      if(i==1){
                                          orbele[3]=So[i];
                                      }
                                      if(i==2){
                                          orbele[4]=So[i];
                                      }
                                      if(i==3){
                                          orbele[6]=So[i];
                                      }

                                  }
                              }
                              eval++;
                              ui->spinBox_5->setValue(eval);
                              qApp->processEvents(QEventLoop::AllEvents);
                              MainWindow::VAmplitudeA();
                              MainWindow::VAmplitudeB();
                              MainWindow::ConstructMatrix();
                              yt=MainWindow::DivideConquer();
                              qApp->processEvents(QEventLoop::AllEvents);

                              if(upda==1){
                                  upda=0;
                                  LogFile<<"New Parameters found."<<endl;
                                  LogFile<<"Residuum: "<<r1<<endl;
                                  LogFile<<scientific<<"P: "<<orbele[0]<<endl;
                                  LogFile<<scientific<<"e: "<<orbele[1]<<endl;
                                  LogFile<<scientific<<"KA: "<<orbele[2]<<endl;
                                  LogFile<<scientific<<"KB: "<<orbele[3]<<endl;
                                  LogFile<<scientific<<"Gamma: "<<orbele[4]<<endl;
                                  LogFile<<scientific<<"T0: "<<orbele[5]<<endl;
                                  LogFile<<scientific<<"Omega: "<<orbele[6]<<endl;
                                  LogFile<<endl;
                              }

                              if(abortt==1){
                                  //output results
                                  LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                      for(int i = 0; i<nu;i++){
                                          LogFile<<P[Pl][i]<<endl;
                                      }
                                  for(int i=0; i<nu+1; i++){
                                      cout<<y[i]<<endl;
                                      opt1<<setprecision(14)<<y[i]<<endl;
                                      for(int j =0; j<nu; j++){
                                          opt2<<setprecision(14)<<P[i][j]<<endl;
                                      }
                                  }
                                  opt1.close();
                                  opt2.close();
                                  abortt=0;
                                  this->setCursor(QCursor(Qt::ArrowCursor));
                                  return;
                              }

                              //Beta_1_1 branch
                              if(yt>yh){
                                  cout<<"yt <yh tot ";
                                  LogFile<<"yt>yh, total contraction; ";
                                  for(int j=0; j<nu+1; j++){
                                      for(int i =0; i<nu; i++){
                                          P[j][i]=P[Pl][i]+btot*(P[j][i]-P[Pl][i]);
                                          if(add==0){
                                              orbele[i]=P[j][i];    // no parameter locked
                                          }
                                          if(add==1){   // one parameter locked
                                              if(Plock==1){
                                                  orbele[i+1]=P[j][i];
                                              }
                                              if(elock==1){
                                                  if(i==0){
                                                      orbele[0]=P[j][i];
                                                  }
                                                  if(i>0){
                                                      orbele[i+1]=P[j][i];
                                                  }
                                              }
                                              if(T0lock==1){
                                                  if(i==5){
                                                      orbele[6]=P[j][i];
                                                  }
                                                  else orbele[i]=P[j][i];
                                              }
                                          }
                                          if(add==2){   // two parameters locked
                                              if((Plock==1) & (elock==1)){
                                                  orbele[i+2]=P[j][i];
                                              }
                                              if((Plock==1) & (T0lock==1)){
                                                  if(i==4){
                                                      orbele[6]=P[j][i];
                                                  }
                                                  else{
                                                      orbele[i+1]=P[j][i];
                                                  }
                                              }
                                              if((elock==1) & (T0lock==1)){
                                                  if(i==0){
                                                      orbele[0]=P[j][i];
                                                  }
                                                  if(i==1) orbele[2]=P[j][i];
                                                  if(i==2) orbele[3]=P[j][i];
                                                  if(i==3) orbele[4]=P[j][i];
                                                  if(i==4){
                                                      orbele[6]=P[j][i];
                                                  }
                                              }
                                          }
                                          if(add==3){      // three parameters locked
                                              if(i==0){
                                                  orbele[2]=P[j][i];
                                              }
                                              if(i==1){
                                                  orbele[3]=P[j][i];
                                              }
                                              if(i==2){
                                                  orbele[4]=P[j][i];
                                              }
                                              if(i==3){
                                                  orbele[6]=P[j][i];
                                              }

                                          }
                                      }
                                      eval++;
                                      ui->spinBox_5->setValue(eval);
                                      qApp->processEvents(QEventLoop::AllEvents);
                                      MainWindow::VAmplitudeA();
                                      MainWindow::VAmplitudeB();
                                      MainWindow::ConstructMatrix();
                                      y[j]=MainWindow::DivideConquer();
                                      qApp->processEvents(QEventLoop::AllEvents);

                                      if(upda==1){
                                          upda=0;

                                          LogFile<<"New Parameters found."<<endl;
                                          LogFile<<"Residuum: "<<r1<<endl;
                                          LogFile<<scientific<<"P: "<<orbele[0]<<endl;
                                          LogFile<<scientific<<"e: "<<orbele[1]<<endl;
                                          LogFile<<scientific<<"KA: "<<orbele[2]<<endl;
                                          LogFile<<scientific<<"KB: "<<orbele[3]<<endl;
                                          LogFile<<scientific<<"Gamma: "<<orbele[4]<<endl;
                                          LogFile<<scientific<<"T0: "<<orbele[5]<<endl;
                                          LogFile<<scientific<<"Omega: "<<orbele[6]<<endl;
                                          LogFile<<endl;
                                      }

                                      if(abortt==1){
                                          //output results
                                          LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                              for(int i = 0; i<nu;i++){
                                                  LogFile<<P[Pl][i]<<endl;
                                              }
                                          for(int i=0; i<nu+1; i++){
                                              cout<<y[i]<<endl;
                                              opt1<<setprecision(14)<<y[i]<<endl;
                                              for(int j =0; j<nu; j++){
                                                  opt2<<setprecision(14)<<P[i][j]<<endl;
                                              }
                                          }
                                          opt1.close();
                                          opt2.close();
                                          abortt=0;
                                          this->setCursor(QCursor(Qt::ArrowCursor));
                                          return;
                                      }
                                  }
                              }//end beta_1_1 branch

                              //Beta_1_2 branch
                              if(yt<=yh){
                                  cout<<"yt<=yh sco ";
                                  LogFile<<"yt<=yh, single contraction; ";
                                  for(int i=0; i<nu; i++){
                                      P[Ph][i]=So[i];
                                      if(add==0){
                                          orbele[i]=So[i];    // no parameter locked
                                      }
                                      if(add==1){   // one parameter locked
                                          if(Plock==1){
                                              orbele[i+1]=So[i];
                                          }
                                          if(elock==1){
                                              if(i==0){
                                                  orbele[0]=So[i];
                                              }
                                              if(i>0){
                                                  orbele[i+1]=So[i];
                                              }
                                          }
                                          if(T0lock==1){
                                              if(i==5){
                                                  orbele[6]=So[i];
                                              }
                                              else orbele[i]=So[i];
                                          }
                                      }
                                      if(add==2){   // two parameters locked
                                          if((Plock==1) & (elock==1)){
                                              orbele[i+2]=So[i];
                                          }
                                          if((Plock==1) & (T0lock==1)){
                                              if(i==4){
                                                  orbele[6]=So[i];
                                              }
                                              else{
                                                  orbele[i+1]=So[i];
                                              }
                                          }
                                          if((elock==1) & (T0lock==1)){
                                              if(i==0){
                                                  orbele[0]=So[i];
                                              }
                                              if(i==1) orbele[2]=So[i];
                                              if(i==2) orbele[3]=So[i];
                                              if(i==3) orbele[4]=So[i];
                                              if(i==4){
                                                  orbele[6]=So[i];
                                              }
                                          }
                                      }
                                      if(add==3){      // three parameters locked
                                          if(i==0){
                                              orbele[2]=So[i];
                                          }
                                          if(i==1){
                                              orbele[3]=So[i];
                                          }
                                          if(i==2){
                                              orbele[4]=So[i];
                                          }
                                          if(i==3){
                                              orbele[6]=So[i];
                                          }

                                      }
                                  }
                                  /*
                                  eval++;
                                  ui->spinBox_5->setValue(eval);
                                  qApp->processEvents(QEventLoop::AllEvents);
                                  MainWindow::VAmplitudeA();
                                  MainWindow::VAmplitudeB();
                                  MainWindow::ConstructMatrix();
                                  */
                                  y[Ph]=yt;//MainWindow::DivideConquer();
                                  if(abortt==1){
                                      //output results
                                      LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                          for(int i = 0; i<nu;i++){
                                              LogFile<<P[Pl][i]<<endl;
                                          }
                                      for(int i=0; i<nu+1; i++){
                                          cout<<y[i]<<endl;
                                          opt1<<setprecision(14)<<y[i]<<endl;
                                          for(int j =0; j<nu; j++){
                                              opt2<<setprecision(14)<<P[i][j]<<endl;
                                          }
                                      }
                                      opt1.close();
                                      opt2.close();
                                      abortt=0;
                                      this->setCursor(QCursor(Qt::ArrowCursor));
                                      return;
                                  }
                                  qApp->processEvents(QEventLoop::AllEvents);
                              }//end Beta_1_2 branch
                          }//Beta_1 branch end
                      }//end Beta branch

                      /*for(int i=0; i<num; i++){
                          QString index= QString::number(i);
                          QString velocity=QString::number(RV1[i]);
                          QString sum=QString::number(RV3[i]);
                          ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+sum);
                          //cout<<RV1[i]<<" "<<RV3[i]<<endl;
                      }*/

                  }// end main loop


                  //looking for highest value
                  yh=y[0];
                  for (int j=0; j<nu+1; j++){
                        if(y[j]>yh){
                            yh = y[j];
                            Ph = j;
                        }
                  }

                  //looking for smallest value
                  yl=yh;
                  for (int j=0; j<nu+1; j++){
                        if(y[j]<yl){
                            yl=y[j];
                            Pl = j;
                        }
                  }

                  //looking for second highest value
                  ysh=yl;
                  for (int j=0; j<nu+1; j++){
                        if((y[j]>ysh)&(y[j]<yh)){
                            ysh=y[j];
                            Psh=j;
                        }
                  }

                  //output results
                  for(int i=0; i<nu+1;i++){
                        cout<<y[i]<<endl;
                        opt1<<setprecision(14)<<y[i]<<endl;
                        for(int j=0; j<nu; j++){
                            opt2<<setprecision(14)<<P[i][j]<<endl;
                        }
                  }

               for(int i =0; i<nu; i++){
                  if(add==0){
                      orbele[i]=P[Pl][i];    // no parameter locked
                  }
                  if(add==1){   // one parameter locked
                      if(Plock==1){
                          orbele[i+1]=P[Pl][i];
                      }
                      if(elock==1){
                          if(i==0){
                              orbele[0]=P[Pl][i];
                          }
                          if(i>0){
                              orbele[i+1]=P[Pl][i];
                          }
                      }
                      if(T0lock==1){
                          if(i==5){
                              orbele[6]=P[Pl][i];
                          }
                          else orbele[i]=P[Pl][i];
                      }
                  }
                  if(add==2){   // two parameters locked
                      if((Plock==1) & (elock==1)){
                          orbele[i+2]=P[Pl][i];
                      }
                      if((Plock==1) & (T0lock==1)){
                          if(i==4){
                              orbele[6]=P[Pl][i];
                          }
                          else{
                              orbele[i+1]=P[Pl][i];
                          }
                      }
                      if((elock==1) & (T0lock==1)){
                          if(i==0){
                              orbele[0]=So[i];
                          }
                          if(i==1) orbele[2]=P[Pl][i];
                          if(i==2) orbele[3]=P[Pl][i];
                          if(i==3) orbele[4]=P[Pl][i];
                          if(i==4){
                              orbele[6]=P[Pl][i];
                          }
                      }
                  }
                  if(add==3){      // three parameters locked
                      if(i==0){
                          orbele[2]=P[Pl][i];
                      }
                      if(i==1){
                          orbele[3]=P[Pl][i];
                      }
                      if(i==2){
                          orbele[4]=P[Pl][i];
                      }
                      if(i==3){
                          orbele[6]=P[Pl][i];
                      }

                  }
              }

                  cout<<endl;
                  cout<<setprecision(12)<<"Simplex Mean: "<<ym<<" Simplex STD: "<<ys<<endl;
                  cout<<endl;
                  cout<<"Orbitelements after "<<zaehler<<" Iterations and "<<eval<<" Evaluations:"<<endl;
                  cout<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;

                  LogFile<<endl;
                  LogFile<<"Optimisation ends after "<<zaehler<<" Iterations and "<<eval<<" Evaluations:"<<endl;;
                  LogFile<<"Mean: "<<ym<<" and STD "<<ys<<endl;
                  LogFile<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;

                      for(int i = 0; i<nu;i++){
                          cout<<orbele[i]<<";\t";
                          LogFile<<orbele[i]<<";\t";
                      }

                cout<<endl;
                LogFile<<endl;

          }


    system_clock::time_point time2 = system_clock::now();

    std::time_t tt1, tt2;

      tt1 = system_clock::to_time_t ( time1 );

      tt2 = system_clock::to_time_t ( time2 );

    double CPUtime = std::difftime(tt2, tt1);
    QString CPtime = QString::number(CPUtime/60);
    ui->lineEdit_6->setText(CPtime);
    QString residual=QString::number(r1);
    ui->lineEdit_7->setText(residual);

    ui->checkBox_9->setChecked(false);
    ui->checkBox_11->setChecked(true);
    ui->checkBox_12->setChecked(false);

    r1=1;

    this->setCursor(QCursor(Qt::ArrowCursor));
}


//****************************************
// Matrix construction for DSM
//****************************************
void MainWindow::ConstructMatrix(){

    this->setCursor(QCursor(Qt::WaitCursor));

    int a=0;

    // matrix for SB2 system
    if(ui->checkBox_4->isChecked()){

    //looking for maximum RV absolute
    RV1max=abs(RV1[0]);
    RV3max=abs(RV3[0]);
    for (int i=0; i<num; i++){
        if (abs(RV1[i])>abs(RV1max)){
            RV1max=RV1[i];
        }
        if (abs(RV3[i])>abs(RV3max)){
            RV3max=RV3[i];
        }
    }

    //looking for minimum and maximum RV
    RV1min=RV1max;
    RV3min=RV3max;
    RV1maxi=RV1[0];
    RV3maxi=RV3[0];
    for(int i=0; i<num; i++){
        if(RV1[i]<RV1min){
            RV1min=RV1[i];
        }
        if(RV3[i]<RV3min){
            RV3min=RV3[i];
        }
        if(RV1[i]>RV1maxi){
            RV1maxi=RV1[i];
        }
        if(RV3[i]>RV3maxi){
            RV3maxi=RV3[i];
        }
    }

    RV1amin=RV1[0];

    for(int i=0; i<num; i++){
        if(abs(RV1[i])<RV1amin){
            RV1amin=RV1[i];
        }
    }

    ui->doubleSpinBox->setValue(RV1max);
    ui->doubleSpinBox_2->setValue(RV3max);

    dv=(pow(10,(W(5)-W(4)))-1)*c0;
    difference=W(5)-W(4);
    ui->doubleSpinBox_3->setValue(dv);

    elements=0;

    double RV1b[num], RV3b[num];
    int RV1c[num], RV3c[num], RV1cmin, RV1cmax, RV2cmin, RV2cmax;

    for(int j=0; j<num; j++){
        RV1a=RV1[j]/dv;
        RV3a=RV3[j]/dv;
        RV1b[j]=abs(RV1[j])/dv-abs(RV1a);
        RV3b[j]=abs(RV3[j])/dv-abs(RV3a);

        if(RV1b[j]>=0.5){
            if(RV1[j]>=0){
                RV1c[j]=RV1[j]/dv+1;
            }
            if(RV1[j]<0){
                RV1c[j]=RV1[j]/dv-1;
            }
        }
        else{
            RV1c[j]=RV1[j]/dv;
        }

        if(RV3b[j]>=0.5){
            if(RV3[j]>=0){
                RV3c[j]=RV3[j]/dv+1;
            }
            if(RV3[j]<0){
                RV3c[j]=RV3[j]/dv-1;
            }
        }
        else{
             RV3c[j]=RV3[j]/dv;
            }
        if(j==0){
            RV1cmin=RV1c[j];
            RV1cmax=RV1c[j];
            RV2cmin=RV3c[j];
            RV2cmax=RV3c[j];
            RV1m=abs(RV1c[j]);
            RV3m=abs(RV3c[j]);
        }
        else{
            if(RV1c[j]<RV1cmin){
                RV1cmin=RV1c[j];
            }
            if(RV1c[j]>RV1cmax){
                RV1cmax=RV1c[j];
            }
            if(RV3c[j]<RV2cmin){
                RV2cmin=RV3c[j];
            }
            if(RV3c[j]>RV2cmax){
                RV2cmax=RV3c[j];
            }
            if(abs(RV1c[j])>RV1m){
                RV1m = abs(RV1c[j]);
            }
            if(abs(RV3c[j])>RV3m){
                RV3m = abs(RV3c[j]);
            }
        }
    }

    RV1a=RV1c[0];
    RV3a=RV3c[0];

    if(RV1cmin<0){
            if(RV1cmax<0){
            bso1=logbin+abs(RV1cmin);
            }
            else{
                bso1=logbin+abs(RV1cmin)+RV1cmax;
            }
        }
        else{
            bso1=logbin+RV1cmax;
        }
        if(RV2cmin<0){
            if(RV2cmax<0){
            bso2=logbin+abs(RV2cmin);
            }
            else{
                bso2=logbin+abs(RV2cmin)+RV2cmax;
            }
        }
        else{
            bso2=logbin+RV2cmax;
        }

    cout<<endl;
    cout<<"bso1 "<<bso1<<endl;
    cout<<"bso2 "<<bso2<<endl;

    Mn=num*logbin;

    if(ui->checkBox_8->isChecked()){
        Mm=bso1+bso2+logbin;    //telluric +logbin
    }

    else{
        Mm=bso1+bso2;
    }

    M.resize(Mn,Mm);
    Mt.resize(Mm,Mn);
    X.resize(Mm);
    cout<<Mm<<" "<<Mn<<endl;


    //constructing initial SB2 transformation matrix with telluric lines
    if(ui->checkBox_8->isChecked()){
        for (int n=0; n<num*logbin; n++){//num*logbin
            if(n==0){
                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                    RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                    RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }
            if(n==logbin+a*logbin){
                a+=1;
                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                    RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }
            for (int m=0; m<Mm; m++){
                if((n-a*logbin+RV1a)==m or (n+bso1-1-a*logbin+RV3a==m) or (m==n-a*logbin+bso1+bso2)){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                    if(bidi==1){
                        if(m==n-a*logbin+bso1+bso2){
                            M(n,m)=Mtel[a];
                        }
                        if((n+bso1-1-a*logbin+RV3a==m) & (m>=bso1)){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        if(m<Mm-1) {
                            if(m==n-a*logbin+bso1+bso2){
                                M(n,m+1)=Mtel[a];
                            }
                        if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m+1)=Mval2[a];
                        }
                        else M(n,m+1)=Mval1[a];
                        m=m+1;
                        ++elements;
                        }
                        ++elements;
                    }
                    if(bidi==0){
                        if(m==n-a*logbin+bso1+bso2){
                            M(n,m)=Mtel[a];
                        }
                        if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        ++elements;
                    }
                }
                else{
                    M(n,m)=0;
                }
            }
        }
    }

    else{   //without telluric lines
        for (int n=0; n<num*logbin; n++){   //num*logbin

            if(n==0){
                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                        RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                    RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }
            if(n==logbin+a*logbin){
                a+=1;

                if(RV1cmin>0){
                    RV1a=RV1c[a];
                }
                else{
                        RV1a=abs(RV1cmin)+RV1c[a];
                }

                if(RV2cmin>0){
                    RV3a=RV3c[a];
                }
                else{
                    RV3a=abs(RV2cmin)+RV3c[a];
                }
            }

            for (int m=0; m<Mm; m++){
                if((n-a*logbin+RV1a)==m or n+bso1-1-a*logbin+RV3a==m){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                    if(bidi==1){
                        if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        if(m<Mm-1) {
                            if((n+bso1-1-a*logbin+RV3a==m) & (m>=bso1)){
                                M(n,m+1)=Mval2[a];
                            }
                            else M(n,m+1)=Mval1[a];
                            m=m+1;
                            ++elements;
                        }
                        ++elements;
                    }
                    if(bidi==0){
                    if(n+bso1-1-a*logbin+RV3a==m){
                        M(n,m)=Mval2[a];
                    }
                    else M(n,m)=Mval1[a];
                    ++elements;
                    }
                }
                else{
                    M(n,m)=0;
                }
             }
          }
       }
    }

    //construct matrix for SB1 system
    if(ui->checkBox_3->isChecked()){

        //looking for maximum RV absolute
        RV1max=abs(RV1[0]);

        for (int i=0; i<num; i++){
            if (abs(RV1[i])>abs(RV1max)){
                RV1max=RV1[i];
            }
        }

        //looking for minimum and maximum RV
        RV1min=RV1max;

        RV1maxi=RV1[0];

        for(int i=0; i<num; i++){
            if(RV1[i]<RV1min){
                RV1min=RV1[i];
            }

            if(RV1[i]>RV1maxi){
                RV1maxi=RV1[i];
            }

        }

        RV1amin=RV1[0];
        RV1min=RV1[0];

        for(int i=0; i<num; i++){
            if(abs(RV1[i])<RV1amin){
            RV1amin=RV1[i];
            }
            if(RV1[i]<RV1min){
                RV1min=RV1[i];
            }
        }

        ui->doubleSpinBox->setValue(RV1max);;

    dv=(pow(10,(W(5)-W(4)))-1)*c0;
    difference=W(5)-W(4);
    ui->doubleSpinBox_3->setValue(dv);

    elements=0;
    int a=0;

    double RV1b[num];
    int RV1c[num], RV1cmin, RV1cmax;

    for(int j=0; j<num; j++){
        RV1a=RV1[j]/dv;
        RV1b[j]=abs(RV1[j])/dv-abs(RV1a);

        if(RV1b[j]>=0.5){
            if(RV1[j]>=0){
                RV1c[j]=RV1[j]/dv+1;
            }
            if(RV1[j]<0){
                RV1c[j]=RV1[j]/dv-1;
            }
        }
        else{
            RV1c[j]=RV1[j]/dv;
        }
        if(j==0){
            RV1cmin = RV1c[j];
            RV1cmax = RV1c[j];
            RV1m=abs(RV1c[j]);
        }
        else{
            if(RV1c[j]<RV1cmin){
                RV1cmin=RV1c[j];
            }
            if(RV1c[j]>RV1cmax){
                RV1cmax=RV1c[j];
            }
            if(abs(RV1c[j])>RV1m){
                RV1m = abs(RV1c[j]);
            }
        }

    }

    if(RV1cmin<0){
            if(RV1cmax<0){
            bso1=logbin+abs(RV1cmin);
            }
            else{
                bso1=logbin+abs(RV1cmin)+RV1cmax;
            }
        }
        else{
            bso1=logbin+RV1cmax;
        }

    cout<<"bso1 "<<bso1<<endl;

    Mn=num*logbin;

    if(ui->checkBox_8->isChecked()){
    Mm=bso1+logbin;    //telluric +logbin
    }
    else{
        Mm=bso1;
    }
    M.resize(Mn,Mm);
    Mt.resize(Mm,Mn);
    X.resize(Mm);
    cout<<Mm<<" "<<Mn<<endl;

    Mval1.resize(num);
    Mval2.resize(num);
    Mtel.resize(num);

    RV1a=RV1c[0];

        //constructing initial transformation matrix with tellric lines
        if(ui->checkBox_8->isChecked()){
            for (int n=0; n<num*logbin; n++){//num*logbin
                if(n==0){
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }

                if(n==logbin+a*logbin){
                    a+=1;
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                        RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }

                for (int m=0; m<Mm; m++){
                    if((n-a*logbin+RV1a)==m or m==n-a*logbin+bso1){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                        if(bidi==1){
                            if(m==n-a*logbin+bso1){
                                M(n,m)=Mtel[a];
                            }
                            else M(n,m)=Mval1[a];
                            if(m<Mm-1) {
                                if(m==n-a*logbin+bso1){
                                    M(n,m+1)=Mtel[a];
                                }

                                else M(n,m+1)=Mval1[a];
                                m=m+1;
                                ++elements;
                            }
                            ++elements;
                        }
                        if(bidi==0){
                            if(m==n-a*logbin+bso1){
                                M(n,m)=Mtel[a];
                            }
                            else M(n,m)=Mval1[a];
                            ++elements;
                        }
                    }
                    else{
                        M(n,m)=0;
                    }
                }
            }
        }

        //without telluric lines
        else{
            for (int n=0; n<num*logbin; n++){//num*logbin
                if(n==0){
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }
                if(n==logbin+a*logbin){
                    a+=1;
                    if(RV1cmin>0){
                        RV1a=RV1c[a];
                    }
                    else{
                            RV1a=abs(RV1cmin)+RV1c[a];
                    }
                }
                for (int m=0; m<Mm; m++){
                    if((n-a*logbin+RV1a)==m){//or n+logbin+2*abs(RV1m)-a*logbin+RV3a+abs(RV3m)==m
                        if(bidi==1){
                            M(n,m)=Mval1[a];
                            if(m<Mm-1) {
                                M(n,m+1)=Mval1[a];
                                m=m+1;
                                ++elements;
                            }
                            ++elements;
                        }
                        if(bidi==0){
                            M(n,m)=Mval1[a];
                            ++elements;
                            }
                        }
                        else{
                            M(n,m)=0;
                        }
                    }
                }
            }
        }

    a=0;

    loc.resize(2,elements);
    val.resize(elements);
    int elcount=0;

    for(int i=0; i<elements; i++){
        val(i)=Mval1[a];
    }

    for(int n=0; n<Mn;n++){
        for(int m=0; m<Mm;m++){
            if(M(n,m)>0){
                loc(0,elcount)=n;
                loc(1,elcount)=m;
                ++elcount;
            }
         }
      }
    }

//*******************************************
//ECON for optimization
//*******************************************
double MainWindow::DivideConquer(){

    residu=0;

    mat U, V;
    vec w;

    if(Mm>Mn){
        Mm=Mn;
    }

    vec res(Mm);

    int cBS;

    if(ui->checkBox_5->isChecked()){
        svd_econ(U,w,V,M, "both", "std");
    }

    else ++cBS;

    if(ui->checkBox_6->isChecked()){
        svd_econ(U,w,V,M, "both", "dc");
    }

    else ++cBS;

    if(cBS == 0){
        QMessageBox::information(this, "Error", "Select method for ECON.");
        abortt=1;
        return 0;
    }

    int i,j,jj;
        double s;
        double thresh =-1;
        double eps = numeric_limits<double>::epsilon();
        vec tmp(Mm);
        double tsh = (thresh >= 0. ? thresh : 0.5*sqrt(Mm+Mn+1.)*w(0)*eps);
        cout <<"tsh "<<tsh<<endl;
        for (j=0;j<Mm;j++) {
            s=0.0;
            if (w(j) > tsh) {
                for (i=0;i<Mn;i++) s += U(i,j)*C(i);
                s /= w(j);
            }
            tmp(j)=s;
        }

        int index1=0;
        int index2=0;
        int index3=0;

        for (j=0;j<Mm;j++) {
            s=0.0;
            if(w(j)>tsh){
                for (jj=0;jj<Mm;jj++) s += V(j,jj)*tmp(jj);
                X(j)=s;
                }
            }
        
        res=M*X;

        int resbins=0;

        for(int i=0; i<Mn; i++){

            if(res(i)>10*tsh){

                if(ui->checkBox_33->isChecked()){
                    if(ui->checkBox_14->isChecked()){
                        if((W(i)>log10(ui->doubleSpinBox_14->value())) & (W(i)<log10(ui->doubleSpinBox_15->value()))){
                            ++resbins;
                            residu+=pow((res(i)-C(i)),2);
                        }
                    }

                        else{
                            if((pow((res(i)-C(i)),2)>residu) & (pow((res(i)-C(i)),2)<1) & (W(i)>log10(ui->doubleSpinBox_14->value())) & (W(i)<log10(ui->doubleSpinBox_15->value()))){
                                residu=pow((res(i)-C(i)),2);
                            }
                        }
                }

                else{
            if(ui->checkBox_14->isChecked()){
                    ++resbins;
                    residu+=pow((res(i)-C(i)),2);
            }

                else{
                    if((pow((res(i)-C(i)),2)>residu) & (pow((res(i)-C(i)),2)<1)){
                        residu=pow((res(i)-C(i)),2);
                    }
                }

                }
            }
        }

        if(ui->checkBox_14->isChecked()){
        residu=sqrt(residu);
        }

        cout<<"Residuum: "<<residu<<endl;
        cout<<"  # Bins: "<<resbins<<endl;
        cout<<"  r/Bins: "<<residu/resbins<<endl;

        // better value found, write data to files.
        if(residu<r1){

            r1=residu;
            upda=1;

            for(int i=0; i<num; i++){
                QString index= QString::number(i);
                if(ui->checkBox_3->isChecked()){
                    QString velocity=QString::number(RV1[i]);
                    ui->plainTextEdit->appendPlainText(index+" "+velocity);
                }
                if(ui->checkBox_4->isChecked()){
                    QString velocity=QString::number(RV1[i]);
                    QString sum=QString::number(RV3[i]);
                    ui->plainTextEdit->appendPlainText(index+" "+velocity+" "+sum);
                }
             }

            if(ui->checkBox_16->isChecked()){
            cout<<"New Parameters found:"<<endl;
            //Logfile<<"New Parameters found:"<<endl;
            cout<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;
            //Logfile<<"Period; Eccentricity; Amplitude A; Amplitude B; Systemic V.; T_peri; Omega A;"<<endl;
                for(int i=0; i<7;i++){
                    cout<<setprecision(14)<<orbele[i]<<" ";
                    //Logfile<<setprecision(14)<<orbele[i]<<" ";
                }
                cout<<endl;
                //Logfile<<endl;

            ui->plainTextEdit_2->appendPlainText("New Parameters found:\nPeriod; Eccentricity; Amplitude A; Amplitude B; Systemic V; T_peri; Omega A;");
            QString Peri = QString::number(orbele[0]);
            QString Ecce = QString::number(orbele[1]);
            QString AA = QString::number(orbele[2]);
            QString AB = QString::number(orbele[3]);
            QString SV = QString::number(orbele[4]);
            QString Tper = QString::number(orbele[5]);
            QString wper = QString::number(orbele[6]);
            ui->plainTextEdit_2->appendPlainText(Peri+" "+Ecce+" "+AA+" "+AB+" "+SV+" "+Tper+" "+wper);
            }
            else{
                cout<<"Found better RVs."<<endl;
                //Logfile<<"Found better RVs."<<endl;
                cout<<"Residuum: "<<r1<<endl;
                //Logfile<<"Residuum: "<<r1<<endl;
            }

            ui->spinBox_6->setValue(eval);

            QString residual=QString::number(residu);
            ui->lineEdit_7->setText(residual);

            QString output=ui->lineEdit_5->text();
            string out = output.toUtf8().constData();
            std::ostringstream file1NameStream(out);
            file1NameStream<<path<<"/"<<out;
            std::string file1Name = file1NameStream.str();
            ofstream file1(file1Name.c_str());

            QString output2=ui->lineEdit_8->text();
            string out2 = output2.toUtf8().constData();
            std::ostringstream file2NameStream(out2);
            file2NameStream<<path<<"/"<<out2;
            std::string file2Name = file2NameStream.str();
            ofstream file2(file2Name.c_str());

            QString tel=ui->lineEdit_10->text();
            string telout = tel.toUtf8().constData();
            std::ostringstream telNameStream(telout);
            telNameStream<<path<<"/"<<telout;
            std::string telName = telNameStream.str();
            ofstream tell(telName.c_str());

            QString output3=ui->lineEdit_13->text();
            string out3 = output3.toUtf8().constData();
            std::ostringstream file3NameStream(out3);
            file3NameStream<<path<<"/"<<out3;
            std::string file3Name = file3NameStream.str();
            ofstream file5(file3Name.c_str());

            std::ostringstream file5NameStream("compl.txt");
            file5NameStream<<path<<"/compl.txt";
            std::string file5Name = file5NameStream.str();
            ofstream file7(file5Name.c_str());

            QString QError=ui->lineEdit_20->text();
            string errout = QError.toUtf8().constData();
            std::ostringstream file6NameStream(errout);
            file6NameStream<<path<<"/"<<errout;
            std::string file6Name = file6NameStream.str();
            ofstream file8(file6Name.c_str());

            QVector<double> err(logbin), errmean(logbin);

            for(int i =0; i<logbin;i++){
                errmean[i]=0;
                for(int n = 0; n<num; n++){
                    errmean[i]+=(res(i+n*logbin)-C(i+n*logbin))/num;

                }
                file8<<pow(10,W(i))<<"\t"<<errmean[i]<<endl;
            }


            for (j=0;j<Mm;j++) {
                 if(abs(X(j))>10*tsh){
                    if(j<bso1){
                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(fluxratio/(fluxratio+1)-0.5);
                        //}

                        file1<<pow(10,(W(0)+(index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index1;
                    }
                    if((j>=bso1)&(j<bso1+bso2)){
                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(1.0/(fluxratio+1)-0.5);
                        //}
                        file2<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index2;
                    }
                    if(j>=bso1+bso2){
                        tell<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
            }

        for(int n=0; n<X.size(); n++){
            file7<<n<<" "<<X(n)<<endl;
        }
        index1=0;
        index2=0;
        index3=0;

        for(int i=0; i<res.size(); i++){
                file5<<pow(10,W(i))<<" "<<(res(i)-C(i))<<endl;
        }

        // Save all Updates
        if(ui->checkBox_19->isChecked()){

            string erro = "error_";
            std::ostringstream upd1NameStream(erro);
            upd1NameStream<<path<<"/"<<erro<<updac<<".dat";
            std::string upd1Name = upd1NameStream.str();
            ofstream upda1(upd1Name.c_str());

            string sCA = "compa_";
            std::ostringstream upd2NameStream(sCA);
            upd2NameStream<<path<<"/"<<sCA<<updac<<".dat";
            std::string upd2Name = upd2NameStream.str();
            ofstream upda2(upd2Name.c_str());

            string sCB = "compb_";
            std::ostringstream upd3NameStream(sCB);
            upd3NameStream<<path<<"/"<<sCB<<updac<<".dat";
            std::string upd3Name = upd3NameStream.str();
            ofstream upda3(upd3Name.c_str());

            string sTel = "tellur_";
            std::ostringstream upd5NameStream(sTel);
            upd5NameStream<<path<<"/"<<sTel<<updac<<".dat";
            std::string upd5Name = upd5NameStream.str();
            ofstream upda5(upd5Name.c_str());

            string sDif = "differences_";
            std::ostringstream upd4NameStream(sDif);
            upd4NameStream<<path<<"/"<<sDif<<updac<<".dat";
            std::string upd4Name = upd4NameStream.str();
            ofstream upda4(upd4Name.c_str());

            for(int i =0; i<logbin-1;i++){
                upda1<<pow(10,W(i))<<"\t"<<errmean[i]<<endl;
                errmean[i]=0;
            }

            for (j=0;j<Mm;j++) {
                 if(abs(X(j))>10*tsh){
                    if(j<bso1){

                        upda2<<pow(10,(W(0)+(index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index1;
                    }
                    if((j>=bso1)&(j<bso1+bso2)){
                        upda3<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index2;
                    }
                    if(j>=bso1+bso2){
                        upda5<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
            }

            index1=0;
            index2=0;
            index3=0;

            for(int i=0; i<res.size(); i++){
                    upda4<<pow(10,W(i))<<" "<<(res(i)-C(i))<<endl;
            }

        }

        if(ui->checkBox_10->isChecked()){
            MainWindow::on_pushButton_6_clicked();
        }

        if(ui->checkBox_19->isChecked()){

            QString qEval = QString::number(updac);
                QString sav1="Sol_"+qEval;
                QString save1=qPath+"/"+sav1+".png";
                ui->customPlot->savePng(save1);

                QString sav2="Diff_"+qEval;
                QString save2=qPath+"/"+sav2+".png";
                ui->customPlot_3->savePng(save2);
        }

                    ++updac;

        if(ui->checkBox_7->isChecked()){
            std::ostringstream file1NameStream("rcomp1.txt");
            file1NameStream<<path<<"/rcomp1.txt";
            std::string file1Name = file1NameStream.str();
            ofstream file3(file1Name.c_str());

            std::ostringstream file2NameStream("rcomp2.txt");
            file2NameStream<<path<<"/rcomp2.txt";
            std::string file2Name = file2NameStream.str();
            ofstream file4(file2Name.c_str());

            QVector<double> Xr(Mm);

            for(int i=0; i<Mm-1; i++){
                Xr[index3]=(X(i)+X(i+1))/2;
                if(i<bso1){
                    file3<<pow(10,(W(0)+(2*index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<Xr[index3]<<endl;
                    ++index1;
                }
                if((i>=bso1) & (i<bso1+bso2)){
                    file4<<pow(10,(W(0)+(2*index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<Xr[index3]<<endl;
                    ++index2;
                }
                ++i;
                ++index3;
                }
            }
        }
        qApp->processEvents(QEventLoop::AllEvents);

        return residu;
    }

//*******************************************************
//plot results in main window
//*******************************************************
void MainWindow::on_pushButton_6_clicked()
{
    ui->customPlot->clearGraphs();
    ui->customPlot_2->clearGraphs();
    ui->customPlot_3->clearGraphs();

    double xs1, xs2, ys1, ys2;
    string zeile, one, two;
    QString plot1=ui->lineEdit_5->text();
    string plot11 = plot1.toUtf8().constData();
    std::ostringstream datNameStream(plot11);
    datNameStream<<path<<"/"<<plot11;
    std::string datName = datNameStream.str();
    ifstream toplot1(datName.c_str());

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qPath+"/"+plot1+" does not exist!");
       return;
    }

    QString plot2=ui->lineEdit_8->text();
    string plot12 = plot2.toUtf8().constData();
    std::ostringstream dat2NameStream(plot12);
    dat2NameStream<<path<<"/"<<plot12;
    std::string dat2Name = dat2NameStream.str();
    ifstream toplot2(dat2Name.c_str());

    QFile checkfile2(dat2Name.c_str());

    if(!checkfile2.exists()){
        qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qPath+"/"+plot2+" does not exist!");
       return;
    }

    QString qError=ui->lineEdit_20->text();
    string errorf=qError.toUtf8().constData();
    std::ostringstream dat4NameStream(errorf);
    dat4NameStream<<path<<"/"<<errorf;
    std::string dat4Name = dat4NameStream.str();
    ifstream errors(dat4Name.c_str());

    QFile checkfile4(dat4Name.c_str());

    if(!checkfile4.exists()){
        qDebug()<<"The file "<<checkfile4.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qPath+"/"+qError+" does not exist!");
       return;
    }

    int number_of_lines=0;

    while(std::getline(toplot1, zeile))
       ++ number_of_lines;

    toplot1.clear();
    toplot1.seekg(0, ios::beg);

    QVector<double> a(number_of_lines), b(number_of_lines);

    for (int i=0; i<number_of_lines; i++){
        toplot1 >> one >>two;
        istringstream ist(one);
        ist >> a[i];
        istringstream ist2(two);
        ist2 >> b[i];
    }
    toplot1.close();

    xs1=a[0];
    xs2=a[number_of_lines-1];
    for(int i=0; i<number_of_lines; i++){
        if(a[i]<xs1){
            xs1=a[i];
        }
    }

    for(int i=0; i<number_of_lines; i++){
        if(a[i]>xs2){
            xs2=a[i];
        }
    }


    ys1=b[0];
    ys2=b[number_of_lines-1];
    for(int i=0; i<number_of_lines; i++){
        if(b[i]<ys1){
            ys1=b[i];
        }
    }
    for(int i=0; i<number_of_lines; i++){
        if(b[i]>ys2){
            ys2=b[i];
        }
    }


    number_of_lines=0;

    while(std::getline(toplot2, zeile))
       ++ number_of_lines;

    toplot2.clear();
    toplot2.seekg(0, ios::beg);

    QVector<double> c(number_of_lines), d(number_of_lines);

    for (int i=0; i<number_of_lines; i++){
        toplot2 >> one >>two;
        istringstream ist(one);
        ist >> c[i];
        istringstream ist2(two);
        ist2 >> d[i];
    }
    toplot2.close();

    for(int i=0; i<number_of_lines; i++){
        if(c[i]<xs1){
            xs1=c[i];
        }
    }

    for(int i=0; i<number_of_lines; i++){
        if(c[i]>xs2){
            xs2=c[i];
        }
    }

    for(int i=0; i<number_of_lines; i++){
        if(d[i]<ys1){
            ys1=d[i];
        }
    }
    for(int i=0; i<number_of_lines; i++){
        if(d[i]>ys2){
            ys2=d[i];
        }
    }

    number_of_lines=0;

    while(std::getline(errors, zeile))
       ++ number_of_lines;

    errors.clear();
    errors.seekg(0, ios::beg);

    QVector<double> e(number_of_lines), f(number_of_lines);

    for (int i=0; i<number_of_lines; i++){
        errors >> one >>two;
        istringstream ist(one);
        ist >> e[i];
        istringstream ist2(two);
        ist2 >> f[i];
    }
    errors.close();

     ui->customPlot->addGraph();
     ui->customPlot->graph(0)->setData(a, b);
     ui->customPlot->graph(0)->rescaleAxes();
     ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
     ui->customPlot->addGraph();
     ui->customPlot->graph(1)->setData(c, d);
     ui->customPlot->graph(1)->rescaleAxes(true);
     ui->customPlot->graph(1)->setPen(QPen(Qt::red));
     ui->customPlot->addGraph();
     ui->customPlot->graph(2)->setData(e, f);
     ui->customPlot->yAxis->rescale(true);
     ui->customPlot->graph(2)->setPen(QPen(Qt::black));
     ui->customPlot->replot();

     //Differences
     QString plot3=ui->lineEdit_13->text();
     string plot13 = plot3.toUtf8().constData();
     std::ostringstream dat3NameStream(plot13);
     dat3NameStream<<path<<"/"<<plot13;
     std::string dat3Name = dat3NameStream.str();
     ifstream toplot3(dat3Name.c_str());

     QFile checkfile3(datName.c_str());

     if(!checkfile3.exists()){
         qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
         QMessageBox::information(this, "Error", "File "+qPath+"/"+plot3+" does not exist!");
        return;
     }


     number_of_lines=0;

     while(std::getline(toplot3, zeile))
        ++ number_of_lines;

     toplot3.clear();
     toplot3.seekg(0, ios::beg);

     double offset = ui->doubleSpinBox_5->value();
     int index = 0;
     double sres=0, sres2=0;

     a.resize(number_of_lines);
     b.resize(number_of_lines);

     for (int i=0; i<number_of_lines; i++){
        toplot3 >> one >>two;
        istringstream ist(one);
        ist >> a[i];
        istringstream ist2(two);
        ist2 >> b[i];
        if((i>0) & (a[i]<a[i-1])){
            cout<<sres<<"\t"<<sres2<<endl;
            sres = 0;
            sres2 = 0;
            ++index;
        }
        if(ui->checkBox_13->isChecked()){
            if(a[i]>ui->doubleSpinBox_14->value() & (a[i]<ui->doubleSpinBox_15->value())){
                sres+=b[i];
                sres2+=pow(b[i],2);
            }
            else{
                // do nothing...
            }
        }
        else{
            sres+=b[i];
            sres2+=pow(b[i],2);
        }
        b[i]=b[i]+index*offset;
     }

     toplot3.close();

      ui->customPlot_3->addGraph();
      ui->customPlot_3->graph(0)->setData(a, b);
      ui->customPlot_3->graph(0)->setPen(QPen(Qt::red));
      ui->customPlot_3->graph(0)->setLineStyle(QCPGraph::lsNone);
      ui->customPlot_3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
      ui->customPlot_3->xAxis->setRange(xs1, xs2);
      ui->customPlot_3->yAxis->setRange(-0.5, 0.5+index*offset);
      ui->customPlot_3->replot();

      //telluric lines
      if(ui->checkBox_8->isChecked()){

          QString plot5=ui->lineEdit_10->text();
          string plot15 = plot5.toUtf8().constData();
          std::ostringstream dat5NameStream(plot15);
          dat5NameStream<<path<<"/"<<plot15;
          std::string dat5Name = dat5NameStream.str();
          ifstream toplot5(dat5Name.c_str());

          QFile checkfile5(dat5Name.c_str());

          if(!checkfile5.exists()){
              qDebug()<<"The file "<<checkfile5.fileName()<<" does not exist.";
              QMessageBox::information(this, "Error", "File "+qPath+"/"+plot5+" does not exist!");
             return;
          }

          number_of_lines=0;

          while(std::getline(toplot5, zeile))
             ++ number_of_lines;

          toplot5.clear();
          toplot5.seekg(0, ios::beg);

          a.resize(number_of_lines);
          b.resize(number_of_lines);

          for (int i=0; i<number_of_lines; i++){
            toplot5 >> one >>two;
            istringstream ist(one);
            ist >> a[i];
            istringstream ist2(two);
            ist2 >> b[i];
          }
          toplot5.close();

          ui->customPlot_2->addGraph();
          ui->customPlot_2->graph(0)->setData(a, b);
          ui->customPlot_2->graph(0)->setPen(QPen(Qt::red));
          ui->customPlot_2->addGraph();
          ui->customPlot_2->xAxis->setRange(xs1, xs2);
          ui->customPlot_2->yAxis->rescale(true);
          ui->customPlot_2->replot();

      }

}

void MainWindow::on_spinBox_3_valueChanged()
{
    zaehler=ui->spinBox_3->value();
}

void MainWindow::on_checkBox_9_clicked()
{
    if(ui->checkBox_9->isChecked()){
        ui->checkBox_11->setChecked(false);
        ui->checkBox_12->setChecked(false);
    }
    else{
        ui->checkBox_11->setChecked(true);
        ui->checkBox_12->setChecked(false);
    }
}

void MainWindow::on_checkBox_11_clicked()
{
    if(ui->checkBox_11->isChecked()){
        ui->checkBox_9->setChecked(false);
        ui->checkBox_12->setChecked(false);
    }
    else{
        ui->checkBox_9->setChecked(true);
        ui->checkBox_12->setChecked(false);
    }
}

void MainWindow::on_doubleSpinBox_4_valueChanged()
{
    fluxratio=ui->doubleSpinBox_4->value();
}

void MainWindow::on_pushButton_7_clicked()
{
    abortt=1;
}

void MainWindow::on_checkBox_12_clicked()
{
    if(ui->checkBox_12->isChecked()){
        ui->checkBox_11->setChecked(false);
        ui->checkBox_9->setChecked(false);
    }
    else{
        ui->checkBox_11->setChecked(true);
        ui->checkBox_9->setChecked(false);
    }
}

void MainWindow::on_checkBox_4_clicked()
{
    ui->checkBox_3->setChecked(false);
    ui->checkBox_34->setChecked(false);
    ui->checkBox_8->setEnabled(true);
}

void MainWindow::on_checkBox_3_clicked()
{
    ui->checkBox_4->setChecked(false);
    ui->checkBox_34->setChecked(false);
    ui->checkBox_8->setEnabled(true);
}

void MainWindow::on_checkBox_14_clicked()
{
    ui->checkBox_15->setChecked(false);
}

void MainWindow::on_checkBox_15_clicked()
{
    ui->checkBox_14->setChecked(false);
}

void MainWindow::on_actionEditor_triggered()
{
    int hgh = ui->spinBox_2->value() - ui->spinBox->value() + 1;
    qEdit =new Edit(this);
    qEdit->seData(ui->lineEdit_4->text(), hgh);
    qEdit->show();
}


void MainWindow::on_checkBox_16_clicked()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
}

double Ofunction (double XX[], double RVt, double RVT0, double RVP, double RVe){
    double func;

    func= abs(RVe*sin(XX[0])+2*M_PI*(RVt-RVT0)/RVP-XX[0]);

    return func;

    }

// RVs for A
void MainWindow::VAmplitudeA()
{
    double E, theta;

    RVT0 = orbele[5];
    RVP = orbele[0];
    RVe = orbele[1];


    for(int m=0; m<num; m++){
        if(orbele[1]!=0){

            RVt = otimes[m];
            MainWindow::findroot();
            E=RVE;
            theta=2*(atan(tan(E/2)*sqrt((1+orbele[1])/(1-orbele[1]))));
        }
        else{
            E = M_PI*(otimes[m]-orbele[5])/orbele[0];
            theta=2*(atan(tan(E/2)));
        }

    RV1[m] = orbele[4] + orbele[2]*(cos(theta+orbele[6])+orbele[1]*cos(orbele[6]));

    }
}

// RVs for B
void MainWindow::VAmplitudeB()
{
    double E, theta;

    RVT0 = orbele[5];
    RVP = orbele[0];
    RVe = orbele[1];

    for(int m=0; m<num; m++){

        if(orbele[1]!=0){
            RVt = otimes[m];
            MainWindow::findroot();
            E=RVE;
            theta=2*(atan(tan(E/2)*sqrt((1+orbele[1])/(1-orbele[1]))));
        }
        else{
            E = M_PI*(otimes[m]-orbele[5])/orbele[0];
            theta=2*(atan(tan(E/2)));
        }

    RV3[m] = orbele[4] + orbele[3]*(cos(theta+(orbele[6]+M_PI))+orbele[1]*cos(orbele[6]+M_PI));

    }
}

void MainWindow::on_actionArithmetic_triggered()
{
    qArith =new Arithmetic(this);
    qArith->seData(ui->lineEdit_4->text());
    qArith->show();
}

void MainWindow::on_actionManual_triggered()
{
    QDesktopServices::openUrl(QUrl("Manual.pdf"));
}

//*******************************************************
// Estimate error
//*******************************************************
void MainWindow::ErrorEstimation()
{

    int epara;
    double bestr;
    string zeile, one;

    QString qOptval = ui->lineEdit_16->text();
    string file1opt = qOptval.toUtf8().constData();
    std::ostringstream file1NameStream(file1opt);
    file1NameStream<<path<<"/"<<file1opt;
    std::string file1Name = file1NameStream.str();
    ifstream initiate1(file1Name.c_str());

    QFile checkfile5(file1Name.c_str());

    if(!checkfile5.exists()){
        qDebug()<<"The file "<<checkfile5.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qPath+"/"+qOptval+" does not exist!");
       return;
    }

    epara=0;

    while(std::getline(initiate1, zeile))
       ++ epara;

    initiate1.clear();
    initiate1.seekg(0, ios::beg);

    QVector<double> bestrB(epara), VariP(7);

    for (int i=0; i<epara; i++){
    initiate1 >> one;
    istringstream ist(one);
    ist >> bestrB[i];
    }
    initiate1.close();

    bestr = bestrB[0];

    for(int i =0; i<epara; i++){
        if(bestrB[i]<bestr){
            bestr = bestrB[i];
        }
    }

    cout<<"Best Value: "<<bestr<<endl;

    ifstream initorbit("orbitelements.dat");

    for(int i=0; i<7; i++){
        initorbit >> eins;
        istringstream istr1(eins);
        istr1 >> orbele[i];
    }
    initorbit.close();


    mat Errors(7,7);
    QVector<double> STDError(7);

    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        return;
    }


        QFile checkfile1(file1Name.c_str());

        if(!checkfile1.exists()){
            qDebug()<<"The optimisation file "<<checkfile1.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptval+" does not exist.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

        QString qOptm = ui->lineEdit_17->text();
        string file2opt = qOptm.toUtf8().constData();
        std::ostringstream file2NameStream(file2opt);
        file2NameStream<<path<<"/"<<file2opt;
        std::string file2Name = file2NameStream.str();
        ifstream initiate2(file2Name.c_str());

        QFile checkfile(file2Name.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The optimisation file "<<checkfile.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptm+" does not exist.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

        int number_of_lines =0;

        int NVar1=0, Plock=0, elock=0, T0lock=0;
        int Bin;
        int add=0;

        if(ui->checkBox_18->isChecked()){
            ++add;
            Plock=1;
        }
        if(ui->checkBox_17->isChecked()){
            ++add;
            elock=1;
        }
        if(ui->checkBox_35->isChecked()){
            ++add;
            T0lock=1;
        }

        NVar1=7-add;
        cout<<add<<endl;
        cout<<NVar1<<endl;
        cout<<epara<<endl;

        if(epara!=NVar1+1){
            qDebug()<<"Wrong optimisation file.";
            QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptval+" does not match to specifications in main window.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

        double Bres;
        int NVar = (NVar1+1)+(NVar1+1)*NVar1/2;
        QVector<double> Res(NVar);

        for (int i=0; i<epara; i++){
        initiate1 >> one;
        istringstream ist(one);
        ist >> Res[i];
        if(i==0){
            Bin = i;
            Bres = Res[0];
        }
        if(Res[i]<Bres){
            Bres=Res[i];
            Bin = i;
        }
        }
        initiate1.close();

        number_of_lines=0;

        while(std::getline(initiate2, zeile))
           ++ number_of_lines;

        initiate2.clear();
        initiate2.seekg(0, ios::beg);

        if(number_of_lines!=NVar1*(NVar1+1)){
            qDebug()<<"Wrong optimisation file.";
            QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptm+" does not match to specifications in main window.");
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

        double V[NVar][NVar1];

        for (int i=0; i<NVar1+1; i++){
            for(int e =0; e<NVar1; e++){
        initiate2 >> one;
        istringstream ist(one);
        ist >> V[i][e];
            }

        }
        initiate2.close();

        int indicat=0;

        for(int e =0; e<NVar1; e++){
            for(int i=0; i<NVar1; i++){
                if(V[i][e]==V[i+1][e]){
                    ++indicat;
                }
            }
            if(indicat==NVar1){
                QString ftar = QString::number(e);
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Warning!", "Parameter "+ftar+" has not changed. This will lead to a singular matrix inversion. Do you want to vary it?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {    // Variing parameters
                  qDebug() << "Variing Parameter "+ftar;
                    if(e+add==0){

                    }
                    if(e+add==1){

                    }
                    if(e+add==2){

                    }
                    if(e+add==3){

                    }


                } else {    //abort error estimation
                  qDebug() << "Error estimation via curvature matrix aborted.";
                  return;
                }
            }
            else{
                indicat=0;
            }
        }

        int laufa=0, laufb=0, laufc=0;
        QVector<double> Theta0j(NVar1);

        /*for(int i =0; i<NVar1; i++){
            if(i!=Bin){
                Theta0j(laufa)=EP
            }
        }*/


        QVector<double> Chi0i(NVar1);
        mat Chi(NVar1,NVar1);
        QString qChi;

        for(int i = 0; i<NVar1; i++){
            for(int j = 0; j<NVar1; j++){
                Chi(i,j)=0;
            }
        }

        for(int i =0; i<NVar1; i++){

            laufc=0;

            for(int n =i; n<NVar1+1; n++){
                if(n!=i){

                for(int j =0; j<NVar1; j++){
                        V[laufa+NVar1+1][j]=(V[i][j]+V[n][j])/2;
                        if(add==0){
                        orbele[j]=V[laufa+NVar1+1][j];
                        }
                        if(add==1){
                            if(Plock==1){
                                orbele[j+1]=V[laufa+NVar1+1][j];
                            }
                            if(elock==1){
                                if(j==0){
                                orbele[0]=V[laufa+NVar1+1][j];
                                }
                                if(j>0){
                                    orbele[j+1]=V[laufa+NVar1+1][j];
                                }
                            }
                            if(T0lock==1){
                                if(j==5){
                                    orbele[6]=V[laufa+NVar1+1][j];
                                }
                                else orbele[j]=V[laufa+NVar1+1][j];
                            }
                        }
                        if(add==2){
                            if((Plock==1) & (elock==1)){
                                orbele[j+2]=V[laufa+NVar1+1][j];
                            }
                            if((Plock==1) & (T0lock==1)){
                                if(j==4){
                                    orbele[6]=V[laufa+NVar1+1][j];
                                }
                                else{
                                    orbele[j+1]=V[laufa+NVar1+1][j];
                                }
                            }
                            if((elock==1) & (T0lock==1)){
                                if(j==0) orbele[0]=V[laufa+NVar1+1][j];
                                if(j==1) orbele[2]=V[laufa+NVar1+1][j];
                                if(j==2) orbele[3]=V[laufa+NVar1+1][j];
                                if(j==3) orbele[4]=V[laufa+NVar1+1][j];
                                if(j==4) orbele[6]=V[laufa+NVar1+1][j];
                            }
                        }
                        if(add==3){
                            if(j==0) orbele[2]=V[laufa+NVar1+1][j];
                            if(j==1) orbele[3]=V[laufa+NVar1+1][j];
                            if(j==2) orbele[4]=V[laufa+NVar1+1][j];
                            if(j==3) orbele[6]=V[laufa+NVar1+1][j];
                        }
                }

                MainWindow::VAmplitudeA();
                MainWindow::VAmplitudeB();
                MainWindow::ConstructMatrix();
                Res[laufa+NVar1+1]=MainWindow::DivideConquer();
                qApp->processEvents(QEventLoop::AllEvents);
                if(abortt==1){
                    abortt=0;
                    QMessageBox::information(this, "Abort", "Error estimation aborted.");
                    return;
                }

                if(i==Bin){
                    cout<<"Chi0i["<<laufb<<"]: ";
                    Chi0i[laufb] = Res[laufa+NVar1+1];
                    cout<<Chi0i[laufb]<<endl;
                    QString qlau = QString::number(laufb);
                    qChi = QString::number(Chi0i[laufb]);
                    ui->plainTextEdit_2->appendPlainText("Chi0i["+qlau+"]: "+qChi);
                    ++laufb;
                }
                cout<<"Chi["<<i<<"]["<<laufc<<"]: ";
                Chi(i,laufc)=Res[laufa+NVar1+1];
                cout<<Chi(i,laufc)<<endl;
                QString qlau = QString::number(laufc);
                QString qi = QString::number(i);
                qChi = QString::number(Chi(i,laufc));
                ui->plainTextEdit_2->appendPlainText("Chi["+qi+"]["+qlau+"]: "+qChi);
                ++laufc;
                ++laufa;
                }

            }
        }

        cout<<endl;
        cout<<Chi<<endl;
        cout<<endl;
        cout<<"Vector a:"<<endl;

        mat Q(NVar1,NVar1);
        QVector<double> a(NVar1);
        mat B(NVar1,NVar1);

        laufa=0;
        laufb=0;

        for(int i =0; i<NVar1+1; i++){
            if(i!=Bin){
            a[laufb] = 4*pow(Chi0i[laufb],2)-pow(Res[i],2)-3*pow(Res[Bin],2);
            cout<<a[laufb]<<endl;
            ++laufb;

            }
        }

        for(int i =0; i<NVar1+1; i++){
            if(i!=Bin){

            for(int j = 0; j<NVar1; j++){

                Q(laufa,j)=V[i][j]-V[Bin][j];
                }
            ++laufa;
            }
        }

        cout<<endl;
        cout<<"Matrix Q:"<<endl;
        cout<<Q<<endl;
        cout<<endl;

        laufa=0;

        for(int i =0; i<NVar1; i++){
            for(int j =0; j<NVar1-1;j++){
                if(Q(j,i)==Q(j+1,i)){
                    ++laufa;
                }
            }
            if(laufa==NVar1-1){
                QMessageBox::information(this, "Error", "Inversion of Matrix Q appears to be singular.");
                ui->plainTextEdit_2->appendPlainText("Inversion of Matrix Q appears to be singular.");
                return;
            }
            laufa=0;
        }

        laufa=0;
        laufb=0;

        mat q = inv(Q);
        mat qt = q.t();

        for(int i = 0; i<NVar1+1; i++){
            if(i!=Bin){
                for(int n = 0; n<NVar1; n++){
                    if(n==laufa){
                    B(laufa,n)=2*(pow(Chi(laufa,n),2)+pow(Res[Bin],2)-2*pow(Chi0i[laufa],2));

                    }
                    else{
                        B(laufa,n) = 2*(pow(Chi(laufa,n),2)+pow(Res[Bin],2)-pow(Chi0i[laufa],2)-pow(Chi(Bin,n),2));
                    }
                }
                ++laufa;
            }
        }

        mat Cov;

        Cov = B*q;
        Cov = qt*Cov/2;

        mat Gamma = inv(Cov);

        cout<<q<<endl;
        cout<<endl;
        cout<<qt<<endl;
        cout<<endl;
        cout<<B<<endl;
        cout<<endl;
        cout<<Cov<<endl;
        cout<<endl;
        cout<<Gamma<<endl;

        QString qErr = ui->lineEdit_24->text();
        string fileErr = qErr.toUtf8().constData();
        std::ostringstream fileErrStream(fileErr);
        fileErrStream<<path<<"/"<<fileErr;
        std::string file2Err = fileErrStream.str();
        ofstream Err(file2Err.c_str());

        Err<<"Matrix Q:"<<endl;
        Err<<Q<<endl;
        Err<<endl;
        Err<<"Inversion of Q:"<<endl;
        Err<<q<<endl;
        Err<<endl;
        Err<<"Matrix qt:"<<endl;
        Err<<qt<<endl;
        Err<<endl;
        Err<<"Matrix B:"<<endl;
        Err<<B<<endl;
        Err<<endl;
        Err<<"Matrix Cov:"<<endl;
        Err<<Cov<<endl;
        Err<<endl;
        Err<<"Matrix Gamma:"<<endl;
        Err<<Gamma<<endl;

        ui->plainTextEdit_2->appendPlainText("Data written to file "+qErr);
}



void MainWindow::on_pushButton_8_clicked()
{
    this->setCursor(QCursor(Qt::WaitCursor));

    ui->plainTextEdit_2->appendPlainText("Error Estimation");

    MainWindow::ErrorEstimation();

    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::ComputeDifferences()
{
    double DiffA, DiffB;

    //Systemic velocity
    if(CDI==0){

        DiffA = dv - orbele[4];
    }

    //Amplitude A
    if(CDI==0){

    }

    //Amplitude B
    if(CDI==0){

    }

    //Periode
    if(CDI==0){

    }

    //Eccentricity
    if(CDI==0){

    }

    //Periastron time
    if(CDI==0){

    }

    //length of periastron
    if(CDI==0){

    }

    if(DiffA>DiffB){
        Diff = DiffA;
    }
    else Diff = DiffB;
}


void MainWindow::on_pushButton_4_clicked()
{
    MainWindow::Optimisation();

    if(reinitiate==1){
        reinitiate=0;
        inrein=1;
        MainWindow::on_pushButton_4_clicked();
    }
    this->setCursor(QCursor(Qt::ArrowCursor));

}

void MainWindow::on_checkBox_34_clicked()
{
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_8->setEnabled(false);
}

// Save settings
void MainWindow::on_pushButton_9_clicked()
{
    QFile SPS(qPath+"/"+ui->lineEdit_22->text()+".sps");
    QString qSPS = ui->lineEdit_22->text();
    string sps = qSPS.toUtf8().constData();
    std::ostringstream datANameStream(sps);
    datANameStream<<path<<"/"<<sps<<".sps";
    std::string datAName = datANameStream.str();

    if(SPS.exists()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning!", "The file already exists. \n\n Do you want to overwrite it?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";

          ofstream outSPS(datAName.c_str());

          outSPS<<ui->spinBox->value()<<endl;       // first spectrum
          outSPS<<ui->spinBox_2->value()<<endl;     // last spectrum
          outSPS<<ui->spinBox_3->value()<<endl;     // number of iterations
          outSPS<<ui->spinBox_4->value()<<endl;     // iterations
          outSPS<<ui->spinBox_5->value()<<endl;     // evaluations
          outSPS<<ui->spinBox_6->value()<<endl;     // last update

          outSPS<<ui->doubleSpinBox->value()<<endl;     // max. RV comp 1
          outSPS<<ui->doubleSpinBox_2->value()<<endl;   // max RV comp 2
          outSPS<<ui->doubleSpinBox_3->value()<<endl;   // velocity step size
          outSPS<<ui->doubleSpinBox_4->value()<<endl;   // flux ratio
          outSPS<<ui->doubleSpinBox_5->value()<<endl;   // off-set difference
          outSPS<<ui->doubleSpinBox_14->value()<<endl;  // lower wavelength for residuum calculation
          outSPS<<ui->doubleSpinBox_15->value()<<endl;  // upper wavelength for residuum calculation

          int cBox = ui->comboBox->currentIndex();
          outSPS<<cBox<<endl;

          QString qstring;
          string outs;
          qstring = ui->lineEdit->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;       // RV data CroCo

          qstring = ui->lineEdit_2->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;     // velocity file

          qstring = ui->lineEdit_3->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // common name rebined spectra

          qstring = ui->lineEdit_4->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // work path

          qstring = ui->lineEdit_5->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // output component spectra 1

          qstring = ui->lineEdit_6->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // time

          qstring = ui->lineEdit_7->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // min. residuum

          qstring = ui->lineEdit_8->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // output component spectra 2

          qstring = ui->lineEdit_9->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // extension of fits table

          qstring = ui->lineEdit_10->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // output of static component

          qstring = ui->lineEdit_11->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // name of wavelength column in fits table

          qstring = ui->lineEdit_12->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // name of intensity column in fits table

          qstring = ui->lineEdit_13->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // output differences

          qstring = ui->lineEdit_14->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // initial residua

          qstring = ui->lineEdit_15->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // initial variables

          qstring = ui->lineEdit_16->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // optimised reidua

          qstring = ui->lineEdit_17->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // optimised variables

          qstring = ui->lineEdit_18->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // file with obs. times for optimisation on orbital parameters

          qstring = ui->lineEdit_19->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // extension rebined spectra

          qstring = ui->lineEdit_20->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // output error

          qstring = ui->lineEdit_21->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // lofgile

          qstring = ui->lineEdit_24->text();
          outs = qstring.toUtf8().constData();
          outSPS<<outs<<endl;   // parameter error file

          if(ui->checkBox->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_2->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_3->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_4->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_5->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_6->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_7->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_8->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_9->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_10->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_11->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_12->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_13->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_14->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_15->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_16->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_17->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_18->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_32->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_33->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_34->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }

          if(ui->checkBox_35->isChecked()){
              outSPS<<"1"<<endl;
          }
          else{
              outSPS<<"0"<<endl;
          }


        } else {
          qDebug() << "Yes was *not* clicked";
          return;
        }
    }

    else{

        ofstream outSPS(datAName.c_str());

        outSPS<<ui->spinBox->value()<<endl;       // first spectrum
        outSPS<<ui->spinBox_2->value()<<endl;     // last spectrum
        outSPS<<ui->spinBox_3->value()<<endl;     // number of iterations
        outSPS<<ui->spinBox_4->value()<<endl;     // iterations
        outSPS<<ui->spinBox_5->value()<<endl;     // evaluations
        outSPS<<ui->spinBox_6->value()<<endl;     // last update

        outSPS<<ui->doubleSpinBox->value()<<endl;     // max. RV comp 1
        outSPS<<ui->doubleSpinBox_2->value()<<endl;   // max RV comp 2
        outSPS<<ui->doubleSpinBox_3->value()<<endl;   // velocity step size
        outSPS<<ui->doubleSpinBox_4->value()<<endl;   // flux ratio
        outSPS<<ui->doubleSpinBox_5->value()<<endl;   // off-set difference
        outSPS<<ui->doubleSpinBox_14->value()<<endl;  // lower wavelength for residuum calculation
        outSPS<<ui->doubleSpinBox_15->value()<<endl;  // upper wavelength for residuum calculation

        int cBox = ui->comboBox->currentIndex();
        outSPS<<cBox<<endl;

        QString qstring;
        string outs;
        qstring = ui->lineEdit->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;       // RV data CroCo

        qstring = ui->lineEdit_2->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;     // velocity file

        qstring = ui->lineEdit_3->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // common name rebined spectra

        qstring = ui->lineEdit_4->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // work path

        qstring = ui->lineEdit_5->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // output component spectra 1

        qstring = ui->lineEdit_6->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // time

        qstring = ui->lineEdit_7->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // min. residuum

        qstring = ui->lineEdit_8->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // output component spectra 2

        qstring = ui->lineEdit_9->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // extension of fits table

        qstring = ui->lineEdit_10->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // output of static component

        qstring = ui->lineEdit_11->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // name of wavelength column in fits table

        qstring = ui->lineEdit_12->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // name of intensity column in fits table

        qstring = ui->lineEdit_13->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // output differences

        qstring = ui->lineEdit_14->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // initial residua

        qstring = ui->lineEdit_15->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // initial variables

        qstring = ui->lineEdit_16->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // optimised reidua

        qstring = ui->lineEdit_17->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // optimised variables

        qstring = ui->lineEdit_18->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // file with obs. times for optimisation on orbital parameters

        qstring = ui->lineEdit_19->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // extension rebined spectra

        qstring = ui->lineEdit_20->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // output error

        qstring = ui->lineEdit_21->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // lofgile

        qstring = ui->lineEdit_24->text();
        outs = qstring.toUtf8().constData();
        outSPS<<outs<<endl;   // parameter error file

        if(ui->checkBox->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_2->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_3->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_4->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_5->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_6->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_7->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_8->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_9->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_10->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_11->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_12->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_13->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_14->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_15->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_16->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_17->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_18->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_32->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_33->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_34->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }

        if(ui->checkBox_35->isChecked()){
            outSPS<<"1"<<endl;
        }
        else{
            outSPS<<"0"<<endl;
        }
    }
}

// load
void MainWindow::on_pushButton_10_clicked()
{
    QString qSPS = qPath+"/"+ ui->lineEdit_23->text() +".sps";
    QFile SPS(qSPS);

    string sps = qSPS.toUtf8().constData();
    std::ostringstream datANameStream(sps);
    datANameStream<<sps;
    std::string datAName = datANameStream.str();

    if(!SPS.exists()){
        QMessageBox::information(this, "Error", "File "+ qSPS +" does not exist.");
        return;
    }
    else{
        int SetP = 58, SetN=14, SetH=22, SetB=22;
        QVector<double> SetV(SetN);
        QVector<string> SetC(SetH);
        QVector<int> SetCB(SetB);

        ifstream inSPS(datAName.c_str());
        string eins, cstring;

        for(int g=0; g<SetP; g++){
        inSPS >> eins;
        istringstream ist(eins);
        if((g>SetN-1) & (g<SetN+SetH)){
            ist >> SetC[g-SetN];
        }
        if(g<SetN){

        ist >> SetV[g];
        }
        else{
            ist >> SetCB[g-SetN-SetH];
        }
        }
        inSPS.close();

        ui->spinBox->setValue(SetV[0]);
        ui->spinBox_2->setValue(SetV[1]);
        ui->spinBox_3->setValue(SetV[2]);
        ui->spinBox_4->setValue(SetV[3]);
        ui->spinBox_5->setValue(SetV[4]);
        ui->spinBox_6->setValue(SetV[5]);

        ui->doubleSpinBox->setValue(SetV[6]);
        ui->doubleSpinBox_2->setValue(SetV[7]);
        ui->doubleSpinBox_3->setValue(SetV[8]);
        ui->doubleSpinBox_4->setValue(SetV[9]);
        ui->doubleSpinBox_5->setValue(SetV[10]);
        ui->doubleSpinBox_14->setValue(SetV[11]);
        ui->doubleSpinBox_15->setValue(SetV[12]);

        ui->comboBox->setCurrentIndex(SetV[13]);

        QString qSet = QString::fromStdString(SetC[0]);
        ui->lineEdit->setText(qSet);

        qSet = QString::fromStdString(SetC[1]);
        ui->lineEdit_2->setText(qSet);

        qSet = QString::fromStdString(SetC[2]);
        ui->lineEdit_3->setText(qSet);

        qSet = QString::fromStdString(SetC[3]);
        ui->lineEdit_4->setText(qSet);

        qSet = QString::fromStdString(SetC[4]);
        ui->lineEdit_5->setText(qSet);

        qSet = QString::fromStdString(SetC[5]);
        ui->lineEdit_6->setText(qSet);

        qSet = QString::fromStdString(SetC[6]);
        ui->lineEdit_7->setText(qSet);

        qSet = QString::fromStdString(SetC[7]);
        ui->lineEdit_8->setText(qSet);

        qSet = QString::fromStdString(SetC[8]);
        ui->lineEdit_9->setText(qSet);

        qSet = QString::fromStdString(SetC[9]);
        ui->lineEdit_10->setText(qSet);

        qSet = QString::fromStdString(SetC[10]);
        ui->lineEdit_11->setText(qSet);

        qSet = QString::fromStdString(SetC[11]);
        ui->lineEdit_12->setText(qSet);

        qSet = QString::fromStdString(SetC[12]);
        ui->lineEdit_13->setText(qSet);

        qSet = QString::fromStdString(SetC[13]);
        ui->lineEdit_14->setText(qSet);

        qSet = QString::fromStdString(SetC[14]);
        ui->lineEdit_15->setText(qSet);

        qSet = QString::fromStdString(SetC[15]);
        ui->lineEdit_16->setText(qSet);

        qSet = QString::fromStdString(SetC[16]);
        ui->lineEdit_17->setText(qSet);

        qSet = QString::fromStdString(SetC[17]);
        ui->lineEdit_18->setText(qSet);

        qSet = QString::fromStdString(SetC[18]);
        ui->lineEdit_19->setText(qSet);

        qSet = QString::fromStdString(SetC[19]);
        ui->lineEdit_20->setText(qSet);

        qSet = QString::fromStdString(SetC[20]);
        ui->lineEdit_21->setText(qSet);

        qSet = QString::fromStdString(SetC[21]);
        ui->lineEdit_24->setText(qSet);

        if(SetCB[0]==0){
            ui->checkBox->setChecked(false);
        }
        else{
            ui->checkBox->setChecked(true);
        }

        if(SetCB[1]==0){
            ui->checkBox_2->setChecked(false);
        }
        else{
            ui->checkBox_2->setChecked(true);
        }

        if(SetCB[2]==0){
            ui->checkBox_3->setChecked(false);
        }
        else{
            ui->checkBox_3->setChecked(true);
        }

        if(SetCB[3]==0){
            ui->checkBox_4->setChecked(false);
        }
        else{
            ui->checkBox_4->setChecked(true);
        }

        if(SetCB[4]==0){
            ui->checkBox_5->setChecked(false);
        }
        else{
            ui->checkBox_5->setChecked(true);
        }

        if(SetCB[5]==0){
            ui->checkBox_6->setChecked(false);
        }
        else{
            ui->checkBox_6->setChecked(true);
        }

        if(SetCB[6]==0){
            ui->checkBox_7->setChecked(false);
        }
        else{
            ui->checkBox_7->setChecked(true);
        }

        if(SetCB[7]==0){
            ui->checkBox_8->setChecked(false);
        }
        else{
            ui->checkBox_8->setChecked(true);
        }

        if(SetCB[8]==0){
            ui->checkBox_9->setChecked(false);
        }
        else{
            ui->checkBox_9->setChecked(true);
        }

        if(SetCB[9]==0){
            ui->checkBox_10->setChecked(false);
        }
        else{
            ui->checkBox_10->setChecked(true);
        }

        if(SetCB[10]==0){
            ui->checkBox_11->setChecked(false);
        }
        else{
            ui->checkBox_11->setChecked(true);
        }

        if(SetCB[11]==0){
            ui->checkBox_12->setChecked(false);
        }
        else{
            ui->checkBox_12->setChecked(true);
        }

        if(SetCB[12]==0){
            ui->checkBox_13->setChecked(false);
        }
        else{
            ui->checkBox_13->setChecked(true);
        }

        if(SetCB[13]==0){
            ui->checkBox_14->setChecked(false);
        }
        else{
            ui->checkBox_14->setChecked(true);
        }

        if(SetCB[14]==0){
            ui->checkBox_15->setChecked(false);
        }
        else{
            ui->checkBox_15->setChecked(true);
        }

        if(SetCB[15]==0){
            ui->checkBox_16->setChecked(false);
        }
        else{
            ui->checkBox_16->setChecked(true);
        }

        if(SetCB[16]==0){
            ui->checkBox_17->setChecked(false);
        }
        else{
            ui->checkBox_17->setChecked(true);
        }

        if(SetCB[17]==0){
            ui->checkBox_18->setChecked(false);
        }
        else{
            ui->checkBox_18->setChecked(true);
        }

        if(SetCB[18]==0){
            ui->checkBox_32->setChecked(false);
        }
        else{
            ui->checkBox_32->setChecked(true);
        }

        if(SetCB[19]==0){
            ui->checkBox_33->setChecked(false);
        }
        else{
            ui->checkBox_33->setChecked(true);
        }

        if(SetCB[20]==0){
            ui->checkBox_34->setChecked(false);
        }
        else{
            ui->checkBox_34->setChecked(true);
        }

        if(SetCB[21]==0){
            ui->checkBox_35->setChecked(false);
        }
        else{
            ui->checkBox_35->setChecked(true);
        }

    }
}

//**************************************
// root finding
//**************************************
void MainWindow::findroot(){

    int rn=1, rPh, rPl, rPsh, rzaehler=40, reval=0;
    double ryh, rysh, ryl, rym, ryi, rys, ryt;
    double rstep=0.1;
    double rgamma=2.0;	//expansion coeff.
    double ralpha =1.0;	//reflection coeff.
    double rbeta=0.5;	//contraction coeff.
    double rbtot=0.5;	//total contraction coeff.
    double ry[rn+1], rPm[rn+1][rn], rZ[rn], rCX[rn], rS[rn], rEm[rn], rXX[rn], re[rn][rn];

        //initial points
        rPm[0][0]=2*M_PI*(RVt-RVT0)/RVP-2*RVe;
        for (int i=0; i<rn+1; i++){
            for (int j=0; j<rn; j++){
                if((i>0) & (i==j+1)){
                    re[i][j]=1;
                }
                else{
                    re[i][j]=0;
                }
                if(i==0){
                    rXX[j]=rPm[i][j];
                }
                if(i!=0){
                    rPm[i][j]=rPm[0][j]+rstep*re[i][j];
                    rXX[j]=rPm[i][j];
                }
            }
            ry[i]=Ofunction(rXX, RVt, RVT0, RVP, RVe);
            reval++;
        }

        //start main loop
        for (int tc=0; tc<rzaehler; tc++){

        //initialize next step
            rym=0;
            rys=0;
            for (int i=0; i<rn; i++){
                rZ[i]=0;
            }

        //looking for highest value
            ryh=ry[0];
            for (int j=0; j<rn+1; j++){
                if(ry[j]>=ryh){
                    ryh = ry[j];
                    rPh = j;
                }
            }

        //looking for smallest value
            ryl=ryh;
            for (int j=0; j<rn+1; j++){
                if(ry[j]<ryl){
                    ryl=ry[j];
                    rPl = j;
                }
            }

        // second highest value
            rysh=ryl;
            ryh=ry[rPh];
            ryl=ry[rPl];
            rysh=ry[rPsh];

        //computing mean and sigma
            for (int i=0; i<rn+1; i++){
                rym+=ry[i]/(rn+1);
            }
            for (int i=0; i<rn+1; i++){
                rys+=sqrt(pow((ry[i]-rym),2));
            }
            rys=rys/(rn);

        //compute centroid
            for (int j=0; j<rn; j++){
                for (int i=0; i<rn+1; i++){
                    if (i!=rPh){
                        rZ[j]+=rPm[i][j]/rn;
                    }
                }
            }

        //reflect highest value at centroid
            for (int i=0; i<rn; i++){
                rCX[i]=rZ[i]+ralpha*(rZ[i]-rPm[rPh][i]);
            }
            ryi=Ofunction(rCX, RVt, RVT0, RVP, RVe);
            reval++;

            if(ryi<ryl){
                for (int i=0; i<rn; i++){
                    rEm[i]=rZ[i]+rgamma*(rCX[i]-rZ[i]);
                }
                ryt=Ofunction(rEm, RVt, RVT0, RVP, RVe);
                reval++;
                if(ryt<ryl){
                    for (int i=0; i<rn; i++){
                        rPm[rPh][i]=rEm[i];
                    }
                    ry[rPh]=ryt;
                }
                if (ryt>=ryl){
                    for (int i=0; i<rn; i++){
                        rPm[rPh][i]=rCX[i];
                    }
                    reval++;
                    ry[rPh]=Ofunction(rCX, RVt, RVT0, RVP, RVe);
                }
            }

            if(ryi>=ryl){
                if(ryi<=rysh){
                    for(int i=0; i<rn; i++){
                        rPm[rPh][i]=rCX[i];
                    }
                    reval++;
                    ry[rPh]=Ofunction(rCX, RVt, RVT0, RVP, RVe);
                }
                if(ryi>rysh){
                    if(ryi<=ryh){
                        for(int i=0; i<rn; i++){
                            rPm[rPh][i]=rCX[i];
                        }
                        reval++;
                        ry[rPh]=Ofunction(rCX, RVt, RVT0, RVP, RVe);
                        ryh=ry[rPh];
                    }
                    for(int i=0; i<rn; i++){
                        rS[i]=rZ[i]+rbeta*(rPm[rPh][i]-rZ[i]);
                    }
                    ryt=Ofunction(rS, RVt, RVT0, RVP, RVe);
                    reval++;
                    if(ryt>ryh){
                        for (int j=0; j<rn+1; j++){
                            for (int i=0; i<rn; i++){
                                rPm[j][i]=rPm[rPl][i]+rbtot*(rPm[j][i]-rPm[rPl][i]); //total contraction
                                rXX[i]=rPm[j][i];
                            }
                            ry[j]=Ofunction(rXX, RVt, RVT0, RVP, RVe);
                            reval++;
                        }
                    }

                    if(ryt<=ryh){
                        for(int i=0; i<rn; i++){
                            rPm[rPh][i]=rS[i];
                        }
                        reval++;
                        ry[rPh]=Ofunction(rS, RVt, RVT0, RVP, RVe);
                    }
                }
            }
        }//end main loop

        //looking for highest value
        ryh=ry[0];
        for (int j=0; j<rn+1; j++){
            if(ry[j]>=ryh){
                ryh = ry[j];
                rPh = j;
            }
        }

        //looking for smallest value
        ryl=ryh;
        for (int j=0; j<rn+1; j++){
            if(ry[j]<ryl){
                ryl=ry[j];
                rPl = j;
            }
        }

        //looking for second highest value
        rysh=ryl;
        for (int j=0; j<rn+1; j++){
            if(ry[j]>rysh & ry[j]<ryh){
                rysh=ry[j];
                rPsh=j;
            }
        }

        RVE=rPm[rPl][0];
}

//*************************
// apply file names
//*************************
void MainWindow::on_pushButton_11_clicked()
{
    QString cName = ui->lineEdit_25->text();
    QString cExt = ui->lineEdit_26->text();

    ui->lineEdit_5->setText(cName+"_1."+cExt);
    ui->lineEdit_8->setText(cName+"_2."+cExt);
    ui->lineEdit_10->setText(cName+"_tell."+cExt);
    ui->lineEdit_13->setText(cName+"_diff."+cExt);
    ui->lineEdit_14->setText("initval_"+cName+"."+cExt);
    ui->lineEdit_15->setText("initmat_"+cName+"."+cExt);
    ui->lineEdit_16->setText("optval_"+cName+"."+cExt);
    ui->lineEdit_17->setText("optmat_"+cName+"."+cExt);
    ui->lineEdit_20->setText(cName+"_error."+cExt);
    ui->lineEdit_21->setText(cName+"_log."+cExt);
    ui->lineEdit_22->setText(cName+"_conf");
    ui->lineEdit_23->setText(cName+"_conf");
}

void MainWindow::on_pushButton_12_clicked()
{
    this->setCursor(QCursor(Qt::WaitCursor));

     MainWindow::read_data();

     this->setCursor(QCursor(Qt::WaitCursor));

     int smin= ui->spinBox->value();
     int smax = ui->spinBox_2->value();
     int mlo = (smax-smin+1)*logbin, zlin=0;
     string liner, ein1, zwei2;

     QString fext=ui->lineEdit_19->text();
     string sfext = fext.toUtf8().constData();

     // subtract A
     if(ui->checkBox_20->isChecked()){
         mat MA(mlo, bso1);
         vec sol1(mlo), wav(bso1), sol2(bso1), diff(mlo);


             for(int i = 0; i<mlo; i++){
                 for(int n = 0; n<bso1; n++){
                     MA(i,n) = M(i,n);
                 }
             }



             QString input=ui->lineEdit_5->text();
             string in = input.toUtf8().constData();
             std::ostringstream inNameStream(in);
             inNameStream<<path<<"/"<<in;
             std::string inName = inNameStream.str();
             ifstream file1(inName.c_str());

             QFile checkfile1(inName.c_str());

             if(!checkfile1.exists()){
                 qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
                 QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+".txt does not exist!");
                 this->setCursor(QCursor(Qt::ArrowCursor));
                 return;
             }

             while(std::getline(file1, liner))
                ++zlin;

             file1.clear();
             file1.seekg(0, ios::beg);

             for(int i =0;i <zlin; i++){
                 if(i<bso1){
                 file1 >> ein1 >>zwei2;
                 istringstream ist(ein1);
                 ist >> wav[i];
                 istringstream ist2(zwei2);
                 ist2 >> sol2[i];
                 }
                 else{
                     wav[i]=0;
                     sol2[i]=0;
                 }
             }


             sol1 = MA*sol2;
             diff = C-sol1;

             cout<<C<<endl;
             cout<<endl;

             for(int e = 0; e<smax-smin+1; e++){
                QString output=ui->lineEdit_27->text();
                string out = output.toUtf8().constData();
                std::ostringstream file1NameStream(out);
                file1NameStream<<path<<"/"<<out<<"_"<<e<<sfext;
                std::string file1Name = file1NameStream.str();
                ofstream file2(file1Name.c_str());

                for(int i=e*logbin; i< (e+1)*logbin; i++){
                     if(pow(10,W[i])>1){
                        file2<<setprecision(14)<<pow(10,(W[i]))<<"\t"<<diff[i]<<endl;
                    }
                }
                file2.close();
             }

     }

     // subtract B
     if(ui->checkBox_21->isChecked()){
         mat MA(mlo, bso2);
         vec sol1(mlo), wav(bso2), sol2(bso2), diff(mlo);


             for(int i = 0; i<mlo; i++){
                 for(int n = 0; n<bso2; n++){
                     MA(i,n) = M(i,n+bso1);
                 }
             }

             QString input=ui->lineEdit_8->text();
             string in = input.toUtf8().constData();
             std::ostringstream inNameStream(in);
             inNameStream<<path<<"/"<<in;
             std::string inName = inNameStream.str();
             ifstream file1(inName.c_str());

             QFile checkfile1(inName.c_str());

             if(!checkfile1.exists()){
                 qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
                 QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+".txt does not exist!");
                 this->setCursor(QCursor(Qt::ArrowCursor));
                 return;
             }

             while(std::getline(file1, liner))
                ++zlin;

             file1.clear();
             file1.seekg(0, ios::beg);

             for(int i =0;i <zlin; i++){
                 if(i<bso2){
                 file1 >> ein1 >>zwei2;
                 istringstream ist(ein1);
                 ist >> wav[i];
                 istringstream ist2(zwei2);
                 ist2 >> sol2[i];
                 }
                 else{
                     wav[i]=0;
                     sol2[i]=0;
                 }
             }


             sol1 = MA*sol2;
             diff = C-sol1;

             cout<<C<<endl;
             cout<<endl;

             for(int e = 0; e<smax-smin+1; e++){
                QString output=ui->lineEdit_27->text();
                string out = output.toUtf8().constData();
                std::ostringstream file1NameStream(out);
                file1NameStream<<path<<"/"<<out<<"_"<<e<<sfext;
                std::string file1Name = file1NameStream.str();
                ofstream file2(file1Name.c_str());

                for(int i=e*logbin; i< (e+1)*logbin; i++){
                     if(pow(10,W[i])>1){
                        file2<<setprecision(14)<<pow(10,(W[i]))<<"\t"<<diff[i]<<endl;
                    }
                }
                file2.close();
             }

     }

     this->setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::on_spinBox_8_valueChanged()
{

/*
    cores = ui->spinBox_8->value();

    if(cores>ui->spinBox_7->value()){
        QString qcores = QString::number(ui->spinBox_7->value());
        QMessageBox::information(this, "Error", "Set number of cores <= "+qcores+".");
        return;
    }

    openblas_set_num_threads(cores);
    */

}

void MainWindow::on_checkBox_22_clicked()
{
    if(ui->checkBox_22->isChecked()){
        ui->customPlot_2->show();
    }
    else{
        ui->customPlot_2->hide();
    }
}
