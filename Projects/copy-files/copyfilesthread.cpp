#include "copyfilesthread.h"

#include <QDir>

CopyFilesThread::CopyFilesThread(QObject *parent) : QThread(parent)
  , m_newPath(QString())
  , m_oldPath(QString())
{

}

void CopyFilesThread::processPath(const QString &oldPath, const QString &newPath)
{

    if(newPath.isEmpty() || oldPath.isEmpty())
        return;

    createMultiDirectory(newPath);  // 创建目录

    m_oldPath = oldPath;
    m_newPath = newPath;

    start();
}

void CopyFilesThread::run()
{

    copyAllFiles(m_oldPath, m_newPath);

}

void CopyFilesThread::copyAllFiles(const QString &oldPath, const QString &newPath)
{
    QDir oldDir(oldPath);
    QDir newDir(newPath);

    if(!newDir.exists())
        newDir.mkdir(newPath);
}

QString CopyFilesThread::createMultiDirectory(const QString &path)
{
    QDir dir(path);
    if(dir.exists())
        return dir.canonicalPath();

    QString parentPath = createMultiDirectory(path.left(path.lastIndexOf("/")));
    QString dirName = path.mid(path.lastIndexOf("/")+1);
    QDir parentDir(parentPath);
    if(!parentDir.isEmpty())
        parentDir.mkdir(dirName);
    return parentDir.filePath(dirName);

}
