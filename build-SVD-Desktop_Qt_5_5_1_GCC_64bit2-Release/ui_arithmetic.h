/********************************************************************************
** Form generated from reading UI file 'arithmetic.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARITHMETIC_H
#define UI_ARITHMETIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Arithmetic
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QLineEdit *lineEdit_5;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *Arithmetic)
    {
        if (Arithmetic->objectName().isEmpty())
            Arithmetic->setObjectName(QStringLiteral("Arithmetic"));
        Arithmetic->resize(395, 272);
        verticalLayout = new QVBoxLayout(Arithmetic);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(Arithmetic);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(Arithmetic);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(lineEdit);

        checkBox = new QCheckBox(Arithmetic);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        horizontalLayout->addWidget(checkBox);

        checkBox_2 = new QCheckBox(Arithmetic);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        horizontalLayout->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(Arithmetic);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        horizontalLayout->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(Arithmetic);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        horizontalLayout->addWidget(checkBox_4);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(Arithmetic);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        lineEdit_2 = new QLineEdit(Arithmetic);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        sizePolicy.setHeightForWidth(lineEdit_2->sizePolicy().hasHeightForWidth());
        lineEdit_2->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(lineEdit_2);

        checkBox_5 = new QCheckBox(Arithmetic);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));

        horizontalLayout_2->addWidget(checkBox_5);

        checkBox_6 = new QCheckBox(Arithmetic);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));

        horizontalLayout_2->addWidget(checkBox_6);

        lineEdit_5 = new QLineEdit(Arithmetic);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));

        horizontalLayout_2->addWidget(lineEdit_5);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_3 = new QLabel(Arithmetic);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        lineEdit_3 = new QLineEdit(Arithmetic);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        sizePolicy.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(lineEdit_3);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_4 = new QLabel(Arithmetic);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_6->addWidget(label_4);

        lineEdit_4 = new QLineEdit(Arithmetic);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        horizontalLayout_6->addWidget(lineEdit_4);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_2 = new QPushButton(Arithmetic);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(pushButton_2);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 84, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton = new QPushButton(Arithmetic);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(Arithmetic);
        QObject::connect(pushButton, SIGNAL(clicked()), Arithmetic, SLOT(close()));

        QMetaObject::connectSlotsByName(Arithmetic);
    } // setupUi

    void retranslateUi(QDialog *Arithmetic)
    {
        Arithmetic->setWindowTitle(QApplication::translate("Arithmetic", "Dialog", 0));
        label->setText(QApplication::translate("Arithmetic", "A:", 0));
        checkBox->setText(QApplication::translate("Arithmetic", "A+B", 0));
        checkBox_2->setText(QApplication::translate("Arithmetic", "A-B", 0));
        checkBox_3->setText(QApplication::translate("Arithmetic", "A*B", 0));
        checkBox_4->setText(QApplication::translate("Arithmetic", "A/B", 0));
        label_2->setText(QApplication::translate("Arithmetic", "B:", 0));
        checkBox_5->setText(QApplication::translate("Arithmetic", "A +", 0));
        checkBox_6->setText(QApplication::translate("Arithmetic", "A *", 0));
        label_3->setText(QApplication::translate("Arithmetic", "Out:", 0));
        label_4->setText(QApplication::translate("Arithmetic", "Work Path:", 0));
        pushButton_2->setText(QApplication::translate("Arithmetic", "Do", 0));
        pushButton->setText(QApplication::translate("Arithmetic", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class Arithmetic: public Ui_Arithmetic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARITHMETIC_H
