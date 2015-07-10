#ifndef BRAZO_H
#define BRAZO_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class brazo;
}

class brazo : public QWidget
{
    Q_OBJECT

public:
    explicit brazo(QWidget *parent = 0);
    void rotarVerde(int key);
    void rotarRojo(int key);
    void rotarNaranja(int key);
    ~brazo();

private:
    Ui::brazo *ui;

    QGraphicsScene* scene;
    int sceneX;
    int sceneY;

    QGraphicsPixmapItem* bace;
    QPoint* pos_bace;
    QPoint* pos_centro_bace;

    QGraphicsPixmapItem* verde;
    int anguloVerde;
    QGraphicsPixmapItem* rojo;
    int anguloRojo;
    QGraphicsPixmapItem* naranja;
    int anguloNaranja;

    QGraphicsItemGroup* grupoBrazo1;
    QGraphicsItemGroup* grupoBrazo2;

};

#endif // BRAZO_H
