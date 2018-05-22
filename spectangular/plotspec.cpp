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

QString qSpPath, QSpline="spline.dat";
string spPath, spline="spline.dat";
QVector<double> aps(1), bps(1), cps(1), dps(1), eps(1), fps(1), d1ps(1), d2ps(1), asc(1), bsc(1), sc(1), csc(1), dsc(1), errw(1), erry(1);
int clicks=0, slength, EWstat=0;
double xc, yc;
tk::spline s;

PlotSpec::PlotSpec(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotSpec)
{
    ui->setupUi(this);

    this->setWindowTitle("Spectrum Plotter");

    ui->checkBox_16->setChecked(true);

    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(showPointToolTip(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(writeCoords(QMouseEvent*)));

    ui->customPlot->xAxis->setLabel("Wavelength [A]");
    ui->checkBox->setChecked(true);
    ui->checkBox_17->setChecked(true);

    ui->checkBox_14->setChecked(true);
    ui->doubleSpinBox_19->setValue(1);

    ui->lineEdit_8->setText("Ha_ad_2.txt");
    ui->lineEdit_9->setText("Ha_ad_1.txt");

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

void PlotSpec::seData(QString str, QString str2, QString str3, QString str4)
{
    ui->lineEdit_5->setText(str);
    ui->lineEdit->setText(str2);
    ui->lineEdit_7->setText(str3);
    ui->lineEdit_12->setText(str4);
    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();
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

    QString qlfile = qSpPath+"/"+QSpline;
    QFileInfo qlFile(qlfile);

    ostringstream spName(spline);
    spName<<spPath<<"/"<<spline;
    string spname = spName.str();

    ofstream spfile;

    if(ui->checkBox_16->isChecked()){

        if(qlFile.exists()){
            spfile.open(spname.c_str(), std::ios_base::app);
        }
        else{
            spfile.open(spname.c_str());
        }
        spfile<<xc<<" "<<yc<<endl;

        ++clicks;
    }
    else{
        if(EWstat==0){
        ui->doubleSpinBox_17->setValue(xc);
        ++EWstat;
        }
        else{
            EWstat=0;
            ui->doubleSpinBox_18->setValue(xc);
        }
    }

}


string one, two, zeile;
double xs1, xs2, ys1, ys2;

//****************************************************************
//plot spectrum
//****************************************************************
void PlotSpec::on_pushButton_2_clicked()
{
    ui->customPlot->clearGraphs();

    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

    QPen pen1, pen2, pen3, pen4, pen5;

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
    ui->customPlot->graph(0)->setData(aps, bps);
    pen1.setColor(Qt::blue);
    pen1.setWidth(ui->spinBox_2->value());
    ui->customPlot->graph(0)->setPen(pen1);

    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setData(cps, dps);
    pen2.setColor(Qt::red);
    pen2.setWidth(ui->spinBox_2->value());
    ui->customPlot->graph(1)->setPen(pen2);
    int n=2;

    if(ui->checkBox_15->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(n)->setData(errw, erry);
        pen5.setColor(Qt::black);
        pen5.setWidth(ui->spinBox_2->value());
        ui->customPlot->graph(n)->setPen(pen5);
        ++n;
    }

    if(ui->checkBox_9->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(n)->setData(aps, eps);
        pen3.setColor(Qt::black);
        pen3.setWidth(ui->spinBox_2->value());
        ui->customPlot->graph(n)->setPen(pen3);
        ++n;
    }
    if(ui->checkBox_10->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(n)->setData(cps, fps);
        pen4.setColor(Qt::green);
        pen4.setWidth(ui->spinBox_2->value());
        ui->customPlot->graph(n)->setPen(pen4);;
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
    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

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

    QString plot2=ui->lineEdit_13->text();
    string plot12 = plot2.toUtf8().constData();
    std::ostringstream dat2NameStream(plot12);
    dat2NameStream<<spPath<<"/"<<plot12;
    std::string dat2Name = dat2NameStream.str();
    ofstream toplot2(dat2Name.c_str());

    QString plot1=ui->lineEdit_14->text();
    string plot11 = plot1.toUtf8().constData();
    std::ostringstream datNameStream(plot11);
    datNameStream<<spPath<<"/"<<plot11;
    std::string datName = datNameStream.str();
    ofstream toplot1(datName.c_str());

    for(int i=0; i<aps.size(); i++){
        d1ps[i]=bps[i]+(1-eps[i]);
        toplot1<<aps[i]<<"\t"<<d1ps[i]<<endl;

    }

    for(int i=0; i<cps.size(); i++){
        d2ps[i]=dps[i]-(1-eps[i]);
        toplot2<<aps[i]<<"\t"<<d2ps[i]<<endl;
    }
    toplot1.close();
    toplot2.close();

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

    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

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
    double hiw=ui->doubleSpinBox_18->value(), mean=0, signr=0, wleft, wright, ileft, iright, steig, ab;
    int points=0;

    for (int i=0; i<number_of_lines; i++){
    toplot1 >> one >>two;
    istringstream ist(one);
    ist >> a[i];
    istringstream ist2(two);
    ist2 >> b[i];
    }
    toplot1.close();

    for(int i =0; i<number_of_lines; i++){
        if(a[i]<=loww & a[i+1]>loww){
            wleft=a[i];
            ileft=b[i];
        }
        if(a[i]<=hiw & a[i+1]>hiw){
            wright=a[i];
            iright=b[i];
        }
    }

    steig=(iright-ileft)/(wright-wleft);
    ab = ileft-steig*wleft;

    for(int i=0; i<number_of_lines; i++){
        if(a[i]>=loww & a[i]<=hiw){
            b[i]=b[i]+(ileft-(steig*a[i]+ab));
            mean +=b[i];
            ++points;
        }
    }

    mean = mean/points;

    cout<<"mean: "<<mean<<endl;

    for(int i=0; i<number_of_lines; i++){
        if(a[i]>=loww & a[i]<=hiw){
            signr+=pow((b[i]-mean),2);
        }
    }

    signr = sqrt(signr/(points-1));
    cout<<"stabw: "<<signr<<endl;

    signr = mean/signr;

    ui->doubleSpinBox_15->setValue(abs(signr));
}

//*******************************************************
// compute EW in specified region
//*******************************************************
void PlotSpec::EW(){

    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

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
    double hiw=ui->doubleSpinBox_18->value(), Wtot=0, Wline=0, Wew=0, Ilow=0, Ihigh=0, EW=0;
    int points=0;

    for (int i=0; i<number_of_lines; i++){
    toplot1 >> one >>two;
    istringstream ist(one);
    ist >> a[i];
    istringstream ist2(two);
    ist2 >> b[i];
    }
    toplot1.close();

    for(int i =1; i<number_of_lines; i++){
        if(a[i-1]<=loww & a[i]>=loww){
            Ilow=b[i];
        }
        else{
            Ilow=Ilow;
        }
        if(a[i-1]<=hiw & a[i]>=hiw){
            Ihigh = b[i];
        }
        else{
            Ihigh=Ihigh;
        }
        if(a[i-1]>loww & a[i-1]<hiw){
            Wline+=(a[i]-a[i-1])*(b[i]+b[i-1])/2;
        }
    }

    Wtot = (Ilow+Ihigh)/2*(hiw-loww);
    Wew = Wtot - Wline;
    EW = Wew/(Ilow+Ihigh)*2;
    cout<<"EW measurment:"<<endl;
    cout<<"I_low: "<<Ilow<<"\tI_high: "<<Ihigh<<endl;
    cout<<"W_tot: "<<Wtot<<"\tW_line: "<<Wline<<"\tW_ew: "<<Wew<<"\tEW: "<<EW<<endl;
    ui->doubleSpinBox_16->setValue(EW);

}

//*******************************************************
//aplly spline fit
//*******************************************************
void PlotSpec::on_pushButton_6_clicked()
{
    ui->customPlot->clearGraphs();

    int points=0;
    string line, eins, zwei;

    string st3line="spline.dat";
    ostringstream st3Name(st3line);
    st3Name<<spPath<<"/"<<st3line;
    string s3Name = st3Name.str();

    QFile checkfile0(s3Name.c_str());

    if(!checkfile0.exists()){
        qDebug()<<"The file "<<checkfile0.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+checkfile0.fileName()+" does not exist!");
       return;
    }

    ifstream sdat(s3Name.c_str());

    while(std::getline(sdat, line))
       ++ points;

    sdat.clear();
    sdat.seekg(0, ios::beg);

    if(points < 3){
        QMessageBox::information(this, "Error", "Not enough data points for cubic spline fit.");
        return;
    }

    /*
    if(points != clicks){
        QMessageBox::information(this, "Error", "Old data in file. Reset data first.");
        return;
    }*/


    vector<double> XC(points), YC(points);
    QVector<double> Xc(points), Yc(points);

    for (int i=0; i<points; i++){
        sdat >> eins >>zwei;
        istringstream ist(eins);
        ist >> XC[i];
        Xc[i]=XC[i];
        if((i>0) & (XC[i]<XC[i-1])){
            QMessageBox::information(this, "Error", "Spline data not ordered in increasing wavelength at position "+i);
            return;
        }
        istringstream ist2(zwei);
        ist2 >> YC[i];
        Yc[i]=YC[i];
    }
    sdat.close();

    s.set_points(XC,YC);

    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

    QString in1=ui->lineEdit->text();
    string in11 = in1.toUtf8().constData();
    std::ostringstream datNameStream(in11);
    datNameStream<<spPath<<"/"<<in11;
    std::string datName = datNameStream.str();

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+in1+" does not exist!");
       return;
    }
    ifstream toplot1(datName.c_str());

    slength=0;
    double x;

    while(std::getline(toplot1, zeile))
       ++ slength;

    toplot1.clear();
    toplot1.seekg(0, ios::beg);

    asc.resize(slength);
    bsc.resize(slength);
    sc.resize(slength);

    string st2line="splineline.dat";
    ostringstream st2Name(st2line);
    st2Name<<spPath<<"/"<<st2line;
    string s2Name = st2Name.str();
    ofstream sline(s2Name.c_str());

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

    if(ui->checkBox_17->isChecked()){
        QString in2=ui->lineEdit_7->text();
        string in12 = in2.toUtf8().constData();
        std::ostringstream dat2NameStream(in12);
        dat2NameStream<<spPath<<"/"<<in12;
        std::string dat2Name = dat2NameStream.str();

        QFile checkfile2(dat2Name.c_str());

        if(!checkfile2.exists()){
            qDebug()<<"The file "<<checkfile2.fileName()<<" does not exist.";
            QMessageBox::information(this, "Error", "File "+qSpPath+"/"+in2+" does not exist!");
            return;
        }

        ifstream toplot2(dat2Name.c_str());

        slength=0;

        while(std::getline(toplot2, zeile))
            ++slength;

        toplot2.clear();
        toplot2.seekg(0, ios::beg);

        csc.resize(slength);
        dsc.resize(slength);
        sc.resize(slength);

        string stline="splineline2.dat";
        ostringstream stName(stline);
        stName<<spPath<<"/"<<stline;
        string sName = stName.str();
        ofstream sline2(sName.c_str());

        for (int i=0; i<slength; i++){
            toplot2 >> one >>two;
            istringstream ist(one);
            ist >> csc[i];
            x=csc[i];
            sline2<<x<<" "<<s(x)<<endl;
            istringstream ist2(two);
            ist2 >> dsc[i];
            if(ui->checkBox_13->isChecked()){
                dsc[i]=dsc[i]/(2-s(x));
            }
            if(ui->checkBox_14->isChecked()){
                dsc[i]=dsc[i]+(s(x)-1/(ui->doubleSpinBox_19->value()+1));
            }
        }
        toplot1.close();
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(asc, bsc);
    ui->customPlot->xAxis->rescale(true);
    ui->customPlot->yAxis->rescale(true);
    if(ui->checkBox_17->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setData(csc, dsc);
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->xAxis->rescale(true);
        ui->customPlot->yAxis->rescale(true);
    }
    ui->customPlot->replot();
}

//*******************************************************
//clear spline data
//*******************************************************
void PlotSpec::on_pushButton_7_clicked()
{
    string stline="spline.dat";
    ostringstream stName(stline);
    stName<<spPath<<"/"<<stline;
    string sName = stName.str();
    remove(sName.c_str());

}

//*******************************************************
//show spline
//*******************************************************
void PlotSpec::on_pushButton_8_clicked()
{
    ui->customPlot->clearGraphs();

    string stline="splineline.dat";
    ostringstream stName(stline);
    stName<<spPath<<"/"<<stline;
    string sName = stName.str();

    QFile checkfile(sName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+checkfile.fileName()+" does not exist!");
        return;
    }

    ifstream input(sName.c_str());

    int ssp=0;
    string line;
    while(std::getline(input, line))
       ++ ssp;

    input.clear();
    input.seekg(0, ios::beg);

    string eins, zwei;

    QVector<double> f(ssp), g(ssp);

    for(int i=0; i<ssp; i++){
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
    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

    QString res = ui->lineEdit_8->text();
    string res1 = res.toUtf8().constData();
    std::ostringstream datNameStream(res1);
    datNameStream<<spPath<<"/"<<res1;
    std::string datName = datNameStream.str();

    QString res2 = ui->lineEdit_9->text();
    string res12 = res2.toUtf8().constData();
    std::ostringstream dat2NameStream(res12);
    dat2NameStream<<spPath<<"/"<<res12;
    std::string dat2Name = dat2NameStream.str();

     QFile QIn1(datName.c_str());
     QFile QIn2(dat2Name.c_str());

    if((QIn1.exists() or QIn2.exists())){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Warning!", "The files already exists. \n\n Do you want to overwrite it?",
                                  QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                qDebug() << "Overwrite file.";
                ofstream out(datName.c_str());
                ofstream out2(dat2Name.c_str());

                for(int i=0; i<asc.size(); i++){
                    out<<asc[i]<<" "<<bsc[i]<<endl;
                }

                for(int i=0; i<asc.size(); i++){
                    out2<<csc[i]<<" "<<dsc[i]<<endl;
                }
                out.close();
                out2.close();
            }
            else{
                qDebug()<< "Save file aborted.";
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }

    }

    else{
        ofstream out(datName.c_str());
        ofstream out2(dat2Name.c_str());

        for(int i=0; i<asc.size(); i++){
            out<<asc[i]<<" "<<bsc[i]<<endl;
        }

        for(int i=0; i<asc.size(); i++){
            out2<<csc[i]<<" "<<dsc[i]<<endl;
        }
        out.close();
        out2.close();
    }

}

