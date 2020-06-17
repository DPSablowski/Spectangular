#include "edit.h"
#include "ui_edit.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <QMessageBox>
#include <QFile>
#include <iostream>
#include <QDebug>
#include <iomanip>

using namespace std;

int obser;
QString qEPath;
string ePath;
QVector<double> PER(1), ECC(1), AMPA(1), AMPB(1), GAM(1), PERI(1), LPERI(1);

Edit::Edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);

    this->setWindowTitle("Editor");

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Depth");

    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"A");

    ui->tableWidget_3->setColumnCount(1);
    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<"B");

    ui->tableWidget_4->setColumnCount(1);
    ui->tableWidget_4->setHorizontalHeaderLabels(QStringList()<<"Ratio");

    ui->doubleSpinBox->setValue(1.0);
    ui->doubleSpinBox_2->setValue(0.5);
    ui->doubleSpinBox_3->setValue(0.5);
    ui->doubleSpinBox_4->setValue(2.0);
    ui->doubleSpinBox_5->setValue(1);

    string sbin = "BinaryData.dat";
    ifstream binaries(sbin.c_str());

    QFile qBin(sbin.c_str());

    if(!qBin.exists()){
        qDebug()<<"No data base file for orbits of binaries present.";
        QMessageBox::information(this, "Error", "Data base of orbital elements not present.");
    }


    else{
        int lines=0;
        string zeile1, eins1, zwei1, drei1, vier1, fuenf1, sechs1, sieben1, acht1;

        while(std::getline(binaries, zeile1))
        ++ lines;

        binaries.clear();
        binaries.seekg(0, ios::beg);

        QVector<string> names(lines);

        PER.resize(lines);
        ECC.resize(lines);
        AMPA.resize(lines);
        AMPB.resize(lines);
        GAM.resize(lines);
        PERI.resize(lines);
        LPERI.resize(lines);

        for(int i=0; i < lines; i++){
            binaries >> eins1 >> zwei1 >> drei1 >> vier1 >> fuenf1 >> sechs1 >> sieben1 >> acht1;
            istringstream str1(eins1);
            str1 >> names[i];
            QString qstr = QString::fromStdString(str1.str());
            ui->comboBox->addItem(qstr);
            istringstream str2(zwei1);
            str2 >> PER[i];
            istringstream str3(drei1);
            str3 >> ECC[i];
            istringstream str4(vier1);
            str4 >> AMPA[i];
            istringstream str5(fuenf1);
            str5 >> AMPB[i];
            istringstream str6(sechs1);
            str6 >> GAM[i];
            istringstream str7(sieben1);
            str7 >> PERI[i];
            istringstream str8(acht1);
            str8 >> LPERI[i];
        }

        ui->doubleSpinBox_6->setValue(PER[0]);
        ui->doubleSpinBox_7->setValue(ECC[0]);
        ui->doubleSpinBox_8->setValue(AMPA[0]);
        ui->doubleSpinBox_9->setValue(AMPB[0]);
        ui->doubleSpinBox_10->setValue(GAM[0]);
        ui->doubleSpinBox_11->setValue(PERI[0]);
        ui->doubleSpinBox_12->setValue(LPERI[0]);

    }

}

Edit::~Edit()
{
    delete ui;
}

void Edit::seData(QString str, int g, double k)
{
    ui->lineEdit_3->setText(str);
    ui->doubleSpinBox_20->setValue(k);
    ui->spinBox->setValue(g);
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();
}

//**************************************************
//set rows in tables
//**************************************************
void Edit::on_spinBox_valueChanged()
{
    obser=ui->spinBox->value();

    ui->tableWidget->setRowCount(obser);
    ui->tableWidget_2->setRowCount(obser);
    ui->tableWidget_3->setRowCount(obser);
    ui->tableWidget_4->setRowCount(obser);
    double FA = ui->doubleSpinBox_20->value()/(ui->doubleSpinBox_20->value()+1.0);
    double FB = 1.0/(ui->doubleSpinBox_20->value()+1.0);
    double k = ui->doubleSpinBox_20->value();

    for(int i=0; i<obser; i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("1.0"));

        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(QString::number(FA)));

        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem(QString::number(FB)));

        ui->tableWidget_4->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget_4->setItem(i,0,new QTableWidgetItem(QString::number(k)));
    }
}

