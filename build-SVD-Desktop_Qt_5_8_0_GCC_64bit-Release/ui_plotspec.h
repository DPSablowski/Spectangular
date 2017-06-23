/********************************************************************************
** Form generated from reading UI file 'plotspec.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTSPEC_H
#define UI_PLOTSPEC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_PlotSpec
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_9;
    QCustomPlot *customPlot;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_8;
    QDoubleSpinBox *doubleSpinBox_17;
    QDoubleSpinBox *doubleSpinBox_18;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *pushButton_12;
    QDoubleSpinBox *doubleSpinBox_15;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_13;
    QDoubleSpinBox *doubleSpinBox_16;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_15;
    QCheckBox *checkBox_16;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *checkBox_14;
    QDoubleSpinBox *doubleSpinBox_19;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *checkBox_13;
    QPushButton *pushButton_6;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_16;
    QLineEdit *lineEdit_8;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_17;
    QLineEdit *lineEdit_9;
    QPushButton *pushButton_9;
    QLabel *label_18;
    QHBoxLayout *horizontalLayout_14;
    QPushButton *pushButton_10;
    QLineEdit *lineEdit_10;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *pushButton_11;
    QLineEdit *lineEdit_11;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_4;
    QCheckBox *checkBox_4;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_6;
    QLineEdit *lineEdit_2;
    QLabel *label_7;
    QLineEdit *lineEdit_3;
    QLabel *label_9;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit_4;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QLabel *label_11;
    QSpinBox *spinBox;
    QLabel *label_13;
    QSpinBox *spinBox_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *checkBox_5;
    QDoubleSpinBox *doubleSpinBox_5;
    QCheckBox *checkBox_6;
    QDoubleSpinBox *doubleSpinBox_6;
    QCheckBox *checkBox_11;
    QDoubleSpinBox *doubleSpinBox_13;
    QCheckBox *checkBox_9;
    QDoubleSpinBox *doubleSpinBox_9;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBox_10;
    QPushButton *pushButton_5;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_10;
    QLineEdit *lineEdit_7;
    QCheckBox *checkBox_7;
    QDoubleSpinBox *doubleSpinBox_7;
    QCheckBox *checkBox_8;
    QDoubleSpinBox *doubleSpinBox_8;
    QCheckBox *checkBox_12;
    QDoubleSpinBox *doubleSpinBox_14;
    QCheckBox *checkBox_10;
    QDoubleSpinBox *doubleSpinBox_11;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBox_12;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_16;
    QCheckBox *checkBox_15;
    QLineEdit *lineEdit_12;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton;

    void setupUi(QDialog *PlotSpec)
    {
        if (PlotSpec->objectName().isEmpty())
            PlotSpec->setObjectName(QStringLiteral("PlotSpec"));
        PlotSpec->resize(1143, 642);
        PlotSpec->setMinimumSize(QSize(200, 50));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(205, 205, 205, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(230, 230, 230, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(102, 102, 102, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(137, 137, 137, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        PlotSpec->setPalette(palette);
        QFont font;
        font.setPointSize(9);
        PlotSpec->setFont(font);
        gridLayout = new QGridLayout(PlotSpec);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        customPlot = new QCustomPlot(PlotSpec);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy);
        customPlot->setMinimumSize(QSize(200, 0));

        horizontalLayout_9->addWidget(customPlot);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        doubleSpinBox_17 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_17->setObjectName(QStringLiteral("doubleSpinBox_17"));
        doubleSpinBox_17->setDecimals(4);
        doubleSpinBox_17->setMaximum(9999.99);

        horizontalLayout_8->addWidget(doubleSpinBox_17);

        doubleSpinBox_18 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_18->setObjectName(QStringLiteral("doubleSpinBox_18"));
        doubleSpinBox_18->setDecimals(4);
        doubleSpinBox_18->setMaximum(9999.99);

        horizontalLayout_8->addWidget(doubleSpinBox_18);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        pushButton_12 = new QPushButton(PlotSpec);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_12->sizePolicy().hasHeightForWidth());
        pushButton_12->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(pushButton_12);

        doubleSpinBox_15 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_15->setObjectName(QStringLiteral("doubleSpinBox_15"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(doubleSpinBox_15->sizePolicy().hasHeightForWidth());
        doubleSpinBox_15->setSizePolicy(sizePolicy2);
        doubleSpinBox_15->setMaximum(100000);

        horizontalLayout_7->addWidget(doubleSpinBox_15);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        pushButton_13 = new QPushButton(PlotSpec);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        sizePolicy1.setHeightForWidth(pushButton_13->sizePolicy().hasHeightForWidth());
        pushButton_13->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(pushButton_13);

        doubleSpinBox_16 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_16->setObjectName(QStringLiteral("doubleSpinBox_16"));
        sizePolicy2.setHeightForWidth(doubleSpinBox_16->sizePolicy().hasHeightForWidth());
        doubleSpinBox_16->setSizePolicy(sizePolicy2);
        doubleSpinBox_16->setDecimals(5);
        doubleSpinBox_16->setMaximum(999.99);

        horizontalLayout_6->addWidget(doubleSpinBox_16);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_15 = new QLabel(PlotSpec);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_17->addWidget(label_15);

        checkBox_16 = new QCheckBox(PlotSpec);
        checkBox_16->setObjectName(QStringLiteral("checkBox_16"));
        sizePolicy2.setHeightForWidth(checkBox_16->sizePolicy().hasHeightForWidth());
        checkBox_16->setSizePolicy(sizePolicy2);

        horizontalLayout_17->addWidget(checkBox_16);


        verticalLayout_2->addLayout(horizontalLayout_17);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        checkBox_14 = new QCheckBox(PlotSpec);
        checkBox_14->setObjectName(QStringLiteral("checkBox_14"));
        sizePolicy2.setHeightForWidth(checkBox_14->sizePolicy().hasHeightForWidth());
        checkBox_14->setSizePolicy(sizePolicy2);

        horizontalLayout_12->addWidget(checkBox_14);

        doubleSpinBox_19 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_19->setObjectName(QStringLiteral("doubleSpinBox_19"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_19->sizePolicy().hasHeightForWidth());
        doubleSpinBox_19->setSizePolicy(sizePolicy1);
        doubleSpinBox_19->setDecimals(4);
        doubleSpinBox_19->setMaximum(999.99);

        horizontalLayout_12->addWidget(doubleSpinBox_19);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        checkBox_13 = new QCheckBox(PlotSpec);
        checkBox_13->setObjectName(QStringLiteral("checkBox_13"));
        sizePolicy2.setHeightForWidth(checkBox_13->sizePolicy().hasHeightForWidth());
        checkBox_13->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(checkBox_13);

        pushButton_6 = new QPushButton(PlotSpec);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        sizePolicy1.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(pushButton_6);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        pushButton_8 = new QPushButton(PlotSpec);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        sizePolicy1.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(pushButton_8);

        pushButton_7 = new QPushButton(PlotSpec);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        sizePolicy1.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(pushButton_7);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_16 = new QLabel(PlotSpec);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_11->addWidget(label_16);

        lineEdit_8 = new QLineEdit(PlotSpec);
        lineEdit_8->setObjectName(QStringLiteral("lineEdit_8"));
        sizePolicy1.setHeightForWidth(lineEdit_8->sizePolicy().hasHeightForWidth());
        lineEdit_8->setSizePolicy(sizePolicy1);

        horizontalLayout_11->addWidget(lineEdit_8);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_17 = new QLabel(PlotSpec);
        label_17->setObjectName(QStringLiteral("label_17"));

        horizontalLayout_13->addWidget(label_17);

        lineEdit_9 = new QLineEdit(PlotSpec);
        lineEdit_9->setObjectName(QStringLiteral("lineEdit_9"));
        sizePolicy1.setHeightForWidth(lineEdit_9->sizePolicy().hasHeightForWidth());
        lineEdit_9->setSizePolicy(sizePolicy1);

        horizontalLayout_13->addWidget(lineEdit_9);


        verticalLayout_2->addLayout(horizontalLayout_13);

        pushButton_9 = new QPushButton(PlotSpec);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        sizePolicy1.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(pushButton_9);

        label_18 = new QLabel(PlotSpec);
        label_18->setObjectName(QStringLiteral("label_18"));

        verticalLayout_2->addWidget(label_18);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        pushButton_10 = new QPushButton(PlotSpec);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        sizePolicy1.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(pushButton_10);

        lineEdit_10 = new QLineEdit(PlotSpec);
        lineEdit_10->setObjectName(QStringLiteral("lineEdit_10"));
        sizePolicy1.setHeightForWidth(lineEdit_10->sizePolicy().hasHeightForWidth());
        lineEdit_10->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(lineEdit_10);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        pushButton_11 = new QPushButton(PlotSpec);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        sizePolicy1.setHeightForWidth(pushButton_11->sizePolicy().hasHeightForWidth());
        pushButton_11->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(pushButton_11);

        lineEdit_11 = new QLineEdit(PlotSpec);
        lineEdit_11->setObjectName(QStringLiteral("lineEdit_11"));
        sizePolicy1.setHeightForWidth(lineEdit_11->sizePolicy().hasHeightForWidth());
        lineEdit_11->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(lineEdit_11);


        verticalLayout_2->addLayout(horizontalLayout_15);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_9->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(PlotSpec);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        doubleSpinBox = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setDecimals(4);
        doubleSpinBox->setMinimum(-9999);
        doubleSpinBox->setMaximum(9999);
        doubleSpinBox->setSingleStep(0.1);

        horizontalLayout->addWidget(doubleSpinBox);

        label_3 = new QLabel(PlotSpec);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        doubleSpinBox_2 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(4);
        doubleSpinBox_2->setMinimum(-9999);
        doubleSpinBox_2->setMaximum(9999);
        doubleSpinBox_2->setSingleStep(0.1);

        horizontalLayout->addWidget(doubleSpinBox_2);

        label_5 = new QLabel(PlotSpec);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        doubleSpinBox_3 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setMinimum(-9999);
        doubleSpinBox_3->setMaximum(9999);
        doubleSpinBox_3->setValue(0);

        horizontalLayout->addWidget(doubleSpinBox_3);

        label_4 = new QLabel(PlotSpec);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        doubleSpinBox_4 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));
        doubleSpinBox_4->setMinimum(-9999);
        doubleSpinBox_4->setMaximum(9999);
        doubleSpinBox_4->setSingleStep(0.1);

        horizontalLayout->addWidget(doubleSpinBox_4);

        checkBox_4 = new QCheckBox(PlotSpec);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        horizontalLayout->addWidget(checkBox_4);

        lineEdit_6 = new QLineEdit(PlotSpec);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lineEdit_6->sizePolicy().hasHeightForWidth());
        lineEdit_6->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(lineEdit_6);

        pushButton_3 = new QPushButton(PlotSpec);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        sizePolicy1.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(PlotSpec);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_6 = new QLabel(PlotSpec);
        label_6->setObjectName(QStringLiteral("label_6"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_6);

        lineEdit_2 = new QLineEdit(PlotSpec);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        sizePolicy3.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(lineEdit_2);

        label_7 = new QLabel(PlotSpec);
        label_7->setObjectName(QStringLiteral("label_7"));
        sizePolicy4.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_7);

        lineEdit_3 = new QLineEdit(PlotSpec);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        sizePolicy3.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(lineEdit_3);

        label_9 = new QLabel(PlotSpec);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        lineEdit_5 = new QLineEdit(PlotSpec);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        sizePolicy3.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(lineEdit_5);

        pushButton_4 = new QPushButton(PlotSpec);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        sizePolicy1.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(pushButton_4);

        lineEdit_4 = new QLineEdit(PlotSpec);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        sizePolicy1.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(lineEdit_4);

        checkBox = new QCheckBox(PlotSpec);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout_3->addWidget(checkBox);

        checkBox_2 = new QCheckBox(PlotSpec);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        horizontalLayout_3->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(PlotSpec);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        horizontalLayout_3->addWidget(checkBox_3);

        label_11 = new QLabel(PlotSpec);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_3->addWidget(label_11);

        spinBox = new QSpinBox(PlotSpec);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        sizePolicy1.setHeightForWidth(spinBox->sizePolicy().hasHeightForWidth());
        spinBox->setSizePolicy(sizePolicy1);
        spinBox->setValue(9);

        horizontalLayout_3->addWidget(spinBox);

        label_13 = new QLabel(PlotSpec);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_3->addWidget(label_13);

        spinBox_2 = new QSpinBox(PlotSpec);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setMinimum(1);

        horizontalLayout_3->addWidget(spinBox_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(PlotSpec);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy4.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy4);

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(PlotSpec);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        sizePolicy3.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(lineEdit);

        checkBox_5 = new QCheckBox(PlotSpec);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));

        horizontalLayout_2->addWidget(checkBox_5);

        doubleSpinBox_5 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_5->setObjectName(QStringLiteral("doubleSpinBox_5"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_5->sizePolicy().hasHeightForWidth());
        doubleSpinBox_5->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(doubleSpinBox_5);

        checkBox_6 = new QCheckBox(PlotSpec);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));

        horizontalLayout_2->addWidget(checkBox_6);

        doubleSpinBox_6 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_6->setObjectName(QStringLiteral("doubleSpinBox_6"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_6->sizePolicy().hasHeightForWidth());
        doubleSpinBox_6->setSizePolicy(sizePolicy1);
        doubleSpinBox_6->setDecimals(3);
        doubleSpinBox_6->setMinimum(-99);
        doubleSpinBox_6->setSingleStep(0.1);

        horizontalLayout_2->addWidget(doubleSpinBox_6);

        checkBox_11 = new QCheckBox(PlotSpec);
        checkBox_11->setObjectName(QStringLiteral("checkBox_11"));

        horizontalLayout_2->addWidget(checkBox_11);

        doubleSpinBox_13 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_13->setObjectName(QStringLiteral("doubleSpinBox_13"));
        doubleSpinBox_13->setDecimals(4);
        doubleSpinBox_13->setMinimum(-9999);
        doubleSpinBox_13->setMaximum(9999.99);

        horizontalLayout_2->addWidget(doubleSpinBox_13);

        checkBox_9 = new QCheckBox(PlotSpec);
        checkBox_9->setObjectName(QStringLiteral("checkBox_9"));

        horizontalLayout_2->addWidget(checkBox_9);

        doubleSpinBox_9 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_9->setObjectName(QStringLiteral("doubleSpinBox_9"));
        doubleSpinBox_9->setDecimals(3);
        doubleSpinBox_9->setMinimum(-99);
        doubleSpinBox_9->setSingleStep(0.1);

        horizontalLayout_2->addWidget(doubleSpinBox_9);

        label_8 = new QLabel(PlotSpec);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_2->addWidget(label_8);

        doubleSpinBox_10 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_10->setObjectName(QStringLiteral("doubleSpinBox_10"));
        doubleSpinBox_10->setDecimals(4);
        doubleSpinBox_10->setMinimum(-99);
        doubleSpinBox_10->setSingleStep(0.001);

        horizontalLayout_2->addWidget(doubleSpinBox_10);

        pushButton_5 = new QPushButton(PlotSpec);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        horizontalLayout_2->addWidget(pushButton_5);

        horizontalSpacer_2 = new QSpacerItem(40, 15, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_10 = new QLabel(PlotSpec);
        label_10->setObjectName(QStringLiteral("label_10"));
        sizePolicy4.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy4);

        horizontalLayout_4->addWidget(label_10);

        lineEdit_7 = new QLineEdit(PlotSpec);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        sizePolicy3.setHeightForWidth(lineEdit_7->sizePolicy().hasHeightForWidth());
        lineEdit_7->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(lineEdit_7);

        checkBox_7 = new QCheckBox(PlotSpec);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));

        horizontalLayout_4->addWidget(checkBox_7);

        doubleSpinBox_7 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_7->setObjectName(QStringLiteral("doubleSpinBox_7"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_7->sizePolicy().hasHeightForWidth());
        doubleSpinBox_7->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(doubleSpinBox_7);

        checkBox_8 = new QCheckBox(PlotSpec);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));

        horizontalLayout_4->addWidget(checkBox_8);

        doubleSpinBox_8 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_8->setObjectName(QStringLiteral("doubleSpinBox_8"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_8->sizePolicy().hasHeightForWidth());
        doubleSpinBox_8->setSizePolicy(sizePolicy1);
        doubleSpinBox_8->setDecimals(3);
        doubleSpinBox_8->setMinimum(-99);
        doubleSpinBox_8->setSingleStep(0.1);

        horizontalLayout_4->addWidget(doubleSpinBox_8);

        checkBox_12 = new QCheckBox(PlotSpec);
        checkBox_12->setObjectName(QStringLiteral("checkBox_12"));

        horizontalLayout_4->addWidget(checkBox_12);

        doubleSpinBox_14 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_14->setObjectName(QStringLiteral("doubleSpinBox_14"));
        doubleSpinBox_14->setDecimals(4);
        doubleSpinBox_14->setMinimum(-9999);
        doubleSpinBox_14->setMaximum(9999.99);

        horizontalLayout_4->addWidget(doubleSpinBox_14);

        checkBox_10 = new QCheckBox(PlotSpec);
        checkBox_10->setObjectName(QStringLiteral("checkBox_10"));

        horizontalLayout_4->addWidget(checkBox_10);

        doubleSpinBox_11 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_11->setObjectName(QStringLiteral("doubleSpinBox_11"));
        doubleSpinBox_11->setDecimals(3);
        doubleSpinBox_11->setMinimum(-99);
        doubleSpinBox_11->setSingleStep(0.1);

        horizontalLayout_4->addWidget(doubleSpinBox_11);

        label_12 = new QLabel(PlotSpec);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_4->addWidget(label_12);

        doubleSpinBox_12 = new QDoubleSpinBox(PlotSpec);
        doubleSpinBox_12->setObjectName(QStringLiteral("doubleSpinBox_12"));
        doubleSpinBox_12->setDecimals(4);
        doubleSpinBox_12->setMinimum(-99);
        doubleSpinBox_12->setSingleStep(0.001);

        horizontalLayout_4->addWidget(doubleSpinBox_12);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        checkBox_15 = new QCheckBox(PlotSpec);
        checkBox_15->setObjectName(QStringLiteral("checkBox_15"));

        horizontalLayout_16->addWidget(checkBox_15);

        lineEdit_12 = new QLineEdit(PlotSpec);
        lineEdit_12->setObjectName(QStringLiteral("lineEdit_12"));
        sizePolicy1.setHeightForWidth(lineEdit_12->sizePolicy().hasHeightForWidth());
        lineEdit_12->setSizePolicy(sizePolicy1);

        horizontalLayout_16->addWidget(lineEdit_12);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_5);

        pushButton = new QPushButton(PlotSpec);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        horizontalLayout_16->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_16);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);


        retranslateUi(PlotSpec);
        QObject::connect(pushButton, SIGNAL(clicked()), PlotSpec, SLOT(close()));

        QMetaObject::connectSlotsByName(PlotSpec);
    } // setupUi

    void retranslateUi(QDialog *PlotSpec)
    {
        PlotSpec->setWindowTitle(QApplication::translate("PlotSpec", "Dialog", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("PlotSpec", "SNR", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("PlotSpec", "EW", Q_NULLPTR));
        label_15->setText(QApplication::translate("PlotSpec", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Normalization</span></p></body></html>", Q_NULLPTR));
        checkBox_16->setText(QApplication::translate("PlotSpec", "Yes", Q_NULLPTR));
        checkBox_14->setText(QApplication::translate("PlotSpec", "correct to ratio:", Q_NULLPTR));
        checkBox_13->setText(QApplication::translate("PlotSpec", "divide", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("PlotSpec", "Apply", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("PlotSpec", "Show", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("PlotSpec", "Reset", Q_NULLPTR));
        label_16->setText(QApplication::translate("PlotSpec", "File 1:", Q_NULLPTR));
        label_17->setText(QApplication::translate("PlotSpec", "File 2:", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("PlotSpec", "Save:", Q_NULLPTR));
        label_18->setText(QApplication::translate("PlotSpec", "Spline Data", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("PlotSpec", "Save", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("PlotSpec", "Load", Q_NULLPTR));
        label_2->setText(QApplication::translate("PlotSpec", "x1:", Q_NULLPTR));
        label_3->setText(QApplication::translate("PlotSpec", "x2:", Q_NULLPTR));
        label_5->setText(QApplication::translate("PlotSpec", "y1:", Q_NULLPTR));
        label_4->setText(QApplication::translate("PlotSpec", "y2:", Q_NULLPTR));
        checkBox_4->setText(QApplication::translate("PlotSpec", "legend", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("PlotSpec", "find values", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("PlotSpec", "Plot", Q_NULLPTR));
        label_6->setText(QApplication::translate("PlotSpec", "x Label", Q_NULLPTR));
        label_7->setText(QApplication::translate("PlotSpec", "y Label", Q_NULLPTR));
        label_9->setText(QApplication::translate("PlotSpec", "work path", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("PlotSpec", "Save as:", Q_NULLPTR));
        checkBox->setText(QApplication::translate("PlotSpec", "pdf", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("PlotSpec", "png", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("PlotSpec", "jpg", Q_NULLPTR));
        label_11->setText(QApplication::translate("PlotSpec", "Font Size:", Q_NULLPTR));
        label_13->setText(QApplication::translate("PlotSpec", "Line Width", Q_NULLPTR));
        label->setText(QApplication::translate("PlotSpec", "<html><head/><body><p><span style=\" font-weight:600; color:#0c3ff2;\">File 1</span></p></body></html>", Q_NULLPTR));
        checkBox_5->setText(QApplication::translate("PlotSpec", "y*", Q_NULLPTR));
        checkBox_6->setText(QApplication::translate("PlotSpec", "y+", Q_NULLPTR));
        checkBox_11->setText(QApplication::translate("PlotSpec", "x+", Q_NULLPTR));
        checkBox_9->setText(QApplication::translate("PlotSpec", "f(x)=", Q_NULLPTR));
        label_8->setText(QApplication::translate("PlotSpec", "+(x-x0)*", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("PlotSpec", "Add 1-f", Q_NULLPTR));
        label_10->setText(QApplication::translate("PlotSpec", "<html><head/><body><p><span style=\" font-weight:600; color:#e30b0b;\">File 2</span></p></body></html>", Q_NULLPTR));
        checkBox_7->setText(QApplication::translate("PlotSpec", "y*", Q_NULLPTR));
        checkBox_8->setText(QApplication::translate("PlotSpec", "y+", Q_NULLPTR));
        checkBox_12->setText(QApplication::translate("PlotSpec", "x+", Q_NULLPTR));
        checkBox_10->setText(QApplication::translate("PlotSpec", "f(x)=", Q_NULLPTR));
        label_12->setText(QApplication::translate("PlotSpec", "+(x-x0)*", Q_NULLPTR));
        checkBox_15->setText(QApplication::translate("PlotSpec", "Error", Q_NULLPTR));
        pushButton->setText(QApplication::translate("PlotSpec", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PlotSpec: public Ui_PlotSpec {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTSPEC_H
