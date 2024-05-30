#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "logindlg.h"
#include "connection.h"

#include <QApplication>
#include <QMutex>
#include <QFile>
#include <QMessageLogContext>
#include <QDebug>
#include <QDateTime>
#include <QDir>

// 消息处理函数
void outputMessage(QtMsgType type,const QMessageLogContext &context,const QString &msg);

int main(int argc, char *argv[])
{
    // 安装消息处理函数
    qInstallMessageHandler(outputMessage);

    // 使Qt根据操作系统提供的显示比例因子缩放应用程序
    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication::setOrganizationName("CCMTest");  // 设置应用程序的组织名称
    QApplication::setApplicationName("YQLServer");  // 设置应用程序名称
    QApplication::setApplicationVersion(QT_VERSION_STR);  // 设置应用程序版本

    qRegisterMetaType<LogLevel>("LogLevel");  // 注册自定义类型，注册后可以用于信号-槽函数的参数

    if(!createConnection())
    {
        return 0;
    }

    QApplication a(argc, argv);
    qDebug() << QObject::tr("====================================程序启动========================================");
    LoginDlg dlg;

    if(dlg.exec() == QDialog::Accepted){
        MainWindow w;
        w.setUiEnable(dlg.isAdministrator());
        w.show();
        return a.exec();
    } else {
        return 0;
    }
}

void outputMessage(QtMsgType type,const QMessageLogContext &context,const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QByteArray localMsg = msg.toLocal8Bit();
    QString text;
    switch (type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        break;
    default:
        text = QString("Debug:");
    }

    // 设置输出信息格式
    const QString context_info = QString("File:(%1)").arg(QString(context.file)); // F文件L行数
    const QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
#ifdef DEBUG
    const QString strMessage = QString("[%1] [%2]\t[%3]\t[%4]").arg(text).arg(strDateTime).arg(context_info).arg(msg);
#else
    const QString strMessage = QString("[%1] [%2]\t%3").arg(text).arg(strDateTime).arg(msg);
#endif
    // 输出信息至文件中（读写、追加形式）
    const QString strFilepath = QString(QApplication::applicationDirPath() + "/log/");
    const QString strFileName = QString(strFilepath + QDateTime::currentDateTime().toString("yyyy-MM-dd").append("-log.txt"));
    if(!QDir(strFilepath).exists())
    {
        QDir(strFilepath).mkdir(strFilepath);
    }
    QFile file(strFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();
    // 解锁
    mutex.unlock();
}
