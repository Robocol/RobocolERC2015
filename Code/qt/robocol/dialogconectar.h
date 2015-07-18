#ifndef DIALOGCONECTAR_H
#define DIALOGCONECTAR_H

#include <QDialog>

namespace Ui {
class DialogConectar;
}

class DialogConectar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConectar(QWidget *parent = 0);
    void cambiarDer(int estado);
    void cambiarIzq(int estado);
    void cambiarBrazo(int estado);
    ~DialogConectar();

private:
    Ui::DialogConectar *ui;
};

#endif // DIALOGCONECTAR_H
