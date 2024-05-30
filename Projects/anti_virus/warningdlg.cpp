#include "warningdlg.h"
#include "ui_warningdlg.h"

warningDlg::warningDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::warningDlg)
{
    ui->setupUi(this);
     this->setWindowFlag(Qt::WindowStaysOnTopHint);
}

warningDlg::~warningDlg()
{
    delete ui;
}

void warningDlg::on_pushButton_exit_clicked()
{
    emit sendExit(true);

}
