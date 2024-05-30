#pragma execution_character_set("utf-8")
#include "controllerserver.h"
#include "DccmSocket.h"

#include "worker.h"


Worker::Worker(QObject *parent) : QObject(parent),m_idx(0),m_ip("127.0.0.1"),m_port(10086)
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    msg = nullptr;
    m_state = State_Finished;
    m_socket = nullptr;
}

Worker::Worker(int port, int idx, QObject *parent):
    QObject(parent),
    m_ip("127.0.0.1"),
    m_port(port),
    m_idx(idx)
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    msg = nullptr;
    m_state = State_Finished;
    m_socket = nullptr;
}

Worker::Worker(const QString &ip, int port, int idx, QObject *parent):
    QObject(parent),
    m_ip(ip),
    m_port(port),
    m_idx(idx)
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    msg = nullptr;
    m_state = State_Finished;
    m_socket = nullptr;
}

Worker::~Worker()
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
    SAFE_DELETE_PTR(msg);
    if(m_server && m_server->isListening())
    {
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("断开连接，ip:%1, port:%2").arg(m_ip).arg(m_port));
        m_server->close();
    }
}

void Worker::init()
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;

    m_server = new ControllerServer(m_idx,this);
    if(!m_server->listen(QHostAddress(m_ip),m_port))
    {
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("监听socket连接失败，ip:%3, port:%1, error:%2")
                                      .arg(m_port)
                                      .arg(m_server->errorString())
                                      .arg(m_ip));
        return;
    }
    emit signal_sendWorkerMessage(NormalLevel,m_idx,tr("监听socket连接，ip:%2, port:%1").arg(m_port).arg(m_ip));
    connect(m_server,&ControllerServer::signal_hasNewConnect,this,&Worker::hasNewConnection);
    m_socket = nullptr;

    m_state = State_Finished;

}

void Worker::hasNewConnection(qintptr handle, int idx)
{
    Q_UNUSED(idx)

    m_socket = new QTcpSocket(this);

    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "] m_socket:" << m_socket;

    m_socket->setSocketDescriptor(handle);
    connect(m_socket,&QTcpSocket::disconnected,this,&Worker::signal_socketDisconnect);
    connect(m_socket,&QTcpSocket::disconnected,m_socket,&QTcpSocket::deleteLater);  // socket断开连接，自动清理
    connect(m_socket,&QTcpSocket::readyRead,this,&Worker::hasReadyRead);  // socket中有数据准备读取
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displaySocketError(QAbstractSocket::SocketError)));  // socket发生错误，显示错误信息
    emit signal_newConnect();  // 有新的连接，发送信号
    emit signal_sendWorkerMessage(NormalLevel,m_idx,tr("有新的socket连接，ip:%1, port:%2")
                                  .arg(m_socket->peerAddress().toString())
                                  .arg(m_socket->peerPort()));

    msg = new SockMsg;
    msg->CmdID = 0;
    msg->Status = 0;
    msg->Result = 0;
    memset(msg->ErrInfo,0,sizeof(msg->ErrInfo));
    memset(msg->TestData,0,sizeof(msg->TestData));
}

void Worker::hasReadyRead()
{
    readSocketData();
}

void Worker::on_sendSocketCmd(int cmd, const QString &sn)
{
    sendSocketData(cmd,sn,this->m_idx);
}

void Worker::displaySocketError(QAbstractSocket::SocketError error)
{
    if(m_socket && m_socket->isValid())
    {
        qDebug() << tr("关闭套接字");
        m_socket->close();
    }
    if(QAbstractSocket::RemoteHostClosedError == error )
    {
        emit signal_sendWorkerMessage(NormalLevel,m_idx,tr("socket连接已经关闭：%1").arg(m_socket->errorString()));
    }
    else
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("socket发生错误：%1").arg(m_socket->errorString()));
}

void Worker::on_restoreSocketState()
{
    m_state = State_Finished;
}

