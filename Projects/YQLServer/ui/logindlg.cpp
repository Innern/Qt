#pragma execution_character_set("utf-8")
#include "logindlg.h"
#include "ui_logindlg.h"
#include "modifypswdialog.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg),
    m_isAdministrator(false),
    m_bClose(false)
{
    ui->setupUi(this);
//    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->lineEdit_passwd->setFocus();
    ui->pushButton_OK->setDefault(true);
//    if(ui->comboBox_userOp->currentText() == "Operator")
//        ui->lineEdit_passwd->setEnabled(false);
//    else
//        ui->lineEdit_passwd->setEnabled(ui->comboBox_userOp->currentIndex()==User_Administrator);
    ui->lineEdit_passwd->setEnabled(ui->comboBox_userOp->currentIndex()==User_Administrator);
    on_comboBox_userOp_activated(ui->comboBox_userOp->currentIndex());
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

bool LoginDlg::isAdministrator() const
{
    return m_isAdministrator;
}

bool LoginDlg::closeLoginDialog() const
{
    return m_bClose;
}


void LoginDlg::on_pushButton_OK_clicked()
{
    const QString user = ui->comboBox_userOp->currentText();
    const QString passwd = ui->lineEdit_passwd->text();
    if(user == "Administrator"){
        if(passwd.isEmpty()){
            QMessageBox::warning(this,tr("请输入密码"),tr("请输入密码再登录"),QMessageBox::Ok);
            ui->lineEdit_passwd->setFocus();
        } else {
            QSqlQuery query;
            query.exec("select * from password;");
            query.last();
//            qDebug() << tr("Current psw:") << query.value(0).toString();
            if(passwd == query.value("pwd").toString()){
                m_isAdministrator = true;
                QDialog::accept();
            } else {
                QMessageBox::warning(this,tr("密码错误"),tr("密码输入错误，请重新输入"),QMessageBox::Ok);
                ui->lineEdit_passwd->clear();
                ui->lineEdit_passwd->setFocus();
            }
        }

    } else {
        m_isAdministrator = false;
        QDialog::accept();
    }

}

void LoginDlg::on_pushButton_Cancel_clicked()
{
    QDialog::reject();
}

void LoginDlg::on_comboBox_userOp_currentTextChanged(const QString &arg1)
{
    if(arg1 == "Operator")
        ui->lineEdit_passwd->setEnabled(false);
    else if(arg1 == "Administrator") {
        ui->lineEdit_passwd->setEnabled(true);
        ui->lineEdit_passwd->clear();
        ui->lineEdit_passwd->setFocus();
    }
}

void LoginDlg::on_pushButton_ModifyPSW_clicked()
{
    qDebug() << tr("修改密码");
    ModifyPSWDialog modPswDlg;
    if(modPswDlg.exec() == QDialog::Accepted){
        ui->lineEdit_passwd->setFocus();
    }
}

void LoginDlg::on_comboBox_userOp_activated(int index)
{
    ui->pushButton_ModifyPSW->setEnabled(index==User_Administrator);
}
