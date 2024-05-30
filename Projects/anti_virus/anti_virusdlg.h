/***************************************
 文件监视系统 QFileSystemWatcher
 头文件 #include <QFileSystemWatcher>
 QFileSystemWatcher用来监视目录或者文件是否改变。观察指定路径的列表监视文件或目录的修改。
函数接口:
  addPath()/ addPaths()增加要监视的目录或文件。
  removePath()/removePaths() 移除监视的目录或文件。
  files()可以获取到监视的文件，directories()获取监视的目录
有两个信号：
  void directoryChanged(const QString &path)//目录改变信号
  void fileChanged(const QString &path)//文件改变信号
需要注意的是 文件或目录 删除或重命名后，文件系统自动解除监视，即自动卸载。
最多只能监视256个文件或目录。

  ***************************************/


/*******************************************
 *
 * Q:什么情况下需要更新文件呢?
 * A:   1.打开程序的时候需要获取监视的文件,此时需要判断文件大小是否超过规格，超过规格报错，更新后重新获取获取监视的文件，此时需要判断文件大小是否超过规格，超过规格报错
 *      2.文件改变时报错，更新文件后需要重新获取监视的文件，此时需要判断文件大小是否超过规格，超过规格报错
 *      3.文件所在目录名改变时重新获取监视的文件
 * ******************************************/


#ifndef ANTI_VIRUSDLG_H
#define ANTI_VIRUSDLG_H

#include <QDialog>

#include <QFileSystemWatcher>
#include <QDir>
#include <QSystemTrayIcon>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QTextCodec>
#include <iostream>
#include <stdio.h>
#include "warningdlg.h"


#pragma execution_character_set("utf-8")
#define AUTO_CLOSE 1

QT_BEGIN_NAMESPACE
namespace Ui { class anti_virusDlg; }
QT_END_NAMESPACE

#define REG_ROOT_PATH   ("//HKEY_CURRENT_USER//Software//anti_virus//")

class anti_virusDlg : public QDialog
{
    Q_OBJECT

public:
    anti_virusDlg(QWidget *parent = nullptr);
    ~anti_virusDlg();

protected:
    void changeEvent(QEvent* e);
    void timerEvent(QTimerEvent* evetn);
private:
    QStringList getFileNames(const QString& path,const QString& filter,qint64& maxsize);

    //更新监视的文件并获取文件最大size
    void updateWatcherFiles();

    //设置系统托盘
    void setSystemTrayIcon();

signals:
    void sendFileWatcher(bool exit);

    void send_file_size_exp();
private slots:
    void showFileChanged();
    void responseSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void exitApplication();
    void hideWarning(bool exit);

    void timeUpdate();



private:
    Ui::anti_virusDlg *ui;
    QFileSystemWatcher* m_fileWatcher;
    QSystemTrayIcon* m_systemTrayIcon;//设置为全局变量，当窗口状态改变时做出响应
    warningDlg m_warDlg;
   int m_iTimerEvent;
   qint64 m_iFileMaxSize;
   QString m_strDirPath;
   QStringList m_watcherFiles;

};
#endif // ANTI_VIRUSDLG_H
