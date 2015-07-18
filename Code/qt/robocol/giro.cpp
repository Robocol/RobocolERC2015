#include "giro.h"
#include "ui_giro.h"

Giro::Giro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Giro)
{
    ui->setupUi(this);
}

Giro::~Giro()
{
    delete ui;
}

void Giro::on_buttonBox_accepted()
{
    if(ui->r1->isChecked())
        tipoGiro = 1;
    else if(ui->r2->isChecked())
        tipoGiro = 2;
    else
        tipoGiro = 3;
}
