#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPoint>
#include <QPen>
#include <QTimer>

#include "comunicacion.h"
#include "cuerpo.h"
#include "btazo2.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int conectarServidores(char* ip);
    int recibirAArchivo(int sfd,char* nombre);
    struct servidores{
        int llantas_izq;
        int llantas_der;
        int brazo;
    };

    ~MainWindow();

private slots:
    void estadoActual();
    void conectar();
    void desconectar();
    void cambiarEstado(int izq,int dere,int bra);
    void mandarFecha();
    void slotTimer();
    void comando();
    void camara();


    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    cuerpo *ventanaCuerpo;
    btazo2 *ventanaBrazo;
    servidores estado;
    int funcion;

    //constantes estado de los servidores
    static const int NO_CONECTADO = 0;
    static const int INICIADO = 1;
    static const int SEGURO = 2;
    static const int NORMAL = 3;
    static const int ERROR_RECUPERACION = 4;

    QTimer *timer;
    int numGiro;
    int fino;


protected:
    void keyPressEvent(QKeyEvent *event);


};

#endif // MAINWINDOW_H
