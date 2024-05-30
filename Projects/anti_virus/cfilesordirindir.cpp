#include "cfilesordirindir.h"

CFilesOrDirInDir::CFilesOrDirInDir(QString dirpath):
    m_dirpath(dirpath)
{

}

QStringList CFilesOrDirInDir::getFilePathNameOfSplDir(QString dirPath)
{
    QStringList filePathNames;
    QDir splDir(dirPath);
    QFileInfoList fileInfoListInSplDir = splDir.entryInfoList(QDir::Files | QDir::Hidden);
    QFileInfo tempFileInfo;
    foreach (tempFileInfo, fileInfoListInSplDir)
    {
        // absoluteFilePath()   -- 截取到的是文件夹的路径
        // absolutePath()       -- 截取的是文件夹所在路径
        filePathNames << tempFileInfo.absoluteFilePath();
    }
    return filePathNames;

}

QStringList CFilesOrDirInDir::getDirPathOfSplDir(QString dirPath)
{
    QStringList dirPaths;
    QDir splDir(dirPath);
    QFileInfoList fileInfoListInSplDir = splDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    QFileInfo tempFileInfo;
    foreach (tempFileInfo, fileInfoListInSplDir)
    {
        dirPaths << tempFileInfo.absoluteFilePath();
    }
    return dirPaths;
}

QStringList CFilesOrDirInDir::getAllChildDirs(QString dirPath)
{
    QStringList filePathNames;
    // 首先，得到这个目录下面的文件全部
    filePathNames << getFilePathNameOfSplDir(dirPath);

    QStringList childDirs;
    childDirs << getDirPathOfSplDir(dirPath);

    QString tempChildDir;
    foreach (tempChildDir, childDirs)
    {
        // 取其子文件夹内容
        filePathNames << getAllChildDirs(tempChildDir);
    }
    return filePathNames;
}
