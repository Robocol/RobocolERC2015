#include "cuerpo.h"
#include "ui_cuerpo.h"
#include <QDebug>
#include "cliente.h"
#include <QKeyEvent>



int g2 = 0;

cuerpo::cuerpo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cuerpo)
{
    ui->setupUi(this);


    QSize s = ui->grafico->size();
    x_plane = s.width();
    y_plane = s.height();
    scene = new QGraphicsScene(0,0,x_plane,y_plane);
    ui->grafico->setScene(scene);

    antena = new QGraphicsPixmapItem;
    QPixmap a("://imagenes/Antena.png");
    antena->setPixmap(a);
    antena->setPos((x_plane/4)+20,(y_plane/4)-8);
    antena->setTransformOriginPoint(QPoint(22,87));
    scene->addItem(antena);

    pwm1 = 0;
    pwm2 = 0;
    pwm3 = 0;
    pwm4 = 0;

    espera = new QTimer(this);
    connect(espera, SIGNAL(timeout()),this, SLOT(enviarDireccion()));
    entro = false;

}


void cuerpo::enviarDireccion()
{
    QString comando;
    if(pwm2<0)
        comando = QString("mover/traccion/b/%1").arg(pwm2*-1);
    else
        comando = QString("mover/traccion/f/%1").arg(pwm2);



    QByteArray ba = comando.toLocal8Bit();
    const char* linea = ba.data();
    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando((char*)linea,sfd);
    cerrarConexion(sfd);

    int sfd2 = conectarServidor(IP_LLANTAS_IZQUIERDA);
    enviarComando((char*)linea,sfd2);
    cerrarConexion(sfd2);

    qDebug()<< "envio comando de movimiento";
    entro = false;
    espera->stop();
    time = 200;
}

void cuerpo::adelante()
{
    if(!entro)
    {
        qDebug()<< "empezo a contar";
        espera->start(time);
        entro = true;
    }


    if(pwm1!=240)
        pwm1 =pwm1+1 ;
    if(pwm2!=240)
        pwm2 =pwm2+1;
    if(pwm3!=240)
        pwm3 =pwm3+1;
    if(pwm4!=240)
        pwm4 =pwm4+1;

    ui->izq_sup->display(pwm1);
    ui->dere_sup->display(pwm2);
    ui->izq_inf->display(pwm3);
    ui->dere_inf->display(pwm4);


//    QString comando;
//    if(pwm2<0)
//        comando = QString("mover/traccion/b/%1").arg(pwm2*-1);
//    else
//        comando = QString("mover/traccion/f/%1").arg(pwm2);



//    QByteArray ba = comando.toLocal8Bit();
//    const char* linea = ba.data();
//    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
//    enviarComando((char*)linea,sfd);
//    cerrarConexion(sfd);

//    int sfd2 = conectarServidor(IP_LLANTAS_IZQUIERDA);
//    enviarComando((char*)linea,sfd2);
//    cerrarConexion(sfd2);
}

void cuerpo::atras()
{

    if(!entro)
    {
        qDebug()<< "empezo a contar";
        espera->start(time);
        entro = true;
    }

    if(pwm1!=-244)
        pwm1 =pwm1-1 ;
    if(pwm2!=-244)
        pwm2 =pwm2-1;
    if(pwm3!=-244)
        pwm3 =pwm3-1;
    if(pwm4!=-244)
        pwm4 =pwm4-1;

    ui->izq_sup->display(pwm1);
    ui->dere_sup->display(pwm2);
    ui->izq_inf->display(pwm3);
    ui->dere_inf->display(pwm4);


//    QString comando;
//    if(pwm2<0)
//        comando = QString("mover/traccion/b/%1").arg(pwm2*-1);
//    else
//        comando = QString("mover/traccion/f/%1").arg(pwm2);

//    QByteArray ba = comando.toLocal8Bit();
//    const char* linea = ba.data();

//    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
//    enviarComando((char*)linea,sfd);
//    cerrarConexion(sfd);

//    int sfd2 = conectarServidor(IP_LLANTAS_IZQUIERDA);
//    enviarComando((char*)linea,sfd2);
//    cerrarConexion(sfd2);

}

void cuerpo::parar()
{
    pwm1 = 0;
    pwm2 = 0;
    pwm3 = 0;
    pwm4 = 0;
    ui->izq_sup->display(pwm1);
    ui->dere_sup->display(pwm2);
    ui->izq_inf->display(pwm3);
    ui->dere_inf->display(pwm4);
}

void cuerpo::girarDerecha()
{
    qDebug()<<"entro a giro r";
    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando("mover/traccion/r/220/0",sfd);
    cerrarConexion(sfd);

    int sfd2 = conectarServidor(IP_LLANTAS_IZQUIERDA);
    enviarComando("mover/traccion/r/220/0",sfd2);
    cerrarConexion(sfd2);
}

void cuerpo::girarIzquierda()
{
    qDebug()<<"entro a giro l";
    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando("mover/traccion/l/200/0",sfd);
    cerrarConexion(sfd);

    int sfd2 = conectarServidor(IP_LLANTAS_IZQUIERDA);
    enviarComando("mover/traccion/l/200/0",sfd2);
    cerrarConexion(sfd2);
}

cuerpo::~cuerpo()
{
    delete ui;
}

void cuerpo::on_pushButton_clicked()
{
    g2+=10;
    antena->setRotation(g2);
}

