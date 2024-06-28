#pragma execution_character_set("utf-8")
#include "cutlightdlg.h"
#include "ui_cutlightdlg.h"
#include "commondef.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QFile>
#include <QTextCodec>

CutLightDlg::CutLightDlg(const QString &com, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutLightDlg)
{
    ui->setupUi(this);
//    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    // 获取端口号
    QStringList portNames;
    foreach (const QSerialPortInfo serialPortInfo, QSerialPortInfo::availablePorts()) {
        QSerialPort port;
        port.setPort(serialPortInfo);
        if(port.open(QIODevice::ReadWrite))
        {
            port.close();
            portNames.append(port.portName());
        }
    }

    qDebug() << tr("COM list:") << portNames;
    ui->comboBox_COM->addItems(portNames);
    ui->comboBox_COM->setCurrentText(com);
    QString strCom = com;
    m_com = strCom.right(strCom.length() - strCom.indexOf("M")-1).toInt();
    m_lightType = ui->comboBox_lightType->currentIndex();

    process = nullptr;
}

CutLightDlg::~CutLightDlg()
{
    delete ui;
}

bool CutLightDlg::changeLight(int channel)
{
    bool Flag=true;

    int index = ui->comboBox_lightType->currentIndex();
    switch (index) {
    case 0:  // G4C
        return changeLightForG4C(channel);
        break;
    case 1:  // 慧照光源
        m_errorString = QString(tr("暂未添加慧照光源切光功能"));
        break;
    default:
        break;
    }

    QThread::msleep(1500);

    return Flag;
}

QString CutLightDlg::LastErrorString()
{
    return m_errorString;
}

void CutLightDlg::on_comboBox_COM_currentIndexChanged(const QString &arg1)
{
    // 获取com口
    QString strCom = arg1;
    m_com = strCom.right(strCom.length() - strCom.indexOf("M")-1).toInt();
    emit signal_comChange(arg1);
}

void CutLightDlg::on_comboBox_lightChannel_currentIndexChanged(int index)
{
    const int channel = index + TET_LIGHT_CHANNEL1;
    changeLight(channel);
}

void CutLightDlg::showProcessResult()
{
    QTextCodec *codec = QTextCodec::codecForLocale();

    m_errorString = codec->toUnicode(process->readAll());
    qDebug() << "showResult:\n" << m_errorString;
    emit signal_showCutlightResult(m_errorString,NormalLevel);
}

bool CutLightDlg::changeLightForG4C(int channel)
{
    // 调用exe实现切光
    const QString strpath = QCoreApplication::applicationDirPath() + PATH_EXTRATOOL + PATH_EXTRATOOL_NAME;
    if(!QFile(strpath).exists())
    {
        m_errorString = QString(tr("切光工具不存在，请检查文件：%1").arg(strpath));
        emit signal_showCutlightResult(m_errorString,ErrorLevel);
        return false;
    }
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "] " << strpath;
    process = new QProcess(this);

    connect(process,&QProcess::readyRead,this,&CutLightDlg::showProcessResult);

    QStringList cmdList;
    cmdList.append(QString::number(2));
    cmdList.append(QString::number(m_lightType));
    cmdList.append(QString::number(m_com));
    cmdList.append(QString::number(channel-TET_LIGHT_CHANNEL1));
    process->start(strpath,cmdList);  // 运行进程
    // 等待进程结束
    process->waitForFinished();
    // 获取进程退出代码
    int exitCode = process->exitCode();
    if(exitCode != 0)
    {
        // 非0表示运行失败
        m_errorString = QString(tr("切光失败，exitCode:%1").arg(exitCode));
        qDebug() << m_errorString;
        emit signal_showCutlightResult(m_errorString,ErrorLevel);
        return false;
    }
    else
    {
        m_errorString = QString(tr("切光成功"));
        qDebug() << m_errorString;
        emit signal_showCutlightResult(m_errorString,RightLevel);
        return true;
    }
}

void CutLightDlg::on_comboBox_lightType_currentIndexChanged(int index)
{
    m_lightType = index;
}
