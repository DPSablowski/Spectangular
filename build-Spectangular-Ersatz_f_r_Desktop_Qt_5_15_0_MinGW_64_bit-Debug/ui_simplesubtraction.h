/********************************************************************************
** Form generated from reading UI file 'simplesubtraction.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLESUBTRACTION_H
#define UI_SIMPLESUBTRACTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SimpleSubtraction
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_3;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_4;
    QHBoxLayout *horizontalLayout_4;
    QComboBox *comboBox;
    QLabel *label_7;
    QLineEdit *lineEdit_3;
    QLabel *label_8;
    QLineEdit *lineEdit_4;
    QLabel *label_9;
    QLineEdit *lineEdit_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_10;
    QLineEdit *lineEdit_6;
    QLabel *label_11;
    QLineEdit *lineEdit_7;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_12;
    QLineEdit *lineEdit_8;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *SimpleSubtraction)
    {
        if (SimpleSubtraction->objectName().isEmpty())
            SimpleSubtraction->setObjectName(QString::fromUtf8("SimpleSubtraction"));
        SimpleSubtraction->resize(721, 310);
        verticalLayout = new QVBoxLayout(SimpleSubtraction);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(SimpleSubtraction);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(SimpleSubtraction);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(lineEdit);

        label_3 = new QLabel(SimpleSubtraction);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        doubleSpinBox = new QDoubleSpinBox(SimpleSubtraction);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setDecimals(4);
        doubleSpinBox->setMinimum(-9999.000000000000000);
        doubleSpinBox->setValue(0.000000000000000);

        horizontalLayout_2->addWidget(doubleSpinBox);

        label_4 = new QLabel(SimpleSubtraction);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        doubleSpinBox_2 = new QDoubleSpinBox(SimpleSubtraction);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(4);
        doubleSpinBox_2->setMinimum(-9999.000000000000000);
        doubleSpinBox_2->setMaximum(9999.989999999999782);

        horizontalLayout_2->addWidget(doubleSpinBox_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_2 = new QLabel(SimpleSubtraction);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_2 = new QLineEdit(SimpleSubtraction);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        sizePolicy.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(lineEdit_2);

        label_5 = new QLabel(SimpleSubtraction);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        doubleSpinBox_3 = new QDoubleSpinBox(SimpleSubtraction);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setDecimals(4);
        doubleSpinBox_3->setMinimum(-9999.000000000000000);
        doubleSpinBox_3->setMaximum(9999.989999999999782);

        horizontalLayout_3->addWidget(doubleSpinBox_3);

        label_6 = new QLabel(SimpleSubtraction);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_3->addWidget(label_6);

        doubleSpinBox_4 = new QDoubleSpinBox(SimpleSubtraction);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
        doubleSpinBox_4->setDecimals(4);
        doubleSpinBox_4->setMinimum(-9999.989999999999782);
        doubleSpinBox_4->setMaximum(9999.989999999999782);

        horizontalLayout_3->addWidget(doubleSpinBox_4);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        comboBox = new QComboBox(SimpleSubtraction);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_4->addWidget(comboBox);

        label_7 = new QLabel(SimpleSubtraction);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        horizontalLayout_4->addWidget(label_7);

        lineEdit_3 = new QLineEdit(SimpleSubtraction);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        sizePolicy.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(lineEdit_3);

        label_8 = new QLabel(SimpleSubtraction);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_4->addWidget(label_8);

        lineEdit_4 = new QLineEdit(SimpleSubtraction);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        sizePolicy.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(lineEdit_4);

        label_9 = new QLabel(SimpleSubtraction);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_4->addWidget(label_9);

        lineEdit_5 = new QLineEdit(SimpleSubtraction);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        sizePolicy.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(lineEdit_5);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_10 = new QLabel(SimpleSubtraction);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_5->addWidget(label_10);

        lineEdit_6 = new QLineEdit(SimpleSubtraction);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        sizePolicy.setHeightForWidth(lineEdit_6->sizePolicy().hasHeightForWidth());
        lineEdit_6->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(lineEdit_6);

        label_11 = new QLabel(SimpleSubtraction);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_5->addWidget(label_11);

        lineEdit_7 = new QLineEdit(SimpleSubtraction);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        sizePolicy.setHeightForWidth(lineEdit_7->sizePolicy().hasHeightForWidth());
        lineEdit_7->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(lineEdit_7);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_12 = new QLabel(SimpleSubtraction);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_6->addWidget(label_12);

        lineEdit_8 = new QLineEdit(SimpleSubtraction);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        sizePolicy.setHeightForWidth(lineEdit_8->sizePolicy().hasHeightForWidth());
        lineEdit_8->setSizePolicy(sizePolicy);

        horizontalLayout_6->addWidget(lineEdit_8);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_2 = new QPushButton(SimpleSubtraction);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(SimpleSubtraction);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SimpleSubtraction);
        QObject::connect(pushButton, SIGNAL(clicked()), SimpleSubtraction, SLOT(close()));

        QMetaObject::connectSlotsByName(SimpleSubtraction);
    } // setupUi

    void retranslateUi(QDialog *SimpleSubtraction)
    {
        SimpleSubtraction->setWindowTitle(QCoreApplication::translate("SimpleSubtraction", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SimpleSubtraction", "Spectrum 1:", nullptr));
        label_3->setText(QCoreApplication::translate("SimpleSubtraction", "RV A:", nullptr));
        label_4->setText(QCoreApplication::translate("SimpleSubtraction", "RV B:", nullptr));
        label_2->setText(QCoreApplication::translate("SimpleSubtraction", "Spectrum 2:", nullptr));
        label_5->setText(QCoreApplication::translate("SimpleSubtraction", "RV A:", nullptr));
        label_6->setText(QCoreApplication::translate("SimpleSubtraction", "RV B:", nullptr));
        label_7->setText(QCoreApplication::translate("SimpleSubtraction", "Extension", nullptr));
        label_8->setText(QCoreApplication::translate("SimpleSubtraction", "W.Col.", nullptr));
        label_9->setText(QCoreApplication::translate("SimpleSubtraction", "I.Col.", nullptr));
        label_10->setText(QCoreApplication::translate("SimpleSubtraction", "Output A:", nullptr));
        label_11->setText(QCoreApplication::translate("SimpleSubtraction", "Output B:", nullptr));
        label_12->setText(QCoreApplication::translate("SimpleSubtraction", "Work Path:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("SimpleSubtraction", "Do It", nullptr));
        pushButton->setText(QCoreApplication::translate("SimpleSubtraction", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleSubtraction: public Ui_SimpleSubtraction {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLESUBTRACTION_H
