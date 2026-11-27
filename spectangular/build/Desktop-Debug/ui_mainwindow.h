/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.18
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSpectrum_Plotter;
    QAction *actionAbout;
    QAction *actionBug_Report;
    QAction *actionCorrelation;
    QAction *actionEditor;
    QAction *actionArithmetic;
    QAction *actionOpen_Configuration;
    QAction *actionSave_Configuration;
    QAction *actionSave_Configuration_As;
    QAction *actionQuit;
    QAction *actionManual;
    QAction *actionSubtract;
    QAction *actionOrbitFit;
    QAction *actionFluxTemplate;
    QAction *actionBaryCorr;
    QAction *actionPeriod;
    QWidget *centralWidget;
    QVBoxLayout *vCentral;
    QHBoxLayout *hWorkflow;
    QPushButton *pushButton_2;
    QLabel *lbl_76;
    QPushButton *pushButton_3;
    QLabel *lbl_77;
    QPushButton *pushButton_4;
    QLabel *lbl_78;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QSpacerItem *sp_79;
    QLabel *lbl_80;
    QLineEdit *lineEdit_4;
    QHBoxLayout *hBody;
    QTabWidget *tabSettings;
    QWidget *tabData;
    QVBoxLayout *tabData_l;
    QScrollArea *tabData_s;
    QWidget *tabData_c;
    QVBoxLayout *tabData_v;
    QGroupBox *grpSpectra;
    QGridLayout *grdSpectra;
    QLabel *lbl_1;
    QLineEdit *lineEdit_3;
    QLabel *lbl_2;
    QSpinBox *spinBox;
    QLabel *lbl_3;
    QSpinBox *spinBox_2;
    QLineEdit *lineEdit_19;
    QLabel *lbl_4;
    QComboBox *comboBox;
    QLabel *lbl_5;
    QLineEdit *lineEdit_9;
    QLabel *lbl_6;
    QLineEdit *lineEdit_11;
    QLabel *lbl_7;
    QLineEdit *lineEdit_12;
    QCheckBox *checkBox_42;
    QDoubleSpinBox *doubleSpinBox_18;
    QGroupBox *grpSystem;
    QGridLayout *grdSystem;
    QLabel *lbl_8;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_34;
    QCheckBox *checkBox_8;
    QLabel *lbl_9;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *lbl_10;
    QLabel *lbl_11;
    QComboBox *comboBox_ratio;
    QLabel *lbl_12;
    QDoubleSpinBox *doubleSpinBox_lref;
    QLabel *lbl_13;
    QDoubleSpinBox *doubleSpinBox_TA;
    QDoubleSpinBox *doubleSpinBox_TB;
    QLabel *lbl_14;
    QLineEdit *lineEdit_ratio;
    QGroupBox *grpRV;
    QGridLayout *grdRV;
    QCheckBox *checkBox_2;
    QLineEdit *lineEdit_2;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QCheckBox *checkBox_13;
    QCheckBox *checkBox_16;
    QLineEdit *lineEdit_18;
    QPushButton *pushButton_15;
    QLabel *lbl_orbit;
    QLineEdit *lineEdit_orbit;
    QLabel *lbl_bc;
    QLineEdit *lineEdit_bc;
    QComboBox *comboBox_bcframe;
    QLabel *lbl_tellw;
    QLineEdit *lineEdit_tellw;
    QCheckBox *checkBox_tellfit;
    QCheckBox *checkBox_tellrange;
    QLineEdit *lineEdit_tellrange;
    QWidget *row_telldetect;
    QHBoxLayout *row_telldetect_l;
    QPushButton *pushButton_telldetect;
    QDoubleSpinBox *doubleSpinBox_telldet;
    QGroupBox *grpNoise;
    QGridLayout *grdNoise;
    QLabel *lbl_15;
    QDoubleSpinBox *doubleSpinBox_16;
    QLabel *lbl_16;
    QCheckBox *checkBox_41;
    QLineEdit *lineEdit_29;
    QCheckBox *checkBox_align;
    QDoubleSpinBox *doubleSpinBox_alignWidth;
    QGroupBox *grpRead;
    QGridLayout *grdRead;
    QLabel *lbl_17;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *lbl_18;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *lbl_19;
    QDoubleSpinBox *doubleSpinBox_3;
    QSpacerItem *sp_20;
    QWidget *tabModel;
    QVBoxLayout *tabModel_l;
    QScrollArea *tabModel_s;
    QWidget *tabModel_c;
    QVBoxLayout *tabModel_v;
    QGroupBox *grpShift;
    QGridLayout *grdShift;
    QLabel *lbl_21;
    QComboBox *comboBox_interp;
    QSpacerItem *sp_22;
    QLabel *lbl_23;
    QGroupBox *grpSolver;
    QGridLayout *grdSolver;
    QLabel *lbl_24;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QSpacerItem *sp_25;
    QLabel *lbl_26;
    QSpinBox *spinBox_8;
    QLabel *lbl_27;
    QSpinBox *spinBox_7;
    QLabel *lbl_tdamp;
    QDoubleSpinBox *doubleSpinBox_tdamp;
    QGroupBox *grpResiduum;
    QGridLayout *grdResiduum;
    QLabel *lbl_28;
    QCheckBox *checkBox_14;
    QCheckBox *checkBox_15;
    QSpacerItem *sp_29;
    QCheckBox *checkBox_40;
    QCheckBox *checkBox_33;
    QWidget *rowRange;
    QHBoxLayout *rowRange_l;
    QDoubleSpinBox *doubleSpinBox_14;
    QLabel *lbl_30;
    QDoubleSpinBox *doubleSpinBox_15;
    QLabel *lbl_31;
    QSpacerItem *sp_32;
    QCheckBox *checkBox_mask;
    QLineEdit *lineEdit_mask;
    QCheckBox *checkBox_resstatic;
    QSpacerItem *sp_33;
    QWidget *tabOpt;
    QVBoxLayout *tabOpt_l;
    QScrollArea *tabOpt_s;
    QWidget *tabOpt_c;
    QVBoxLayout *tabOpt_v;
    QGroupBox *grpTarget;
    QVBoxLayout *vTarget;
    QLabel *labelFitTarget;
    QLabel *labelFreeElements;
    QHBoxLayout *hFree;
    QCheckBox *checkBox_18;
    QCheckBox *checkBox_17;
    QWidget *row_checkBox_31;
    QHBoxLayout *row_checkBox_31_l;
    QCheckBox *checkBox_31;
    QLabel *lbl_34;
    QSpacerItem *sp_35;
    QWidget *row_checkBox_36;
    QHBoxLayout *row_checkBox_36_l;
    QCheckBox *checkBox_36;
    QLabel *lbl_36;
    QSpacerItem *sp_37;
    QCheckBox *checkBox_37;
    QWidget *row_checkBox_35;
    QHBoxLayout *row_checkBox_35_l;
    QCheckBox *checkBox_35;
    QLabel *lbl_38;
    QSpacerItem *sp_39;
    QCheckBox *checkBox_38;
    QSpacerItem *sp_42;
    QHBoxLayout *hFixedRV;
    QLabel *labelFixedRV;
    QLineEdit *lineEdit_fixA;
    QLabel *labelFixedRVB;
    QLineEdit *lineEdit_fixB;
    QGroupBox *grpMethod;
    QGridLayout *grdMethod;
    QLabel *lbl_45;
    QComboBox *comboBox_method;
    QLabel *lbl_46;
    QCheckBox *checkBox_9;
    QCheckBox *checkBox_11;
    QCheckBox *checkBox_12;
    QLabel *lbl_47;
    QSpinBox *spinBox_3;
    QGroupBox *grpStop;
    QGridLayout *grdStop;
    QCheckBox *checkBox_23;
    QDoubleSpinBox *doubleSpinBox_17;
    QLabel *lbl_48;
    QSpinBox *spinBox_11;
    QLabel *lbl_49;
    QCheckBox *checkBox_32;
    QGroupBox *grpErr;
    QGridLayout *grdErr;
    QLabel *lbl_52;
    QComboBox *comboBox_err;
    QLabel *lbl_53;
    QSpinBox *spinBox_mc;
    QSpacerItem *sp_54;
    QGroupBox *grpFlux;
    QGridLayout *grdFlux;
    QPushButton *pushButton_13;
    QLabel *lbl_50;
    QCheckBox *checkBox_29;
    QCheckBox *checkBox_30;
    QSpacerItem *sp_51;
    QGroupBox *grpLineScan;
    QGridLayout *grdLineScan;
    QLabel *lbl_ls1;
    QLineEdit *lineEdit_lsGrid;
    QLabel *lbl_ls2;
    QLineEdit *lineEdit_lsLines;
    QLabel *lbl_ls3;
    QLineEdit *lineEdit_lsOut;
    QPushButton *pushButton_lsFind;
    QPushButton *pushButton_lsScan;
    QCheckBox *checkBox_lsSave;
    QPushButton *pushButton_lsShow;
    QProgressBar *progressBar_lsScan;
    QSpacerItem *sp_55;
    QWidget *tabOut;
    QVBoxLayout *tabOut_l;
    QScrollArea *tabOut_s;
    QWidget *tabOut_c;
    QVBoxLayout *tabOut_v;
    QGroupBox *grpNames;
    QGridLayout *grdNames;
    QLabel *lbl_56;
    QLineEdit *lineEdit_25;
    QLineEdit *lineEdit_26;
    QPushButton *pushButton_11;
    QLabel *lbl_57;
    QGroupBox *grpResults;
    QGridLayout *grdResults;
    QLabel *lbl_58;
    QLineEdit *lineEdit_5;
    QLabel *lbl_59;
    QLineEdit *lineEdit_8;
    QLabel *lbl_60;
    QLineEdit *lineEdit_10;
    QLabel *lbl_61;
    QLineEdit *lineEdit_13;
    QLabel *lbl_62;
    QLineEdit *lineEdit_20;
    QLabel *lbl_63;
    QLineEdit *lineEdit_24;
    QLabel *lbl_stat;
    QLineEdit *lineEdit_stat;
    QCheckBox *checkBox_19;
    QCheckBox *checkBox_norm;
    QLabel *lbl_normKnot;
    QDoubleSpinBox *doubleSpinBox_normKnot;
    QLabel *lbl_normLow;
    QDoubleSpinBox *doubleSpinBox_normLow;
    QGroupBox *grpState;
    QGridLayout *grdState;
    QLabel *lbl_64;
    QLineEdit *lineEdit_14;
    QLineEdit *lineEdit_15;
    QLabel *lbl_65;
    QLineEdit *lineEdit_16;
    QLineEdit *lineEdit_17;
    QLabel *lbl_66;
    QLineEdit *lineEdit_21;
    QLabel *lbl_best;
    QLineEdit *lineEdit_best;
    QLabel *lbl_optdata;
    QLineEdit *lineEdit_optdata;
    QGroupBox *grpAnalysis;
    QGridLayout *grdAnalysis;
    QCheckBox *checkBox_anaFit;
    QLineEdit *lineEdit_anaFit;
    QCheckBox *checkBox_anaLines;
    QLineEdit *lineEdit_lines;
    QLineEdit *lineEdit_anaLines;
    QSpacerItem *sp_67;
    QTabWidget *tabPlots;
    QWidget *tabSpectra;
    QVBoxLayout *tabSpectra_l;
    QLabel *labelLegend;
    QCustomPlot *customPlot;
    QCustomPlot *customPlot_2;
    QHBoxLayout *hSpecOpts;
    QCheckBox *checkBox_10;
    QCheckBox *checkBox_22;
    QLabel *lbl_68;
    QDoubleSpinBox *doubleSpinBox_5;
    QLabel *lbl_69;
    QSpinBox *spinBox_6;
    QSpacerItem *sp_70;
    QPushButton *pushButton_6;
    QHBoxLayout *hStatPlots;
    QCustomPlot *customPlot_5;
    QCustomPlot *customPlot_6;
    QHBoxLayout *hStatOpts;
    QLabel *lbl_72;
    QDoubleSpinBox *doubleSpinBox_st1;
    QLabel *lbl_73;
    QDoubleSpinBox *doubleSpinBox_st2;
    QLabel *lbl_74;
    QSpacerItem *sp_75;
    QPushButton *pushButton_stat;
    QWidget *tabConv;
    QVBoxLayout *tabConv_l;
    QCustomPlot *customPlot_4;
    QHBoxLayout *hTextOut;
    QGroupBox *groupBox_messages;
    QVBoxLayout *vMessages;
    QPlainTextEdit *plainTextEdit_2;
    QGroupBox *groupBox_rvs;
    QVBoxLayout *vRVs;
    QPlainTextEdit *plainTextEdit;
    QFrame *frameStatus;
    QHBoxLayout *hStatus;
    QLabel *lbl_81;
    QSpinBox *spinBox_4;
    QLabel *lbl_82;
    QSpinBox *spinBox_5;
    QLabel *lbl_83;
    QLineEdit *lineEdit_7;
    QLabel *lbl_84;
    QLineEdit *lineEdit_6;
    QSpacerItem *sp_85;
    QLabel *labelRunInfo;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuHelp;
    QStatusBar *statusBar;
    QDockWidget *dockSubtract;
    QWidget *dockSubtractContents;
    QGridLayout *grdSubtract;
    QPushButton *pushButton_12;
    QCheckBox *checkBox_20;
    QCheckBox *checkBox_21;
    QLabel *lbl_87;
    QLineEdit *lineEdit_27;
    QCheckBox *checkBox_24;
    QCheckBox *checkBox_25;
    QCheckBox *checkBox_26;
    QCheckBox *checkBox_28;
    QLineEdit *lineEdit_28;
    QLabel *lbl_88;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1440, 920);
        actionSpectrum_Plotter = new QAction(MainWindow);
        actionSpectrum_Plotter->setObjectName(QString::fromUtf8("actionSpectrum_Plotter"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/Icons/SPlotter.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSpectrum_Plotter->setIcon(icon);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionBug_Report = new QAction(MainWindow);
        actionBug_Report->setObjectName(QString::fromUtf8("actionBug_Report"));
        actionCorrelation = new QAction(MainWindow);
        actionCorrelation->setObjectName(QString::fromUtf8("actionCorrelation"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/Icons/Corr.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCorrelation->setIcon(icon1);
        actionEditor = new QAction(MainWindow);
        actionEditor->setObjectName(QString::fromUtf8("actionEditor"));
        actionArithmetic = new QAction(MainWindow);
        actionArithmetic->setObjectName(QString::fromUtf8("actionArithmetic"));
        actionOpen_Configuration = new QAction(MainWindow);
        actionOpen_Configuration->setObjectName(QString::fromUtf8("actionOpen_Configuration"));
        actionSave_Configuration = new QAction(MainWindow);
        actionSave_Configuration->setObjectName(QString::fromUtf8("actionSave_Configuration"));
        actionSave_Configuration_As = new QAction(MainWindow);
        actionSave_Configuration_As->setObjectName(QString::fromUtf8("actionSave_Configuration_As"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionManual = new QAction(MainWindow);
        actionManual->setObjectName(QString::fromUtf8("actionManual"));
        actionSubtract = new QAction(MainWindow);
        actionSubtract->setObjectName(QString::fromUtf8("actionSubtract"));
        actionOrbitFit = new QAction(MainWindow);
        actionOrbitFit->setObjectName(QString::fromUtf8("actionOrbitFit"));
        actionFluxTemplate = new QAction(MainWindow);
        actionFluxTemplate->setObjectName(QString::fromUtf8("actionFluxTemplate"));
        actionBaryCorr = new QAction(MainWindow);
        actionBaryCorr->setObjectName(QString::fromUtf8("actionBaryCorr"));
        actionPeriod = new QAction(MainWindow);
        actionPeriod->setObjectName(QString::fromUtf8("actionPeriod"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        vCentral = new QVBoxLayout(centralWidget);
        vCentral->setSpacing(6);
        vCentral->setObjectName(QString::fromUtf8("vCentral"));
        vCentral->setContentsMargins(8, 8, 8, 8);
        hWorkflow = new QHBoxLayout();
        hWorkflow->setSpacing(6);
        hWorkflow->setObjectName(QString::fromUtf8("hWorkflow"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 32));

        hWorkflow->addWidget(pushButton_2);

        lbl_76 = new QLabel(centralWidget);
        lbl_76->setObjectName(QString::fromUtf8("lbl_76"));

        hWorkflow->addWidget(lbl_76);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(0, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/Icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon2);

        hWorkflow->addWidget(pushButton_3);

        lbl_77 = new QLabel(centralWidget);
        lbl_77->setObjectName(QString::fromUtf8("lbl_77"));

        hWorkflow->addWidget(lbl_77);

        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(0, 32));
        pushButton_4->setIcon(icon2);

        hWorkflow->addWidget(pushButton_4);

        lbl_78 = new QLabel(centralWidget);
        lbl_78->setObjectName(QString::fromUtf8("lbl_78"));

        hWorkflow->addWidget(lbl_78);

        pushButton_8 = new QPushButton(centralWidget);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setMinimumSize(QSize(0, 32));
        pushButton_8->setIcon(icon2);

        hWorkflow->addWidget(pushButton_8);

        pushButton_7 = new QPushButton(centralWidget);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setMinimumSize(QSize(0, 32));

        hWorkflow->addWidget(pushButton_7);

        sp_79 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hWorkflow->addItem(sp_79);

        lbl_80 = new QLabel(centralWidget);
        lbl_80->setObjectName(QString::fromUtf8("lbl_80"));

        hWorkflow->addWidget(lbl_80);

        lineEdit_4 = new QLineEdit(centralWidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setMinimumSize(QSize(380, 0));

        hWorkflow->addWidget(lineEdit_4);


        vCentral->addLayout(hWorkflow);

        hBody = new QHBoxLayout();
        hBody->setSpacing(8);
        hBody->setObjectName(QString::fromUtf8("hBody"));
        tabSettings = new QTabWidget(centralWidget);
        tabSettings->setObjectName(QString::fromUtf8("tabSettings"));
        tabSettings->setMaximumSize(QSize(640, 16777215));
        tabSettings->setMinimumSize(QSize(520, 0));
        tabData = new QWidget();
        tabData->setObjectName(QString::fromUtf8("tabData"));
        tabData_l = new QVBoxLayout(tabData);
        tabData_l->setObjectName(QString::fromUtf8("tabData_l"));
        tabData_l->setContentsMargins(0, 0, 0, 0);
        tabData_s = new QScrollArea(tabData);
        tabData_s->setObjectName(QString::fromUtf8("tabData_s"));
        tabData_s->setWidgetResizable(true);
        tabData_s->setFrameShape(QFrame::NoFrame);
        tabData_c = new QWidget();
        tabData_c->setObjectName(QString::fromUtf8("tabData_c"));
        tabData_v = new QVBoxLayout(tabData_c);
        tabData_v->setSpacing(8);
        tabData_v->setObjectName(QString::fromUtf8("tabData_v"));
        tabData_v->setContentsMargins(8, 8, 8, 8);
        grpSpectra = new QGroupBox(tabData_c);
        grpSpectra->setObjectName(QString::fromUtf8("grpSpectra"));
        grdSpectra = new QGridLayout(grpSpectra);
        grdSpectra->setObjectName(QString::fromUtf8("grdSpectra"));
        lbl_1 = new QLabel(grpSpectra);
        lbl_1->setObjectName(QString::fromUtf8("lbl_1"));

        grdSpectra->addWidget(lbl_1, 0, 0, 1, 1);

        lineEdit_3 = new QLineEdit(grpSpectra);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        grdSpectra->addWidget(lineEdit_3, 0, 1, 1, 1);

        lbl_2 = new QLabel(grpSpectra);
        lbl_2->setObjectName(QString::fromUtf8("lbl_2"));

        grdSpectra->addWidget(lbl_2, 0, 2, 1, 1);

        spinBox = new QSpinBox(grpSpectra);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimumSize(QSize(56, 0));
        spinBox->setMaximumSize(QSize(56, 16777215));

        grdSpectra->addWidget(spinBox, 0, 3, 1, 1);

        lbl_3 = new QLabel(grpSpectra);
        lbl_3->setObjectName(QString::fromUtf8("lbl_3"));

        grdSpectra->addWidget(lbl_3, 0, 4, 1, 1);

        spinBox_2 = new QSpinBox(grpSpectra);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMaximum(999);
        spinBox_2->setValue(1);
        spinBox_2->setMinimumSize(QSize(56, 0));
        spinBox_2->setMaximumSize(QSize(56, 16777215));

        grdSpectra->addWidget(spinBox_2, 0, 5, 1, 1);

        lineEdit_19 = new QLineEdit(grpSpectra);
        lineEdit_19->setObjectName(QString::fromUtf8("lineEdit_19"));
        lineEdit_19->setMinimumSize(QSize(52, 0));
        lineEdit_19->setMaximumSize(QSize(52, 16777215));

        grdSpectra->addWidget(lineEdit_19, 0, 6, 1, 1);

        lbl_4 = new QLabel(grpSpectra);
        lbl_4->setObjectName(QString::fromUtf8("lbl_4"));

        grdSpectra->addWidget(lbl_4, 1, 0, 1, 1);

        comboBox = new QComboBox(grpSpectra);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setMinimumSize(QSize(80, 0));
        comboBox->setMaximumSize(QSize(80, 16777215));

        grdSpectra->addWidget(comboBox, 1, 1, 1, 1);

        lbl_5 = new QLabel(grpSpectra);
        lbl_5->setObjectName(QString::fromUtf8("lbl_5"));

        grdSpectra->addWidget(lbl_5, 2, 0, 1, 1);

        lineEdit_9 = new QLineEdit(grpSpectra);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));

        grdSpectra->addWidget(lineEdit_9, 2, 1, 1, 1);

        lbl_6 = new QLabel(grpSpectra);
        lbl_6->setObjectName(QString::fromUtf8("lbl_6"));

        grdSpectra->addWidget(lbl_6, 2, 2, 1, 1);

        lineEdit_11 = new QLineEdit(grpSpectra);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));
        lineEdit_11->setMinimumSize(QSize(56, 0));
        lineEdit_11->setMaximumSize(QSize(56, 16777215));

        grdSpectra->addWidget(lineEdit_11, 2, 3, 1, 1);

        lbl_7 = new QLabel(grpSpectra);
        lbl_7->setObjectName(QString::fromUtf8("lbl_7"));

        grdSpectra->addWidget(lbl_7, 2, 4, 1, 1);

        lineEdit_12 = new QLineEdit(grpSpectra);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));
        lineEdit_12->setMinimumSize(QSize(56, 0));
        lineEdit_12->setMaximumSize(QSize(56, 16777215));

        grdSpectra->addWidget(lineEdit_12, 2, 5, 1, 2);

        checkBox_42 = new QCheckBox(grpSpectra);
        checkBox_42->setObjectName(QString::fromUtf8("checkBox_42"));

        grdSpectra->addWidget(checkBox_42, 3, 0, 1, 4);

        doubleSpinBox_18 = new QDoubleSpinBox(grpSpectra);
        doubleSpinBox_18->setObjectName(QString::fromUtf8("doubleSpinBox_18"));
        doubleSpinBox_18->setDecimals(3);
        doubleSpinBox_18->setMaximum(1000.000000000000000);
        doubleSpinBox_18->setSingleStep(0.100000000000000);
        doubleSpinBox_18->setMinimumSize(QSize(110, 0));
        doubleSpinBox_18->setMaximumSize(QSize(110, 16777215));

        grdSpectra->addWidget(doubleSpinBox_18, 3, 4, 1, 3);


        tabData_v->addWidget(grpSpectra);

        grpSystem = new QGroupBox(tabData_c);
        grpSystem->setObjectName(QString::fromUtf8("grpSystem"));
        grdSystem = new QGridLayout(grpSystem);
        grdSystem->setObjectName(QString::fromUtf8("grdSystem"));
        lbl_8 = new QLabel(grpSystem);
        lbl_8->setObjectName(QString::fromUtf8("lbl_8"));

        grdSystem->addWidget(lbl_8, 0, 0, 1, 1);

        checkBox_3 = new QCheckBox(grpSystem);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setEnabled(true);

        grdSystem->addWidget(checkBox_3, 0, 1, 1, 1);

        checkBox_4 = new QCheckBox(grpSystem);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setEnabled(true);

        grdSystem->addWidget(checkBox_4, 0, 2, 1, 1);

        checkBox_34 = new QCheckBox(grpSystem);
        checkBox_34->setObjectName(QString::fromUtf8("checkBox_34"));
        checkBox_34->setEnabled(false);

        grdSystem->addWidget(checkBox_34, 0, 3, 1, 1);

        checkBox_8 = new QCheckBox(grpSystem);
        checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));

        grdSystem->addWidget(checkBox_8, 0, 4, 1, 1);

        lbl_9 = new QLabel(grpSystem);
        lbl_9->setObjectName(QString::fromUtf8("lbl_9"));
        lbl_9->setTextFormat(Qt::RichText);

        grdSystem->addWidget(lbl_9, 1, 0, 1, 1);

        doubleSpinBox_4 = new QDoubleSpinBox(grpSystem);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
        doubleSpinBox_4->setDecimals(5);
        doubleSpinBox_4->setSingleStep(0.020000000000000);
        doubleSpinBox_4->setMinimumSize(QSize(90, 0));
        doubleSpinBox_4->setMaximumSize(QSize(90, 16777215));

        grdSystem->addWidget(doubleSpinBox_4, 1, 1, 1, 1);

        lbl_10 = new QLabel(grpSystem);
        lbl_10->setObjectName(QString::fromUtf8("lbl_10"));

        grdSystem->addWidget(lbl_10, 1, 2, 1, 3);

        lbl_11 = new QLabel(grpSystem);
        lbl_11->setObjectName(QString::fromUtf8("lbl_11"));

        grdSystem->addWidget(lbl_11, 2, 0, 1, 1);

        comboBox_ratio = new QComboBox(grpSystem);
        comboBox_ratio->addItem(QString());
        comboBox_ratio->addItem(QString());
        comboBox_ratio->addItem(QString());
        comboBox_ratio->setObjectName(QString::fromUtf8("comboBox_ratio"));
        comboBox_ratio->setMinimumSize(QSize(100, 0));
        comboBox_ratio->setMaximumSize(QSize(100, 16777215));

        grdSystem->addWidget(comboBox_ratio, 2, 1, 1, 1);

        lbl_12 = new QLabel(grpSystem);
        lbl_12->setObjectName(QString::fromUtf8("lbl_12"));
        lbl_12->setTextFormat(Qt::RichText);

        grdSystem->addWidget(lbl_12, 2, 2, 1, 1);

        doubleSpinBox_lref = new QDoubleSpinBox(grpSystem);
        doubleSpinBox_lref->setObjectName(QString::fromUtf8("doubleSpinBox_lref"));
        doubleSpinBox_lref->setDecimals(1);
        doubleSpinBox_lref->setMaximum(1000000.000000000000000);
        doubleSpinBox_lref->setSingleStep(100.000000000000000);
        doubleSpinBox_lref->setMinimumSize(QSize(110, 0));
        doubleSpinBox_lref->setMaximumSize(QSize(110, 16777215));

        grdSystem->addWidget(doubleSpinBox_lref, 2, 3, 1, 2);

        lbl_13 = new QLabel(grpSystem);
        lbl_13->setObjectName(QString::fromUtf8("lbl_13"));
        lbl_13->setTextFormat(Qt::RichText);

        grdSystem->addWidget(lbl_13, 3, 0, 1, 1);

        doubleSpinBox_TA = new QDoubleSpinBox(grpSystem);
        doubleSpinBox_TA->setObjectName(QString::fromUtf8("doubleSpinBox_TA"));
        doubleSpinBox_TA->setDecimals(0);
        doubleSpinBox_TA->setMinimum(0.000000000000000);
        doubleSpinBox_TA->setMaximum(1000000.000000000000000);
        doubleSpinBox_TA->setSingleStep(100.000000000000000);
        doubleSpinBox_TA->setValue(10000.000000000000000);
        doubleSpinBox_TA->setMinimumSize(QSize(100, 0));
        doubleSpinBox_TA->setMaximumSize(QSize(100, 16777215));

        grdSystem->addWidget(doubleSpinBox_TA, 3, 1, 1, 1);

        doubleSpinBox_TB = new QDoubleSpinBox(grpSystem);
        doubleSpinBox_TB->setObjectName(QString::fromUtf8("doubleSpinBox_TB"));
        doubleSpinBox_TB->setDecimals(0);
        doubleSpinBox_TB->setMinimum(0.000000000000000);
        doubleSpinBox_TB->setMaximum(1000000.000000000000000);
        doubleSpinBox_TB->setSingleStep(100.000000000000000);
        doubleSpinBox_TB->setValue(10000.000000000000000);
        doubleSpinBox_TB->setMinimumSize(QSize(100, 0));
        doubleSpinBox_TB->setMaximumSize(QSize(100, 16777215));

        grdSystem->addWidget(doubleSpinBox_TB, 3, 2, 1, 1);

        lbl_14 = new QLabel(grpSystem);
        lbl_14->setObjectName(QString::fromUtf8("lbl_14"));

        grdSystem->addWidget(lbl_14, 3, 3, 1, 1);

        lineEdit_ratio = new QLineEdit(grpSystem);
        lineEdit_ratio->setObjectName(QString::fromUtf8("lineEdit_ratio"));

        grdSystem->addWidget(lineEdit_ratio, 3, 4, 1, 1);


        tabData_v->addWidget(grpSystem);

        grpRV = new QGroupBox(tabData_c);
        grpRV->setObjectName(QString::fromUtf8("grpRV"));
        grdRV = new QGridLayout(grpRV);
        grdRV->setObjectName(QString::fromUtf8("grdRV"));
        checkBox_2 = new QCheckBox(grpRV);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        grdRV->addWidget(checkBox_2, 0, 0, 1, 1);

        lineEdit_2 = new QLineEdit(grpRV);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        grdRV->addWidget(lineEdit_2, 0, 1, 1, 3);

        checkBox = new QCheckBox(grpRV);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        grdRV->addWidget(checkBox, 1, 0, 1, 1);

        lineEdit = new QLineEdit(grpRV);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        grdRV->addWidget(lineEdit, 1, 1, 1, 2);

        checkBox_13 = new QCheckBox(grpRV);
        checkBox_13->setObjectName(QString::fromUtf8("checkBox_13"));

        grdRV->addWidget(checkBox_13, 1, 3, 1, 1);

        checkBox_16 = new QCheckBox(grpRV);
        checkBox_16->setObjectName(QString::fromUtf8("checkBox_16"));

        grdRV->addWidget(checkBox_16, 2, 0, 1, 1);

        lineEdit_18 = new QLineEdit(grpRV);
        lineEdit_18->setObjectName(QString::fromUtf8("lineEdit_18"));

        grdRV->addWidget(lineEdit_18, 2, 1, 1, 2);

        pushButton_15 = new QPushButton(grpRV);
        pushButton_15->setObjectName(QString::fromUtf8("pushButton_15"));

        grdRV->addWidget(pushButton_15, 2, 3, 1, 1);

        lbl_orbit = new QLabel(grpRV);
        lbl_orbit->setObjectName(QString::fromUtf8("lbl_orbit"));

        grdRV->addWidget(lbl_orbit, 3, 0, 1, 1);

        lineEdit_orbit = new QLineEdit(grpRV);
        lineEdit_orbit->setObjectName(QString::fromUtf8("lineEdit_orbit"));

        grdRV->addWidget(lineEdit_orbit, 3, 1, 1, 2);

        lbl_bc = new QLabel(grpRV);
        lbl_bc->setObjectName(QString::fromUtf8("lbl_bc"));

        grdRV->addWidget(lbl_bc, 4, 0, 1, 1);

        lineEdit_bc = new QLineEdit(grpRV);
        lineEdit_bc->setObjectName(QString::fromUtf8("lineEdit_bc"));

        grdRV->addWidget(lineEdit_bc, 4, 1, 1, 1);

        comboBox_bcframe = new QComboBox(grpRV);
        comboBox_bcframe->addItem(QString());
        comboBox_bcframe->addItem(QString());
        comboBox_bcframe->setObjectName(QString::fromUtf8("comboBox_bcframe"));

        grdRV->addWidget(comboBox_bcframe, 4, 2, 1, 2);

        lbl_tellw = new QLabel(grpRV);
        lbl_tellw->setObjectName(QString::fromUtf8("lbl_tellw"));

        grdRV->addWidget(lbl_tellw, 5, 0, 1, 1);

        lineEdit_tellw = new QLineEdit(grpRV);
        lineEdit_tellw->setObjectName(QString::fromUtf8("lineEdit_tellw"));

        grdRV->addWidget(lineEdit_tellw, 5, 1, 1, 1);

        checkBox_tellfit = new QCheckBox(grpRV);
        checkBox_tellfit->setObjectName(QString::fromUtf8("checkBox_tellfit"));

        grdRV->addWidget(checkBox_tellfit, 5, 2, 1, 2);

        checkBox_tellrange = new QCheckBox(grpRV);
        checkBox_tellrange->setObjectName(QString::fromUtf8("checkBox_tellrange"));

        grdRV->addWidget(checkBox_tellrange, 6, 0, 1, 1);

        lineEdit_tellrange = new QLineEdit(grpRV);
        lineEdit_tellrange->setObjectName(QString::fromUtf8("lineEdit_tellrange"));

        grdRV->addWidget(lineEdit_tellrange, 6, 1, 1, 1);

        row_telldetect = new QWidget(grpRV);
        row_telldetect->setObjectName(QString::fromUtf8("row_telldetect"));
        row_telldetect_l = new QHBoxLayout(row_telldetect);
        row_telldetect_l->setObjectName(QString::fromUtf8("row_telldetect_l"));
        row_telldetect_l->setContentsMargins(0, 0, 0, 0);
        pushButton_telldetect = new QPushButton(row_telldetect);
        pushButton_telldetect->setObjectName(QString::fromUtf8("pushButton_telldetect"));

        row_telldetect_l->addWidget(pushButton_telldetect);

        doubleSpinBox_telldet = new QDoubleSpinBox(row_telldetect);
        doubleSpinBox_telldet->setObjectName(QString::fromUtf8("doubleSpinBox_telldet"));
        doubleSpinBox_telldet->setDecimals(1);
        doubleSpinBox_telldet->setMinimum(1.000000000000000);
        doubleSpinBox_telldet->setMaximum(50.000000000000000);
        doubleSpinBox_telldet->setValue(5.000000000000000);

        row_telldetect_l->addWidget(doubleSpinBox_telldet);


        grdRV->addWidget(row_telldetect, 6, 2, 1, 2);


        tabData_v->addWidget(grpRV);

        grpNoise = new QGroupBox(tabData_c);
        grpNoise->setObjectName(QString::fromUtf8("grpNoise"));
        grdNoise = new QGridLayout(grpNoise);
        grdNoise->setObjectName(QString::fromUtf8("grdNoise"));
        lbl_15 = new QLabel(grpNoise);
        lbl_15->setObjectName(QString::fromUtf8("lbl_15"));

        grdNoise->addWidget(lbl_15, 0, 0, 1, 1);

        doubleSpinBox_16 = new QDoubleSpinBox(grpNoise);
        doubleSpinBox_16->setObjectName(QString::fromUtf8("doubleSpinBox_16"));
        doubleSpinBox_16->setDecimals(1);
        doubleSpinBox_16->setMinimum(0.000000000000000);
        doubleSpinBox_16->setMaximum(100000.000000000000000);
        doubleSpinBox_16->setSingleStep(10.000000000000000);
        doubleSpinBox_16->setValue(0.000000000000000);
        doubleSpinBox_16->setMinimumSize(QSize(90, 0));
        doubleSpinBox_16->setMaximumSize(QSize(90, 16777215));

        grdNoise->addWidget(doubleSpinBox_16, 0, 1, 1, 1);

        lbl_16 = new QLabel(grpNoise);
        lbl_16->setObjectName(QString::fromUtf8("lbl_16"));

        grdNoise->addWidget(lbl_16, 0, 2, 1, 2);

        checkBox_41 = new QCheckBox(grpNoise);
        checkBox_41->setObjectName(QString::fromUtf8("checkBox_41"));

        grdNoise->addWidget(checkBox_41, 1, 0, 1, 2);

        lineEdit_29 = new QLineEdit(grpNoise);
        lineEdit_29->setObjectName(QString::fromUtf8("lineEdit_29"));

        grdNoise->addWidget(lineEdit_29, 1, 2, 1, 2);

        checkBox_align = new QCheckBox(grpNoise);
        checkBox_align->setObjectName(QString::fromUtf8("checkBox_align"));

        grdNoise->addWidget(checkBox_align, 2, 0, 1, 2);

        doubleSpinBox_alignWidth = new QDoubleSpinBox(grpNoise);
        doubleSpinBox_alignWidth->setObjectName(QString::fromUtf8("doubleSpinBox_alignWidth"));
        doubleSpinBox_alignWidth->setDecimals(0);
        doubleSpinBox_alignWidth->setMinimum(5.000000000000000);
        doubleSpinBox_alignWidth->setMaximum(2000.000000000000000);
        doubleSpinBox_alignWidth->setSingleStep(5.000000000000000);
        doubleSpinBox_alignWidth->setValue(30.000000000000000);

        grdNoise->addWidget(doubleSpinBox_alignWidth, 2, 2, 1, 2);


        tabData_v->addWidget(grpNoise);

        grpRead = new QGroupBox(tabData_c);
        grpRead->setObjectName(QString::fromUtf8("grpRead"));
        grdRead = new QGridLayout(grpRead);
        grdRead->setObjectName(QString::fromUtf8("grdRead"));
        lbl_17 = new QLabel(grpRead);
        lbl_17->setObjectName(QString::fromUtf8("lbl_17"));

        grdRead->addWidget(lbl_17, 0, 0, 1, 1);

        doubleSpinBox = new QDoubleSpinBox(grpRead);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox->setReadOnly(true);
        doubleSpinBox->setEnabled(false);
        doubleSpinBox->setMinimum(-999.000000000000000);
        doubleSpinBox->setMaximum(999.000000000000000);
        doubleSpinBox->setMinimumSize(QSize(90, 0));
        doubleSpinBox->setMaximumSize(QSize(90, 16777215));

        grdRead->addWidget(doubleSpinBox, 0, 1, 1, 1);

        lbl_18 = new QLabel(grpRead);
        lbl_18->setObjectName(QString::fromUtf8("lbl_18"));

        grdRead->addWidget(lbl_18, 0, 2, 1, 1);

        doubleSpinBox_2 = new QDoubleSpinBox(grpRead);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_2->setReadOnly(true);
        doubleSpinBox_2->setEnabled(false);
        doubleSpinBox_2->setMinimum(-999.000000000000000);
        doubleSpinBox_2->setMaximum(999.990000000000009);
        doubleSpinBox_2->setMinimumSize(QSize(90, 0));
        doubleSpinBox_2->setMaximumSize(QSize(90, 16777215));

        grdRead->addWidget(doubleSpinBox_2, 0, 3, 1, 1);

        lbl_19 = new QLabel(grpRead);
        lbl_19->setObjectName(QString::fromUtf8("lbl_19"));

        grdRead->addWidget(lbl_19, 1, 0, 1, 1);

        doubleSpinBox_3 = new QDoubleSpinBox(grpRead);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        doubleSpinBox_3->setReadOnly(true);
        doubleSpinBox_3->setEnabled(false);
        doubleSpinBox_3->setFrame(false);
        doubleSpinBox_3->setDecimals(5);
        doubleSpinBox_3->setSingleStep(0.050000000000000);
        doubleSpinBox_3->setMinimumSize(QSize(90, 0));
        doubleSpinBox_3->setMaximumSize(QSize(90, 16777215));

        grdRead->addWidget(doubleSpinBox_3, 1, 1, 1, 1);


        tabData_v->addWidget(grpRead);

        sp_20 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        tabData_v->addItem(sp_20);

        tabData_s->setWidget(tabData_c);

        tabData_l->addWidget(tabData_s);

        tabSettings->addTab(tabData, QString());
        tabModel = new QWidget();
        tabModel->setObjectName(QString::fromUtf8("tabModel"));
        tabModel_l = new QVBoxLayout(tabModel);
        tabModel_l->setObjectName(QString::fromUtf8("tabModel_l"));
        tabModel_l->setContentsMargins(0, 0, 0, 0);
        tabModel_s = new QScrollArea(tabModel);
        tabModel_s->setObjectName(QString::fromUtf8("tabModel_s"));
        tabModel_s->setWidgetResizable(true);
        tabModel_s->setFrameShape(QFrame::NoFrame);
        tabModel_c = new QWidget();
        tabModel_c->setObjectName(QString::fromUtf8("tabModel_c"));
        tabModel_v = new QVBoxLayout(tabModel_c);
        tabModel_v->setSpacing(8);
        tabModel_v->setObjectName(QString::fromUtf8("tabModel_v"));
        tabModel_v->setContentsMargins(8, 8, 8, 8);
        grpShift = new QGroupBox(tabModel_c);
        grpShift->setObjectName(QString::fromUtf8("grpShift"));
        grdShift = new QGridLayout(grpShift);
        grdShift->setObjectName(QString::fromUtf8("grdShift"));
        lbl_21 = new QLabel(grpShift);
        lbl_21->setObjectName(QString::fromUtf8("lbl_21"));

        grdShift->addWidget(lbl_21, 0, 0, 1, 1);

        comboBox_interp = new QComboBox(grpShift);
        comboBox_interp->addItem(QString());
        comboBox_interp->addItem(QString());
        comboBox_interp->setObjectName(QString::fromUtf8("comboBox_interp"));
        comboBox_interp->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        comboBox_interp->setMinimumSize(QSize(100, 0));
        comboBox_interp->setMaximumSize(QSize(100, 16777215));

        grdShift->addWidget(comboBox_interp, 0, 1, 1, 1);

        sp_22 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        grdShift->addItem(sp_22, 0, 2, 1, 1);

        lbl_23 = new QLabel(grpShift);
        lbl_23->setObjectName(QString::fromUtf8("lbl_23"));

        grdShift->addWidget(lbl_23, 1, 0, 1, 3);


        tabModel_v->addWidget(grpShift);

        grpSolver = new QGroupBox(tabModel_c);
        grpSolver->setObjectName(QString::fromUtf8("grpSolver"));
        grdSolver = new QGridLayout(grpSolver);
        grdSolver->setObjectName(QString::fromUtf8("grdSolver"));
        lbl_24 = new QLabel(grpSolver);
        lbl_24->setObjectName(QString::fromUtf8("lbl_24"));

        grdSolver->addWidget(lbl_24, 0, 0, 1, 1);

        checkBox_5 = new QCheckBox(grpSolver);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));

        grdSolver->addWidget(checkBox_5, 0, 1, 1, 1);

        checkBox_6 = new QCheckBox(grpSolver);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

        grdSolver->addWidget(checkBox_6, 0, 2, 1, 1);

        checkBox_7 = new QCheckBox(grpSolver);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));

        grdSolver->addWidget(checkBox_7, 0, 3, 1, 1);

        sp_25 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        grdSolver->addItem(sp_25, 0, 4, 1, 1);

        lbl_26 = new QLabel(grpSolver);
        lbl_26->setObjectName(QString::fromUtf8("lbl_26"));

        grdSolver->addWidget(lbl_26, 1, 0, 1, 1);

        spinBox_8 = new QSpinBox(grpSolver);
        spinBox_8->setObjectName(QString::fromUtf8("spinBox_8"));
        spinBox_8->setMinimum(1);
        spinBox_8->setMaximum(9999);
        spinBox_8->setMinimumSize(QSize(60, 0));
        spinBox_8->setMaximumSize(QSize(60, 16777215));

        grdSolver->addWidget(spinBox_8, 1, 1, 1, 1);

        lbl_27 = new QLabel(grpSolver);
        lbl_27->setObjectName(QString::fromUtf8("lbl_27"));

        grdSolver->addWidget(lbl_27, 1, 2, 1, 1);

        spinBox_7 = new QSpinBox(grpSolver);
        spinBox_7->setObjectName(QString::fromUtf8("spinBox_7"));
        spinBox_7->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_7->setReadOnly(true);
        spinBox_7->setEnabled(false);
        spinBox_7->setMinimum(1);
        spinBox_7->setMaximum(9999);
        spinBox_7->setMinimumSize(QSize(50, 0));
        spinBox_7->setMaximumSize(QSize(50, 16777215));

        grdSolver->addWidget(spinBox_7, 1, 3, 1, 1);

        lbl_tdamp = new QLabel(grpSolver);
        lbl_tdamp->setObjectName(QString::fromUtf8("lbl_tdamp"));

        grdSolver->addWidget(lbl_tdamp, 2, 0, 1, 1);

        doubleSpinBox_tdamp = new QDoubleSpinBox(grpSolver);
        doubleSpinBox_tdamp->setObjectName(QString::fromUtf8("doubleSpinBox_tdamp"));
        doubleSpinBox_tdamp->setDecimals(6);
        doubleSpinBox_tdamp->setMinimum(0.000000000000000);
        doubleSpinBox_tdamp->setMaximum(1.000000000000000);
        doubleSpinBox_tdamp->setSingleStep(0.001000000000000);
        doubleSpinBox_tdamp->setValue(0.001000000000000);

        grdSolver->addWidget(doubleSpinBox_tdamp, 2, 1, 1, 1);


        tabModel_v->addWidget(grpSolver);

        grpResiduum = new QGroupBox(tabModel_c);
        grpResiduum->setObjectName(QString::fromUtf8("grpResiduum"));
        grdResiduum = new QGridLayout(grpResiduum);
        grdResiduum->setObjectName(QString::fromUtf8("grdResiduum"));
        lbl_28 = new QLabel(grpResiduum);
        lbl_28->setObjectName(QString::fromUtf8("lbl_28"));

        grdResiduum->addWidget(lbl_28, 0, 0, 1, 1);

        checkBox_14 = new QCheckBox(grpResiduum);
        checkBox_14->setObjectName(QString::fromUtf8("checkBox_14"));

        grdResiduum->addWidget(checkBox_14, 0, 1, 1, 1);

        checkBox_15 = new QCheckBox(grpResiduum);
        checkBox_15->setObjectName(QString::fromUtf8("checkBox_15"));

        grdResiduum->addWidget(checkBox_15, 0, 2, 1, 1);

        sp_29 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        grdResiduum->addItem(sp_29, 0, 3, 1, 1);

        checkBox_40 = new QCheckBox(grpResiduum);
        checkBox_40->setObjectName(QString::fromUtf8("checkBox_40"));

        grdResiduum->addWidget(checkBox_40, 1, 0, 1, 4);

        checkBox_33 = new QCheckBox(grpResiduum);
        checkBox_33->setObjectName(QString::fromUtf8("checkBox_33"));

        grdResiduum->addWidget(checkBox_33, 2, 0, 1, 1);

        rowRange = new QWidget(grpResiduum);
        rowRange->setObjectName(QString::fromUtf8("rowRange"));
        rowRange_l = new QHBoxLayout(rowRange);
        rowRange_l->setObjectName(QString::fromUtf8("rowRange_l"));
        rowRange_l->setContentsMargins(0, 0, 0, 0);
        doubleSpinBox_14 = new QDoubleSpinBox(rowRange);
        doubleSpinBox_14->setObjectName(QString::fromUtf8("doubleSpinBox_14"));
        doubleSpinBox_14->setDecimals(4);
        doubleSpinBox_14->setMaximum(9999.989999999999782);
        doubleSpinBox_14->setMinimumSize(QSize(100, 0));
        doubleSpinBox_14->setMaximumSize(QSize(100, 16777215));

        rowRange_l->addWidget(doubleSpinBox_14);

        lbl_30 = new QLabel(rowRange);
        lbl_30->setObjectName(QString::fromUtf8("lbl_30"));

        rowRange_l->addWidget(lbl_30);

        doubleSpinBox_15 = new QDoubleSpinBox(rowRange);
        doubleSpinBox_15->setObjectName(QString::fromUtf8("doubleSpinBox_15"));
        doubleSpinBox_15->setDecimals(4);
        doubleSpinBox_15->setMaximum(9999.989999999999782);
        doubleSpinBox_15->setMinimumSize(QSize(100, 0));
        doubleSpinBox_15->setMaximumSize(QSize(100, 16777215));

        rowRange_l->addWidget(doubleSpinBox_15);

        lbl_31 = new QLabel(rowRange);
        lbl_31->setObjectName(QString::fromUtf8("lbl_31"));

        rowRange_l->addWidget(lbl_31);

        sp_32 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        rowRange_l->addItem(sp_32);


        grdResiduum->addWidget(rowRange, 2, 1, 1, 3);

        checkBox_mask = new QCheckBox(grpResiduum);
        checkBox_mask->setObjectName(QString::fromUtf8("checkBox_mask"));

        grdResiduum->addWidget(checkBox_mask, 3, 0, 1, 1);

        lineEdit_mask = new QLineEdit(grpResiduum);
        lineEdit_mask->setObjectName(QString::fromUtf8("lineEdit_mask"));

        grdResiduum->addWidget(lineEdit_mask, 3, 1, 1, 3);

        checkBox_resstatic = new QCheckBox(grpResiduum);
        checkBox_resstatic->setObjectName(QString::fromUtf8("checkBox_resstatic"));

        grdResiduum->addWidget(checkBox_resstatic, 4, 0, 1, 4);


        tabModel_v->addWidget(grpResiduum);

        sp_33 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        tabModel_v->addItem(sp_33);

        tabModel_s->setWidget(tabModel_c);

        tabModel_l->addWidget(tabModel_s);

        tabSettings->addTab(tabModel, QString());
        tabOpt = new QWidget();
        tabOpt->setObjectName(QString::fromUtf8("tabOpt"));
        tabOpt_l = new QVBoxLayout(tabOpt);
        tabOpt_l->setObjectName(QString::fromUtf8("tabOpt_l"));
        tabOpt_l->setContentsMargins(0, 0, 0, 0);
        tabOpt_s = new QScrollArea(tabOpt);
        tabOpt_s->setObjectName(QString::fromUtf8("tabOpt_s"));
        tabOpt_s->setWidgetResizable(true);
        tabOpt_s->setFrameShape(QFrame::NoFrame);
        tabOpt_c = new QWidget();
        tabOpt_c->setObjectName(QString::fromUtf8("tabOpt_c"));
        tabOpt_v = new QVBoxLayout(tabOpt_c);
        tabOpt_v->setSpacing(8);
        tabOpt_v->setObjectName(QString::fromUtf8("tabOpt_v"));
        tabOpt_v->setContentsMargins(8, 8, 8, 8);
        grpTarget = new QGroupBox(tabOpt_c);
        grpTarget->setObjectName(QString::fromUtf8("grpTarget"));
        vTarget = new QVBoxLayout(grpTarget);
        vTarget->setObjectName(QString::fromUtf8("vTarget"));
        labelFitTarget = new QLabel(grpTarget);
        labelFitTarget->setObjectName(QString::fromUtf8("labelFitTarget"));

        vTarget->addWidget(labelFitTarget);

        labelFreeElements = new QLabel(grpTarget);
        labelFreeElements->setObjectName(QString::fromUtf8("labelFreeElements"));

        vTarget->addWidget(labelFreeElements);

        hFree = new QHBoxLayout();
        hFree->setSpacing(14);
        hFree->setObjectName(QString::fromUtf8("hFree"));
        checkBox_18 = new QCheckBox(grpTarget);
        checkBox_18->setObjectName(QString::fromUtf8("checkBox_18"));
        checkBox_18->setChecked(true);
        checkBox_18->setEnabled(true);

        hFree->addWidget(checkBox_18);

        checkBox_17 = new QCheckBox(grpTarget);
        checkBox_17->setObjectName(QString::fromUtf8("checkBox_17"));
        checkBox_17->setChecked(true);
        checkBox_17->setEnabled(true);

        hFree->addWidget(checkBox_17);

        row_checkBox_31 = new QWidget(grpTarget);
        row_checkBox_31->setObjectName(QString::fromUtf8("row_checkBox_31"));
        row_checkBox_31_l = new QHBoxLayout(row_checkBox_31);
        row_checkBox_31_l->setObjectName(QString::fromUtf8("row_checkBox_31_l"));
        row_checkBox_31_l->setContentsMargins(0, 0, 0, 0);
        checkBox_31 = new QCheckBox(row_checkBox_31);
        checkBox_31->setObjectName(QString::fromUtf8("checkBox_31"));
        checkBox_31->setChecked(true);

        row_checkBox_31_l->addWidget(checkBox_31);

        lbl_34 = new QLabel(row_checkBox_31);
        lbl_34->setObjectName(QString::fromUtf8("lbl_34"));
        lbl_34->setTextFormat(Qt::RichText);

        row_checkBox_31_l->addWidget(lbl_34);

        sp_35 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        row_checkBox_31_l->addItem(sp_35);


        hFree->addWidget(row_checkBox_31);

        row_checkBox_36 = new QWidget(grpTarget);
        row_checkBox_36->setObjectName(QString::fromUtf8("row_checkBox_36"));
        row_checkBox_36_l = new QHBoxLayout(row_checkBox_36);
        row_checkBox_36_l->setObjectName(QString::fromUtf8("row_checkBox_36_l"));
        row_checkBox_36_l->setContentsMargins(0, 0, 0, 0);
        checkBox_36 = new QCheckBox(row_checkBox_36);
        checkBox_36->setObjectName(QString::fromUtf8("checkBox_36"));
        checkBox_36->setChecked(true);

        row_checkBox_36_l->addWidget(checkBox_36);

        lbl_36 = new QLabel(row_checkBox_36);
        lbl_36->setObjectName(QString::fromUtf8("lbl_36"));
        lbl_36->setTextFormat(Qt::RichText);

        row_checkBox_36_l->addWidget(lbl_36);

        sp_37 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        row_checkBox_36_l->addItem(sp_37);


        hFree->addWidget(row_checkBox_36);

        checkBox_37 = new QCheckBox(grpTarget);
        checkBox_37->setObjectName(QString::fromUtf8("checkBox_37"));
        checkBox_37->setChecked(false);

        hFree->addWidget(checkBox_37);

        row_checkBox_35 = new QWidget(grpTarget);
        row_checkBox_35->setObjectName(QString::fromUtf8("row_checkBox_35"));
        row_checkBox_35_l = new QHBoxLayout(row_checkBox_35);
        row_checkBox_35_l->setObjectName(QString::fromUtf8("row_checkBox_35_l"));
        row_checkBox_35_l->setContentsMargins(0, 0, 0, 0);
        checkBox_35 = new QCheckBox(row_checkBox_35);
        checkBox_35->setObjectName(QString::fromUtf8("checkBox_35"));
        checkBox_35->setChecked(true);
        checkBox_35->setEnabled(true);

        row_checkBox_35_l->addWidget(checkBox_35);

        lbl_38 = new QLabel(row_checkBox_35);
        lbl_38->setObjectName(QString::fromUtf8("lbl_38"));
        lbl_38->setTextFormat(Qt::RichText);

        row_checkBox_35_l->addWidget(lbl_38);

        sp_39 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        row_checkBox_35_l->addItem(sp_39);


        hFree->addWidget(row_checkBox_35);

        checkBox_38 = new QCheckBox(grpTarget);
        checkBox_38->setObjectName(QString::fromUtf8("checkBox_38"));
        checkBox_38->setChecked(true);

        hFree->addWidget(checkBox_38);

        sp_42 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hFree->addItem(sp_42);


        vTarget->addLayout(hFree);

        hFixedRV = new QHBoxLayout();
        hFixedRV->setSpacing(6);
        hFixedRV->setObjectName(QString::fromUtf8("hFixedRV"));
        labelFixedRV = new QLabel(grpTarget);
        labelFixedRV->setObjectName(QString::fromUtf8("labelFixedRV"));

        hFixedRV->addWidget(labelFixedRV);

        lineEdit_fixA = new QLineEdit(grpTarget);
        lineEdit_fixA->setObjectName(QString::fromUtf8("lineEdit_fixA"));

        hFixedRV->addWidget(lineEdit_fixA);

        labelFixedRVB = new QLabel(grpTarget);
        labelFixedRVB->setObjectName(QString::fromUtf8("labelFixedRVB"));

        hFixedRV->addWidget(labelFixedRVB);

        lineEdit_fixB = new QLineEdit(grpTarget);
        lineEdit_fixB->setObjectName(QString::fromUtf8("lineEdit_fixB"));

        hFixedRV->addWidget(lineEdit_fixB);


        vTarget->addLayout(hFixedRV);


        tabOpt_v->addWidget(grpTarget);

        grpMethod = new QGroupBox(tabOpt_c);
        grpMethod->setObjectName(QString::fromUtf8("grpMethod"));
        grdMethod = new QGridLayout(grpMethod);
        grdMethod->setObjectName(QString::fromUtf8("grdMethod"));
        lbl_45 = new QLabel(grpMethod);
        lbl_45->setObjectName(QString::fromUtf8("lbl_45"));

        grdMethod->addWidget(lbl_45, 0, 0, 1, 1);

        comboBox_method = new QComboBox(grpMethod);
        comboBox_method->addItem(QString());
        comboBox_method->addItem(QString());
        comboBox_method->setObjectName(QString::fromUtf8("comboBox_method"));
        comboBox_method->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        comboBox_method->setMinimumSize(QSize(210, 0));
        comboBox_method->setMaximumSize(QSize(210, 16777215));

        grdMethod->addWidget(comboBox_method, 0, 1, 1, 3);

        lbl_46 = new QLabel(grpMethod);
        lbl_46->setObjectName(QString::fromUtf8("lbl_46"));

        grdMethod->addWidget(lbl_46, 1, 0, 1, 1);

        checkBox_9 = new QCheckBox(grpMethod);
        checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));

        grdMethod->addWidget(checkBox_9, 1, 1, 1, 1);

        checkBox_11 = new QCheckBox(grpMethod);
        checkBox_11->setObjectName(QString::fromUtf8("checkBox_11"));

        grdMethod->addWidget(checkBox_11, 1, 2, 1, 1);

        checkBox_12 = new QCheckBox(grpMethod);
        checkBox_12->setObjectName(QString::fromUtf8("checkBox_12"));

        grdMethod->addWidget(checkBox_12, 1, 3, 1, 1);

        lbl_47 = new QLabel(grpMethod);
        lbl_47->setObjectName(QString::fromUtf8("lbl_47"));

        grdMethod->addWidget(lbl_47, 2, 0, 1, 1);

        spinBox_3 = new QSpinBox(grpMethod);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setMaximum(9999);
        spinBox_3->setValue(9999);
        spinBox_3->setMinimumSize(QSize(80, 0));
        spinBox_3->setMaximumSize(QSize(80, 16777215));

        grdMethod->addWidget(spinBox_3, 2, 1, 1, 1);


        tabOpt_v->addWidget(grpMethod);

        grpStop = new QGroupBox(tabOpt_c);
        grpStop->setObjectName(QString::fromUtf8("grpStop"));
        grdStop = new QGridLayout(grpStop);
        grdStop->setObjectName(QString::fromUtf8("grdStop"));
        checkBox_23 = new QCheckBox(grpStop);
        checkBox_23->setObjectName(QString::fromUtf8("checkBox_23"));

        grdStop->addWidget(checkBox_23, 0, 0, 1, 1);

        doubleSpinBox_17 = new QDoubleSpinBox(grpStop);
        doubleSpinBox_17->setObjectName(QString::fromUtf8("doubleSpinBox_17"));
        doubleSpinBox_17->setDecimals(3);
        doubleSpinBox_17->setMinimum(0.001000000000000);
        doubleSpinBox_17->setMaximum(1000.000000000000000);
        doubleSpinBox_17->setSingleStep(0.050000000000000);
        doubleSpinBox_17->setValue(0.100000000000000);
        doubleSpinBox_17->setMinimumSize(QSize(70, 0));
        doubleSpinBox_17->setMaximumSize(QSize(70, 16777215));

        grdStop->addWidget(doubleSpinBox_17, 0, 1, 1, 1);

        lbl_48 = new QLabel(grpStop);
        lbl_48->setObjectName(QString::fromUtf8("lbl_48"));

        grdStop->addWidget(lbl_48, 0, 2, 1, 1);

        spinBox_11 = new QSpinBox(grpStop);
        spinBox_11->setObjectName(QString::fromUtf8("spinBox_11"));
        spinBox_11->setMaximum(100000);
        spinBox_11->setValue(0);
        spinBox_11->setMinimumSize(QSize(70, 0));
        spinBox_11->setMaximumSize(QSize(70, 16777215));

        grdStop->addWidget(spinBox_11, 0, 3, 1, 1);

        lbl_49 = new QLabel(grpStop);
        lbl_49->setObjectName(QString::fromUtf8("lbl_49"));

        grdStop->addWidget(lbl_49, 0, 4, 1, 1);

        checkBox_32 = new QCheckBox(grpStop);
        checkBox_32->setObjectName(QString::fromUtf8("checkBox_32"));

        grdStop->addWidget(checkBox_32, 1, 0, 1, 5);


        tabOpt_v->addWidget(grpStop);

        grpErr = new QGroupBox(tabOpt_c);
        grpErr->setObjectName(QString::fromUtf8("grpErr"));
        grdErr = new QGridLayout(grpErr);
        grdErr->setObjectName(QString::fromUtf8("grdErr"));
        lbl_52 = new QLabel(grpErr);
        lbl_52->setObjectName(QString::fromUtf8("lbl_52"));

        grdErr->addWidget(lbl_52, 0, 0, 1, 1);

        comboBox_err = new QComboBox(grpErr);
        comboBox_err->addItem(QString());
        comboBox_err->addItem(QString());
        comboBox_err->setObjectName(QString::fromUtf8("comboBox_err"));
        comboBox_err->setMinimumSize(QSize(130, 0));
        comboBox_err->setMaximumSize(QSize(130, 16777215));

        grdErr->addWidget(comboBox_err, 0, 1, 1, 1);

        lbl_53 = new QLabel(grpErr);
        lbl_53->setObjectName(QString::fromUtf8("lbl_53"));

        grdErr->addWidget(lbl_53, 0, 2, 1, 1);

        spinBox_mc = new QSpinBox(grpErr);
        spinBox_mc->setObjectName(QString::fromUtf8("spinBox_mc"));
        spinBox_mc->setMinimum(3);
        spinBox_mc->setMaximum(10000);
        spinBox_mc->setValue(30);
        spinBox_mc->setMinimumSize(QSize(80, 0));
        spinBox_mc->setMaximumSize(QSize(80, 16777215));

        grdErr->addWidget(spinBox_mc, 0, 3, 1, 1);

        sp_54 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        grdErr->addItem(sp_54, 0, 4, 1, 1);


        tabOpt_v->addWidget(grpErr);

        grpFlux = new QGroupBox(tabOpt_c);
        grpFlux->setObjectName(QString::fromUtf8("grpFlux"));
        grdFlux = new QGridLayout(grpFlux);
        grdFlux->setObjectName(QString::fromUtf8("grdFlux"));
        pushButton_13 = new QPushButton(grpFlux);
        pushButton_13->setObjectName(QString::fromUtf8("pushButton_13"));

        grdFlux->addWidget(pushButton_13, 0, 0, 1, 1);

        lbl_50 = new QLabel(grpFlux);
        lbl_50->setObjectName(QString::fromUtf8("lbl_50"));

        grdFlux->addWidget(lbl_50, 0, 1, 1, 1);

        checkBox_29 = new QCheckBox(grpFlux);
        checkBox_29->setObjectName(QString::fromUtf8("checkBox_29"));

        grdFlux->addWidget(checkBox_29, 0, 2, 1, 1);

        checkBox_30 = new QCheckBox(grpFlux);
        checkBox_30->setObjectName(QString::fromUtf8("checkBox_30"));

        grdFlux->addWidget(checkBox_30, 0, 3, 1, 1);

        sp_51 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        grdFlux->addItem(sp_51, 0, 4, 1, 1);


        tabOpt_v->addWidget(grpFlux);

        grpLineScan = new QGroupBox(tabOpt_c);
        grpLineScan->setObjectName(QString::fromUtf8("grpLineScan"));
        grdLineScan = new QGridLayout(grpLineScan);
        grdLineScan->setObjectName(QString::fromUtf8("grdLineScan"));
        lbl_ls1 = new QLabel(grpLineScan);
        lbl_ls1->setObjectName(QString::fromUtf8("lbl_ls1"));

        grdLineScan->addWidget(lbl_ls1, 0, 0, 1, 1);

        lineEdit_lsGrid = new QLineEdit(grpLineScan);
        lineEdit_lsGrid->setObjectName(QString::fromUtf8("lineEdit_lsGrid"));

        grdLineScan->addWidget(lineEdit_lsGrid, 0, 1, 1, 1);

        lbl_ls2 = new QLabel(grpLineScan);
        lbl_ls2->setObjectName(QString::fromUtf8("lbl_ls2"));

        grdLineScan->addWidget(lbl_ls2, 0, 2, 1, 1);

        lineEdit_lsLines = new QLineEdit(grpLineScan);
        lineEdit_lsLines->setObjectName(QString::fromUtf8("lineEdit_lsLines"));

        grdLineScan->addWidget(lineEdit_lsLines, 0, 3, 1, 1);

        lbl_ls3 = new QLabel(grpLineScan);
        lbl_ls3->setObjectName(QString::fromUtf8("lbl_ls3"));

        grdLineScan->addWidget(lbl_ls3, 1, 0, 1, 1);

        lineEdit_lsOut = new QLineEdit(grpLineScan);
        lineEdit_lsOut->setObjectName(QString::fromUtf8("lineEdit_lsOut"));

        grdLineScan->addWidget(lineEdit_lsOut, 1, 1, 1, 1);

        pushButton_lsFind = new QPushButton(grpLineScan);
        pushButton_lsFind->setObjectName(QString::fromUtf8("pushButton_lsFind"));

        grdLineScan->addWidget(pushButton_lsFind, 1, 2, 1, 1);

        pushButton_lsScan = new QPushButton(grpLineScan);
        pushButton_lsScan->setObjectName(QString::fromUtf8("pushButton_lsScan"));

        grdLineScan->addWidget(pushButton_lsScan, 1, 3, 1, 1);

        checkBox_lsSave = new QCheckBox(grpLineScan);
        checkBox_lsSave->setObjectName(QString::fromUtf8("checkBox_lsSave"));

        grdLineScan->addWidget(checkBox_lsSave, 2, 0, 1, 2);

        pushButton_lsShow = new QPushButton(grpLineScan);
        pushButton_lsShow->setObjectName(QString::fromUtf8("pushButton_lsShow"));

        grdLineScan->addWidget(pushButton_lsShow, 2, 2, 1, 2);

        progressBar_lsScan = new QProgressBar(grpLineScan);
        progressBar_lsScan->setObjectName(QString::fromUtf8("progressBar_lsScan"));
        progressBar_lsScan->setMaximum(1);
        progressBar_lsScan->setValue(0);

        grdLineScan->addWidget(progressBar_lsScan, 3, 0, 1, 4);


        tabOpt_v->addWidget(grpLineScan);

        sp_55 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        tabOpt_v->addItem(sp_55);

        tabOpt_s->setWidget(tabOpt_c);

        tabOpt_l->addWidget(tabOpt_s);

        tabSettings->addTab(tabOpt, QString());
        tabOut = new QWidget();
        tabOut->setObjectName(QString::fromUtf8("tabOut"));
        tabOut_l = new QVBoxLayout(tabOut);
        tabOut_l->setObjectName(QString::fromUtf8("tabOut_l"));
        tabOut_l->setContentsMargins(0, 0, 0, 0);
        tabOut_s = new QScrollArea(tabOut);
        tabOut_s->setObjectName(QString::fromUtf8("tabOut_s"));
        tabOut_s->setWidgetResizable(true);
        tabOut_s->setFrameShape(QFrame::NoFrame);
        tabOut_c = new QWidget();
        tabOut_c->setObjectName(QString::fromUtf8("tabOut_c"));
        tabOut_v = new QVBoxLayout(tabOut_c);
        tabOut_v->setSpacing(8);
        tabOut_v->setObjectName(QString::fromUtf8("tabOut_v"));
        tabOut_v->setContentsMargins(8, 8, 8, 8);
        grpNames = new QGroupBox(tabOut_c);
        grpNames->setObjectName(QString::fromUtf8("grpNames"));
        grdNames = new QGridLayout(grpNames);
        grdNames->setObjectName(QString::fromUtf8("grdNames"));
        lbl_56 = new QLabel(grpNames);
        lbl_56->setObjectName(QString::fromUtf8("lbl_56"));

        grdNames->addWidget(lbl_56, 0, 0, 1, 1);

        lineEdit_25 = new QLineEdit(grpNames);
        lineEdit_25->setObjectName(QString::fromUtf8("lineEdit_25"));

        grdNames->addWidget(lineEdit_25, 0, 1, 1, 1);

        lineEdit_26 = new QLineEdit(grpNames);
        lineEdit_26->setObjectName(QString::fromUtf8("lineEdit_26"));
        lineEdit_26->setMinimumSize(QSize(60, 0));
        lineEdit_26->setMaximumSize(QSize(60, 16777215));

        grdNames->addWidget(lineEdit_26, 0, 2, 1, 1);

        pushButton_11 = new QPushButton(grpNames);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));

        grdNames->addWidget(pushButton_11, 0, 3, 1, 1);

        lbl_57 = new QLabel(grpNames);
        lbl_57->setObjectName(QString::fromUtf8("lbl_57"));

        grdNames->addWidget(lbl_57, 1, 0, 1, 4);


        tabOut_v->addWidget(grpNames);

        grpResults = new QGroupBox(tabOut_c);
        grpResults->setObjectName(QString::fromUtf8("grpResults"));
        grdResults = new QGridLayout(grpResults);
        grdResults->setObjectName(QString::fromUtf8("grdResults"));
        lbl_58 = new QLabel(grpResults);
        lbl_58->setObjectName(QString::fromUtf8("lbl_58"));

        grdResults->addWidget(lbl_58, 0, 0, 1, 1);

        lineEdit_5 = new QLineEdit(grpResults);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        grdResults->addWidget(lineEdit_5, 0, 1, 1, 1);

        lbl_59 = new QLabel(grpResults);
        lbl_59->setObjectName(QString::fromUtf8("lbl_59"));

        grdResults->addWidget(lbl_59, 1, 0, 1, 1);

        lineEdit_8 = new QLineEdit(grpResults);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));

        grdResults->addWidget(lineEdit_8, 1, 1, 1, 1);

        lbl_60 = new QLabel(grpResults);
        lbl_60->setObjectName(QString::fromUtf8("lbl_60"));

        grdResults->addWidget(lbl_60, 2, 0, 1, 1);

        lineEdit_10 = new QLineEdit(grpResults);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));

        grdResults->addWidget(lineEdit_10, 2, 1, 1, 1);

        lbl_61 = new QLabel(grpResults);
        lbl_61->setObjectName(QString::fromUtf8("lbl_61"));

        grdResults->addWidget(lbl_61, 3, 0, 1, 1);

        lineEdit_13 = new QLineEdit(grpResults);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));

        grdResults->addWidget(lineEdit_13, 3, 1, 1, 1);

        lbl_62 = new QLabel(grpResults);
        lbl_62->setObjectName(QString::fromUtf8("lbl_62"));

        grdResults->addWidget(lbl_62, 4, 0, 1, 1);

        lineEdit_20 = new QLineEdit(grpResults);
        lineEdit_20->setObjectName(QString::fromUtf8("lineEdit_20"));

        grdResults->addWidget(lineEdit_20, 4, 1, 1, 1);

        lbl_63 = new QLabel(grpResults);
        lbl_63->setObjectName(QString::fromUtf8("lbl_63"));

        grdResults->addWidget(lbl_63, 5, 0, 1, 1);

        lineEdit_24 = new QLineEdit(grpResults);
        lineEdit_24->setObjectName(QString::fromUtf8("lineEdit_24"));

        grdResults->addWidget(lineEdit_24, 5, 1, 1, 1);

        lbl_stat = new QLabel(grpResults);
        lbl_stat->setObjectName(QString::fromUtf8("lbl_stat"));

        grdResults->addWidget(lbl_stat, 6, 0, 1, 1);

        lineEdit_stat = new QLineEdit(grpResults);
        lineEdit_stat->setObjectName(QString::fromUtf8("lineEdit_stat"));

        grdResults->addWidget(lineEdit_stat, 6, 1, 1, 1);

        checkBox_19 = new QCheckBox(grpResults);
        checkBox_19->setObjectName(QString::fromUtf8("checkBox_19"));

        grdResults->addWidget(checkBox_19, 7, 0, 1, 2);

        checkBox_norm = new QCheckBox(grpResults);
        checkBox_norm->setObjectName(QString::fromUtf8("checkBox_norm"));

        grdResults->addWidget(checkBox_norm, 8, 0, 1, 2);

        lbl_normKnot = new QLabel(grpResults);
        lbl_normKnot->setObjectName(QString::fromUtf8("lbl_normKnot"));

        grdResults->addWidget(lbl_normKnot, 9, 0, 1, 1);

        doubleSpinBox_normKnot = new QDoubleSpinBox(grpResults);
        doubleSpinBox_normKnot->setObjectName(QString::fromUtf8("doubleSpinBox_normKnot"));
        doubleSpinBox_normKnot->setDecimals(0);
        doubleSpinBox_normKnot->setMinimum(10.000000000000000);
        doubleSpinBox_normKnot->setMaximum(5000.000000000000000);
        doubleSpinBox_normKnot->setSingleStep(10.000000000000000);
        doubleSpinBox_normKnot->setValue(80.000000000000000);

        grdResults->addWidget(doubleSpinBox_normKnot, 9, 1, 1, 1);

        lbl_normLow = new QLabel(grpResults);
        lbl_normLow->setObjectName(QString::fromUtf8("lbl_normLow"));

        grdResults->addWidget(lbl_normLow, 10, 0, 1, 1);

        doubleSpinBox_normLow = new QDoubleSpinBox(grpResults);
        doubleSpinBox_normLow->setObjectName(QString::fromUtf8("doubleSpinBox_normLow"));
        doubleSpinBox_normLow->setDecimals(1);
        doubleSpinBox_normLow->setMinimum(0.500000000000000);
        doubleSpinBox_normLow->setMaximum(5.000000000000000);
        doubleSpinBox_normLow->setSingleStep(0.100000000000000);
        doubleSpinBox_normLow->setValue(1.500000000000000);

        grdResults->addWidget(doubleSpinBox_normLow, 10, 1, 1, 1);


        tabOut_v->addWidget(grpResults);

        grpState = new QGroupBox(tabOut_c);
        grpState->setObjectName(QString::fromUtf8("grpState"));
        grdState = new QGridLayout(grpState);
        grdState->setObjectName(QString::fromUtf8("grdState"));
        lbl_64 = new QLabel(grpState);
        lbl_64->setObjectName(QString::fromUtf8("lbl_64"));

        grdState->addWidget(lbl_64, 0, 0, 1, 1);

        lineEdit_14 = new QLineEdit(grpState);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));

        grdState->addWidget(lineEdit_14, 0, 1, 1, 1);

        lineEdit_15 = new QLineEdit(grpState);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));

        grdState->addWidget(lineEdit_15, 0, 2, 1, 1);

        lbl_65 = new QLabel(grpState);
        lbl_65->setObjectName(QString::fromUtf8("lbl_65"));

        grdState->addWidget(lbl_65, 1, 0, 1, 1);

        lineEdit_16 = new QLineEdit(grpState);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));

        grdState->addWidget(lineEdit_16, 1, 1, 1, 1);

        lineEdit_17 = new QLineEdit(grpState);
        lineEdit_17->setObjectName(QString::fromUtf8("lineEdit_17"));

        grdState->addWidget(lineEdit_17, 1, 2, 1, 1);

        lbl_66 = new QLabel(grpState);
        lbl_66->setObjectName(QString::fromUtf8("lbl_66"));

        grdState->addWidget(lbl_66, 2, 0, 1, 1);

        lineEdit_21 = new QLineEdit(grpState);
        lineEdit_21->setObjectName(QString::fromUtf8("lineEdit_21"));

        grdState->addWidget(lineEdit_21, 2, 1, 1, 2);

        lbl_best = new QLabel(grpState);
        lbl_best->setObjectName(QString::fromUtf8("lbl_best"));

        grdState->addWidget(lbl_best, 3, 0, 1, 1);

        lineEdit_best = new QLineEdit(grpState);
        lineEdit_best->setObjectName(QString::fromUtf8("lineEdit_best"));

        grdState->addWidget(lineEdit_best, 3, 1, 1, 2);

        lbl_optdata = new QLabel(grpState);
        lbl_optdata->setObjectName(QString::fromUtf8("lbl_optdata"));

        grdState->addWidget(lbl_optdata, 4, 0, 1, 1);

        lineEdit_optdata = new QLineEdit(grpState);
        lineEdit_optdata->setObjectName(QString::fromUtf8("lineEdit_optdata"));

        grdState->addWidget(lineEdit_optdata, 4, 1, 1, 2);


        tabOut_v->addWidget(grpState);

        grpAnalysis = new QGroupBox(tabOut_c);
        grpAnalysis->setObjectName(QString::fromUtf8("grpAnalysis"));
        grdAnalysis = new QGridLayout(grpAnalysis);
        grdAnalysis->setObjectName(QString::fromUtf8("grdAnalysis"));
        checkBox_anaFit = new QCheckBox(grpAnalysis);
        checkBox_anaFit->setObjectName(QString::fromUtf8("checkBox_anaFit"));
        checkBox_anaFit->setChecked(true);

        grdAnalysis->addWidget(checkBox_anaFit, 0, 0, 1, 1);

        lineEdit_anaFit = new QLineEdit(grpAnalysis);
        lineEdit_anaFit->setObjectName(QString::fromUtf8("lineEdit_anaFit"));

        grdAnalysis->addWidget(lineEdit_anaFit, 0, 1, 1, 2);

        checkBox_anaLines = new QCheckBox(grpAnalysis);
        checkBox_anaLines->setObjectName(QString::fromUtf8("checkBox_anaLines"));
        checkBox_anaLines->setChecked(true);

        grdAnalysis->addWidget(checkBox_anaLines, 1, 0, 1, 1);

        lineEdit_lines = new QLineEdit(grpAnalysis);
        lineEdit_lines->setObjectName(QString::fromUtf8("lineEdit_lines"));

        grdAnalysis->addWidget(lineEdit_lines, 1, 1, 1, 1);

        lineEdit_anaLines = new QLineEdit(grpAnalysis);
        lineEdit_anaLines->setObjectName(QString::fromUtf8("lineEdit_anaLines"));

        grdAnalysis->addWidget(lineEdit_anaLines, 1, 2, 1, 1);


        tabOut_v->addWidget(grpAnalysis);

        sp_67 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        tabOut_v->addItem(sp_67);

        tabOut_s->setWidget(tabOut_c);

        tabOut_l->addWidget(tabOut_s);

        tabSettings->addTab(tabOut, QString());

        hBody->addWidget(tabSettings);

        tabPlots = new QTabWidget(centralWidget);
        tabPlots->setObjectName(QString::fromUtf8("tabPlots"));
        tabSpectra = new QWidget();
        tabSpectra->setObjectName(QString::fromUtf8("tabSpectra"));
        tabSpectra_l = new QVBoxLayout(tabSpectra);
        tabSpectra_l->setSpacing(4);
        tabSpectra_l->setObjectName(QString::fromUtf8("tabSpectra_l"));
        tabSpectra_l->setContentsMargins(4, 4, 4, 4);
        labelLegend = new QLabel(tabSpectra);
        labelLegend->setObjectName(QString::fromUtf8("labelLegend"));
        labelLegend->setTextFormat(Qt::RichText);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelLegend->sizePolicy().hasHeightForWidth());
        labelLegend->setSizePolicy(sizePolicy);

        tabSpectra_l->addWidget(labelLegend);

        customPlot = new QCustomPlot(tabSpectra);
        customPlot->setObjectName(QString::fromUtf8("customPlot"));
        customPlot->setMinimumSize(QSize(300, 260));

        tabSpectra_l->addWidget(customPlot);

        customPlot_2 = new QCustomPlot(tabSpectra);
        customPlot_2->setObjectName(QString::fromUtf8("customPlot_2"));
        customPlot_2->setMinimumSize(QSize(300, 120));

        tabSpectra_l->addWidget(customPlot_2);

        hSpecOpts = new QHBoxLayout();
        hSpecOpts->setObjectName(QString::fromUtf8("hSpecOpts"));
        checkBox_10 = new QCheckBox(tabSpectra);
        checkBox_10->setObjectName(QString::fromUtf8("checkBox_10"));

        hSpecOpts->addWidget(checkBox_10);

        checkBox_22 = new QCheckBox(tabSpectra);
        checkBox_22->setObjectName(QString::fromUtf8("checkBox_22"));

        hSpecOpts->addWidget(checkBox_22);

        lbl_68 = new QLabel(tabSpectra);
        lbl_68->setObjectName(QString::fromUtf8("lbl_68"));

        hSpecOpts->addWidget(lbl_68);

        doubleSpinBox_5 = new QDoubleSpinBox(tabSpectra);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));
        doubleSpinBox_5->setSingleStep(0.100000000000000);
        doubleSpinBox_5->setValue(0.300000000000000);
        doubleSpinBox_5->setMinimumSize(QSize(70, 0));
        doubleSpinBox_5->setMaximumSize(QSize(70, 16777215));

        hSpecOpts->addWidget(doubleSpinBox_5);

        lbl_69 = new QLabel(tabSpectra);
        lbl_69->setObjectName(QString::fromUtf8("lbl_69"));

        hSpecOpts->addWidget(lbl_69);

        spinBox_6 = new QSpinBox(tabSpectra);
        spinBox_6->setObjectName(QString::fromUtf8("spinBox_6"));
        spinBox_6->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_6->setReadOnly(true);
        spinBox_6->setEnabled(false);
        spinBox_6->setMaximum(9999);
        spinBox_6->setMinimumSize(QSize(70, 0));
        spinBox_6->setMaximumSize(QSize(70, 16777215));

        hSpecOpts->addWidget(spinBox_6);

        sp_70 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hSpecOpts->addItem(sp_70);

        pushButton_6 = new QPushButton(tabSpectra);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));

        hSpecOpts->addWidget(pushButton_6);


        tabSpectra_l->addLayout(hSpecOpts);

        hStatPlots = new QHBoxLayout();
        hStatPlots->setObjectName(QString::fromUtf8("hStatPlots"));
        customPlot_5 = new QCustomPlot(tabSpectra);
        customPlot_5->setObjectName(QString::fromUtf8("customPlot_5"));
        customPlot_5->setMinimumSize(QSize(300, 200));

        hStatPlots->addWidget(customPlot_5);

        customPlot_6 = new QCustomPlot(tabSpectra);
        customPlot_6->setObjectName(QString::fromUtf8("customPlot_6"));
        customPlot_6->setMinimumSize(QSize(250, 200));

        hStatPlots->addWidget(customPlot_6);


        tabSpectra_l->addLayout(hStatPlots);

        hStatOpts = new QHBoxLayout();
        hStatOpts->setObjectName(QString::fromUtf8("hStatOpts"));
        lbl_72 = new QLabel(tabSpectra);
        lbl_72->setObjectName(QString::fromUtf8("lbl_72"));

        hStatOpts->addWidget(lbl_72);

        doubleSpinBox_st1 = new QDoubleSpinBox(tabSpectra);
        doubleSpinBox_st1->setObjectName(QString::fromUtf8("doubleSpinBox_st1"));
        doubleSpinBox_st1->setDecimals(2);
        doubleSpinBox_st1->setMaximum(1000000.000000000000000);
        doubleSpinBox_st1->setSingleStep(1.000000000000000);
        doubleSpinBox_st1->setMinimumSize(QSize(100, 0));
        doubleSpinBox_st1->setMaximumSize(QSize(100, 16777215));

        hStatOpts->addWidget(doubleSpinBox_st1);

        lbl_73 = new QLabel(tabSpectra);
        lbl_73->setObjectName(QString::fromUtf8("lbl_73"));

        hStatOpts->addWidget(lbl_73);

        doubleSpinBox_st2 = new QDoubleSpinBox(tabSpectra);
        doubleSpinBox_st2->setObjectName(QString::fromUtf8("doubleSpinBox_st2"));
        doubleSpinBox_st2->setDecimals(2);
        doubleSpinBox_st2->setMaximum(1000000.000000000000000);
        doubleSpinBox_st2->setSingleStep(1.000000000000000);
        doubleSpinBox_st2->setMinimumSize(QSize(100, 0));
        doubleSpinBox_st2->setMaximumSize(QSize(100, 16777215));

        hStatOpts->addWidget(doubleSpinBox_st2);

        lbl_74 = new QLabel(tabSpectra);
        lbl_74->setObjectName(QString::fromUtf8("lbl_74"));

        hStatOpts->addWidget(lbl_74);

        sp_75 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hStatOpts->addItem(sp_75);

        pushButton_stat = new QPushButton(tabSpectra);
        pushButton_stat->setObjectName(QString::fromUtf8("pushButton_stat"));

        hStatOpts->addWidget(pushButton_stat);


        tabSpectra_l->addLayout(hStatOpts);

        tabPlots->addTab(tabSpectra, QString());
        tabConv = new QWidget();
        tabConv->setObjectName(QString::fromUtf8("tabConv"));
        tabConv_l = new QVBoxLayout(tabConv);
        tabConv_l->setSpacing(4);
        tabConv_l->setObjectName(QString::fromUtf8("tabConv_l"));
        tabConv_l->setContentsMargins(4, 4, 4, 4);
        customPlot_4 = new QCustomPlot(tabConv);
        customPlot_4->setObjectName(QString::fromUtf8("customPlot_4"));
        customPlot_4->setMinimumSize(QSize(300, 200));

        tabConv_l->addWidget(customPlot_4);

        hTextOut = new QHBoxLayout();
        hTextOut->setObjectName(QString::fromUtf8("hTextOut"));
        groupBox_messages = new QGroupBox(tabConv);
        groupBox_messages->setObjectName(QString::fromUtf8("groupBox_messages"));
        vMessages = new QVBoxLayout(groupBox_messages);
        vMessages->setObjectName(QString::fromUtf8("vMessages"));
        vMessages->setContentsMargins(4, 4, 4, 4);
        plainTextEdit_2 = new QPlainTextEdit(groupBox_messages);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));

        vMessages->addWidget(plainTextEdit_2);


        hTextOut->addWidget(groupBox_messages);

        groupBox_rvs = new QGroupBox(tabConv);
        groupBox_rvs->setObjectName(QString::fromUtf8("groupBox_rvs"));
        vRVs = new QVBoxLayout(groupBox_rvs);
        vRVs->setObjectName(QString::fromUtf8("vRVs"));
        vRVs->setContentsMargins(4, 4, 4, 4);
        plainTextEdit = new QPlainTextEdit(groupBox_rvs);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        vRVs->addWidget(plainTextEdit);


        hTextOut->addWidget(groupBox_rvs);


        tabConv_l->addLayout(hTextOut);

        tabPlots->addTab(tabConv, QString());

        hBody->addWidget(tabPlots);


        vCentral->addLayout(hBody);

        frameStatus = new QFrame(centralWidget);
        frameStatus->setObjectName(QString::fromUtf8("frameStatus"));
        frameStatus->setFrameShape(QFrame::StyledPanel);
        hStatus = new QHBoxLayout(frameStatus);
        hStatus->setObjectName(QString::fromUtf8("hStatus"));
        hStatus->setContentsMargins(4, 4, 4, 4);
        lbl_81 = new QLabel(frameStatus);
        lbl_81->setObjectName(QString::fromUtf8("lbl_81"));

        hStatus->addWidget(lbl_81);

        spinBox_4 = new QSpinBox(frameStatus);
        spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));
        spinBox_4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_4->setReadOnly(true);
        spinBox_4->setEnabled(false);
        spinBox_4->setMaximum(9999);
        spinBox_4->setMinimumSize(QSize(70, 0));
        spinBox_4->setMaximumSize(QSize(70, 16777215));

        hStatus->addWidget(spinBox_4);

        lbl_82 = new QLabel(frameStatus);
        lbl_82->setObjectName(QString::fromUtf8("lbl_82"));

        hStatus->addWidget(lbl_82);

        spinBox_5 = new QSpinBox(frameStatus);
        spinBox_5->setObjectName(QString::fromUtf8("spinBox_5"));
        spinBox_5->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_5->setReadOnly(true);
        spinBox_5->setEnabled(false);
        spinBox_5->setMaximum(99999);
        spinBox_5->setMinimumSize(QSize(80, 0));
        spinBox_5->setMaximumSize(QSize(80, 16777215));

        hStatus->addWidget(spinBox_5);

        lbl_83 = new QLabel(frameStatus);
        lbl_83->setObjectName(QString::fromUtf8("lbl_83"));

        hStatus->addWidget(lbl_83);

        lineEdit_7 = new QLineEdit(frameStatus);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setReadOnly(true);
        lineEdit_7->setEnabled(false);
        lineEdit_7->setMinimumSize(QSize(150, 0));
        lineEdit_7->setMaximumSize(QSize(150, 16777215));

        hStatus->addWidget(lineEdit_7);

        lbl_84 = new QLabel(frameStatus);
        lbl_84->setObjectName(QString::fromUtf8("lbl_84"));

        hStatus->addWidget(lbl_84);

        lineEdit_6 = new QLineEdit(frameStatus);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setReadOnly(true);
        lineEdit_6->setEnabled(false);
        lineEdit_6->setMinimumSize(QSize(110, 0));
        lineEdit_6->setMaximumSize(QSize(110, 16777215));

        hStatus->addWidget(lineEdit_6);

        sp_85 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hStatus->addItem(sp_85);

        labelRunInfo = new QLabel(frameStatus);
        labelRunInfo->setObjectName(QString::fromUtf8("labelRunInfo"));

        hStatus->addWidget(labelRunInfo);


        vCentral->addWidget(frameStatus);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1585, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockSubtract = new QDockWidget(MainWindow);
        dockSubtract->setObjectName(QString::fromUtf8("dockSubtract"));
        dockSubtractContents = new QWidget();
        dockSubtractContents->setObjectName(QString::fromUtf8("dockSubtractContents"));
        grdSubtract = new QGridLayout(dockSubtractContents);
        grdSubtract->setObjectName(QString::fromUtf8("grdSubtract"));
        grdSubtract->setContentsMargins(8, 8, 8, 8);
        pushButton_12 = new QPushButton(dockSubtractContents);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));

        grdSubtract->addWidget(pushButton_12, 0, 0, 1, 1);

        checkBox_20 = new QCheckBox(dockSubtractContents);
        checkBox_20->setObjectName(QString::fromUtf8("checkBox_20"));

        grdSubtract->addWidget(checkBox_20, 0, 1, 1, 1);

        checkBox_21 = new QCheckBox(dockSubtractContents);
        checkBox_21->setObjectName(QString::fromUtf8("checkBox_21"));

        grdSubtract->addWidget(checkBox_21, 0, 2, 1, 1);

        lbl_87 = new QLabel(dockSubtractContents);
        lbl_87->setObjectName(QString::fromUtf8("lbl_87"));

        grdSubtract->addWidget(lbl_87, 0, 3, 1, 1);

        lineEdit_27 = new QLineEdit(dockSubtractContents);
        lineEdit_27->setObjectName(QString::fromUtf8("lineEdit_27"));

        grdSubtract->addWidget(lineEdit_27, 0, 4, 1, 1);

        checkBox_24 = new QCheckBox(dockSubtractContents);
        checkBox_24->setObjectName(QString::fromUtf8("checkBox_24"));

        grdSubtract->addWidget(checkBox_24, 1, 0, 1, 1);

        checkBox_25 = new QCheckBox(dockSubtractContents);
        checkBox_25->setObjectName(QString::fromUtf8("checkBox_25"));

        grdSubtract->addWidget(checkBox_25, 1, 1, 1, 1);

        checkBox_26 = new QCheckBox(dockSubtractContents);
        checkBox_26->setObjectName(QString::fromUtf8("checkBox_26"));

        grdSubtract->addWidget(checkBox_26, 1, 2, 1, 1);

        checkBox_28 = new QCheckBox(dockSubtractContents);
        checkBox_28->setObjectName(QString::fromUtf8("checkBox_28"));

        grdSubtract->addWidget(checkBox_28, 1, 3, 1, 1);

        lineEdit_28 = new QLineEdit(dockSubtractContents);
        lineEdit_28->setObjectName(QString::fromUtf8("lineEdit_28"));

        grdSubtract->addWidget(lineEdit_28, 1, 4, 1, 1);

        lbl_88 = new QLabel(dockSubtractContents);
        lbl_88->setObjectName(QString::fromUtf8("lbl_88"));

        grdSubtract->addWidget(lbl_88, 2, 0, 1, 5);

        dockSubtract->setWidget(dockSubtractContents);
        MainWindow->addDockWidget(Qt::BottomDockWidgetArea, dockSubtract);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionOpen_Configuration);
        menuFile->addAction(actionSave_Configuration);
        menuFile->addAction(actionSave_Configuration_As);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuTools->addAction(actionSpectrum_Plotter);
        menuTools->addAction(actionCorrelation);
        menuTools->addAction(actionEditor);
        menuTools->addAction(actionArithmetic);
        menuTools->addAction(actionSubtract);
        menuTools->addAction(actionOrbitFit);
        menuTools->addAction(actionFluxTemplate);
        menuTools->addAction(actionBaryCorr);
        menuTools->addAction(actionPeriod);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionBug_Report);
        menuHelp->addAction(actionManual);

        retranslateUi(MainWindow);

        tabSettings->setCurrentIndex(0);
        tabPlots->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Spectangular", nullptr));
        actionSpectrum_Plotter->setText(QCoreApplication::translate("MainWindow", "Spectrum Plotter", nullptr));
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        actionBug_Report->setText(QCoreApplication::translate("MainWindow", "Bug Report", nullptr));
        actionCorrelation->setText(QCoreApplication::translate("MainWindow", "Correlation", nullptr));
        actionEditor->setText(QCoreApplication::translate("MainWindow", "Editor", nullptr));
        actionArithmetic->setText(QCoreApplication::translate("MainWindow", "Arithmetic", nullptr));
        actionOpen_Configuration->setText(QCoreApplication::translate("MainWindow", "Open Configuration...", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen_Configuration->setToolTip(QCoreApplication::translate("MainWindow", "Load settings from a configuration file (.sps).", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionOpen_Configuration->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_Configuration->setText(QCoreApplication::translate("MainWindow", "Save Configuration", nullptr));
#if QT_CONFIG(tooltip)
        actionSave_Configuration->setToolTip(QCoreApplication::translate("MainWindow", "Save the settings to the current configuration file.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSave_Configuration->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_Configuration_As->setText(QCoreApplication::translate("MainWindow", "Save Configuration As...", nullptr));
#if QT_CONFIG(tooltip)
        actionSave_Configuration_As->setToolTip(QCoreApplication::translate("MainWindow", "Save the settings to a new configuration file (.sps).", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSave_Configuration_As->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(tooltip)
        actionQuit->setToolTip(QCoreApplication::translate("MainWindow", "Close Spectangular.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionManual->setText(QCoreApplication::translate("MainWindow", "Manual", nullptr));
        actionSubtract->setText(QCoreApplication::translate("MainWindow", "Subtract component\342\200\246", nullptr));
        actionOrbitFit->setText(QCoreApplication::translate("MainWindow", "Orbit fit to RVs\342\200\246", nullptr));
        actionFluxTemplate->setText(QCoreApplication::translate("MainWindow", "Flux ratio from synthetic spectra\342\200\246", nullptr));
        actionBaryCorr->setText(QCoreApplication::translate("MainWindow", "Barycentric correction\342\200\246", nullptr));
        actionPeriod->setText(QCoreApplication::translate("MainWindow", "Period analysis\342\200\246", nullptr));
        centralWidget->setStyleSheet(QCoreApplication::translate("MainWindow", "QLineEdit[readOnly=\"true\"], QAbstractSpinBox[readOnly=\"true\"] { color: palette(text); background: palette(window); }", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_2->setToolTip(QCoreApplication::translate("MainWindow", "Read the spectra and RVs, build the matrix.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "1  Read data", nullptr));
        lbl_76->setText(QCoreApplication::translate("MainWindow", "\342\200\272", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_3->setToolTip(QCoreApplication::translate("MainWindow", "Solve once with the current RVs and write the output files.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "2  Solve", nullptr));
        lbl_77->setText(QCoreApplication::translate("MainWindow", "\342\200\272", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_4->setToolTip(QCoreApplication::translate("MainWindow", "Optimise RVs or orbital elements (method: Optimisation tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "3  Optimise", nullptr));
        lbl_78->setText(QCoreApplication::translate("MainWindow", "\342\200\272", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_8->setToolTip(QCoreApplication::translate("MainWindow", "Standard errors of the orbital elements.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "4  Error estimation", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_7->setToolTip(QCoreApplication::translate("MainWindow", "Stop the running optimisation or error estimation.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        lbl_80->setText(QCoreApplication::translate("MainWindow", "Work folder", nullptr));
        lineEdit_4->setText(QString());
        grpSpectra->setTitle(QCoreApplication::translate("MainWindow", "Spectra", nullptr));
        lbl_1->setText(QCoreApplication::translate("MainWindow", "File name", nullptr));
        lineEdit_3->setText(QString());
        lbl_2->setText(QCoreApplication::translate("MainWindow", "index", nullptr));
        lbl_3->setText(QCoreApplication::translate("MainWindow", "to", nullptr));
        lbl_4->setText(QCoreApplication::translate("MainWindow", "Format", nullptr));
        lbl_5->setText(QCoreApplication::translate("MainWindow", "FITS table", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_9->setToolTip(QCoreApplication::translate("MainWindow", "FITS: name of the binary table extension (optional).", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_9->setText(QString());
        lbl_6->setText(QCoreApplication::translate("MainWindow", "\316\273 col.", nullptr));
        lineEdit_11->setText(QString());
        lbl_7->setText(QCoreApplication::translate("MainWindow", "flux col.", nullptr));
        lineEdit_12->setText(QString());
        checkBox_42->setText(QCoreApplication::translate("MainWindow", "Resample to a common log grid, step", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_42->setToolTip(QCoreApplication::translate("MainWindow", "Resample all spectra onto one common, equidistant log10(lambda) grid when reading (cubic spline), with the step given here. Without the tick this happens automatically (step auto) when the spectra are not on one common log grid: linear wavelengths (Angstrom) or individual log grids. Range: the part covered by all spectra.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        doubleSpinBox_18->setToolTip(QCoreApplication::translate("MainWindow", "Step of the common log grid in km/s per pixel. auto: the finest median sampling among the spectra.", nullptr));
#endif // QT_CONFIG(tooltip)
        doubleSpinBox_18->setSpecialValueText(QCoreApplication::translate("MainWindow", "auto", nullptr));
        doubleSpinBox_18->setSuffix(QCoreApplication::translate("MainWindow", " km/s", nullptr));
        grpSystem->setTitle(QCoreApplication::translate("MainWindow", "System", nullptr));
        lbl_8->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        checkBox_3->setText(QCoreApplication::translate("MainWindow", "SB1", nullptr));
        checkBox_4->setText(QCoreApplication::translate("MainWindow", "SB2", nullptr));
        checkBox_34->setText(QCoreApplication::translate("MainWindow", "SB3", nullptr));
        checkBox_8->setText(QCoreApplication::translate("MainWindow", "Static lines", nullptr));
        lbl_9->setText(QCoreApplication::translate("MainWindow", "Flux ratio F<sub>A</sub>/F<sub>B</sub>", nullptr));
        lbl_10->setText(QCoreApplication::translate("MainWindow", "per spectrum: Tools \342\200\272 Editor", nullptr));
        lbl_11->setText(QCoreApplication::translate("MainWindow", "\316\273 dependence", nullptr));
        comboBox_ratio->setItemText(0, QCoreApplication::translate("MainWindow", "constant", nullptr));
        comboBox_ratio->setItemText(1, QCoreApplication::translate("MainWindow", "Planck", nullptr));
        comboBox_ratio->setItemText(2, QCoreApplication::translate("MainWindow", "table", nullptr));

#if QT_CONFIG(tooltip)
        comboBox_ratio->setToolTip(QCoreApplication::translate("MainWindow", "Wavelength dependence of the flux ratio F_A/F_B. constant: the same ratio at all wavelengths. Planck: ratio of two Planck functions with T_A and T_B. table: file with two columns (wavelength [A], ratio), interpolated linearly. The value in 'Flux ratio' (and per spectrum in the Editor or from the flux-ratio optimisation) is the ratio at the reference wavelength.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_12->setText(QCoreApplication::translate("MainWindow", "at \316\273<sub>ref</sub>", nullptr));
        doubleSpinBox_lref->setSuffix(QCoreApplication::translate("MainWindow", " \303\205", nullptr));
        doubleSpinBox_lref->setSpecialValueText(QCoreApplication::translate("MainWindow", "auto", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_lref->setToolTip(QCoreApplication::translate("MainWindow", "Reference wavelength [A] at which the flux ratio is given. auto: centre of the data (geometric mean).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_13->setText(QCoreApplication::translate("MainWindow", "T<sub>A</sub>, T<sub>B</sub>", nullptr));
        doubleSpinBox_TA->setSuffix(QCoreApplication::translate("MainWindow", " K", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_TA->setToolTip(QCoreApplication::translate("MainWindow", "Effective temperature of A [K] (Planck).", nullptr));
#endif // QT_CONFIG(tooltip)
        doubleSpinBox_TB->setSuffix(QCoreApplication::translate("MainWindow", " K", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_TB->setToolTip(QCoreApplication::translate("MainWindow", "Effective temperature of B [K] (Planck).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_14->setText(QCoreApplication::translate("MainWindow", "table", nullptr));
        lineEdit_ratio->setText(QCoreApplication::translate("MainWindow", "fluxratio.dat", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_ratio->setToolTip(QCoreApplication::translate("MainWindow", "Flux-ratio table: two columns, wavelength [A] and F_A/F_B.", nullptr));
#endif // QT_CONFIG(tooltip)
        grpRV->setTitle(QCoreApplication::translate("MainWindow", "Radial velocities (start values; selects what is fitted)", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_2->setToolTip(QCoreApplication::translate("MainWindow", "Start RVs from a file (one line per spectrum: RV_A RV_B). The optimisation fits individual RVs.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_2->setText(QCoreApplication::translate("MainWindow", "From file", nullptr));
        lineEdit_2->setText(QString());
#if QT_CONFIG(tooltip)
        checkBox->setToolTip(QCoreApplication::translate("MainWindow", "Start RVs from cross-correlation files <name><index>.txt. The optimisation fits individual RVs.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox->setText(QCoreApplication::translate("MainWindow", "Cross-correlation", nullptr));
        lineEdit->setText(QString());
        checkBox_13->setText(QCoreApplication::translate("MainWindow", "flip", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_16->setToolTip(QCoreApplication::translate("MainWindow", "RVs from the orbital elements file (below) and the observation times. The optimisation fits orbital elements.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_16->setText(QCoreApplication::translate("MainWindow", "From orbit, times", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_15->setToolTip(QCoreApplication::translate("MainWindow", "Compute and list the RVs from the orbital elements file and the times.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_15->setText(QCoreApplication::translate("MainWindow", "Show RVs", nullptr));
        lbl_orbit->setText(QCoreApplication::translate("MainWindow", "Orbital elements", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_orbit->setToolTip(QCoreApplication::translate("MainWindow", "File with the seven orbital elements (P, e, K_A, K_B, gamma, T_peri, omega in rad), one per line, in the work folder. Start values of the orbit mode; the orbit optimisation writes its best elements back to it. Used by the Editor and the orbit fit (Tools) as well.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_bc->setText(QCoreApplication::translate("MainWindow", "Barycentric correction", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_bc->setToolTip(QCoreApplication::translate("MainWindow", "File with the barycentric correction BC [km/s] per spectrum (last number of each line, e.g. <prefix>barycorr.txt of Tools -> Barycentric correction); empty: none. Spectra in the observatory frame: the RVs of the orbit are converted to the observatory frame, (1 + v_obs/c) = (1 + v/c) / (1 + BC/c). Barycentrically corrected spectra: the static lines are shifted by BC. Needed for the orbit mode with static lines.", nullptr));
#endif // QT_CONFIG(tooltip)
        comboBox_bcframe->setItemText(0, QCoreApplication::translate("MainWindow", "observatory frame", nullptr));
        comboBox_bcframe->setItemText(1, QCoreApplication::translate("MainWindow", "barycentric (corrected)", nullptr));

#if QT_CONFIG(tooltip)
        comboBox_bcframe->setToolTip(QCoreApplication::translate("MainWindow", "Frame of the spectra: observatory frame (as observed; static lines at rest) or barycentrically corrected (lambda_bary = lambda_obs (1 + BC/c); static lines move by BC).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_tellw->setText(QCoreApplication::translate("MainWindow", "Static line strength", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_tellw->setToolTip(QCoreApplication::translate("MainWindow", "Start values of the strength of the static lines per spectrum: a file with one value per line, e.g. the airmass (<prefix>airmass.txt of Tools -> Barycentric correction), scaled to a mean of 1. Empty: from edit.dat (Editor) or 1.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        checkBox_tellfit->setToolTip(QCoreApplication::translate("MainWindow", "Fit the strength of the static lines per spectrum (telluric lines vary with airmass and water vapour): Solve and the start of an optimisation alternate solving and a least-squares scale per spectrum until the strengths no longer change (at most 10 times); during an optimisation they are kept. Mean 1. Exclude saturated bands (Model tab): their depth does not scale linearly.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_tellfit->setText(QCoreApplication::translate("MainWindow", "fit per spectrum", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_tellrange->setToolTip(QCoreApplication::translate("MainWindow", "Solve the static spectrum only in the wavelength ranges of the file (observatory frame), zero elsewhere: fewer unknowns, faster, and no broad structure outside. If the file does not exist, a default with all telluric O2 and H2O systems of the optical (weak ones included) is written to the work folder; edit it, or let \"Detect\" find the ranges in your data.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_tellrange->setText(QCoreApplication::translate("MainWindow", "Static lines only in", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_tellrange->setToolTip(QCoreApplication::translate("MainWindow", "File of ranges: per line from to [A] (air, observatory frame) and optionally a group name (e.g. O2, H2O); '#' comments. With \"fit per spectrum\" every group gets its own strength per spectrum.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        pushButton_telldetect->setToolTip(QCoreApplication::translate("MainWindow", "Ranges from the static spectrum of the last Solve (with static lines; best over the whole range, option above off): pixels deviating by more than the given number of sigma from a running median (5 A), widened by 0.5 A, gaps below 1 A closed; absorption and emission (sky lines). Written to <file>_detected.<ext> and set above.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_telldetect->setText(QCoreApplication::translate("MainWindow", "Detect", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_telldet->setToolTip(QCoreApplication::translate("MainWindow", "Threshold of \"Detect\" in units of the noise of the static spectrum (robust, from the median absolute deviation).", nullptr));
#endif // QT_CONFIG(tooltip)
        doubleSpinBox_telldet->setSuffix(QCoreApplication::translate("MainWindow", " \317\203", nullptr));
        grpNoise->setTitle(QCoreApplication::translate("MainWindow", "Noise", nullptr));
        lbl_15->setText(QCoreApplication::translate("MainWindow", "S/N of the spectra", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_16->setToolTip(QCoreApplication::translate("MainWindow", "Signal-to-noise ratio of the (continuum-normalised) spectra; noise sigma = 1/(S/N). auto: estimated from the residuum (residuum^2 / degrees of freedom).", nullptr));
#endif // QT_CONFIG(tooltip)
        doubleSpinBox_16->setSpecialValueText(QCoreApplication::translate("MainWindow", "auto", nullptr));
        lbl_16->setText(QCoreApplication::translate("MainWindow", "auto: from the residuum", nullptr));
        checkBox_41->setText(QCoreApplication::translate("MainWindow", "Weight per spectrum by S/N from", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_41->setToolTip(QCoreApplication::translate("MainWindow", "Weighted least squares: each spectrum is weighted with 1/sigma^2, sigma = 1/(S/N), S/N per spectrum from the file (one value per line, same order as the spectra). Solve writes an estimate from the residuals to snr_estimated.dat.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        lineEdit_29->setToolTip(QCoreApplication::translate("MainWindow", "File with one S/N value per spectrum, or \"auto\": estimate the S/N per spectrum from the residuals (3 iterations at the first solve, written to snr_estimated.dat).", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_29->setText(QCoreApplication::translate("MainWindow", "snr.dat", nullptr));
        checkBox_align->setText(QCoreApplication::translate("MainWindow", "Align continua to the mean, smoothing", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_align->setToolTip(QCoreApplication::translate("MainWindow", "Before disentangling, each spectrum is divided by its smoothed ratio to the mean of all spectra (boxcar of the given width, applied twice; excluded ranges left out). This removes differences of the normalisation between the spectra, which the disentangling turns into broad anti-correlated wiggles of A and B. Also removes real broad differences, e.g. from shifted Balmer wings, on scales of the width and above.", nullptr));
#endif // QT_CONFIG(tooltip)
        doubleSpinBox_alignWidth->setSuffix(QCoreApplication::translate("MainWindow", " \303\205", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_alignWidth->setToolTip(QCoreApplication::translate("MainWindow", "Width of the boxcar (applied twice) for the ratio of each spectrum to the mean spectrum. Must be much larger than the RV shifts of the lines (a few \303\205).", nullptr));
#endif // QT_CONFIG(tooltip)
        grpRead->setTitle(QCoreApplication::translate("MainWindow", "After reading", nullptr));
        lbl_17->setText(QCoreApplication::translate("MainWindow", "max |RV| A [km/s]", nullptr));
        lbl_18->setText(QCoreApplication::translate("MainWindow", "max |RV| B [km/s]", nullptr));
        lbl_19->setText(QCoreApplication::translate("MainWindow", "km/s per pixel", nullptr));
        tabSettings->setTabText(tabSettings->indexOf(tabData), QCoreApplication::translate("MainWindow", "Data", nullptr));
        grpShift->setTitle(QCoreApplication::translate("MainWindow", "Shift between pixels", nullptr));
        lbl_21->setText(QCoreApplication::translate("MainWindow", "Interpolation", nullptr));
        comboBox_interp->setItemText(0, QCoreApplication::translate("MainWindow", "Linear", nullptr));
        comboBox_interp->setItemText(1, QCoreApplication::translate("MainWindow", "Cubic", nullptr));

#if QT_CONFIG(tooltip)
        comboBox_interp->setToolTip(QCoreApplication::translate("MainWindow", "Shift of the component spectra between pixels. Linear: 2 pixels, residuum has kinks at whole-pixel shifts. Cubic: 4-pixel Catmull-Rom (Keys) interpolation, smooth in the RVs and more accurate for narrow lines.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_23->setText(QCoreApplication::translate("MainWindow", "Cubic: smooth in the RVs, more accurate, 3\342\200\2234\303\227 slower per solve.", nullptr));
        grpSolver->setTitle(QCoreApplication::translate("MainWindow", "Linear system", nullptr));
        lbl_24->setText(QCoreApplication::translate("MainWindow", "Solver", nullptr));
        checkBox_5->setText(QCoreApplication::translate("MainWindow", "LSQR", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_5->setToolTip(QCoreApplication::translate("MainWindow", "Iterative least-squares solver on the sparse matrix (fast, little memory).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_6->setText(QCoreApplication::translate("MainWindow", "SVD (DaC)", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_6->setToolTip(QCoreApplication::translate("MainWindow", "Truncated SVD (divide and conquer) on a dense copy of the matrix (slow, needs much memory).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_7->setText(QCoreApplication::translate("MainWindow", "Bidiagonal Matrix", nullptr));
        lbl_26->setText(QCoreApplication::translate("MainWindow", "Cores", nullptr));
#if QT_CONFIG(tooltip)
        spinBox_8->setToolTip(QCoreApplication::translate("MainWindow", "Number of cores used by LSQR (OpenMP) and by the SVD (multi-threaded BLAS such as OpenBLAS). Default 2: in a scaling test on long spectra more cores took longer.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_27->setText(QCoreApplication::translate("MainWindow", "of", nullptr));
        lbl_tdamp->setText(QCoreApplication::translate("MainWindow", "Damping static", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_tdamp->setToolTip(QCoreApplication::translate("MainWindow", "LSQR with static lines: damping of the static spectrum, relative to the squared norm of each of its columns (0: off). The broad structure of the static spectrum is nearly undetermined (only the relative motion of the stars separates it from A and B): without damping LSQR needs many more iterations (iota Tri A, 47 x 70 000 pixels: 11 720 instead of 3 870) and A, B and the static spectrum take up large broad deviations of opposite sign. The lines of the static spectrum become smaller by this fraction.", nullptr));
#endif // QT_CONFIG(tooltip)
        grpResiduum->setTitle(QCoreApplication::translate("MainWindow", "Residuum", nullptr));
        lbl_28->setText(QCoreApplication::translate("MainWindow", "Criterion", nullptr));
        checkBox_14->setText(QCoreApplication::translate("MainWindow", "Mean (\317\207\302\262)", nullptr));
        checkBox_15->setText(QCoreApplication::translate("MainWindow", "Peak", nullptr));
        checkBox_40->setText(QCoreApplication::translate("MainWindow", "Exclude edges (pixels covered by single spectra only)", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_40->setToolTip(QCoreApplication::translate("MainWindow", "Compute the residuum without the outer pixels of each spectrum (RV range in pixels + 2). There the reconstructed spectra are constrained by single spectra only, and the residuum jumps when an RV crosses a pixel boundary.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_33->setText(QCoreApplication::translate("MainWindow", "Only from", nullptr));
        lbl_30->setText(QCoreApplication::translate("MainWindow", "to", nullptr));
        lbl_31->setText(QCoreApplication::translate("MainWindow", "\303\205", nullptr));
        checkBox_mask->setText(QCoreApplication::translate("MainWindow", "Exclude ranges from", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_mask->setToolTip(QCoreApplication::translate("MainWindow", "Leave wavelength ranges out of the solution and the residuum, e.g. telluric bands: file in the work folder with from and to in \303\205 (frame of the input spectra), one range per line. If telluric_mask.dat does not exist, it is created with the strongest telluric bands and the strong chromospheric lines (Balmer, Ca II H&K, Mg b, Na D, Ca II IR triplet) and can then be edited.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_mask->setText(QCoreApplication::translate("MainWindow", "telluric_mask.dat", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_mask->setToolTip(QCoreApplication::translate("MainWindow", "File with the excluded ranges (from, to in \303\205 per line, further text ignored; # comments). Default telluric_mask.dat: created with the strongest telluric bands and strong chromospheric lines if missing.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_resstatic->setText(QCoreApplication::translate("MainWindow", "Exclude the ranges of the static lines from the residuum", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_resstatic->setToolTip(QCoreApplication::translate("MainWindow", "Compute the residuum (and the residual statistics and the S/N estimate) without the pixels in the ranges of the file of the Data tab, \"Static lines only in\" (observatory frame; shifted by BC in corrected spectra). They stay in the solution. Takes effect with Read data or when ticked.", nullptr));
#endif // QT_CONFIG(tooltip)
        tabSettings->setTabText(tabSettings->indexOf(tabModel), QCoreApplication::translate("MainWindow", "Model", nullptr));
        grpTarget->setTitle(QCoreApplication::translate("MainWindow", "What is fitted", nullptr));
        labelFitTarget->setText(QCoreApplication::translate("MainWindow", "Individual RVs", nullptr));
        labelFreeElements->setText(QCoreApplication::translate("MainWindow", "Free orbital elements:", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_18->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_18->setText(QCoreApplication::translate("MainWindow", "P", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_17->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_17->setText(QCoreApplication::translate("MainWindow", "e", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_31->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_31->setText(QString());
        lbl_34->setText(QCoreApplication::translate("MainWindow", "K<sub>A</sub>", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_36->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_36->setText(QString());
        lbl_36->setText(QCoreApplication::translate("MainWindow", "K<sub>B</sub>", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_37->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab). Default fixed: the disentangling does not determine gamma (a common shift of the RVs is absorbed by the component spectra, also with static lines).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_37->setText(QCoreApplication::translate("MainWindow", "\316\263", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_35->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_35->setText(QString());
        lbl_38->setText(QCoreApplication::translate("MainWindow", "T<sub>peri</sub>", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_38->setToolTip(QCoreApplication::translate("MainWindow", "Free in the orbit fit (unticked: fixed at the value in the orbital elements file, Data tab).", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_38->setText(QCoreApplication::translate("MainWindow", "\317\211", nullptr));
        labelFixedRV->setText(QCoreApplication::translate("MainWindow", "Fixed RVs (spectrum numbers) A", nullptr));
        lineEdit_fixA->setPlaceholderText(QCoreApplication::translate("MainWindow", "none", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_fixA->setToolTip(QCoreApplication::translate("MainWindow", "Spectra whose RV of component A stays at its start value, e.g. 0, 3, 7-9 (numbers as in the file names). Empty: all free.", nullptr));
#endif // QT_CONFIG(tooltip)
        labelFixedRVB->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        lineEdit_fixB->setPlaceholderText(QCoreApplication::translate("MainWindow", "none", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_fixB->setToolTip(QCoreApplication::translate("MainWindow", "Spectra whose RV of component B stays at its start value, e.g. 0, 3, 7-9. Empty: all free.", nullptr));
#endif // QT_CONFIG(tooltip)
        grpMethod->setTitle(QCoreApplication::translate("MainWindow", "Method", nullptr));
        lbl_45->setText(QCoreApplication::translate("MainWindow", "Method", nullptr));
        comboBox_method->setItemText(0, QCoreApplication::translate("MainWindow", "Nelder-Mead", nullptr));
        comboBox_method->setItemText(1, QCoreApplication::translate("MainWindow", "Levenberg-Marquardt", nullptr));

#if QT_CONFIG(tooltip)
        comboBox_method->setToolTip(QCoreApplication::translate("MainWindow", "Nelder-Mead: simplex search, robust from far away, slow near the minimum. Levenberg-Marquardt: uses the derivatives of the residuals (variable projection), converges in few iterations near the minimum; local method, needs a reasonable start. Only for residuum \"Mean\".", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_46->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        checkBox_9->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
        checkBox_11->setText(QCoreApplication::translate("MainWindow", "Continue", nullptr));
        checkBox_12->setText(QCoreApplication::translate("MainWindow", "From initial file", nullptr));
        lbl_47->setText(QCoreApplication::translate("MainWindow", "Max. iterations", nullptr));
        grpStop->setTitle(QCoreApplication::translate("MainWindow", "Stop and restarts", nullptr));
        checkBox_23->setText(QCoreApplication::translate("MainWindow", "Auto Stop when \316\224\317\207\302\262 <", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_17->setToolTip(QCoreApplication::translate("MainWindow", "Auto Stop: the optimisation has converged when the best chi^2 improved by less than this value within the window. With Reinitiate also the criterion between two restarts.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_48->setText(QCoreApplication::translate("MainWindow", "over", nullptr));
#if QT_CONFIG(tooltip)
        spinBox_11->setToolTip(QCoreApplication::translate("MainWindow", "Window for Auto Stop: number of iterations over which the improvement is measured. auto: 10 x number of parameters, at least 200.", nullptr));
#endif // QT_CONFIG(tooltip)
        spinBox_11->setSpecialValueText(QCoreApplication::translate("MainWindow", "auto", nullptr));
        lbl_49->setText(QCoreApplication::translate("MainWindow", "it.", nullptr));
        checkBox_32->setText(QCoreApplication::translate("MainWindow", "Reinitiate after convergence", nullptr));
        grpErr->setTitle(QCoreApplication::translate("MainWindow", "Error estimation (button 4)", nullptr));
        lbl_52->setText(QCoreApplication::translate("MainWindow", "Method", nullptr));
        comboBox_err->setItemText(0, QCoreApplication::translate("MainWindow", "Curvature", nullptr));
        comboBox_err->setItemText(1, QCoreApplication::translate("MainWindow", "Monte Carlo", nullptr));

#if QT_CONFIG(tooltip)
        comboBox_err->setToolTip(QCoreApplication::translate("MainWindow", "Curvature: quadratic model of the residuum around the best point (fast; a lower bound at moderate S/N). Monte Carlo: noise with the scatter of the residuals added to the best model, the orbit refitted (Levenberg-Marquardt) for every realisation; errors = scatter of the fits (slow: one fit per realisation).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_53->setText(QCoreApplication::translate("MainWindow", "realisations", nullptr));
#if QT_CONFIG(tooltip)
        spinBox_mc->setToolTip(QCoreApplication::translate("MainWindow", "Number of Monte Carlo realisations.", nullptr));
#endif // QT_CONFIG(tooltip)
        grpFlux->setTitle(QCoreApplication::translate("MainWindow", "Flux ratios", nullptr));
        pushButton_13->setText(QCoreApplication::translate("MainWindow", "Optimise flux ratios", nullptr));
        lbl_50->setText(QCoreApplication::translate("MainWindow", "criterion", nullptr));
        checkBox_29->setText(QCoreApplication::translate("MainWindow", "Residuum", nullptr));
        checkBox_30->setText(QCoreApplication::translate("MainWindow", "slope m", nullptr));
        grpLineScan->setTitle(QCoreApplication::translate("MainWindow", "Line width scan (orbit grid)", nullptr));
        lbl_ls1->setText(QCoreApplication::translate("MainWindow", "Orbit grid file", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_lsGrid->setToolTip(QCoreApplication::translate("MainWindow", "7 rows (P, e, K_A, K_B, gamma, T_peri, omega as in the orbital element file), one column per orbit", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_lsGrid->setText(QCoreApplication::translate("MainWindow", "orbitgrid.dat", nullptr));
        lbl_ls2->setText(QCoreApplication::translate("MainWindow", "Line list", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_lsLines->setToolTip(QCoreApplication::translate("MainWindow", "Lines of the scan, written by Find Lines / Apply", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_lsLines->setText(QCoreApplication::translate("MainWindow", "scanlines.dat", nullptr));
        lbl_ls3->setText(QCoreApplication::translate("MainWindow", "Result file", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_lsOut->setToolTip(QCoreApplication::translate("MainWindow", "Result of the scan: elements, residuum, width of the lines of the second component", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_lsOut->setText(QCoreApplication::translate("MainWindow", "linescan.dat", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_lsFind->setToolTip(QCoreApplication::translate("MainWindow", "Finds absorption lines in the current solution; add or remove them in the plot, then Apply", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_lsFind->setText(QCoreApplication::translate("MainWindow", "Find Lines", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_lsScan->setToolTip(QCoreApplication::translate("MainWindow", "Residuum and line width for every orbit of the grid (the other settings of the Data and Model tabs are used)", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_lsScan->setText(QCoreApplication::translate("MainWindow", "Scan orbits", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_lsSave->setToolTip(QCoreApplication::translate("MainWindow", "Writes the continuum-normalised A and B of every orbit (<result>_001_A.dat, _001_B.dat, ...; settings of the continuum as in the Output tab)", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_lsSave->setText(QCoreApplication::translate("MainWindow", "Save the spectra of every orbit", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_lsShow->setToolTip(QCoreApplication::translate("MainWindow", "Opens the result window from the files of an earlier scan (result file and <result>_profiles.dat), without computing", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_lsShow->setText(QCoreApplication::translate("MainWindow", "Show results", nullptr));
#if QT_CONFIG(tooltip)
        progressBar_lsScan->setToolTip(QCoreApplication::translate("MainWindow", "Orbits of the grid computed so far", nullptr));
#endif // QT_CONFIG(tooltip)
        progressBar_lsScan->setFormat(QCoreApplication::translate("MainWindow", "%v / %m orbits", nullptr));
        tabSettings->setTabText(tabSettings->indexOf(tabOpt), QCoreApplication::translate("MainWindow", "Optimisation", nullptr));
        grpNames->setTitle(QCoreApplication::translate("MainWindow", "File names", nullptr));
        lbl_56->setText(QCoreApplication::translate("MainWindow", "Common name", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "Apply", nullptr));
        lbl_57->setText(QCoreApplication::translate("MainWindow", "Sets all names below; each can still be edited.", nullptr));
        grpResults->setTitle(QCoreApplication::translate("MainWindow", "Results", nullptr));
        lbl_58->setText(QCoreApplication::translate("MainWindow", "Component A", nullptr));
        lbl_59->setText(QCoreApplication::translate("MainWindow", "Component B", nullptr));
        lbl_60->setText(QCoreApplication::translate("MainWindow", "Static lines", nullptr));
        lbl_61->setText(QCoreApplication::translate("MainWindow", "Differences", nullptr));
        lbl_62->setText(QCoreApplication::translate("MainWindow", "Mean difference", nullptr));
        lbl_63->setText(QCoreApplication::translate("MainWindow", "Error estimation", nullptr));
        lbl_stat->setText(QCoreApplication::translate("MainWindow", "Residual statistics", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_stat->setToolTip(QCoreApplication::translate("MainWindow", "Residuals per spectrum (mean, rms, std of observation - model), written by Update in the convergence tab.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_19->setText(QCoreApplication::translate("MainWindow", "Save every improvement (files and plots)", nullptr));
        checkBox_norm->setText(QCoreApplication::translate("MainWindow", "Also write continuum-normalised A and B (*_norm) and plot them", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_norm->setToolTip(QCoreApplication::translate("MainWindow", "Fits a continuum to each disentangled component: a cubic spline per piece between excluded ranges, through the upper points (points more than the chosen number of sigma below the fit are lines, more than 3 sigma above are spikes; sigma from the points above the fit; iterated). Writes A and B divided by it as <name>_norm.<ext> beside the files as they come from the disentangling, and plots the normalised spectra.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_normKnot->setText(QCoreApplication::translate("MainWindow", "Continuum: spline knots every", nullptr));
        doubleSpinBox_normKnot->setSuffix(QCoreApplication::translate("MainWindow", " \303\205", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_normKnot->setToolTip(QCoreApplication::translate("MainWindow", "Distance of the spline knots of the continuum fit. Smaller: follows broader wiggles more closely but also broad lines (Balmer wings); larger: smoother. Pieces shorter than this get a straight line (shorter than 20 \303\205: a constant).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_normLow->setText(QCoreApplication::translate("MainWindow", "Continuum: reject points below the fit by more than", nullptr));
        doubleSpinBox_normLow->setSuffix(QCoreApplication::translate("MainWindow", " \317\203", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_normLow->setToolTip(QCoreApplication::translate("MainWindow", "Points this far below the continuum fit count as lines and are left out (sigma from the points above the fit). Smaller: the continuum follows the upper envelope more closely (line-rich, cool stars); larger: closer to the mean of the continuum points (white noise, few lines).", nullptr));
#endif // QT_CONFIG(tooltip)
        grpState->setTitle(QCoreApplication::translate("MainWindow", "Optimisation state", nullptr));
        lbl_64->setText(QCoreApplication::translate("MainWindow", "Initial simplex", nullptr));
        lbl_65->setText(QCoreApplication::translate("MainWindow", "Current simplex", nullptr));
        lbl_66->setText(QCoreApplication::translate("MainWindow", "Log", nullptr));
        lbl_best->setText(QCoreApplication::translate("MainWindow", "Best state", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_best->setToolTip(QCoreApplication::translate("MainWindow", "Best values found so far, written on every improvement while an optimisation runs; the program adds _rv (RVs of all spectra) or _orbit (orbital elements) to the name, e.g. beststate_rv.dat. Continue and every restart start from it if it exists and belongs to the current mode (and, for RVs, the same spectra); otherwise from the inputs, and the file is created anew. Give each setup in the same folder its own name.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_optdata->setText(QCoreApplication::translate("MainWindow", "Convergence data", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_optdata->setToolTip(QCoreApplication::translate("MainWindow", "Iteration, evaluation and residuum of every step of the optimisation (convergence plot).", nullptr));
#endif // QT_CONFIG(tooltip)
        grpAnalysis->setTitle(QCoreApplication::translate("MainWindow", "Analysis of the solution (run in Tools \342\206\222 Period analysis)", nullptr));
        checkBox_anaFit->setText(QCoreApplication::translate("MainWindow", "RV O-C and line strength", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_anaFit->setToolTip(QCoreApplication::translate("MainWindow", "With the component spectra fixed, every spectrum is fitted with its own shift of A and B (O-C of the RVs, with errors) and its own strength of the lines of A and B (1 = as in the component spectra).", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_anaFit->setText(QCoreApplication::translate("MainWindow", "rv_oc.dat", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_anaFit->setToolTip(QCoreApplication::translate("MainWindow", "Per spectrum: time, phase, RV of the model, O-C of the RV and line strength of A and B with standard errors, RMS of the fit.", nullptr));
#endif // QT_CONFIG(tooltip)
        checkBox_anaLines->setText(QCoreApplication::translate("MainWindow", "Statistics per line, windows from", nullptr));
#if QT_CONFIG(tooltip)
        checkBox_anaLines->setToolTip(QCoreApplication::translate("MainWindow", "Mean, RMS and equivalent width of observation - model in every window of the file (from, to in \303\205, frame of the input spectra, optional label; created with strong lines if missing), for every spectrum.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_lines->setText(QCoreApplication::translate("MainWindow", "lines.dat", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_lines->setToolTip(QCoreApplication::translate("MainWindow", "Windows: from, to in \303\205 (frame of the input spectra, wide enough for the RVs) and a label per line; # comments. Default lines.dat: created with Balmer, He I/II, Mg II, Na D, Ca II and Mg b if missing.", nullptr));
#endif // QT_CONFIG(tooltip)
        lineEdit_anaLines->setText(QCoreApplication::translate("MainWindow", "linestat.dat", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_anaLines->setToolTip(QCoreApplication::translate("MainWindow", "Per spectrum and window: time, phase, label, pixels, mean, RMS and equivalent width [\303\205] of observation - model.", nullptr));
#endif // QT_CONFIG(tooltip)
        tabSettings->setTabText(tabSettings->indexOf(tabOut), QCoreApplication::translate("MainWindow", "Output", nullptr));
        labelLegend->setText(QCoreApplication::translate("MainWindow", "<span style=\"color:#1f4fd1\">\342\226\240</span> Component A &nbsp; <span style=\"color:#d12f2f\">\342\226\240</span> Component B &nbsp; \342\226\240 Mean difference", nullptr));
        checkBox_10->setText(QCoreApplication::translate("MainWindow", "Live update", nullptr));
        checkBox_22->setText(QCoreApplication::translate("MainWindow", "Show static lines", nullptr));
        lbl_68->setText(QCoreApplication::translate("MainWindow", "Offset", nullptr));
#if QT_CONFIG(tooltip)
        lbl_68->setToolTip(QCoreApplication::translate("MainWindow", "Vertical offset between the differences of the spectra in the lower plot.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_69->setText(QCoreApplication::translate("MainWindow", "best at eval.", nullptr));
#if QT_CONFIG(tooltip)
        lbl_69->setToolTip(QCoreApplication::translate("MainWindow", "Number of the evaluation (Solve) that gave the best residuum so far.", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Plot", nullptr));
#if QT_CONFIG(tooltip)
        customPlot_6->setToolTip(QCoreApplication::translate("MainWindow", "RMS and mean of O-M against the number of the spectrum; spectra whose RMS lies more than 5 robust standard deviations (1.48 MAD) above the median are labelled.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_72->setText(QCoreApplication::translate("MainWindow", "Residuals per spectrum, from", nullptr));
        doubleSpinBox_st1->setSpecialValueText(QCoreApplication::translate("MainWindow", "auto", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_st1->setToolTip(QCoreApplication::translate("MainWindow", "Wavelength range of the residual statistics [A], e.g. one line. auto: residuum range of the Model tab (Only from ... to), if switched on, otherwise the whole spectrum (without the edges).", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_73->setText(QCoreApplication::translate("MainWindow", "to", nullptr));
        doubleSpinBox_st2->setSpecialValueText(QCoreApplication::translate("MainWindow", "auto", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBox_st2->setToolTip(QCoreApplication::translate("MainWindow", "Upper end of the wavelength range [A]. auto: as the lower end.", nullptr));
#endif // QT_CONFIG(tooltip)
        lbl_74->setText(QCoreApplication::translate("MainWindow", "\303\205", nullptr));
        pushButton_stat->setText(QCoreApplication::translate("MainWindow", "Update", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_stat->setToolTip(QCoreApplication::translate("MainWindow", "Recompute the residual statistics and write diffstatistic.dat.", nullptr));
#endif // QT_CONFIG(tooltip)
        tabPlots->setTabText(tabPlots->indexOf(tabSpectra), QCoreApplication::translate("MainWindow", "Spectra", nullptr));
        groupBox_messages->setTitle(QCoreApplication::translate("MainWindow", "Messages", nullptr));
        groupBox_rvs->setTitle(QCoreApplication::translate("MainWindow", "RVs", nullptr));
        tabPlots->setTabText(tabPlots->indexOf(tabConv), QCoreApplication::translate("MainWindow", "Convergence && messages", nullptr));
        lbl_81->setText(QCoreApplication::translate("MainWindow", "Iteration", nullptr));
        lbl_82->setText(QCoreApplication::translate("MainWindow", "Evaluations", nullptr));
        lbl_83->setText(QCoreApplication::translate("MainWindow", "Best residuum", nullptr));
        lineEdit_7->setText(QString());
        lbl_84->setText(QCoreApplication::translate("MainWindow", "Time [min]", nullptr));
        lineEdit_6->setText(QString());
        labelRunInfo->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MainWindow", "Tools", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
        dockSubtract->setWindowTitle(QCoreApplication::translate("MainWindow", "Subtract component", nullptr));
        pushButton_12->setText(QCoreApplication::translate("MainWindow", "Subtract", nullptr));
        checkBox_20->setText(QCoreApplication::translate("MainWindow", "A", nullptr));
        checkBox_21->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        lbl_87->setText(QCoreApplication::translate("MainWindow", "output name", nullptr));
        checkBox_24->setText(QCoreApplication::translate("MainWindow", "shift A", nullptr));
        checkBox_25->setText(QCoreApplication::translate("MainWindow", "shift B", nullptr));
        checkBox_26->setText(QCoreApplication::translate("MainWindow", "add", nullptr));
        checkBox_28->setText(QCoreApplication::translate("MainWindow", "mult", nullptr));
        lbl_88->setText(QCoreApplication::translate("MainWindow", "Subtracts the contribution of component A or B (from compl.txt) from every spectrum; optionally shifted to its rest frame, with a constant added or multiplied.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
