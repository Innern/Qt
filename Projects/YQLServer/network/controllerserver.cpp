#pragma execution_character_set("utf-8")
#include "controllerserver.h"

#include <QThread>

ControllerServer::ControllerServer(int idx, QObject *parent) :
    QTcpServer(parent),
    m_idx(idx)
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
}

ControllerServer::~ControllerServer()
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
}

void ControllerServer::incomingConnection(qintptr handle){
    emit signal_hasNewConnect(handle,m_idx);
}
