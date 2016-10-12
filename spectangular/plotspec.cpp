#include "plotspec.h"
#include "ui_plotspec.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <QLineEdit>
#include <QPixmap>
#include <QLabel>
#include <QVector>
#include <QFile>
#include <QtDebug>
#include <QCheckBox>
#include "spline.h"

using namespace std;

QString qSpPath;
string spPath;
QVector<double> aps(1), bps(1), cps(1), dps(1), eps(1), fps(1), d1ps(1), d2ps(1), asc(1), bsc(1), sc(1), csc(1), dsc(1), errw(1), erry(1);
int clicks=0, slength;
double xc, yc;
ofstream spline("spline.dat");
tk::spline s;

PlotSpec::PlotSpec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotSpec)
{
    ui->setupUi(this);

    this->setWindowTitle("Spectrum Plotter");

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this ,SLOT(showPointToolTip(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseDoubleClick(QMouseEvent*)), this,SLOT(writeCoords(QMouseEvent*)));

    ui->lineEdit->setText("95_orb_1.txt");
    ui->lineEdit_7->setText("comp2.txt");
    ui->customPlot->xAxis->setLabel("Wavelength [A]");
    ui->checkBox->setChecked(true);
    ui->lineEdit_5->setText("/home/daniels/Observations/CTIO/BAT99_95");
    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

    ui->checkBox_14->setChecked(true);
    ui->doubleSpinBox_19->setValue(1);

    QFont legendFont = font();
    legendFont.setPointSize(16);
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->xAxis->setLabelFont(legendFont);
    ui->customPlot->yAxis->setLabelFont(legendFont);
    ui->customPlot->xAxis->setTickLabelFont(legendFont);
    ui->customPlot->yAxis->setTickLabelFont(legendFont);
}

PlotSpec::~PlotSpec()
{
    delete ui;
}

//********************************************************
//show mouse coordinates
//********************************************************
void PlotSpec::showPointToolTip(QMouseEvent *event)
{

    xc = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    yc = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    setToolTip(QString("%1 , %2").arg(xc).arg(yc));
}

//********************************************************
//write coordinates to file for spline fit
//********************************************************
void PlotSpec::writeCoords(QMouseEvent *event){

    xc = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
    yc = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

    spline<<xc<<" "<<yc<<endl;

    ++clicks;
}

string one, two, zeile;
double xs1, xs2, ys1, ys2;

