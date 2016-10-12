/********************************************************************************
** Form generated from reading UI file 'orbitfit.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ORBITFIT_H
#define UI_ORBITFIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OrbitFit
{
public:
    QPushButton *pushButton;
    QFrame *line;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_2;
    QWidget *widget2;
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
    QWidget *widget3;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_16;
    QLineEdit *lineEdit_2;

    void setupUi(QDialog *OrbitFit)
    {
        if (OrbitFit->objectName().isEmpty())
            OrbitFit->setObjectName(QStringLiteral("OrbitFit"));
        OrbitFit->resize(801, 318);
        pushButton = new QPushButton(OrbitFit);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 270, 95, 30));
        line = new QFrame(OrbitFit);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(283, 20, 20, 201));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        widget = new QWidget(OrbitFit);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 20, 237, 34));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        widget1 = new QWidget(OrbitFit);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(20, 90, 223, 32));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        checkBox = new QCheckBox(widget1);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout_2->addWidget(checkBox);

        checkBox_2 = new QCheckBox(widget1);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        horizontalLayout_2->addWidget(checkBox_2);

        pushButton_2 = new QPushButton(widget1);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(pushButton_2);

        widget2 = new QWidget(OrbitFit);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setGeometry(QRect(350, 50, 217, 262));
        verticalLayout = new QVBoxLayout(widget2);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(widget2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        doubleSpinBox = new QDoubleSpinBox(widget2);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));

        horizontalLayout_3->addWidget(doubleSpinBox);

        label_9 = new QLabel(widget2);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_3->addWidget(label_9);

        doubleSpinBox_8 = new QDoubleSpinBox(widget2);
        doubleSpinBox_8->setObjectName(QStringLiteral("doubleSpinBox_8"));

        horizontalLayout_3->addWidget(doubleSpinBox_8);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(widget2);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        doubleSpinBox_2 = new QDoubleSpinBox(widget2);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));

        horizontalLayout_4->addWidget(doubleSpinBox_2);

        label_10 = new QLabel(widget2);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_4->addWidget(label_10);

        doubleSpinBox_9 = new QDoubleSpinBox(widget2);
        doubleSpinBox_9->setObjectName(QStringLiteral("doubleSpinBox_9"));

        horizontalLayout_4->addWidget(doubleSpinBox_9);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_4 = new QLabel(widget2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_5->addWidget(label_4);

        doubleSpinBox_3 = new QDoubleSpinBox(widget2);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));

        horizontalLayout_5->addWidget(doubleSpinBox_3);

        label_11 = new QLabel(widget2);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_5->addWidget(label_11);

        doubleSpinBox_10 = new QDoubleSpinBox(widget2);
        doubleSpinBox_10->setObjectName(QStringLiteral("doubleSpinBox_10"));

        horizontalLayout_5->addWidget(doubleSpinBox_10);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_5 = new QLabel(widget2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_6->addWidget(label_5);

        doubleSpinBox_4 = new QDoubleSpinBox(widget2);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));

        horizontalLayout_6->addWidget(doubleSpinBox_4);

        label_12 = new QLabel(widget2);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_6->addWidget(label_12);

        doubleSpinBox_11 = new QDoubleSpinBox(widget2);
        doubleSpinBox_11->setObjectName(QStringLiteral("doubleSpinBox_11"));

        horizontalLayout_6->addWidget(doubleSpinBox_11);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_6 = new QLabel(widget2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_7->addWidget(label_6);

        doubleSpinBox_5 = new QDoubleSpinBox(widget2);
        doubleSpinBox_5->setObjectName(QStringLiteral("doubleSpinBox_5"));

        horizontalLayout_7->addWidget(doubleSpinBox_5);

        label_13 = new QLabel(widget2);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_7->addWidget(label_13);

        doubleSpinBox_12 = new QDoubleSpinBox(widget2);
        doubleSpinBox_12->setObjectName(QStringLiteral("doubleSpinBox_12"));

        horizontalLayout_7->addWidget(doubleSpinBox_12);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_7 = new QLabel(widget2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_8->addWidget(label_7);

        doubleSpinBox_6 = new QDoubleSpinBox(widget2);
        doubleSpinBox_6->setObjectName(QStringLiteral("doubleSpinBox_6"));

        horizontalLayout_8->addWidget(doubleSpinBox_6);

        label_14 = new QLabel(widget2);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_8->addWidget(label_14);

        doubleSpinBox_13 = new QDoubleSpinBox(widget2);
        doubleSpinBox_13->setObjectName(QStringLiteral("doubleSpinBox_13"));

        horizontalLayout_8->addWidget(doubleSpinBox_13);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_8 = new QLabel(widget2);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_9->addWidget(label_8);

        doubleSpinBox_7 = new QDoubleSpinBox(widget2);
        doubleSpinBox_7->setObjectName(QStringLiteral("doubleSpinBox_7"));

        horizontalLayout_9->addWidget(doubleSpinBox_7);

        label_15 = new QLabel(widget2);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout_9->addWidget(label_15);

        doubleSpinBox_14 = new QDoubleSpinBox(widget2);
        doubleSpinBox_14->setObjectName(QStringLiteral("doubleSpinBox_14"));

        horizontalLayout_9->addWidget(doubleSpinBox_14);


        verticalLayout->addLayout(horizontalLayout_9);

        widget3 = new QWidget(OrbitFit);
        widget3->setObjectName(QStringLiteral("widget3"));
        widget3->setGeometry(QRect(40, 60, 230, 34));
        horizontalLayout_10 = new QHBoxLayout(widget3);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_16 = new QLabel(widget3);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout_10->addWidget(label_16);

        lineEdit_2 = new QLineEdit(widget3);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_10->addWidget(lineEdit_2);


        retranslateUi(OrbitFit);
        QObject::connect(pushButton, SIGNAL(clicked()), OrbitFit, SLOT(close()));

        QMetaObject::connectSlotsByName(OrbitFit);
    } // setupUi

    void retranslateUi(QDialog *OrbitFit)
    {
        OrbitFit->setWindowTitle(QApplication::translate("OrbitFit", "Dialog", 0));
        pushButton->setText(QApplication::translate("OrbitFit", "Close", 0));
        label_2->setText(QApplication::translate("OrbitFit", "Times & RVs", 0));
        checkBox->setText(QApplication::translate("OrbitFit", "SB1", 0));
        checkBox_2->setText(QApplication::translate("OrbitFit", "SB2", 0));
        pushButton_2->setText(QApplication::translate("OrbitFit", "Go", 0));
        label->setText(QApplication::translate("OrbitFit", "P:", 0));
        label_9->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_3->setText(QApplication::translate("OrbitFit", "e:", 0));
        label_10->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_4->setText(QApplication::translate("OrbitFit", "<html><head/><body><p>K<span style=\" vertical-align:sub;\">1</span>:</p></body></html>", 0));
        label_11->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_5->setText(QApplication::translate("OrbitFit", "<html><head/><body><p>K<span style=\" vertical-align:sub;\">2</span>:</p></body></html>", 0));
        label_12->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_6->setText(QApplication::translate("OrbitFit", "<html><head/><body><p>V:</p></body></html>", 0));
        label_13->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_7->setText(QApplication::translate("OrbitFit", "<html><head/><body><p>T<span style=\" vertical-align:sub;\">0</span>:</p></body></html>", 0));
        label_14->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_8->setText(QApplication::translate("OrbitFit", "<html><head/><body><p>w<span style=\" vertical-align:sub;\">1</span>:</p></body></html>", 0));
        label_15->setText(QApplication::translate("OrbitFit", "+/-", 0));
        label_16->setText(QApplication::translate("OrbitFit", "Work Path:", 0));
    } // retranslateUi

};

namespace Ui {
    class OrbitFit: public Ui_OrbitFit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ORBITFIT_H
