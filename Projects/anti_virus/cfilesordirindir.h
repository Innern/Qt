#ifndef CFILESORDIRINDIR_H
#define CFILESORDIRINDIR_H

#include <QString>
#include <QStringList>
#include <QFileInfoList>
#include <QDir>
#include <QFileInfoList>

class CFilesOrDirInDir
{

public:
    /*********************
     * 参数:指定目录
     * ******************/
    CFilesOrDirInDir(QString dirpath = nullptr);


    QStringList getFilePathNameOfSplDir(QString dirPath);

    QStringList getDirPathOfSplDir(QString dirPath);
    /**********************
     * 函数功能:获取指定目录下的所有目录
     * return：QStringList 绝对路径 所有目录的list
     * ********************/
    QStringList getAllChildDirs( QString dirPath);



private:
    QString m_dirpath;
};

#endif // CFILESORDIRINDIR_H