//****************************************************************
//plot spectrum
//****************************************************************
void PlotSpec::on_pushButton_2_clicked()
{
    ui->customPlot->clearGraphs();

    QPen pen1, pen2, pen3, pen4;

    QString plot1=ui->lineEdit->text();
    string plot11 = plot1.toUtf8().constData();
    std::ostringstream datNameStream(plot11);
    datNameStream<<spPath<<"/"<<plot11;
    std::string datName = datNameStream.str();
    ifstream toplot1(datName.c_str());

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+plot1+" does not exist!");
       return;
    }

    int number_of_lines =0;

    while(std::getline(toplot1, zeile))
       ++ number_of_lines;

    toplot1.clear();
    toplot1.seekg(0, ios::beg);

    aps.resize(number_of_lines);
    bps.resize(number_of_lines);
    eps.resize(number_of_lines);
    d1ps.resize(number_of_lines);

    for (int i=0; i<number_of_lines; i++){
    toplot1 >> one >>two;
    istringstream ist(one);
    ist >> aps[i];
    if(ui->checkBox_11->isChecked()){
        aps[i]=aps[i]+ui->doubleSpinBox_13->value();
    }
    istringstream ist2(two);
    ist2 >> bps[i];
    if(ui->checkBox_5->isChecked()){
        bps[i]=bps[i]*ui->doubleSpinBox_5->value();
    }
    if(ui->checkBox_6->isChecked()){
        bps[i]=bps[i]+ui->doubleSpinBox_6->value();
    }
    eps[i]=ui->doubleSpinBox_9->value()+(aps[i]-aps[0])*ui->doubleSpinBox_10->value();
    }
    toplot1.close();

    QString plot2=ui->lineEdit_7->text();
    string plot12 = plot2.toUtf8().constData();
    std::ostringstream dat2NameStream(plot12);
    dat2NameStream<<spPath<<"/"<<plot12;
    std::string dat2Name = dat2NameStream.str();
    ifstream toplot2(dat2Name.c_str());

    QFile checkfile1(dat2Name.c_str());

    if(!checkfile1.exists()){
        qDebug()<<"The file "<<checkfile1.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+plot2+" does not exist!");
       return;
    }

    number_of_lines =0;

    while(std::getline(toplot2, zeile))
       ++ number_of_lines;

    toplot2.clear();
    toplot2.seekg(0, ios::beg);

    cps.resize(number_of_lines);
    dps.resize(number_of_lines);
    fps.resize(number_of_lines);
    d2ps.resize(number_of_lines);

    for (int i=0; i<number_of_lines; i++){
    toplot2 >> one >>two;
    istringstream ist3(one);
    ist3 >> cps[i];
    if(ui->checkBox_12->isChecked()){
        cps[i]=cps[i]+ui->doubleSpinBox_14->value();
    }
    istringstream ist4(two);
    ist4 >> dps[i];
    if(ui->checkBox_7->isChecked()){
        dps[i]=dps[i]*ui->doubleSpinBox_7->value();
    }
    if(ui->checkBox_8->isChecked()){
        dps[i]=dps[i]+ui->doubleSpinBox_8->value();
    }
    fps[i]=ui->doubleSpinBox_11->value()+(cps[i]-cps[0])*ui->doubleSpinBox_12->value();
    }
    toplot2.close();

    // error file
    QString plot3=ui->lineEdit_12->text();
    string plot13 = plot3.toUtf8().constData();
    std::ostringstream dat3NameStream(plot13);
    dat3NameStream<<spPath<<"/"<<plot13;
    std::string dat3Name = dat3NameStream.str();
    ifstream toplot3(dat3Name.c_str());

    QFile checkfile3(dat3Name.c_str());

    if(!checkfile3.exists()){
        qDebug()<<"The file "<<checkfile3.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+plot3+" does not exist!");
       return;
    }

    number_of_lines =0;

    while(std::getline(toplot3, zeile))
       ++ number_of_lines;

    toplot3.clear();
    toplot3.seekg(0, ios::beg);

    errw.resize(number_of_lines);
    erry.resize(number_of_lines);

    for(int i=0; i<number_of_lines; i++){
        toplot3 >> one >> two;
        istringstream ist5(one);
        ist5 >> errw[i];
        istringstream ist6(two);
        ist6 >> erry[i];
        erry[i]=abs(erry[i]);
    }
    toplot3.close();


    // plot data
    ui->customPlot->addGraph();
    if(ui->checkBox_4->isChecked()){
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    QString label=ui->lineEdit_6->text();
    ui->customPlot->graph(0)->setName(label);
    }
    if(ui->checkBox_15->isChecked()){
        ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
        ui->customPlot->graph(0)->setErrorType(QCPGraph::etValue);
        ui->customPlot->graph(0)->setDataValueError(aps, bps, erry);
    }
    else{
        ui->customPlot->graph(0)->setData(aps, bps);
    }
    pen1.setColor(Qt::blue);
    pen1.setWidth(ui->spinBox_2->value());
    ui->customPlot->graph(0)->setPen(pen1);

    ui->customPlot->addGraph();
    if(ui->checkBox_15->isChecked()){
        ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsStepCenter);
        ui->customPlot->graph(1)->setErrorType(QCPGraph::etValue);
        ui->customPlot->graph(1)->setDataValueError(cps, dps, erry);
    }
    else{
        ui->customPlot->graph(1)->setData(cps, dps);
    }
    pen2.setColor(Qt::red);
    pen2.setWidth(ui->spinBox_2->value());
    ui->customPlot->graph(1)->setPen(pen2);

    if(ui->checkBox_9->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setData(aps, eps);
        pen3.setColor(Qt::black);
        pen3.setWidth(ui->spinBox_2->value());
        ui->customPlot->graph(2)->setPen(pen3);
    }
    if(ui->checkBox_10->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setData(cps, fps);
        pen4.setColor(Qt::green);
        pen4.setWidth(ui->spinBox_2->value());
        ui->customPlot->graph(3)->setPen(pen4);;
    }
    ui->customPlot->xAxis->setRange(xs1, xs2);
    ui->customPlot->yAxis->setRange(ys1, ys2);
    ui->customPlot->replot();
}

//****************************************************************
//find values
//****************************************************************
void PlotSpec::on_pushButton_3_clicked()
{
    QString plot1=ui->lineEdit->text();
    string plot11 = plot1.toUtf8().constData();
    std::ostringstream datNameStream(plot11);
    datNameStream<<spPath<<"/"<<plot11;
    std::string datName = datNameStream.str();
    ifstream toplot1(datName.c_str());

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+plot1+" does not exist!");
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
    if(ui->checkBox_11->isChecked()){
        a[i]=a[i]+ui->doubleSpinBox_13->value();
    }
    istringstream ist2(two);
    ist2 >> b[i];
    if(ui->checkBox_5->isChecked()){
        b[i]=b[i]*ui->doubleSpinBox_5->value();
    }
    if(ui->checkBox_6->isChecked()){
        b[i]=b[i]+ui->doubleSpinBox_6->value();
    }
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

    ui->doubleSpinBox->setValue(xs1);
    ui->doubleSpinBox_2->setValue(xs2);

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


    ui->doubleSpinBox_3->setValue(ys1);
    ui->doubleSpinBox_4->setValue(ys2);
}



