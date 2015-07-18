#include "dialogconectar.h"
#include "ui_dialogconectar.h"

DialogConectar::DialogConectar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConectar)
{
    ui->setupUi(this);
}

DialogConectar::~DialogConectar()
{
    delete ui;
}

void DialogConectar::cambiarDer(int estado)
{
    ui->s2->setText(QString::number(estado));
}

void DialogConectar::cambiarIzq(int estado)
{
    ui->s1->setText(QString::number(estado));
}

void DialogConectar::cambiarBrazo(int estado)
{
    ui->s3->setText(QString::number(estado));
}
