/********************************************************************************
** Form generated from reading UI file 'cuerpo.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUERPO_H
#define UI_CUERPO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_cuerpo
{
public:
    QLabel *inf_isq;
    QGraphicsView *grafico;
    QLabel *inf_der;
    QPushButton *pushButton;
    QLabel *sup_der;
    QLabel *cuerpo_2;
    QLabel *sup_isq;
    QLCDNumber *izq_sup;
    QLCDNumber *dere_sup;
    QLCDNumber *izq_inf;
    QLCDNumber *dere_inf;

    void setupUi(QWidget *cuerpo)
    {
        if (cuerpo->objectName().isEmpty())
            cuerpo->setObjectName(QStringLiteral("cuerpo"));
        cuerpo->resize(644, 508);
        inf_isq = new QLabel(cuerpo);
        inf_isq->setObjectName(QStringLiteral("inf_isq"));
        inf_isq->setGeometry(QRect(153, 328, 54, 91));
        inf_isq->setCursor(QCursor(Qt::PointingHandCursor));
        inf_isq->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/llantaI.png")));
        inf_isq->setScaledContents(true);
        grafico = new QGraphicsView(cuerpo);
        grafico->setObjectName(QStringLiteral("grafico"));
        grafico->setEnabled(true);
        grafico->setGeometry(QRect(290, 270, 222, 222));
        grafico->setStyleSheet(QLatin1String("background: transparent;\n"
"border: none;\n"
"\n"
""));
        grafico->setLineWidth(0);
        inf_der = new QLabel(cuerpo);
        inf_der->setObjectName(QStringLiteral("inf_der"));
        inf_der->setGeometry(QRect(458, 328, 54, 91));
        inf_der->setCursor(QCursor(Qt::PointingHandCursor));
        inf_der->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/llantaD.png")));
        inf_der->setScaledContents(true);
        pushButton = new QPushButton(cuerpo);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 430, 99, 27));
        sup_der = new QLabel(cuerpo);
        sup_der->setObjectName(QStringLiteral("sup_der"));
        sup_der->setGeometry(QRect(458, 16, 54, 91));
        sup_der->setCursor(QCursor(Qt::PointingHandCursor));
        sup_der->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/llantaD.png")));
        sup_der->setScaledContents(true);
        cuerpo_2 = new QLabel(cuerpo);
        cuerpo_2->setObjectName(QStringLiteral("cuerpo_2"));
        cuerpo_2->setGeometry(QRect(187, 16, 290, 415));
        cuerpo_2->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/Interfaz.png")));
        cuerpo_2->setScaledContents(true);
        sup_isq = new QLabel(cuerpo);
        sup_isq->setObjectName(QStringLiteral("sup_isq"));
        sup_isq->setGeometry(QRect(153, 16, 54, 91));
        sup_isq->setCursor(QCursor(Qt::PointingHandCursor));
        sup_isq->setPixmap(QPixmap(QString::fromUtf8(":/imagenes/llantaI.png")));
        sup_isq->setScaledContents(true);
        izq_sup = new QLCDNumber(cuerpo);
        izq_sup->setObjectName(QStringLiteral("izq_sup"));
        izq_sup->setGeometry(QRect(40, 20, 81, 71));
        dere_sup = new QLCDNumber(cuerpo);
        dere_sup->setObjectName(QStringLiteral("dere_sup"));
        dere_sup->setGeometry(QRect(530, 30, 81, 71));
        izq_inf = new QLCDNumber(cuerpo);
        izq_inf->setObjectName(QStringLiteral("izq_inf"));
        izq_inf->setGeometry(QRect(40, 330, 81, 71));
        dere_inf = new QLCDNumber(cuerpo);
        dere_inf->setObjectName(QStringLiteral("dere_inf"));
        dere_inf->setGeometry(QRect(530, 330, 81, 71));

        retranslateUi(cuerpo);

        QMetaObject::connectSlotsByName(cuerpo);
    } // setupUi

    void retranslateUi(QWidget *cuerpo)
    {
        cuerpo->setWindowTitle(QApplication::translate("cuerpo", "Form", 0));
        inf_isq->setText(QString());
        inf_der->setText(QString());
        pushButton->setText(QApplication::translate("cuerpo", "PushButton", 0));
        sup_der->setText(QString());
        cuerpo_2->setText(QString());
        sup_isq->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class cuerpo: public Ui_cuerpo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUERPO_H