void PlotSpec::on_doubleSpinBox_valueChanged()
{
    xs1=ui->doubleSpinBox->value();
}

void PlotSpec::on_doubleSpinBox_2_valueChanged()
{
    xs2=ui->doubleSpinBox_2->value();
}

void PlotSpec::on_doubleSpinBox_3_valueChanged()
{
    ys1=ui->doubleSpinBox_3->value();
}

void PlotSpec::on_doubleSpinBox_4_valueChanged()
{
    ys2=ui->doubleSpinBox_4->value();
}

void PlotSpec::on_lineEdit_2_editingFinished()
{
    QString xlabel=ui->lineEdit_2->text();
    ui->customPlot->xAxis->setLabel(xlabel);
}

void PlotSpec::on_lineEdit_3_editingFinished()
{
    QString ylabel=ui->lineEdit_3->text();
    ui->customPlot->yAxis->setLabel(ylabel);
}

void PlotSpec::on_pushButton_4_clicked()
{
    if(ui->checkBox->isChecked()){
        QString sav=ui->lineEdit_4->text();
        QString save=qSpPath+"/"+sav +".pdf";
        ui->customPlot->savePdf(save);
    }
    if(ui->checkBox_2->isChecked()){
        QString sav=ui->lineEdit_4->text();
        QString save=qSpPath+"/"+sav+".png";
        ui->customPlot->savePng(save);
    }
    if(ui->checkBox_3->isChecked()){
        QString sav=ui->lineEdit_4->text();
        QString save=qSpPath+"/"+sav+".jpg";
        ui->customPlot->saveJpg(save);
    }
}

void PlotSpec::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
}

void PlotSpec::on_checkBox_2_clicked()
{
    ui->checkBox_3->setChecked(false);
    ui->checkBox->setChecked(false);
}

void PlotSpec::on_checkBox_3_clicked()
{
    ui->checkBox_2->setChecked(false);
    ui->checkBox->setChecked(false);
}

void PlotSpec::on_lineEdit_5_textEdited()
{
    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();
}

void PlotSpec::on_spinBox_valueChanged()
{
    QFont legendFont = font();
    legendFont.setPointSize(ui->spinBox->value());
    ui->customPlot->xAxis->setLabelFont(legendFont);
    ui->customPlot->yAxis->setLabelFont(legendFont);
    ui->customPlot->xAxis->setTickLabelFont(legendFont);
    ui->customPlot->yAxis->setTickLabelFont(legendFont);
}

void PlotSpec::on_doubleSpinBox_10_valueChanged()
{
    PlotSpec::on_pushButton_2_clicked();
}

void PlotSpec::on_doubleSpinBox_12_valueChanged()
{
    PlotSpec::on_pushButton_2_clicked();
}

void PlotSpec::on_doubleSpinBox_9_valueChanged()
{
    PlotSpec::on_pushButton_2_clicked();
}

void PlotSpec::on_doubleSpinBox_11_valueChanged()
{
    PlotSpec::on_pushButton_2_clicked();
}

//*******************************************************
//add 1-f to file 1
//*******************************************************
void PlotSpec::on_pushButton_5_clicked()
{
    ui->customPlot->clearGraphs();

    for(int i=0; i<aps.size(); i++){
        d1ps[i]=bps[i]+(1-eps[i]);
    }

    for(int i=0; i<cps.size(); i++){
        d2ps[i]=dps[i]+(1-fps[i]);
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(aps, d1ps);
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(cps, d2ps);
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->replot();
}

//*******************************************************
// compute SNR in specified region
//*******************************************************
void PlotSpec::SNR(){

    QString plot1=ui->lineEdit->text();
    string plot11 = plot1.toUtf8().constData();
    std::ostringstream datNameStream(plot11);
    datNameStream<<spPath<<"/"<<plot11;
    std::string datName = datNameStream.str();
    ifstream toplot1(datName.c_str());

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+plot1+" does not exist!");
       return;
    }

    int number_of_lines=0;

    while(std::getline(toplot1, zeile))
       ++ number_of_lines;

    toplot1.clear();
    toplot1.seekg(0, ios::beg);

    QVector<double> a(number_of_lines), b(number_of_lines);

    double loww=ui->doubleSpinBox_17->value();
    double hiw=ui->doubleSpinBox_18->value(), mean=0, signr=0;
    int points=0;

    for (int i=0; i<number_of_lines; i++){
    toplot1 >> one >>two;
    istringstream ist(one);
    ist >> a[i];
    istringstream ist2(two);
    ist2 >> b[i];
    if(a[i]>=loww & a[i]<=hiw){
        mean +=b[i];
        ++points;
    }
    }
    toplot1.close();

    mean = mean/points;

    cout<<"mean: "<<mean<<endl;

    for(int i=0; i<number_of_lines; i++){
        if(a[i]>=loww & a[i]<=hiw){
            signr+=pow((b[i]-mean),2);
        }
    }

    signr = sqrt(signr/(points-1));

    signr = mean/signr;

    ui->doubleSpinBox_15->setValue(abs(signr));
}

