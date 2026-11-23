/********************************************************************************
** Form generated from reading UI file 'plotspec.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTSPEC_H
#define UI_PLOTSPEC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_PlotSpec
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_2;
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
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_14;
    QDoubleSpinBox *doubleSpinBox_20;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *checkBox_13;
    QCheckBox *checkBox_17;
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
    QCheckBox *checkBox_18;
    QCheckBox *checkBox_19;
    QPushButton *pushButton_5;
    QLineEdit *lineEdit_13;
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
    QCheckBox *checkBox_20;
    QCheckBox *checkBox_21;
    QLineEdit *lineEdit_14;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_16;
    QCheckBox *checkBox_15;
    QLineEdit *lineEdit_12;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton;

    void setupUi(QDialog *PlotSpec)
    {
        if (PlotSpec->objectName().isEmpty())
            PlotSpec->setObjectName("PlotSpec");
        PlotSpec->resize(1153, 632);
        PlotSpec->setMinimumSize(QSize(200, 50));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(205, 205, 205, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Light, brush2);
        QBrush brush3(QColor(230, 230, 230, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Midlight, brush3);
        QBrush brush4(QColor(102, 102, 102, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush4);
        QBrush brush5(QColor(137, 137, 137, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush2);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush3);
        QBrush brush6(QColor(255, 255, 220, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush7(QColor(0, 0, 0, 128));
        brush7.setStyle(Qt::BrushStyle::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::PlaceholderText, brush7);
#endif
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Text, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ButtonText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush8(QColor(0, 0, 0, 128));
        brush8.setStyle(Qt::BrushStyle::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::PlaceholderText, brush8);
#endif
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Light, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Midlight, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::BrightText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush4);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Shadow, brush);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipText, brush);
        QBrush brush9(QColor(0, 0, 0, 128));
        brush9.setStyle(Qt::BrushStyle::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::PlaceholderText, brush9);
#endif
        PlotSpec->setPalette(palette);
        QFont font;
        font.setPointSize(9);
        PlotSpec->setFont(font);
        gridLayout = new QGridLayout(PlotSpec);
        gridLayout->setObjectName("gridLayout");
        scrollArea = new QScrollArea(PlotSpec);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1133, 612));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        customPlot = new QCustomPlot(scrollAreaWidgetContents);
        customPlot->setObjectName("customPlot");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy);
        customPlot->setMinimumSize(QSize(200, 0));

        horizontalLayout_9->addWidget(customPlot);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        doubleSpinBox_17 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_17->setObjectName("doubleSpinBox_17");
        doubleSpinBox_17->setDecimals(4);
        doubleSpinBox_17->setMaximum(9999.989999999999782);

        horizontalLayout_8->addWidget(doubleSpinBox_17);

        doubleSpinBox_18 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_18->setObjectName("doubleSpinBox_18");
        doubleSpinBox_18->setDecimals(4);
        doubleSpinBox_18->setMaximum(9999.989999999999782);

        horizontalLayout_8->addWidget(doubleSpinBox_18);


        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        pushButton_12 = new QPushButton(scrollAreaWidgetContents);
        pushButton_12->setObjectName("pushButton_12");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_12->sizePolicy().hasHeightForWidth());
        pushButton_12->setSizePolicy(sizePolicy1);

        horizontalLayout_7->addWidget(pushButton_12);

        doubleSpinBox_15 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_15->setObjectName("doubleSpinBox_15");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(doubleSpinBox_15->sizePolicy().hasHeightForWidth());
        doubleSpinBox_15->setSizePolicy(sizePolicy2);
        doubleSpinBox_15->setMaximum(99999.990000000005239);

        horizontalLayout_7->addWidget(doubleSpinBox_15);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        pushButton_13 = new QPushButton(scrollAreaWidgetContents);
        pushButton_13->setObjectName("pushButton_13");
        sizePolicy1.setHeightForWidth(pushButton_13->sizePolicy().hasHeightForWidth());
        pushButton_13->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(pushButton_13);

        doubleSpinBox_16 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_16->setObjectName("doubleSpinBox_16");
        sizePolicy2.setHeightForWidth(doubleSpinBox_16->sizePolicy().hasHeightForWidth());
        doubleSpinBox_16->setSizePolicy(sizePolicy2);
        doubleSpinBox_16->setDecimals(5);
        doubleSpinBox_16->setMaximum(999.990000000000009);

        horizontalLayout_6->addWidget(doubleSpinBox_16);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName("horizontalLayout_17");
        label_15 = new QLabel(scrollAreaWidgetContents);
        label_15->setObjectName("label_15");

        horizontalLayout_17->addWidget(label_15);

        checkBox_16 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_16->setObjectName("checkBox_16");
        sizePolicy2.setHeightForWidth(checkBox_16->sizePolicy().hasHeightForWidth());
        checkBox_16->setSizePolicy(sizePolicy2);

        horizontalLayout_17->addWidget(checkBox_16);


        verticalLayout_2->addLayout(horizontalLayout_17);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName("horizontalLayout_12");
        checkBox_14 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_14->setObjectName("checkBox_14");
        sizePolicy2.setHeightForWidth(checkBox_14->sizePolicy().hasHeightForWidth());
        checkBox_14->setSizePolicy(sizePolicy2);

        horizontalLayout_12->addWidget(checkBox_14);

        doubleSpinBox_19 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_19->setObjectName("doubleSpinBox_19");
        sizePolicy1.setHeightForWidth(doubleSpinBox_19->sizePolicy().hasHeightForWidth());
        doubleSpinBox_19->setSizePolicy(sizePolicy1);
        doubleSpinBox_19->setDecimals(4);
        doubleSpinBox_19->setMaximum(999.990000000000009);

        horizontalLayout_12->addWidget(doubleSpinBox_19);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName("horizontalLayout_18");
        label_14 = new QLabel(scrollAreaWidgetContents);
        label_14->setObjectName("label_14");

        horizontalLayout_18->addWidget(label_14);

        doubleSpinBox_20 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_20->setObjectName("doubleSpinBox_20");
        sizePolicy1.setHeightForWidth(doubleSpinBox_20->sizePolicy().hasHeightForWidth());
        doubleSpinBox_20->setSizePolicy(sizePolicy1);
        doubleSpinBox_20->setDecimals(3);
        doubleSpinBox_20->setSingleStep(0.100000000000000);

        horizontalLayout_18->addWidget(doubleSpinBox_20);


        verticalLayout_2->addLayout(horizontalLayout_18);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        checkBox_13 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_13->setObjectName("checkBox_13");
        sizePolicy2.setHeightForWidth(checkBox_13->sizePolicy().hasHeightForWidth());
        checkBox_13->setSizePolicy(sizePolicy2);

        horizontalLayout_5->addWidget(checkBox_13);

        checkBox_17 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_17->setObjectName("checkBox_17");

        horizontalLayout_5->addWidget(checkBox_17);

        pushButton_6 = new QPushButton(scrollAreaWidgetContents);
        pushButton_6->setObjectName("pushButton_6");
        sizePolicy1.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(pushButton_6);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        pushButton_8 = new QPushButton(scrollAreaWidgetContents);
        pushButton_8->setObjectName("pushButton_8");
        sizePolicy1.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(pushButton_8);

        pushButton_7 = new QPushButton(scrollAreaWidgetContents);
        pushButton_7->setObjectName("pushButton_7");
        sizePolicy1.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy1);

        horizontalLayout_10->addWidget(pushButton_7);


        verticalLayout_2->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName("horizontalLayout_11");
        label_16 = new QLabel(scrollAreaWidgetContents);
        label_16->setObjectName("label_16");

        horizontalLayout_11->addWidget(label_16);

        lineEdit_8 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_8->setObjectName("lineEdit_8");
        sizePolicy1.setHeightForWidth(lineEdit_8->sizePolicy().hasHeightForWidth());
        lineEdit_8->setSizePolicy(sizePolicy1);

        horizontalLayout_11->addWidget(lineEdit_8);


        verticalLayout_2->addLayout(horizontalLayout_11);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName("horizontalLayout_13");
        label_17 = new QLabel(scrollAreaWidgetContents);
        label_17->setObjectName("label_17");

        horizontalLayout_13->addWidget(label_17);

        lineEdit_9 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_9->setObjectName("lineEdit_9");
        sizePolicy1.setHeightForWidth(lineEdit_9->sizePolicy().hasHeightForWidth());
        lineEdit_9->setSizePolicy(sizePolicy1);

        horizontalLayout_13->addWidget(lineEdit_9);


        verticalLayout_2->addLayout(horizontalLayout_13);

        pushButton_9 = new QPushButton(scrollAreaWidgetContents);
        pushButton_9->setObjectName("pushButton_9");
        sizePolicy1.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(pushButton_9);

        label_18 = new QLabel(scrollAreaWidgetContents);
        label_18->setObjectName("label_18");

        verticalLayout_2->addWidget(label_18);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName("horizontalLayout_14");
        pushButton_10 = new QPushButton(scrollAreaWidgetContents);
        pushButton_10->setObjectName("pushButton_10");
        sizePolicy1.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(pushButton_10);

        lineEdit_10 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_10->setObjectName("lineEdit_10");
        sizePolicy1.setHeightForWidth(lineEdit_10->sizePolicy().hasHeightForWidth());
        lineEdit_10->setSizePolicy(sizePolicy1);

        horizontalLayout_14->addWidget(lineEdit_10);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName("horizontalLayout_15");
        pushButton_11 = new QPushButton(scrollAreaWidgetContents);
        pushButton_11->setObjectName("pushButton_11");
        sizePolicy1.setHeightForWidth(pushButton_11->sizePolicy().hasHeightForWidth());
        pushButton_11->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(pushButton_11);

        lineEdit_11 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_11->setObjectName("lineEdit_11");
        sizePolicy1.setHeightForWidth(lineEdit_11->sizePolicy().hasHeightForWidth());
        lineEdit_11->setSizePolicy(sizePolicy1);

        horizontalLayout_15->addWidget(lineEdit_11);


        verticalLayout_2->addLayout(horizontalLayout_15);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_9->addLayout(verticalLayout_2);


        gridLayout_2->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_2 = new QLabel(scrollAreaWidgetContents);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        doubleSpinBox = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox->setObjectName("doubleSpinBox");
        doubleSpinBox->setDecimals(4);
        doubleSpinBox->setMinimum(-9999.000000000000000);
        doubleSpinBox->setMaximum(9999.000000000000000);
        doubleSpinBox->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox);

        label_3 = new QLabel(scrollAreaWidgetContents);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        doubleSpinBox_2 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_2->setObjectName("doubleSpinBox_2");
        doubleSpinBox_2->setDecimals(4);
        doubleSpinBox_2->setMinimum(-9999.000000000000000);
        doubleSpinBox_2->setMaximum(9999.000000000000000);
        doubleSpinBox_2->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox_2);

        label_5 = new QLabel(scrollAreaWidgetContents);
        label_5->setObjectName("label_5");

        horizontalLayout->addWidget(label_5);

        doubleSpinBox_3 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_3->setObjectName("doubleSpinBox_3");
        doubleSpinBox_3->setMinimum(-9999.000000000000000);
        doubleSpinBox_3->setMaximum(9999.000000000000000);
        doubleSpinBox_3->setValue(0.000000000000000);

        horizontalLayout->addWidget(doubleSpinBox_3);

        label_4 = new QLabel(scrollAreaWidgetContents);
        label_4->setObjectName("label_4");

        horizontalLayout->addWidget(label_4);

        doubleSpinBox_4 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_4->setObjectName("doubleSpinBox_4");
        doubleSpinBox_4->setMinimum(-9999.000000000000000);
        doubleSpinBox_4->setMaximum(99999.000000000000000);
        doubleSpinBox_4->setSingleStep(0.100000000000000);

        horizontalLayout->addWidget(doubleSpinBox_4);

        checkBox_4 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_4->setObjectName("checkBox_4");

        horizontalLayout->addWidget(checkBox_4);

        lineEdit_6 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_6->setObjectName("lineEdit_6");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lineEdit_6->sizePolicy().hasHeightForWidth());
        lineEdit_6->setSizePolicy(sizePolicy3);

        horizontalLayout->addWidget(lineEdit_6);

        pushButton_3 = new QPushButton(scrollAreaWidgetContents);
        pushButton_3->setObjectName("pushButton_3");
        sizePolicy1.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(scrollAreaWidgetContents);
        pushButton_2->setObjectName("pushButton_2");
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_6 = new QLabel(scrollAreaWidgetContents);
        label_6->setObjectName("label_6");
        QSizePolicy sizePolicy4(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_6);

        lineEdit_2 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_2->setObjectName("lineEdit_2");
        sizePolicy3.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(lineEdit_2);

        label_7 = new QLabel(scrollAreaWidgetContents);
        label_7->setObjectName("label_7");
        sizePolicy4.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy4);

        horizontalLayout_3->addWidget(label_7);

        lineEdit_3 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_3->setObjectName("lineEdit_3");
        sizePolicy3.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(lineEdit_3);

        label_9 = new QLabel(scrollAreaWidgetContents);
        label_9->setObjectName("label_9");

        horizontalLayout_3->addWidget(label_9);

        lineEdit_5 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_5->setObjectName("lineEdit_5");
        sizePolicy3.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(lineEdit_5);

        pushButton_4 = new QPushButton(scrollAreaWidgetContents);
        pushButton_4->setObjectName("pushButton_4");
        sizePolicy1.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(pushButton_4);

        lineEdit_4 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_4->setObjectName("lineEdit_4");
        sizePolicy1.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(lineEdit_4);

        checkBox = new QCheckBox(scrollAreaWidgetContents);
        checkBox->setObjectName("checkBox");

        horizontalLayout_3->addWidget(checkBox);

        checkBox_2 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_2->setObjectName("checkBox_2");

        horizontalLayout_3->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_3->setObjectName("checkBox_3");

        horizontalLayout_3->addWidget(checkBox_3);

        label_11 = new QLabel(scrollAreaWidgetContents);
        label_11->setObjectName("label_11");

        horizontalLayout_3->addWidget(label_11);

        spinBox = new QSpinBox(scrollAreaWidgetContents);
        spinBox->setObjectName("spinBox");
        sizePolicy1.setHeightForWidth(spinBox->sizePolicy().hasHeightForWidth());
        spinBox->setSizePolicy(sizePolicy1);
        spinBox->setValue(9);

        horizontalLayout_3->addWidget(spinBox);

        label_13 = new QLabel(scrollAreaWidgetContents);
        label_13->setObjectName("label_13");

        horizontalLayout_3->addWidget(label_13);

        spinBox_2 = new QSpinBox(scrollAreaWidgetContents);
        spinBox_2->setObjectName("spinBox_2");
        spinBox_2->setMinimum(1);

        horizontalLayout_3->addWidget(spinBox_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label = new QLabel(scrollAreaWidgetContents);
        label->setObjectName("label");
        sizePolicy4.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy4);

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(scrollAreaWidgetContents);
        lineEdit->setObjectName("lineEdit");
        sizePolicy3.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy3);

        horizontalLayout_2->addWidget(lineEdit);

        checkBox_5 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_5->setObjectName("checkBox_5");

        horizontalLayout_2->addWidget(checkBox_5);

        doubleSpinBox_5 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_5->setObjectName("doubleSpinBox_5");
        sizePolicy1.setHeightForWidth(doubleSpinBox_5->sizePolicy().hasHeightForWidth());
        doubleSpinBox_5->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(doubleSpinBox_5);

        checkBox_6 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_6->setObjectName("checkBox_6");

        horizontalLayout_2->addWidget(checkBox_6);

        doubleSpinBox_6 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_6->setObjectName("doubleSpinBox_6");
        sizePolicy1.setHeightForWidth(doubleSpinBox_6->sizePolicy().hasHeightForWidth());
        doubleSpinBox_6->setSizePolicy(sizePolicy1);
        doubleSpinBox_6->setDecimals(3);
        doubleSpinBox_6->setMinimum(-99.000000000000000);
        doubleSpinBox_6->setSingleStep(0.100000000000000);

        horizontalLayout_2->addWidget(doubleSpinBox_6);

        checkBox_11 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_11->setObjectName("checkBox_11");

        horizontalLayout_2->addWidget(checkBox_11);

        doubleSpinBox_13 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_13->setObjectName("doubleSpinBox_13");
        doubleSpinBox_13->setDecimals(4);
        doubleSpinBox_13->setMinimum(-9999.000000000000000);
        doubleSpinBox_13->setMaximum(9999.989999999999782);

        horizontalLayout_2->addWidget(doubleSpinBox_13);

        checkBox_9 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_9->setObjectName("checkBox_9");

        horizontalLayout_2->addWidget(checkBox_9);

        doubleSpinBox_9 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_9->setObjectName("doubleSpinBox_9");
        doubleSpinBox_9->setDecimals(4);
        doubleSpinBox_9->setMinimum(-99.000000000000000);
        doubleSpinBox_9->setSingleStep(0.100000000000000);

        horizontalLayout_2->addWidget(doubleSpinBox_9);

        label_8 = new QLabel(scrollAreaWidgetContents);
        label_8->setObjectName("label_8");

        horizontalLayout_2->addWidget(label_8);

        doubleSpinBox_10 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_10->setObjectName("doubleSpinBox_10");
        doubleSpinBox_10->setDecimals(5);
        doubleSpinBox_10->setMinimum(-99.000000000000000);
        doubleSpinBox_10->setSingleStep(0.001000000000000);

        horizontalLayout_2->addWidget(doubleSpinBox_10);

        checkBox_18 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_18->setObjectName("checkBox_18");

        horizontalLayout_2->addWidget(checkBox_18);

        checkBox_19 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_19->setObjectName("checkBox_19");

        horizontalLayout_2->addWidget(checkBox_19);

        pushButton_5 = new QPushButton(scrollAreaWidgetContents);
        pushButton_5->setObjectName("pushButton_5");

        horizontalLayout_2->addWidget(pushButton_5);

        lineEdit_13 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_13->setObjectName("lineEdit_13");
        sizePolicy1.setHeightForWidth(lineEdit_13->sizePolicy().hasHeightForWidth());
        lineEdit_13->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(lineEdit_13);

        horizontalSpacer_2 = new QSpacerItem(40, 15, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_10 = new QLabel(scrollAreaWidgetContents);
        label_10->setObjectName("label_10");
        sizePolicy4.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy4);

        horizontalLayout_4->addWidget(label_10);

        lineEdit_7 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_7->setObjectName("lineEdit_7");
        sizePolicy3.setHeightForWidth(lineEdit_7->sizePolicy().hasHeightForWidth());
        lineEdit_7->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(lineEdit_7);

        checkBox_7 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_7->setObjectName("checkBox_7");

        horizontalLayout_4->addWidget(checkBox_7);

        doubleSpinBox_7 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_7->setObjectName("doubleSpinBox_7");
        sizePolicy1.setHeightForWidth(doubleSpinBox_7->sizePolicy().hasHeightForWidth());
        doubleSpinBox_7->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(doubleSpinBox_7);

        checkBox_8 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_8->setObjectName("checkBox_8");

        horizontalLayout_4->addWidget(checkBox_8);

        doubleSpinBox_8 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_8->setObjectName("doubleSpinBox_8");
        sizePolicy1.setHeightForWidth(doubleSpinBox_8->sizePolicy().hasHeightForWidth());
        doubleSpinBox_8->setSizePolicy(sizePolicy1);
        doubleSpinBox_8->setDecimals(3);
        doubleSpinBox_8->setMinimum(-99.000000000000000);
        doubleSpinBox_8->setSingleStep(0.100000000000000);

        horizontalLayout_4->addWidget(doubleSpinBox_8);

        checkBox_12 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_12->setObjectName("checkBox_12");

        horizontalLayout_4->addWidget(checkBox_12);

        doubleSpinBox_14 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_14->setObjectName("doubleSpinBox_14");
        doubleSpinBox_14->setDecimals(4);
        doubleSpinBox_14->setMinimum(-9999.000000000000000);
        doubleSpinBox_14->setMaximum(9999.989999999999782);

        horizontalLayout_4->addWidget(doubleSpinBox_14);

        checkBox_10 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_10->setObjectName("checkBox_10");

        horizontalLayout_4->addWidget(checkBox_10);

        doubleSpinBox_11 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_11->setObjectName("doubleSpinBox_11");
        doubleSpinBox_11->setDecimals(4);
        doubleSpinBox_11->setMinimum(-99.000000000000000);
        doubleSpinBox_11->setSingleStep(0.100000000000000);

        horizontalLayout_4->addWidget(doubleSpinBox_11);

        label_12 = new QLabel(scrollAreaWidgetContents);
        label_12->setObjectName("label_12");

        horizontalLayout_4->addWidget(label_12);

        doubleSpinBox_12 = new QDoubleSpinBox(scrollAreaWidgetContents);
        doubleSpinBox_12->setObjectName("doubleSpinBox_12");
        doubleSpinBox_12->setDecimals(5);
        doubleSpinBox_12->setMinimum(-99.000000000000000);
        doubleSpinBox_12->setSingleStep(0.001000000000000);

        horizontalLayout_4->addWidget(doubleSpinBox_12);

        checkBox_20 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_20->setObjectName("checkBox_20");

        horizontalLayout_4->addWidget(checkBox_20);

        checkBox_21 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_21->setObjectName("checkBox_21");

        horizontalLayout_4->addWidget(checkBox_21);

        lineEdit_14 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_14->setObjectName("lineEdit_14");
        sizePolicy1.setHeightForWidth(lineEdit_14->sizePolicy().hasHeightForWidth());
        lineEdit_14->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(lineEdit_14);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName("horizontalLayout_16");
        checkBox_15 = new QCheckBox(scrollAreaWidgetContents);
        checkBox_15->setObjectName("checkBox_15");

        horizontalLayout_16->addWidget(checkBox_15);

        lineEdit_12 = new QLineEdit(scrollAreaWidgetContents);
        lineEdit_12->setObjectName("lineEdit_12");
        sizePolicy1.setHeightForWidth(lineEdit_12->sizePolicy().hasHeightForWidth());
        lineEdit_12->setSizePolicy(sizePolicy1);

        horizontalLayout_16->addWidget(lineEdit_12);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_5);

        pushButton = new QPushButton(scrollAreaWidgetContents);
        pushButton->setObjectName("pushButton");
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);

        horizontalLayout_16->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_16);


        gridLayout_2->addLayout(verticalLayout, 1, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(PlotSpec);
        QObject::connect(pushButton, &QPushButton::clicked, PlotSpec, qOverload<>(&QDialog::close));

        QMetaObject::connectSlotsByName(PlotSpec);
    } // setupUi

    void retranslateUi(QDialog *PlotSpec)
    {
        PlotSpec->setWindowTitle(QCoreApplication::translate("PlotSpec", "Dialog", nullptr));
        pushButton_12->setText(QCoreApplication::translate("PlotSpec", "SNR", nullptr));
        pushButton_13->setText(QCoreApplication::translate("PlotSpec", "EW", nullptr));
        label_15->setText(QCoreApplication::translate("PlotSpec", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Normalization</span></p></body></html>", nullptr));
        checkBox_16->setText(QCoreApplication::translate("PlotSpec", "Yes", nullptr));
        checkBox_14->setText(QCoreApplication::translate("PlotSpec", "Correct to Ratio:", nullptr));
        label_14->setText(QCoreApplication::translate("PlotSpec", "To Continuum:", nullptr));
        checkBox_13->setText(QCoreApplication::translate("PlotSpec", "Divide", nullptr));
        checkBox_17->setText(QCoreApplication::translate("PlotSpec", "Both", nullptr));
        pushButton_6->setText(QCoreApplication::translate("PlotSpec", "Apply", nullptr));
        pushButton_8->setText(QCoreApplication::translate("PlotSpec", "Show", nullptr));
        pushButton_7->setText(QCoreApplication::translate("PlotSpec", "Reset", nullptr));
        label_16->setText(QCoreApplication::translate("PlotSpec", "File 1:", nullptr));
        label_17->setText(QCoreApplication::translate("PlotSpec", "File 2:", nullptr));
        pushButton_9->setText(QCoreApplication::translate("PlotSpec", "Save:", nullptr));
        label_18->setText(QCoreApplication::translate("PlotSpec", "Spline Data", nullptr));
        pushButton_10->setText(QCoreApplication::translate("PlotSpec", "Save", nullptr));
        pushButton_11->setText(QCoreApplication::translate("PlotSpec", "Load", nullptr));
        label_2->setText(QCoreApplication::translate("PlotSpec", "x1:", nullptr));
        label_3->setText(QCoreApplication::translate("PlotSpec", "x2:", nullptr));
        label_5->setText(QCoreApplication::translate("PlotSpec", "y1:", nullptr));
        label_4->setText(QCoreApplication::translate("PlotSpec", "y2:", nullptr));
        checkBox_4->setText(QCoreApplication::translate("PlotSpec", "legend", nullptr));
        pushButton_3->setText(QCoreApplication::translate("PlotSpec", "find values", nullptr));
        pushButton_2->setText(QCoreApplication::translate("PlotSpec", "Plot", nullptr));
        label_6->setText(QCoreApplication::translate("PlotSpec", "x Label", nullptr));
        label_7->setText(QCoreApplication::translate("PlotSpec", "y Label", nullptr));
        label_9->setText(QCoreApplication::translate("PlotSpec", "work path", nullptr));
        pushButton_4->setText(QCoreApplication::translate("PlotSpec", "Save as:", nullptr));
        checkBox->setText(QCoreApplication::translate("PlotSpec", "pdf", nullptr));
        checkBox_2->setText(QCoreApplication::translate("PlotSpec", "png", nullptr));
        checkBox_3->setText(QCoreApplication::translate("PlotSpec", "jpg", nullptr));
        label_11->setText(QCoreApplication::translate("PlotSpec", "Font Size:", nullptr));
        label_13->setText(QCoreApplication::translate("PlotSpec", "Line Width", nullptr));
        label->setText(QCoreApplication::translate("PlotSpec", "<html><head/><body><p><span style=\" font-weight:600; color:#0c3ff2;\">File 1</span></p></body></html>", nullptr));
        checkBox_5->setText(QCoreApplication::translate("PlotSpec", "y*", nullptr));
        checkBox_6->setText(QCoreApplication::translate("PlotSpec", "y+", nullptr));
        checkBox_11->setText(QCoreApplication::translate("PlotSpec", "x+", nullptr));
        checkBox_9->setText(QCoreApplication::translate("PlotSpec", "f(x)=", nullptr));
        label_8->setText(QCoreApplication::translate("PlotSpec", "+(x-x0)*", nullptr));
        checkBox_18->setText(QCoreApplication::translate("PlotSpec", "-(f(x)-1)", nullptr));
        checkBox_19->setText(QCoreApplication::translate("PlotSpec", "+(f(x)-1)", nullptr));
        pushButton_5->setText(QCoreApplication::translate("PlotSpec", "Compensate", nullptr));
        label_10->setText(QCoreApplication::translate("PlotSpec", "<html><head/><body><p><span style=\" font-weight:600; color:#e30b0b;\">File 2</span></p></body></html>", nullptr));
        checkBox_7->setText(QCoreApplication::translate("PlotSpec", "y*", nullptr));
        checkBox_8->setText(QCoreApplication::translate("PlotSpec", "y+", nullptr));
        checkBox_12->setText(QCoreApplication::translate("PlotSpec", "x+", nullptr));
        checkBox_10->setText(QCoreApplication::translate("PlotSpec", "f(x)=", nullptr));
        label_12->setText(QCoreApplication::translate("PlotSpec", "+(x-x0)*", nullptr));
        checkBox_20->setText(QCoreApplication::translate("PlotSpec", "-(f(x)-1)", nullptr));
        checkBox_21->setText(QCoreApplication::translate("PlotSpec", "+(f(x)-1)", nullptr));
        checkBox_15->setText(QCoreApplication::translate("PlotSpec", "Error", nullptr));
        pushButton->setText(QCoreApplication::translate("PlotSpec", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlotSpec: public Ui_PlotSpec {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTSPEC_H
