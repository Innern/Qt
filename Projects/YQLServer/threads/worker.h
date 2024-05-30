#ifndef WORKER_H
#define WORKER_H

#include "commondef.h"
#include "DccmSocket.h"

#include <QObject>
#include <QtNetwork>
#include <QMutex>

class ControllerServer;

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);
    Worker(int port,int idx,QObject *parent = nullptr);
    Worker(const QString &ip,int port,int idx,QObject *parent = nullptr);
    ~Worker();
signals:
    void signal_sendWorkerMessage(LogLevel level,int idx,const QString &msg);
    void signal_socketDisconnect();
    void signal_newConnect();
    void signal_getSocketState(int state,int idx);
    void signal_moduleClosed(int idx);
    void signal_restoreUI();
public slots:
    void init();
    void hasNewConnection(qintptr handle,int idx);
    void hasReadyRead();
    void on_sendSocketCmd(int cmd, const QString &sn);
    void displaySocketError(QAbstractSocket::SocketError);
    void on_restoreSocketState();
private:
    bool readSocketData();
    bool parseSocketMessage(const SockMsg* msg,QString &errmsg);
    bool sendSocketData(int cmd, const QString &sn="", int idx=0);
    void setSocketState(int state);
    int getSocketState() const;

private:
    ControllerServer *m_server;
    QTcpSocket *m_socket;

    QString m_ip;
    int m_port;
    int m_idx;

    SockMsg *msg;

    QMutex m_mutex;
    int m_state;  // 0:测试完成，等待命令；1:测试中； 2:测试失败；3:测试失败，模组关闭；
};

#endif // WORKER_H
