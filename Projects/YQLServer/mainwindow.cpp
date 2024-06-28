#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "worker.h"
#include "copythread.h"
#include "include/connection.h"
#include "cutlightdlg.h"
#include "snbindingdialog.h"
#include "logindlg.h"

#include <QThread>
#include <QDebug>
#include <QCloseEvent>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
// #include <QDesktopWidget>
#else
#include <QGuiApplication>
#include <QTextCodec>
#endif

#include <QFileDialog>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_preConnectCnt(0),
    m_hasConnectCnt(0),
    m_moduleHasCloedCnt(0),
    m_windowStyle("fusion"),
    m_dccfFile("dccf"),
    m_connectCnt(-1),
    m_port(-1)
{
    // 输出日志信息
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    ui->setupUi(this);

    this->setAttribute(Qt::WA_QuitOnClose);
    setWindowTitle(tr("%1-%2").arg(this->windowTitle()).arg(APP_VERSION));
#if 0
    if(!readSettings())  // 获取界面设计及控件变量相关信息
    {
        this->close();
        return;
    }
#endif
    readSettings();
    initParams();
    initStatus();
    init();
}

MainWindow::~MainWindow()
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    delete ui;
}

void MainWindow::setUiEnable(bool isAdministrator)
{
    ui->pushButton_startTest->setEnabled(true);
    ui->pushButton_sendCmd->setEnabled(false);

    ui->lineEdit_IP->setEnabled(isAdministrator);
    ui->spinBox_port->setEnabled(isAdministrator);
    ui->spinBox_connectCnt->setEnabled(isAdministrator);
    ui->spinBox_SN->setEnabled(isAdministrator);
    ui->spinBox_cmd->setEnabled(isAdministrator);
    ui->spinBox_cutLightDelay->setEnabled(isAdministrator);

    ui->actionExit->setEnabled(isAdministrator);
    ui->actionSave_UI->setEnabled(isAdministrator);
    ui->actionLoad_DCCF->setEnabled(isAdministrator);
    ui->actionCopy_Program->setEnabled(isAdministrator);
    ui->actionCut_Liight->setEnabled(isAdministrator);
    ui->actionClose_Mes->setEnabled(isAdministrator);
    ui->actionSNBinding->setEnabled(true);
    ui->actionOpen_Program->setEnabled(true);
    ui->actionClose_Program->setEnabled(false);
    ui->actionOpen_Camera->setEnabled(isAdministrator);
    ui->actionClose_Camera->setEnabled(isAdministrator);
    ui->actionClear_Log->setEnabled(true);
}

void MainWindow::displaySocketMessage(LogLevel level, int idx, const QString &msg)
{
    QString strlog = QString("camera_%1 %2").arg(idx).arg(msg);
    addLog(strlog,level);
}

void MainWindow::hasSocketDisconnect()
{
    //    addLog(tr("有socket断开连接"),ErrorLevel);
    m_hasConnectCnt--;
    if(0 == m_hasConnectCnt)  // 所有连接全部断开
    {
        // 断开连接时，使能：loadDccf、copy program、ip, open program、MES设置
        ui->lineEdit_IP->setEnabled(true);
        ui->spinBox_connectCnt->setEnabled(true);
        ui->spinBox_port->setEnabled(true);
        ui->actionLoad_DCCF->setEnabled(true);
        ui->actionCopy_Program->setEnabled(true);
        ui->actionOpen_Program->setEnabled(true);
        m_connecStatusLabel->setPixmap(QPixmap(":/images/FAIL.png").scaled(SCALED_IMAGE_WIDTH,SCALED_IMAGE_HEIGHT));
    }
}

void MainWindow::hasNewConnection()
{
    if(m_hasConnectCnt == 0)
    {
        // 有新的连接时，设置disabled: loadDccf、copy program、ip
        // 有新的连接时，使能：开始测试、发送指令
        ui->lineEdit_IP->setEnabled(false);
        ui->spinBox_connectCnt->setEnabled(false);
        ui->spinBox_port->setEnabled(false);
        ui->actionLoad_DCCF->setEnabled(false);
        ui->actionCopy_Program->setEnabled(false);
        ui->actionClose_Program->setEnabled(false);
        ui->actionOpen_Camera->setEnabled(false);;
        ui->actionClose_Camera->setEnabled(false);

        m_connecStatusLabel->setPixmap(QPixmap(":/images/OK.png").scaled(SCALED_IMAGE_WIDTH,SCALED_IMAGE_HEIGHT));
    }
    m_hasConnectCnt++;
    if(m_hasConnectCnt == m_threads.size())
    {
        ui->pushButton_sendCmd->setEnabled(true);
        ui->pushButton_startTest->setEnabled(true);
        ui->actionOpen_Camera->setEnabled(true);
        ui->actionClose_Camera->setEnabled(true);
        ui->actionClose_Program->setEnabled(true);
    }
}

void MainWindow::getSocketState(int state, int idx)
{
    m_mapSocketState[idx] = state;
    checkSocketState();
}