//*************************************************
//work path
//*************************************************
void Edit::on_lineEdit_3_textChanged()
{
    qEPath = ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();
}

//***************************************************
//apply line strength button, write data to file
//***************************************************
void Edit::on_pushButton_2_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "edit.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    ofstream edit(dat1Name.c_str());

    obser=ui->spinBox->value();

    QTableWidgetItem *item;

    for(int i=0; i<3*obser; i++){
        if(i<obser){
        item=ui->tableWidget->item(i,0);
        }
        if(i>=obser & i<2*obser){
        item=ui->tableWidget_2->item(i-obser,0);
        }
        if(i>=2*obser){
        item=ui->tableWidget_3->item(i-2*obser,0);
        }
                edit<<item->text().toDouble()<<endl;
    }
}

//************************************************
//remove line strength data
//************************************************
void Edit::on_pushButton_3_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "edit.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    remove(dat1Name.c_str());
}

//************************************************
// apply optimization coeff.
//************************************************
void Edit::on_pushButton_6_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "optcoef.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    ofstream opt(dat1Name.c_str());

    opt<<ui->doubleSpinBox->value()<<endl;
    opt<<ui->doubleSpinBox_2->value()<<endl;
    opt<<ui->doubleSpinBox_3->value()<<endl;
    opt<<ui->doubleSpinBox_4->value()<<endl;

    opt.close();
}

//*********************************************
//remove optimization coeff.
//*********************************************
void Edit::on_pushButton_7_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "optcoef.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    remove(dat1Name.c_str());
}

//*********************************************
// apply initial orbit paramters
//*********************************************
void Edit::on_pushButton_8_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "orbitelements.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    ofstream orb(dat1Name.c_str());

    orb<<setprecision(14)<<ui->doubleSpinBox_6->value()<<endl;
    orb<<setprecision(14)<<ui->doubleSpinBox_7->value()<<endl;
    orb<<setprecision(14)<<ui->doubleSpinBox_8->value()<<endl;
    orb<<setprecision(14)<<ui->doubleSpinBox_9->value()<<endl;
    orb<<setprecision(14)<<ui->doubleSpinBox_10->value()<<endl;
    orb<<setprecision(14)<<ui->doubleSpinBox_11->value()<<endl;
    orb<<setprecision(14)<<ui->doubleSpinBox_12->value()/180*M_PI;

    orb.close();
}

//********************************************
//remove orbit elements file
//********************************************
void Edit::on_pushButton_9_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "orbitelements.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    remove(dat1Name.c_str());
}

//********************************************
// apply orbit from data base
//********************************************
void Edit::on_pushButton_10_clicked(){

    int dey=ui->comboBox->currentIndex();
    ui->doubleSpinBox_6->setValue(PER[dey]);
    ui->doubleSpinBox_7->setValue(ECC[dey]);
    ui->doubleSpinBox_8->setValue(AMPA[dey]);
    ui->doubleSpinBox_9->setValue(AMPB[dey]);
    ui->doubleSpinBox_10->setValue(GAM[dey]);
    ui->doubleSpinBox_11->setValue(PERI[dey]);
    ui->doubleSpinBox_12->setValue(LPERI[dey]);

}

