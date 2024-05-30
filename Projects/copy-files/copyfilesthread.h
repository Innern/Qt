#ifndef COPYFILESTHREAD_H
#define COPYFILESTHREAD_H

#include <QThread>

class CopyFilesThread : public QThread
{
    Q_OBJECT
public:
    explicit CopyFilesThread(QObject *parent = nullptr);

    void processPath(const QString &oldPath, const QString &newPath);

signals:

public slots:

protected:
    void run();

private:
    void copyAllFiles(const QString &oldPath, const QString &newPath);
    QString createMultiDirectory(const QString &path);
private:
    QString m_newPath;
    QString m_oldPath;
};

#endif // COPYFILESTHREAD_H
