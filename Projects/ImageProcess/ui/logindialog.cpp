#include "logindialog.h"
#include "ui_logindialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
  , m_enterMainWindow(false)
{
    ui->setupUi(this);
//    setQss();
//    setFixedSize(400, 300);

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::enterMainWindow()
{
    return false;
}

void LoginDialog::setQss()
{
    const QString filename = ":/qss/basic.qss";
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, tr("Warning"), tr("Can't apply style!"), QMessageBox::Yes);
        return;
    }

    QTextStream in(&file);
    setStyleSheet(in.readAll());
}

bool LoginDialog::checkMatch()
{
    return true;
}

void LoginDialog::on_buttonBox_clicked(QAbstractButton *button)
{

}

void LoginDialog::on_pushButton_OK_clicked()
{
    reject();
}