//*****************************************
//Save
//*****************************************
void Edit::on_pushButton_4_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    QFile SCL(qEPath+"/"+ui->lineEdit->text()+".scl");
    QString qSCL = ui->lineEdit->text();
    string scl = qSCL.toUtf8().constData();
    std::ostringstream datANameStream(scl);
    datANameStream<<ePath<<"/"<<scl<<".scl";
    std::string datAName = datANameStream.str();

    if(SCL.exists()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning!", "The file already exists. \n\n Do you want to overwrite it?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";

          ofstream outEdit(datAName.c_str());

          outEdit<<setprecision(10)<<ui->spinBox->value()<<endl;          //number of spectra
          outEdit<<setprecision(10)<<ui->doubleSpinBox->value()<<endl;    // opt coeff 1
          outEdit<<setprecision(10)<<ui->doubleSpinBox_2->value()<<endl;  // opt coeff 2
          outEdit<<setprecision(10)<<ui->doubleSpinBox_3->value()<<endl;  // opt coeff 3
          outEdit<<setprecision(10)<<ui->doubleSpinBox_4->value()<<endl;  // opt coeff 4
          outEdit<<setprecision(10)<<ui->doubleSpinBox_5->value()<<endl;  // step size initial simplex
          outEdit<<setprecision(10)<<ui->doubleSpinBox_13->value()<<endl;  // delta P
          outEdit<<setprecision(10)<<ui->doubleSpinBox_14->value()<<endl;  // delta e
          outEdit<<setprecision(10)<<ui->doubleSpinBox_15->value()<<endl;  // delta K1
          outEdit<<setprecision(10)<<ui->doubleSpinBox_16->value()<<endl;  // delta K2
          outEdit<<setprecision(10)<<ui->doubleSpinBox_17->value()<<endl;  // delta systemic velocity
          outEdit<<setprecision(10)<<ui->doubleSpinBox_18->value()<<endl;  // delta T0
          outEdit<<setprecision(10)<<ui->doubleSpinBox_19->value()<<endl;  // delta omega

        } else {
          qDebug() << "Yes was *not* clicked";
          return;
        }
    }

    else{

        ofstream outEdit(datAName.c_str());

        outEdit<<setprecision(10)<<ui->spinBox->value()<<endl;          //number of spectra
        outEdit<<setprecision(10)<<ui->doubleSpinBox->value()<<endl;    // opt coeff 1
        outEdit<<setprecision(10)<<ui->doubleSpinBox_2->value()<<endl;  // opt coeff 2
        outEdit<<setprecision(10)<<ui->doubleSpinBox_3->value()<<endl;  // opt coeff 3
        outEdit<<setprecision(10)<<ui->doubleSpinBox_4->value()<<endl;  // opt coeff 4
        outEdit<<setprecision(10)<<ui->doubleSpinBox_5->value()<<endl;  // step size initial simplex
        outEdit<<setprecision(10)<<ui->doubleSpinBox_13->value()<<endl;  // delta P
        outEdit<<setprecision(10)<<ui->doubleSpinBox_14->value()<<endl;  // delta e
        outEdit<<setprecision(10)<<ui->doubleSpinBox_15->value()<<endl;  // delta K1
        outEdit<<setprecision(10)<<ui->doubleSpinBox_16->value()<<endl;  // delta K2
        outEdit<<setprecision(10)<<ui->doubleSpinBox_17->value()<<endl;  // delta systemic velocity
        outEdit<<setprecision(10)<<ui->doubleSpinBox_18->value()<<endl;  // delta T0
        outEdit<<setprecision(10)<<ui->doubleSpinBox_19->value()<<endl;  // delta omega
    }
}