//*******************************************************
// compute EW in specified region
//*******************************************************
void PlotSpec::EW(){

}

//*******************************************************
//aplly spline fit
//*******************************************************
void PlotSpec::on_pushButton_6_clicked()
{
    ui->customPlot->clearGraphs();

    int points=0;
    string line, eins, zwei;

    ifstream sdat("spline.dat");

    while(std::getline(sdat, line))
       ++ points;

    sdat.clear();
    sdat.seekg(0, ios::beg);

    if(points < 3){
        QMessageBox::information(this, "Error", "Not enough data points for cubic spline fit.");
        return;
    }

    if(points != clicks){
        QMessageBox::information(this, "Error", "Old data in file. Reset data first.");
        return;
    }


    vector<double> XC(points), YC(points);
    QVector<double> Xc(points), Yc(points);

    for (int i=0; i<points; i++){
    sdat >> eins >>zwei;
    istringstream ist(eins);
    ist >> XC[i];
    Xc[i]=XC[i];
    istringstream ist2(zwei);
    ist2 >> YC[i];
    Yc[i]=YC[i];
    }
    sdat.close();

    s.set_points(XC,YC);

    QString in1=ui->lineEdit->text();
    string in11 = in1.toUtf8().constData();
    std::ostringstream datNameStream(in11);
    datNameStream<<spPath<<"/"<<in11;
    std::string datName = datNameStream.str();
    ifstream toplot1(datName.c_str());

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+in1+" does not exist!");
       return;
    }

    slength=0;
    double x;

    while(std::getline(toplot1, zeile))
       ++ slength;

    toplot1.clear();
    toplot1.seekg(0, ios::beg);

    asc.resize(slength);
    bsc.resize(slength);
    sc.resize(slength);

    ofstream sline("splineline.dat");

    for (int i=0; i<slength; i++){
    toplot1 >> one >>two;
    istringstream ist(one);
    ist >> asc[i];
    x=asc[i];
    sline<<x<<" "<<s(x)<<endl;
    istringstream ist2(two);
    ist2 >> bsc[i];
    if(ui->checkBox_13->isChecked()){
    bsc[i]=bsc[i]/s(x);
    }
    if(ui->checkBox_14->isChecked()){
        bsc[i]=bsc[i]+(1/(ui->doubleSpinBox_19->value()+1)-s(x));
    }
    }
    toplot1.close();

    QString in2=ui->lineEdit_7->text();
    string in12 = in2.toUtf8().constData();
    std::ostringstream dat2NameStream(in12);
    dat2NameStream<<spPath<<"/"<<in12;
    std::string dat2Name = dat2NameStream.str();
    ifstream toplot2(dat2Name.c_str());

    QFile checkfile2(dat2Name.c_str());

    if(!checkfile2.exists()){
        qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+in2+" does not exist!");
       return;
    }

    slength=0;

    while(std::getline(toplot2, zeile))
       ++ slength;

    toplot2.clear();
    toplot2.seekg(0, ios::beg);

    csc.resize(slength);
    dsc.resize(slength);
    sc.resize(slength);

    ofstream sline2("splineline2.dat");

    for (int i=0; i<slength; i++){
    toplot2 >> one >>two;
    istringstream ist(one);
    ist >> csc[i];
    x=csc[i];
    sline2<<x<<" "<<s(x)<<endl;
    istringstream ist2(two);
    ist2 >> dsc[i];
    if(ui->checkBox_13->isChecked()){
    dsc[i]=dsc[i]/s(x);
    }
    if(ui->checkBox_14->isChecked()){
        dsc[i]=dsc[i]+(s(x)-1/(ui->doubleSpinBox_19->value()+1));
    }
    }
    toplot1.close();

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(asc, bsc);
    ui->customPlot->xAxis->rescale(true);
    ui->customPlot->yAxis->rescale(true);
    ui->customPlot->replot();
}

