#-------------------------------------------------
#
# Project created by QtCreator 2015-04-19T20:48:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = robocol
TEMPLATE = app

CONFIG += console
SOURCES += main.cpp\
        mainwindow.cpp \
    cuerpo.cpp \
    brazo.cpp \
    btazo2.cpp \
    cliente.cpp \
    dialogconectar.cpp \
    dialog_desconectar.cpp \
    menu.cpp \
    giro.cpp \
    qmpwidget.cpp

HEADERS  += mainwindow.h \
    comunicacion.h \
    cuerpo.h \
    brazo.h \
    btazo2.h \
    cliente.h \
    dialogconectar.h \
    dialog_desconectar.h \
    menu.h \
    giro.h \
    qmpyuvreader.h \
    qmpwidget.h

FORMS    += mainwindow.ui \
    cuerpo.ui \
    brazo.ui \
    btazo2.ui \
    dialogconectar.ui \
    dialog_desconectar.ui \
    menu.ui \
    giro.ui

RESOURCES += \
    rsc.qrc
QT += webkitwidgets
