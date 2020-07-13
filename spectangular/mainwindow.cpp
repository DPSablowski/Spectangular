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
//#include <thread>
#include <QFileInfo>
//#include <cblas.h>

using namespace std;
using namespace arma;
using namespace arpack;
using namespace blas;
using std::chrono::system_clock;

mat ZZ(2,3);

int reinitiate=0, mini, CDI, maxi, num, checker=0, bso1, bso2, logbin, RV1m, RV3m, RV1a, RV3a, Mn, Mm, elements, bidi=0, error=0, zaehler, abortt=0, eval=0;
int upda;       // live update
int updac;      // save all updates
int inrein=0;   // reinitiate
int abauto=0;   // auto abort
int runow=0;    // start optimisation from terminal without windows; 1 for yes, 0 for no.
int overw=0;    //
int sequence=0; // sequence
int optfratios=0;   // optmize on flux ratios;
unsigned int cores;
string path, eins, zwei, line;
QString qPath, qExtension, qWCol, qICol, qInitval, qInitmat, qOptval, qOptmat;
QVector<double> RV1(1), RV2(1), RV3(1), edits(5), Mval1(1), Mval2(1), Mtel(1), otimes(1), orbele(7), dorbele(7);
QVector<double> cpoints(8);
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
mat Mt(Mm,Mn); //transpose of transformation matrix
sp_mat A(Mm,Mn);
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
    ui->lineEdit_3->setText("rebined_spectra_");
    ui->lineEdit_18->setText("times.dat");
    ui->lineEdit_4->setText(QDir::currentPath());
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    ui->lineEdit_14->setText("initval_star_orb.dat");
    qInitval = ui->lineEdit_14->text();
    ui->lineEdit_15->setText("initmat_star_orb.dat");
    qInitmat = ui->lineEdit_15->text();

    ui->lineEdit_16->setText("optval_star_orb.dat");
    qOptval = ui->lineEdit_16->text();
    ui->lineEdit_17->setText("optmat_star_orb.dat");
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

    ui->spinBox_3->setValue(9999);
    zaehler=ui->spinBox_3->value();

    ui->doubleSpinBox_4->setValue(1);
    fluxratio=ui->doubleSpinBox_4->value();

    ui->lineEdit_26->setText("txt");
    ui->lineEdit_25->setText("star_orb");

    string slfile = "star_orb_conf";
    QString qslfile = QString::fromStdString(slfile);

    ui->lineEdit_22->setText(qslfile);
    ui->lineEdit_23->setText(qslfile);

    ui->lineEdit_24->setText("errors.dat");

    ui->checkBox_32->setChecked(true);
    ui->checkBox_23->setChecked(true);
    ui->checkBox_29->setChecked(true);

    ui->spinBox_10->setEnabled(false);
    ui->spinBox_9->setEnabled(false);
    //ui->lineEdit_19->setEnabled(false);

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

    ui->customPlot_4->xAxis2->setVisible(true);
    ui->customPlot_4->xAxis2->setTickLabels(false);
    ui->customPlot_4->yAxis2->setVisible(true);
    ui->customPlot_4->yAxis2->setTickLabels(false);
    ui->customPlot_4->yAxis->setLabel("Residuum");
    ui->customPlot_4->xAxis->setLabel("Iterations");

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
    ui->spinBox_9->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");
    ui->spinBox_10->setStyleSheet("QSpinBox{background: transparent; border: 1px solid black;}");

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
    ui->lineEdit_22->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_23->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_24->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_25->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_26->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_27->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");
    ui->lineEdit_28->setStyleSheet("QLineEdit{background: transparent; color: black; border: 1px solid black;}");

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this ,SLOT(showPointToolTip(QMouseEvent*)));
    connect(ui->customPlot_3, SIGNAL(mouseMove(QMouseEvent*)), this ,SLOT(showPointToolTip_3(QMouseEvent*)));

    //openblas_set_num_threads(2);
    cores =thread::hardware_concurrency();

    ui->spinBox_7->setValue(cores);
    ui->spinBox_8->setValue(cores);
    ui->spinBox_8->setMaximum(cores);

    //cout<<"Run optimization now? 0 no, 1 yes: "<<endl;
    //cin>>runow;
    //runow=1;

    if(runow==1){
        MainWindow::Input();
    }
    else{
        cout<<"Open GUI."<<endl;
    }
}

void MainWindow::Input()
{
    string slfile;
    cout<<"Name of config file:"<<endl;
    cin>>slfile;
    QString qslfile = QString::fromStdString(slfile);
    ui->lineEdit_22->setText(qslfile);
    ui->lineEdit_23->setText(qslfile);
    MainWindow::on_pushButton_10_clicked();     // load Data
    //MainWindow::on_pushButton_5_clicked();      // ECON SVD
    MainWindow::on_pushButton_4_clicked();      // optimise
    cout<<"The END."<<endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//************************************************
// function to calculate inclination
//************************************************
double MainWindow::inclinations (){
    double xA1 = (cpoints[0]+cpoints[1])/2;
    double xA2 = (cpoints[4]+cpoints[5])/2;
    double xB1 = (cpoints[2]+cpoints[3])/2;
    double xB2 = (cpoints[6]+cpoints[7])/2;
    double m=0.0;

    QString input="frcompa.dat";
    string data1 = input.toUtf8().constData();
    std::ostringstream dat1NameStream(data1);
    dat1NameStream<<path<<"/"<<data1;
    std::string dat1Name = dat1NameStream.str();
    ifstream inA(dat1Name.c_str());

    int npix=0;
    string line, eins, zwei;
    while(std::getline(inA, line))
       ++npix;

    inA.clear();
    inA.seekg(0, ios::beg);
    int nmean1=0, nmean2=0;
    double meanA1=0.0, meanA2=0.0, meanB1=0.0, meanB2=0.0;
    double x=0.0, y=0.0;

    for(int i =0; i<npix; i++){
        inA>> eins >>zwei;
        istringstream ist(eins);
        ist >> x;
        istringstream ist2(zwei);
        ist2 >> y;
        if((x>=cpoints[0]) & (x<=cpoints[1])){
            meanA1+=y;
            ++nmean1;
        }
        else{
            if((x>=cpoints[4]) & (x<=cpoints[5])){
                meanA2+=y;
                ++nmean2;
            }
            else{
                //
            }
        }
    }
    meanA1=meanA1/nmean1;
    meanA2=meanA2/nmean2;

    m = pow(((meanA2-meanA1)/(xA2-xA1)),2);
    cout<<"mA^2: "<<m<<endl;

    QString input2="frcompb.dat";
    string data2 = input2.toUtf8().constData();
    std::ostringstream dat2NameStream(data2);
    dat2NameStream<<path<<"/"<<data2;
    std::string dat2Name = dat2NameStream.str();
    ifstream inB(dat2Name.c_str());

    npix=0;

    while(std::getline(inB, line))
       ++npix;

    inB.clear();
    inB.seekg(0, ios::beg);
    nmean1=0;
    nmean2=0;

    for(int i =0; i<npix; i++){
        inB>> eins >>zwei;
        istringstream ist(eins);
        ist >> x;
        istringstream ist2(zwei);
        ist2 >> y;
        if((x>=cpoints[2]) & (x<=cpoints[3])){
            meanB1+=y;
            ++nmean1;
        }
        else{
            if((x>=cpoints[6]) & (x<=cpoints[7])){
                meanB2+=y;
                ++nmean2;
            }
            else{
                //
            }
        }
    }
    meanB1=meanB1/nmean1;
    meanB2=meanB2/nmean2;
    cout<<"mean: "<<meanB1<<"; "<<meanB2<<"; "<<nmean1<<"; "<<nmean2<<endl;
    m += pow(((meanB2-meanB1)/(xB2-xB1)),2);
    cout<<"mA^2 + mB^2: "<<m<<endl;
    m = sqrt(m)/2;
    cout<<"m: "<<m<<endl;

    return m;
    }

//********************************************************
//show mouse coordinates
//********************************************************
void MainWindow::showPointToolTip(QMouseEvent *event)
{

    double xc = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    double yc = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(xc).arg(yc));
}

