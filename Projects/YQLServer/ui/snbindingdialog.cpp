#pragma execution_character_set("utf-8")
#include "snbindingdialog.h"
#include "ui_snbindingdialog.h"

#include <QComboBox>
#include <QRegExp>
#include <QTextCodec>
#include <QDebug>
#include <QSettings>
#include <QThread>
#include <QMessageBox>

SNBindingDialog::SNBindingDialog( const QString exePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SNBindingDialog),
    m_strExePath(exePath)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(this,&SNBindingDialog::signal_showTableWidgetItem,this,&SNBindingDialog::showTableWidgetItem);

    process = new QProcess(this);
    cmdList.append(QString::number(1));
    connect(process,&QProcess::readyRead,this,&SNBindingDialog::showProcessResult);
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(showProcessFinished(int,QProcess::ExitStatus)));

    process->start(m_strExePath,cmdList);
}

SNBindingDialog::~SNBindingDialog()
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    delete ui;
}

void SNBindingDialog::showProcessResult()
{
    QTextCodec *codec = QTextCodec::codecForLocale();
    const QString strResult = codec->toUnicode(process->readAll());
    m_snList.clear();
    QRegExp rx(":([0-9a-zA-Z-]*)");
    int pos=0;
    while ((pos = rx.indexIn(strResult,pos)) != -1) {
        m_snList << rx.cap(1);
        pos += rx.matchedLength();
    }
    qDebug() << m_snList;
    emit signal_showTableWidgetItem(m_snList);
}

void SNBindingDialog::showProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "[SNBinding] exitCode:" << exitCode << ",ExitStatus:" << exitStatus;
}

void SNBindingDialog::showTableWidgetItem(const QStringList &list)
{
    const int snCnt = list.size();
    // 设置行列数、标头
    const QStringList colList = QStringList() << tr("sn") << tr("工装");
    ui->tableWidget_snList->setRowCount(snCnt);
    ui->tableWidget_snList->setColumnCount(colList.size());
    // 设置标头
    ui->tableWidget_snList->setHorizontalHeaderItem(0,new QTableWidgetItem(colList.at(0)));
    ui->tableWidget_snList->setHorizontalHeaderItem(1,new QTableWidgetItem(colList.at(1)));
    QStringList socketList;
    for(int i=0;i<snCnt;++i)
    {
        ui->tableWidget_snList->setVerticalHeaderItem(i,new QTableWidgetItem(QString::number(i)));
        socketList.append(tr("工装%1").arg(i+1));
    }

    // 创建表格项目，插入到指定的单元格中
    for(int i=0;i<snCnt;++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(m_snList.at(i));
        // 设置单元格不能编辑
        item->setFlags(item->flags() & ~(Qt::ItemIsEditable));
        ui->tableWidget_snList->setItem(i,0,item);
        QComboBox *combox = new QComboBox();
        combox->addItems(socketList);
        combox->setCurrentIndex(i);
        ui->tableWidget_snList->setCellWidget(i,1,combox);
    }

    // 设置表格随内容变化长宽
    ui->tableWidget_snList->resizeColumnsToContents();
    ui->tableWidget_snList->resizeRowsToContents();
    // 设置表格随窗口大小变化
    ui->tableWidget_snList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void SNBindingDialog::getSnList()
{
    process->start(m_strExePath,cmdList);
}

void SNBindingDialog::on_pushButton_refresh_clicked()
{
    ui->tableWidget_snList->clear();
    process->start(m_strExePath,cmdList);
}

void SNBindingDialog::on_pushButton_binding_clicked()
{
    // 获取当前单元格中的内容

    QStringList snList;
    QList<int> socketIdxs;
    for(int i=0;i<m_snList.size();++i)
    {
        snList << ui->tableWidget_snList->item(i,0)->text();
        QComboBox *combox = qobject_cast<QComboBox*>(ui->tableWidget_snList->cellWidget(i,1));
        socketIdxs.append( combox->currentIndex());
    }

    QSettings settings("CCM","CameraTest");
    settings.beginGroup("SNBinding");
    for(int i=0;i<m_snList.size();++i)
    {
        settings.setValue(tr("ID_%1").arg(socketIdxs.at(i)),snList.at(i));
        qDebug() << "binding: " << tr("ID_%1").arg(socketIdxs.at(i)) << " " << snList.at(i);
    }
    settings.endGroup();

    QMessageBox::information(this,QApplication::applicationName(),tr("绑定成功，请重新打开程式"),QMessageBox::Yes);
}
