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

Edit::Edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edit)
{
    ui->setupUi(this);

    this->setWindowTitle("Editor");

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Ratio");

    ui->tableWidget_2->setColumnCount(1);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"Ratio");

    ui->tableWidget_3->setColumnCount(1);
    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList()<<"Ratio");

    ui->doubleSpinBox->setValue(1.0);
    ui->doubleSpinBox_2->setValue(0.5);
    ui->doubleSpinBox_3->setValue(0.5);
    ui->doubleSpinBox_4->setValue(2.0);
    ui->doubleSpinBox_5->setValue(1);

    ui->doubleSpinBox_6->setValue(104.02128);
    ui->doubleSpinBox_7->setValue(0.00089);
    ui->doubleSpinBox_8->setValue(25.9611);
    ui->doubleSpinBox_9->setValue(26.840);
    ui->doubleSpinBox_10->setValue(29.9378);
    ui->doubleSpinBox_11->setValue(48147.6);
    ui->doubleSpinBox_12->setValue(342.6);

    ui->comboBox->addItem("Capella");
    ui->comboBox->addItem("Mizar");
    ui->comboBox->addItem("BAT99_12");
    ui->comboBox->addItem("BAT99_19");
    ui->comboBox->addItem("BAT99_32");
    ui->comboBox->addItem("BAT99_77");
    ui->comboBox->addItem("BAT99_92");
    ui->comboBox->addItem("BAT99_95");
    ui->comboBox->addItem("BAT99_99");
    ui->comboBox->addItem("BAT99_103");
    ui->comboBox->addItem("BAT99_113");
    ui->comboBox->addItem("BAT99_119");
    ui->comboBox->addItem("NGC 3603-A1");
    ui->comboBox->addItem("NN Del");
    ui->comboBox->addItem("PX Hya");
    ui->comboBox->addItem("HR5110");

    ui->lineEdit_3->setText("/home/daniels/work1/Observations/Capella/Lithium");
    qEPath=ui->lineEdit_3->text();
    ePath = qEPath.toUtf8().constData();

}

Edit::~Edit()
{
    delete ui;
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

    for(int i=0; i<obser; i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget->setItem(i,0,new QTableWidgetItem("1.0"));

        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem("1.0"));

        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem("1"));
        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem("1.0"));
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
    ofstream edit("edit.dat");


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
    remove("edit.dat");
}

//************************************************
// apply optimization coeff.
//************************************************
void Edit::on_pushButton_6_clicked()
{
    ofstream opt("optcoef.dat");

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
    remove("optcoef.dat");
}

//*********************************************
// apply initial orbit paramters
//*********************************************
void Edit::on_pushButton_8_clicked()
{
    ofstream orb("orbitelements.dat");

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
    remove("orbitelements.dat");
}