void MainWindow::addLog(const QString &str, LogLevel level)
{
    QTextCursor cursor = ui->textEdit_log->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_log->setTextCursor(cursor);
    switch (level) {
    case SendLevel:
        ui->textEdit_log->setTextColor(Qt::magenta);
        break;
    case WaitLevel:
    case GrayLevel:
        ui->textEdit_log->setTextColor(Qt::gray);
        break;
    case ReceiveLevel:
        ui->textEdit_log->setTextColor(Qt::darkCyan);
        break;
    case RightLevel:
        ui->textEdit_log->setTextColor(Qt::darkGreen);
        break;
    case ErrorLevel:
        ui->textEdit_log->setTextColor(Qt::red);
        break;
    case NormalLevel:
        ui->textEdit_log->setTextColor(Qt::black);
        break;
    case ReservedLevel:
        ui->textEdit_log->setTextColor(Qt::darkGray);
        break;
    default:
        QColor tmpClr = ui->textEdit_log->textColor();
        ui->textEdit_log->setTextColor(tmpClr);
        break;
    }
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();


    ui->textEdit_log->append(date.toString("[yyyy/MM/dd") + time.toString(" hh:mm:ss.zzz] ") + str);
    qDebug() << str.toStdString().data();
}

void MainWindow::readUISettings()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    // 窗口几何位置信息
    QByteArray geometry = settings.value("geometry").toByteArray();
    if(geometry.isEmpty())
    {
        // 如果没有保存几何信息，设置默认的程序大小及位置
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QRect avaliableRect = QApplication::desktop()->availableGeometry();  // 屏幕几何大小
#else
    const QRect avaliableRect = QGuiApplication::primaryScreen()->geometry();  // 屏幕几何大小
#endif
        resize(avaliableRect.width()/3,avaliableRect.height()/3);
        move((avaliableRect.width() - width())/2, (avaliableRect.height() - height())/2);
    }
    else
    {
        restoreGeometry(geometry);
    }

    // 窗口布局状态信息
    QByteArray state = settings.value("state").toByteArray();
    if(!state.isEmpty())
    {
        restoreState(state);
    }
    return;
}

void MainWindow::writeUISettings()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    settings.setValue("geometry",saveGeometry());
    settings.setValue("state",saveState());
    return;
}

bool MainWindow::readSqlSettings()
{
#if 0
    // 从数据库中获取设置

    // 执行查询操作
    QSqlQuery query;
    if(!query.exec("select * from Settings"))
    {
        if(QMessageBox::Yes == QMessageBox::critical(this,tr("ERROR"),tr("获取设置失败，请确认数据库表中是否有数据."),QMessageBox::Yes))
        {
            // 退出程序
            //            this->close();
            return false;
        }
    }

    // 如果表中有数据，就读取数据
    if (query.next())
    {
        m_dccfFile = query.value(1).toString();
        m_connectCnt = query.value(2).toInt();
        m_ip = query.value(3).toString();
        m_port = query.value(4).toInt();
        m_windowStyle = query.value(5).toString();
        qDebug() << m_dccfFile << m_connectCnt << m_ip << m_port << m_windowStyle;
    }
    else
    {
        // 如果表中没有数据，提示
        if(QMessageBox::Yes == QMessageBox::critical(this,tr("ERROR"),tr("表中没有数据，请添加一条记录，error:%1.").arg(query.lastError().text()),QMessageBox::Yes))
        {
            // 退出程序
            //            this->close();
            return false;
        }
    }
    return true;
#else
    // 从ini文件中获取
    QSettings settings("boot",QSettings::Format::IniFormat,this);
    m_dccfFile = settings.value("Path/dccf",".dccf").toString();
    m_connectCnt = settings.value("Connect/connectCnt",1).toInt();
    m_ip = settings.value("Connect/ip","127.0.0.1").toString();
    m_port = settings.value("Connect/port",10086).toInt();
    m_windowStyle = settings.value("Style/windowStyle","fusion").toString();
    m_com = settings.value("Light/Com","COM1").toString();
    m_delayTimes = settings.value("Light/delayTime",1500).toInt();
    return true;
#endif
}

bool MainWindow::writeSqlSettings()
{
#if 0
    QSqlQuery query;
    const int idx = 1;
    // 先查询是否有记录
    query.exec("select * from Settings");
    if(!query.next())  // 如果没有数据
    {
        qDebug() << m_dccfFile;
        // 没有数据执行插入操作
        const QString sql = QString("insert into Settings(idx,dccfname,connectCnt,ip,port,windowStyle) values (%1,'%2',%3,'%4',%5,'%6');").arg(idx).arg(m_dccfFile).arg(m_connectCnt).arg(m_ip).arg(m_port).arg(m_windowStyle);
        queryTable(sql);
    }
    else
    {
        // 有数据执行更新操作
        const QString sql = QString("update Settings set dccfname='%2',connectCnt=%3,ip='%4',port=%5,windowStyle='%6' where idx=%1;").arg(idx).arg(m_dccfFile).arg(m_connectCnt).arg(m_ip).arg(m_port).arg(m_windowStyle);
        queryTable(sql);
    }
    return true;
#else
    QSettings settings("boot",QSettings::Format::IniFormat,this);
    settings.setValue("Path/dccf",m_dccfFile);
    settings.setValue("Connect/connectCnt",m_connectCnt);
    settings.setValue("Connect/ip",m_ip);
    settings.setValue("Connect/port",m_port);
    settings.setValue("Style/windowStyle",m_windowStyle);
    settings.setValue("Light/Com",m_com);
    settings.setValue("Light/delayTime",m_delayTimes);

    return true;
#endif
}

bool MainWindow::readSettings()
{
    readUISettings();
    bool bret = readSqlSettings();
    cache2ui();  // 将读取到的变量显示到界面上
    return bret;
}

bool MainWindow::writeSettings()
{
    ui2cache();  // 从控件上获取变量
    writeUISettings();
    bool bret = writeSqlSettings();
    return bret;
}

void MainWindow::cache2ui()
{
    ui->spinBox_connectCnt->setValue(m_connectCnt);
    ui->lineEdit_IP->setText(m_ip);
    ui->spinBox_port->setValue(m_port);
}