//*******************************************************
// Save spline data
//*******************************************************
void PlotSpec::on_pushButton_10_clicked()
{
    string line, eins, zwei;

    string stline="spline.dat";
    ostringstream stName(stline);
    stName<<spPath<<"/"<<stline;
    string sName = stName.str();

    QFile checkfile(sName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+checkfile.fileName()+" does not exist!");
        return;
    }

    ifstream spdat(sName.c_str());

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

    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

    QString res = ui->lineEdit_10->text();
    string res1 = res.toUtf8().constData();
    std::ostringstream datNameStream(res1);
    datNameStream<<spPath<<"/"<<res1;
    std::string datName = datNameStream.str();

    QFile QIn1(datName.c_str());

   if(QIn1.exists() ){
           QMessageBox::StandardButton reply;
           reply = QMessageBox::question(this, "Warning!", "The files already exists. \n\n Do you want to overwrite it?",
                                 QMessageBox::Yes|QMessageBox::No);
           if (reply == QMessageBox::Yes) {
               qDebug() << "Overwrite file.";
               ofstream out(datName.c_str());

               for(int i=0; i<ass.size(); i++){
                   out<<ass[i]<<" "<<bss[i]<<endl;
               }
               out.close();
           }
           else{
               qDebug()<< "Save file aborted.";
               this->setCursor(QCursor(Qt::ArrowCursor));
               return;
           }

   }

   else{
       ofstream out(datName.c_str());

       for(int i=0; i<ass.size(); i++){
           out<<ass[i]<<" "<<bss[i]<<endl;
       }
       out.close();
   }



}

