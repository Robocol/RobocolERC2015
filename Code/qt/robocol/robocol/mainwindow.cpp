#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSize>
#include <QDebug>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPoint>
#include <QPen>
#include <QDateTime>
#include <QKeyEvent>
#include "dialogconectar.h"
#include "dialog_desconectar.h"
#include "menu.h"

#include"cliente.h"



int g = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "inicializacion";


    ventanaCuerpo = new cuerpo(ui->contenedorCuerpo);
    ventanaBrazo = new btazo2(ui->contenedorBrazo);
    ui->contenedorCuerpo->resize(622,528);
    ui->scrollArea->installEventFilter(this);
    QFile file("in.txt");
    QTextStream in(&file);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    in << "hola\n";
    in << "hola1\n";
    file.close();

    QTextCursor cursor = ui->plainTextEdit->textCursor();
    qDebug() << cursor.position();
    ui->plainTextEdit->insertPlainText("Mensage>> ");
    cursor = ui->plainTextEdit->textCursor();
    qDebug() << cursor.columnNumber();

    // conexion de los votones de menu con el mundo
    connect(ui->actionEstado_actual,SIGNAL(triggered()),this,SLOT(estadoActual()));
    connect(ui->actionConectar_2,SIGNAL(triggered()),this,SLOT(conectar()));
    connect(ui->actionDesconectar,SIGNAL(triggered()),this,SLOT(desconectar()));
    connect(ui->actionMandar_fecha,SIGNAL(triggered()),this,SLOT(mandarFecha()));


    //inicializacion de la estructura de estados
    estado.brazo = 0;
    estado.llantas_der=0;
    estado.llantas_izq=0;

    //timer para pedir reporte
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(slotTimer()));
    timer->start(2000);


}

/**
 * @brief MainWindow::slotTimer
 * slot cuando pasa el tiempo programado
 */
