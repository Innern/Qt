#include "lotterythread.h"

#include <QTime>
#include <QRandomGenerator>
#include <QSharedPointer>

LotteryThread::LotteryThread(QObject *parent) : QThread(parent)
  , m_run(false)
  , m_idCount(0)
  , m_luckyCount(0)
  , m_pOwnerStatus(nullptr)
{

}

LotteryThread::~LotteryThread()
{
    m_mutex.lock();
    m_run = false;
    m_mutex.unlock();
    wait();
}

void LotteryThread::processLottery(int count)
{
    m_idCount = count;

    m_run = true;
    start();
}

void LotteryThread::setLuckyCount(int cnt)
{
    m_luckyCount = cnt;
}

void LotteryThread::setIntervalTime(int msec)
{
    m_intervalTime = msec;
}

void LotteryThread::setPeopleList(const StaffList &staffList)
{
    if(!m_pOwnerStatus){
        m_pOwnerStatus = new int[staffList.size()];
    }
    memset(m_pOwnerStatus, 0, sizeof(m_pOwnerStatus[0]) * staffList.size());

    for(int i=0;i<staffList.size();++i) {
        m_pOwnerStatus[i] = staffList.at(i).ownPrize;
    }
}

void LotteryThread::stopProcess()
{
    m_mutex.lock();
    m_run = false;
    m_mutex.unlock();
}

void LotteryThread::run()
{
#if QT_VERSION < QT_VERSION_CHECK(5,15,0)
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
#endif

    if(m_idCount<=0 || m_luckyCount<=0){
        return;
    }

    int randomID;
    int *winFlag = new int[m_idCount];
    QSharedPointer<int> pWinFlag(winFlag);
    while (m_run) {
        m_luckyIds.clear();
//        memset(winFlag, 0, sizeof(winFlag[0])*m_idCount);
        memcpy(winFlag, m_pOwnerStatus, sizeof(m_pOwnerStatus[0])*m_idCount);
        for(int i=0;i<m_luckyCount;i++){
            do{
                randomID = QRandomGenerator::global()->bounded(m_idCount);
            }while(winFlag[randomID] != 0);

            winFlag[randomID] = 1;
            m_luckyIds.append(randomID);
        }
        emit sendLuckyID(randomID);
        emit sendLuckyIds(m_luckyIds);
        msleep(m_intervalTime);
    }

    emit sendFinalLuckyIds(m_luckyIds);
    if(m_pOwnerStatus){
        delete[] m_pOwnerStatus;
        m_pOwnerStatus = nullptr;
    }
}
