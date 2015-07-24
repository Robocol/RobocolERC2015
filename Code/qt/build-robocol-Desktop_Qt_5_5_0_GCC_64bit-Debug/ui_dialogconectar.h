/********************************************************************************
** Form generated from reading UI file 'dialogconectar.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCONECTAR_H
#define UI_DIALOGCONECTAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogConectar
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_4;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *s1;
    QLineEdit *s2;
    QLineEdit *s3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogConectar)
    {
        if (DialogConectar->objectName().isEmpty())
            DialogConectar->setObjectName(QStringLiteral("DialogConectar"));
        DialogConectar->resize(303, 165);
        widget = new QWidget(DialogConectar);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 0, 285, 159));
        verticalLayout_4 = new QVBoxLayout(widget);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout->addWidget(label_4);

        label_5 = new QLabel(widget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout->addWidget(label_3);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        s1 = new QLineEdit(widget);
        s1->setObjectName(QStringLiteral("s1"));
        s1->setReadOnly(true);

        verticalLayout_2->addWidget(s1);

        s2 = new QLineEdit(widget);
        s2->setObjectName(QStringLiteral("s2"));
        s2->setReadOnly(true);

        verticalLayout_2->addWidget(s2);

        s3 = new QLineEdit(widget);
        s3->setObjectName(QStringLiteral("s3"));
        s3->setReadOnly(true);

        verticalLayout_2->addWidget(s3);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_3);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout_4->addWidget(buttonBox);


        retranslateUi(DialogConectar);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogConectar, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogConectar, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogConectar);
    } // setupUi

    void retranslateUi(QDialog *DialogConectar)
    {
        DialogConectar->setWindowTitle(QApplication::translate("DialogConectar", "Dialog", 0));
        label_4->setText(QApplication::translate("DialogConectar", "Servidor", 0));
        label_5->setText(QApplication::translate("DialogConectar", "Estado", 0));
        label->setText(QApplication::translate("DialogConectar", "LLantas izquierda:", 0));
        label_2->setText(QApplication::translate("DialogConectar", "LLantas derechas:", 0));
        label_3->setText(QApplication::translate("DialogConectar", "Brazo:", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogConectar: public Ui_DialogConectar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCONECTAR_H
