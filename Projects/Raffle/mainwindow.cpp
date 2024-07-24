#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "threads/lotterythread.h"

#include <QtWidgets>
#include <QtMultimedia>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pInfoDlg(new InformationsDialog(this))
    , m_thread(Q_NULLPTR)
    , m_pressCnt(-1)
    , m_showPrizeInfo(false)
    , m_start(false)
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);
    loadStyleSheet();

    ui->pushButton_showInfos->setDefault(false);
    //    ui->pushButton_start->setDefault(false);
    ui->pushButton_start->hide();

    ui->label_awardType->hide();
    ui->label_prize->hide();

    qDeleteAll(m_labelList);

    // layoutDrawWidget = new QHBoxLayout(ui->widget_drawLuckys);
    layoutDrawWidget = new QVBoxLayout(ui->widget_drawLuckys);
    layoutDrawWidget->setContentsMargins(0,10,0,10);
    layoutDrawWidget->addSpacerItem(new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding));

    connect(m_pInfoDlg, &InformationsDialog::infoChanged, this, &MainWindow::getChangeInfo);
    connect(this, &MainWindow::staffStateChanged, m_pInfoDlg, &InformationsDialog::changeStaffState);

    m_thread = new LotteryThread;
    connect(m_thread, &LotteryThread::sendLuckyIds, this, &MainWindow::getLuckyStaffs);
    connect(m_thread, &LotteryThread::sendFinalLuckyIds, this, &MainWindow::processFinalLucky);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateTimeOut);
    m_timer->start(1000);

    initPlayer();

    m_player->play();

    ui->label_company->setText(m_pInfoDlg->getCompanyName());


    getChangeInfo();
    changeLuckyStatus();

}

MainWindow::~MainWindow()
{
    delete ui;
    m_thread->wait();
    m_thread->deleteLater();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_0:
    {

    }
        return;
#if 0
    case Qt::Key_Down:  // 上下键选择奖品
    {
        if(m_prizes.size() == 0){
            QMessageBox::warning(this, tr("Raffle"), tr("Please set prizes first!"));
            return;
        }
        m_pressCnt++;
        if(m_pressCnt >= m_prizes.size())
            m_pressCnt = 0;
        ui->label_prize->setText(m_prizes[m_pressCnt].name);
    }
        return;
    case Qt::Key_Up:
    {
        if(m_prizes.size() == 0){
            QMessageBox::warning(this, tr("Raffle"), tr("Please set prizes first!"));
            return;
        }
        m_pressCnt--;
        if(m_pressCnt < 0)
            m_pressCnt = m_prizes.size()-1;
        ui->label_prize->setText(m_prizes[m_pressCnt].name);
    }
        return;
#else
    case Qt::Key_Up:
    case Qt::Key_Down:
    {
        selectCurrentPrize(event->key() == Qt::Key_Up);
    }
        return;
#endif
    case Qt::Key_Space:  // 空格键开始和停止
    {
        on_pushButton_start_clicked();
    }
        return;
    case Qt::Key_S:
    {
        if(m_player->state() == QMediaPlayer::PlayingState){
            m_player->pause();
        } else {
            m_player->play();
        }
        return;
    }
    case Qt::Key_H:
    {
        if(ui->label_awardType->isHidden()){
            ui->label_awardType->show();
            ui->label_prize->show();
            ui->label_prizeImage->show();
        } else {
            ui->label_awardType->hide();
            ui->label_prize->hide();
            ui->label_prizeImage->hide();
        }
        return;
    }
    default:
        return QMainWindow::keyPressEvent(event);
    }

}

void MainWindow::getLuckyStaffs(const QList<int> &ids)
{
    Staff staff;
    QString text;
    text.clear();
    for(int i=0;i<ids.size();i++){
        QLabel *label = m_labelList.at(i);
        label->show();
        staff = m_staffs.at(ids[i]);
        text = QString("%2 - %3").arg(staff.id).arg(staff.name);
        // qDebug() << text;
        label->setText(text);
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    if(m_staffs.isEmpty()){
        if(QMessageBox::Ok == QMessageBox::information(this, tr("Warning"), tr("Please load staff&prize information first."),
                                                       QMessageBox::Ok, QMessageBox::Cancel)) {
            on_pushButton_showInfos_clicked();
        }
        return;
    }

    //    ui->pushButton_start->setFocus();
    m_start = !m_start;
    ui->pushButton_start->setText(!m_start ? tr("开始") : tr("停止"));
    if(m_start){
        getChangeInfo();
        Prize prize = m_prizes.at(m_pressCnt);
        //         qDebug() << tr("The prize %1 has %2 lucky people.").arg(prize.name).arg(prize.num);

        int onceDraw = prize.onceDraw;  // 依次抽取的人数
        int &remaining = prize.onceDraw;
        if(prize.remaining < onceDraw){
            // 按照剩余的抽取
            onceDraw = prize.remaining;
        }
        if(onceDraw <= 0){
            if(QMessageBox::Yes == QMessageBox::warning(this, tr("Raffle"), tr("Current prize remaining 0.\nDo you want to draw free?"),
                                                        QMessageBox::Yes | QMessageBox::Cancel)){
                onceDraw = QInputDialog::getInt(this, tr("Raffle"), tr("Please input lucky count(0~10):"),5, 0, 10);
            } else{
                return;
            }
        }
        // qDebug() << tr("remaining prize: ") << prize.remaining;
        // 创建显示结果的标签
        int labelCnt = m_labelList.size();
        if(labelCnt < onceDraw){
            for(int i=labelCnt;i<onceDraw;++i){
                m_labelList.append(createLabel());
            }
            layoutDrawWidget->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Expanding));
        } else {
            hideAllLuckyLabels();
            for(int i=0;i<onceDraw;++i){
                QLabel *label = m_labelList.at(i);
                label->show();
            }
        }
        // qDebug() << tr("once draw count: ") << onceDraw;
        // qDebug() << tr("%1 remaining %2").arg(prize.name).arg(prize.remaining);
        m_thread->setLuckyCount(onceDraw);
        m_thread->setPeopleList(m_staffs);
        m_thread->setIntervalTime(m_pInfoDlg->getDrawTime());
        m_thread->processLottery(m_staffs.size());
        m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        m_playlist->setCurrentIndex(1);
        //        m_player->play();
    } else {
        if(m_thread && m_thread->isRunning()){
            m_thread->stopProcess();
            m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
            m_playlist->setCurrentIndex(2);
            QThread::sleep(1);
        }
    }
}

