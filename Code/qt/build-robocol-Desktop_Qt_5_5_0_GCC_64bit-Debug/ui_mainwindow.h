/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStop;
    QAction *actionConectar;
    QAction *actionDesconectar;
    QAction *actionConectar_2;
    QAction *actionEstado_actual;
    QAction *actionMansar_Fecha;
    QAction *actionMandar_fecha;
    QAction *actionComando;
    QAction *actionCamara;
    QWidget *centralWidget;
    QToolBox *toolBox;
    QWidget *page;
    QWidget *page_3;
    QWidget *Motor3;
    QWidget *page_2;
    QWidget *page_4;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *rojo;
    QSpacerItem *verticalSpacer;
    QLineEdit *rosado;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *azul;
    QTabWidget *Inferior;
    QWidget *tab;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QPlainTextEdit *plainTextEdit;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QWidget *contenedorCuerpo;
    QWidget *tab_4;
    QWidget *contenedorBrazo;
    QMenuBar *menuBar;
    QMenu *menuStart;
    QMenu *menuAcciones;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(833, 693);
        actionStop = new QAction(MainWindow);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionConectar = new QAction(MainWindow);
        actionConectar->setObjectName(QStringLiteral("actionConectar"));
        actionDesconectar = new QAction(MainWindow);
        actionDesconectar->setObjectName(QStringLiteral("actionDesconectar"));
        actionConectar_2 = new QAction(MainWindow);
        actionConectar_2->setObjectName(QStringLiteral("actionConectar_2"));
        actionEstado_actual = new QAction(MainWindow);
        actionEstado_actual->setObjectName(QStringLiteral("actionEstado_actual"));
        actionMansar_Fecha = new QAction(MainWindow);
        actionMansar_Fecha->setObjectName(QStringLiteral("actionMansar_Fecha"));
        actionMandar_fecha = new QAction(MainWindow);
        actionMandar_fecha->setObjectName(QStringLiteral("actionMandar_fecha"));
        actionComando = new QAction(MainWindow);
        actionComando->setObjectName(QStringLiteral("actionComando"));
        actionCamara = new QAction(MainWindow);
        actionCamara->setObjectName(QStringLiteral("actionCamara"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        toolBox = new QToolBox(centralWidget);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        toolBox->setGeometry(QRect(0, 0, 181, 501));
        toolBox->setAutoFillBackground(false);
        toolBox->setFrameShape(QFrame::NoFrame);
        toolBox->setFrameShadow(QFrame::Plain);
        toolBox->setLineWidth(1);
        toolBox->setMidLineWidth(0);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        page->setGeometry(QRect(0, 0, 181, 346));
        toolBox->addItem(page, QStringLiteral("Motor 1"));
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        page_3->setGeometry(QRect(0, 0, 181, 346));
        toolBox->addItem(page_3, QStringLiteral("Motor 2"));
        Motor3 = new QWidget();
        Motor3->setObjectName(QStringLiteral("Motor3"));
        Motor3->setGeometry(QRect(0, 0, 181, 346));
        toolBox->addItem(Motor3, QStringLiteral("Motor 3"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 181, 346));
        toolBox->addItem(page_2, QStringLiteral("Motor 4"));
        toolBox->setItemText(toolBox->indexOf(page_2), QStringLiteral("Motor 4"));
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        page_4->setGeometry(QRect(0, 0, 181, 346));
        layoutWidget = new QWidget(page_4);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 148, 167));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        rojo = new QLineEdit(layoutWidget);
        rojo->setObjectName(QStringLiteral("rojo"));
        rojo->setStyleSheet(QLatin1String("color:#FF0000;\n"
""));
        rojo->setReadOnly(true);

        verticalLayout->addWidget(rojo);

        verticalSpacer = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        rosado = new QLineEdit(layoutWidget);
        rosado->setObjectName(QStringLiteral("rosado"));
        rosado->setStyleSheet(QStringLiteral("color:#FF3399;"));
        rosado->setReadOnly(true);

        verticalLayout->addWidget(rosado);

        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        azul = new QLineEdit(layoutWidget);
        azul->setObjectName(QStringLiteral("azul"));
        azul->setStyleSheet(QStringLiteral("color: blue;"));
        azul->setReadOnly(true);

        verticalLayout->addWidget(azul);

        toolBox->addItem(page_4, QStringLiteral("Brazo"));
        Inferior = new QTabWidget(centralWidget);
        Inferior->setObjectName(QStringLiteral("Inferior"));
        Inferior->setGeometry(QRect(0, 500, 811, 131));
        Inferior->setTabShape(QTabWidget::Rounded);
        Inferior->setElideMode(Qt::ElideNone);
        Inferior->setUsesScrollButtons(true);
        Inferior->setDocumentMode(false);
        Inferior->setTabsClosable(false);
        Inferior->setMovable(false);
        Inferior->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        scrollArea = new QScrollArea(tab);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 811, 101));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 809, 99));
        plainTextEdit = new QPlainTextEdit(scrollAreaWidgetContents);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(0, 0, 811, 101));
        scrollArea->setWidget(scrollAreaWidgetContents);
        Inferior->addTab(tab, QString());
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(180, 0, 631, 531));
        tabWidget_2->setTabPosition(QTabWidget::North);
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        contenedorCuerpo = new QWidget(tab_3);
        contenedorCuerpo->setObjectName(QStringLiteral("contenedorCuerpo"));
        contenedorCuerpo->setGeometry(QRect(0, 0, 521, 431));
        contenedorCuerpo->setStyleSheet(QStringLiteral(""));
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        contenedorBrazo = new QWidget(tab_4);
        contenedorBrazo->setObjectName(QStringLiteral("contenedorBrazo"));
        contenedorBrazo->setGeometry(QRect(0, 0, 621, 501));
        tabWidget_2->addTab(tab_4, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 833, 25));
        menuStart = new QMenu(menuBar);
        menuStart->setObjectName(QStringLiteral("menuStart"));
        menuAcciones = new QMenu(menuBar);
        menuAcciones->setObjectName(QStringLiteral("menuAcciones"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuStart->menuAction());
        menuBar->addAction(menuAcciones->menuAction());
        menuStart->addAction(actionConectar_2);
        menuStart->addAction(actionDesconectar);
        menuStart->addSeparator();
        menuStart->addAction(actionEstado_actual);
        menuAcciones->addAction(actionMandar_fecha);
        menuAcciones->addAction(actionComando);
        menuAcciones->addAction(actionCamara);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(4);
        Inferior->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Robocol", 0));
        actionStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        actionConectar->setText(QApplication::translate("MainWindow", "Conectar", 0));
        actionDesconectar->setText(QApplication::translate("MainWindow", "Desconectar", 0));
        actionConectar_2->setText(QApplication::translate("MainWindow", "Conectar", 0));
        actionEstado_actual->setText(QApplication::translate("MainWindow", "Estado actual", 0));
        actionMansar_Fecha->setText(QApplication::translate("MainWindow", "Mandar_fecha", 0));
        actionMandar_fecha->setText(QApplication::translate("MainWindow", "Mandar_fecha", 0));
        actionComando->setText(QApplication::translate("MainWindow", "comando", 0));
        actionCamara->setText(QApplication::translate("MainWindow", "camara", 0));
        toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("MainWindow", "Motor 1", 0));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("MainWindow", "Motor 2", 0));
        toolBox->setItemText(toolBox->indexOf(Motor3), QApplication::translate("MainWindow", "Motor 3", 0));
        toolBox->setItemText(toolBox->indexOf(page_4), QApplication::translate("MainWindow", "Brazo", 0));
        Inferior->setTabText(Inferior->indexOf(tab), QApplication::translate("MainWindow", "Consola", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("MainWindow", "Chasis", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("MainWindow", "Brazo", 0));
        menuStart->setTitle(QApplication::translate("MainWindow", "Inicio", 0));
        menuAcciones->setTitle(QApplication::translate("MainWindow", "Acciones", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
