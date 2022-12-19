/********************************************************************************
** Form generated from reading UI file 'correlation.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CORRELATION_H
#define UI_CORRELATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
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

class Ui_correlation
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_16;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_12;
    QLineEdit *lineEdit_5;
    QCheckBox *checkBox_2;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_14;
    QCheckBox *checkBox_4;
    QSpinBox *spinBox_3;
    QLabel *label_11;
    QSpinBox *spinBox_4;
    QSpacerItem *horizontalSpacer_16;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBox_2;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer_10;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QCheckBox *checkBox_3;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_9;
    QDoubleSpinBox *doubleSpinBox_3;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QSpinBox *spinBox;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_4;
    QSpacerItem *horizontalSpacer_12;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QSpinBox *spinBox_2;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_13;
    QDoubleSpinBox *doubleSpinBox_5;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer_13;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_10;
    QLineEdit *lineEdit_3;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QLineEdit *lineEdit_4;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_15;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *correlation)
    {
        if (correlation->objectName().isEmpty())
            correlation->setObjectName(QString::fromUtf8("correlation"));
        correlation->resize(664, 360);
        QFont font;
        font.setPointSize(9);
        correlation->setFont(font);
        gridLayout = new QGridLayout(correlation);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        groupBox = new QGroupBox(correlation);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(110, 0));
        label->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_3->addWidget(label);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_3->addWidget(label_12);

        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_5->sizePolicy().hasHeightForWidth());
        lineEdit_5->setSizePolicy(sizePolicy1);
        lineEdit_5->setMinimumSize(QSize(25, 0));
        lineEdit_5->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_3->addWidget(lineEdit_5);

        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        horizontalLayout_3->addWidget(checkBox_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        checkBox_4 = new QCheckBox(groupBox);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        sizePolicy1.setHeightForWidth(checkBox_4->sizePolicy().hasHeightForWidth());
        checkBox_4->setSizePolicy(sizePolicy1);
        checkBox_4->setMinimumSize(QSize(110, 0));
        checkBox_4->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_14->addWidget(checkBox_4);

        spinBox_3 = new QSpinBox(groupBox);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setMaximum(999);

        horizontalLayout_14->addWidget(spinBox_3);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_14->addWidget(label_11);

        spinBox_4 = new QSpinBox(groupBox);
        spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));
        spinBox_4->setMaximum(999);

        horizontalLayout_14->addWidget(spinBox_4);

        horizontalSpacer_16 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_16);


        verticalLayout->addLayout(horizontalLayout_14);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        sizePolicy.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
        label_8->setSizePolicy(sizePolicy);
        label_8->setMinimumSize(QSize(110, 0));
        label_8->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_10->addWidget(label_8);

        doubleSpinBox_2 = new QDoubleSpinBox(groupBox);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(4);
        doubleSpinBox_2->setSingleStep(0.100000000000000);
        doubleSpinBox_2->setValue(1.000000000000000);

        horizontalLayout_10->addWidget(doubleSpinBox_2);

        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        horizontalLayout_10->addWidget(checkBox);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_10);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(110, 0));
        label_2->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        sizePolicy1.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(lineEdit_2);

        checkBox_3 = new QCheckBox(groupBox);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        horizontalLayout_2->addWidget(checkBox_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy);
        label_9->setMinimumSize(QSize(110, 0));
        label_9->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_11->addWidget(label_9);

        doubleSpinBox_3 = new QDoubleSpinBox(groupBox);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setDecimals(4);
        doubleSpinBox_3->setSingleStep(0.100000000000000);
        doubleSpinBox_3->setValue(1.000000000000000);

        horizontalLayout_11->addWidget(doubleSpinBox_3);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_11);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(110, 0));
        label_3->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_4->addWidget(label_3);

        spinBox = new QSpinBox(groupBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMaximum(9999);

        horizontalLayout_4->addWidget(spinBox);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_5->addWidget(pushButton_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(110, 0));
        label_4->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_6->addWidget(label_4);

        doubleSpinBox = new QDoubleSpinBox(groupBox);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setEnabled(false);
        doubleSpinBox->setMinimum(-9999.000000000000000);
        doubleSpinBox->setMaximum(9999.000000000000000);

        horizontalLayout_6->addWidget(doubleSpinBox);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        sizePolicy.setHeightForWidth(label_6->sizePolicy().hasHeightForWidth());
        label_6->setSizePolicy(sizePolicy);
        label_6->setMinimumSize(QSize(110, 0));
        label_6->setMaximumSize(QSize(110, 16777215));

        horizontalLayout_12->addWidget(label_6);

        doubleSpinBox_4 = new QDoubleSpinBox(groupBox);
        doubleSpinBox_4->setObjectName(QString::fromUtf8("doubleSpinBox_4"));
        doubleSpinBox_4->setEnabled(false);
        doubleSpinBox_4->setDecimals(6);

        horizontalLayout_12->addWidget(doubleSpinBox_4);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_12);


        verticalLayout->addLayout(horizontalLayout_12);


        horizontalLayout_16->addWidget(groupBox);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_2 = new QGroupBox(correlation);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(150, 0));
        label_5->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_7->addWidget(label_5);

        spinBox_2 = new QSpinBox(groupBox_2);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setMaximum(9999);

        horizontalLayout_7->addWidget(spinBox_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(150, 0));
        label_13->setMaximumSize(QSize(150, 16777215));

        horizontalLayout_15->addWidget(label_13);

        doubleSpinBox_5 = new QDoubleSpinBox(groupBox_2);
        doubleSpinBox_5->setObjectName(QString::fromUtf8("doubleSpinBox_5"));
        doubleSpinBox_5->setDecimals(3);
        doubleSpinBox_5->setMinimum(-99.989999999999995);
        doubleSpinBox_5->setMaximum(999.990000000000009);

        horizontalLayout_15->addWidget(doubleSpinBox_5);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_14);


        verticalLayout_2->addLayout(horizontalLayout_15);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QString::fromUtf8("horizontalLayout_13"));
        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));

        horizontalLayout_13->addWidget(pushButton_4);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_13);


        verticalLayout_2->addLayout(horizontalLayout_13);


        verticalLayout_4->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(correlation);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(75, 0));
        label_10->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_8->addWidget(label_10);

        lineEdit_3 = new QLineEdit(groupBox_3);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        sizePolicy1.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy1);

        horizontalLayout_8->addWidget(lineEdit_3);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        sizePolicy.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy);
        label_7->setMinimumSize(QSize(75, 0));
        label_7->setMaximumSize(QSize(75, 16777215));

        horizontalLayout_9->addWidget(label_7);

        lineEdit_4 = new QLineEdit(groupBox_3);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        sizePolicy1.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy1);

        horizontalLayout_9->addWidget(lineEdit_4);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_9);


        verticalLayout_3->addLayout(horizontalLayout_9);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addWidget(groupBox_3);


        horizontalLayout_16->addLayout(verticalLayout_4);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_15);


        verticalLayout_5->addLayout(horizontalLayout_16);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout_5, 0, 0, 1, 1);


        retranslateUi(correlation);
        QObject::connect(pushButton, SIGNAL(clicked()), correlation, SLOT(close()));

        QMetaObject::connectSlotsByName(correlation);
    } // setupUi

    void retranslateUi(QDialog *correlation)
    {
        correlation->setWindowTitle(QCoreApplication::translate("correlation", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("correlation", "Cross-Correlation", nullptr));
        label->setText(QCoreApplication::translate("correlation", "Spectrum:", nullptr));
        label_12->setText(QCoreApplication::translate("correlation", "*.", nullptr));
        checkBox_2->setText(QCoreApplication::translate("correlation", "Log(x)", nullptr));
        checkBox_4->setText(QCoreApplication::translate("correlation", "Sequence from", nullptr));
        label_11->setText(QCoreApplication::translate("correlation", "to", nullptr));
        label_8->setText(QCoreApplication::translate("correlation", "Continuum:", nullptr));
        checkBox->setText(QCoreApplication::translate("correlation", "Bidiagonal Matrix used", nullptr));
        label_2->setText(QCoreApplication::translate("correlation", "Template:", nullptr));
        checkBox_3->setText(QCoreApplication::translate("correlation", "Log(x)", nullptr));
        label_9->setText(QCoreApplication::translate("correlation", "Continuum:", nullptr));
        label_3->setText(QCoreApplication::translate("correlation", "Correlation Shift:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("correlation", "Correlation", nullptr));
        label_4->setText(QCoreApplication::translate("correlation", "Result [pix]:", nullptr));
        label_6->setText(QCoreApplication::translate("correlation", "Result [log(A)]:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("correlation", "Shift/Correct Spectrum", nullptr));
        label_5->setText(QCoreApplication::translate("correlation", "Correct Spectrum by [pix]:", nullptr));
        label_13->setText(QCoreApplication::translate("correlation", "Add systematic Shift:", nullptr));
        pushButton_4->setText(QCoreApplication::translate("correlation", "Correct", nullptr));
        groupBox_3->setTitle(QString());
        label_10->setText(QCoreApplication::translate("correlation", "Save As:", nullptr));
        label_7->setText(QCoreApplication::translate("correlation", "Work Path:", nullptr));
        pushButton->setText(QCoreApplication::translate("correlation", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class correlation: public Ui_correlation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CORRELATION_H
