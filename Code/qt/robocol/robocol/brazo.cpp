#include "brazo.h"
#include "ui_brazo.h"

brazo::brazo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::brazo)
{
    ui->setupUi(this);

    sceneX = 621;
    sceneY = 501;
    scene = new QGraphicsScene(0,0,sceneX,sceneY);
    ui->grafico->setScene(scene);


    pos_bace = new QPoint(10,215);
    pos_centro_bace = new QPoint(pos_bace->x()+54,pos_bace->y()+82);
    bace = new  QGraphicsPixmapItem;
    QPixmap ba("://azul");
    bace->setPixmap(ba);
    bace->setPos(QPointF(*pos_bace));
    scene->addItem(bace);


    verde = new QGraphicsPixmapItem;
    QPixmap ve("://verde");
    verde->setPixmap(ve);
    verde->setPos(0,0);
    anguloVerde = 0;

    rojo = new QGraphicsPixmapItem;
    QPixmap ro("://rojo");
    rojo->setPixmap(ro);
    rojo->setPos(0,0);
    anguloRojo =0;

    naranja = new QGraphicsPixmapItem;
    QPixmap na("://naranja");
    naranja->setPixmap(na);
    naranja->setPos(95,36-80);
    naranja->setTransformOriginPoint(47,68);

    grupoBrazo2 = new QGraphicsItemGroup;
    grupoBrazo2->addToGroup(rojo);
    grupoBrazo2->addToGroup(naranja);
    grupoBrazo2->setPos(85,-35);
    grupoBrazo2->setTransformOriginPoint(49,50);

    grupoBrazo1 = new QGraphicsItemGroup;
    grupoBrazo1->addToGroup(verde);
    grupoBrazo1->addToGroup(grupoBrazo2);
    grupoBrazo1->setPos(30,165);
    grupoBrazo1->setTransformOriginPoint(35,122);
    scene->addItem(grupoBrazo1);
}

void brazo::rotarVerde(int key)
{
    if(key==86)
        anguloVerde = anguloVerde+1;
    if(key==66)
        anguloVerde = anguloVerde-1;
    grupoBrazo1->setRotation(anguloVerde);
}

void brazo::rotarRojo(int key)
{
    if(key==82)
        anguloRojo = anguloRojo +1;
    if(key==84)
        anguloRojo = anguloRojo -1;
    grupoBrazo2->setRotation(anguloRojo);
}

void brazo::rotarNaranja(int key)
{
    if(key==90)
        anguloNaranja = anguloNaranja +1;
    if(key==88)
        anguloNaranja = anguloNaranja -1;
    naranja->setRotation(anguloNaranja);
}



brazo::~brazo()
{
    delete ui;
}
