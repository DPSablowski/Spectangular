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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OrbitFit
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QFrame *line;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_8;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_10;
    QDoubleSpinBox *doubleSpinBox_9;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label_11;
    QDoubleSpinBox *doubleSpinBox_10;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_4;
    QLabel *label_12;
    QDoubleSpinBox *doubleSpinBox_11;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_5;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBox_12;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_7;
    QDoubleSpinBox *doubleSpinBox_6;
    QLabel *label_14;
    QDoubleSpinBox *doubleSpinBox_13;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBox_7;
    QLabel *label_15;
    QDoubleSpinBox *doubleSpinBox_14;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_16;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QDialog *OrbitFit)
    {
        if (OrbitFit->objectName().isEmpty())
            OrbitFit->setObjectName(QString::fromUtf8("OrbitFit"));
        OrbitFit->resize(520, 318);
        gridLayout = new QGridLayout(OrbitFit);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
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


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        line = new QFrame(OrbitFit);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 1, 3, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(OrbitFit);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_3->addWidget(label);

        doubleSpinBox = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setDecimals(5);
        doubleSpinBox->setMaximum(9999.989999999999782);

        horizontalLayout_3->addWidget(doubleSpinBox);

        label_9 = new QLabel(OrbitFit);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_3->addWidget(label_9);

        doubleSpinBox_8 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_8->setObjectName(QString::fromUtf8("doubleSpinBox_8"));

        horizontalLayout_3->addWidget(doubleSpinBox_8);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(OrbitFit);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_4->addWidget(label_3);

        doubleSpinBox_2 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(5);
        doubleSpinBox_2->setMaximum(1.000000000000000);

        horizontalLayout_4->addWidget(doubleSpinBox_2);

        label_10 = new QLabel(OrbitFit);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_4->addWidget(label_10);

        doubleSpinBox_9 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_9->setObjectName(QString::fromUtf8("doubleSpinBox_9"));

        horizontalLayout_4->addWidget(doubleSpinBox_9);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(OrbitFit);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_5->addWidget(label_4);

        doubleSpinBox_3 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setDecimals(5);
        doubleSpinBox_3->setMaximum(9999.989999999999782);

        horizontalLayout_5->addWidget(doubleSpinBox_3);

        label_11 = new QLabel(OrbitFit);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_5->addWidget(label_11);

        doubleSpinBox_10 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_10->setObjectName(QString::fromUtf8("doubleSpinBox_10"));

        horizontalLayout_5->addWidget(doubleSpinBox_10);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(OrbitFit);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_6->addWidget(label_5);

        doubleSpinBox_4 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
        doubleSpinBox_4->setDecimals(5);
        doubleSpinBox_4->setMaximum(9999.989999999999782);

        horizontalLayout_6->addWidget(doubleSpinBox_4);

        label_12 = new QLabel(OrbitFit);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_6->addWidget(label_12);

        doubleSpinBox_11 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_11->setObjectName(QString::fromUtf8("doubleSpinBox_11"));

        horizontalLayout_6->addWidget(doubleSpinBox_11);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(OrbitFit);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        doubleSpinBox_5 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));
        doubleSpinBox_5->setDecimals(5);
        doubleSpinBox_5->setMaximum(9999.989999999999782);

        horizontalLayout_7->addWidget(doubleSpinBox_5);

        label_13 = new QLabel(OrbitFit);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_7->addWidget(label_13);

        doubleSpinBox_12 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_12->setObjectName(QString::fromUtf8("doubleSpinBox_12"));

        horizontalLayout_7->addWidget(doubleSpinBox_12);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_7 = new QLabel(OrbitFit);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_8->addWidget(label_7);

        doubleSpinBox_6 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_6->setObjectName(QString::fromUtf8("doubleSpinBox_6"));
        doubleSpinBox_6->setDecimals(5);
        doubleSpinBox_6->setMaximum(9999999.990000000223517);

        horizontalLayout_8->addWidget(doubleSpinBox_6);

        label_14 = new QLabel(OrbitFit);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_8->addWidget(label_14);

        doubleSpinBox_13 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_13->setObjectName(QString::fromUtf8("doubleSpinBox_13"));

        horizontalLayout_8->addWidget(doubleSpinBox_13);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_8 = new QLabel(OrbitFit);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_9->addWidget(label_8);

        doubleSpinBox_7 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_7->setObjectName(QString::fromUtf8("doubleSpinBox_7"));
        doubleSpinBox_7->setDecimals(4);
        doubleSpinBox_7->setMaximum(999.990000000000009);

        horizontalLayout_9->addWidget(doubleSpinBox_7);

        label_15 = new QLabel(OrbitFit);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_9->addWidget(label_15);

        doubleSpinBox_14 = new QDoubleSpinBox(OrbitFit);
        doubleSpinBox_14->setObjectName(QString::fromUtf8("doubleSpinBox_14"));

        horizontalLayout_9->addWidget(doubleSpinBox_14);


        verticalLayout->addLayout(horizontalLayout_9);


        gridLayout->addLayout(verticalLayout, 0, 2, 4, 1);

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


        gridLayout->addLayout(horizontalLayout_10, 1, 0, 1, 1);

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


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        pushButton = new QPushButton(OrbitFit);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(pushButton, 3, 0, 1, 1);


        retranslateUi(OrbitFit);
        QObject::connect(pushButton, SIGNAL(clicked()), OrbitFit, SLOT(close()));

        QMetaObject::connectSlotsByName(OrbitFit);
    } // setupUi

    void retranslateUi(QDialog *OrbitFit)
    {
        OrbitFit->setWindowTitle(QCoreApplication::translate("OrbitFit", "Dialog", nullptr));
        label_2->setText(QCoreApplication::translate("OrbitFit", "Times & RVs", nullptr));
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
        label_16->setText(QCoreApplication::translate("OrbitFit", "Work Path:", nullptr));
        checkBox->setText(QCoreApplication::translate("OrbitFit", "SB1", nullptr));
        checkBox_2->setText(QCoreApplication::translate("OrbitFit", "SB2", nullptr));
        pushButton_2->setText(QCoreApplication::translate("OrbitFit", "Go", nullptr));
        pushButton->setText(QCoreApplication::translate("OrbitFit", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OrbitFit: public Ui_OrbitFit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORBITFIT_H