//*******************************************************
// Load spline data
//*******************************************************
void PlotSpec::on_pushButton_11_clicked()
{
    string line, eins, zwei;

    qSpPath=ui->lineEdit_5->text();
    spPath = qSpPath.toUtf8().constData();

    QString res = ui->lineEdit_11->text();
    string res1 = res.toUtf8().constData();
    std::ostringstream datNameStream(res1);
    datNameStream<<spPath<<"/"<<res1;
    std::string datName = datNameStream.str();

    QFile checkfile(datName.c_str());

    if(!checkfile.exists()){
        qDebug()<<"The file "<<checkfile.fileName()<<" does not exist.";
        QMessageBox::information(this, "Error", "File "+qSpPath+"/"+checkfile.fileName()+" does not exist!");
        return;
    }

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

    string stline="spline.dat";
    ostringstream stName(stline);
    stName<<spPath<<"/"<<stline;
    string sName = stName.str();
    ofstream spdat(sName.c_str());

    for(int i=0; i<ass.size(); i++){
        spdat<<ass[i]<<" "<<bss[i]<<endl;
    }
    spdat.close();

}

//*******************************************************
// push button SNR
//*******************************************************
void PlotSpec::on_pushButton_12_clicked()
{
    PlotSpec::SNR();
}

void PlotSpec::on_pushButton_13_clicked()
{
    PlotSpec::EW();
}

void PlotSpec::on_checkBox_14_clicked()
{
    if(ui->checkBox_14->isChecked()){
        ui->checkBox_13->setChecked(false);
    }
    else{
        ui->checkBox_13->setChecked(true);
    }
}

void PlotSpec::on_checkBox_13_clicked()
{
    if(ui->checkBox_13->isChecked()){
        ui->checkBox_14->setChecked(false);
    }
    else{
        ui->checkBox_14->setChecked(true);
    }
}
