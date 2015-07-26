/********************************************************************************
** Form generated from reading UI file 'giro.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GIRO_H
#define UI_GIRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Giro
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QRadioButton *r1;
    QRadioButton *r2;
    QRadioButton *r3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Giro)
    {
        if (Giro->objectName().isEmpty())
            Giro->setObjectName(QStringLiteral("Giro"));
        Giro->resize(196, 131);
        gridLayout = new QGridLayout(Giro);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        r1 = new QRadioButton(Giro);
        r1->setObjectName(QStringLiteral("r1"));

        verticalLayout->addWidget(r1);

        r2 = new QRadioButton(Giro);
        r2->setObjectName(QStringLiteral("r2"));

        verticalLayout->addWidget(r2);

        r3 = new QRadioButton(Giro);
        r3->setObjectName(QStringLiteral("r3"));

        verticalLayout->addWidget(r3);

        buttonBox = new QDialogButtonBox(Giro);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(Giro);
        QObject::connect(buttonBox, SIGNAL(accepted()), Giro, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Giro, SLOT(reject()));

        QMetaObject::connectSlotsByName(Giro);
    } // setupUi

    void retranslateUi(QDialog *Giro)
    {
        Giro->setWindowTitle(QApplication::translate("Giro", "Giro", 0));
        r1->setText(QApplication::translate("Giro", "Spin", 0));
        r2->setText(QApplication::translate("Giro", "Diferencial", 0));
        r3->setText(QApplication::translate("Giro", "Diferencial diagonal", 0));
    } // retranslateUi

};

namespace Ui {
    class Giro: public Ui_Giro {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GIRO_H