void MainWindow::ui2cache()
{
    m_connectCnt = ui->spinBox_connectCnt->value();
    m_ip = ui->lineEdit_IP->text();
    m_port = ui->spinBox_port->value();
}

bool MainWindow::loadInspectionsInfo(const QString &dccfFile)
{
    if(!QFileInfo(dccfFile).exists())
    {
        addLog(tr("测试程序dccf不存在：%1").arg(QFileInfo(dccfFile).fileName()),ErrorLevel);
        return false;
    }

    QSettings settings(dccfFile,QSettings::Format::IniFormat);

    QString tmp;
    // 获取[INSPECTION_DLL]下的测试项名称

    settings.beginGroup(INSPECTION_DLL);
    const int nameCnt = settings.allKeys().size();
    QList<QString> name;
    for(int i=0;i<nameCnt;i++)
    {
        tmp = settings.value(QString("Item_%1").arg(i)).toString();  // 获取到的值包含.dll，添加到name中时去除.dll
        name.append(tmp.left(tmp.length() - 4));
    }
    settings.endGroup();

    QList<int> environment;
    settings.beginGroup(INSPECTION_DLL_ENVTYPE);
    for(int i=0;i<nameCnt;i++)
    {
        environment.append(settings.value(name.at(i)+".dll").toInt());
    }
    settings.endGroup();

    QList<QString> errcode;
    settings.beginGroup(INSPECTION_DLL_ERRCODE);
    for(int i=0;i<nameCnt;i++)
    {
        errcode.append(settings.value(name.at(i)+".dll").toString());
    }
    settings.endGroup();

    QList<QString> dllName;
    for(int i=0;i<nameCnt;++i)
    {
        dllName.append(settings.value(tr("%1/Library").arg(name.at(i))).toString());
    }

    // 赋值给成员变量
    m_inspections.clear();
    InspectionInfo insp;
    for(int i=0;i<name.size();i++)
    {
        insp.name = name.at(i);
        insp.environment = environment.at(i);
        insp.errcode = errcode.at(i);
        insp.dllName = dllName.at(i);
        m_inspections.insert(i,insp);
        addLog(tr("%5 - Loaded %1 (%2) %3 %4").arg(insp.name).arg(insp.dllName).arg(insp.environment).arg(insp.errcode).arg(i+1),GrayLevel);
    }
    addLog(tr("当前测试程式：%1, 测试项目个数：%2").arg(projectLabel->text()).arg(m_inspections.size()));

    return true;
}

void MainWindow::clearWorkerThreads()
{
    for(int i=0;i<m_threads.size();++i)
    {
        if(m_threads[i] && m_threads[i]->isRunning())
        {
            m_threads[i]->quit();
            m_threads[i]->wait();
        }
        SAFE_DELETE_PTR(m_threads[i]);
    }
    m_threads.clear();
}

void MainWindow::clearCopyThreads()
{
    for(int i=0;i<m_copyThreads.size();++i)
    {
        if(m_copyThreads[i] && m_copyThreads[i]->isRunning())
        {
            m_copyThreads[i]->quit();
            m_copyThreads[i]->wait();
        }
        SAFE_DELETE_PTR(m_copyThreads[i]);
    }
    m_copyThreads.clear();
}

void MainWindow::clearProcessPointer()
{
    if(m_hasProcessClosed < m_process.size())
    {
        for(int i=0;i<m_process.size();++i)
        {
            if(m_process[i] && m_process[i]->isOpen())
            {
                m_process[i]->close();
                m_process[i]->waitForFinished();
            }
            SAFE_DELETE_ARRAY(m_process[i]);
        }
    }
    m_process.clear();
}

void MainWindow::setMESStatus(const QString &dccfFile)
{
    // 从dccf中获取mes状态
    QSettings settings(QFileInfo(dccfFile).canonicalFilePath(),QSettings::IniFormat);
    // settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    int mesState = settings.value("DataSavingTab/SaveToSQL_En",0).toInt();
    if(mesState == 0)  // mes关闭状态
    {
        ui->actionClose_Mes->setText(tr("Open MES"));
        ui->actionClose_Mes->setIcon(QIcon(":/images/MES_OFF.png"));
    }
    else  // mes 打开状态
    {
        ui->actionClose_Mes->setText(tr("Close MES"));
        ui->actionClose_Mes->setIcon(QIcon(":/images/MES_ON.png"));
    }
}

