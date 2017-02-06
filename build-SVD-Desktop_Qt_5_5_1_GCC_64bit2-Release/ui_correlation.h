/********************************************************************************
** Form generated from reading UI file 'correlation.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CORRELATION_H
#define UI_CORRELATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit;
    QCheckBox *checkBox_2;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_8;
    QSpacerItem *horizontalSpacer_10;
    QDoubleSpinBox *doubleSpinBox_2;
    QCheckBox *checkBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit_2;
    QCheckBox *checkBox_3;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer_11;
    QDoubleSpinBox *doubleSpinBox_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_4;
    QSpinBox *spinBox;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_6;
    QDoubleSpinBox *doubleSpinBox;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_6;
    QSpacerItem *horizontalSpacer_12;
    QDoubleSpinBox *doubleSpinBox_4;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_7;
    QSpinBox *spinBox_2;
    QHBoxLayout *horizontalLayout_13;
    QPushButton *pushButton_4;
    QSpacerItem *horizontalSpacer_13;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *pushButton_3;
    QSpacerItem *horizontalSpacer_8;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QSpacerItem *horizontalSpacer_9;
    QLineEdit *lineEdit_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *correlation)
    {
        if (correlation->objectName().isEmpty())
            correlation->setObjectName(QStringLiteral("correlation"));
        correlation->resize(243, 503);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(99, 242, 88, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(205, 255, 201, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(152, 248, 144, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(49, 121, 44, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(66, 161, 58, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush7(QColor(177, 248, 171, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        correlation->setPalette(palette);
        QFont font;
        font.setPointSize(9);
        correlation->setFont(font);
        verticalLayout = new QVBoxLayout(correlation);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(correlation);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        lineEdit = new QLineEdit(correlation);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(lineEdit);

        checkBox_2 = new QCheckBox(correlation);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        horizontalLayout_3->addWidget(checkBox_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_8 = new QLabel(correlation);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_10->addWidget(label_8);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_10);

        doubleSpinBox_2 = new QDoubleSpinBox(correlation);
        doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
        doubleSpinBox_2->setDecimals(4);
        doubleSpinBox_2->setSingleStep(0.1);

        horizontalLayout_10->addWidget(doubleSpinBox_2);


        verticalLayout->addLayout(horizontalLayout_10);

        checkBox = new QCheckBox(correlation);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout->addWidget(checkBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(correlation);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        lineEdit_2 = new QLineEdit(correlation);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        sizePolicy.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(lineEdit_2);

        checkBox_3 = new QCheckBox(correlation);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        horizontalLayout_2->addWidget(checkBox_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_9 = new QLabel(correlation);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_11->addWidget(label_9);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_11);

        doubleSpinBox_3 = new QDoubleSpinBox(correlation);
        doubleSpinBox_3->setObjectName(QStringLiteral("doubleSpinBox_3"));
        doubleSpinBox_3->setDecimals(4);
        doubleSpinBox_3->setSingleStep(0.1);

        horizontalLayout_11->addWidget(doubleSpinBox_3);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(correlation);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        spinBox = new QSpinBox(correlation);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setMaximum(9999);

        horizontalLayout_4->addWidget(spinBox);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_2 = new QPushButton(correlation);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_5->addWidget(pushButton_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_4 = new QLabel(correlation);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_6->addWidget(label_4);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        doubleSpinBox = new QDoubleSpinBox(correlation);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setEnabled(false);
        doubleSpinBox->setMinimum(-9999);
        doubleSpinBox->setMaximum(9999);

        horizontalLayout_6->addWidget(doubleSpinBox);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_6 = new QLabel(correlation);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_12->addWidget(label_6);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_12);

        doubleSpinBox_4 = new QDoubleSpinBox(correlation);
        doubleSpinBox_4->setObjectName(QStringLiteral("doubleSpinBox_4"));
        doubleSpinBox_4->setEnabled(false);
        doubleSpinBox_4->setDecimals(5);
        doubleSpinBox_4->setMinimum(-9);
        doubleSpinBox_4->setMaximum(9.99);
        doubleSpinBox_4->setSingleStep(0.001);

        horizontalLayout_12->addWidget(doubleSpinBox_4);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_5 = new QLabel(correlation);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_7->addWidget(label_5);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);

        spinBox_2 = new QSpinBox(correlation);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setMinimum(-9999);
        spinBox_2->setMaximum(9999);

        horizontalLayout_7->addWidget(spinBox_2);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        pushButton_4 = new QPushButton(correlation);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_13->addWidget(pushButton_4);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_13);


        verticalLayout->addLayout(horizontalLayout_13);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        pushButton_3 = new QPushButton(correlation);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_8->addWidget(pushButton_3);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_8);

        lineEdit_3 = new QLineEdit(correlation);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        sizePolicy.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy);

        horizontalLayout_8->addWidget(lineEdit_3);


        verticalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_7 = new QLabel(correlation);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_9->addWidget(label_7);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_9);

        lineEdit_4 = new QLineEdit(correlation);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        sizePolicy.setHeightForWidth(lineEdit_4->sizePolicy().hasHeightForWidth());
        lineEdit_4->setSizePolicy(sizePolicy);

        horizontalLayout_9->addWidget(lineEdit_4);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(correlation);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(correlation);
        QObject::connect(pushButton, SIGNAL(clicked()), correlation, SLOT(close()));

        QMetaObject::connectSlotsByName(correlation);
    } // setupUi

    void retranslateUi(QDialog *correlation)
    {
        correlation->setWindowTitle(QApplication::translate("correlation", "Dialog", 0));
        label->setText(QApplication::translate("correlation", "Spectrum:", 0));
        checkBox_2->setText(QApplication::translate("correlation", "Log(x)", 0));
        label_8->setText(QApplication::translate("correlation", "Continuum:", 0));
        checkBox->setText(QApplication::translate("correlation", "Bidiagonal Matrix used", 0));
        label_2->setText(QApplication::translate("correlation", "Template:", 0));
        checkBox_3->setText(QApplication::translate("correlation", "Log(x)", 0));
        label_9->setText(QApplication::translate("correlation", "Continuum:", 0));
        label_3->setText(QApplication::translate("correlation", "Correlation Shift:", 0));
        pushButton_2->setText(QApplication::translate("correlation", "Correlation", 0));
        label_4->setText(QApplication::translate("correlation", "Result [pix]:", 0));
        label_6->setText(QApplication::translate("correlation", "Result [log(A)]:", 0));
        label_5->setText(QApplication::translate("correlation", "Correct Spectrum by [pix]:", 0));
        pushButton_4->setText(QApplication::translate("correlation", "Correct", 0));
        pushButton_3->setText(QApplication::translate("correlation", "Save as:", 0));
        label_7->setText(QApplication::translate("correlation", "Work Path:", 0));
        pushButton->setText(QApplication::translate("correlation", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class correlation: public Ui_correlation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CORRELATION_H
