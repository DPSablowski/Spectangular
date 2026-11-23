#include "correlation.h"
#include <limits>
#include <algorithm>
#include "ui_correlation.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QMessageBox>
#include <QFile>
#include <iostream>

using namespace std;

string pathCor;
QString qPathCor;
double samplingCor;
double CorShift=0;   // shift in pixels, may be fractional (M7)
int shift, corer=0;

correlation::correlation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::correlation)
{
    ui->setupUi(this);

    this->setWindowTitle("Correlation");

    ui->lineEdit_2->setText("subA_shift_aver.dat");
    ui->lineEdit_5->setText("txt");

    ui->lineEdit_5->setEnabled(false);
    ui->spinBox_3->setEnabled(false);
    ui->spinBox_4->setEnabled(false);
}

correlation::~correlation()
{
    delete ui;
}

void correlation::seData(QString str, QString str2)
{
    ui->lineEdit_4->setText(str);
    ui->lineEdit->setText(str2);
    qPathCor=ui->lineEdit_4->text();
    pathCor = qPathCor.toUtf8().constData();
}

//***************************
//correlation button
//***************************
void correlation::on_pushButton_2_clicked()
{
    // sequence
    if(ui->checkBox_4->isChecked()){
        QString qFile = ui->lineEdit->text();
        QString qExt = "."+ui->lineEdit_5->text();

        int min = ui->spinBox_3->value();
        int max = ui->spinBox_4->value();

        for(int i=min; i<max+1; i++){
            ui->lineEdit->setText(qFile+QString::number(i)+qExt);
            correlation::Do_correlation();
            qApp->processEvents(QEventLoop::AllEvents);
            if(corer==1){
                corer=0;
                return;
            }
            else{
                ui->lineEdit_3->setText(qFile+"shift_"+QString::number(i)+qExt);
                correlation::on_pushButton_4_clicked();
            }
        }
        ui->lineEdit->setText(qFile);
    }
    // single file
    else{
        correlation::Do_correlation();
    }
}