void MainWindow::checkSocketState()
{
    m_timerCheckSocket->stop();
    int nExcuteNextCmd = 0;  // 测试中的窗口个数
    int nFailedAndClose = 0;
    for(int i=0;i<m_hasConnectCnt;++i)
    {
        if(m_mapSocketState[i] == State_Testing)
        {
            nExcuteNextCmd++;
        }

        else if(m_mapSocketState[i] == State_FailedAndModuleClose)  // 是否所有窗口都测试失败已经关闭了模组
        {
            nFailedAndClose++;
        }
    }

    // 所有窗口测试失败，且已经关闭了模组，那么重新开始测试，可以发送其它测试命令
    if(nFailedAndClose == m_hasConnectCnt && !m_cmdQueue.isEmpty())
    {
        m_cmdQueue.clear();
        emit signal_restoreSocketState();

        ui->actionOpen_Camera->setEnabled(true);
        ui->spinBox_SN->setEnabled(true);
        ui->spinBox_cmd->setEnabled(true);
        ui->pushButton_sendCmd->setEnabled(true);
        ui->pushButton_startTest->setEnabled(true);
        ui->pushButton_startTest->setText(tr("开始\n测试"));
        // 重置socket状态
        m_mapSocketState.clear();
        for(int i=0;i<m_hasConnectCnt;++i)
            m_mapSocketState.insert(i,State_Testing);
        return;
    }

    // 所有窗口已经测试结束，可以发送下一条指令，且指令集不为空
    if(nExcuteNextCmd==0 && !m_cmdQueue.isEmpty())
    {
        int cmd = m_cmdQueue.dequeue();
        if(cmd < 100)
        {
            // 对测试项做一些处理，比如切光，测试环境为FINAL的测试项处理
            int channel = m_inspections.value(cmd-1).environment;
            addLog(tr("当前测试项：%1，测试环境：%2").arg(m_inspections.value(cmd-1).name).arg(m_inspections.value(cmd-1).environment));
            if(channel == TET_FINAL)
            {
                cmd = SM_CMD_CAM_STOP;
                m_cmdQueue.clear();
            }
            else if(channel >= TET_LIGHT_CHANNEL1 && channel <= TET_LIGHT_CHANNEL6)
            {
                // 切光
                if(!m_cutlightDlg->changeLight(channel))
                {
                    addLog(tr("变更测试环境失败，测试环境：%1，errorinfo:%2，关闭模组").arg(channel).arg(m_cutlightDlg->LastErrorString()));
                    cmd = SM_CMD_CAM_STOP;
                }
                // 切光后延时
                QThread::msleep(m_delayTimes);
            }
        }

        emit signal_sendSocketCmd(cmd,QString::number(ui->spinBox_SN->value()));
        //        m_timerCheckSocket->start();
    }
}

void MainWindow::moduleHasClosed(int idx)
{
    Q_UNUSED(idx)
    m_moduleHasCloedCnt++;
    if(m_moduleHasCloedCnt == m_hasConnectCnt)
    {
        m_moduleHasCloedCnt = 0;
        // 模组关闭，使能 close program、open camera、close camera、sn、cmd
        //        ui->actionOpen_Program->setEnabled(true);
        ui->actionClose_Program->setEnabled(true);
        ui->actionOpen_Camera->setEnabled(true);
        ui->actionClose_Camera->setEnabled(true);
        ui->spinBox_SN->setEnabled(true);
        ui->spinBox_cmd->setEnabled(true);
        ui->pushButton_startTest->setEnabled(true);
        ui->pushButton_sendCmd->setEnabled(true);
        ui->pushButton_startTest->setText(tr("开始\n测试"));
    }
}

void MainWindow::restoreUI()
{
    //    ui->pushButton_sendCmd->setEnabled(true);
    //    ui->pushButton_startTest->setEnabled(true);
}

void MainWindow::showCopyResult(const QString &str, int ccmidx)
{

    QApplication::restoreOverrideCursor();

    // 获取复制程序结果
    if(str.indexOf("PASS") != -1)
    {
        addLog(tr("Camera_%1 ").arg(ccmidx) + str,RightLevel);
        timerCopyFiles->stop();  // 复制结束，停止定时器
    }
    else if(str.indexOf("FAIL") != -1)
    {
        addLog(tr("Camera_%1 ").arg(ccmidx) + str,ErrorLevel);
        timerCopyFiles->stop();  // 复制结束，停止定时器
    }
    else
    {
        addLog(tr("Camera_%1 ").arg(ccmidx) + str,GrayLevel);
    }


    if(!timerCopyFiles->isActive())
    {
#if 0  // 加载文件时未断开连接，所以这里不需要再重新连接
        if(m_bLoadDccf)  // 如果在复制程式之前，加载了dccf，那么在复制程式完成后需要重新监听
            init();
#endif
        // 复制结束后，使能 copy loadDccf、open program、发送指令、开始测试,MES设置
        ui->actionCopy_Program->setEnabled(true);
        ui->actionLoad_DCCF->setEnabled(true);
        ui->actionOpen_Program->setEnabled(true);
        ui->pushButton_sendCmd->setEnabled(true);
        ui->actionClose_Mes->setEnabled(true);
        ui->spinBox_connectCnt->setEnabled(true);
        ui->actionOpen_Program->setEnabled(true);
        // 显示复制所用时间
        addLog(tr("复制程序用时：%1ms.").arg(m_startTime.msecsTo(QDateTime::currentDateTime())),NormalLevel);
    }

}

void MainWindow::processStateChanged(QProcess::ProcessState state)
{
    if(QProcess::NotRunning == state)
    {
        addLog(tr("Not Running"));
        m_hasProcessClosed++;
        qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "] has closed process:" << m_hasProcessClosed;
        if(m_hasProcessClosed == m_process.size())  // 所有窗口都关闭了
        {
            ui->actionOpen_Program->setEnabled(true);
            ui->actionClose_Mes->setEnabled(true);
            ui->actionClose_Program->setEnabled(false);
            ui->actionOpen_Camera->setEnabled(false);
            ui->actionClose_Camera->setEnabled(false);
            ui->pushButton_sendCmd->setEnabled(false);
            ui->pushButton_startTest->setEnabled(false);
        }
    }
    else if(QProcess::Starting == state)
    {
        addLog(tr("Starting"));
        // 开始启动
        ui->spinBox_connectCnt->setEnabled(false);
    }
    else if(QProcess::Running == state)
    {
        addLog(tr("Running"));
        m_hasProcessClosed--;
        if(m_hasProcessClosed == 0)  // 所有测试程序都打开了
        {
            // 恢复鼠标样式
            QApplication::restoreOverrideCursor();
            ui->actionOpen_Program->setEnabled(false);
            ui->actionClose_Mes->setEnabled(false);
        }
    }
}

