#pragma execution_character_set("utf-8")
#include "modifypswdialog.h"
#include "ui_modifypswdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

ModifyPSWDialog::ModifyPSWDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPSWDialog),
    m_enModify(false)
{
    ui->setupUi(this);
    ui->LineEdit_NewPSW->setEnabled(m_enModify);
    ui->LineEdit_DoubleNewPSW->setEnabled(m_enModify);
    ui->pushButton_OK->setEnabled(m_enModify);
}

ModifyPSWDialog::~ModifyPSWDialog()
{
    delete ui;
}

void ModifyPSWDialog::on_LineEdit_OldPSW_returnPressed()
{
    const QString oldPsw = ui->LineEdit_OldPSW->text();
    // 检查旧密码是否正确
    // 获取数据库中保存的密码
    QString strPsw;
    QSqlQuery query(QSqlDatabase::database());
    if(!query.exec(QString("select * from password")))
    {
        qWarning() << tr("获取密码失败，") << query.lastError();
    }else{
        query.last();
        strPsw = query.value("pwd").toString();
    }
    if(strPsw != oldPsw)
    {
        QMessageBox::warning(this, tr("修改密码"), tr("旧密码不正确"));
        m_enModify=false;

    }else{
        m_enModify=true;

    }
    ui->LineEdit_NewPSW->setEnabled(m_enModify);
    ui->LineEdit_DoubleNewPSW->setEnabled(m_enModify);
    ui->pushButton_OK->setEnabled(m_enModify);
}

void ModifyPSWDialog::on_pushButton_OK_clicked()
{
    if(!m_enModify)
        return;

    const QString newPsw = ui->LineEdit_NewPSW->text();
    const QString doubleNewPsw = ui->LineEdit_DoubleNewPSW->text();

    if(!newPsw.isEmpty() && newPsw.compare(doubleNewPsw)==0){
        qInfo() << tr("修改密码成功");
        // 将新密码保存到数据库中
        QSqlQuery query(QSqlDatabase::database());
        QString strInsert = QString("insert into password values(%1);").arg(newPsw);
        if(!query.exec(strInsert))
        {
            qWarning() << tr("保存新密码失败，") << query.lastError();
        }
        QDialog::accept();  // accept()隐藏当前的模态对话框，并设置返回值Accept
    } else{
        QMessageBox::warning(this,tr("修改密码"), tr("修改密码错误\n新密码为空或新密码与确认密码不一致"));
    }
}

void ModifyPSWDialog::on_pushButton_Cancel_clicked()
{
    qInfo() << tr("【取消修改密码】");
    QDialog::reject();  // reject()隐藏当前的模态对话框，并设置返回值Reject
}