void MainWindow::slotTimer()
{
    ui->plainTextEdit->insertPlainText("leer archivo brazo\nMensage>> ");

    // diagnostico brazo
    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando("diagnosticar/brazo",sfd);
    recibirAArchivo(sfd,"brazo.txt");
    QFile file("brazo.txt");
    QTextStream in(&file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"error al leer el archivo";

    QString palabra = "";
    in>>palabra;
    QStringList list = palabra.split(":");
    QString comando = list.at(0);
    if(QString::compare(comando,"brazo")==0)
    {
        QStringList list2 = list.at(1).split(",");
        ventanaBrazo->set_fantasma(list2.at(0).toInt(),list2.at(1).toInt(),list2.at(2).toInt());
    }

    timer->stop();


}

/**
 * @brief MainWindow::keyPressEvent
 * @param event letra oprimida
 * slot que maneja los eventos al oprimir una tecla
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<event->key();

    // letra p entrar al menu de mando
    if(event->key()==80)
    {
        menu control;
        control.setModal(true);
        control.exec();
        funcion=control.valor;
    }

    if(funcion==2)
    {
        //letra b
        if(event->key()==66)
        {
            ventanaCuerpo->adelante();
        }

        //letra v
        if(event->key()==86)
        {
            ventanaCuerpo->atras();
        }
    }
    else if(funcion==1)
    {
        //letra v o b
        if(event->key()==86 || event->key()==66)
        {
            ventanaBrazo->rotarVerde(event->key());
            ui->rojo->setText(QString::number(ventanaBrazo->darRojo()));
        }
        //letra r o t
        if(event->key()==82 || event->key()==84)
        {
            ventanaBrazo->rotarRojo(event->key());
            ui->rosado->setText(QString::number(ventanaBrazo->darRosado()));
        }
        //letra z o x
        if(event->key()==90 || event->key()==88)
        {
            ventanaBrazo->rotarNaranja(event->key());
            ui->azul->setText(QString::number(ventanaBrazo->darAzul()));
        }
        //letra l
        if(event->key()==76)
        {
            ventanaBrazo->posicion_a();
            ui->rojo->setText(QString::number(ventanaBrazo->darRojo()));
            ui->rosado->setText(QString::number(ventanaBrazo->darRosado()));
            ui->azul->setText(QString::number(ventanaBrazo->darAzul()));
        }
        //letra e
        if(event->key()==69)
        {
            ventanaBrazo->posicion_b();
            ui->rojo->setText(QString::number(ventanaBrazo->darRojo()));
            ui->rosado->setText(QString::number(ventanaBrazo->darRosado()));
            ui->azul->setText(QString::number(ventanaBrazo->darAzul()));
        }
        //letra j
        if(event->key()==74)
        {
            ventanaBrazo->posicion_c();
            ui->rojo->setText(QString::number(ventanaBrazo->darRojo()));
            ui->rosado->setText(QString::number(ventanaBrazo->darRosado()));
            ui->azul->setText(QString::number(ventanaBrazo->darAzul()));
        }
        //letra i
        if(event->key()==73)
        {
            ventanaBrazo->posicion_d();
            ui->rojo->setText(QString::number(ventanaBrazo->darRojo()));
            ui->rosado->setText(QString::number(ventanaBrazo->darRosado()));
            ui->azul->setText(QString::number(ventanaBrazo->darAzul()));
        }
        //letra y
        if(event->key()==89)
        {
            ventanaBrazo->enviarPosicion2(IP_LLANTAS_DERECHA);
            ui->plainTextEdit->insertPlainText("Angulos enviado\n");
            ui->plainTextEdit->insertPlainText("Mensage>> ");
        }
    }



    //    if(event->key() == 16777220)
    //    {
    //        QString te = ui->textEdit->toPlainText();
    //        QStringList list = te.split("\n");
    //        for(int i=0;i<list.size();i++)
    //        {
    //            QString te2 = list.at(i);
    //            qDebug() << te2;
    //        }
    //    }
}

/**
 * @brief MainWindow::estadoActual
 * slor para realizar peticion del estado actual de las tarjetas
 */
void MainWindow::estadoActual()
{

    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando("estado",sfd);
    char LenStr[INT_LEN];
    readLine(sfd, LenStr, INT_LEN);
    QString respuesta = LenStr;
    qDebug()<<respuesta;
    estado.llantas_der = respuesta.toInt();

    //    sfd = conectarServidor(IP_LLANTAS_IZQUIERDA);
    //    enviarComando("estado",sfd);
    //    char LenStr2[INT_LEN];
    //    readLine(sfd, LenStr2, INT_LEN);
    //    respuesta = LenStr2;
    //    estado.llantas_izq = respuesta.toInt();

    //    sfd = conectarServidor(IP_BRAZO);
    //    enviarComando("estado",sfd);
    //    char LenStr3[INT_LEN];
    //    readLine(sfd, LenStr3, INT_LEN);
    //    respuesta = LenStr3;
    //    estado.brazo = respuesta.toInt();

    DialogConectar diag;
    diag.cambiarDer(estado.llantas_der);
    diag.cambiarIzq(estado.llantas_izq);
    diag.cambiarBrazo(estado.brazo);
    diag.setModal(true);
    diag.exec();
}


/**
 * @brief MainWindow::conectarServidores
 * @param ip direccion de la tarjeta que se desea conectar
 * @return -1 si no se pudo conectar. -2 si no se puedo enviar peticion de conectar. -3 no hubo repsuesta del servidor
 * conexion al servidor
 */
int MainWindow::conectarServidores(char *ip)
{
    int sfd = conectarServidor(ip);
    if(sfd==-1)
        return -1;
    if(enviarComando("conectar",sfd)==-1)
        return -2;

    char LenStr[INT_LEN];

    if (readLine(sfd, LenStr, INT_LEN) <= 0) {
        cerrarConexion(sfd);
        return -3;
        qDebug()<<"No hay fin de línea\n";
    }

    QString estado = LenStr;
    return estado.toInt();
}

/**
 * @brief MainWindow::conectar
 * slot para conectar al sarvidor
 */
void MainWindow::conectar()
{
    qDebug() << "conectar";
    int retorno = conectarServidores("192.168.0.103");
    qDebug() << "conectar2";
    switch (retorno) {
    case -1:
        qDebug()<<"no se pudo conectar a llantas derecha";
        break;
    case -2:
        qDebug()<<"no se pudo enviar comando a llantas derecha";
        break;
    case -3:
        qDebug()<<"no se pudo recibir respuesta de llantas derecha";
        break;
    default:
        estado.llantas_der=retorno;
        break;
    }

    //   retorno = conectarServidores(IP_LLANTAS_IZQUIERDA);

    //   switch (retorno) {DialogConectar diag;
    //   case -1:
    //       qDebug()<<"no se pudo conectar a llantas izquierda";
    //       break;
    //   case -2:
    //       qDebug()<<"no se pudo enviar comando a llantas izquierda";
    //       break;
    //   case -3:
    //       qDebug()<<"no se pudo recibir respuesta de llantas izquierda";
    //       break;
    //   default:
    //       estado.llantas_izq=retorno;
    //       break;recibirAArchivo
    //   }

    //   retorno = conectarServidores(IP_BRAZO);
    //   switch (retorno) {
    //   case -1:
    //       qDebug()<<"no se pudo conectar a brazo";
    //       break;
    //   case -2:
    //       qDebug()<<"no se pudo enviar comando a brazo";
    //       break;
    //   case -3:
    //       qDebug()<<"no se pudo recibir respuesta de brazo";
    //       break;
    //   default:
    //       estado.brazo=retorno;
    //       break;
    //   }

    DialogConectar diag;
    diag.cambiarDer(estado.llantas_der);
    diag.setModal(true);
    diag.exec();

}

/**
 * @brief MainWindow::desconectar
 * slot para desconectarse del servidor y apagarlo
 */
void MainWindow::desconectar()
{
    Dialog_desconectar descon;
    descon.setModal(true);
    descon.exec();

    if(descon.dere!=-1)
    {
        estado.llantas_der = descon.dere;
        int sfd = conectarServidor(IP_LLANTAS_DERECHA);
        enviarComando("desconectar",sfd);
    }

    if(descon.izqui!=-1)
    {
        estado.llantas_izq = descon.izqui;
        int sfd = conectarServidor(IP_LLANTAS_IZQUIERDA);
        enviarComando("desconectar",sfd);
    }

    if(descon.braz!=-1)
    {
        estado.brazo = descon.braz;
        int sfd = conectarServidor(IP_BRAZO);
        enviarComando("desconectar",sfd);
    }


}

/**
 * @brief MainWindow::mandarFecha
 * Envia la fecha actual al servidor
 */
void MainWindow::mandarFecha()
{
    // obtener fecha del sistrema
    QDateTime tiempo = QDateTime::currentDateTime();
    int year =tiempo.date().year();
    int month =tiempo.date().month();
    int day = tiempo.date().day();

    int hour = tiempo.time().hour();
    int minute = tiempo.time().minute();
    int second = tiempo.time().second();


    QString printable = QString("%1-%2-%3-%4-%5-%6").arg(year).arg(month).arg(day).arg(hour).arg(minute).arg(second);
    QByteArray ba = printable.toLocal8Bit();
    const char* linea = ba.data();
    qDebug()<< printable;

    //mandar fecha al servidor
    int sfd = conectarServidor("192.168.0.103");
    enviarComando("fecha",sfd);
    char LenStr[INT_LEN];
    readLine(sfd, LenStr, INT_LEN);
    QString respuesta = LenStr;
    qDebug()<<respuesta;
    enviarComando((char*)linea,sfd);
}

/**
 * @brief MainWindow::cambiarEstado
 * @param izq estado nuevo de llantas izquierdas
 * @param dere estado nuevo de llantas derechas
 * @param bra estado nuevo de brazo
 * Cambia el estado de la estructura
 */
void MainWindow::cambiarEstado(int izq,int dere,int bra)
{
    estado.llantas_izq = izq;
    estado.llantas_der = dere;
    estado.brazo = bra;
}


/**
 * @brief MainWindow::recibirAArchivo
 * @param sfd. direccion del proceso
 * @return -1 fallo. 0 funciono
 */
int MainWindow::recibirAArchivo(int sfd, char *nombre)
{
    unsigned char buffer_temp[2000];
    unsigned char buffer[2000];
    int tam =0;
    int acum = 0;
    int numRead=0;
    char LenStr[INT_LEN];
    int outputFd, openFlags;
    mode_t filePerms;

    readLine(sfd, LenStr, INT_LEN);
    tam = atoi(LenStr);
    enviarComando("ok",sfd);

    while(acum<tam)
    {
        numRead = read(sfd, buffer_temp, 2000);
        if (numRead == -1){
            qDebug()<<"Error de lectura";
            return -1;
        }
        memcpy(buffer+acum,buffer_temp,numRead);
        acum=numRead+acum;
    }

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    outputFd = open(nombre,openFlags,filePerms);
    if(outputFd == -1)
    {
        qDebug()<<"Error al abrir archivo";
        return -1;
    }
    if(write(outputFd,buffer,numRead)!=numRead)
    {
        qDebug()<<"Error al escribir el archivo";
        return -1;
    }
    ::close(outputFd);
    return 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