//***************************************
// Load
//***************************************
void Edit::on_pushButton_5_clicked()
{
    QString qSCL = qEPath+"/"+ ui->lineEdit_2->text() +".scl";
    QFile SCL(qSCL);

    string scl = qSCL.toUtf8().constData();
    std::ostringstream datANameStream(scl);
    datANameStream<<scl;
    std::string datAName = datANameStream.str();

    if(!SCL.exists()){
        QMessageBox::information(this, "Error", "File "+ qSCL +" does not exist.");
        return;
    }
    else{
        int EditP = 13;
        QVector<double> EditV(EditP);

        ifstream inEdit(datAName.c_str());
        string eins;

        for(int g=0; g<EditP; g++){
        inEdit >> eins;
        istringstream ist(eins);
        ist >> EditV[g];
        }
        inEdit.close();

        ui->spinBox->setValue(EditV[0]);
        ui->doubleSpinBox->setValue(EditV[1]);
        ui->doubleSpinBox_2->setValue(EditV[2]);
        ui->doubleSpinBox_3->setValue(EditV[3]);
        ui->doubleSpinBox_4->setValue(EditV[4]);
        ui->doubleSpinBox_5->setValue(EditV[5]);
        ui->doubleSpinBox_13->setValue(EditV[6]);
        ui->doubleSpinBox_14->setValue(EditV[7]);
        ui->doubleSpinBox_15->setValue(EditV[8]);
        ui->doubleSpinBox_16->setValue(EditV[9]);
        ui->doubleSpinBox_17->setValue(EditV[10]);
        ui->doubleSpinBox_18->setValue(EditV[11]);
        ui->doubleSpinBox_19->setValue(EditV[12]);

    }
}

//********************************************
// apply initial steps size
//********************************************
void Edit::on_pushButton_11_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "initialstep.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    ofstream init(dat1Name.c_str());

    init<<setprecision(10)<<ui->doubleSpinBox_5->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_13->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_14->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_15->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_16->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_17->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_18->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_19->value()<<endl;
}

//****************************************
// remove initial step size
//****************************************
void Edit::on_pushButton_12_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "initialstep.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    remove(dat1Name.c_str());
}


//****************************************
// apply points for continuum definition
//****************************************
void Edit::on_pushButton_13_clicked()
{
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

    string sedit = "continuumpoints.dat";
    std::ostringstream dat1NameStream(sedit);
    dat1NameStream<<ePath<<"/"<<sedit;
    std::string dat1Name = dat1NameStream.str();
    ofstream init(dat1Name.c_str());

    QString qpoint = ui->lineEdit_4->text();
    double point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_1,1 of comp. A.");
        return;
    }
    else{
        init<<point<<endl;
    }

    qpoint =ui->lineEdit_5->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_1,2 of comp. A.");
        return;
    }
    else{
        init<<point<<endl;
    }

    qpoint = ui->lineEdit_6->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_1,1 of comp. B.");
        return;
    }
    else{
        init<<point<<endl;
    }

    qpoint =ui->lineEdit_7->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_1,2 of comp. B.");
        return;
    }
    else{
        init<<point<<endl;
    }

    qpoint = ui->lineEdit_8->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_2,1 of comp. A.");
        return;
    }
    else{
        init<<point<<endl;
    }

    qpoint =ui->lineEdit_9->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_2,2 of comp. A.");
        return;
    }
    else{
        init<<point<<endl;
    }
    qpoint =ui->lineEdit_10->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_2,1 of comp. B.");
        return;
    }
    else{
        init<<point<<endl;
    }
    qpoint =ui->lineEdit_11->text();
    point = qpoint.toDouble();
    if(qpoint.isEmpty()){
        QMessageBox::information(this, "Error", "Please enter point x_2,2 of comp. B.");
        return;
    }
    else{
        init<<point<<endl;
    }
}

//***************************************
// set relative strength by given ratios
//***************************************
void Edit::on_pushButton_14_clicked()
{
    QTableWidgetItem *wratio;
    QString qratio;
    double k, FA, FB;

    for(int i=0; i<obser; i++){

        wratio=ui->tableWidget_4->item(i,0);
        qratio=wratio->text();
        k = qratio.toDouble();

        FA = k/(k+1.0);
        FB = 1.0/(k+1.0);

        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(QString::number(FA)));

        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem(QString::number(FB)));
    }
}

