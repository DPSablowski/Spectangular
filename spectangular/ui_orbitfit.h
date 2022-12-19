/********************************************************************************
** Form generated from reading UI file 'orbitfit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
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

QT_BEGIN_NAMESPACE

class Ui_OrbitFit
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_17;
    QSpinBox *spinBox;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_16;
    QLineEdit *lineEdit_2;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_19;
    QLineEdit *lineEdit_17;
    QCheckBox *checkBox_3;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_18;
    QLineEdit *lineEdit_18;
    QCheckBox *checkBox_4;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_20;
    QLineEdit *lineEdit_19;
    QCheckBox *checkBox_5;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_21;
    QLineEdit *lineEdit_20;
    QCheckBox *checkBox_6;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_22;
    QLineEdit *lineEdit_21;
    QCheckBox *checkBox_7;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_23;
    QLineEdit *lineEdit_22;
    QCheckBox *checkBox_8;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_24;
    QLineEdit *lineEdit_23;
    QCheckBox *checkBox_9;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit_3;
    QLabel *label_9;
    QLineEdit *lineEdit_10;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_4;
    QLabel *label_10;
    QLineEdit *lineEdit_11;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLineEdit *lineEdit_5;
    QLabel *label_11;
    QLineEdit *lineEdit_12;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QLineEdit *lineEdit_6;
    QLabel *label_12;
    QLineEdit *lineEdit_13;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QLineEdit *lineEdit_7;
    QLabel *label_13;
    QLineEdit *lineEdit_14;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QLineEdit *lineEdit_8;
    QLabel *label_14;
    QLineEdit *lineEdit_15;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QLineEdit *lineEdit_9;
    QLabel *label_15;
    QLineEdit *lineEdit_16;

    void setupUi(QDialog *OrbitFit)
    {
        if (OrbitFit->objectName().isEmpty())
            OrbitFit->setObjectName(QString::fromUtf8("OrbitFit"));
        OrbitFit->resize(805, 287);
        gridLayout = new QGridLayout(OrbitFit);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(OrbitFit);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit = new QLineEdit(OrbitFit);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(lineEdit);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_17 = new QLabel(OrbitFit);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_11->addWidget(label_17);

        spinBox = new QSpinBox(OrbitFit);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(1);
        spinBox->setMaximum(9999);
        spinBox->setValue(50);

        horizontalLayout_11->addWidget(spinBox);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        checkBox = new QCheckBox(OrbitFit);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        horizontalLayout_2->addWidget(checkBox);

        checkBox_2 = new QCheckBox(OrbitFit);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        horizontalLayout_2->addWidget(checkBox_2);

        pushButton_2 = new QPushButton(OrbitFit);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_16 = new QLabel(OrbitFit);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_10->addWidget(label_16);

        lineEdit_2 = new QLineEdit(OrbitFit);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        sizePolicy.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy);

        horizontalLayout_10->addWidget(lineEdit_2);


        verticalLayout_3->addLayout(horizontalLayout_10);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        pushButton = new QPushButton(OrbitFit);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(pushButton);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(OrbitFit);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_19 = new QLabel(groupBox_2);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_19->sizePolicy().hasHeightForWidth());
        label_19->setSizePolicy(sizePolicy1);
        label_19->setMinimumSize(QSize(30, 0));
        label_19->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_12->addWidget(label_19);

        lineEdit_17 = new QLineEdit(groupBox_2);
        lineEdit_17->setObjectName(QString::fromUtf8("lineEdit_17"));
        sizePolicy.setHeightForWidth(lineEdit_17->sizePolicy().hasHeightForWidth());
        lineEdit_17->setSizePolicy(sizePolicy);
        lineEdit_17->setMinimumSize(QSize(120, 0));
        lineEdit_17->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_12->addWidget(lineEdit_17);

        checkBox_3 = new QCheckBox(groupBox_2);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        horizontalLayout_12->addWidget(checkBox_3);


        verticalLayout_2->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        sizePolicy1.setHeightForWidth(label_18->sizePolicy().hasHeightForWidth());
        label_18->setSizePolicy(sizePolicy1);
        label_18->setMinimumSize(QSize(30, 0));
        label_18->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_13->addWidget(label_18);

        lineEdit_18 = new QLineEdit(groupBox_2);
        lineEdit_18->setObjectName(QString::fromUtf8("lineEdit_18"));
        sizePolicy.setHeightForWidth(lineEdit_18->sizePolicy().hasHeightForWidth());
        lineEdit_18->setSizePolicy(sizePolicy);
        lineEdit_18->setMinimumSize(QSize(120, 0));
        lineEdit_18->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_13->addWidget(lineEdit_18);

        checkBox_4 = new QCheckBox(groupBox_2);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));

        horizontalLayout_13->addWidget(checkBox_4);


        verticalLayout_2->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy1.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy1);
        label_20->setMinimumSize(QSize(30, 0));
        label_20->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_14->addWidget(label_20);

        lineEdit_19 = new QLineEdit(groupBox_2);
        lineEdit_19->setObjectName(QString::fromUtf8("lineEdit_19"));
        sizePolicy.setHeightForWidth(lineEdit_19->sizePolicy().hasHeightForWidth());
        lineEdit_19->setSizePolicy(sizePolicy);
        lineEdit_19->setMinimumSize(QSize(120, 0));
        lineEdit_19->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_14->addWidget(lineEdit_19);

        checkBox_5 = new QCheckBox(groupBox_2);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));

        horizontalLayout_14->addWidget(checkBox_5);


        verticalLayout_2->addLayout(horizontalLayout_14);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        sizePolicy1.setHeightForWidth(label_21->sizePolicy().hasHeightForWidth());
        label_21->setSizePolicy(sizePolicy1);
        label_21->setMinimumSize(QSize(30, 0));
        label_21->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_15->addWidget(label_21);

        lineEdit_20 = new QLineEdit(groupBox_2);
        lineEdit_20->setObjectName(QString::fromUtf8("lineEdit_20"));
        sizePolicy.setHeightForWidth(lineEdit_20->sizePolicy().hasHeightForWidth());
        lineEdit_20->setSizePolicy(sizePolicy);
        lineEdit_20->setMinimumSize(QSize(120, 0));
        lineEdit_20->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_15->addWidget(lineEdit_20);

        checkBox_6 = new QCheckBox(groupBox_2);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));

        horizontalLayout_15->addWidget(checkBox_6);


        verticalLayout_2->addLayout(horizontalLayout_15);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        sizePolicy1.setHeightForWidth(label_22->sizePolicy().hasHeightForWidth());
        label_22->setSizePolicy(sizePolicy1);
        label_22->setMinimumSize(QSize(30, 0));
        label_22->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_16->addWidget(label_22);

        lineEdit_21 = new QLineEdit(groupBox_2);
        lineEdit_21->setObjectName(QString::fromUtf8("lineEdit_21"));
        sizePolicy.setHeightForWidth(lineEdit_21->sizePolicy().hasHeightForWidth());
        lineEdit_21->setSizePolicy(sizePolicy);
        lineEdit_21->setMinimumSize(QSize(120, 0));
        lineEdit_21->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_16->addWidget(lineEdit_21);

        checkBox_7 = new QCheckBox(groupBox_2);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));

        horizontalLayout_16->addWidget(checkBox_7);


        verticalLayout_2->addLayout(horizontalLayout_16);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QString::fromUtf8("horizontalLayout_17"));
        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        sizePolicy1.setHeightForWidth(label_23->sizePolicy().hasHeightForWidth());
        label_23->setSizePolicy(sizePolicy1);
        label_23->setMinimumSize(QSize(30, 0));
        label_23->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_17->addWidget(label_23);

        lineEdit_22 = new QLineEdit(groupBox_2);
        lineEdit_22->setObjectName(QString::fromUtf8("lineEdit_22"));
        sizePolicy.setHeightForWidth(lineEdit_22->sizePolicy().hasHeightForWidth());
        lineEdit_22->setSizePolicy(sizePolicy);
        lineEdit_22->setMinimumSize(QSize(120, 0));
        lineEdit_22->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_17->addWidget(lineEdit_22);

        checkBox_8 = new QCheckBox(groupBox_2);
        checkBox_8->setObjectName(QString::fromUtf8("checkBox_8"));

        horizontalLayout_17->addWidget(checkBox_8);


        verticalLayout_2->addLayout(horizontalLayout_17);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_24 = new QLabel(groupBox_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        sizePolicy1.setHeightForWidth(label_24->sizePolicy().hasHeightForWidth());
        label_24->setSizePolicy(sizePolicy1);
        label_24->setMinimumSize(QSize(30, 0));
        label_24->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_18->addWidget(label_24);

        lineEdit_23 = new QLineEdit(groupBox_2);
        lineEdit_23->setObjectName(QString::fromUtf8("lineEdit_23"));
        sizePolicy.setHeightForWidth(lineEdit_23->sizePolicy().hasHeightForWidth());
        lineEdit_23->setSizePolicy(sizePolicy);
        lineEdit_23->setMinimumSize(QSize(120, 0));
        lineEdit_23->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_18->addWidget(lineEdit_23);

        checkBox_9 = new QCheckBox(groupBox_2);
        checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));

        horizontalLayout_18->addWidget(checkBox_9);


        verticalLayout_2->addLayout(horizontalLayout_18);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox = new QGroupBox(OrbitFit);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(30, 0));
        label->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_3->addWidget(label);

        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        sizePolicy.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy);
        lineEdit_3->setMinimumSize(QSize(120, 0));
        lineEdit_3->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_3->addWidget(lineEdit_3);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_3->addWidget(label_9);

        lineEdit_10 = new QLineEdit(groupBox);
        lineEdit_10->setObjectName(QString::fromUtf8("lineEdit_10"));
        sizePolicy.setHeightForWidth(lineEdit_10->sizePolicy().hasHeightForWidth());
        lineEdit_10->setSizePolicy(sizePolicy);
        lineEdit_10->setMinimumSize(QSize(120, 0));
        lineEdit_10->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_3->addWidget(lineEdit_10);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(30, 0));
        label_3->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        sizePolicy.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy);
        lineEdit_4->setMinimumSize(QSize(120, 0));
        lineEdit_4->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_4->addWidget(lineEdit_4);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_4->addWidget(label_10);

        lineEdit_11 = new QLineEdit(groupBox);
        lineEdit_11->setObjectName(QString::fromUtf8("lineEdit_11"));
        sizePolicy.setHeightForWidth(lineEdit_11->sizePolicy().hasHeightForWidth());
        lineEdit_11->setSizePolicy(sizePolicy);
        lineEdit_11->setMinimumSize(QSize(120, 0));
        lineEdit_11->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_4->addWidget(lineEdit_11);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);
        label_4->setMinimumSize(QSize(30, 0));
        label_4->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_5->addWidget(label_4);

        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        sizePolicy.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy);
        lineEdit_5->setMinimumSize(QSize(120, 0));
        lineEdit_5->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_5->addWidget(lineEdit_5);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_5->addWidget(label_11);

        lineEdit_12 = new QLineEdit(groupBox);
        lineEdit_12->setObjectName(QString::fromUtf8("lineEdit_12"));
        sizePolicy.setHeightForWidth(lineEdit_12->sizePolicy().hasHeightForWidth());
        lineEdit_12->setSizePolicy(sizePolicy);
        lineEdit_12->setMinimumSize(QSize(120, 0));
        lineEdit_12->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_5->addWidget(lineEdit_12);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);
        label_5->setMinimumSize(QSize(30, 0));
        label_5->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_6->addWidget(label_5);

        lineEdit_6 = new QLineEdit(groupBox);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        sizePolicy.setHeightForWidth(lineEdit_6->sizePolicy().hasHeightForWidth());
        lineEdit_6->setSizePolicy(sizePolicy);
        lineEdit_6->setMinimumSize(QSize(120, 0));
        lineEdit_6->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_6->addWidget(lineEdit_6);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_6->addWidget(label_12);

        lineEdit_13 = new QLineEdit(groupBox);
        lineEdit_13->setObjectName(QString::fromUtf8("lineEdit_13"));
        sizePolicy.setHeightForWidth(lineEdit_13->sizePolicy().hasHeightForWidth());
        lineEdit_13->setSizePolicy(sizePolicy);
        lineEdit_13->setMinimumSize(QSize(120, 0));
        lineEdit_13->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_6->addWidget(lineEdit_13);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy1.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy1);
        label_6->setMinimumSize(QSize(30, 0));
        label_6->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_7->addWidget(label_6);

        lineEdit_7 = new QLineEdit(groupBox);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        sizePolicy.setHeightForWidth(lineEdit_7->sizePolicy().hasHeightForWidth());
        lineEdit_7->setSizePolicy(sizePolicy);
        lineEdit_7->setMinimumSize(QSize(120, 0));
        lineEdit_7->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_7->addWidget(lineEdit_7);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_7->addWidget(label_13);

        lineEdit_14 = new QLineEdit(groupBox);
        lineEdit_14->setObjectName(QString::fromUtf8("lineEdit_14"));
        sizePolicy.setHeightForWidth(lineEdit_14->sizePolicy().hasHeightForWidth());
        lineEdit_14->setSizePolicy(sizePolicy);
        lineEdit_14->setMinimumSize(QSize(120, 0));
        lineEdit_14->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_7->addWidget(lineEdit_14);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setMinimumSize(QSize(30, 0));
        label_7->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_8->addWidget(label_7);

        lineEdit_8 = new QLineEdit(groupBox);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        sizePolicy.setHeightForWidth(lineEdit_8->sizePolicy().hasHeightForWidth());
        lineEdit_8->setSizePolicy(sizePolicy);
        lineEdit_8->setMinimumSize(QSize(120, 0));
        lineEdit_8->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_8->addWidget(lineEdit_8);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_8->addWidget(label_14);

        lineEdit_15 = new QLineEdit(groupBox);
        lineEdit_15->setObjectName(QString::fromUtf8("lineEdit_15"));
        sizePolicy.setHeightForWidth(lineEdit_15->sizePolicy().hasHeightForWidth());
        lineEdit_15->setSizePolicy(sizePolicy);
        lineEdit_15->setMinimumSize(QSize(120, 0));
        lineEdit_15->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_8->addWidget(lineEdit_15);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy1.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy1);
        label_8->setMinimumSize(QSize(30, 0));
        label_8->setMaximumSize(QSize(30, 16777215));

        horizontalLayout_9->addWidget(label_8);

        lineEdit_9 = new QLineEdit(groupBox);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        sizePolicy.setHeightForWidth(lineEdit_9->sizePolicy().hasHeightForWidth());
        lineEdit_9->setSizePolicy(sizePolicy);
        lineEdit_9->setMinimumSize(QSize(120, 0));
        lineEdit_9->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_9->addWidget(lineEdit_9);

        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_9->addWidget(label_15);

        lineEdit_16 = new QLineEdit(groupBox);
        lineEdit_16->setObjectName(QString::fromUtf8("lineEdit_16"));
        sizePolicy.setHeightForWidth(lineEdit_16->sizePolicy().hasHeightForWidth());
        lineEdit_16->setSizePolicy(sizePolicy);
        lineEdit_16->setMinimumSize(QSize(120, 0));
        lineEdit_16->setMaximumSize(QSize(120, 16777215));

        horizontalLayout_9->addWidget(lineEdit_16);


        verticalLayout->addLayout(horizontalLayout_9);


        gridLayout->addWidget(groupBox, 0, 2, 1, 1);


        retranslateUi(OrbitFit);
        QObject::connect(pushButton, SIGNAL(clicked()), OrbitFit, SLOT(close()));

        QMetaObject::connectSlotsByName(OrbitFit);
    } // setupUi

    void retranslateUi(QDialog *OrbitFit)
    {
        OrbitFit->setWindowTitle(QCoreApplication::translate("OrbitFit", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("OrbitFit", "Times & RVs", nullptr));
        label_17->setText(QCoreApplication::translate("OrbitFit", "# Iterations", nullptr));
        checkBox->setText(QCoreApplication::translate("OrbitFit", "SB1", nullptr));
        checkBox_2->setText(QCoreApplication::translate("OrbitFit", "SB2", nullptr));
        pushButton_2->setText(QCoreApplication::translate("OrbitFit", "Go", nullptr));
        label_16->setText(QCoreApplication::translate("OrbitFit", "Work Path:", nullptr));
        pushButton->setText(QCoreApplication::translate("OrbitFit", "Close", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("OrbitFit", "Start Orbit", nullptr));
        label_19->setText(QCoreApplication::translate("OrbitFit", "P:", nullptr));
        checkBox_3->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        label_18->setText(QCoreApplication::translate("OrbitFit", "e:", nullptr));
        checkBox_4->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        label_20->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>K<span style=\" vertical-align:sub;\">1</span>:</p></body></html>", nullptr));
        checkBox_5->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        label_21->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>K<span style=\" vertical-align:sub;\">2</span>:</p></body></html>", nullptr));
        checkBox_6->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        label_22->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>V:</p></body></html>", nullptr));
        checkBox_7->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        label_23->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>T<span style=\" vertical-align:sub;\">0</span>:</p></body></html>", nullptr));
        checkBox_8->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        label_24->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>w<span style=\" vertical-align:sub;\">1</span>:</p></body></html>", nullptr));
        checkBox_9->setText(QCoreApplication::translate("OrbitFit", "lock", nullptr));
        groupBox->setTitle(QCoreApplication::translate("OrbitFit", "Derived Orbit", nullptr));
        label->setText(QCoreApplication::translate("OrbitFit", "P:", nullptr));
        label_9->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
        label_3->setText(QCoreApplication::translate("OrbitFit", "e:", nullptr));
        label_10->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
        label_4->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>K<span style=\" vertical-align:sub;\">1</span>:</p></body></html>", nullptr));
        label_11->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
        label_5->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>K<span style=\" vertical-align:sub;\">2</span>:</p></body></html>", nullptr));
        label_12->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
        label_6->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>V:</p></body></html>", nullptr));
        label_13->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
        label_7->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>T<span style=\" vertical-align:sub;\">0</span>:</p></body></html>", nullptr));
        label_14->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
        label_8->setText(QCoreApplication::translate("OrbitFit", "<html><head/><body><p>w<span style=\" vertical-align:sub;\">1</span>:</p></body></html>", nullptr));
        label_15->setText(QCoreApplication::translate("OrbitFit", "+/-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OrbitFit: public Ui_OrbitFit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORBITFIT_H
