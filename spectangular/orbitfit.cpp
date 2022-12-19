#include "orbitfit.h"
#include "ui_orbitfit.h"
#include <QFile>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QMessageBox>
#include <iostream>
#include <iomanip>
#include <QFileInfo>

using namespace std;

QString qOFPath;
string OFPath;

QVector<double> tfit(1), RVAfit(1), RVBfit(1);
int numfit=0, SBfit=0;


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

//***************************
// read RVs and Times
//***************************
void OrbitFit::readData()
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

        QFile checkfile(datName.c_str());

        if(!checkfile.exists()){
            QMessageBox::information(this, "Error", "File"+qOFPath+"/"+input+" does not exist!");
            this->setCursor(QCursor(Qt::ArrowCursor));
           return;
        }
        ifstream inp(datName.c_str());

        numfit=0;
        SBfit=0;

        string line, eins, zwei;

        while(std::getline(inp, line))
           ++numfit;

        inp.clear();
        inp.seekg(0, ios::beg);

        tfit.resize(numfit);
        RVAfit.resize(numfit);

        for(int g=0; g<numfit; g++){
            inp >> eins >>zwei;
            istringstream ist(eins);
            ist >> tfit[g];
            istringstream ist2(zwei);
            ist2 >> RVAfit[g];
        }

        inp.close();

    }

    // SB2 data
    else if(ui->checkBox_2->isChecked()){

        QString input=ui->lineEdit->text();
        string in = input.toUtf8().constData();
        std::ostringstream datNameStream(in);
        datNameStream<<OFPath<<"/"<<in;
        std::string datName = datNameStream.str();

        QFile checkfile(datName.c_str());

        if(!checkfile.exists()){
            QMessageBox::information(this, "Error", "File"+qOFPath+"/"+input+" does not exist!");
            this->setCursor(QCursor(Qt::ArrowCursor));
           return;
        }
        ifstream inp(datName.c_str());

        numfit=0;
        SBfit=1;

        string line, eins, zwei, drei;

        while(std::getline(inp, line))
           ++numfit;

        inp.clear();
        inp.seekg(0, ios::beg);

        tfit.resize(numfit);
        RVAfit.resize(numfit);
        RVBfit.resize(numfit);


        for(int g=0; g<numfit; g++){
            inp >> eins >>zwei>>drei;
            istringstream ist(eins);
            ist >> tfit[g];
            istringstream ist2(zwei);
            ist2 >> RVAfit[g];
            istringstream ist3(drei);
            ist3>>RVBfit[g];
        }

        inp.close();

    }
}

//******************************
// compute residuum
//******************************
double residuumfunction(double ele[]){

    if(SBfit==0){   // SB1
        for(int i=0; i<numfit; i++){

        }
    }

    else if(SBfit==1){  // SB2
        for(int i=0; i<numfit; i++){

        }
    }
}

//******************************
// Fit Orbital Elements
//******************************
void OrbitFit::on_pushButton_2_clicked(){

    QString qlFile = ui->lineEdit_2->text()+"/logile.dat";
    QFileInfo QLFile(qlFile);
    string lFile = qlFile.toUtf8().constData();
    std::ostringstream lFileNameStream(lFile);
    lFileNameStream<<lFile;
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
        LogFile<<"  Orbit Fit LogFile"<<endl;
        LogFile<<"******************************"<<endl;
        LogFile<<" "<<asctime(ts)<<endl;
    }

    int nu=7;
    int add=0;

    QVector<int> pvf(7);
    LogFile<<"Locked parameters: ";

    if(ui->checkBox_3->isChecked()){ // P
        pvf[0]=1; // fixed
        LogFile<<"P ";
        ++add;
    }
    else{
        pvf[0]=0; // free
    }
    if(ui->checkBox_4->isChecked()){ // e
        pvf[1]=1;
        LogFile<<"e ";
        ++add;
    }
    else{
        pvf[1]=0;
    }
    if(ui->checkBox_5->isChecked()){ // KA
        pvf[2]=1;
        LogFile<<"KA ";
        ++add;
    }
    else{
        pvf[2]=0;
    }
    if(ui->checkBox_6->isChecked()){ // KB
        pvf[3]=1;
        LogFile<<"KB ";
        ++add;
    }
    else{
        pvf[3]=0;
    }
    if(ui->checkBox_7->isChecked()){ // Gamma
        pvf[4]=1;
        LogFile<<"Gamma ";
        ++add;
    }
    else{
        pvf[4]=0;
    }
    if(ui->checkBox_8->isChecked()){ // T0
        pvf[5]=1;
        LogFile<<"T0 ";
        ++add;
    }
    else{
        pvf[5]=0;
    }
    if(ui->checkBox_9->isChecked()){ // Omega
        pvf[6]=1;
        LogFile<<"Omega ";
        ++add;
    }
    else{
        pvf[6]=0;
    }

    nu = nu-add;

    double y[nu+1], P[nu+1][nu], Z[nu], Co[nu], So[nu], Eo[nu], e[nu+1][nu], X[nu];
    QVector<double> porbele(nu), ddorbele(nu), orbele(7), dorbele(7);

    orbele[0]=ui->lineEdit_17->text().toDouble();
    orbele[1]=ui->lineEdit_18->text().toDouble();
    orbele[2]=ui->lineEdit_19->text().toDouble();
    orbele[3]=ui->lineEdit_20->text().toDouble();
    orbele[4]=ui->lineEdit_21->text().toDouble();
    orbele[5]=ui->lineEdit_22->text().toDouble();
    orbele[6]=ui->lineEdit_23->text().toDouble();

    X[0]=orbele[0];
    X[1]=orbele[1];
    X[2]=orbele[2];
    X[3]=orbele[3];
    X[4]=orbele[4];
    X[5]=orbele[5];
    X[6]=orbele[6];

    dorbele[0]=orbele[0]/10;
    dorbele[1]=orbele[1]/10;
    dorbele[2]=orbele[2]/10;
    dorbele[3]=orbele[3]/10;
    dorbele[4]=orbele[4]/10;
    dorbele[5]=orbele[5]/10;
    dorbele[6]=orbele[6]/10;

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

    // create new initial simplex
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
            cout<<P[i][j]<<" ";
            LogFile<<P[i][j]<<" ";
        }
        LogFile<<endl;

        for(int e=0; e<7; e++){
            if(pvf[e]==0){
                X[e]=P[i][pcount];
                ++pcount;
            }
            else{
                //
            }
        }
        pcount=0;

        y[i]=residuumfunction(X);
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