void MainWindow::on_pushButton_showInfos_clicked()
{
    hideAllLuckyLabels();

    m_pInfoDlg->exec();

}

void MainWindow::getChangeInfo()
{
    m_staffs = m_pInfoDlg->getStaffs();
    m_prizes = m_pInfoDlg->getPrizes();
}

void MainWindow::processFinalLucky(const QList<int> &ids)
{
    int curPrizeID = m_prizes[m_pressCnt].id;  // 当前抽取的奖品ID
    // qDebug() << tr("process prize id : ") << curPrizeID;
    emit staffStateChanged(curPrizeID, ids);

    changeLuckyStatus();

    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_playlist->setCurrentIndex(3);
}

void MainWindow::updateTimeOut()
{
    changeLuckyStatus();
}

void MainWindow::selectCurrentPrize(bool up)
{
    hideAllLuckyLabels();

    ui->label_awardType->show();
    ui->label_prize->show();

    if(m_prizes.size() == 0){
        QMessageBox::warning(this, tr("Raffle"), tr("Please set prizes first!"));
        return;
    }
    if(up){
        m_pressCnt--;
        // if(m_pressCnt < 0)
        //     m_pressCnt = m_prizes.size()-1;
    } else {
        m_pressCnt++;
        // if(m_pressCnt >= m_prizes.size())
        //     m_pressCnt = 0;
    }
    m_pressCnt = (m_pressCnt < 0) ? (m_prizes.size()-1) :
                                    (m_pressCnt >= m_prizes.size()) ? 0 :
                                                                      m_pressCnt;

    Prize prize = m_prizes.at(m_pressCnt);
    //    ui->label_prize->setText(prize.name);
    ui->label_prize->setText(QString("%1 %2/%3").arg(prize.name).arg(prize.remaining).arg(prize.num));
    QString pixmapPath = giftsPath() + QString("%1.png").arg(m_pressCnt+1);
    // qDebug() << pixmapPath;
    pixmapPath = QDir(pixmapPath).canonicalPath();
    if(pixmapPath.isEmpty()){
        pixmapPath = giftsPath() + QString("0.png");
    }
    // qDebug() << pixmapPath;

    ui->label_prizeImage->setPixmap(QPixmap(pixmapPath));
    ui->label_prizeImage->setScaledContents(true);  // 根据label大小缩放图片
    ui->label_awardType->setText(prize.level);
}

QString MainWindow::giftsPath()
{
    return QApplication::applicationDirPath() + QString("/gift_images/");
}

QString MainWindow::mediaPath()
{
    return QApplication::applicationDirPath() + QString("/audio/");
}

QLabel *MainWindow::createLabel()
{
    QLabel *label = new QLabel;
    QFont font;
    font.setPointSize(32);
    label->setFont(font);
    label->setStyleSheet("color: rgb(254, 215, 34);");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    layoutDrawWidget->addWidget(label);
    layoutDrawWidget->addSpacerItem(new QSpacerItem(0,20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    return label;
}

void MainWindow::hideAllLuckyLabels()
{
    foreach (QLabel *label, m_labelList) {
        label->clear();
        label->hide();
    }
}

void MainWindow::loadStyleSheet()
{
    const QString qssFile = QString(":/qss/generalStyle.qss");
    QFile file(qssFile);
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MainWindow::changeLuckyStatus()
{
    m_luckyPeopleCnt = m_pInfoDlg->getOwnPrizeCnt();
    ui->label_LuckyStatus->setText(QString("%1/%2").arg(m_luckyPeopleCnt).arg(m_staffs.size()));
    if(m_pressCnt != -1){
        m_prizes = m_pInfoDlg->getPrizes();
        Prize prize = m_prizes.at(m_pressCnt);
        ui->label_prize->setText(QString("%1 %2/%3").arg(prize.name).arg(prize.remaining).arg(prize.num));
    }
}

void MainWindow::initPlayer()
{
    m_playlist = new QMediaPlaylist(this);
    // 添加媒体资源
    m_playlist->addMedia(QUrl::fromLocalFile(mediaPath() + m_pInfoDlg->getBackgroundMusic()));
    m_playlist->addMedia(QUrl::fromLocalFile(mediaPath() + m_pInfoDlg->getRollMusic()));
    m_playlist->addMedia(QUrl::fromLocalFile(mediaPath() + QString("musicOfDraw.mp3")));
    m_playlist->addMedia(QUrl::fromLocalFile(mediaPath() + m_pInfoDlg->getLuckyMusic()));
    m_playlist->setCurrentIndex(0);
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    m_player = new QMediaPlayer(this);
    m_player->setPlaylist(m_playlist);  // 设置播放列表

}
