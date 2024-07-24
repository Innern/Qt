#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/informationsdialog.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QMediaPlaylist;
class QMediaPlayer;
QT_END_NAMESPACE
class InformationsDialog;
class LotteryThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void staffStateChanged(int prizeID, const QList<int> &luckyIDs);

protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void getLuckyStaffs(const QList<int> &ids);
    void on_pushButton_start_clicked();
    void on_pushButton_showInfos_clicked();
    void getChangeInfo();
    void processFinalLucky(const QList<int> &ids);
    void updateTimeOut();

private:
    void selectCurrentPrize(bool up);
    static QString giftsPath();
    static QString mediaPath();

    QLabel *createLabel();
    void hideAllLuckyLabels();

    void loadStyleSheet();

    void changeLuckyStatus();

    void initPlayer();

private:
    Ui::MainWindow *ui;
    QWidget *centWidget;
    QList<QPushButton*> prizeBtns;  // 奖品按钮
    QList<QLabel*> m_labelList;
    QVBoxLayout *layoutDrawWidget;

    int m_pressCnt;  // 向上或向下按键的次数，向上-1，向下+1
    bool m_showPrizeInfo;  // 是否显示奖品信息
    InformationsDialog *m_pInfoDlg;  // 数据库信息显示&修改界面
    LotteryThread *m_thread;  // 抽奖线程
    StaffList m_staffs;  // 参与抽奖人员名单
    PrizeList m_prizes;  // 奖品列表
    bool m_start;  // 开始或停止

    int m_luckyPeopleCnt;

    QTimer *m_timer;

    QMediaPlaylist *m_playlist;
    QMediaPlayer *m_player;


};
#endif // MAINWINDOW_H