//************************
// load ratios from file
//************************
void Edit::on_pushButton_15_clicked()
{
    QVector<double> k(1);

    QString qSCL = qEPath+"/"+ ui->lineEdit_12->text();
    QFile SCL(qSCL);

    string scl = qSCL.toUtf8().constData();
    std::ostringstream datANameStream(scl);
    datANameStream<<scl;
    std::string datAName = datANameStream.str();

    if(!SCL.exists()){
        QMessageBox::information(this, "Error", "File "+ qSCL +" does not exist.");
        return;
    }
    else{
        obser=0;
        ifstream inEdit(datAName.c_str());
        string eins, lines;

        while(std::getline(inEdit, lines))
              ++obser;

        inEdit.clear();
        inEdit.seekg(0, ios::beg);

        k.resize(obser);

        for(int g=0; g<obser; g++){
            inEdit >> eins;
            istringstream ist(eins);
            ist >> k[g];

        }
        inEdit.close();
    }
    ui->spinBox->setValue(obser);

    QTableWidgetItem *wratio;
    QString qratio;
    double FA, FB;

    for(int i=0; i<obser; i++){

        ui->tableWidget_4->setItem(i,0,new QTableWidgetItem(QString::number(k[i])));

        if(ui->checkBox->isChecked()){
            FB = k[i]/(k[i]+1.0);
            FA = 1.0/(k[i]+1.0);
        }
        else{
            FA = k[i]/(k[i]+1.0);
            FB = 1.0/(k[i]+1.0);
        }

        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(QString::number(FA)));

        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem(QString::number(FB)));
    }
}

//*******************************************
// Help button for component strength and continuum point definition
//*******************************************
void Edit::on_pushButton_16_clicked()
{
    QMessageBox::information(this, "Help", "In the upper three tables, you can specify the component strength for binary components A, B, and telluric (static) component. Thes strengths have to add to one in each row."
                                           "\nYou can also load a file with ratios (one column, one row per spectrum) with the Load Ratios button at top."
                                           "\nThe 8 line edits at bottom are used to spedify points in the disentangled spectra to compute a linear function. The slope of the function can be used for optimization on flux ratios."
                                           " Left two columns of these line edits for component A right for B, top ones for left, bottom for right point. See manual for more details.\n"
                                           "Hit the Apply button(s) to create a file with your definitions.\n"
                                           "Reset button will delete the file with the user defined values.");
}

//*****************************************
// Help button for transformation coeff. of simplex
//*****************************************
void Edit::on_pushButton_17_clicked()
{
    QMessageBox::information(this, "Halp", "The coef. for the transfomations of the simplex done by the Downhill-Simplex algorithm implemented, can be specified here. If not given, the default values given here are used."
                                           "\nHit the Apply button to create a file with your definitions."
                                           "\nReset button will delete the file with the user defined values.");
}

//****************************************
// Help button for step sizes to construct initial simplex
//****************************************
void Edit::on_pushButton_18_clicked()
{
    QMessageBox::information(this, "Help", "The optimization algorithm creates an initial simplex. This is done by specifying step sizes, added to the initial values of the corresponding variables n to construct a simplex n+1 points.\n"
                                           "Step size is used for optimization on radial velocities and is given here as multiples of the pixel size in velocity space (output on main window).\n"
                                           "Optimization on flux ratios uses the step size as well, but multiplied by 0.1."
                                           "\nHit the Apply button to create a file with your definitions."
                                           "\nReset button will delete the file with the user defined values.");
}

//****************************************
// Help button orbital parameters
//****************************************
void Edit::on_pushButton_19_clicked()
{
    QMessageBox::information(this, "Help", "The orbital elements used as start values for the optimization are set here.\n"
                                           "Orbital elements can also be loaded from the drop-down menu. Add an orbit to the file BinaryData.dat if not present in the list."
                                           "\nThis setting is mandatory for optimization on orbital elements!."
                                           "\nHit the Apply button to create a file with your definitions."
                                           "\nReset button will delete the file with the user defined values.");
}
