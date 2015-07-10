#include "cuerpo.h"
#include "ui_cuerpo.h"
#include <QDebug>
#include "cliente.h"



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
}

void cuerpo::adelante()
{
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

    QString comando = QString("mover/traccion/f/%1").arg(pwm2);
    QByteArray ba = comando.toLocal8Bit();
    const char* linea = ba.data();
    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando((char*)linea,sfd);
}

void cuerpo::atras()
{
    if(pwm1!=0)
        pwm1 =pwm1-1 ;
    if(pwm2!=0)
        pwm2 =pwm2-1;
    if(pwm3!=0)
        pwm3 =pwm3-1;
    if(pwm4!=0)
        pwm4 =pwm4-1;

    ui->izq_sup->display(pwm1);
    ui->dere_sup->display(pwm2);
    ui->izq_inf->display(pwm3);
    ui->dere_inf->display(pwm4);

    QString comando = QString("mover/traccion/b/%1").arg(pwm2);
    QByteArray ba = comando.toLocal8Bit();
    const char* linea = ba.data();
    int sfd = conectarServidor(IP_LLANTAS_DERECHA);
    enviarComando((char*)linea,sfd);

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
