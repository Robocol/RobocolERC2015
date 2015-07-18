#include "menu.h"
#include "ui_menu.h"
#include <QDebug>
#include <QKeyEvent>

menu::menu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
}


void menu::keyPressEvent(QKeyEvent *event)
{

    // letra s
    if(event->key()==83)
        ui->radioButton_2->setChecked(true);
    //letra w
    if(event->key()==87)
        ui->radioButton->setChecked(true);
}

menu::~menu()
{
    delete ui;
}

void menu::on_buttonBox_accepted()
{
     if(ui->radioButton->isChecked()==true)
         valor = 1;
     else valor = 2;

}
