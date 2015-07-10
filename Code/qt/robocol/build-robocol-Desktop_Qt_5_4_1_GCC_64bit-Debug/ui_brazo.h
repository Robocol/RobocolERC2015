/********************************************************************************
** Form generated from reading UI file 'brazo.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BRAZO_H
#define UI_BRAZO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_brazo
{
public:
    QGraphicsView *grafico;

    void setupUi(QWidget *brazo)
    {
        if (brazo->objectName().isEmpty())
            brazo->setObjectName(QStringLiteral("brazo"));
        brazo->resize(621, 501);
        grafico = new QGraphicsView(brazo);
        grafico->setObjectName(QStringLiteral("grafico"));
        grafico->setEnabled(true);
        grafico->setGeometry(QRect(0, 0, 620, 501));
        grafico->setStyleSheet(QLatin1String("background: transparent;\n"
""));

        retranslateUi(brazo);

        QMetaObject::connectSlotsByName(brazo);
    } // setupUi

    void retranslateUi(QWidget *brazo)
    {
        brazo->setWindowTitle(QApplication::translate("brazo", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class brazo: public Ui_brazo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BRAZO_H
