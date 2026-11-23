/********************************************************************************
** Form generated from reading UI file 'orbitfit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORBITFIT_H
#define UI_ORBITFIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_OrbitFit
{
public:
    QHBoxLayout *hMain;
    QVBoxLayout *vLeft;
    QGroupBox *grpData;
    QGridLayout *grdData;
    QCheckBox *checkBox_main;
    QCheckBox *checkBox_file;
    QLineEdit *lineEdit;
    QLabel *lbl_1;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_off;
    QLabel *lbl_2;
    QLineEdit *lineEdit_2;
    QLabel *labelData;
    QGroupBox *grpStart;
    QGridLayout *grdStart;
    QLabel *lbl_3;
    QLineEdit *lineEdit_17;
    QCheckBox *checkBox_3;
    QLabel *lbl_6;
    QLineEdit *lineEdit_18;
    QCheckBox *checkBox_4;
    QLabel *lbl_9;
    QLineEdit *lineEdit_19;
    QCheckBox *checkBox_5;
    QLabel *lbl_12;
    QLineEdit *lineEdit_20;
    QCheckBox *checkBox_6;
    QLabel *lbl_15;
    QLineEdit *lineEdit_21;
    QCheckBox *checkBox_7;
    QLabel *lbl_18;
    QLineEdit *lineEdit_22;
    QCheckBox *checkBox_8;
    QLabel *lbl_21;
    QLineEdit *lineEdit_23;
    QCheckBox *checkBox_9;
    QPushButton *pushButton_load;
    QLabel *lbl_24;
    QSpinBox *spinBox;
    QGroupBox *grpResult;
    QGridLayout *grdResult;
    QLabel *lbl_4;
    QLineEdit *lineEdit_3;
    QLabel *lbl_5;
    QLineEdit *lineEdit_10;
    QLabel *lbl_7;
    QLineEdit *lineEdit_4;
    QLabel *lbl_8;
    QLineEdit *lineEdit_11;
    QLabel *lbl_10;
    QLineEdit *lineEdit_5;
    QLabel *lbl_11;
    QLineEdit *lineEdit_12;
    QLabel *lbl_13;
    QLineEdit *lineEdit_6;
    QLabel *lbl_14;
    QLineEdit *lineEdit_13;
    QLabel *lbl_16;
    QLineEdit *lineEdit_7;
    QLabel *lbl_17;
    QLineEdit *lineEdit_14;
    QLabel *lbl_19;
    QLineEdit *lineEdit_8;
    QLabel *lbl_20;
    QLineEdit *lineEdit_15;
    QLabel *lbl_22;
    QLineEdit *lineEdit_9;
    QLabel *lbl_23;
    QLineEdit *lineEdit_16;
    QLabel *labelStats;
    QHBoxLayout *hButtons;
    QPushButton *pushButton_2;
    QPushButton *pushButton_write;
    QSpacerItem *sp_25;
    QPushButton *pushButton;
    QVBoxLayout *vRight;
    QCustomPlot *customPlot;
    QCustomPlot *customPlot_res;

    void setupUi(QDialog *OrbitFit)
    {
        if (OrbitFit->objectName().isEmpty())
            OrbitFit->setObjectName(QString::fromUtf8("OrbitFit"));
        OrbitFit->resize(1100, 720);
        hMain = new QHBoxLayout(OrbitFit);
        hMain->setObjectName(QString::fromUtf8("hMain"));
        vLeft = new QVBoxLayout();
        vLeft->setObjectName(QString::fromUtf8("vLeft"));
        grpData = new QGroupBox(OrbitFit);
        grpData->setObjectName(QString::fromUtf8("grpData"));
        grdData = new QGridLayout(grpData);
        grdData->setObjectName(QString::fromUtf8("grdData"));
        checkBox_main = new QCheckBox(grpData);
        checkBox_main->setObjectName(QString::fromUtf8("checkBox_main"));

        grdData->addWidget(checkBox_main, 0, 0, 1, 2);

        checkBox_file = new QCheckBox(grpData);
        checkBox_file->setObjectName(QString::fromUtf8("checkBox_file"));

        grdData->addWidget(checkBox_file, 1, 0, 1, 1);

        lineEdit = new QLineEdit(grpData);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        grdData->addWidget(lineEdit, 1, 1, 1, 3);

        lbl_1 = new QLabel(grpData);
        lbl_1->setObjectName(QString::fromUtf8("lbl_1"));

        grdData->addWidget(lbl_1, 2, 0, 1, 1);

        checkBox = new QCheckBox(grpData);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        grdData->addWidget(checkBox, 2, 1, 1, 1);

        checkBox_2 = new QCheckBox(grpData);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        grdData->addWidget(checkBox_2, 2, 2, 1, 1);

        checkBox_off = new QCheckBox(grpData);
        checkBox_off->setObjectName(QString::fromUtf8("checkBox_off"));

        grdData->addWidget(checkBox_off, 2, 3, 1, 1);

        lbl_2 = new QLabel(grpData);
        lbl_2->setObjectName(QString::fromUtf8("lbl_2"));

        grdData->addWidget(lbl_2, 3, 0, 1, 1);

        lineEdit_2 = new QLineEdit(grpData);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        grdData->addWidget(lineEdit_2, 3, 1, 1, 3);

        labelData = new QLabel(grpData);
        labelData->setObjectName(QString::fromUtf8("labelData"));

        grdData->addWidget(labelData, 4, 0, 1, 4);


        vLeft->addWidget(grpData);

        grpStart = new QGroupBox(OrbitFit);
        grpStart->setObjectName(QString::fromUtf8("grpStart"));
        grdStart = new QGridLayout(grpStart);
        grdStart->setObjectName(QString::fromUtf8("grdStart"));
        lbl_3 = new QLabel(grpStart);
        lbl_3->setObjectName(QString::fromUtf8("lbl_3"));
        lbl_3->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_3, 0, 0, 1, 1);

        lineEdit_17 = new QLineEdit(grpStart);
        lineEdit_17->setObjectName(QString::fromUtf8("lineEdit_17"));

        grdStart->addWidget(lineEdit_17, 0, 1, 1, 1);

        checkBox_3 = new QCheckBox(grpStart);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        grdStart->addWidget(checkBox_3, 0, 2, 1, 1);

        lbl_6 = new QLabel(grpStart);
        lbl_6->setObjectName(QString::fromUtf8("lbl_6"));
        lbl_6->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_6, 1, 0, 1, 1);

        lineEdit_18 = new QLineEdit(grpStart);
        lineEdit_18->setObjectName(QString::fromUtf8("lineEdit_18"));

        grdStart->addWidget(lineEdit_18, 1, 1, 1, 1);

        checkBox_4 = new QCheckBox(grpStart);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

        grdStart->addWidget(checkBox_4, 1, 2, 1, 1);

        lbl_9 = new QLabel(grpStart);
        lbl_9->setObjectName(QString::fromUtf8("lbl_9"));
        lbl_9->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_9, 2, 0, 1, 1);

        lineEdit_19 = new QLineEdit(grpStart);
        lineEdit_19->setObjectName(QString::fromUtf8("lineEdit_19"));

        grdStart->addWidget(lineEdit_19, 2, 1, 1, 1);

        checkBox_5 = new QCheckBox(grpStart);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));

        grdStart->addWidget(checkBox_5, 2, 2, 1, 1);

        lbl_12 = new QLabel(grpStart);
        lbl_12->setObjectName(QString::fromUtf8("lbl_12"));
        lbl_12->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_12, 3, 0, 1, 1);

        lineEdit_20 = new QLineEdit(grpStart);
        lineEdit_20->setObjectName(QString::fromUtf8("lineEdit_20"));

        grdStart->addWidget(lineEdit_20, 3, 1, 1, 1);

        checkBox_6 = new QCheckBox(grpStart);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

        grdStart->addWidget(checkBox_6, 3, 2, 1, 1);

        lbl_15 = new QLabel(grpStart);
        lbl_15->setObjectName(QString::fromUtf8("lbl_15"));
        lbl_15->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_15, 4, 0, 1, 1);

        lineEdit_21 = new QLineEdit(grpStart);
        lineEdit_21->setObjectName(QString::fromUtf8("lineEdit_21"));

        grdStart->addWidget(lineEdit_21, 4, 1, 1, 1);

        checkBox_7 = new QCheckBox(grpStart);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));

        grdStart->addWidget(checkBox_7, 4, 2, 1, 1);

        lbl_18 = new QLabel(grpStart);
        lbl_18->setObjectName(QString::fromUtf8("lbl_18"));
        lbl_18->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_18, 5, 0, 1, 1);

        lineEdit_22 = new QLineEdit(grpStart);
        lineEdit_22->setObjectName(QString::fromUtf8("lineEdit_22"));

        grdStart->addWidget(lineEdit_22, 5, 1, 1, 1);

        checkBox_8 = new QCheckBox(grpStart);
        checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));

        grdStart->addWidget(checkBox_8, 5, 2, 1, 1);

        lbl_21 = new QLabel(grpStart);
        lbl_21->setObjectName(QString::fromUtf8("lbl_21"));
        lbl_21->setTextFormat(Qt::RichText);

        grdStart->addWidget(lbl_21, 6, 0, 1, 1);

        lineEdit_23 = new QLineEdit(grpStart);
        lineEdit_23->setObjectName(QString::fromUtf8("lineEdit_23"));

        grdStart->addWidget(lineEdit_23, 6, 1, 1, 1);

        checkBox_9 = new QCheckBox(grpStart);
        checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));

        grdStart->addWidget(checkBox_9, 6, 2, 1, 1);

        pushButton_load = new QPushButton(grpStart);
        pushButton_load->setObjectName(QString::fromUtf8("pushButton_load"));

        grdStart->addWidget(pushButton_load, 7, 0, 1, 3);

        lbl_24 = new QLabel(grpStart);
        lbl_24->setObjectName(QString::fromUtf8("lbl_24"));

        grdStart->addWidget(lbl_24, 8, 0, 1, 1);

        spinBox = new QSpinBox(grpStart);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(10000);
        spinBox->setValue(200);

        grdStart->addWidget(spinBox, 8, 1, 1, 1);


        vLeft->addWidget(grpStart);

        grpResult = new QGroupBox(OrbitFit);
        grpResult->setObjectName(QString::fromUtf8("grpResult"));
        grdResult = new QGridLayout(grpResult);
        grdResult->setObjectName(QString::fromUtf8("grdResult"));
        lbl_4 = new QLabel(grpResult);
        lbl_4->setObjectName(QString::fromUtf8("lbl_4"));
        lbl_4->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_4, 0, 0, 1, 1);

        lineEdit_3 = new QLineEdit(grpResult);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setReadOnly(true);

        grdResult->addWidget(lineEdit_3, 0, 1, 1, 1);

        lbl_5 = new QLabel(grpResult);
        lbl_5->setObjectName(QString::fromUtf8("lbl_5"));

        grdResult->addWidget(lbl_5, 0, 2, 1, 1);

        lineEdit_10 = new QLineEdit(grpResult);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));
        lineEdit_10->setReadOnly(true);

        grdResult->addWidget(lineEdit_10, 0, 3, 1, 1);

        lbl_7 = new QLabel(grpResult);
        lbl_7->setObjectName(QString::fromUtf8("lbl_7"));
        lbl_7->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_7, 1, 0, 1, 1);

        lineEdit_4 = new QLineEdit(grpResult);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setReadOnly(true);

        grdResult->addWidget(lineEdit_4, 1, 1, 1, 1);

        lbl_8 = new QLabel(grpResult);
        lbl_8->setObjectName(QString::fromUtf8("lbl_8"));

        grdResult->addWidget(lbl_8, 1, 2, 1, 1);

        lineEdit_11 = new QLineEdit(grpResult);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));
        lineEdit_11->setReadOnly(true);

        grdResult->addWidget(lineEdit_11, 1, 3, 1, 1);

        lbl_10 = new QLabel(grpResult);
        lbl_10->setObjectName(QString::fromUtf8("lbl_10"));
        lbl_10->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_10, 2, 0, 1, 1);

        lineEdit_5 = new QLineEdit(grpResult);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setReadOnly(true);

        grdResult->addWidget(lineEdit_5, 2, 1, 1, 1);

        lbl_11 = new QLabel(grpResult);
        lbl_11->setObjectName(QString::fromUtf8("lbl_11"));

        grdResult->addWidget(lbl_11, 2, 2, 1, 1);

        lineEdit_12 = new QLineEdit(grpResult);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));
        lineEdit_12->setReadOnly(true);

        grdResult->addWidget(lineEdit_12, 2, 3, 1, 1);

        lbl_13 = new QLabel(grpResult);
        lbl_13->setObjectName(QString::fromUtf8("lbl_13"));
        lbl_13->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_13, 3, 0, 1, 1);

        lineEdit_6 = new QLineEdit(grpResult);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setReadOnly(true);

        grdResult->addWidget(lineEdit_6, 3, 1, 1, 1);

        lbl_14 = new QLabel(grpResult);
        lbl_14->setObjectName(QString::fromUtf8("lbl_14"));

        grdResult->addWidget(lbl_14, 3, 2, 1, 1);

        lineEdit_13 = new QLineEdit(grpResult);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));
        lineEdit_13->setReadOnly(true);

        grdResult->addWidget(lineEdit_13, 3, 3, 1, 1);

        lbl_16 = new QLabel(grpResult);
        lbl_16->setObjectName(QString::fromUtf8("lbl_16"));
        lbl_16->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_16, 4, 0, 1, 1);

        lineEdit_7 = new QLineEdit(grpResult);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setReadOnly(true);

        grdResult->addWidget(lineEdit_7, 4, 1, 1, 1);

        lbl_17 = new QLabel(grpResult);
        lbl_17->setObjectName(QString::fromUtf8("lbl_17"));

        grdResult->addWidget(lbl_17, 4, 2, 1, 1);

        lineEdit_14 = new QLineEdit(grpResult);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));
        lineEdit_14->setReadOnly(true);

        grdResult->addWidget(lineEdit_14, 4, 3, 1, 1);

        lbl_19 = new QLabel(grpResult);
        lbl_19->setObjectName(QString::fromUtf8("lbl_19"));
        lbl_19->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_19, 5, 0, 1, 1);

        lineEdit_8 = new QLineEdit(grpResult);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setReadOnly(true);

        grdResult->addWidget(lineEdit_8, 5, 1, 1, 1);

        lbl_20 = new QLabel(grpResult);
        lbl_20->setObjectName(QString::fromUtf8("lbl_20"));

        grdResult->addWidget(lbl_20, 5, 2, 1, 1);

        lineEdit_15 = new QLineEdit(grpResult);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));
        lineEdit_15->setReadOnly(true);

        grdResult->addWidget(lineEdit_15, 5, 3, 1, 1);

        lbl_22 = new QLabel(grpResult);
        lbl_22->setObjectName(QString::fromUtf8("lbl_22"));
        lbl_22->setTextFormat(Qt::RichText);

        grdResult->addWidget(lbl_22, 6, 0, 1, 1);

        lineEdit_9 = new QLineEdit(grpResult);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setReadOnly(true);

        grdResult->addWidget(lineEdit_9, 6, 1, 1, 1);

        lbl_23 = new QLabel(grpResult);
        lbl_23->setObjectName(QString::fromUtf8("lbl_23"));

        grdResult->addWidget(lbl_23, 6, 2, 1, 1);

        lineEdit_16 = new QLineEdit(grpResult);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));
        lineEdit_16->setReadOnly(true);

        grdResult->addWidget(lineEdit_16, 6, 3, 1, 1);

        labelStats = new QLabel(grpResult);
        labelStats->setObjectName(QString::fromUtf8("labelStats"));
        labelStats->setWordWrap(true);

        grdResult->addWidget(labelStats, 7, 0, 1, 4);


        vLeft->addWidget(grpResult);

        hButtons = new QHBoxLayout();
        hButtons->setObjectName(QString::fromUtf8("hButtons"));
        pushButton_2 = new QPushButton(OrbitFit);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        hButtons->addWidget(pushButton_2);

        pushButton_write = new QPushButton(OrbitFit);
        pushButton_write->setObjectName(QString::fromUtf8("pushButton_write"));

        hButtons->addWidget(pushButton_write);

        sp_25 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hButtons->addItem(sp_25);

        pushButton = new QPushButton(OrbitFit);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        hButtons->addWidget(pushButton);


        vLeft->addLayout(hButtons);


        hMain->addLayout(vLeft);

        vRight = new QVBoxLayout();
        vRight->setObjectName(QString::fromUtf8("vRight"));
        customPlot = new QCustomPlot(OrbitFit);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setMinimumSize(QSize(420, 260));

        vRight->addWidget(customPlot);

        customPlot_res = new QCustomPlot(OrbitFit);
        customPlot_res->setObjectName(QString::fromUtf8("customPlot_res"));
        customPlot_res->setMinimumSize(QSize(420, 140));

        vRight->addWidget(customPlot_res);


        hMain->addLayout(vRight);


        retranslateUi(OrbitFit);

        QMetaObject::connectSlotsByName(OrbitFit);
    } // setupUi

    void retranslateUi(QDialog *OrbitFit)
    {
        OrbitFit->setWindowTitle(QCoreApplication::translate("OrbitFit", "Orbit fit", nullptr));
        grpData->setTitle(QCoreApplication::translate("OrbitFit", "Radial velocities", nullptr));
        checkBox_main->setText(QCoreApplication::translate("OrbitFit", "From the main window", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_main->setToolTip(QCoreApplication::translate("OrbitFit", "Times of observation (times file of the Data tab, or the orbit's times) and the current RVs of the main window, e.g. after an optimisation of individual RVs.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_file->setText(QCoreApplication::translate("OrbitFit", "From file", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit->setToolTip(QCoreApplication::translate("OrbitFit", "Columns: time, RV_A [, RV_B [, sigma_A, sigma_B]] (km/s), file in the work folder.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_1->setText(QCoreApplication::translate("OrbitFit", "System", nullptr));
        checkBox->setText(QCoreApplication::translate("OrbitFit", "SB1", nullptr));
        checkBox_2->setText(QCoreApplication::translate("OrbitFit", "SB2", nullptr));
        checkBox_off->setText(QCoreApplication::translate("OrbitFit", "Own zero point for B (offset)", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_off->setToolTip(QCoreApplication::translate("OrbitFit", "Fit an offset of the RVs of B. Use it for RVs from disentangling without static lines: there the zero point of each component is arbitrary (the mean RVs keep their start values).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_2->setText(QCoreApplication::translate("OrbitFit", "Work folder", nullptr));
        labelData->setText(QString());
        grpStart->setTitle(QCoreApplication::translate("OrbitFit", "Start orbit", nullptr));
        lbl_3->setText(QCoreApplication::translate("OrbitFit", "P [d]", nullptr));
        checkBox_3->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        lbl_6->setText(QCoreApplication::translate("OrbitFit", "e", nullptr));
        checkBox_4->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        lbl_9->setText(QCoreApplication::translate("OrbitFit", "K<sub>A</sub> [km/s]", nullptr));
        checkBox_5->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        lbl_12->setText(QCoreApplication::translate("OrbitFit", "K<sub>B</sub> [km/s]", nullptr));
        checkBox_6->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        lbl_15->setText(QCoreApplication::translate("OrbitFit", "\316\263 [km/s]", nullptr));
        checkBox_7->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        lbl_18->setText(QCoreApplication::translate("OrbitFit", "T<sub>peri</sub> [d]", nullptr));
        checkBox_8->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        lbl_21->setText(QCoreApplication::translate("OrbitFit", "\317\211<sub>A</sub> [deg]", nullptr));
        checkBox_9->setText(QCoreApplication::translate("OrbitFit", "fixed", nullptr));
        pushButton_load->setText(QCoreApplication::translate("OrbitFit", "Load orbital elements", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_load->setToolTip(QCoreApplication::translate("OrbitFit", "Start values from the orbital elements file of the main window (Data tab, default orbitelements.dat) in the work folder (omega converted to degrees).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_24->setText(QCoreApplication::translate("OrbitFit", "Max. iterations", nullptr));
        grpResult->setTitle(QCoreApplication::translate("OrbitFit", "Fitted orbit", nullptr));
        lbl_4->setText(QCoreApplication::translate("OrbitFit", "P [d]", nullptr));
        lbl_5->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        lbl_7->setText(QCoreApplication::translate("OrbitFit", "e", nullptr));
        lbl_8->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        lbl_10->setText(QCoreApplication::translate("OrbitFit", "K<sub>A</sub> [km/s]", nullptr));
        lbl_11->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        lbl_13->setText(QCoreApplication::translate("OrbitFit", "K<sub>B</sub> [km/s]", nullptr));
        lbl_14->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        lbl_16->setText(QCoreApplication::translate("OrbitFit", "\316\263 [km/s]", nullptr));
        lbl_17->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        lbl_19->setText(QCoreApplication::translate("OrbitFit", "T<sub>peri</sub> [d]", nullptr));
        lbl_20->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        lbl_22->setText(QCoreApplication::translate("OrbitFit", "\317\211<sub>A</sub> [deg]", nullptr));
        lbl_23->setText(QCoreApplication::translate("OrbitFit", "\302\261", nullptr));
        labelStats->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("OrbitFit", "Fit", nullptr));
        pushButton_write->setText(QCoreApplication::translate("OrbitFit", "Write orbital elements", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_write->setToolTip(QCoreApplication::translate("OrbitFit", "Writes the fitted elements to the orbital elements file of the main window (Data tab; start for the orbit mode of the main window).", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton->setText(QCoreApplication::translate("OrbitFit", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OrbitFit: public Ui_OrbitFit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORBITFIT_H
