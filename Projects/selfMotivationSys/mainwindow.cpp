#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QSettings>
#include <QTimer>
#include <QDir>
#include <QProcess>

#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_planWidget = new Dialog();
    m_label = new QLabel();

    connect(m_planWidget, &Dialog::sendText, this, &MainWindow::saveText);

    //设置状态栏
    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);

    m_iSignDays = 0;
    //设置label信息
    setMessageLabel();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_planWidget;
}

#define RUN_KEY "HKEY_CURRENT_USER\\Software\\selfMotivationSys\\"

void MainWindow::on_pushButton_sign_clicked()
{
    QDate currentDate = QDate::currentDate();
    QString strTime = currentDate.toString("yyyy-MM-dd");


    QString strReg;
    strReg.sprintf("%s\\", RUN_KEY);
    QSettings *pReg = new QSettings(strReg, QSettings::NativeFormat);

    pReg->setValue("date", strTime); //设置注册表值
    pReg->setValue("isSign", "true");
    delete pReg;


    QString strLabelInfo = "";
    strLabelInfo.sprintf("你已经签到%d天，加油^_^!", m_iSignDays+1);
    ui->label_showMainMessage->setText(strLabelInfo);
    //弹出新的对话框
    m_planWidget->show();
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString strTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    m_label->setText(strTime);
    ui->statusbar->addWidget(m_label);
}

void MainWindow::saveText(const QString &text)
{
    QString path = QDir::currentPath();
    path += "/record.txt";
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QByteArray ba = text.toLocal8Bit();
    file.write(ba);
    file.close();

    QProcess *process = new QProcess();
    process->start(path);

}

void MainWindow::setMessageLabel()
{
    QString strReg;
    strReg.sprintf("%s\\", RUN_KEY);
    QSettings *pReg = new QSettings(strReg, QSettings::NativeFormat);

    QVariant v = pReg->value("date"); //读取注册表值
    QString strDate = v.toString();



    QDate fileDate;
    fileDate = fileDate.fromString(strDate,"yyyy-MM-dd");
    QDate currentDate = QDate::currentDate();
    m_iSignDays = fileDate.daysTo(currentDate); //计算两个日期之间的天数
    QString strLabelInfo = "";
    strLabelInfo.sprintf("你已经签到%d天，加油^_^!", m_iSignDays);
    if("true" == pReg->value("isSign").toString())
        strLabelInfo.sprintf("你已经签到%d天，加油^_^!", m_iSignDays+1);
    ui->label_showMainMessage->setText(strLabelInfo);

    delete pReg;
}
