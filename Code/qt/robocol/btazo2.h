#ifndef BTAZO2_H
#define BTAZO2_H

#define default_rojo 50
#define default_rosado -35
#define default_azul -15

#include <QWidget>


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class btazo2;
}

class btazo2 : public QWidget
{
    Q_OBJECT

public:
    explicit btazo2(QWidget *parent = 0);
    void rotarVerde(int key);
    void rotarRojo(int key);
    void rotarNaranja(int key);
    void enviarPosicion(char* ip,int estado, int a1,int a2,int a3);
    void enviarPosicion2(char* ip);

    void posicion_a();
    void posicion_b();
    void posicion_c();
    void posicion_d();
    void set_fantasma(int red, int pink, int blue, int bace, int muneca);

    int darRojo();
    int darRosado();
    int darAzul();

    ~btazo2();

private slots:


    void on_horizontalSlider_sliderMoved(int position);

    void on_verticalSlider_sliderMoved(int position);

    void on_dial_sliderMoved(int position);

private:
    Ui::btazo2 *ui;

    QGraphicsScene* scene;
    QGraphicsPixmapItem* bace;
    QGraphicsPixmapItem* rojo;
    QGraphicsPixmapItem* rosado;
    QGraphicsPixmapItem* azul;
    QGraphicsPixmapItem* flechaBase;
    QGraphicsPixmapItem* flechaMuneca;

    QGraphicsPixmapItem* bace_fantasma;
    QGraphicsPixmapItem* rojo_fantasma;
    QGraphicsPixmapItem* rosado_fantasma;
    QGraphicsPixmapItem* azul_fantasma;

    QGraphicsItemGroup* grupoBrazo1;
    QGraphicsItemGroup* grupoBrazo2;
    QGraphicsItemGroup* grupoBrazo3;

    QGraphicsItemGroup* grupoBrazo1_fantasma;
    QGraphicsItemGroup* grupoBrazo2_fantasma;
    QGraphicsItemGroup* grupoBrazo3_fantasma;


    int angulo_rojo;
    int angulo_rosado;
    int angulo_azul;
    int angulo_flecha_bace;
    int angulo_flecha_muneca;

    int angulo_rojo_fantasma;
    int angulo_rosado_fantasma;
    int angulo_azul_fantasma;

    int positionX;
    int positionY;
    double scale;

};

#endif // BTAZO2_H
