#include "btazo2.h"
#include "ui_btazo2.h"
#include <QDebug>
#include "cliente.h"

btazo2::btazo2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::btazo2)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(0,0,618,498);
    ui->grafico->setScene(scene);

    flechaBase = new QGraphicsPixmapItem;
    QPixmap fl("://flecha");
    flechaBase->setPixmap(fl);
    flechaBase->setTransformOriginPoint(128,128);
    flechaBase->setPos(493-100,373-100);
    flechaBase->setScale(0.3);
    scene->addItem(flechaBase);
    angulo_flecha_bace = 45;
    flechaBase->setRotation(angulo_flecha_bace);

    flechaMuneca = new QGraphicsPixmapItem;
    flechaMuneca->setPixmap(fl);
    flechaMuneca->setTransformOriginPoint(128,128);
    flechaMuneca->setPos(493-200,373-100);
    flechaMuneca->setScale(0.3);
    scene->addItem(flechaMuneca);
    angulo_flecha_muneca = 45;
    flechaMuneca->setRotation(angulo_flecha_muneca);

    bace =new QGraphicsPixmapItem;
    QPixmap ba("://amarillo");
    bace->setPixmap(ba);
    bace->setPos(0,0);

    rojo = new QGraphicsPixmapItem;
    QPixmap ro ("://rojo");
    rojo->setPixmap(ro);
    rojo->setPos(0,0);
    angulo_rojo=0;


    rosado = new QGraphicsPixmapItem;
    QPixmap ros("://rosado");
    rosado->setPixmap(ros);
    rosado->setPos(0,0);
    angulo_rosado = 0;

    azul = new QGraphicsPixmapItem;
    QPixmap az("://azul");
    azul->setPixmap(az);
    azul->setPos(163,-49);
    azul->setTransformOriginPoint(23,84);
    angulo_azul=0;


    grupoBrazo3 = new QGraphicsItemGroup;
    grupoBrazo3->addToGroup(rosado);
    grupoBrazo3->addToGroup(azul);
    grupoBrazo3->setPos(112,-55);
    grupoBrazo3->setTransformOriginPoint(68,69);


    grupoBrazo2 = new QGraphicsItemGroup;
    grupoBrazo2->addToGroup(rojo);
    grupoBrazo2->addToGroup(grupoBrazo3);
    grupoBrazo2->setPos(23,-59);
    grupoBrazo2->setTransformOriginPoint(49,170);

    grupoBrazo1 = new QGraphicsItemGroup;
    grupoBrazo1->addToGroup(bace);
    grupoBrazo1->addToGroup(grupoBrazo2);
    grupoBrazo1->setTransformOriginPoint(0,0);
    positionX = 0;
    positionY = 0;
    grupoBrazo1->setPos(positionX,positionY);
    scale=1;
    grupoBrazo1->setScale(scale);

    scene->addItem(grupoBrazo1);

    grupoBrazo2->setRotation(default_rojo);
    grupoBrazo3->setRotation(default_rosado);
    azul->setRotation(default_azul);






    // brazo fantasma
    bace_fantasma =new QGraphicsPixmapItem;
    QPixmap ba2("://fantasma1");
    bace_fantasma->setPixmap(ba2);
    bace_fantasma->setPos(0,0);

    rojo_fantasma = new QGraphicsPixmapItem;
    QPixmap ro2 ("://fantasma2");
    rojo_fantasma->setPixmap(ro2);
    rojo_fantasma->setPos(0,0);
    angulo_rojo_fantasma=0;


    rosado_fantasma = new QGraphicsPixmapItem;
    QPixmap ros2("://fantasma3");
    rosado_fantasma->setPixmap(ros2);
    rosado_fantasma->setPos(0,0);
    angulo_rosado_fantasma = 0;

    azul_fantasma = new QGraphicsPixmapItem;
    QPixmap az2("://fantasma4");
    azul_fantasma->setPixmap(az2);
    azul_fantasma->setPos(163,-49);
    azul_fantasma->setTransformOriginPoint(23,84);
    angulo_azul_fantasma=0;


    grupoBrazo3_fantasma = new QGraphicsItemGroup;
    grupoBrazo3_fantasma->addToGroup(rosado_fantasma);
    grupoBrazo3_fantasma->addToGroup(azul_fantasma);
    grupoBrazo3_fantasma->setPos(112,-55);
    grupoBrazo3_fantasma->setTransformOriginPoint(68,69);


    grupoBrazo2_fantasma = new QGraphicsItemGroup;
    grupoBrazo2_fantasma->addToGroup(rojo_fantasma);
    grupoBrazo2_fantasma->addToGroup(grupoBrazo3_fantasma);
    grupoBrazo2_fantasma->setPos(23,-59);
    grupoBrazo2_fantasma->setTransformOriginPoint(49,170);

    grupoBrazo1_fantasma = new QGraphicsItemGroup;
    grupoBrazo1_fantasma->addToGroup(bace_fantasma);
    grupoBrazo1_fantasma->addToGroup(grupoBrazo2_fantasma);
    grupoBrazo1_fantasma->setTransformOriginPoint(0,0);
    positionX = 0;
    positionY = 0;
    grupoBrazo1_fantasma->setPos(positionX,positionY);
    scale=1;
    grupoBrazo1_fantasma->setScale(scale);

    scene->addItem(grupoBrazo1_fantasma);

    grupoBrazo2_fantasma->setRotation(default_rojo);
    grupoBrazo3_fantasma->setRotation(default_rosado);
    azul_fantasma->setRotation(default_azul);


}

