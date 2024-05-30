#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>

class CopyThread : public QThread
{
    Q_OBJECT
    enum ErrorCopyThread{CopyFail=1};
public:
    CopyThread(const QString &oldpath,const QString &newpath,int ccmIndex);
    ~CopyThread();

private:
    bool copyAllFiles(const QString &oldPath,const QString newPath);
    bool clearAllFiles(const QString &filepath);
protected:
    void run() override;
signals:
    void signal_copyFinish(const QString log,int ccmIndex);

private:
    QString m_oldPath;
    QString m_newPath;
    int m_ccmIndex;
};

#endif // COPYTHREAD_H
