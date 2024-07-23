#ifndef LOTTERYTHREAD_H
#define LOTTERYTHREAD_H
#include "ui/informationsdialog.h"
#include <QThread>
#include <QMutex>

class LotteryThread : public QThread
{
    Q_OBJECT
public:
    explicit LotteryThread(QObject *parent = nullptr);
    ~LotteryThread();

    void processLottery(int count);
    void setLuckyCount(int cnt);
    void setPeopleList(const StaffList &staffList);
signals:
    void sendLuckyID(int id);
    void sendLuckyIds(const QList<int> &ids);
    void sendFinalLuckyIds(const QList<int> &ids);
public slots:
    void stopProcess();
protected:
    void run() override;
private:
    int m_idCount;  // 参与抽奖的总人数
    int m_luckyCount;  // 抽奖的人数
    bool m_run;
    QMutex m_mutex;
    QList<int> m_luckyIds;
    int *m_pOwnerStatus;
};

#endif // LOTTERYTHREAD_H
