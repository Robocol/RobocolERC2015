#ifndef DIALOG_DESCONECTAR_H
#define DIALOG_DESCONECTAR_H

#include <QDialog>

namespace Ui {
class Dialog_desconectar;
}

class Dialog_desconectar : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_desconectar(QWidget *parent = 0);
    int izqui;
    int dere;
    int braz;
    ~Dialog_desconectar();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog_desconectar *ui;

};

#endif // DIALOG_DESCONECTAR_H