//********************************************************
//show mouse coordinates
//********************************************************
void MainWindow::showPointToolTip_3(QMouseEvent *event)
{

    double xc = ui->customPlot_3->xAxis->pixelToCoord(event->pos().x());
    double yc = ui->customPlot_3->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(xc).arg(yc));
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
                if(runow==0){
                    QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+fError+".txt does not exist!");
                }
                else{
                    cout<<"Error 1: File "<<path<<"/"<<data1<<g<<" does not exist!"<<endl;
                    MainWindow::Input();
                }
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
                return;
            }
            else{
                // do nothing
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

            dat1.close();
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
                if(runow==0){
                    QMessageBox::information(this, "Error", "Error 2: File "+qPath+"/"+input2+" does not exist!");
                }
                else{
                    cout<<"Error 2: File "<<path<<"/"<<data2<<" does not exist!"<<endl;
                    MainWindow::Input();
                }
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
               return;
            }
            else{

                ifstream dat2(dat2Name.c_str());

                entries=0;
                while(std::getline(dat2, liner))
                    ++entries;

                dat2.clear();
                dat2.seekg(0, ios::beg);

                if(entries!=num){
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 3: Time file "+qPath+"/"+input2+" does not match to the number of observations.");
                    }
                    else{
                        cout<<"Error 3: File "<<path<<"/"<<data2<<" does not match to the number of observations."<<endl;
                        MainWindow::Input();
                    }
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
            dat2.close();
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
                if(runow==0){
                    QMessageBox::information(this, "Error", "Error 4: File"+qPath+"/"+inputt+" does not exist!");
                }
                else{
                    cout<<"Error 4: File "<<path<<"/"<<times<<" does not exist!"<<endl;
                    MainWindow::Input();
                }
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
               return;
            }
            else{

                ifstream dat(intName.c_str());

                entries=0;
                while(std::getline(dat, liner))
                   ++entries;

                dat.clear();
                dat.seekg(0, ios::beg);

                if(entries!=num){
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 5: Time file "+qPath+"/"+inputt+" does not match to the number of observations.");
                    }
                    else{
                        cout<<"Error 5: File "<<path<<"/"<<times<<" does not match to the number of observations."<<endl;
                        MainWindow::Input();
                    }
                    error=1;
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    return;
                }


                for(int g=0; g<num; g++){
                    dat >> eins;
                    istringstream istr(eins);
                    istr >> otimes[g];
                }
                dat.close();
            }

            string orbit = "orbitelements.dat";
            std::ostringstream orbNameStream(orbit);
            orbNameStream<<path<<"/"<<orbit;
            std::string orbName = orbNameStream.str();

            QFile checkorb(orbName.c_str());

            if(!checkorb.exists()){
                qDebug()<<"The file "<<checkorb.fileName()<<" does not exist.";
                if(runow==0){
                    QMessageBox::information(this, "Error", "Error 6: File "+checkorb.fileName()+" does not exist! Set the orbital elements via the Editor.");
                }
                else{
                    cout<<"Error 6: File "<<path<<"/"<<orbit<<" does not exist! Set the orbital elements via the Editor."<<endl;
                    MainWindow::Input();
                }
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
               return;
            }
            else{

                ifstream initorbit(orbName.c_str());

                for(int i=0; i<7; i++){
                    initorbit >> eins;
                    istringstream istr1(eins);
                    istr1 >> orbele[i];
                }
                initorbit.close();
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
            else{
                //
            }
            if (abs(RV3[i])>abs(RV3max)){
                RV3max=RV3[i];
            }
            else{
                //
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
            else{
                //
            }
            if(RV3[i]<RV3min){
                RV3min=RV3[i];
            }
            else{
                //
            }
            if(RV1[i]>RV1maxi){
                RV1maxi=RV1[i];
            }
            else{
                //
            }
            if(RV3[i]>RV3maxi){
                RV3maxi=RV3[i];
            }
            else{
                //
            }
        }

        RV1amin=RV1[0];
        for(int i=0; i<num; i++){
            if(abs(RV1[i])<RV1amin){
                RV1amin=RV1[i];
            }
            else{
                //
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
                        if(runow==0){
                            QMessageBox::information(this, "Error", "Error 7: File"+qPath+"/"+input+fError+".txt does not exist!");
                        }
                        else{
                            cout<<"Error 7: File "<<path<<"/"<<data1<<g<<" does not exist!"<<endl;
                            MainWindow::Input();
                        }
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        error=1;
                        return;
                    }
                    else{

                        ifstream dat1(dat1Name.c_str());

                        dat1 >> eins;
                        istringstream ist(eins);
                        ist >> RV1[g-mini];

                        QString index= QString::number(g);
                        QString velocity=QString::number(RV1[g-mini]);

                        ui->plainTextEdit->appendPlainText(index+" "+velocity);
                        dat1.close();
                    }
                }
            }
            else{
               //
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
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 8: File"+qPath+"/"+input2+" does not exist!");
                    }
                    else{
                        cout<<"Error 8: File "<<path<<"/"<<data2<<" does not exist!"<<endl;
                        MainWindow::Input();
                    }
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                   return;
                }

                else{
                    ifstream dat2(dat2Name.c_str());

                    entries=0;
                    while(std::getline(dat2, liner))
                       ++entries;

                    dat2.clear();
                    dat2.seekg(0, ios::beg);

                    if(entries!=num){
                        if(runow==0){
                            QMessageBox::information(this, "Error", "Error 9: Time file "+qPath+"/"+input2+" does not match to the number of observations.");
                        }
                        else{
                            cout<<"Error 9: File "<<path<<"/"<<data2<<" does not match to the number of observations."<<endl;
                            MainWindow::Input();
                        }
                        error=1;
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        return;
                    }
                    else{

                        for(int g=0; g<=maxi; g++){
                            dat2 >> eins;
                            istringstream ist(eins);

                            if(g>=mini){
                                ist >> RV1[g-mini];
                                QString index= QString::number(g);
                                QString velocity=QString::number(RV1[g]);
                                ui->plainTextEdit->appendPlainText(index+" "+velocity);
                            }
                            else{
                                // do nothing
                            }
                        }
                        dat2.close();
                    }
                }
            }
            else{
                //
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
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 10: File"+qPath+"/"+inputt+" does not exist!");
                    }
                    else{
                        cout<<"Error 10: File "<<path<<"/"<<times<<" does not exist!"<<endl;
                        MainWindow::Input();
                    }
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                   return;
                }
                else{

                    ifstream dat(intName.c_str());

                    entries=0;
                    while(std::getline(dat, liner))
                       ++entries;

                    dat.clear();
                    dat.seekg(0, ios::beg);

                    if(entries!=num){
                        if(runow==0){
                            QMessageBox::information(this, "Error", "Error 11: Time file "+qPath+"/"+inputt+" does not match to the number of observations.");
                        }
                        else{
                            cout<<"Error 1: File "<<path<<"/"<<times<<" does not match to the number of observations."<<endl;
                            MainWindow::Input();
                        }
                        error=1;
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        return;
                    }
                    else{

                        for(int g=0; g<num; g++){
                            dat >> eins;
                            istringstream istr(eins);
                            istr >> otimes[g];
                        }
                        dat.close();
                    }
                }

                string orbit = "orbitelements.dat";
                std::ostringstream orbNameStream(orbit);
                orbNameStream<<path<<"/"<<orbit;
                std::string orbName = orbNameStream.str();

                QFile checkorb2(orbName.c_str());

                if(!checkorb2.exists()){
                    qDebug()<<"The file "<<checkorb2.fileName()<<" does not exist.";
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 12: File "+checkorb2.fileName()+" does not exist! Set the orbital elements via the Editor.");
                    }
                    else{
                        cout<<"Error 12: File "<<path<<"/"<<orbit<<" does not exist! Set the orbital elements via the Editor."<<endl;
                        MainWindow::Input();
                    }
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    error=1;
                   return;
                }
                else{

                    ifstream initorbit(orbName.c_str());

                    for(int i=0; i<7; i++){
                        initorbit >> eins;
                        istringstream istr1(eins);
                        istr1 >> orbele[i];
                    }
                    initorbit.close();
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
                else{
                    //
                }
            }

            RV1amin=RV1[0];
            RV1min=RV1[0];
            for(int i=0; i<num; i++){
                if(abs(RV1[i])<RV1amin){
                    RV1amin=RV1[i];
                }
                else{
                    //
                }
                if(RV1[i]<RV1min){
                    RV1min=RV1[i];
                }
                else{
                    //
                }
            }
    }

        ui->doubleSpinBox->setValue(RV1max);
        ui->doubleSpinBox_2->setValue(RV3max);

        if(checker==1){
            if(runow==0){
                QMessageBox::information(this,"Choose Input", "Error 13: Please choose input file.");
            }
            else{
                cout<<"Error 13: Please choose intput file."<<endl;
                MainWindow::Input();
            }
            this->setCursor(QCursor(Qt::ArrowCursor));
            error=1;
            return;
        }

        else if(checker==0){
            if(runow==0){
                QMessageBox::information(this,"Choose Input", "Error 14: Please choose input data from file or unknown (via global optimization).");
            }
            else{
                cout<<"Error 14: Please choose input data from file or unknown (via global optimization)."<<endl;
                MainWindow::Input();
            }
            this->setCursor(QCursor(Qt::ArrowCursor));
            error=1;
            return;
        }
        checker=0;

    //read measurements and write to solution vector

    string outM = "matrixconstr.dat";
    std::ostringstream MNameStream(outM);
    MNameStream<<path<<"/"<<outM;
    std::string MName = MNameStream.str();
    ofstream file1(MName.c_str());

    QString fext=ui->lineEdit_19->text();
    string sfext = fext.toUtf8().constData();

    //input as fits-file
    if(ui->comboBox->currentIndex()==0){
        this->setCursor(QCursor(Qt::WaitCursor));

        for(int g=mini; g<=maxi; g++){

            QString input3=ui->lineEdit_3->text();
            string data3 = input3.toUtf8().constData();
            std::ostringstream dat3NameStream(data3);
            if(ui->checkBox_27->isChecked()){
                dat3NameStream<<path<<"/"<<data3<<sequence<<"_"<<g<<sfext;
            }
            else{
                dat3NameStream<<path<<"/"<<data3<<g<<sfext;
            }
            std::string dat3Name = dat3NameStream.str();

            QFile checkfile3(dat3Name.c_str());

            if(!checkfile3.exists()){
                qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
                QString fError= QString::number(g);
                if(runow==0){
                    QMessageBox::information(this, "Error", "Error 15: Spectrum "+qPath+"/"+input3+fError+fext+" does not exist!");
                }
                else{
                    cout<<"Error 15: File "<<path<<"/"<<data3<<g<<" does not exist!"<<endl;
                MainWindow::Input();
                }
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
                return;
            }
            else{
                ifstream dat3(dat3Name.c_str());

                CCfits::FITS::setVerboseMode(true);

                try
                {

                    //open file for reading
                    shared_ptr<CCfits::FITS> input_file(new CCfits::FITS(dat3Name.c_str(),CCfits::Read,true));

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
                    if(ui->checkBox_33->isChecked() & (g==mini)){
                          if(ui->doubleSpinBox_14->value()<W[0] or (ui->doubleSpinBox_14->value()>W[logbin-1]) or (ui->doubleSpinBox_15->value()<W[0]) or (ui->doubleSpinBox_15->value()>W[logbin-1])){
                              if(runow==0){
                                  QMessageBox::information(this, "Error", "Error 16: Please check the wavelength range for residuum computation.");
                              }
                              else{
                                  cout<<"Error 16: Please check the wavelength range for residuum computation."<<endl;
                                  MainWindow::Input();
                              }
                              error=1;
                              this->setCursor(QCursor(Qt::ArrowCursor));
                              return;
                          }
                     }
                     else{
                        //
                     }
                     }
                        catch (CCfits::FitsException&)
                     {
                    std::cerr << " Fits Exception Thrown by test function \n";
                    }
                }
            }
        }

    //input as ASCII-file
    else if(ui->comboBox->currentIndex()==1){
        for(int g=mini; g<=maxi; g++){

            QString input3=ui->lineEdit_3->text();
            string data3 = input3.toUtf8().constData();
            std::ostringstream dat3NameStream(data3);
            if(ui->checkBox_27->isChecked()){
                dat3NameStream<<path<<"/"<<data3<<sequence<<"_"<<g<<sfext;
            }
            else{
                dat3NameStream<<path<<"/"<<data3<<g<<sfext;
            }
            std::string dat3Name = dat3NameStream.str();

            QFile checkfile3(dat3Name.c_str());

            if(!checkfile3.exists()){
                qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
                QString fError= QString::number(g);
                if(runow==0){
                    QMessageBox::information(this, "Error", "Error 17: Spectrum "+qPath+"/"+input3+fError+fext+" does not exist!");
                }
                else{
                    cout<<"Error 17: File "<<path<<"/"<<data3<<g<<sfext<<" does not exist!"<<endl;
                    MainWindow::Input();
                }
                this->setCursor(QCursor(Qt::ArrowCursor));
                error=1;
                return;
            }
            else{
                ifstream dat3(dat3Name.c_str());

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
                dat3.close();
            }
            /*
            if(ui->checkBox_33->isChecked() & (g==mini)){
                if(ui->doubleSpinBox_14->value()<W[0] or (ui->doubleSpinBox_14->value()>W[logbin-1]) or (ui->doubleSpinBox_15->value()<W[0]) or (ui->doubleSpinBox_15->value()>W[logbin-1])){
                    QMessageBox::information(this, "Warning", "Please check the wavelength range for residuum computation.");
                    error=1;
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    return;
                }
            }*/
        }
    }

    // sequence; compute residuum in...
    if(ui->checkBox_27->isChecked()){
        if(logbin<81){
            //
        }
        else{
            ui->checkBox_33->setChecked(true);
            ui->doubleSpinBox_14->setValue(pow(10,W(40)));
            ui->doubleSpinBox_15->setValue(pow(10,W(logbin-41)));
        }
    }
    else{
        //
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
        file1<<RV1[j]<<"\t"<<RV3[j]<<"\t"<<RV1a<<"\t"<<RV3a<<"\t"<<RV1b[j]<<"\t"<<RV3b[j]<<"\t";

        if(RV1b[j]>=0.5){
            if(RV1[j]>=0){
                RV1c[j]=RV1[j]/dv+1;
            }
            else{

            }
            if(RV1[j]<0){
                RV1c[j]=RV1[j]/dv-1;
            }
            else{

            }
        }
        else{
            RV1c[j]=RV1[j]/dv;
        }

        if(RV3b[j]>=0.5){
            if(RV3[j]>=0){
                RV3c[j]=RV3[j]/dv+1;
            }
            else{

            }
            if(RV3[j]<0){
                RV3c[j]=RV3[j]/dv-1;
            }
            else{

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
            else{

            }
            if(RV1c[j]>RV1cmax){
                RV1cmax=RV1c[j];
            }
            else{

            }
            if(RV3c[j]<RV2cmin){
                RV2cmin=RV3c[j];
            }
            else{

            }
            if(RV3c[j]>RV2cmax){
                RV2cmax=RV3c[j];
            }
            else{

            }
            if(abs(RV1c[j])>RV1m){
                RV1m = abs(RV1c[j]);
            }
            else{

            }
            if(abs(RV3c[j])>RV3m){
                RV3m = abs(RV3c[j]);
            }
            else{

            }
        }

        file1<<RV1c[j]<<"\t"<<RV3c[j]<<"\t"<<endl;
    }
    file1<<endl;
    file1<<RV1cmin<<"\t"<<RV1cmax<<"\t"<<RV2cmin<<"\t"<<RV2cmax<<"\t"<<RV1m<<"\t"<<RV3m<<endl;

    RV1a=RV1c[0];
    RV3a=RV3c[0];
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
    else{

    }

    cout<<"bso1 "<<bso1<<endl;
    cout<<"bso2 "<<bso2<<endl;

    file1<<"M is a "<<num*logbin<<" x "<<bso1+bso2<<" Matrix."<<endl;
    file1<<endl;

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
           if(runow==0){
              QMessageBox::information(this, "Error", "Error 18: Edit file does not match to specified number of observations.");
           }
           else{
               cout<<"Error 18: Edit file does not match to specified number of observations."<<endl;
               MainWindow::Input();
          }
          this->setCursor(QCursor(Qt::ArrowCursor));
          error=1;
          return;
        }
        else{

            for(int i=0; i<num_lines1; i++){
                edit >> einse;
                istringstream istr7(einse);

                if(i<num){
                    istr7>>Mtel[i];
                }
                else{
                    //
                }
                if((i>=num) & (i<2*num)){
                    istr7>>Mval1[i-num];
                }
                else{
                    //
                }
                if((i>=2*num) & (i<3*num)){
                    istr7>>Mval2[i-2*num];
                }
                else{
                    //
                }
            }
            edit.close();
        }
    }

    else {
        for(int i=0; i<num; i++){
            Mtel[i]=1.0;
            Mval2[i]=1.0/(1.0+ui->doubleSpinBox_4->value());
            Mval1[i]=ui->doubleSpinBox_4->value()/(1.0+ui->doubleSpinBox_4->value());
        }
         cout<<"Elements A:\t"<<Mval1[0]<<endl;
         cout<<"Elements B:\t"<<Mval2[0]<<endl;
         cout<<"Elements static:\t"<<Mtel[0]<<endl;
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
                        else if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        if(m<Mm-1) {
                            if(m==n-a*logbin+bso1+bso2){
                                M(n,m+1)=Mtel[a];
                            }
                            else if(n+bso1-1-a*logbin+RV3a==m){
                                M(n,m+1)=Mval2[a];
                            }
                            else M(n,m+1)=Mval1[a];
                            m=m+1;
                            ++elements;
                        }
                        else{
                            //
                        }
                        ++elements;
                    }
                    else if(bidi==0){
                        if(m==n-a*logbin+bso1+bso2){
                            M(n,m)=Mtel[a];
                        }
                        else if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        ++elements;
                    }
                    else{
                        //
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
            else if(n==logbin+a*logbin){
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
            else{
                //
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
                        else{
                            //
                        }
                        ++elements;
                    }
                    else if(bidi==0){
                        if(n-a*logbin+RV1a==m){

                            M(n,m)=Mval1[a];
                        }
                        else M(n,m)=Mval2[a];
                        ++elements;
                    }
                    else{
                        //
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
    else if(ui->checkBox_3->isChecked()){

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
                else{
                    //
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
                            else{
                                //
                            }
                            ++elements;
                        }
                        else if(bidi==0){
                            if(m==n-a*logbin+bso1){
                                M(n,m)=Mtel[a];
                            }
                            else M(n,m)=Mval1[a];
                            ++elements;
                        }
                        else{
                            //
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
                else{
                    //
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
                else{
                    //
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
                          else{
                              //
                          }
                          ++elements;
                       }
                       else if(bidi==0){
                          M(n,m)=Mval1[a];
                          ++elements;
                       }
                       else{
                           //
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

    else if(ui->checkBox_34->isChecked()){

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
            else{
                //
            }
        }
    }


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
        if(runow==0){
            QMessageBox::information(this, "Error", "Error 19: At least two observations at (hopefully) different phases are necessary.");
        }
        else{
            cout<<"Error 19: At least two observations at (hopefully) different phases are necessary."<<endl;
            MainWindow::Input();
        }
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;
    }

    else{
        MainWindow::disableButtons();
        MainWindow::read_data();
    }

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        MainWindow::enableButtons();
        return;
    }
    else{
        residu=0;
    }

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
                        file1<<setprecision(14)<<pow(10,(W(0)+(index1+RV1min/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index1;
                    }
                    else if((j>=bso1) & (j<bso1+bso2)){
                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(1.0/(fluxratio+1)-0.5);
                        //}
                        file2<<setprecision(14)<<pow(10,(W(0)+(index2+RV3min/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index2;
                    }
                    else if(j>=bso1+bso2){
                        tell<<setprecision(14)<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
                else{

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

                    file3<<setprecision(14)<<pow(10,(W(0)+(2*index1-RV1maxi/dv)*difference))<<"\t"<<X(i)<<endl;
                    //file3<<setprecision(14)<<pow(10,(W(i)-(RV1maxi/dv)*difference))<<"\t"<<X(i)<<endl;
                    ++index1;
                }
                else if((i>=bso1) & (i<bso1+bso2)){
                    file4<<setprecision(14)<<pow(10,(W(0)+(index2*2+-RV3maxi/dv)*difference))<<"\t"<<X(i)<<endl;
                    //file4<<setprecision(14)<<pow(10,(W(i)-(RV3maxi/dv)*difference))<<"\t"<<X(i)<<endl;
                    ++index2;
                }
                else if(i>=bso1+bso2){
                    tell<<setprecision(14)<<pow(10,(W(0)+2*index3*difference))<<"\t"<<X(i)<<endl;
                    //tell<<setprecision(14)<<pow(10,W(i))<<"\t"<<X(i)<<endl;
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
                file5<<setprecision(14)<<pow(10,W(i))<<" "<<res(i)-C(i)<<endl;
                residu+=pow((res(i)-C(i)),2);
            }
            else{
                //
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
        MainWindow::enableButtons();

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
                                            "Daniel P. Sablowski\n\n"
                                            "Version 1.01 2020\n\n"
                                            "It makes use of the 'Armadillo' C++ linear algebra library, OpenBLAS, CCfits and libraries therein.\n"
                                            "It uses the spline.h from https://kluge.in-chemnitz.de/opensource/spline/ \n"
                                            "It is provided AS IS WITHOUT WARRANTY of ANY KIND.\n\n"
                                            "Licensed under the Apache 2.0 licence conditions.");
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
        if(runow==0){
            QMessageBox::information(this, "Error", "Error 20: At least two observations at (hopefully) different phases are necessary.");
        }
        else{
            cout<<"Error 20: At least two observations at (hopefully) different phases are necessary."<<endl;
            MainWindow::Input();
        }
        return;
    }

    MainWindow::disableButtons();
    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        MainWindow::enableButtons();
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

    int cBS=0;

    if(ui->checkBox_5->isChecked()){
        svd_econ(U,w,V,M, "both", "std");
    }
    else ++cBS;

    if(ui->checkBox_6->isChecked()){
        svd_econ(U,w,V,M, "both", "dc");
    }
    else ++cBS;

    if(ui->checkBox_39->isChecked()){
        A = M;
        svds(U,w,V,A,Mm);
    }
    else ++cBS;

    if(cBS == 0){
        if(runow==0){
            QMessageBox::information(this, "Error", "Error 21: Select method for ECON.");
        }
        else{
            cout<<"Error 21: Select method for ECON."<<endl;
            MainWindow::Input();
        }
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
                        //file1<<setprecision(14)<<pow(10,(W(j)-((RV3maxi+orbele[4])/dv)*difference))<<"\t"<<(X(j))<<endl;
                        ++index1;
                    }
                    if((j>=bso1)&(j<bso1+bso2)){

                        file10<<setprecision(14)<<W(j)<<"\t"<<X(j)<<endl;

                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(1.0/(fluxratio+1)-0.5);
                        //}
                        file2<<setprecision(14)<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<(X(j))<<endl;
                        //file2<<setprecision(14)<<pow(10,(W(j)-((RV1maxi+orbele[4])/dv)*difference))<<"\t"<<(X(j))<<endl;
                        ++index2;
                    }
                    if(j>=bso1+bso2){

                        tell<<setprecision(14)<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        //tell<<setprecision(14)<<pow(10,(W(j)))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
            }
            int XSize= X.size();

        for(int n=0; n<XSize; n++){
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
                else if((i>=bso1) & (i<bso1+bso2)){
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
        MainWindow::enableButtons();

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
    else if(ui->comboBox->currentIndex()==1){
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
    abauto=0;

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

    if(ui->checkBox_33->isChecked()){
        LogFile<<"Compute Residuum from "<<ui->doubleSpinBox_14->value()<<" to "<<ui->doubleSpinBox_15->value()<<"."<<endl;
    }
    else{
        LogFile<<"Compute Residuum in full range."<<endl;
    }
    LogFile<<endl;

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
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 22: Edit file does not match to specified number of observations.");
                    }
                    else{
                        cout<<"Error 22: Edit file does not match to specified number of observations."<<endl;
                        MainWindow::Input();
                    }
                    error=1;
                    return;
                }

        for(int i=0; i<num_lines1; i++){
            edit >> einse;
            istringstream istr7(einse);

            if(i<num){
                istr7>>Mtel[i];
            }
            else if((i>=num) & (i<2*num)){
                istr7>>Mval1[i-num];
            }
            else if(i>=2*num){
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
    dorbele[0]=dP;
    dorbele[1]=de;
    dorbele[2]=dKA;
    dorbele[3]=dKB;
    dorbele[4]=dGamma;
    dorbele[5]=dT0;
    dorbele[6]=dOmega;

    double ym2=0, ys2=0;
    int stagnate=0;

    string sopt = "optdata.dat";
    std::ostringstream soptNameStream(sopt);
    soptNameStream<<path<<"/"<<sopt;
    std::string soptName = soptNameStream.str();
    ofstream soptd(soptName.c_str());

    LogFile<<"Step: "<<step<<"; dP: "<<dP<<"; de: "<<de<<"; dKA: "<<dKA<<"; dKB: "<<dKB<<"; dGamma: "<<dGamma<<"; dT0: "<<dT0<<"; dOmega: "<<dOmega<<endl;
    LogFile<<endl;

    // optimisation on individual RVs
    if(ui->checkBox->isChecked() or ui->checkBox_2->isChecked()){

        LogFile<<"Optimisation on individual RVs."<<endl;

        LogFile<<endl;

        double y[nu+1], Z[nu], Co[nu], So[nu], Eo[nu];
        mat P(nu+1,nu), e(nu+1,nu);

    //create new initial data
    if(ui->checkBox_9->isChecked()){
        r1=0.0;

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

            if((inrein==0) & (QIn.exists() or QIn2.exists())){
                if(runow==0){
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Warning!", "The optimisation files already exists. \n\n Do you want to overwrite it?",
                                          QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                    qDebug() << "Overwrite initial data.";
                    }
                    else{
                        qDebug()<< "Initiation aborted.";
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        MainWindow::enableButtons();
                        return;
                    }
                }
                else{
                    cout<<"Warning: The optimisation files already exists. Do you want to overwrite it? 0 no, yes 1: "<<endl;
                    cin >> overw;
                    if(overw==0){
                        this->setCursor(QCursor(Qt::ArrowCursor));
                        MainWindow::enableButtons();
                        return;
                    }
                    else{
                        qDebug() << "Overwrite initial data.";
                    }
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
        soptd<<"0\t"<<eval<<"\t"<<y[i]<<endl;
        qApp->processEvents(QEventLoop::AllEvents);
        if(upda==1){
            upda=0;
            abauto += 1;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<y[i]<<endl;
            MainWindow::plot_functionvalues();
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            else if(ui->checkBox_4->isChecked()){
                for(int i = 0; i<nu/2;i++){
                    LogFile<<RV1[i]<<"\t"<<RV3[i]<<endl;
                }
            }
            LogFile<<endl;
        }
        else {
            //
        }

        if(abortt==1){
            LogFile<<"Initiation was aborted."<<endl;
            abortt=0;
            sequence=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            MainWindow::enableButtons();
            return;
        }
        if(i==0){
            r1 = y[i];
            MainWindow::on_pushButton_6_clicked();
        }
        else if(y[i] < r1){
            r1=y[i];
            if(ui->checkBox_10->isChecked()){
                MainWindow::on_pushButton_6_clicked();
                MainWindow::enableButtons();
            }
            else{
                //
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
    else if(ui->checkBox_11->isChecked()){

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
            if(runow==0){
            QMessageBox::information(this, "Error", "Error 23: The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
            }
            else{
                cout<<"Error 23: The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
                MainWindow::Input();
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }

        int number_of_lines =0;

        while(std::getline(initiate1, zeile))
           ++ number_of_lines;

        initiate1.clear();
        initiate1.seekg(0, ios::beg);

        if(number_of_lines!=nu+1){
            qDebug()<<"Wrong optimisation file.";
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 24: The optimisation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first.");
            }
            else{
                cout<<"Error 24: The optimisation file does not exist! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
                MainWindow::Input();
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The optimisation file does not match to the data! When running optimization the first time for a given set of data, the optimization needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }

        for (int i=0; i<number_of_lines; i++){
            initiate1 >> one;
            istringstream ist(one);
            ist >> y[i];
            if(i==0) r1=y[0];
            else if(y[i]<r1){
                r1=y[i];
            }
            }
        ui->lineEdit_7->setText(QString::number(r1));
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
    else if(ui->checkBox_12->isChecked()){

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
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 25: The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
            }
            else{
                cout<<"Error 25: The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                MainWindow::Input();
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The initiation file does not exist! When running optimization the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }

        int number_of_lines =0;

        while(std::getline(initiate1, zeile))
           ++ number_of_lines;

        initiate1.clear();
        initiate1.seekg(0, ios::beg);

        if(number_of_lines!=nu+1){
            qDebug()<<"Wrong initiate file.";
            if(runow==0){
                QMessageBox::information(this, "Error", "Error 26: The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimisation needs to be initiated first.");
            }
            else{
                cout<<"Error 26: The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                MainWindow::Input();
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            LogFile<<"The initiation file does not match to the data! When running optimization the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
            MainWindow::enableButtons();
            return;
        }
        else{
            // do nothing
        }

        for (int i=0; i<number_of_lines; i++){
            initiate1 >> one;
            istringstream ist(one);
            ist >> y[i];
            if(i==0) r1=y[0];
                else if(y[i]<r1){
                    r1=y[i];
                }
            }
            initiate1.close();
            ui->lineEdit_7->setText(QString::number(r1));

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
            else{
                //
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

            string optback1 = "optval_vel_backup.dat";
            std::ostringstream back1NameStream(optback1);
            back1NameStream<<path<<"/"<<optback1;
            std::string back1Name = back1NameStream.str();
            ofstream back1(back1Name.c_str());


            string optback2 = "optmat_vel_backup.dat";
            std::ostringstream back2NameStream(optback2);
            back2NameStream<<path<<"/"<<optback2;
            std::string back2Name = back2NameStream.str();
            ofstream back2(back2Name.c_str());

            for(int i=0; i<nu+1;i++){
                //cout<<y[i]<<endl;
                back1<<setprecision(12)<<y[i]<<endl;
                for(int j=0; j<nu; j++){
                    back2<<setprecision(12)<<P(i,j)<<endl;
                }
            }

            LogFile<<endl;
            LogFile<<"Iteration: "<<t+1<<endl;
            MainWindow::plot_functionvalues();
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


        // stagnation
        if((ys==ys2) & (ym==ym2)){
            ++stagnate;

            // reinitiation activated
            if(ui->checkBox_32->isChecked()){

                // auto stop
                if(ui->checkBox_23->isChecked()){
                    if(ui->spinBox_6->value()==1){
                        // use next sequence and start optimisation
                        if(ui->checkBox_27->isChecked()){

                            LogFile<<"Finished Sequence "<<sequence<<". Setting up next sequence."<<endl;
                            LogFile<<"Using optimized RVs from last sequence."<<endl;
                            ++sequence;
                            MainWindow::on_pushButton_11_clicked(); // setting file names
                            ui->checkBox_9->setChecked(true);       // new optimisaton
                            ui->checkBox_11->setChecked(false);     // disable continue
                            ui->spinBox_3->setValue(9999);          // # iterations
                            MainWindow::on_pushButton_4_clicked();  // start optimisation

                        }
                        // stop optimisation
                        else{
                            cout<<"Optimisatin stopped; no better parameters found; stagnating."<<endl;
                            LogFile<<"Optimisatin stopped; no better parameters found; stagnating."<<endl;
                            MainWindow::enableButtons();
                            return;
                        }
                    }
                    // no auto stop applied but active
                    else{
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
                        else if(ui->checkBox_4->isChecked()){
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
                }

                // no auto stop activated, reinitiate
                else{
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
                    else if(ui->checkBox_4->isChecked()){
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

            }
            // no auto stop and reinitiation activated
            else{
                QString stagn = QString::number(stagnate);
                LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                cout<<"Optimisation may stagnate. No change in mean and STD of simplex points since "<<stagnate<<" iterations."<<endl;
                ui->plainTextEdit_2->appendPlainText("Optimisation may stagnate. No change in mean and STD of simplex points since "+stagn+" iterations.");
            }
        }

        // no stagnation
        else {
            stagnate =0;
        }

        cout<<"Stag-Index: "<<stagnate<<endl;

        ys2=ys;
        ym2=ym;

        cout<<"Simplex Mean: "<<ym<<"; Simplex STD: "<<ys<<endl;

        LogFile<<setprecision(12)<<"Mean: "<<ym<<"\tSTD: "<<ys<<endl;

        //compute centroid
        for (int j=0; j<nu; j++){
            for (int i=0; i<nu+1; i++){
                if (i!=Ph){
                    Z[j]+=P(i,j)/nu;
                }
            }
        }

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
            else if(SB1==1){
                RV1[i]=Co[i];
            }
        }
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        yi=MainWindow::DivideConquer();
        soptd<<t<<"\t"<<eval<<"\t"<<yi<<endl;
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
            else if(ui->checkBox_4->isChecked()){
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
            else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                else if(SB1==1){
                    RV1[i]=Eo[i];
                }
            }
            eval++;
            ui->spinBox_5->setValue(eval);
            qApp->processEvents(QEventLoop::AllEvents);
            MainWindow::ConstructMatrix();
            yt=MainWindow::DivideConquer();
            soptd<<t<<"\t"<<eval<<"\t"<<yt<<endl;
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
                else if(ui->checkBox_4->isChecked()){
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
                else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }
            if(yt<yl){
                cout<<"exp ";
                for (int i=0; i<nu; i++){
                    P(Ph,i)=Eo[i];
                }

            y[Ph]=yt;
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                else if(SB1==1){
                    RV1[i]=Co[i];
                }
            }

            y[Ph]=yi;
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                    else if(SB1==1){
                        RV1[i]=Co[i];
                    }
                }

            y[Ph]=yi;
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                    else if(SB1==1){
                        RV1[i]=Co[i];
                    }
                }

            y[Ph]=yi;
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
                if(ui->checkBox_3->isChecked()){
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
                    }
                }
                else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
            else if(SB1==1){
                RV1[i]=So[i];
            }
        }
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        yt=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);
        soptd<<t<<"\t"<<eval<<"\t"<<yt<<endl;
        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<yt<<endl;
            MainWindow::plot_functionvalues();
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            else if(ui->checkBox_4->isChecked()){
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
            else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                    else if(SB1==1){
                        RV1[i]=P(j,i);
                    }
                }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[j]=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);
        cout<<j+1<<" of "<<nu+1<<" points contracted."<<endl;
        soptd<<t<<"\t"<<eval<<"\t"<<y[j]<<endl;

        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New RVs found; Residuum: "<<y[j]<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<RV1[i]<<endl;
                }
            }
            else if(ui->checkBox_4->isChecked()){
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
            else if(ui->checkBox_4->isChecked()){
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
            sequence=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            MainWindow::enableButtons();
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
                else if(SB1==1){
                    RV1[i]=So[i];
                }
            }

        y[Ph]=yt;
        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best RVs:"<<endl;
            if(ui->checkBox_3->isChecked()){
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
                }
            }
            else if(ui->checkBox_4->isChecked()){
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
                sequence=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
        else if(ui->checkBox_4->isChecked()){
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

      //optimization on orbit**********************************************************
      else{

              nu=7;
              int add=0;

              QVector<int> pvf(7);

              if(ui->checkBox_18->isChecked()){ // P
                  pvf[0]=1; // fixed
                  LogFile<<"P locked ";
                  ++add;
              }
              else{
                  pvf[0]=0; // free
              }
              if(ui->checkBox_17->isChecked()){ // e
                  pvf[1]=1;
                  LogFile<<"e locked ";
                  ++add;
              }
              else{
                  pvf[1]=0;
              }
              if(ui->checkBox_31->isChecked()){ // KA
                  pvf[2]=1;
                  LogFile<<"KA locked ";
                  ++add;
              }
              else{
                  pvf[2]=0;
              }
              if(ui->checkBox_36->isChecked()){ // KB
                  pvf[3]=1;
                  LogFile<<"KB locked ";
                  ++add;
              }
              else{
                  pvf[3]=0;
              }
              if(ui->checkBox_37->isChecked()){ // Gamma
                  pvf[4]=1;
                  LogFile<<"Gamma locked ";
                  ++add;
              }
              else{
                  pvf[4]=0;
              }
              if(ui->checkBox_35->isChecked()){ // T0
                  pvf[5]=1;
                  LogFile<<"T0 locked ";
                  ++add;
              }
              else{
                  pvf[5]=0;
              }
              if(ui->checkBox_38->isChecked()){ // Omega
                  pvf[6]=1;
                  LogFile<<"Omega locked ";
                  ++add;
              }
              else{
                  pvf[6]=0;
              }

              LogFile<<"Optimisation on orbital parameters."<<endl;

              nu = nu-add;
              //cout<<"add: "<<add<<endl;

              LogFile<<endl;

              double y[nu+1], P[nu+1][nu], Z[nu], Co[nu], So[nu], Eo[nu], e[nu+1][nu];
              QVector<double> porbele(nu), ddorbele(nu);
              int pcount=0;

              for(int i=0; i<nu+add; i++){
                  if(pvf[i]==0){
                      porbele[pcount]=orbele[i];
                      ddorbele[pcount]=dorbele[i];
                      ++pcount;
                  }
                  else{
                      //
                  }
              }
              pcount=0;

              //create new initial data
              if(ui->checkBox_9->isChecked()){
                  r1=0.0;

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

                    if((inrein==0) & (QIn.exists() or QIn2.exists())){
                        if(runow==0){
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
                                MainWindow::enableButtons();
                                return;
                            }
                        }
                        else{
                            cout<<"Warning: The optimisation files already exists. Do you want to overwrite it? 0 no, yes 1: "<<endl;
                            cin >> overw;
                            if(overw==0){
                                qDebug()<< "Initiation aborted.";
                                this->setCursor(QCursor(Qt::ArrowCursor));
                                MainWindow::enableButtons();
                                return;
                            }
                            else{
                                qDebug() << "Overwrite initial data.";
                                LogFile<<"Create new initial simplex with "<<nu+1<<" points."<<endl;
                                LogFile<<"Elements:"<<endl;
                            }
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

                  for(int i=0; i<7; i++){
                      if(pvf[i]==0){
                          P[0][pcount]=porbele[pcount];
                          cout<<P[0][pcount]<<" ";
                          LogFile<<P[0][pcount]<<" ";
                          ++pcount;
                      }
                      else{
                          //
                      }
                  }
                  cout<<endl;
                  LogFile<<endl;
                  pcount=0;

                  for(int i=0; i<nu+1; i++){
                      for(int j=0; j<nu; j++){
                          if((i>0)&(i==j+1)){
                              e[i][j]=ddorbele[i-1];
                          }
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
                      }
                      LogFile<<endl;

                      for(int e=0; e<7; e++){
                          if(pvf[e]==0){
                              orbele[e]=P[i][pcount];
                              ++pcount;
                          }
                          else{
                              //
                          }
                      }
                      pcount=0;

                      eval++;
                      ui->spinBox_5->setValue(eval);
                      qApp->processEvents(QEventLoop::AllEvents);
                      MainWindow::VAmplitudeA();
                      MainWindow::VAmplitudeB();
                      MainWindow::ConstructMatrix();
                      y[i]=MainWindow::DivideConquer();
                      qApp->processEvents(QEventLoop::AllEvents);
                      soptd<<"0\t"<<eval<<"\t"<<y[i]<<endl;

                      if(upda==1){
                          upda=0;
                          LogFile<<endl;
                          LogFile<<"New Parameters found."<<endl;
                          LogFile<<"Residuum: "<<y[i]<<endl;
                          LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                          LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                          LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                          LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                          LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                          LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                          LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                          LogFile<<endl;
                      }

                      LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;

                      if(abortt==1){
                          abortt=0;
                          sequence=0;
                          LogFile<<"Initiation aborted."<<endl;
                          this->setCursor(QCursor(Qt::ArrowCursor));
                          MainWindow::enableButtons();
                          return;
                      }
                      if(i==0){
                          r1=y[0];
                          MainWindow::on_pushButton_6_clicked();
                      }
                      else if(y[i] < r1){
                        r1 = y[i];
                        if(ui->checkBox_10->isChecked()){
                            MainWindow::on_pushButton_6_clicked();
                        }
                      }

                      cout<<setprecision(12)<<y[i]<<endl;
                      init<<setprecision(14)<<y[i]<<endl;
                  }
              }

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
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                          MainWindow::Input();
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The optimisation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }

                  int number_of_lines =0;

                  while(std::getline(initiate1, zeile))
                     ++ number_of_lines;

                  initiate1.clear();
                  initiate1.seekg(0, ios::beg);

                  if(number_of_lines!=nu+1){
                      qDebug()<<"Wrong optimisation file.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The optimisation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The optimisation file does match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                          MainWindow::Input();
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The optimisation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }

                  for (int i=0; i<number_of_lines; i++){
                    initiate1 >> one;
                    istringstream ist(one);
                    ist >> y[i];
                    if(i==0){
                        r1=y[0];
                    }
                    else if(y[i]<r1){
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

                  QFile checkfile(file2Name.c_str());

                  if(!checkfile.exists()){
                      qDebug()<<"The initiation file "<<checkfile.fileName()<<" does not exist.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                          MainWindow::Input();
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      LogFile<<"The initiation file does not exist! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }
                  ifstream initiate2(file2Name.c_str());

                  int number_of_lines =0;

                  while(std::getline(initiate1, zeile))
                     ++ number_of_lines;

                  initiate1.clear();
                  initiate1.seekg(0, ios::beg);

                  if(number_of_lines!=nu+1){
                      qDebug()<<"Wrong initiate file.";
                      if(runow==0){
                        QMessageBox::information(this, "Error", "The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first.");
                      }
                      else{
                          cout<<"The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      }
                     this->setCursor(QCursor(Qt::ArrowCursor));
                      cout<<number_of_lines<<endl;
                      LogFile<<"The initiation file does not match to the data! When running optimisation the first time for a given set of data, the optimisation needs to be initiated first."<<endl;
                      MainWindow::enableButtons();
                      return;
                  }

                  for (int i=0; i<number_of_lines; i++){
                    initiate1 >> one;
                    istringstream ist(one);
                    ist >> y[i];
                    if(i==0){
                        r1=y[0];
                    }
                    else if(y[i]<r1){
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

                      string optback1 = "optval_vel_backup.dat";
                      std::ostringstream back1NameStream(optback1);
                      back1NameStream<<path<<"/"<<optback1;
                      std::string back1Name = back1NameStream.str();
                      ofstream back1(back1Name.c_str());


                      string optback2 = "optmat_vel_backup.dat";
                      std::ostringstream back2NameStream(optback2);
                      back2NameStream<<path<<"/"<<optback2;
                      std::string back2Name = back2NameStream.str();
                      ofstream back2(back2Name.c_str());

                      for(int i=0; i<nu+1;i++){
                          //cout<<y[i]<<endl;
                          back1<<setprecision(12)<<y[i]<<endl;
                          for(int j=0; j<nu; j++){
                              back2<<setprecision(12)<<P[i][j]<<endl;
                          }
                      }

                      LogFile<<endl;

                      LogFile<<"Iteration: "<<t+1<<"; ";
                      MainWindow::plot_functionvalues();
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

                          // reinitiation
                          if(ui->checkBox_32->isChecked()){

                              //auto stop
                              if(ui->checkBox_23->isChecked()){
                                  if(ui->spinBox_6->value()==1){
                                      if(ui->checkBox_27->isChecked()){ // use next sequence
                                          LogFile<<"Finished Sequence "<<sequence<<". Setting up next sequence."<<endl;
                                          LogFile<<"Using optimized RVs from last sequence."<<endl;
                                          ++sequence;
                                          MainWindow::on_pushButton_11_clicked(); // setting file names
                                          ui->checkBox_9->setChecked(true);       // new optimisaton
                                          ui->checkBox_11->setChecked(false);     // disable continue
                                          ui->spinBox_3->setValue(9999);          // # iterations
                                          MainWindow::on_pushButton_4_clicked();  // start optimisation
                                      }
                                      else{ // no sequence specified, stop optimization
                                          cout<<"Optimisatin stopped; no better parameters found; stagnating."<<endl;
                                          LogFile<<"Optimisatin stopped; no better parameters found; stagnating."<<endl;
                                          MainWindow::enableButtons();
                                          return;
                                      }
                                  }
                                  // no auto stop applied but active
                                  else{
                                      cout<<"Reinitiate optimisation with current best orbit."<<endl;
                                      ui->plainTextEdit_2->appendPlainText("Reinitiate optimisation with current best orbit.");
                                      LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                                      LogFile<<endl;
                                      LogFile<<"Reinitiate optimisation with current best orbit."<<endl;

                                      string filerein = "orbitelements.dat";
                                      std::ostringstream reinNameStream(filerein);
                                      reinNameStream<<path<<"/"<<filerein;
                                      std::string reinName = reinNameStream.str();
                                      ofstream borbit(reinName.c_str());

                                      for(int e=0; e<7; e++){
                                          if(pvf[e]==0){
                                              borbit<<setprecision(14)<<P[Pl][pcount]<<endl;
                                              ++pcount;
                                          }
                                          else{
                                              borbit<<setprecision(14)<<orbele[e]<<endl;
                                          }
                                      }
                                      pcount=0;
                                      ui->checkBox_9->setChecked(true);
                                      ui->checkBox_11->setChecked(false);
                                      ui->checkBox_12->setChecked(false);

                                      reinitiate=1;
                                      return;
                                  }
                              }

                              // no auto stop activated, reinitiate
                              else{
                                cout<<"Reinitiate optimisation with current best orbit."<<endl;
                                ui->plainTextEdit_2->appendPlainText("Reinitiate optimisation with current best orbit.");
                                LogFile<<"Stagnation of DSM: "<<stagnate<<endl;
                                LogFile<<endl;
                                LogFile<<"Reinitiate optimisation with current best orbit."<<endl;

                                string filerein = "orbitelements.dat";
                                std::ostringstream reinNameStream(filerein);
                                reinNameStream<<path<<"/"<<filerein;
                                std::string reinName = reinNameStream.str();
                                ofstream borbit(reinName.c_str());

                                for(int e=0; e<7; e++){
                                    if(pvf[e]==0){
                                        borbit<<setprecision(14)<<P[Pl][pcount]<<endl;
                                        ++pcount;
                                    }
                                    else{
                                        borbit<<setprecision(14)<<orbele[e]<<endl;
                                    }
                                }
                                pcount=0;
                                ui->checkBox_9->setChecked(true);
                                ui->checkBox_11->setChecked(false);
                                ui->checkBox_12->setChecked(false);

                                reinitiate=1;
                                return;
                            }
                        }
                          // no reinitiated or auto stop active
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
                      }
                      for(int e=0; e<7; e++){
                          if(pvf[e]==0){
                              orbele[e]=Co[pcount];
                              ++pcount;
                          }
                          else{
                              //
                          }
                      }
                      pcount=0;
                      eval++;
                      ui->spinBox_5->setValue(eval);
                      qApp->processEvents(QEventLoop::AllEvents);
                      MainWindow::VAmplitudeA();
                      MainWindow::VAmplitudeB();
                      MainWindow::ConstructMatrix();
                      yi=MainWindow::DivideConquer();
                      qApp->processEvents(QEventLoop::AllEvents);
                      soptd<<t<<"\t"<<eval<<"\t"<<yi<<endl;

                      if(upda==1){
                          upda=0;
                          LogFile<<endl;
                          LogFile<<"New Parameters found."<<endl;
                          LogFile<<"Residuum: "<<yi<<endl;
                          LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                          LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                          LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                          LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                          LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                          LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                          LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                          LogFile<<endl;
                      }

                      if(abortt==1){
                          //output results
                          LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                              for(int i = 0; i<nu;i++){
                                  LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                          sequence=0;
                          this->setCursor(QCursor(Qt::ArrowCursor));
                          MainWindow::enableButtons();
                          return;
                      }
                      cout<<"reflection 1"<<endl;

                      //Alpha Branch, better than best point=So[i]=So[i]
                      if(yi<yl){
                          cout<<"expansion"<<endl;
                          LogFile<<"yi<yl, expansion; ";
                          for(int i=0; i<nu; i++){
                              Eo[i]=Z[i]+Gamma*(Co[i]-Z[i]);
                          }
                          for(int e=0; e<7; e++){
                              if(pvf[e]==0){
                                  orbele[e]=Eo[pcount];
                                  ++pcount;
                              }
                              else{
                                  //
                              }
                          }
                          pcount=0;
                          eval++;
                          ui->spinBox_5->setValue(eval);
                          qApp->processEvents(QEventLoop::AllEvents);
                          MainWindow::VAmplitudeA();
                          MainWindow::VAmplitudeB();
                          MainWindow::ConstructMatrix();
                          yt=MainWindow::DivideConquer();
                          soptd<<t<<"\t"<<eval<<"\t"<<yt<<endl;
                          qApp->processEvents(QEventLoop::AllEvents);

                          if(upda==1){
                              upda=0;
                              LogFile<<endl;
                              LogFile<<"New Parameters found."<<endl;
                              LogFile<<"Residuum: "<<yt<<endl;
                              LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                              LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                              LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                              LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                              LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                              LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                              LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                              LogFile<<endl;
                          }

                          if(abortt==1){
                              //output results
                              LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                  for(int i = 0; i<nu;i++){
                                      LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                              sequence=0;
                              this->setCursor(QCursor(Qt::ArrowCursor));
                              MainWindow::enableButtons();
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
                              }
                              for(int e=0; e<7; e++){
                                  if(pvf[e]==0){
                                      orbele[e]=Co[pcount];
                                      ++pcount;
                                  }
                                  else{
                                      //
                                  }
                              }
                              pcount=0;

                              y[Ph]=yi;//MainWindow::DivideConquer();
                              if(abortt==1){
                                  //output results
                                  LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                      for(int i = 0; i<nu;i++){
                                          LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                                  sequence=0;
                                  this->setCursor(QCursor(Qt::ArrowCursor));
                                  MainWindow::enableButtons();
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

                              y[Ph]=yi;//MainWindow::DivideConquer();
                              if(abortt==1){
                                  //output results
                                  LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                      for(int i = 0; i<nu;i++){
                                          LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                                  sequence=0;
                                  this->setCursor(QCursor(Qt::ArrowCursor));
                                  MainWindow::enableButtons();
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

                                  y[Ph]=yi;//MainWindow::DivideConquer();
                                  if(abortt==1){
                                      //output results
                                      LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                          for(int i = 0; i<nu;i++){
                                              LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                                      sequence=0;
                                      this->setCursor(QCursor(Qt::ArrowCursor));
                                      MainWindow::enableButtons();
                                      return;
                                  }
                                  qApp->processEvents(QEventLoop::AllEvents);
                                  yh=y[Ph];
                              }
                              for(int i=0; i<nu; i++){
                                  So[i]=Z[i]+beta*(P[Ph][i]-Z[i]);
                              }
                                  for(int e=0; e<7; e++){
                                      if(pvf[e]==0){
                                          orbele[e]=So[pcount];
                                          ++pcount;
                                      }
                                      else{
                                          //
                                      }
                                  }
                                  pcount=0;
                              eval++;
                              ui->spinBox_5->setValue(eval);
                              qApp->processEvents(QEventLoop::AllEvents);
                              MainWindow::VAmplitudeA();
                              MainWindow::VAmplitudeB();
                              MainWindow::ConstructMatrix();
                              yt=MainWindow::DivideConquer();
                              soptd<<t<<"\t"<<eval<<"\t"<<yt<<endl;
                              qApp->processEvents(QEventLoop::AllEvents);

                              if(upda==1){
                                  upda=0;
                                  LogFile<<endl;
                                  LogFile<<"New Parameters found."<<endl;
                                  LogFile<<"Residuum: "<<yt<<endl;
                                  LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                                  LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                                  LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                                  LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                                  LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                                  LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                                  LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                                  LogFile<<endl;
                              }

                              if(abortt==1){
                                  //output results
                                  LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                      for(int i = 0; i<nu;i++){
                                          LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                                  sequence=0;
                                  this->setCursor(QCursor(Qt::ArrowCursor));
                                  MainWindow::enableButtons();
                                  return;
                              }

                              //Beta_1_1 branch
                              if(yt>yh){
                                  cout<<"yt <yh tot ";
                                  LogFile<<"yt>yh, total contraction; ";
                                  for(int j=0; j<nu+1; j++){
                                      for(int i =0; i<nu; i++){
                                          P[j][i]=P[Pl][i]+btot*(P[j][i]-P[Pl][i]);

                                          for(int e=0; e<7; e++){
                                              if(pvf[e]==0){
                                                  orbele[e]=P[j][pcount];
                                                  ++pcount;
                                              }
                                              else{
                                                  //
                                              }
                                          }
                                          pcount=0;
                                      }
                                      eval++;
                                      ui->spinBox_5->setValue(eval);
                                      qApp->processEvents(QEventLoop::AllEvents);
                                      MainWindow::VAmplitudeA();
                                      MainWindow::VAmplitudeB();
                                      MainWindow::ConstructMatrix();
                                      y[j]=MainWindow::DivideConquer();
                                      soptd<<t<<"\t"<<eval<<"\t"<<y[j]<<endl;
                                      qApp->processEvents(QEventLoop::AllEvents);
                                      cout<<j+1<<" of "<<nu+1<<" points contracted."<<endl;

                                      if(upda==1){
                                          upda=0;
                                          LogFile<<endl;
                                          LogFile<<"New Parameters found."<<endl;
                                          LogFile<<"Residuum: "<<y[j]<<endl;
                                          LogFile<<setprecision(14)<<"P: "<<orbele[0]<<endl;
                                          LogFile<<setprecision(14)<<"e: "<<orbele[1]<<endl;
                                          LogFile<<setprecision(14)<<"KA: "<<orbele[2]<<endl;
                                          LogFile<<setprecision(14)<<"KB: "<<orbele[3]<<endl;
                                          LogFile<<setprecision(14)<<"Gamma: "<<orbele[4]<<endl;
                                          LogFile<<setprecision(14)<<"T0: "<<orbele[5]<<endl;
                                          LogFile<<setprecision(14)<<"Omega: "<<orbele[6]<<endl;
                                          LogFile<<endl;
                                      }

                                      if(abortt==1){
                                          //output results
                                          LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                              for(int i = 0; i<nu;i++){
                                                  LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                                          sequence=0;
                                          this->setCursor(QCursor(Qt::ArrowCursor));
                                          MainWindow::enableButtons();
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
                                  }
                                  for(int e=0; e<7; e++){
                                      if(pvf[e]==0){
                                          orbele[e]=So[pcount];
                                          ++pcount;
                                      }
                                      else{
                                          //
                                      }
                                  }
                                  pcount=0;
                                  y[Ph]=yt;//MainWindow::DivideConquer();
                                  if(abortt==1){
                                      //output results
                                      LogFile<<"Optimisation aborted. Current best Elements:"<<endl;
                                          for(int i = 0; i<nu;i++){
                                              LogFile<<setprecision(14)<<P[Pl][i]<<endl;
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
                                      sequence=0;
                                      this->setCursor(QCursor(Qt::ArrowCursor));
                                      MainWindow::enableButtons();
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

                  for(int e=0; e<7; e++){
                      if(pvf[e]==0){
                          orbele[e]=P[Pl][pcount];
                          ++pcount;
                      }
                  }
                  pcount=0;

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
                          cout<<setprecision(14)<<orbele[i]<<";\t";
                          LogFile<<setprecision(14)<<orbele[i]<<";\t";
                      }

                cout<<endl;
                LogFile<<endl;

          } // end optimization on orbit


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

    r1=1.0;

    this->setCursor(QCursor(Qt::ArrowCursor));
}

//***************************************
// Plot residuum vs evaluations and interations
//***************************************
void MainWindow::plot_functionvalues(){


    string fvalues = "optdata.dat";
    std::ostringstream valuesNameStream(fvalues);
    valuesNameStream<<path<<"/"<<fvalues;
    std::string valuesName = valuesNameStream.str();


    QFile checkfile1(valuesName.c_str());

    if(!checkfile1.exists()){
        qDebug()<<"Error 2: The file "<<checkfile1.fileName()<<" does not exist.";
    }
    else{
        ifstream fvals(valuesName.c_str());

        int pointsi=0;
        string oeins, ozwei, odrei, oline;

        while(std::getline(fvals, oline))
        ++ pointsi;

        fvals.clear();
        fvals.seekg(0, ios::beg);

        QVector<double> oiter(pointsi), oeval(pointsi), ovals(pointsi);

        for (int i =0; i < pointsi; i++){
            fvals >> oeins >> ozwei >> odrei;
            istringstream istr7(oeins);
            istr7 >> oiter[i];
            istringstream istr8(ozwei);
            istr8 >> oeval[i];
            istringstream istr9(odrei);
            istr9 >> ovals[i];
        }
        fvals.close();

        ui->customPlot_4->clearGraphs();
        ui->customPlot_4->addGraph();
        ui->customPlot_4->graph(0)->addData(oiter, ovals);
        ui->customPlot_4->rescaleAxes();
        ui->customPlot_4->replot();
    }


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
            else{
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
            else{
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
            else if(RV1c[j]>RV1cmax){
                RV1cmax=RV1c[j];
            }
            if(RV3c[j]<RV2cmin){
                RV2cmin=RV3c[j];
            }
            else if(RV3c[j]>RV2cmax){
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
                        else if((n+bso1-1-a*logbin+RV3a==m) & (m>=bso1)){
                            M(n,m)=Mval2[a];
                        }
                        else M(n,m)=Mval1[a];
                        if(m<Mm-1) {
                            if(m==n-a*logbin+bso1+bso2){
                                M(n,m+1)=Mtel[a];
                            }
                        else if(n+bso1-1-a*logbin+RV3a==m){
                            M(n,m+1)=Mval2[a];
                        }
                        else M(n,m+1)=Mval1[a];
                        m=m+1;
                        ++elements;
                        }
                        ++elements;
                    }
                    else if(bidi==0){
                        if(m==n-a*logbin+bso1+bso2){
                            M(n,m)=Mtel[a];
                        }
                        else if(n+bso1-1-a*logbin+RV3a==m){
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
                        if(m<Mm-1){
                            if((n+bso1-1-a*logbin+RV3a==m) & (m>=bso1)){
                                M(n,m+1)=Mval2[a];
                            }
                            else M(n,m+1)=Mval1[a];
                            m=m+1;
                            ++elements;
                        }
                        ++elements;
                    }
                    else if(bidi==0){
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

            else if(RV1[i]>RV1maxi){
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
            else{
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
            else if(RV1c[j]>RV1cmax){
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
                        else if(bidi==0){
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
                else if(n==logbin+a*logbin){
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
                        else if(bidi==0){
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

    if(ui->checkBox_5->isChecked()){
        svd_econ(U,w,V,M, "both", "std");
    }
    else{
        svd_econ(U,w,V,M, "both", "dc");
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
        if(((residu<r1) or (r1==0.0)) and (optfratios==0)){

            r1=residu;
            upda=1;

            for(int i=0; i<num; i++){
                QString index= QString::number(i);
                if(ui->checkBox_3->isChecked()){
                    QString velocity=QString::number(RV1[i]);
                    ui->plainTextEdit->appendPlainText(index+" "+velocity);
                }
                else if(ui->checkBox_4->isChecked()){
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

                        file1<<setprecision(14)<<pow(10,(W(0)+(index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index1;
                    }
                    else if((j>=bso1)&(j<bso1+bso2)){
                        //if(ui->checkBox_4->isChecked()){
                        //    X(j)=X(j)+(1.0/(fluxratio+1)-0.5);
                        //}
                        file2<<setprecision(14)<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index2;
                    }
                    else if(j>=bso1+bso2){
                        tell<<setprecision(14)<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
            }

            int XSize=X.size();
        for(int n=0; n<XSize; n++){
            file7<<n<<" "<<X(n)<<endl;
        }
        index1=0;
        index2=0;
        index3=0;

        int ressize=res.size();
        for(int i=0; i<ressize; i++){
                file5<<setprecision(14)<<pow(10,W(i))<<" "<<(res(i)-C(i))<<endl;
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

                        upda2<<setprecision(14)<<pow(10,(W(0)+(index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index1;
                    }
                    else if((j>=bso1)&(j<bso1+bso2)){
                        upda3<<setprecision(14)<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                        ++index2;
                    }
                    else if(j>=bso1+bso2){
                        upda5<<setprecision(14)<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                        ++index3;
                    }
                }
            }

            index1=0;
            index2=0;
            index3=0;

            ressize=res.size();
            for(int i=0; i<ressize; i++){
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
                    file3<<setprecision(14)<<pow(10,(W(0)+(2*index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<Xr[index3]<<endl;
                    ++index1;
                }
                else if((i>=bso1) & (i<bso1+bso2)){
                    file4<<setprecision(14)<<pow(10,(W(0)+(2*index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<Xr[index3]<<endl;
                    ++index2;
                }
                ++i;
                ++index3;
                }
            }
        }
        else{
            // write to files to determine inclinations
            if(optfratios==1){

                QVector<double> err(logbin), errmean(logbin);

                string erro = "frerror.dat";
                std::ostringstream upd1NameStream(erro);
                upd1NameStream<<path<<"/"<<erro;
                std::string upd1Name = upd1NameStream.str();
                ofstream upda1(upd1Name.c_str());

                string sCA = "frcompa.dat";
                std::ostringstream upd2NameStream(sCA);
                upd2NameStream<<path<<"/"<<sCA;
                std::string upd2Name = upd2NameStream.str();
                ofstream upda2(upd2Name.c_str());

                string sCB = "frcompb.dat";
                std::ostringstream upd3NameStream(sCB);
                upd3NameStream<<path<<"/"<<sCB;
                std::string upd3Name = upd3NameStream.str();
                ofstream upda3(upd3Name.c_str());

                string sTel = "frtellur.dat";
                std::ostringstream upd5NameStream(sTel);
                upd5NameStream<<path<<"/"<<sTel;
                std::string upd5Name = upd5NameStream.str();
                ofstream upda5(upd5Name.c_str());

                string sDif = "frdifferences.dat";
                std::ostringstream upd4NameStream(sDif);
                upd4NameStream<<path<<"/"<<sDif;
                std::string upd4Name = upd4NameStream.str();
                ofstream upda4(upd4Name.c_str());

                for(int i =0; i<logbin;i++){
                    errmean[i]=0;
                    for(int n = 0; n<num; n++){
                        errmean[i]+=(res(i+n*logbin)-C(i+n*logbin))/num;

                    }
                    upda1<<pow(10,W(i))<<"\t"<<errmean[i]<<endl;
                    errmean[i]=0;
                }

                for (j=0;j<Mm;j++) {
                     if(abs(X(j))>10*tsh){
                        if(j<bso1){

                            upda2<<setprecision(14)<<pow(10,(W(0)+(index1-(RV3maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                            ++index1;
                        }
                        else if((j>=bso1)&(j<bso1+bso2)){
                            upda3<<setprecision(14)<<pow(10,(W(0)+(index2-(RV1maxi+orbele[4])/dv)*difference))<<"\t"<<X(j)<<endl;
                            ++index2;
                        }
                        else if(j>=bso1+bso2){
                            upda5<<setprecision(14)<<pow(10,(W(0)+index3*difference))<<"\t"<<X(j)<<endl;
                            ++index3;
                        }
                    }
                }
                index1=0;
                index2=0;
                index3=0;

                int ressize=res.size();
                for(int i=0; i<ressize; i++){
                        upda4<<pow(10,W(i))<<" "<<(res(i)-C(i))<<endl;
                }

                // use function slope to optimise
                if(ui->checkBox_30->isChecked()){
                    residu=MainWindow::inclinations();
                }
                // use residu, calculated above, to optimise
                else{
                    //
                }

                if(residu<r1){
                    r1=residu;
                    upda=1;
                    cout<<"Better flux ratios found."<<endl;
                    for(int i=0; i<num; i++){
                        cout<<Mval1[i]<<"\t"<<Mval2[i]<<endl;
                    }
                    ui->spinBox_6->setValue(eval);

                    QString residual=QString::number(residu);
                    ui->lineEdit_7->setText(residual);
                }
                if(r1==0.0){
                    r1=residu;
                    upda=1;
                }

            }
            else{

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
    if((runow==0) & (optfratios==0)){       //plot only of in GUI-mode
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

        QFile checkfile(datName.c_str());

        if(!checkfile.exists()){
            qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+plot1+" does not exist!");
            return;
        }
        ifstream toplot1(datName.c_str());

        QString plot2=ui->lineEdit_8->text();
        string plot12 = plot2.toUtf8().constData();
        std::ostringstream dat2NameStream(plot12);
        dat2NameStream<<path<<"/"<<plot12;
        std::string dat2Name = dat2NameStream.str();

        QFile checkfile2(dat2Name.c_str());

        if(!checkfile2.exists()){
            qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+plot2+" does not exist!");
            return;
        }
        ifstream toplot2(dat2Name.c_str());

        QString qError=ui->lineEdit_20->text();
        string errorf=qError.toUtf8().constData();
        std::ostringstream dat4NameStream(errorf);
        dat4NameStream<<path<<"/"<<errorf;
        std::string dat4Name = dat4NameStream.str();  

        QString qDiff="diffstatistic.dat";
        string sdiff=qDiff.toUtf8().constData();
        std::ostringstream dat5NameStream(sdiff);
        dat5NameStream<<path<<"/"<<sdiff;
        std::string dat5Name = dat5NameStream.str();
        ofstream diffs(dat5Name.c_str());

        QFile checkfile4(dat4Name.c_str());

        if(!checkfile4.exists()){
            qDebug()<<"The file "<<checkfile4.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+qError+" does not exist!");
            return;
        }
        ifstream errors(dat4Name.c_str());

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
            ++number_of_lines;

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

        QFile checkfile3(datName.c_str());

        if(!checkfile3.exists()){
            qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qPath+"/"+plot3+" does not exist!");
            return;
        }

        ifstream toplot3(dat3Name.c_str());

        number_of_lines=0;

        while(std::getline(toplot3, zeile))
        ++number_of_lines;
        cout<<number_of_lines<<endl;

        toplot3.clear();
        toplot3.seekg(0, ios::beg);

        double offset = ui->doubleSpinBox_5->value();
        int index = 0, count=0;
        double sres=0.0, sres2=0.0, reasm=0.0;

        a.resize(number_of_lines);
        b.resize(number_of_lines);

        for (int i=0; i<number_of_lines; i++){
            toplot3 >> one >>two;
            istringstream ist(one);
            ist >> a[i];
            istringstream ist2(two);
            ist2 >> b[i];
            if(i>0){
                if((sres!=0) & (a[i]<a[i-1])){
                    diffs<<sres<<"\t"<<sres2<<"\t"<<reasm<<"\t"<<count<<endl;
                }
                else{
                    //
                }
                reasm=sres/count;
                sres = 0;
                sres2 = 0;
                count=0;
                ++index;
            }
            else{
                //
            }
            if(ui->checkBox_33->isChecked()){
                if((a[i]>ui->doubleSpinBox_14->value()) & (a[i]<ui->doubleSpinBox_15->value())){
                    sres+=b[i];
                    sres2+=pow(b[i],2);
                    ++count;
                }
                else{
                    // do nothing...
                }
            }
            else{
                sres+=b[i];
                sres2+=pow(b[i],2);
                ++count;
            }
            //b[i]=b[i]+index*offset;
        }
        if(sres!=0){
            diffs<<sres<<"\t"<<sres2<<"\t"<<reasm<<"\t"<<count<<endl;
        }
        else{
            //
        }
        cout<<"end"<<endl;

        diffs.close();
        toplot3.close();

        ifstream indiffs(dat5Name.c_str());

        string three, four;
        QVector<double> vreasm(index), vsres(index), vsres2(index), vcount(index);

        for (int i=0; i<index; i++){
            indiffs >> one >>two>>three>>four;
            istringstream ist(one);
            ist >> vsres[i];
            istringstream ist2(two);
            ist2 >> vsres2[i];
            istringstream ist3(three);
            ist3 >> vreasm[i];
            istringstream ist4(four);
            ist4 >> vcount[i];
        }
        indiffs.close();
        sres=0.0;
        sres2=0.0;
        count=0;
        index=0;
        ofstream diffs2(dat5Name.c_str());

        for(int i=0; i<number_of_lines; i++){
            if(i>0){
                if(a[i]<a[i-1]){
                reasm=sres/count;
                diffs2<<sres<<"\t"<<vsres[index]<<"\t"<<sres2<<"\t"<<vsres2[index]<<"\t"<<reasm<<"\t"<<count<<endl;
                sres = 0.0;
                sres2 = 0.0;
                count=0;
                ++index;
                }
                else {
                    // do nothing
                }
            }
            else  {
                // do nothing
            }
            if(ui->checkBox_33->isChecked()){
                if((a[i]>ui->doubleSpinBox_14->value()) & (a[i]<ui->doubleSpinBox_15->value())){
                    sres+=vreasm[index]-b[i];
                    sres2+=pow((vreasm[index]-b[i]),2);
                    ++count;
                }
                else{
                    // do nothing...
                }
            }
            else{
                sres+=b[i];
                sres2+=pow(b[i],2);
                ++count;
            }
            b[i]=b[i]+index*offset;
        }
         diffs2<<sres<<"\t"<<vsres[index]<<"\t"<<sres2<<"\t"<<vsres2[index]<<"\t"<<reasm<<"\t"<<count<<endl;
         diffs2.close();

        ui->customPlot_3->addGraph();
        ui->customPlot_3->graph(0)->setData(a, b);
        ui->customPlot_3->graph(0)->setPen(QPen(Qt::red));
        ui->customPlot_3->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->customPlot_3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
        ui->customPlot_3->xAxis->setRange(xs1, xs2);
        ui->customPlot_3->yAxis->rescale(true);
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

    // plot files flux optimisation
    else{
        if((runow==0) & (optfratios==1)){
            ui->customPlot->clearGraphs();
            ui->customPlot_2->clearGraphs();
            ui->customPlot_3->clearGraphs();

            double xs1, xs2, ys1, ys2;
            string zeile, one, two;
            QString plot1="frcompa.dat";
            string plot11 = plot1.toUtf8().constData();
            std::ostringstream datNameStream(plot11);
            datNameStream<<path<<"/"<<plot11;
            std::string datName = datNameStream.str();

            QFile checkfile(datName.c_str());

            if(!checkfile.exists()){
                qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+plot1+" does not exist!");
                return;
            }
            ifstream toplot1(datName.c_str());

            QString plot2="frcompb.dat";
            string plot12 = plot2.toUtf8().constData();
            std::ostringstream dat2NameStream(plot12);
            dat2NameStream<<path<<"/"<<plot12;
            std::string dat2Name = dat2NameStream.str();

            QFile checkfile2(dat2Name.c_str());

            if(!checkfile2.exists()){
                qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+plot2+" does not exist!");
                return;
            }
            ifstream toplot2(dat2Name.c_str());

            QString qError="frerror.dat";
            string errorf=qError.toUtf8().constData();
            std::ostringstream dat4NameStream(errorf);
            dat4NameStream<<path<<"/"<<errorf;
            std::string dat4Name = dat4NameStream.str();

            QFile checkfile4(dat4Name.c_str());

            if(!checkfile4.exists()){
                qDebug()<<"The file "<<checkfile4.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+qError+" does not exist!");
                return;
            }
            ifstream errors(dat4Name.c_str());

            int number_of_lines=0;

            while(std::getline(toplot1, zeile))
                ++number_of_lines;

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
                ++number_of_lines;

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

            // differences

            QString qDiff="frdiffstatistic.dat";
            string sdiff=qDiff.toUtf8().constData();
            std::ostringstream dat5NameStream(sdiff);
            dat5NameStream<<path<<"/"<<sdiff;
            std::string dat5Name = dat5NameStream.str();
            ofstream diffs(dat5Name.c_str());

            QString plot3="frdifferences.dat";
            string plot13 = plot3.toUtf8().constData();
            std::ostringstream dat3NameStream(plot13);
            dat3NameStream<<path<<"/"<<plot13;
            std::string dat3Name = dat3NameStream.str();

            QFile checkfile3(datName.c_str());

            if(!checkfile3.exists()){
                qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "File "+qPath+"/"+plot3+" does not exist!");
                return;
            }

            ifstream toplot3(dat3Name.c_str());

            number_of_lines=0;

            while(std::getline(toplot3, zeile))
            ++number_of_lines;

            toplot3.clear();
            toplot3.seekg(0, ios::beg);

            double offset = ui->doubleSpinBox_5->value();
            int index = 0, count=0;
            double sres=0.0, sres2=0.0, reasm=0.0;

            a.resize(number_of_lines);
            b.resize(number_of_lines);

            for (int i=0; i<number_of_lines; i++){
                toplot3 >> one >>two;
                istringstream ist(one);
                ist >> a[i];
                istringstream ist2(two);
                ist2 >> b[i];
                if((i>0)){
                    reasm=sres/count;
                    if((sres!=0) & (a[i]<a[i-1])){
                        reasm=sres/count;
                        diffs<<sres<<"\t"<<sres2<<"\t"<<reasm<<"\t"<<count<<endl;
                        sres = 0;
                        sres2 = 0;
                        count=0;
                        ++index;
                    }
                    else{
                        //
                    }
                }
                else{
                    // do nothing
                }
                if(ui->checkBox_33->isChecked()){
                    if((a[i]>ui->doubleSpinBox_14->value()) & (a[i]<ui->doubleSpinBox_15->value())){
                        sres+=b[i];
                        sres2+=pow(b[i],2);
                        ++count;
                    }
                    else{
                        // do nothing...
                    }
                }
                else{
                    sres+=b[i];
                    sres2+=pow(b[i],2);
                    ++count;
                }
                //b[i]=b[i]+index*offset;
            }
            if(sres!=0){
                diffs<<sres<<"\t"<<sres2<<"\t"<<reasm<<"\t"<<count<<endl;
            }
            else{
                //
            }

            diffs.close();
            toplot3.close();

            ifstream indiffs(dat5Name.c_str());

            string three, four;
            QVector<double> vreasm(index), vsres(index), vsres2(index), vcount(index);

            for (int i=0; i<index; i++){
                indiffs >> one >>two>>three>>four;
                istringstream ist(one);
                ist >> vsres[i];
                istringstream ist2(two);
                ist2 >> vsres2[i];
                istringstream ist3(three);
                ist3 >> vreasm[i];
                istringstream ist4(four);
                ist4 >> vcount[i];
            }
            indiffs.close();
            sres=0.0;
            sres2=0.0;
            count=0;
            index=0;
            ofstream diffs2(dat5Name.c_str());

            for(int i=0; i<number_of_lines; i++){
                if((i>0)){
                    if((sres!=0) & (a[i]<a[i-1])){
                        reasm=sres/count;
                        diffs2<<sres<<"\t"<<vsres[index]<<"\t"<<sres2<<"\t"<<vsres2[index]<<"\t"<<reasm<<"\t"<<count<<endl;
                        sres = 0.0;
                        sres2 = 0.0;
                        count=0;
                        ++index;
                    }
                    else{
                        // do nothing
                    }
                }
                else{
                    //do noting
                }
                if(ui->checkBox_33->isChecked()){
                    if((a[i]>ui->doubleSpinBox_14->value()) & (a[i]<ui->doubleSpinBox_15->value())){
                        sres+=vreasm[index]-b[i];
                        sres2+=pow((vreasm[index]-b[i]),2);
                        ++count;
                    }
                    else{
                        // do nothing...
                    }
                }
                else{
                    sres+=b[i];
                    sres2+=pow(b[i],2);
                    ++count;
                }
                b[i]=b[i]+index*offset;
            }
             diffs2<<sres<<"\t"<<vsres[index]<<"\t"<<sres2<<"\t"<<vsres2[index]<<"\t"<<reasm<<"\t"<<count<<endl;
             diffs2.close();

            ui->customPlot_3->addGraph();
            ui->customPlot_3->graph(0)->setData(a, b);
            ui->customPlot_3->graph(0)->setPen(QPen(Qt::red));
            ui->customPlot_3->graph(0)->setLineStyle(QCPGraph::lsNone);
            ui->customPlot_3->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));
            ui->customPlot_3->xAxis->setRange(xs1, xs2);
            ui->customPlot_3->yAxis->setRange(-0.5, 0.5+index*offset);
            ui->customPlot_3->replot();
        }
        else{
            //
        }
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
    sequence=0;
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
    qEdit->seData(ui->lineEdit_4->text(), hgh, ui->doubleSpinBox_4->value());
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

//***********************************
// RVs for A
//**********************************
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

//**********************************
// RVs for B
//**********************************
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
    qArith->seData(ui->lineEdit_4->text(), ui->lineEdit_13->text(), ui->lineEdit_26->text());
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
        if(runow==0){
            QMessageBox::information(this, "Error", "File "+qPath+"/"+qOptval+" does not exist!");
        }
        else{
            cout<<"Error: File "<<path<<"/"<<file1opt<<" does not exist!"<<endl;
        }
       return;
    }
    else{
        epara=0;
    }

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

    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        return;
    }
    else{
        //
    }


        QFile checkfile1(file1Name.c_str());

        if(!checkfile1.exists()){
            qDebug()<<"The optimisation file "<<checkfile1.fileName()<<" does not exist.";
            if(runow==0){
                QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptval+" does not exist.");
            }
            else{
                cout<<"Error: The optimisation data "<<path<<"/"<<file1opt<<" does not exist!"<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }
        else{
            //
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
            if(runow==0){
                QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptm+" does not exist.");
            }
            else{
                cout<<"Error: The optimisation data "<<path<<"/"<<file2opt<<" does not exist!"<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

        int number_of_lines =0;

        int NVar1=0, Plock=0, elock=0, T0lock=0;
        int Bin;
        bool xyz;
        double rv;

        int nu=7;
        int add=0;

        QVector<int> pvf(7);

        if(ui->checkBox_18->isChecked()){ // P
            pvf[0]=1; // fixed
            ++add;
        }
        else{
            pvf[0]=0; // free
        }
        if(ui->checkBox_17->isChecked()){ // e
            pvf[1]=1;
            ++add;
        }
        else{
            pvf[1]=0;
        }
        if(ui->checkBox_31->isChecked()){ // KA
            pvf[2]=1;
            ++add;
        }
        else{
            pvf[2]=0;
        }
        if(ui->checkBox_36->isChecked()){ // KB
            pvf[3]=1;
            ++add;
        }
        else{
            pvf[3]=0;
        }
        if(ui->checkBox_37->isChecked()){ // Gamma
            pvf[4]=1;
            ++add;
        }
        else{
            pvf[4]=0;
        }
        if(ui->checkBox_35->isChecked()){ // T0
            pvf[5]=1;
            ++add;
        }
        else{
            pvf[5]=0;
        }
        if(ui->checkBox_38->isChecked()){ // Omega
            pvf[6]=1;
            ++add;
        }
        else{
            pvf[6]=0;
        }

        nu = nu-add;
        mat Errors(nu,nu);
        QVector<double> STDError(nu);

        NVar1=7-add;
        cout<<add<<endl;
        cout<<NVar1<<endl;
        cout<<epara<<endl;

        if(epara!=NVar1+1){
            qDebug()<<"Wrong optimisation file.";
            if(runow==0){
                QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptval+" does not match to specifications in main window.");
            }
            else{
                cout<<"Error: The optimisation data "<<path<<"/"<<file1opt<<" does not match to the specification in main window."<<endl;
            }
           this->setCursor(QCursor(Qt::ArrowCursor));
            return;
        }

        double Bres;
        int NVar = (NVar1+1)*(NVar1+2)/2-(NVar1+1);
        QVector<double> Res(epara);
        double h(NVar);
        mat Bi(nu,nu), Gam(nu,nu), Qt(nu,nu);

        for (int i=0; i<epara; i++){
            initiate1 >> one;
            istringstream ist(one);
            ist >> Res[i];
            if(i==0){
                Bin = i;
                Bres = Res[0];
            }
            else if(Res[i]<Bres){
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
            if(runow==0){
                QMessageBox::information(this, "Error", "The optimisation data "+qPath+"/"+qOptm+" does not match to specifications in main window.");
            }
            else{
                cout<<"Error: The optimisation data "<<path<<"/"<<file2opt<<" does not match to the specification in main window."<<endl;
            }
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
        }

        int laufa=0, laufb=0, laufc=0;
        QVector<double> Theta0j(NVar1);

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

            for(int n=i; n<NVar1+1; n++){
                if((n!=i) & (n>i)){
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
                    sequence=0;
                    if(runow==0){
                        QMessageBox::information(this, "Abort", "Error estimation aborted.");
                    }
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
                if(runow==0){
                    QMessageBox::information(this, "Error", "Inversion of Matrix Q appears to be singular.");
                }
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


//*************************************
// Optimisation
//*************************************
void MainWindow::on_pushButton_4_clicked()
{
    MainWindow::disableButtons();
    MainWindow::Optimisation();

    if(reinitiate==1){
        reinitiate=0;
        inrein=1;
        MainWindow::on_pushButton_4_clicked();
    }
    MainWindow::enableButtons();

    this->setCursor(QCursor(Qt::ArrowCursor));

}

void MainWindow::on_checkBox_34_clicked()
{
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_8->setEnabled(false);
}

//************************************************
// Save settings
//************************************************
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

//****************************************
// load
//****************************************
void MainWindow::on_pushButton_10_clicked()
{
    QString qSPS = qPath+"/"+ ui->lineEdit_23->text() +".sps";
    QFile SPS(qSPS);

    string sps = qSPS.toUtf8().constData();
    std::ostringstream datANameStream(sps);
    datANameStream<<sps;
    std::string datAName = datANameStream.str();

    if(!SPS.exists()){
        if(runow==0){
            QMessageBox::information(this, "Error", "File "+ qSPS +" does not exist.");
            return;
        }
        else{
            cout<<"File "<<datAName<<" does not exist."<<endl;
            MainWindow::Input();
        }
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
             else if(g<SetN){
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
            if((ry[j]>rysh) & (ry[j]<ryh)){
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
    QString qSeq = QString::number(sequence);

    if(ui->checkBox_27->isChecked()){
        ui->lineEdit_5->setText(cName+"_"+qSeq+"_1."+cExt);
        ui->lineEdit_8->setText(cName+"_"+qSeq+"_2."+cExt);
        ui->lineEdit_10->setText(cName+"_"+qSeq+"_tell."+cExt);
        ui->lineEdit_13->setText(cName+"_"+qSeq+"_diff."+cExt);

        ui->lineEdit_14->setText("initval_"+qSeq+"_"+cName+"."+cExt);
        ui->lineEdit_15->setText("initmat_"+qSeq+"_"+cName+"."+cExt);
        ui->lineEdit_16->setText("optval_"+qSeq+"_"+cName+"."+cExt);
        ui->lineEdit_17->setText("optmat_"+qSeq+"_"+cName+"."+cExt);
        ui->lineEdit_20->setText(cName+"_"+qSeq+"_error."+cExt);
        ui->lineEdit_21->setText(cName+"_"+qSeq+"_log."+cExt);
        ui->lineEdit_22->setText(cName+"_"+qSeq+"_conf");
        ui->lineEdit_23->setText(cName+"_"+qSeq+"_conf");
    }

    else{
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
}

//*******************************************
// subtract disentangled from observations
//*******************************************
void MainWindow::on_pushButton_12_clicked()
{
    this->setCursor(QCursor(Qt::WaitCursor));

     MainWindow::read_data();

     this->setCursor(QCursor(Qt::WaitCursor));

     int smin= ui->spinBox->value();
     int smax = ui->spinBox_2->value();
     int mlo = (smax-smin+1)*logbin, zlin=0;
     string liner, ein1, zwei2;

     vec sol1(mlo), wav(bso1+bso2), sol2(bso1+bso2), diff(mlo);

     QString fext=ui->lineEdit_19->text();
     string sfext = fext.toUtf8().constData();

     double ofs=0.0;

     if((ui->checkBox_26->isChecked()) or (ui->checkBox_28->isChecked())){
         if(ui->lineEdit_28->text().isEmpty()){
             QMessageBox::information(this, "Error", "No value set to add.");
             return;
         }
         else{
            ofs = ui->lineEdit_28->text().toDouble();
         }
     }
     else{
         //
     }

     // use matrix

     if(ui->checkBox_20->isChecked()){

         QString input=ui->lineEdit_5->text();
         string in = input.toUtf8().constData();
         std::ostringstream inNameStream(in);
         inNameStream<<path<<"/compl.txt";//<<in;
         std::string inName = inNameStream.str();

         QFile checkfile1(inName.c_str());

         if(!checkfile1.exists()){
             qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
             QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+" does not exist!");
             this->setCursor(QCursor(Qt::ArrowCursor));
             return;
         }
         ifstream file1(inName.c_str());

         while(std::getline(file1, liner))
            ++zlin;

         file1.clear();
         file1.seekg(0, ios::beg);

         for(int i =0;i <bso1+bso2; i++){
             file1 >> ein1 >>zwei2;
             istringstream ist(ein1);
             istringstream ist2(zwei2);

             if(i<bso1){
                ist >> wav[i];
                ist2 >> sol2[i];
             }
             else{
                 wav[i]=0;
                 sol2[i]=0;
             }
         }
     }

     else{
            QString input=ui->lineEdit_8->text();
            string in = input.toUtf8().constData();
            std::ostringstream inNameStream(in);
            inNameStream<<path<<"/compl.txt";//<<in;
            std::string inName = inNameStream.str();

            QFile checkfile1(inName.c_str());

            if(!checkfile1.exists()){
                qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
                QMessageBox::information(this, "Error", "Error 1: File"+qPath+"/"+input+" does not exist!");
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            ifstream file1(inName.c_str());

            while(std::getline(file1, liner))
                ++zlin;

            file1.clear();
            file1.seekg(0, ios::beg);

            for(int i =0;i <bso1+bso2; i++){
                file1 >> ein1 >>zwei2;
                istringstream ist(ein1);
                istringstream ist2(zwei2);

                if(i>=bso1){
                    ist >> wav[i];
                    ist2 >> sol2[i];
                }
                else{
                    wav[i]=0;
                    sol2[i]=0;
                }
            }
        }

     sol1 = M*sol2;
     diff=C-sol1;

     for(int e = 0; e<smax-smin+1; e++){
        QString output=ui->lineEdit_27->text();
        string out = output.toUtf8().constData();
        std::ostringstream file1NameStream(out);
        file1NameStream<<path<<"/"<<out<<"_"<<e+smin<<sfext;
        std::string file1Name = file1NameStream.str();
        ofstream file2(file1Name.c_str());

        for(int i=e*logbin; i< (e+1)*logbin; i++){
            if(ui->checkBox_26->isChecked()){
                diff[i]=diff[i]+ofs;
            }
            else{
                if(ui->checkBox_28->isChecked()){
                    diff[i]=diff[i]*ofs;
                }
                else{
                    //
                }
            }
            if(pow(10,W[i])>1){
                if(ui->checkBox_24->isChecked()){
                   file2<<setprecision(14)<<pow(10,(W[i]))*(1-RV3[e]/c0)<<"\t"<<diff[i]<<endl;
                }
                else{
                    if(ui->checkBox_25->isChecked()){
                        file2<<setprecision(14)<<pow(10,(W[i]))*(1-RV1[e]/c0)<<"\t"<<diff[i]<<endl;

                    }
                    else{
                       file2<<setprecision(14)<<pow(10,(W[i]))<<"\t"<<diff[i]<<endl;
                    }
                }
            }
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

    omp_set_num_threads(cores);
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

void MainWindow::on_checkBox_20_clicked()
{
    if(ui->checkBox_20->isChecked()){
        ui->checkBox_21->setChecked(false);
    }
    else{
        ui->checkBox_21->setChecked(true);
    }
}

void MainWindow::on_checkBox_21_clicked()
{
    if(ui->checkBox_21->isChecked()){
        ui->checkBox_20->setChecked(false);
    }
    else{
        ui->checkBox_20->setChecked(true);
    }
}

void MainWindow::on_checkBox_24_clicked()
{
    if(ui->checkBox_24->isChecked()){
        ui->checkBox_25->setChecked(false);
    }
    else{
        //
    }
}

void MainWindow::on_checkBox_25_clicked()
{
    if(ui->checkBox_25->isChecked()){
        ui->checkBox_24->setChecked(false);
    }
    else{
        //
    }
}

void MainWindow::on_checkBox_27_clicked()
{
    if(ui->checkBox_27->isChecked()){
        ui->spinBox_10->setEnabled(true);
        ui->spinBox_9->setEnabled(true);
        ui->checkBox_23->setChecked(true);
        ui->checkBox_23->setEnabled(false);
        ui->checkBox_32->setChecked(true);
        ui->checkBox_32->setEnabled(false);
        ui->spinBox_3->setValue(9999);
    }
    else{
        ui->spinBox_9->setEnabled(false);
        ui->spinBox_10->setEnabled(false);
        ui->checkBox_23->setEnabled(true);
        ui->checkBox_32->setEnabled(true);

    }
}

void MainWindow::on_checkBox_26_clicked()
{
    if(ui->checkBox_26->isChecked()){
        ui->checkBox_28->setChecked(false);
    }
    else{
        //
    }
}

void MainWindow::on_checkBox_28_clicked()
{
    if(ui->checkBox_28->isChecked()){
        ui->checkBox_26->setChecked(false);
    }
}

void MainWindow::disableButtons()
{
   ui->pushButton->setEnabled(false);
   ui->pushButton_2->setEnabled(false);
   ui->pushButton_3->setEnabled(false);
   ui->pushButton_4->setEnabled(false);
   ui->pushButton_5->setEnabled(false);
   ui->pushButton_8->setEnabled(false);
   ui->pushButton_9->setEnabled(false);
   ui->pushButton_10->setEnabled(false);
   ui->pushButton_11->setEnabled(false);
   ui->pushButton_12->setEnabled(false);
   ui->pushButton_13->setEnabled(false);
}

void MainWindow::enableButtons()
{
   ui->pushButton->setEnabled(true);
   ui->pushButton_2->setEnabled(true);
   ui->pushButton_3->setEnabled(true);
   ui->pushButton_4->setEnabled(true);
   ui->pushButton_5->setEnabled(true);
   ui->pushButton_8->setEnabled(true);
   ui->pushButton_9->setEnabled(true);
   ui->pushButton_10->setEnabled(true);
   ui->pushButton_11->setEnabled(true);
   ui->pushButton_12->setEnabled(true);
   ui->pushButton_13->setEnabled(true);
}


//*******************************************
// optimize on flux ratios
//*******************************************
void MainWindow::on_pushButton_13_clicked()
{
    qPath=ui->lineEdit_4->text();
    path = qPath.toUtf8().constData();

    QString input=qPath+"/continuumpoints.dat";
    string data1 = input.toUtf8().constData();
    std::ostringstream dat1NameStream(data1);
    dat1NameStream<<data1;
    std::string dat1Name = dat1NameStream.str();

    QFile checkfile1(dat1Name.c_str());

    if(ui->checkBox_30->isChecked()){

        if(!checkfile1.exists()){
            QMessageBox::information(this, "Error", "File to define continuum does not exist. Please set the points via the Editor.");
            return;
        }
        else{
            string line, eins;
            int nlines=0;
            ifstream points(dat1Name.c_str());
            while(std::getline(points, line))
                ++nlines;

            points.clear();
            points.seekg(0, ios::beg);

            if(nlines!=8){
                QMessageBox::information(this, "Error", "Number of points in continuumpoints.dat is not 8.");
                return;
            }
            else{
                for(int i =0;i <8; i++){
                    points >> eins;
                    istringstream ist(eins);
                    ist>>cpoints[i];
                }
            }
        }
    }
    else{
        //
    }

    optfratios=1;
    MainWindow::disableButtons();

    // optmisation on flux ratios
    //system_clock::time_point time1 = system_clock::now();

    upda=0;
    updac=0;
    abauto=0;

    ui->spinBox_4->setValue(0);
    ui->spinBox_5->setValue(0);

    this->setCursor(QCursor(Qt::WaitCursor));
    MainWindow::read_data();
    this->setCursor(QCursor(Qt::WaitCursor));

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        optfratios=0;
        error=0;
        MainWindow::enableButtons();
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

    int nu=num;
    QVector<double> fratios(num);

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

    LogFile<<"Optimisation of flux ratios."<<endl;

    LogFile<<endl;

    if(ui->checkBox_29->isChecked()){
        LogFile<<"Using residuum as criterion for optimisation."<<endl;
    }
    else{
        LogFile<<"Using function slope as criterion for optimisation."<<endl;
    }

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
                    if(runow==0){
                        QMessageBox::information(this, "Error", "Error 22: Edit file does not match to specified number of observations.");
                    }
                    else{
                        cout<<"Error 22: Edit file does not match to specified number of observations."<<endl;
                        MainWindow::Input();
                    }
                    error=1;
                    optfratios=0;
                    MainWindow::enableButtons();
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    return;
                }
                LogFile<<"Relative component strength and telluric line strength set via Editor."<<endl;
                LogFile<<"The strength of component B and the ratios are calculated via given strength of component A."<<endl;

                for(int i=0; i<num_lines1; i++){
                    edit >> einse;
                    istringstream istr7(einse);

                    if(i<num){
                        istr7>>Mtel[i];
                    }
                    if((i>=num) & (i<2*num)){
                        istr7>>Mval1[i-num];
                        fratios[i-num]=1/Mval1[i-num]-1;
                        Mval2[i-num]=fratios[i-num]/(1+fratios[i-num]);
                    }
                    if(i>=2*num){
                        //istr7>>Mval2[i-2*num];
                        //Mval2[i]=fratios[i]/(1+fratios[i]);
                    }
                }
            }

            else {
                LogFile<<"No relative values set via Editor. Using flux ratio from GUI for all spectra."<<endl;

                for(int i=0; i<num; i++){
                    Mtel[i]=1.0;
                    Mval1[i]=1/(1+ui->doubleSpinBox_4->value());
                    Mval2[i]=ui->doubleSpinBox_4->value()/(1+ui->doubleSpinBox_4->value());
                    fratios[i]=ui->doubleSpinBox_4->value();
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

        Gamma=2.0;
        alpha=1.0;
        beta=0.5;
        btot=0.5;
    }

    string sinit = "initialstep.dat";
    std::ostringstream iniNameStream(sinit);
    iniNameStream<<path<<"/"<<sinit;
    std::string iniName = iniNameStream.str();

    QFileInfo initstep(iniName.c_str());

    if(initstep.exists()){

        ifstream initStep(iniName.c_str());
        double IntS[8];
        string einse;

        for(int i =0; i<8; i++){
            initStep >> einse;
            istringstream istr9(einse);
            istr9 >> IntS[i];
        }
        step = IntS[0]*0.1;
        dP = IntS[1];
        de = IntS[2];
        dKA = IntS[3];
        dKB = IntS[4];
        dGamma = IntS[5];
        dT0 = IntS[6];
        dOmega = IntS[7];

        LogFile<<"User defined initial step size of "<<step<<" used."<<endl;
    }

    else{

        LogFile<<"Default initial step size of 0.1 used."<<endl;

        step=0.1;
        dP = orbele[0]/10;
        de = orbele[1]/10;
        dKA = 3*dv;
        dKB = 3*dv;
        dGamma = 3*dv;
        dT0 = orbele[0]/10;
        dOmega = M_PI/20;
    }

    LogFile<<"Alpha: "<<alpha<<"; Beta: "<<beta<<"; Beta_tot: "<<btot<<"; Gamma: "<<Gamma<<"; Step: "<<step<<endl;

    LogFile<<endl;

    double y[nu+1], Z[nu], Co[nu], So[nu], Eo[nu];
    mat P(nu+1,nu), e(nu+1,nu);

    r1=0.0;

    LogFile<<"Creating new initial simplex with "<<nu+1<<" points."<<endl;

    qInitval = "fr_opt_val.dat";
    string file1init = qInitval.toUtf8().constData();
    std::ostringstream file1NameStream(file1init);
    file1NameStream<<path<<"/"<<file1init;
    std::string file1Name = file1NameStream.str();

    qInitmat = "fr_opt_mat.dat";
    string file2init = qInitmat.toUtf8().constData();
    std::ostringstream file2NameStream(file2init);
    file2NameStream<<path<<"/"<<file2init;
    std::string file2Name = file2NameStream.str();

    if(ui->checkBox_9->isChecked()){
        QFile QIn(file1Name.c_str());
        QFile QIn2(file2Name.c_str());

        if((inrein==0) & (QIn.exists() or QIn2.exists())){
            if(runow==0){
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Warning!", "The optimisation files "+QIn.fileName()+" and "+QIn2.fileName()+" already exists. \n\n Do you want to overwrite it?",
                                      QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                qDebug() << "Overwrite initial data.";
                }
                else{
                    qDebug()<< "Initiation aborted.";
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    MainWindow::enableButtons();
                    optfratios=0;
                    return;
                }
            }
            else{
                cout<<"Warning: The optimisation files already exists. Do you want to overwrite it? 0 no, yes 1: "<<endl;
                cin >> overw;
                if(overw==0){
                    this->setCursor(QCursor(Qt::ArrowCursor));
                    MainWindow::enableButtons();
                    optfratios=0;
                    return;
                }
                else{
                    qDebug() << "Overwrite initial data.";
                }
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
            P(0,i)=fratios[i];
        }

    for(int i=0; i<nu+1; i++){
        for(int j=0; j<nu; j++){

            if((i>0)&(i==j+1)){
                e(i,j)=step;
            }
            else{
                e(i,j)=0;
            }
        }
    }

    for (int i=0; i<nu+1; i++){
        for (int j=0; j<nu; j++){

            P(i,j)=P(0,j)+e(i,j);
            init2<<setprecision(14)<<P(i,j)<<endl;
            //cout<<P(i,j)<<" ";

            Mval1[j]=1/(1+P(i,j));
            Mval2[j]=P(i,j)/(1+P(i,j));
        }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[i]=MainWindow::DivideConquer();
        //soptd<<"0\t"<<eval<<"\t"<<y[i]<<endl;
        qApp->processEvents(QEventLoop::AllEvents);
        if(upda==1){
            upda=0;
            abauto += 1;
            LogFile<<endl;
            LogFile<<"New ratios found; Residuum: "<<y[i]<<endl;
            MainWindow::on_pushButton_6_clicked();
            //MainWindow::plot_functionvalues();
                for(int i = 0; i<nu;i++){
                     LogFile<<Mval1[i]<<"\t"<<Mval2[i]<<endl;
                }
            LogFile<<endl;
        }

        if(abortt==1){
            LogFile<<"Initiation was aborted."<<endl;
            abortt=0;
            sequence=0;
            optfratios=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            MainWindow::enableButtons();
            return;
        }

        cout<<y[i]<<endl;
        init<<setprecision(12)<<y[i]<<endl;
        LogFile<<"Residuum of point "<<i<<": "<<y[i]<<endl;
    }

    init.close();
    init2.close();

    LogFile<<"Initial simplex completed. Start of optimisation."<<endl;

    double ys2=0.0, ym2=0.0;
    int stagnate=0;

    //start optimisation

    qOptval = "optval_flux.dat";
    string file1opt = qOptval.toUtf8().constData();
    std::ostringstream file3NameStream(file1opt);
    file3NameStream<<path<<"/"<<file1opt;
    std::string file3Name = file3NameStream.str();
    ofstream opt1(file3Name.c_str());

    qOptmat = "optmat_flux.dat";
    string file2opt = qOptmat.toUtf8().constData();
    std::ostringstream file4NameStream(file2opt);
    file4NameStream<<path<<"/"<<file2opt;
    std::string file4Name = file4NameStream.str();
    ofstream opt2(file4Name.c_str());

    for (int t=0; t<zaehler; t++){
        ui->spinBox_4->setValue(t+1);

        string optback1 = "optval_flux_backup.dat";
        std::ostringstream back1NameStream(optback1);
        back1NameStream<<path<<"/"<<optback1;
        std::string back1Name = back1NameStream.str();
        ofstream back1(back1Name.c_str());


        string optback2 = "optmat_flux_backup.dat";
        std::ostringstream back2NameStream(optback2);
        back2NameStream<<path<<"/"<<optback2;
        std::string back2Name = back2NameStream.str();
        ofstream back2(back2Name.c_str());

        for(int i=0; i<nu+1;i++){
            //cout<<y[i]<<endl;
            back1<<setprecision(12)<<y[i]<<endl;
            for(int j=0; j<nu; j++){
                back2<<setprecision(12)<<P(i,j)<<endl;
            }
        }

        LogFile<<endl;
        LogFile<<"Iteration: "<<t+1<<endl;
        //MainWindow::plot_functionvalues();
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
            cout<<"stagnating."<<endl;
        }
        else{
            //
        }
        ys2=ys;
        ym2=ym;

        cout<<"Simplex Mean: "<<ym<<"; Simplex STD: "<<ys<<endl;

        LogFile<<setprecision(12)<<"Mean: "<<ym<<"\tSTD: "<<ys<<endl;

        //compute centroid
        for (int j=0; j<nu; j++){
            for (int i=0; i<nu+1; i++){
                if (i!=Ph){
                    Z[j]+=P(i,j)/nu;
                }
            }
        }

        //reflect highest value at centroid
        cout<<"reflection at centroid..."<<endl;
        for (int i=0; i<nu; i++){
            Co[i]=Z[i]+alpha*(Z[i]-P(Ph,i));
            Mval1[i]=1/(1+Co[i]);
            Mval2[i]=Co[i]/(1+Co[i]);
        }
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        yi=MainWindow::DivideConquer();
        //soptd<<t<<"\t"<<eval<<"\t"<<yi<<endl;
        qApp->processEvents(QEventLoop::AllEvents);

        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New ratios found; Residuum: "<<yi<<endl;
                for(int i = 0; i<nu;i++){
                    LogFile<<Co[i]<<endl;
                }
            LogFile<<endl;
            MainWindow::on_pushButton_6_clicked();
            qApp->processEvents(QEventLoop::AllEvents);
        }

        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best ratios:"<<endl;

                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
        }

        //Alpha Branch, better than best point
        if(yi<yl){
           cout<<"expansion 1 ";
            LogFile<<"yi<yl; ";
            for (int i=0; i<nu; i++){
                Eo[i]=Z[i]+Gamma*(Z[i]-P(Ph,i));  //Expansion over worst point
                Mval1[i]=1/(1+Eo[i]);
                Mval2[i]=Eo[i]/(1+Eo[i]);
            }
            eval++;
            ui->spinBox_5->setValue(eval);
            qApp->processEvents(QEventLoop::AllEvents);
            MainWindow::ConstructMatrix();
            yt=MainWindow::DivideConquer();
            //soptd<<t<<"\t"<<eval<<"\t"<<yt<<endl;
            qApp->processEvents(QEventLoop::AllEvents);

            if(upda==1){
                upda=0;
                LogFile<<endl;
                LogFile<<"New ratios found; Residuum: "<<yt<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<Eo[i]<<endl;
                    }
                LogFile<<endl;
                MainWindow::on_pushButton_6_clicked();
                qApp->processEvents(QEventLoop::AllEvents);
            }

            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }
            if(yt<yl){
                cout<<"expansion";
                for (int i=0; i<nu; i++){
                    P(Ph,i)=Eo[i];
                }

            y[Ph]=yt;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);

        }
        if (yt>=yl){
            cout<<"ref1 ";
            LogFile<<"yt<=yl, reflection; ";
            for (int i=0; i<nu; i++){
                P(Ph,i)=Co[i];
                Mval1[i]=1/(1+Co[i]);
                Mval2[i]=Co[i]/(1+Co[i]);
            }

            y[Ph]=yi;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                    Mval1[i]=1/(1+Co[i]);
                    Mval2[i]=Co[i]/(1+Co[i]);
                }

            y[Ph]=yi;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
                    Mval1[i]=1/(1+Co[i]);
                    Mval2[i]=Co[i]/(1+Co[i]);
                }

            y[Ph]=yi;//MainWindow::DivideConquer();
            if(abortt==1){
                //output results
                LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                    for(int i = 0; i<nu;i++){
                        LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }
            qApp->processEvents(QEventLoop::AllEvents);
        //yh=y[Ph];
        }

        for(int i=0; i<nu; i++){
            So[i]=Z[i]+beta*(P(Ph,i)-Z[i]); //simple contraction
            Mval1[i]=1/(1+So[i]);
            Mval2[i]=So[i]/(1+So[i]);
        }
        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        yt=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);
        //soptd<<t<<"\t"<<eval<<"\t"<<yt<<endl;
        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New ratios found; Residuum: "<<yt<<endl;
            //MainWindow::plot_functionvalues();
                for(int i = 0; i<nu;i++){
                    LogFile<<So[i]<<endl;
                }

            LogFile<<endl;
            MainWindow::on_pushButton_6_clicked();
            qApp->processEvents(QEventLoop::AllEvents);
        }

        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
                return;
            }

        //Beta_1_1 Branch
        if(yt>yh){
            cout<<"yt>yh tot ";
            LogFile<<"yt>yh, total contraction, ";
            for (int j=0; j<nu+1; j++){
                for (int i=0; i<nu; i++){
                    P(j,i)=P(Pl,i)+btot*(P(j,i)-P(Pl,i)); //total contraction
                    Mval1[i]=1/(1+P(j,i));
                    Mval2[i]=P(j,i)/(1+P(j,i));
                }

        eval++;
        ui->spinBox_5->setValue(eval);
        qApp->processEvents(QEventLoop::AllEvents);
        MainWindow::ConstructMatrix();
        y[j]=MainWindow::DivideConquer();
        qApp->processEvents(QEventLoop::AllEvents);
        cout<<j+1<<" of "<<nu+1<<" points contracted."<<endl;
        //soptd<<t<<"\t"<<eval<<"\t"<<y[j]<<endl;

        if(upda==1){
            upda=0;
            LogFile<<endl;
            LogFile<<"New ratios found; Residuum: "<<y[j]<<endl;
                for(int i = 0; i<nu;i++){
                    LogFile<<P(j,i)<<endl;
                }
            LogFile<<endl;
            MainWindow::on_pushButton_6_clicked();
            qApp->processEvents(QEventLoop::AllEvents);
        }

        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
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
            sequence=0;
            optfratios=0;
            this->setCursor(QCursor(Qt::ArrowCursor));
            MainWindow::enableButtons();
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
                Mval1[i]=1/(1+So[i]);
                Mval2[i]=So[i]/(1+So[i]);
            }

        y[Ph]=yt;//MainWindow::DivideConquer();
        if(abortt==1){
            //output results
            LogFile<<"Optimisation aborted. Current best ratios:"<<endl;
                for(int i = 0; i<nu;i++){
                    LogFile<<P(Pl,i)<<endl;
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
                sequence=0;
                optfratios=0;
                this->setCursor(QCursor(Qt::ArrowCursor));
                MainWindow::enableButtons();
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
        LogFile<<" ratios after "<<zaehler<<" Iterations and "<<eval<<" Evaluations of Optimisation: "<<endl;
            for(int i = 0; i<nu;i++){
                LogFile<<P(Pl,i)<<endl;
            }

        LogFile.close();

        cout<<"Ratios after "<<zaehler<<" Iterations and "<<eval<<" Evaluations:"<<endl;
        for(int i=0; i<nu;i++){
            cout<<P(Pl,i)<<endl;
        }

    optfratios=0;
    MainWindow::enableButtons();
    this->setCursor(QCursor(Qt::ArrowCursor));

}


void MainWindow::on_checkBox_29_clicked()
{
    if(ui->checkBox_29->isChecked()){
        ui->checkBox_30->setChecked(false);
    }
    else{
        ui->checkBox_30->setChecked(true);
    }
}

void MainWindow::on_checkBox_30_clicked()
{
    if(ui->checkBox_30->isChecked()){
        ui->checkBox_29->setChecked(false);
    }
    else{
        ui->checkBox_29->setChecked(true);
    }
}

//******************************
// sparse matrix solver
//******************************
void MainWindow::separation()
{
    this->setCursor(QCursor(Qt::WaitCursor));
    if(ui->spinBox_2->value()-ui->spinBox->value()<1){
        if(runow==0){
            QMessageBox::information(this, "Error", "Error 19: At least two observations at (hopefully) different phases are necessary.");
        }
        else{
            cout<<"Error 19: At least two observations at (hopefully) different phases are necessary."<<endl;
            MainWindow::Input();
        }
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;
    }

    MainWindow::disableButtons();
    MainWindow::read_data();

    if(error==1){
        this->setCursor(QCursor(Qt::ArrowCursor));
        error=0;
        MainWindow::enableButtons();
        return;
    }
    this->setCursor(QCursor(Qt::WaitCursor));


        std::ostringstream file5NameStream("compl.txt");
        file5NameStream<<path<<"/compl.txt";
        std::string file5Name = file5NameStream.str();
        ofstream file7(file5Name.c_str());

        for(int i =0; i<Mm; i++){
            file7<<i<<"\t"<<X(i)<<endl;
        }

    MainWindow::enableButtons();
    this->setCursor(QCursor(Qt::ArrowCursor));
}
