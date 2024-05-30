#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "commondef.h"

#include <QMainWindow>
#include <QMap>
#include <QQueue>
#include <QTimer>
#include <QLabel>
#include <QSettings>
#include <QProcess>
#include <QSqlDatabase>
#include <QDateTime>

#define MAX_CCMTEST_COUNT   16

class QThread;
class Worker;
class QDateTime;
QT_FORWARD_DECLARE_CLASS(CopyThread)
QT_FORWARD_DECLARE_CLASS(CutLightDlg)
QT_FORWARD_DECLARE_CLASS(SNBindingDialog)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    struct InspectionInfo{
        QString name;
        int environment;
        QString errcode;
        QString dllName;
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setUiEnable(bool isAdministrator);
signals:
    void signal_sendSocketCmd(int cmd,const QString &sn);
    void signal_restoreSocketState();
public slots:
    void displaySocketMessage(LogLevel level,int idx,const QString &msg);
    void hasSocketDisconnect();
    void hasNewConnection();
    void getSocketState(int state,int idx);
    void checkSocketState();
    void moduleHasClosed(int idx);
    void restoreUI();
    void showCopyResult(const QString &str, int ccmidx);
    void processStateChanged(QProcess::ProcessState state);
    void showProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void showCutLightMessage(const QString &msg,int level);
    void comChanged(const QString &com);
private slots:
    void on_pushButton_sendCmd_clicked();

    void on_spinBox_port_valueChanged(int arg1);

    void on_spinBox_connectCnt_valueChanged(int arg1);

    void on_pushButton_startTest_clicked();

    void on_actionWindows_triggered(bool checked);

    void on_actionFusion_triggered(bool checked);

    void on_actionwindowsvista_triggered(bool checked);

    void on_actionLoad_DCCF_triggered();

    void on_actionCopy_Program_triggered();

    void on_actionSave_UI_triggered();

    void on_actionExit_triggered();

    void on_actionOpen_Program_triggered();

    void on_actionClose_Program_triggered();

    void on_actionOpen_Camera_triggered();

    void on_actionClose_Camera_triggered();

    void on_actionClear_Log_triggered();

    void on_lineEdit_IP_returnPressed();

    void on_actionClose_Mes_triggered();

    void on_actionCut_Liight_triggered();

    void on_spinBox_cutLightDelay_valueChanged(int arg1);

    void on_actionSNBinding_triggered();

    void on_actionChangeUser_triggered();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void init();
    void initParams();
    void initStatus();
    void addLog(const QString &str, LogLevel level = NormalLevel);
    void readUISettings();
    void writeUISettings();
    bool readSqlSettings();
    bool writeSqlSettings();
    bool readSettings();
    bool writeSettings();
    void cache2ui();
    void ui2cache();

    bool loadInspectionsInfo(const QString &dccfFile);
    void clearWorkerThreads();
    void clearCopyThreads();
    void clearProcessPointer();

    void setMESStatus(const QString &dccfFile);

private:
    Ui::MainWindow *ui;
//    QThread *thread;
    QVector<QThread*> m_threads;
    int m_preConnectCnt;
    int m_hasConnectCnt;
    int m_moduleHasCloedCnt;
    Worker *worker;
    QMap<int,int> m_mapSocketState;
    QQueue<int> m_cmdQueue;
    QTimer *m_timerCheckSocket;
    QLabel *m_connecStatusLabel;

    QString m_windowStyle;

    QLabel *projectLabel;
    QString m_dccfFile;
    QMap<int,InspectionInfo> m_inspections;

    QTimer *timerCopyFiles;  // 复制文件定时
    QVector<CopyThread*> m_copyThreads;
    bool m_bLoadDccf;
    int m_connectCnt;

    QVector<QProcess*> m_process;
    int m_hasProcessClosed;

    QString m_ip;
    int m_port;

    CutLightDlg *m_cutlightDlg;
    QString m_com;

    int m_delayTimes;

    QString m_strExtraTool;
    SNBindingDialog *bindingDlg;

    QDateTime m_startTime;

};

#endif // MAINWINDOW_H