void btazo2::set_fantasma(int red, int pink, int blue)
{
    angulo_rojo_fantasma = red;
    angulo_rosado_fantasma = pink;
    angulo_azul_fantasma = blue;

    grupoBrazo2_fantasma->setRotation(angulo_rojo_fantasma+default_rojo);
    grupoBrazo3_fantasma->setRotation(angulo_rosado_fantasma+default_rosado);
    azul_fantasma->setRotation(angulo_azul_fantasma+default_azul);
}

void btazo2::rotarVerde(int key)
{
    if(key==86)
        angulo_rojo = angulo_rojo+1;
    if(key==66)
        angulo_rojo = angulo_rojo-1;
    grupoBrazo2->setRotation(angulo_rojo+default_rojo);
}

void btazo2::rotarRojo(int key)
{
    if(key==82)
        angulo_rosado = angulo_rosado +1;
    if(key==84)
        angulo_rosado = angulo_rosado -1;
    grupoBrazo3->setRotation(angulo_rosado+default_rosado);
}

void btazo2::rotarNaranja(int key)
{
    if(key==90)
        angulo_azul = angulo_azul +1;
    if(key==88)
        angulo_azul = angulo_azul -1;
    azul->setRotation(angulo_azul+default_azul);
}

void btazo2::enviarPosicion(char *ip, int estado, int a1, int a2, int a3)
{
    a1 = -a1;
    a2 = -a2;
    a3 = -a3;
    //mandar angulos al servidor
    int sfd = conectarServidor(ip);
    QString comando;
    if(estado==1)
        comando = QString("mover/brazo/normal/%1/%2/%3").arg(a1).arg(a2).arg(a3);
    else
        comando = QString("mover/brazo/auto/%1/%2/%3").arg(a1).arg(a2).arg(a3);
    QByteArray ba = comando.toLocal8Bit();
    const char* linea = ba.data();
    enviarComando((char*)linea,sfd);
}

void btazo2::enviarPosicion2(char *ip)
{
    int sfd = conectarServidor(ip);
    QString comando = QString("mover/brazo/auto/%1/%2/%3/%4/a/%5").arg(-angulo_rojo).arg(-angulo_rosado).arg(-angulo_azul).arg(angulo_flecha_bace).arg(angulo_flecha_muneca);
    QByteArray ba = comando.toLocal8Bit();
    const char* linea = ba.data();
    enviarComando((char*)linea,sfd);
}

void btazo2::posicion_a()
{
    angulo_rojo = 0;
    angulo_rosado=30;
    angulo_azul = 30;

    grupoBrazo2->setRotation(angulo_rojo+default_rojo);
    grupoBrazo3->setRotation(angulo_rosado+default_rosado);
    azul->setRotation(angulo_azul+default_azul);


    enviarPosicion(IP_LLANTAS_DERECHA,2,angulo_rojo,angulo_rosado,angulo_azul);
}

void btazo2::posicion_b()
{
    angulo_rojo = 45;
    angulo_rosado=45;
    angulo_azul =-20;

    grupoBrazo2->setRotation(angulo_rojo+default_rojo);
    grupoBrazo3->setRotation(angulo_rosado+default_rosado);
    azul->setRotation(angulo_azul+default_azul);

    enviarPosicion(IP_LLANTAS_DERECHA,2,angulo_rojo,angulo_rosado,angulo_azul);
}

void btazo2::posicion_c()
{
    angulo_rojo = 45;
    angulo_rosado=45;
    angulo_azul = 20;

    grupoBrazo2->setRotation(angulo_rojo+default_rojo);
    grupoBrazo3->setRotation(angulo_rosado+default_rosado);
    azul->setRotation(angulo_azul+default_azul);

    enviarPosicion(IP_LLANTAS_DERECHA,2,angulo_rojo,angulo_rosado,angulo_azul);
}

void btazo2::posicion_d()
{
    angulo_rojo = -50;
    angulo_rosado= 35;
    angulo_azul = 15;

    grupoBrazo2->setRotation(angulo_rojo+default_rojo);
    grupoBrazo3->setRotation(angulo_rosado+default_rosado);
    azul->setRotation(angulo_azul+default_azul);

    enviarPosicion(IP_LLANTAS_DERECHA,2,angulo_rojo,angulo_rosado,angulo_azul);
}

btazo2::~btazo2()
{
    delete ui;
}



void btazo2::on_horizontalSlider_sliderMoved(int position)
{
    positionX = (position*618)/99;
    grupoBrazo1->setPos(positionX,positionY);
    grupoBrazo1_fantasma->setPos(positionX,positionY);
}

void btazo2::on_verticalSlider_sliderMoved(int position)
{
    positionY = (position*498)/99;
    grupoBrazo1->setPos(positionX,positionY);
    grupoBrazo1_fantasma->setPos(positionX,positionY);
}

void btazo2::on_dial_sliderMoved(int position)
{
    scale = (double)(position)/99;
    grupoBrazo1->setScale(scale);
    grupoBrazo1_fantasma->setScale(scale);
}

int btazo2::darRojo()
{
    return -angulo_rojo;
}

int btazo2::darRosado()
{
    return -angulo_rosado;
}

int btazo2::darAzul()
{
    return -angulo_azul;
}