//**********************
// correlation
//**********************
void correlation::Do_correlation()
{
    this->setCursor(QCursor(Qt::WaitCursor));
    corer=0;

    shift=ui->spinBox->value();
    int numpix1, numpix2;
    string line, eins1, zwei1, eins2, zwei2;

    QString input=ui->lineEdit->text();
    string data1 = input.toUtf8().constData();
    std::ostringstream dat1NameStream(data1);
    dat1NameStream<<pathCor<<"/"<<data1;
    std::string dat1Name = dat1NameStream.str();

    QFile checkfile1(dat1Name.c_str());

    if(!checkfile1.exists()){
        QMessageBox::information(this, "Error", "File"+qPathCor+"/"+input+" does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
        corer=1;
       return;
    }
    ifstream dat1(dat1Name.c_str());

    numpix1=0;

    while(std::getline(dat1, line))
       ++numpix1;

    dat1.clear();
    dat1.seekg(0, ios::beg);

    QString input2=ui->lineEdit_2->text();
    string data2 = input2.toUtf8().constData();
    std::ostringstream dat2NameStream(data2);
    dat2NameStream<<pathCor<<"/"<<data2;
    std::string dat2Name = dat2NameStream.str();
    ifstream dat2(dat2Name.c_str());

    QFile checkfile2(dat2Name.c_str());

    if(!checkfile2.exists()){
        QMessageBox::information(this, "Error", "File"+qPathCor+"/"+input2+" does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
        corer=1;
       return;
    }

    numpix2=0;

    while(std::getline(dat2, line))
       ++numpix2;

    dat2.clear();
    dat2.seekg(0, ios::beg);

    /*
    if(numpix1!=numpix2){
        QMessageBox::information(this, "Error", "Files do not have the same pixel numbers!");
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;
    }
    */


    QVector<double> a(numpix1), b(numpix1), c(numpix2), d(numpix2);



    double cont1=ui->doubleSpinBox_2->value();
    double cont2=ui->doubleSpinBox_3->value();

    for(int g=0; g<numpix1; g++){
        dat1 >> eins1 >>zwei1;
        istringstream ist(eins1);
        ist >> a[g];
        if(ui->checkBox_2->isChecked()){
            a[g]=log10(a[g]);
        }
        istringstream ist2(zwei1);
        ist2 >> b[g];
        b[g]=b[g]-cont1;
    }

     for(int g=0; g<numpix2; g++){
        dat2 >> eins2 >>zwei2;
        istringstream ist3(eins2);
        ist3 >> c[g];
        if(ui->checkBox_3->isChecked()){
            c[g]=log10(c[g]);
        }
        istringstream ist4(zwei2);
        ist4 >>d[g];
        d[g]=d[g]-cont2;
     }

     if(ui->checkBox->isChecked()){
         // average pairs of adjacent pixels (previously step 3 and no shortening, M6)
         for(int g=0; g<numpix2/2; g++){
             c[g]=(c[2*g]+c[2*g+1])/2;
             d[g]=(d[2*g]+d[2*g+1])/2;
         }
         numpix2=numpix2/2;
     }

     ofstream file2((pathCor+"/cropeds.txt").c_str());
     ofstream file3((pathCor+"/cropedt.txt").c_str());

     int counter=0;

     if(numpix1<2 || numpix2<2){
         QMessageBox::information(this, "Error", "Spectrum or template too short.");
         this->setCursor(QCursor(Qt::ArrowCursor));
         return;
     }
     samplingCor=(a[numpix1-1]-a[0])/(numpix1-1);

     // establish a common start; the shortened length is kept (H5)
     if(a[0]<c[0]){
         for(int i=0; i<numpix1; i++){
             if(a[i]>=c[0]){
                 a[counter]=a[i];
                 b[counter]=b[i];
                 file2<<a[counter]<<" "<<b[counter]<<endl;
                 ++counter;
             }
         }
         numpix1=counter;
     }
     else if(a[0]>c[0]){
         for(int i=0; i<numpix2; i++){
             if(c[i]>=a[0]){
                 c[counter]=c[i];
                 d[counter]=d[i];
                 file3<<c[counter]<<" "<<d[counter]<<endl;
                 ++counter;
             }
         }
         numpix2=counter;
     }
     const int n = std::min(numpix1, numpix2);
     if(n<2){
         QMessageBox::information(this, "Error", "Spectrum and template do not overlap.");
         this->setCursor(QCursor(Qt::ArrowCursor));
         return;
     }

     QVector<double> ccf(2*shift+1);

     string file = "correlation.txt";
     std::ostringstream fileNameStream(file);
     fileNameStream<<pathCor<<"/"<<file;
     std::string fileName = fileNameStream.str();
     ofstream file1(fileName.c_str());

     double meant=0;
     double means=0;
     for(int i=0; i<n; i++){
        means+=b[i]/n;
        meant+=d[i]/n;
     }

     double sigmas=0;
     double sigmat=0;
     for(int i=0; i<n; i++){
        sigmas+=pow((b[i]-means),2);
        sigmat+=pow((d[i]-meant),2);
     }

     sigmat=sqrt(sigmat/(n-1));
     sigmas=sqrt(sigmas/(n-1));

     // shifts -shift ... +shift; normalization to the respective overlap (M5);
     // start value -infinity, so that a negative CCF also has a maximum (H6)
     double ccfmax=-std::numeric_limits<double>::infinity();
     CorShift=shift;

     for(int i=0; i<=2*shift; i++){
         ccf[i]=0;
         int overlap=0;
         for(int g=0; g<n; g++){
             int k = g-(i-shift);
             if((k>=0) && (k<n)){
                 ccf[i]+=(b[g]-means)*(d[k]-meant);
                 ++overlap;
             }
         }
         if(overlap>0) ccf[i]/=overlap*sigmat*sigmas;
         file1<<i-shift<<" "<<ccf[i]<<endl;
         if(ccf[i]>ccfmax){
             ccfmax=ccf[i];
             CorShift=i;
         }

     }
     cout<<CorShift-shift<<"\t";

     ui->doubleSpinBox->setValue(CorShift-shift);
     ui->doubleSpinBox_4->setValue(samplingCor*(CorShift-shift));

    this->setCursor(QCursor(Qt::ArrowCursor));

}

//********************************
// correct
//********************************
void correlation::on_pushButton_4_clicked()
{
    string line, eins1, zwei1;

    CorShift=ui->doubleSpinBox->value()+ui->doubleSpinBox_5->value();

    QString input=ui->lineEdit->text();
    string data1 = input.toUtf8().constData();
    std::ostringstream dat1NameStream(data1);
    dat1NameStream<<pathCor<<"/"<<data1;
    std::string dat1Name = dat1NameStream.str();

    QFile checkfile1(dat1Name.c_str());

    if(!checkfile1.exists()){
        QMessageBox::information(this, "Error", "File"+qPathCor+"/"+input+" does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
       return;
    }
    ifstream dat1(dat1Name.c_str());

    int numpix1=0;

    while(std::getline(dat1, line))
       ++numpix1;

    dat1.clear();
    dat1.seekg(0, ios::beg);

    QVector<double> a(numpix1), b(numpix1);

    for(int g=0; g<numpix1; g++){
        dat1 >> eins1 >>zwei1;
        istringstream ist(eins1);
        ist >> a[g];
        if(ui->checkBox_2->isChecked()){
             a[g]=log10(a[g]);
        }
        else{
            //
        }
        istringstream ist2(zwei1);
        ist2 >> b[g];
    }

    if(numpix1<2){
        QMessageBox::information(this, "Error", "Spectrum too short.");
        return;
    }
    samplingCor=(a[numpix1-1]-a[0])/(numpix1-1);   // as in Do_correlation

    QString output=ui->lineEdit_3->text();
    string file1 = output.toUtf8().constData();
    std::ostringstream file1NameStream(file1);
    file1NameStream<<pathCor<<"/"<<file1;
    std::string file1Name = file1NameStream.str();
    ofstream file(file1Name.c_str());

    for(int i=0; i<numpix1; i++){
        a[i]=a[i]-samplingCor*CorShift;
        if(ui->checkBox_2->isChecked()){
            a[i]=pow(10,a[i]);
        }
        else{
            //
        }
        file<<a[i]<<"\t"<<b[i]<<endl;
    }
    cout<<a[5]<<endl;
}

void correlation::on_lineEdit_4_textEdited()
{
    qPathCor=ui->lineEdit_4->text();
    pathCor = qPathCor.toUtf8().constData();
}

void correlation::on_checkBox_4_clicked()
{
    if(ui->checkBox_4->isChecked()){
        ui->lineEdit_5->setEnabled(true);
        ui->spinBox_3->setEnabled(true);
        ui->spinBox_4->setEnabled(true);
    }
    else{
        ui->lineEdit_5->setEnabled(false);
        ui->spinBox_3->setEnabled(false);
        ui->spinBox_4->setEnabled(false);
    }
}
