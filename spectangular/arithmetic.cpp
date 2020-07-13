#include "arithmetic.h"
#include "ui_arithmetic.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QMessageBox>
#include <QFile>
#include <iostream>
#include <iomanip>


using namespace std;

QString qAPath;
string aPath;

Arithmetic::Arithmetic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Arithmetic)
{
    ui->setupUi(this);

    this->setWindowTitle("Arithmetic");
}

Arithmetic::~Arithmetic()
{
    delete ui;
}

void Arithmetic::seData(QString str, QString str1, QString str2)
{
    ui->lineEdit_4->setText(str);
    qAPath=ui->lineEdit_4->text();
    aPath = qAPath.toUtf8().constData();
    ui->lineEdit_6->setText(str1);
    ui->lineEdit_8->setText(str2);
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

//*************************************
// separate Differences
//*************************************
void Arithmetic::on_pushButton_3_clicked()
{
    string eins, zwei, line;
    int dlines1=0, ndiff=0;

    QString inputA=ui->lineEdit_4->text()+"/"+ui->lineEdit_6->text();
    string dataA = inputA.toUtf8().constData();
    std::ostringstream datANameStream(dataA);
    datANameStream<<dataA;
    std::string datAName = datANameStream.str();

    QFile checkfile1(datAName.c_str());

    if(!checkfile1.exists()){
        QMessageBox::information(this, "Error1 ", "Error 1: Spectrum "+checkfile1.fileName()+ " does not exist!");
        this->setCursor(QCursor(Qt::ArrowCursor));
       return;
    }

    ifstream inA(datAName.c_str());

    while(std::getline(inA, line))
       ++dlines1;

    inA.clear();
    inA.seekg(0, ios::beg);

    QVector<double> diffw(dlines1), diffi(dlines1);
    int counter=0;
    double test=0;

    for(int i =0; i<dlines1; i++){

        inA >> eins >> zwei;
        istringstream str(eins);
        istringstream str2(zwei);
        str >> test;

        if(test>1){
            diffw[counter] = test;
            str2 >> diffi[counter];

            if((i>0) & (counter>=1)){
                if(diffw[counter]<diffw[counter-1]){
                    ++ndiff;
                }
            }

            else{
                // do nothing
            }
            ++counter;
        }
        else{
            // do nothing
        }

    }

    dlines1=counter;
    counter=0;
    diffw.resize(dlines1);
    diffi.resize(dlines1);

    //cout<<"ndiff:"<<ndiff<<endl;

    vector<int> pdiff(ndiff+1);
    pdiff[0]=1;
    counter=0;

        for(int i =0; i<dlines1; i++){
            if(counter==0){
                if(i>0){
                    if(diffw[i]<diffw[i-1]){
                        pdiff[counter]=i;
                        //cout<<pdiff[counter]<<endl;
                        ++counter;
                    }
                }
            }
            else{
                if((i>pdiff[counter-1]-1) & (i>0)){
                    if(diffw[i]<diffw[i-1]){
                        pdiff[counter]=i;
                        //cout<<pdiff[counter]<<endl;
                        ++counter;
                    }
                }
            }
        }
        pdiff[ndiff]=dlines1-1;
        //cout<<pdiff[ndiff+1]<<endl;


    for(int i =0; i<=ndiff; i++){
        //cout<<i<<"\t"<<pdiff[i]<<endl;
        counter=0;

        QString outputA=ui->lineEdit_4->text()+"/"+ui->lineEdit_7->text();
        QString qExt = ui->lineEdit_8->text();
        string ext = qExt.toUtf8().constData();
        string outA = outputA.toUtf8().constData();
        std::ostringstream outANameStream(outA);
        outANameStream<<outA<<i<<"."<<ext;
        std::string outAName = outANameStream.str();
        ofstream ouA(outAName.c_str());

        for(int n =0; n<pdiff[i]; n++){
            if(i==0){
                ouA<<diffw[n]<<"\t"<<diffi[n]<<endl;
            }
            else{
                if(n>=pdiff[i-1]){
                    ouA<<setprecision(14)<<diffw[pdiff[i-1]+counter]<<"\t"<<diffi[pdiff[i-1]+counter]<<endl;
                    ++counter;
                }
                else{
                    // no nothing
                }
            }
        }
    }
}
