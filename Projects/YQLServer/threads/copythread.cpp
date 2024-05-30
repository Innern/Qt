#pragma execution_character_set("utf-8")

#include "copythread.h"

#include <QDir>
#include <QDebug>
#include <QDirIterator>

CopyThread::CopyThread(const QString &oldpath,
                       const QString &newpath,
                       int ccmIndex):
    m_oldPath(oldpath),
    m_newPath(newpath),
    m_ccmIndex(ccmIndex)
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
}

CopyThread::~CopyThread()
{
    qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__ << "]	ThreadID:" << QThread::currentThreadId() << "	Point:" << this;
}

bool CopyThread::copyAllFiles(const QString &oldPath,const QString newPath)
{
    // 递归遍历文件
    QDir olddir(oldPath);
    QDir newdir(newPath);
    if(!newdir.exists())
        if(!newdir.mkdir(newPath))
            return false;
    // 遍历旧目录下的所有文件，复制到新目录下
    const QFileInfoList fileInfoList = olddir.entryInfoList();  // 返回目录下所有文件
    foreach (const QFileInfo &fi, fileInfoList) {

//        qDebug() << "[Line:" << __LINE__ << "Function:" << __FUNCTION__  << "] filename:" << fi.fileName();
        //  去除顶级目录
        if(fi.fileName() == "." || fi.fileName() == "..")
            continue;

        // 如果是目录，递归获取所有文件
        else if(fi.isDir())
        {
            if(!copyAllFiles(fi.filePath(),newdir.filePath(fi.fileName())))
            {
                return false;
            }
        }
        else if(fi.isFile())
        {
            // 如果是文件，判断目标目录是否存在相应的文件，如果存在就删除
            if(newdir.exists(fi.fileName()))
            {
                if(!newdir.remove(fi.fileName()))
                    return false;
            }

            // 发送信息
            emit signal_copyFinish(tr("CopyFile:%1").arg(fi.fileName()),m_ccmIndex);
            if(!QFile::copy(fi.filePath(), newdir.filePath(fi.fileName())))
            {
//                qDebug() << tr("%1 copy faile.").arg(fi.fileName());
                return false;
            }

        }
    }

    return true;
}

bool CopyThread::clearAllFiles(const QString &filepath)
{
    QDir dir(filepath);
    if(dir.isEmpty())
        return true;

    QDirIterator dirsIt(filepath,QDir::AllDirs | QDir::Files |QDir::NoDotAndDotDot);
    while (dirsIt.hasNext()) {
        if(!dir.remove(dirsIt.next()))  // 删除文件操作，如果返回false表示这个是目录
            QDir(dirsIt.filePath()).removeRecursively();  // 删除目录以及下属的所有文件及目录
    }
    return true;
}

void CopyThread::run()
{

    // 首先判断目录是否存在
    // 目录不存在，创建目录
    QDir newDir(m_newPath);
    if(newDir.exists() && !newDir.isEmpty())
    {
        // 如果文件夹存在，清空该文件夹下所有文件
        clearAllFiles(m_newPath);
    }

    if(!copyAllFiles(m_oldPath,m_newPath))
    {
        emit signal_copyFinish(tr("FAIL: 测试程式复制失败."),m_ccmIndex);

        return;
    }

    emit signal_copyFinish(tr("PASS: 测试程式复制成功."),m_ccmIndex);

    // 退出线程
    this->exec();

}