void MainWindow::showProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << tr("showFinished:") << exitCode << exitStatus;
}

void MainWindow::showCutLightMessage(const QString &msg, int level)
{
    addLog(msg,(LogLevel)level);
}

void MainWindow::comChanged(const QString &com)
{
    m_com = com;
}

void MainWindow::on_pushButton_sendCmd_clicked()
{
    m_mapSocketState.clear();
    for(int i=0;i<m_hasConnectCnt;++i)
        m_mapSocketState.insert(i,State_Testing);

    emit signal_restoreSocketState();
    const int cmd = ui->spinBox_cmd->value();
    QString sn = QString::number(ui->spinBox_SN->value());
    if(SM_CMD_CLOSE_EXE == cmd || 0 == ui->spinBox_SN->value())
        sn = "";
    emit signal_sendSocketCmd(cmd,sn);
}

void MainWindow::init()
{
#if 0
    if(thread && thread->isRunning())
    {
        thread->quit();
        thread->wait();
    }

    const int port = ui->spinBox_port->value();
    const int ccmtestCnt = ui->spinBox_connectCnt->value();

    thread = new QThread;
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);  // 线程退出，自动清理
    for(int i=0;i<ccmtestCnt;++i)
    {
        worker = new Worker(port+i,i);
        qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "] \tworker: " << worker;
        connect(thread,&QThread::finished,worker,&Worker::deleteLater);  // 线程退出，自动清理
        connect(thread,&QThread::started,worker,&Worker::init);  // 线程开始，对Worker中的成员变量初始化

        connect(worker,SIGNAL(signal_sendWorkerMessage(LogLevel,int,QString)),
                this,SLOT(displaySocketMessage(LogLevel,int,QString)));  // worker发送信息，mainwindow显示到界面上
        connect(this,&MainWindow::signal_sendSocketCmd,worker,&Worker::on_sendSocketCmd);  // mainwindow发送指令，worker接收指令向socket发送指令

        worker->moveToThread(thread);
    }

    thread->start();
#else
    clearWorkerThreads();

    m_mapSocketState.clear();

    const int port = ui->spinBox_port->value();
    const int ccmtestCnt = ui->spinBox_connectCnt->value();
    const QString ip = ui->lineEdit_IP->text();
    m_preConnectCnt = ccmtestCnt;
    m_hasConnectCnt = 0;
    m_threads.resize(ccmtestCnt);
    for(int i=0;i<ccmtestCnt;++i)
    {
        m_threads[i] = new QThread;
        worker = new Worker(ip,port+i,i);
        // 连接信号槽之前调用moveToThread，那么连接信号槽时就不必指定第五个参数
        worker->moveToThread(m_threads[i]);
        qDebug() << "[Line:" << __LINE__ << "Function:" <<  __FUNCTION__ << "]"  << "\tworker: " << worker;
        connect(m_threads[i],&QThread::finished,m_threads[i],&QThread::deleteLater);  // 线程退出，自动清理
        connect(m_threads[i],&QThread::finished,worker,&Worker::deleteLater);  // 线程退出，自动清理
        connect(m_threads[i],&QThread::started,worker,&Worker::init);  // 线程开始，对Worker中的成员变量初始化

        connect(this,&MainWindow::signal_restoreSocketState,worker,&Worker::on_restoreSocketState);
        connect(this,&MainWindow::signal_sendSocketCmd,worker,&Worker::on_sendSocketCmd);  // mainwindow发送指令，worker接收指令向socket发送指令
        connect(worker,SIGNAL(signal_sendWorkerMessage(LogLevel,int,QString)),
                this,SLOT(displaySocketMessage(LogLevel,int,QString)));  // worker发送信息，mainwindow显示到界面上
        connect(worker,&Worker::signal_newConnect,this,&MainWindow::hasNewConnection);  // 有新的连接，接收处理
        connect(worker,&Worker::signal_getSocketState,this,&MainWindow::getSocketState);  // 获取socket状态
        connect(worker,&Worker::signal_moduleClosed,this,&MainWindow::moduleHasClosed);  // 模组已经关闭信号
        connect(worker,&Worker::signal_restoreUI,this,&MainWindow::restoreUI);
        connect(worker,&Worker::signal_socketDisconnect,this,&MainWindow::hasSocketDisconnect);  // worker发送socket断开连接，处理断开连接

        m_threads[i]->start();

        m_mapSocketState.insert(i,State_Testing);
    }


#endif
}

void MainWindow::initParams()
{
    //    thread = nullptr;
    worker = nullptr;
    clearWorkerThreads();
    clearCopyThreads();
    clearProcessPointer();

    m_timerCheckSocket = new QTimer(this);
    m_timerCheckSocket->setInterval(200);
    connect(m_timerCheckSocket,&QTimer::timeout,this,&MainWindow::checkSocketState);

    m_inspections.clear();

    // 复制文件时定时显示log
    timerCopyFiles = new QTimer(this);
    timerCopyFiles->setInterval(500);
    m_bLoadDccf = false;

    m_hasProcessClosed = 0;

    m_cutlightDlg = new CutLightDlg(m_com,this);
    connect(m_cutlightDlg,&CutLightDlg::signal_showCutlightResult,this,&MainWindow::showCutLightMessage);
    connect(m_cutlightDlg,&CutLightDlg::signal_comChange,this,&MainWindow::comChanged);

}