//*******************************************************
//clear spline data
//*******************************************************
void PlotSpec::on_pushButton_7_clicked()
{
    spline.close();
    spline.open("spline.dat", fstream::out);

    for(int i=0; i<clicks; i++){
        spline<<endl;
    }

    spline.clear();
    spline.seekp(0, ios::beg);

    clicks=0;

}

//*******************************************************
//show spline
//*******************************************************
void PlotSpec::on_pushButton_8_clicked()
{
    ui->customPlot->clearGraphs();

    ifstream input("splineline.dat");
    string zeilen, eins, zwei;

    QVector<double> f(slength), g(slength);

    for(int i=0; i<slength; i++){
        input >> eins >> zwei;
        istringstream stri(eins);
        stri >> f[i];
        istringstream stri1(zwei);
        stri1 >> g[i];
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(f, g);
    ui->customPlot->graph(0)->setPen(QPen(Qt::black));
    ui->customPlot->xAxis->rescale();
    ui->customPlot->yAxis->rescale();
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(aps, bps);
    ui->customPlot->xAxis->rescale(true);
    ui->customPlot->yAxis->rescale(true);
    ui->customPlot->replot();
}

//*******************************************************
//write normalized data to file
//*******************************************************
void PlotSpec::on_pushButton_9_clicked()
{
    QString res = ui->lineEdit_8->text();
    string res1 = res.toUtf8().constData();
    std::ostringstream datNameStream(res1);
    datNameStream<<spPath<<"/"<<res1;
    std::string datName = datNameStream.str();
    ofstream out(datName.c_str());

    for(int i=0; i<asc.size(); i++){
        out<<asc[i]<<" "<<bsc[i]<<endl;
    }

    QString res2 = ui->lineEdit_9->text();
    string res12 = res2.toUtf8().constData();
    std::ostringstream dat2NameStream(res12);
    dat2NameStream<<spPath<<"/"<<res12;
    std::string dat2Name = dat2NameStream.str();
    ofstream out2(dat2Name.c_str());

    for(int i=0; i<asc.size(); i++){
        out2<<csc[i]<<" "<<dsc[i]<<endl;
    }
}

//*******************************************************
// Save spline data
//*******************************************************
void PlotSpec::on_pushButton_10_clicked()
{
    string line, eins, zwei;

    ifstream spdat("spline.dat");

    int spoin=0;

    while(std::getline(spdat, line))
       ++ spoin;

    spdat.clear();
    spdat.seekg(0, ios::beg);

    QVector<double> ass(spoin), bss(spoin);

    for (int i=0; i<spoin; i++){
    spdat >> eins >>zwei;
    istringstream ist(eins);
    ist >> ass[i];
    istringstream ist2(zwei);
    ist2 >> bss[i];
    }
    spdat.close();

    QString res = ui->lineEdit_10->text();
    string res1 = res.toUtf8().constData();
    std::ostringstream datNameStream(res1);
    datNameStream<<spPath<<"/"<<res1;
    std::string datName = datNameStream.str();
    ofstream out(datName.c_str());

    for(int i=0; i<ass.size(); i++){
        out<<ass[i]<<" "<<bss[i]<<endl;
    }

}

//*******************************************************
// Load spline data
//*******************************************************
void PlotSpec::on_pushButton_11_clicked()
{
    string line, eins, zwei;

    QString res = ui->lineEdit_11->text();
    string res1 = res.toUtf8().constData();
    std::ostringstream datNameStream(res1);
    datNameStream<<spPath<<"/"<<res1;
    std::string datName = datNameStream.str();
    ifstream out(datName.c_str());

    int spoin=0;

    while(std::getline(out, line))
       ++ spoin;

    out.clear();
    out.seekg(0, ios::beg);

    QVector<double> ass(spoin), bss(spoin);

    for (int i=0; i<spoin; i++){
    out >> eins >>zwei;
    istringstream ist(eins);
    ist >> ass[i];
    istringstream ist2(zwei);
    ist2 >> bss[i];
    }
    out.close();

    ofstream spdat("spline.dat");

    for(int i=0; i<ass.size(); i++){
        spdat<<ass[i]<<" "<<bss[i]<<endl;
    }

}

//*******************************************************
// push button SNR
//*******************************************************
void PlotSpec::on_pushButton_12_clicked()
{
    PlotSpec::SNR();
}
