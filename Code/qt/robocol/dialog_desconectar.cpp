#include "dialog_desconectar.h"
#include "ui_dialog_desconectar.h"
#include <QDebug>


Dialog_desconectar::Dialog_desconectar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_desconectar)
{
    ui->setupUi(this);
}

Dialog_desconectar::~Dialog_desconectar()
{
    delete ui;
}

void Dialog_desconectar::on_buttonBox_accepted()
{
    if(ui->checkBoxBra->isChecked()==true)
        braz=0;
    else
        braz=-1;
    if(ui->checkBoxIzq->isChecked()==true)
        izqui=0;
    else
        izqui=-1;
    if(ui->checkBoxDer->isChecked()==true)
        dere=0;
    else
        dere=-1;

}
