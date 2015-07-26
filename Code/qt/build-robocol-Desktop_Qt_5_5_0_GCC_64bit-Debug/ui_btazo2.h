/********************************************************************************
** Form generated from reading UI file 'btazo2.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BTAZO2_H
#define UI_BTAZO2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_btazo2
{
public:
    QGraphicsView *grafico;
    QSlider *horizontalSlider;
    QSlider *verticalSlider;
    QDial *dial;

    void setupUi(QWidget *btazo2)
    {
        if (btazo2->objectName().isEmpty())
            btazo2->setObjectName(QStringLiteral("btazo2"));
        btazo2->resize(621, 501);
        grafico = new QGraphicsView(btazo2);
        grafico->setObjectName(QStringLiteral("grafico"));
        grafico->setGeometry(QRect(0, 0, 621, 501));
        grafico->setStyleSheet(QStringLiteral("background: transparent;"));
        horizontalSlider = new QSlider(btazo2);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(10, 460, 541, 29));
        horizontalSlider->setOrientation(Qt::Horizontal);
        verticalSlider = new QSlider(btazo2);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setGeometry(QRect(580, 10, 29, 421));
        verticalSlider->setOrientation(Qt::Vertical);
        dial = new QDial(btazo2);
        dial->setObjectName(QStringLiteral("dial"));
        dial->setGeometry(QRect(560, 430, 50, 64));

        retranslateUi(btazo2);

        QMetaObject::connectSlotsByName(btazo2);
    } // setupUi

    void retranslateUi(QWidget *btazo2)
    {
        btazo2->setWindowTitle(QApplication::translate("btazo2", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class btazo2: public Ui_btazo2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BTAZO2_H