bool Worker::readSocketData()
{
#if 0
    if(m_socket->bytesAvailable() == 0)
    {
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("socket中暂时没有可读的数据"));
        return false;
    }
    else
    {
        // 从socket中读取数据
        QByteArray readData = m_socket->readAll();

        if(sizeof(SockMsg) != readData.size())
        {
            emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("收到消息长度：%1，与协议长度 %2 不符")
                                          .arg(readData.size()).arg(sizeof(SockMsg)));
            return false;
        }

        //        SockMsg *msg = new SockMsg;
        memcpy(msg,readData,sizeof(SockMsg));

        emit signal_sendWorkerMessage(ReceiveLevel,m_idx,tr("收到消息：%1/%2/%3/%4/%5")
                                      .arg(msg->CmdID).arg(msg->Status).arg(msg->Result)
                                      .arg(QString::fromLocal8Bit(msg->ErrInfo))
                                      .arg(QString::fromLocal8Bit(msg->TestData)));
    }
#else
    // 从socket中读取数据
    QByteArray readData = m_socket->readAll();

    if(sizeof(SockMsg) != readData.size())
    {
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("收到消息长度：%1，与协议长度 %2 不符")
                                      .arg(readData.size()).arg(sizeof(SockMsg)));
        return false;
    }

    //        SockMsg *msg = new SockMsg;
    memcpy(msg,readData,sizeof(SockMsg));

    emit signal_sendWorkerMessage(ReceiveLevel,m_idx,tr("收到消息：%1/%2/%3/%4/%5")
                                  .arg(msg->CmdID).arg(msg->Status).arg(msg->Result)
                                  .arg(QString::fromLocal8Bit(msg->ErrInfo))
                                  .arg(QString::fromLocal8Bit(msg->TestData)));
#endif
    QString errmsg = "";
    bool bParseResult = parseSocketMessage(msg,errmsg);
    if(!errmsg.isEmpty())
        emit signal_sendWorkerMessage( (bParseResult ? RightLevel: ErrorLevel),m_idx,errmsg);
    emit signal_getSocketState(m_state, m_idx);  // 发送

    return bParseResult;
}

bool Worker::parseSocketMessage(const SockMsg *msg ,QString &errmsg)
{
    bool bret = false;
    errmsg.clear();  // 清空错误信息
    int cmd = msg->CmdID;
    int status = msg->Status;
    int result = msg->Result;
    QString errinfo = QString::fromLocal8Bit(msg->ErrInfo);
    QString testdata = QString::fromLocal8Bit(msg->TestData);

    if(cmd>0 && cmd<100)  // 测试项测试命令
    {
        switch (status) {
        case SM_STATUS_NO_EXECUTE:  // 不响应执行
        {
            errmsg = QString("模组未响应执行，errinfo:%1").arg(errinfo);
            setSocketState(State_Testing);
        }
            break;
        case SM_STATUS_EXECUTE:  // 响应执行
        {
            if(SM_INVLID == result )  // 开始响应执行，测试中
            {
                errmsg = QString("测试程序响应中");
                setSocketState(State_Testing);
                bret = true;
            }
            else if(SM_RESULT_OK == result)
            {
                errmsg = QString("测试OK");
                setSocketState(State_Finished);
                bret = true;
            }
            else
            {
                errmsg = QString("测试FAIL,errinfo:%1").arg(errinfo);
                setSocketState(State_Failed);
            }
        }
            break;
        case SM_STATUS_INSP_END:  // 测试结束
        {
            if(SM_RESULT_OK == result)
            {
                errmsg = QString("测试项测试结束，结果OK");
                setSocketState(State_Finished);
                bret = true;
            }
            else
            {
                errmsg = QString("测试项测试结束，结果NG，errinfo:[%1]").arg(errinfo);
                setSocketState(State_Failed);
            }
        }
            break;
        default:
        {
            errmsg = QString("未知状态，%1/%2/%3/%4/%5").arg(cmd).arg(status).arg(result).arg(errinfo).arg(testdata);
            setSocketState(State_Failed);
        }
            break;
        }
    }
    else if( SM_CMD_CAM_START == cmd)  // 点亮指令
    {
        if(SM_STATUS_EXECUTE == status && SM_RESULT_OK == result)
        {
            errmsg = QString("模组启动成功, testdata:[%1]").arg(testdata);
            setSocketState(State_Finished);
            bret = true;
        }
        else if(SM_STATUS_NO_EXECUTE == status && SM_RESULT_CAM_RUNNING == result)
        {
            errmsg = QString("模组处于运行状态");
            setSocketState(State_Testing);
        }
        else
        {
            errmsg = QString("模组启动失败，errinfo:[%1]").arg(errinfo);
            setSocketState(State_FailedAndModuleClose);
        }
    }
    else if(SM_CMD_CAM_STOP == cmd)  // 关闭模组
    {
        if(SM_STATUS_EXECUTE == status)
        {
            if( SM_RESULT_OK == result || RESULT_INSPECTION_OK == result )
            {
                errmsg = QString("模组已经关闭，测试OK，testdata[%2]").arg(testdata);
                setSocketState(State_Finished);
                emit signal_moduleClosed(m_idx);
                bret = true;
            }
            else if((result > RESULT_INSPECTION_NULL && result < RESULT_INSPECTION_END) ||
                    ((result >= ERROR_CLINDER_MOVE && result < ERROR_3307)))
            {
                errmsg = QString("模组已经关闭，测试NG，errinfo:[%1], result:[%2]").arg(errinfo).arg(result);
                setSocketState(State_FailedAndModuleClose);
                emit signal_moduleClosed(m_idx);
            }
            else
            {
                errmsg = QString("模组关闭失败，errinfo:[%1], testdata[%2], result:[%3]").arg(errinfo).arg(testdata).arg(result);
                setSocketState(State_Failed);
            }
        }
    }

    return bret;
}