void MainWindow::initStatus()
{
    if(m_windowStyle == "windows")
        on_actionWindows_triggered(true);
    else if(m_windowStyle == "fusion")
        on_actionFusion_triggered(true);
    else if(m_windowStyle == "windowsvista")
        on_actionwindowsvista_triggered(true);


    projectLabel = new QLabel;
    projectLabel->setText(QFileInfo(m_dccfFile).baseName());
    // 设置label的字体
    QFont font;
    font.setFamily("Consolas");
    font.setPixelSize(30);
    font.setBold(true);
    projectLabel->setFont(font);
    projectLabel->setStyleSheet("color:blue;");
    projectLabel->setMinimumWidth(200);
    ui->toolBar_projectInfo->addWidget(projectLabel);

    ui->statusBar->addWidget(new QLabel(tr("连接状态：")));
    m_connecStatusLabel = new QLabel;
    m_connecStatusLabel->setPixmap(QPixmap(":/images/FAIL.png").scaled(SCALED_IMAGE_WIDTH,SCALED_IMAGE_HEIGHT));
    ui->statusBar->addWidget(m_connecStatusLabel);

    ui->pushButton_sendCmd->setEnabled(false);
    ui->pushButton_startTest->setEnabled(false);

    ui->lineEdit_IP->setText(m_ip);
    ui->spinBox_connectCnt->setValue(m_connectCnt);
    ui->spinBox_port->setValue(m_port);

    if(m_hasProcessClosed == 0)
    {
        ui->actionClose_Program->setEnabled(false);
        ui->actionOpen_Camera->setEnabled(false);
        ui->actionClose_Camera->setEnabled(false);
    }

    const QString dccfFile = QString("%1/%2_%3/%4").arg(PATH_DCCMTEST).arg(PATH_NAME_CAMERA).arg(0).arg(QFileInfo(m_dccfFile).fileName());
    setMESStatus(dccfFile);

    ui->spinBox_cutLightDelay->setValue(m_delayTimes);

    m_strExtraTool = QApplication::applicationDirPath() + PATH_EXTRATOOL + PATH_EXTRATOOL_NAME;


}

void MainWindow::on_spinBox_port_valueChanged(int arg1)
{
    if(m_port != -1 && m_port != arg1)
        init();
}

