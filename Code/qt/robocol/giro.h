#ifndef GIRO_H
#define GIRO_H

#include <QDialog>

namespace Ui {
class Giro;
}

class Giro : public QDialog
{
    Q_OBJECT

public:
    explicit Giro(QWidget *parent = 0);
    int tipoGiro;
    ~Giro();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Giro *ui;
};

#endif // GIRO_H