//********************************************
// apply orbit from data base
//********************************************
void Edit::on_pushButton_10_clicked()
{
    // Capella
    if(ui->comboBox->currentIndex()==0){
        ui->doubleSpinBox_6->setValue(104.02128);
        ui->doubleSpinBox_7->setValue(0.00089);
        ui->doubleSpinBox_8->setValue(25.9611);
        ui->doubleSpinBox_9->setValue(26.8400);
        ui->doubleSpinBox_10->setValue(29.9378);
        ui->doubleSpinBox_11->setValue(48147.6);
        ui->doubleSpinBox_12->setValue(342.6);
    }

    // Mizar
    if(ui->comboBox->currentIndex()==1){
        ui->doubleSpinBox_6->setValue(20.54);
        ui->doubleSpinBox_7->setValue(0.537);
        ui->doubleSpinBox_8->setValue(68.6);
        ui->doubleSpinBox_9->setValue(67.6);
        ui->doubleSpinBox_10->setValue(-5.6);
        ui->doubleSpinBox_11->setValue(36997);
        ui->doubleSpinBox_12->setValue(106.16);
    }

    // BAT99_12
    if(ui->comboBox->currentIndex()==2){
        ui->doubleSpinBox_6->setValue(3.2358);
        ui->doubleSpinBox_7->setValue(0.34);
        ui->doubleSpinBox_8->setValue(74);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(646);
        ui->doubleSpinBox_11->setValue(52269.84);
        ui->doubleSpinBox_12->setValue(-29);
    }

    // BAT99_19
    if(ui->comboBox->currentIndex()==3){
        ui->doubleSpinBox_6->setValue(17.9947);
        ui->doubleSpinBox_7->setValue(0.34);
        ui->doubleSpinBox_8->setValue(234);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(328);
        ui->doubleSpinBox_11->setValue(52006.6737);
        ui->doubleSpinBox_12->setValue(188.2);
    }

    // BAT99_32
    if(ui->comboBox->currentIndex()==4){
        ui->doubleSpinBox_6->setValue(1.90756);
        ui->doubleSpinBox_7->setValue(0.06);
        ui->doubleSpinBox_8->setValue(120);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(288);
        ui->doubleSpinBox_11->setValue(53011.57);
        ui->doubleSpinBox_12->setValue(250);
    }

    // BAT99_77
    if(ui->comboBox->currentIndex()==5){
        ui->doubleSpinBox_6->setValue(3.00303);
        ui->doubleSpinBox_7->setValue(0.32);
        ui->doubleSpinBox_8->setValue(144);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(339);
        ui->doubleSpinBox_11->setValue(52631.87);
        ui->doubleSpinBox_12->setValue(7);
    }

    // BAT99_92
    if(ui->comboBox->currentIndex()==6){
        ui->doubleSpinBox_6->setValue(4.3125);
        ui->doubleSpinBox_7->setValue(0.02);
        ui->doubleSpinBox_8->setValue(204);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(332);
        ui->doubleSpinBox_11->setValue(52998.03);
        ui->doubleSpinBox_12->setValue(109);
    }

    // BAT99_95
    if(ui->comboBox->currentIndex()==7){
        ui->doubleSpinBox_6->setValue(2.1110);
        ui->doubleSpinBox_7->setValue(0.07);
        ui->doubleSpinBox_8->setValue(107);
        ui->doubleSpinBox_9->setValue(107);
        ui->doubleSpinBox_10->setValue(274);
        ui->doubleSpinBox_11->setValue(52999.87);
        ui->doubleSpinBox_12->setValue(285);
    }

    // BAT99_99
    if(ui->comboBox->currentIndex()==8){
        ui->doubleSpinBox_6->setValue(92.60);
        ui->doubleSpinBox_7->setValue(0);
        ui->doubleSpinBox_8->setValue(91);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(337);
        ui->doubleSpinBox_11->setValue(53007.8);
        ui->doubleSpinBox_12->setValue(0);
    }


    // BAT99_103
    if(ui->comboBox->currentIndex()==9){
        ui->doubleSpinBox_6->setValue(2.75975);
        ui->doubleSpinBox_7->setValue(0.23);
        ui->doubleSpinBox_8->setValue(158);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(388);
        ui->doubleSpinBox_11->setValue(53007.8);
        ui->doubleSpinBox_12->setValue(-41);
    }

    // BAT99_113
    if(ui->comboBox->currentIndex()==10){
        ui->doubleSpinBox_6->setValue(4.699);
        ui->doubleSpinBox_7->setValue(0.20);
        ui->doubleSpinBox_8->setValue(130);
        ui->doubleSpinBox_9->setValue(0);
        ui->doubleSpinBox_10->setValue(390);
        ui->doubleSpinBox_11->setValue(52993.07);
        ui->doubleSpinBox_12->setValue(308);
    }

    // BAT99_119
    if(ui->comboBox->currentIndex()==11){
        ui->doubleSpinBox_6->setValue(159.62413);
        ui->doubleSpinBox_7->setValue(0.8060);
        ui->doubleSpinBox_8->setValue(103.6355);
        ui->doubleSpinBox_9->setValue(117.0775);
        ui->doubleSpinBox_10->setValue(270);
        ui->doubleSpinBox_11->setValue(56501.2889);
        ui->doubleSpinBox_12->setValue(230.82);
    }

    // NGC 3603-A1
    if(ui->comboBox->currentIndex()==12){
        ui->doubleSpinBox_6->setValue(3.7724);
        ui->doubleSpinBox_7->setValue(0.0000);
        ui->doubleSpinBox_8->setValue(330);
        ui->doubleSpinBox_9->setValue(433);
        ui->doubleSpinBox_10->setValue(153);
        ui->doubleSpinBox_11->setValue(3765.25);
        ui->doubleSpinBox_12->setValue(120);
    }

    // NN Del
    if(ui->comboBox->currentIndex()==13){
        ui->doubleSpinBox_6->setValue(99.1648477);
        ui->doubleSpinBox_7->setValue(0.5177);
        ui->doubleSpinBox_8->setValue(36.2432);
        ui->doubleSpinBox_9->setValue(39.4582);
        ui->doubleSpinBox_10->setValue(-9.4046);
        ui->doubleSpinBox_11->setValue(2456787.1095);
        ui->doubleSpinBox_12->setValue(350.5696);
    }

    // PX Hya
    if(ui->comboBox->currentIndex()==14){
        ui->doubleSpinBox_6->setValue(36.1828518);
        ui->doubleSpinBox_7->setValue(0.5652);
        ui->doubleSpinBox_8->setValue(57.6967);
        ui->doubleSpinBox_9->setValue(56.5568);
        ui->doubleSpinBox_10->setValue(-14.9896);
        ui->doubleSpinBox_11->setValue(2456743.2661);
        ui->doubleSpinBox_12->setValue(325.1146);
    }

    // HR5110
    if(ui->comboBox->currentIndex()==15){
        ui->doubleSpinBox_6->setValue(2.613214);
        ui->doubleSpinBox_7->setValue(0.00);
        ui->doubleSpinBox_8->setValue(8.9);
        ui->doubleSpinBox_9->setValue(5.0);
        ui->doubleSpinBox_10->setValue(8.6);
        ui->doubleSpinBox_11->setValue(2445766.655);
        ui->doubleSpinBox_12->setValue(89);
    }
}


//Save
void Edit::on_pushButton_4_clicked()
{
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

// Load
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

// apply initial steps size
void Edit::on_pushButton_11_clicked()
{
    ofstream init("initialstep.dat");

    init<<setprecision(10)<<ui->doubleSpinBox_5->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_13->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_14->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_15->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_16->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_17->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_18->value()<<endl;
    init<<setprecision(10)<<ui->doubleSpinBox_19->value()<<endl;
}

// remove initial step size
void Edit::on_pushButton_12_clicked()
{
    remove("initialstep.dat");
}