void MainWindow::on_spinBox_connectCnt_valueChanged(int arg1)
{
    if(m_connectCnt != -1 && m_connectCnt != arg1)
    {
        init();
        on_actionCopy_Program_triggered();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{


    writeSettings();
#if 0
    if(thread && thread->isRunning())
    {
        thread->quit();
        thread->wait();
    }
#else
    clearCopyThreads();
    clearWorkerThreads();
    clearProcessPointer();
#endif
    event->accept();

}

void MainWindow::on_pushButton_startTest_clicked()
{
    addLog(tr("开始测试"));
    // 开始测试过程中，disabled : open program、close program、open camera、close camera、sn、cmd
    ui->actionOpen_Program->setEnabled(false);
    //    ui->actionClose_Program->setEnabled(false);
    ui->actionOpen_Camera->setEnabled(false);
    ui->actionClose_Camera->setEnabled(false);
    ui->spinBox_SN->setEnabled(false);
    ui->spinBox_cmd->setEnabled(false);
    ui->pushButton_startTest->setEnabled(false);
    ui->pushButton_sendCmd->setEnabled(false);
    ui->pushButton_startTest->setText(tr("测试中\n..."));
    m_mapSocketState.clear();
    for(int i=0;i<m_hasConnectCnt;++i)
        m_mapSocketState.insert(i,State_Testing);

    emit signal_restoreSocketState();
    m_cmdQueue.clear();
    // 获取测试命令
    m_cmdQueue.enqueue(SM_CMD_CAM_START);
    for(int i=0;i<m_inspections.size();i++)
    {
        m_cmdQueue.enqueue(i+1);
    }
    m_cmdQueue.enqueue(SM_CMD_CAM_STOP);

    int cmd = m_cmdQueue.dequeue();
    emit signal_sendSocketCmd(cmd,QString::number(ui->spinBox_SN->value()));
    //    m_timerCheckSocket->start();
}

void MainWindow::on_actionWindows_triggered(bool checked)
{
    Q_UNUSED(checked)
    ui->actionFusion->setChecked(false);
    ui->actionWindows->setChecked(true);
    ui->actionwindowsvista->setChecked(false);
    addLog(tr("设置窗口样式：windows"));
    QApplication::setStyle("windows");
}

void MainWindow::on_actionFusion_triggered(bool checked)
{
    Q_UNUSED(checked)
    ui->actionFusion->setChecked(true);
    ui->actionWindows->setChecked(false);
    ui->actionwindowsvista->setChecked(false);
    addLog(tr("设置窗口样式：fusion"));
    QApplication::setStyle("fusion");
}

void MainWindow::on_actionwindowsvista_triggered(bool checked)
{
    Q_UNUSED(checked)
    ui->actionFusion->setChecked(false);
    ui->actionWindows->setChecked(false);
    ui->actionwindowsvista->setChecked(true);
    addLog(tr("设置窗口样式：windowsvista"));
    QApplication::setStyle("windowsvista");
}

void MainWindow::on_actionLoad_DCCF_triggered()
{
    addLog(tr("加载DCCF"));
    // 加载dccf文件
    m_dccfFile = QFileDialog::getOpenFileName(this,tr("Load DCCF"),QFileInfo(m_dccfFile).filePath(),tr("dccf files(*.dccf);;AllFiles(*.*)"));
    m_bLoadDccf = (!m_dccfFile.isEmpty());
    if(m_dccfFile.isEmpty())
    {
        addLog(tr("未选择文件."),ErrorLevel);
        return;
    }
    addLog(tr("加载测试程序dccf : %1").arg(m_dccfFile));

    // 加载完dccf后设置mes状态
    setMESStatus(m_dccfFile);
#if 0  // 加载文件不需要释放监听连接，仅在Ip、port、连接个数改变的
    if(m_bLoadDccf)  // 如果重新加载了dccf文件，则释放之前监听的资源
    {
        // 停止监听socket连接，删除已经创建的资源
        clearWorkerThreads();
        m_preConnectCnt = 0;  // 设置之前的socket监听个数为0
    }
#endif
    m_mapSocketState.clear();

    // 将dccf名称显示到界面上
    projectLabel->setText(QFileInfo(m_dccfFile).baseName());
    // 加载测试环境及错误代码
    if(!loadInspectionsInfo(m_dccfFile))
    {
        addLog(tr("加载测试项、环境及错误代码失败，请检查dccf文件是否存在"),ErrorLevel);
        return;
    }

    // 加载完dccf后立即复制程式
    on_actionCopy_Program_triggered();

}

void MainWindow::on_actionCopy_Program_triggered()
{
    // 获取dccf所在路径
    const QString dccfPath = QFileInfo(m_dccfFile).absolutePath();
    qDebug() << tr("原始测试程序路径：") << dccfPath;
    if(m_dccfFile.isEmpty() || !QFile(m_dccfFile).exists())
    {
        // dccf为空或者dccf不存在
        addLog(tr("dccf为空或不存在，请重新加载点亮档：%1").arg(m_dccfFile),ErrorLevel);
        return;
    }

    ui->actionCopy_Program->setEnabled(false);  // 复制过程中不允许再点击复制
    // 复制过程中不允许修改ip、port、connectCnt, MES设置、发送指令、开始测试设置为disabled, 不允许 loadDccf、开启测试程式
    ui->lineEdit_IP->setEnabled(false);
    ui->spinBox_port->setEnabled(false);
    ui->spinBox_connectCnt->setEnabled(false);
    ui->pushButton_sendCmd->setEnabled(false);
    ui->pushButton_startTest->setEnabled(false);
    ui->actionLoad_DCCF->setEnabled(false);
    ui->actionOpen_Program->setEnabled(false);
    ui->actionClose_Mes->setEnabled(false);


    QApplication::setOverrideCursor(Qt::WaitCursor);  // 设置鼠标为等待样式

    // 复制程式之前先从界面中获取需要的变量
    ui2cache();

    addLog(tr("复制程式..."),NormalLevel);
    // 如果目标目录不存在创建目录，如果目录存在，删除路径
    if(QDir(PATH_DCCMTEST).exists() && !QDir(PATH_DCCMTEST).removeRecursively())
    {
        addLog(tr("删除原始文件夹%1失败").arg(PATH_DCCMTEST),ErrorLevel);
        return;
    }
    addLog(tr("删除原始文件夹%1成功").arg(PATH_DCCMTEST),RightLevel);
    if(!QDir(PATH_DCCMTEST).exists())
        QDir(PATH_DCCMTEST).mkdir(PATH_DCCMTEST);


    // 复制程式，使用线程复制
    clearCopyThreads();
    m_copyThreads.resize(m_connectCnt);
    for(int i=0;i<m_connectCnt;i++)
    {
        const QString newPath = QString("%1/%2_%3").arg(PATH_DCCMTEST).arg(PATH_NAME_CAMERA).arg(i);
        qDebug() << "[Line:" << __LINE__ << "Function:"  << __FUNCTION__ << "] newPath:" << newPath;
        CopyThread *thread = new CopyThread(dccfPath,newPath,i);  // 创建复制线程
        connect(thread,&CopyThread::signal_copyFinish,this,&MainWindow::showCopyResult);
        connect(thread,&CopyThread::finished,thread,&CopyThread::deleteLater);  // 线程结束后，自动销毁
        m_copyThreads[i] = thread;

        addLog(tr("开始复制测试程式到%1").arg(newPath));

        m_startTime = QDateTime::currentDateTime();
        thread->start();
        if(!timerCopyFiles->isActive())
            timerCopyFiles->start();
    }

}

void MainWindow::on_actionSave_UI_triggered()
{
    addLog(tr("保存界面设置"));
    writeSettings();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_Program_triggered()
{
    ui2cache();
    const int exeCnt = ((m_copyThreads.size() == 0) ? m_connectCnt : qMin(m_copyThreads.size(),m_connectCnt));

    if(m_inspections.size() == 0)
    {
        // 加载测试环境及错误代码
        if(!loadInspectionsInfo(m_dccfFile))
        {
            addLog(tr("加载测试项、环境及错误代码失败，请检查dccf文件是否存在"),ErrorLevel);
            return;
        }
    }


    // 点击打开测试程序时，设置菜单项为disabled，直到测试程序关闭
    ui->actionOpen_Program->setEnabled(false);
    QApplication::setOverrideCursor(Qt::WaitCursor);  // 设置鼠标为等待状态
    addLog(tr("打开测试程序..."));
    // 获取界面设置
    QString exePath;
    QStringList arguments;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    const QRect avaliableRect = QApplication::desktop()->availableGeometry();  // 屏幕几何大小
#else
    const QRect avaliableRect = QGuiApplication::primaryScreen()->geometry();  // 屏幕几何大小
#endif

    QString strlog;

    clearProcessPointer();
    m_process.resize(exeCnt);
    m_hasProcessClosed = exeCnt;
    for(int i=0;i<exeCnt;i++)
    {
        arguments.clear();
        exePath = QString("%1/%2_%3/%4").arg(PATH_DCCMTEST).arg(PATH_NAME_CAMERA).arg(i).arg(PATH_NAME_EXE);
        if(!QFileInfo(exePath).exists())
        {
            addLog(tr("请检查测试程序是否存在 : %1").arg(exePath),ErrorLevel);
            continue;
        }
        arguments.append(tr("%1").arg(i+1));  // 程序编号
        arguments.append(tr("%1").arg(i));  // 工装编号
        arguments.append(tr("%1").arg(avaliableRect.width()/exeCnt * i));  // 程序启动坐标x
        arguments.append(tr("%1").arg(1));  // 程序启动坐标y
        arguments.append(tr("%1").arg(avaliableRect.width()/exeCnt));  // 窗口大小宽
        arguments.append(tr("%1").arg(avaliableRect.height()/4*3));  // 窗口大小高
        arguments.append(tr("%1").arg(m_ip));  // ip
        arguments.append(tr("%1").arg(m_port + i));  // port

        // 启动程序
        strlog.clear();

        strlog.append(tr("%1 %2").arg(exePath).arg(arguments.join(" ")));
        addLog(strlog);

        QProcess *process = new QProcess(this);
        m_process[i] = process;
        // 测试程序已经运行即打开，发送started()信号，捕捉该信号
        // 读取所有运行结果
//        connect(process,&QProcess::started,this,&MainWindow::processStarted);  // 打开测试程序，程序已经运行后，发送started()信号
        connect(process,&QProcess::stateChanged,this,&MainWindow::processStateChanged);  // 测试程序状态改变
        connect(process,&QProcess::readyRead,this,[=](){
            QTextCodec *codec = QTextCodec::codecForLocale();
            addLog(tr("camera_%1 showResult: %2").arg(i).arg(codec->toUnicode(process->readAll())));
        });
        connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(showProcessFinished(int,QProcess::ExitStatus)));


        // 运行进程即打开测试程序
        process->start(exePath,arguments);
    }
}

void MainWindow::on_actionClose_Program_triggered()
{
    if(m_hasProcessClosed < m_process.size())
    {
        clearProcessPointer();
    }

    ui->pushButton_startTest->setText(tr("开始\n测试"));
}

void MainWindow::on_actionOpen_Camera_triggered()
{
    ui->spinBox_cmd->setValue(SM_CMD_CAM_START);  // 设置关闭模组命令
    on_pushButton_sendCmd_clicked();
}

void MainWindow::on_actionClose_Camera_triggered()
{
    ui->spinBox_cmd->setValue(SM_CMD_CAM_STOP);  // 设置关闭模组命令
    on_pushButton_sendCmd_clicked();
}

void MainWindow::on_actionClear_Log_triggered()
{
    ui->textEdit_log->clear();
}

void MainWindow::on_lineEdit_IP_returnPressed()
{
    init();
}

void MainWindow::on_actionClose_Mes_triggered()
{
    if(!QFile(m_strExtraTool).exists())
    {
        addLog(tr("更改MES状态的工具不存在：%1").arg(m_strExtraTool),ErrorLevel);
        return;
    }
    int mesStatus = 0;
    if(ui->actionClose_Mes->text() == tr("Close MES"))
    {
        addLog(tr("关闭MES"));
        mesStatus = 0;
        ui->actionClose_Mes->setText(tr("Open MES"));
        ui->actionClose_Mes->setIcon(QIcon(":/images/MES_OFF.png"));
    }
    else
    {
        addLog(tr("打开MES"));
        mesStatus = 1;
        ui->actionClose_Mes->setText(tr("Close MES"));
        ui->actionClose_Mes->setIcon(QIcon(":/images/MES_ON.png"));
    }

    // 修改ccmTest中的几个测试窗口中的dccf中的mes接口
    for(int i=0;i<m_connectCnt;++i)
    {
        const QString dccfFile = QString("%1/%2_%3/%4").arg(PATH_DCCMTEST).arg(PATH_NAME_CAMERA).arg(i).arg(QFileInfo(m_dccfFile).fileName());
        if(QFile(dccfFile).exists())
        {
            QProcess *process = new QProcess(this);
            QStringList cmdList;
            cmdList.append(QString::number(0));
            cmdList.append(dccfFile);
            cmdList.append(QString::number(mesStatus));
            process->start(m_strExtraTool,cmdList);
        }

    }
}

void MainWindow::on_actionCut_Liight_triggered()
{
    addLog(tr("切光设置"));
    m_cutlightDlg->exec();
}

void MainWindow::on_spinBox_cutLightDelay_valueChanged(int arg1)
{
    m_delayTimes = arg1;
}

void MainWindow::on_actionSNBinding_triggered()
{
    // 绑定测试盒
    if(!QFile(m_strExtraTool).exists())
    {
        addLog(tr("绑定工具不存在，%1").arg(m_strExtraTool),ErrorLevel);
        return;
    }

    bindingDlg = new SNBindingDialog(m_strExtraTool);
    bindingDlg->exec();
}

void MainWindow::on_actionChangeUser_triggered()
{
    addLog(tr("更改用户登录模式"));
    LoginDlg dlg;
    dlg.exec();
    bool isAdmin = dlg.isAdministrator();
    addLog(tr("切换为%1模式").arg( isAdmin ? tr("管理员") : tr("操作员") ));
    setUiEnable(dlg.isAdministrator());
}
