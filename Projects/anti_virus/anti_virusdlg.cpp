#include "anti_virusdlg.h"
#include "ui_anti_virusdlg.h"

#include "cfilesordirindir.h"

anti_virusDlg::anti_virusDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::anti_virusDlg)
    , m_fileWatcher(new QFileSystemWatcher(this))
{
    ui->setupUi(this);

    this->setWindowFlag(Qt::WindowMinMaxButtonsHint);

    updateWatcherFiles();
    setSystemTrayIcon();


#if AUTO_CLOSE
    m_iTimerEvent = startTimer(500);
#endif

    //设置定时器，每隔1s显示LCD时间
    QTimer* timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&anti_virusDlg::timeUpdate);
    timer->start(1000);

    connect(this,&anti_virusDlg::send_file_size_exp,this,&anti_virusDlg::showFileChanged);

    //文件或目录改变时提示
    connect(m_fileWatcher,&QFileSystemWatcher::fileChanged,this,&anti_virusDlg::showFileChanged);
    connect(m_fileWatcher,&QFileSystemWatcher::directoryChanged,this,&anti_virusDlg::showFileChanged);

    //系统托盘操作
    connect(m_systemTrayIcon,&QSystemTrayIcon::activated,this,&anti_virusDlg::responseSysTrayIcon);
    connect(&m_warDlg,&warningDlg::sendExit,this,&anti_virusDlg::hideWarning);


}

anti_virusDlg::~anti_virusDlg()
{
    delete ui;
}

void anti_virusDlg::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::WindowStateChange && isMinimized()){
        hide();
        m_systemTrayIcon->setIcon(QIcon(":/iicons/anti_virus"));
        m_systemTrayIcon->setToolTip(tr("anti_virus"));
        m_systemTrayIcon->show();
        e->ignore();
    }
    QDialog::changeEvent(e);
}

void anti_virusDlg::timerEvent(QTimerEvent *event)
{
#if AUTO_CLOSE
    if(event->timerId() == m_iTimerEvent)
    {
        this->setWindowState(Qt::WindowMinimized);
    }
#endif

    QDialog::timerEvent(event);
}

QStringList anti_virusDlg::getFileNames(const QString &path,const QString& filter,qint64& maxsize)
{
    QDir dirPath(path);
    QStringList filterlist(filter);
    QFileInfoList filesList = dirPath.entryInfoList(filterlist,QDir::AllEntries | QDir::Hidden,QDir::Name);

    QStringList rtFiles;

    for(QFileInfo val : filesList)
    {
        val.isHidden();
        rtFiles << val.filePath();
        maxsize = maxsize > val.size() ? maxsize : val.size();
    }
    return rtFiles;
}

void anti_virusDlg::updateWatcherFiles()
{
    qint64 maxsize= -1;
    m_strDirPath = QDir::currentPath();
    m_strDirPath = m_strDirPath.left(m_strDirPath.lastIndexOf("/")+1);//获取当前目录
    QStringList rtFilesList;
    CFilesOrDirInDir filefind(m_strDirPath);
    QStringList fileList = filefind.getAllChildDirs(m_strDirPath);

    for(QString val:fileList)
    {
        if(val.contains("OFKit.exe",Qt::CaseInsensitive))
        {
            rtFilesList << val;//ofkit.exe
            QFileInfo fileinfo(val);
            maxsize = fileinfo.size();

        }
        if(val.contains("CameraTest.exe",Qt::CaseInsensitive))
        {
            rtFilesList << val;//CameraTest.exe
            QFileInfo fileinfo(val);
            maxsize = fileinfo.size();

            QString strDllPath = val.left(val.lastIndexOf("/"))+"/Inspection/";
            QStringList dllList = getFileNames(strDllPath,"*.dll",maxsize);
            rtFilesList.append(dllList);
        }
    }

    m_fileWatcher->addPaths(rtFilesList);

    const qint64 file_size = 5*1024*1024;
    if(maxsize > file_size)
        emit send_file_size_exp();//文件大小异常，发送信号，报错

}

void anti_virusDlg::setSystemTrayIcon()
{
    m_systemTrayIcon = new QSystemTrayIcon(this);
    QAction* action_exit = new QAction(tr("退出"),this);
    QMenu* menu = new QMenu(this);
    menu->addAction(action_exit);
    m_systemTrayIcon->setContextMenu(menu);

    connect(action_exit,&QAction::triggered,this,&anti_virusDlg::exitApplication);//退出程序


}

void anti_virusDlg::hideWarning(bool exit)
{
    if(exit)
        m_warDlg.hide();
    updateWatcherFiles();
}

void anti_virusDlg::timeUpdate()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    QString strDate_tiem = "";
    strDate_tiem.append(date.toString("yyyy-MM-dd "));
    strDate_tiem.append(time.toString("hh:mm:ss"));
    ui->lcdNumber_time->display(strDate_tiem);
}

void anti_virusDlg::showFileChanged()
{
    m_warDlg.exec();
    updateWatcherFiles();

}


void anti_virusDlg::responseSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
        this->setWindowState( Qt::WindowActive);
        this->show();
        break;
    case QSystemTrayIcon::Trigger:
        m_systemTrayIcon->contextMenu()->show();
        break;
    default:
        break;
    }
}

void anti_virusDlg::exitApplication()
{
    this->close();
}