bool Worker::sendSocketData(int cmd, const QString &sn,int idx)
{
    if(this->m_idx != idx)
        return false;
    // 发送消息之前判断当前测试状态
    const int state = getSocketState();
    if( State_FailedAndModuleClose == state )  // 测试失败关闭模组状态下仅接收点亮模组的指令
    {
        if(SM_CMD_CAM_START != cmd)
        {
//            emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("要发送指令：%1，当前状态下仅接受模组点亮指令(101)").arg(cmd));
            return false;
        }
    }
    else if(State_Failed == state)  // 测试失败，将发送的指令修改为关闭模组的指令,并设置状态为测试失败并关闭模组状态
    {
        cmd = SM_CMD_CAM_STOP;
        setSocketState(State_FailedAndModuleClose);
        emit signal_getSocketState(m_state, m_idx);  // 发送
    }
    else  // 其它状态都置为测试中
    {
        setSocketState(State_Testing);
        emit signal_getSocketState(m_state, m_idx);  // 发送
    }

    if(!msg)
    {
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("还没有连接，不能向socket发送指令"));
        setSocketState(State_FailedAndModuleClose);
        emit signal_getSocketState(m_state, m_idx);  // 发送
        emit signal_restoreUI();
        return false;
    }

    //    SockMsg *msg = new SockMsg;
    msg->CmdID = cmd;
    msg->Status = 0;
    msg->Result = 0;
    memset(msg->ErrInfo,0,sizeof(msg->ErrInfo));
    memset(msg->TestData,0,sizeof(msg->TestData));
    emit signal_sendWorkerMessage(SendLevel,m_idx,tr("发送消息：%1/%2/%3/%4/%5")
                                  .arg(msg->CmdID)
                                  .arg(msg->Status)
                                  .arg(msg->Result)
                                  .arg(QString::fromLocal8Bit(msg->ErrInfo))
                                  .arg(QString::fromLocal8Bit(msg->TestData)));

    if(!sn.isEmpty() && sn.toInt() != 0)
    {
        QByteArray testData = sn.toLocal8Bit();
        memcpy(msg->TestData,testData,sizeof(msg->TestData));
    }

    QByteArray sendbuf;
    sendbuf.resize(sizeof(SockMsg));
    memcpy(sendbuf.data(),msg,sizeof(SockMsg));
    if(m_socket)
    {
        if(sizeof(SockMsg) != m_socket->write(sendbuf))
        {
            emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("向socket发送消息失败，err:%1").arg(m_socket->errorString()));
            return false;
        }
    }
    else
    {
        emit signal_sendWorkerMessage(ErrorLevel,m_idx,tr("还未连接，不能发送数据"));
        return false;
    }

    return true;
}

void Worker::setSocketState(int state)
{
    m_mutex.lock();
    m_state = state;
    m_mutex.unlock();
}

int Worker::getSocketState() const
{
    return m_state;
}
