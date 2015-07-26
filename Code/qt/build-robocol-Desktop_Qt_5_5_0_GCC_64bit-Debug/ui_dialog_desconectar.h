/********************************************************************************
** Form generated from reading UI file 'dialog_desconectar.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_DESCONECTAR_H
#define UI_DIALOG_DESCONECTAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_desconectar
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBoxIzq;
    QCheckBox *checkBoxDer;
    QCheckBox *checkBoxBra;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Dialog_desconectar)
    {
        if (Dialog_desconectar->objectName().isEmpty())
            Dialog_desconectar->setObjectName(QStringLiteral("Dialog_desconectar"));
        Dialog_desconectar->resize(279, 158);
        layoutWidget = new QWidget(Dialog_desconectar);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 261, 141));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(layoutWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QLatin1String("border-style: solid;\n"
"border-width: 1px;"));
        layoutWidget1 = new QWidget(groupBox);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 20, 245, 82));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("border-style: none;"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setStyleSheet(QStringLiteral("border-style: none;"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setStyleSheet(QStringLiteral("border-style: none;"));

        verticalLayout->addWidget(label_3);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        checkBoxIzq = new QCheckBox(layoutWidget1);
        checkBoxIzq->setObjectName(QStringLiteral("checkBoxIzq"));

        verticalLayout_2->addWidget(checkBoxIzq);

        checkBoxDer = new QCheckBox(layoutWidget1);
        checkBoxDer->setObjectName(QStringLiteral("checkBoxDer"));

        verticalLayout_2->addWidget(checkBoxDer);

        checkBoxBra = new QCheckBox(layoutWidget1);
        checkBoxBra->setObjectName(QStringLiteral("checkBoxBra"));

        verticalLayout_2->addWidget(checkBoxBra);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_3->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(Dialog_desconectar);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog_desconectar, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog_desconectar, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog_desconectar);
    } // setupUi

    void retranslateUi(QDialog *Dialog_desconectar)
    {
        Dialog_desconectar->setWindowTitle(QApplication::translate("Dialog_desconectar", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("Dialog_desconectar", "Apagar servidores", 0));
        label->setText(QApplication::translate("Dialog_desconectar", "LLantas izquierda:", 0));
        label_2->setText(QApplication::translate("Dialog_desconectar", "LLantas derechas:", 0));
        label_3->setText(QApplication::translate("Dialog_desconectar", "Brazo:", 0));
        checkBoxIzq->setText(QApplication::translate("Dialog_desconectar", "Desactivar", 0));
        checkBoxDer->setText(QApplication::translate("Dialog_desconectar", "Desactivar", 0));
        checkBoxBra->setText(QApplication::translate("Dialog_desconectar", "Desactivar", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog_desconectar: public Ui_Dialog_desconectar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_DESCONECTAR_H
