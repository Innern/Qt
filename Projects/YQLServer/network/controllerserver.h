#ifndef CONTROLLERSERVER_H
#define CONTROLLERSERVER_H



#include <QTcpServer>

class ControllerServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ControllerServer(int idx,QObject *parent = nullptr);
    ~ControllerServer();

signals:
    void signal_hasNewConnect(qintptr handle,int idx);
public slots:

protected:
    void incomingConnection(qintptr handle) override;
private:
    int m_idx;
};

#endif // CONTROLLERSERVER_H
