#include "mainwindow.h"
#include "mdichild.h"
#include "ui_mainwindow.h"

#include <QToolBar>
#include <QMdiArea>
#include <QTextEdit>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QSignalMapper>
#include <QSettings>
#include <QDesktopWidget>
#include <QDebug>
#include <QMimeData>
#include <QWhatsThis>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mdiArea(new QMdiArea(this))
{
    ui->setupUi(this);
    readSettings();
    connect(mdiArea, &QMdiArea::subWindowActivated, this, &MainWindow::updateMenus);
    createMenu();
    updateMenus();

    setCentralWidget(mdiArea);
    // 设置接收拖放事件
    setAcceptDrops(true);

    setWindowTitle(tr("MDI"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 关闭mdiArea区域的所有子窗口
    mdiArea->closeAllSubWindows();
    // 如果还有窗口未关闭，忽略该事件
    if(mdiArea->currentSubWindow()){
        event->ignore();
    } else
    {
        writeSettings();
        event->accept();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // 如果数据中包含url，就接收动作，否则忽略
    if(event->mimeData()->hasUrls()){
        event->acceptProposedAction();
    } else
        event->ignore();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    // 获取mime数据，如果数据中包含url，就获取路径，并打开
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls()){
        QList<QUrl> urlList = mimeData->urls();
        QString filename = urlList.at(0).toLocalFile();
        if(!filename.isEmpty()){
            openFile(filename);
        }
    }
}

void MainWindow::newFile()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty()){
        openFile(filename);
    }
}

void MainWindow::saveAs()
{
    // 检查当前的活动窗口中的文本内容是否有更改，
    // 如果没有更改，不做任何操作，
    // 否则，保存文件
    MdiChild *child = activateMdiWindow();
    if(child && child->save()){
        ui->statusbar->showMessage(tr("File saved."));
        prependToRecnetFiles(child->currentFile());

    }
}

void MainWindow::switchLayoutDirection()
{
    if(layoutDirection() == Qt::LeftToRight){
        setLayoutDirection(Qt::RightToLeft);
    } else {
        setLayoutDirection(Qt::LeftToRight);
    }
}

void MainWindow::cut()
{
    if(activateMdiWindow())
        activateMdiWindow()->cut();
}

void MainWindow::copy()
{
    if(activateMdiWindow())
        activateMdiWindow()->copy();
}

void MainWindow::paste()
{
    if(activateMdiWindow())
        activateMdiWindow()->paste();
}

void MainWindow::redo()
{
    if(activateMdiWindow())
        activateMdiWindow()->redo();
}

void MainWindow::undo()
{
    if(activateMdiWindow())
        activateMdiWindow()->undo();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About MDI"),
                       tr("The <b>MDI</b> example demonstrates how to write multiple "
                          "document interface applications using Qt."));
}

void MainWindow::updateMenus()
{
    // 当前是否有活动的子窗口，根据活动子窗口设置相应的菜单项是否可用
    MdiChild *child = activateMdiWindow();
    bool hasChild = (child != nullptr);
    saveAsAct->setEnabled(hasChild);
#ifndef QT_NO_CLIPBOARD
    pasteAct->setEnabled(hasChild);
    bool hasSelection = (child &&
                         child->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);
#endif

    redoAction->setEnabled(child && child->document()->isRedoAvailable());
    undoAction->setEnabled(child && child->document()->isUndoAvailable());

    closeAct->setEnabled(hasChild);
    closeAllAct->setEnabled(hasChild);
    tileAct->setEnabled(hasChild);
    cascadeAct->setEnabled(hasChild);
    nextAct->setEnabled(hasChild);
    previousAct->setEnabled(hasChild);
    separatorAct->setVisible(hasChild);
}

void MainWindow::updateWindowMenus()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);

    // 如果有活动窗口，显示间接器
    QList<QMdiSubWindow*> subWindows = mdiArea->subWindowList();
    separatorAct->setEnabled(!subWindows.isEmpty());

    QString text;
    for(int i=0; i<subWindows.size(); ++i){
        MdiChild *child = qobject_cast<MdiChild*>(subWindows.at(i)->widget());
        // 如果子窗口的列表小于9个，添加数字快捷键
        if(i<9){
            text = tr("&%1 %2").arg(i+1).arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i+1).arg(child->userFriendlyCurrentFile());
        }

        QAction *action = windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activateMdiWindow());

        // 关联动作的触发信号到信号映射器的map()槽，map()槽会发射mapped()信号
        connect(action, &QAction::triggered, windowMapper, QOverload<>::of(&QSignalMapper::map));
        // 将动作与相应的窗口部件进行映射
        // 发射mapped()信号时会以这个窗口部件为参数
        windowMapper->setMapping(action, subWindows.at(i));

    }
}

void MainWindow::setActiveSubWindow(QWidget *w)
{
    foreach (QMdiSubWindow *subWindow, mdiArea->subWindowList()) {
        if(w == subWindow->widget()){
            mdiArea->setActiveSubWindow(subWindow);
            break;
        }
    }
}

void MainWindow::updateRecentFileAction()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QStringList recentFiles = readRecentFiles(settings);
    const int count = qMin(int(MaxRecentFiles), recentFiles.size());
    int i=0;
    for(; i<count; ++i){
        const QString filename = QFileInfo(recentFiles.at(i)).fileName();
        recentFileAct[i]->setText(tr("&%1 %2").arg(i+1).arg(filename));
        recentFileAct[i]->setData(recentFiles.at(i));
        recentFileAct[i]->setVisible(true);
    }
    for(;i<MaxRecentFiles;++i){
        recentFileAct[i]->setVisible(false);
    }
}

void MainWindow::openRecentFile()
{
    const QAction *action = qobject_cast<const QAction*>(sender());
    if(action)
        openFile(action->data().toString());
}

void MainWindow::updateLanguage()
{
    auto btn = QMessageBox::information(this, tr("MDI"), tr("Please restart the application to apply language."),
                             QMessageBox::Ok|QMessageBox::Cancel);
    if(btn == QMessageBox::Ok)
        close();
}

void MainWindow::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *newAct = fileMenu->addAction(QIcon(QPixmap(":/images/new.png")),tr("&New"));
    newAct->setShortcut(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAct = fileMenu->addAction(QIcon(QPixmap(":/images/open.png")), tr("&Open"));
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAsAct = fileMenu->addAction(QIcon(QPixmap(":/images/save.png")), tr("&Save"));
    saveAsAct->setShortcut(QKeySequence::Save);
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addSeparator();

    QMenu *recentMenu = fileMenu->addMenu(tr("&Recent"));
    recentSubMenu = recentMenu->menuAction();

    for(int i=0; i<MaxRecentFiles; ++i){
        recentFileAct[i] = recentMenu->addAction(QString(), this, &MainWindow::openRecentFile);
        recentFileAct[i]->setVisible(false);
    }

    connect(recentMenu, &QMenu::aboutToShow, this, &MainWindow::updateRecentFileAction);

    recentSeparatorAct = new QAction(this);
    recentSeparatorAct->setSeparator(true);

    setRecentVisible(hasRecentFiles());

    QAction *layoutAct = fileMenu->addAction(tr("Switch &Layout Direction"));
    connect(layoutAct, &QAction::triggered, this, &MainWindow::switchLayoutDirection);
    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("&Exit"));
    exitAct->setShortcut(QKeySequence::Quit);
    QIcon exitIcon = QIcon::fromTheme("application-exit");
    exitAct->setIcon(exitIcon);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    QToolBar *toolBar = addToolBar(tr("Main Tool Bar"));
    toolBar->addAction(newAct);
    toolBar->addAction(openAct);
    toolBar->addAction(saveAsAct);
    toolBar->addSeparator();

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    cutAct = editMenu->addAction(QIcon(QPixmap(":/images/cut.png")), tr("&Cut"));
    cutAct->setShortcut(QKeySequence::Cut);
    connect(cutAct, &QAction::triggered, this, &MainWindow::cut);

    copyAct = editMenu->addAction(QIcon(QPixmap(":/images/copy.png")), tr("C&opy"));
    copyAct->setShortcut(QKeySequence::Copy);
    connect(copyAct, &QAction::triggered, this, &MainWindow::copy);

    pasteAct = editMenu->addAction(QIcon(QPixmap(":/images/paste.png")), tr("&Paste"));
    pasteAct->setShortcut(QKeySequence::Paste);
    connect(pasteAct, &QAction::triggered, this, &MainWindow::paste);

    redoAction = editMenu->addAction(QIcon(QPixmap(":/images/redo.png")),tr("&Redo"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    undoAction = editMenu->addAction(QIcon(QPixmap(":/images/undo.png")),tr("&Undo"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    toolBar->addAction(cutAct);
    toolBar->addAction(copyAct);
    toolBar->addAction(pasteAct);
    toolBar->addAction(redoAction);
    toolBar->addAction(undoAction);

    toolBar->addSeparator();

    windowMenu = menuBar()->addMenu(tr("&Widnow"));
    connect(windowMenu, &QMenu::aboutToShow, this, &MainWindow::updateWindowMenus);

    windowMapper = new QSignalMapper(this);
    // 映射器重新发射信号，根据信号设置活动窗口
    connect(windowMapper, QOverload<QWidget*>::of(&QSignalMapper::mapped),
            this, &MainWindow::setActiveSubWindow);

    closeAct = new QAction(tr("&Close"));
    connect(closeAct, &QAction::triggered, mdiArea, &QMdiArea::closeActiveSubWindow);

    closeAllAct = new QAction(tr("Close &All"));
    connect(closeAllAct, &QAction::triggered, mdiArea, &QMdiArea::closeAllSubWindows);

    windowMenu->addSeparator();

    tileAct = new QAction(tr("&Tile"));
    tileAct->setCheckable(true);
    connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);

    cascadeAct = new QAction(tr("&Cascade"));
    cascadeAct->setCheckable(true);
    connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);
    windowMenu->addSeparator();
    QActionGroup *subWindowActGroup = new QActionGroup(this);
    subWindowActGroup->addAction(tileAct);
    subWindowActGroup->addAction(cascadeAct);

    nextAct = new QAction(tr("&Next"));
    connect(nextAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);

    previousAct = new QAction(tr("&Previous"));
    connect(previousAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    updateWindowMenus();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QMenu *langMenu = helpMenu->addMenu(tr("&Language"));
    QAction *englishAct = langMenu->addAction(tr("&English"));
    QAction *chineseAct = langMenu->addAction(tr("&Chinese"));
    englishAct->setCheckable(true);
    chineseAct->setCheckable(true);
    englishAct->setData("English");
    chineseAct->setData("Chinese");
    langActGroup = new QActionGroup(this);
    langActGroup->addAction(englishAct);
    langActGroup->addAction(chineseAct);
    connect(langActGroup, &QActionGroup::triggered, this, &MainWindow::updateLanguage);
    if(MainWindow::currentLanguage().compare("chinese", Qt::CaseInsensitive) == 0){
        chineseAct->setChecked(true);
    } else {
        englishAct->setChecked(true);
    }

    QAction *aboutAct = helpMenu->addAction(tr("&About"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);

    // 帮助提示动作
    QAction *whatisAct = QWhatsThis::createAction(this);
    toolBar->addAction(whatisAct);
}

MdiChild *MainWindow::createMdiChild()
{
    // New和Open的时候都需要创建MdiChild，创建之后的操作不同，所以这里编写了一个函数，可以用在New和Open中

    MdiChild *child = new MdiChild();  // 创建子窗口部件
    QMdiSubWindow *subWindow = mdiArea->addSubWindow(child);  // 将创建的子窗口部件添加到mdiArea

#ifndef QT_NO_CLIPBOARD
    connect(child, &MdiChild::copyAvailable, cutAct, &QAction::setEnabled);
    connect(child, &MdiChild::copyAvailable, copyAct, &QAction::setEnabled);
#endif

    connect(child, &MdiChild::redoAvailable, redoAction, &QAction::setEnabled);
    connect(child, &MdiChild::undoAvailable, undoAction, &QAction::setEnabled);

    return child;
}

bool MainWindow::openFile(const QString &filename)
{
    // 如果当前子窗口中包含了要打开的文件的窗口，就不创建新的窗口打开文件了
    QMdiSubWindow *existingSubWin = findMdiSubWindow(filename);
    if(existingSubWin){
        mdiArea->setActiveSubWindow(existingSubWin);
        return true;
    }
    // 如果是打开的新的文件
    bool ok = loadFile(filename);
    if(ok)
        ui->statusbar->showMessage(tr("File loaded."));

    return ok;
}

bool MainWindow::loadFile(const QString &filename)
{
    MdiChild *child = createMdiChild();
    bool bLoad = child->loadFile(filename);
    if(bLoad){
        child->show();
    } else {
        child->close();
    }
    prependToRecnetFiles(filename);
    return bLoad;
}

QMdiSubWindow *MainWindow::findMdiSubWindow(const QString &filename)
{
    // 将文件的规范路径
    QString canonicalFile = QFileInfo(filename).canonicalFilePath();
    // 获取mdiArea的子窗口列表
    const QList<QMdiSubWindow*> subWinList = mdiArea->subWindowList();
    // 检查每一个子窗口的当前文件是否与filename一致，如果有一致的，说明存在，否则不存在，返回NULL
    foreach (QMdiSubWindow *subWindow, subWinList) {
        MdiChild *child = static_cast<MdiChild*>(subWindow->widget());
        if(child->currentFile() == canonicalFile)
            return subWindow;

    }
    return nullptr;
}

void MainWindow::readSettings()
{
    // 组织名、应用程序名
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if(geometry.isEmpty()){
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width()/3, availableGeometry.height()/3);
        move((availableGeometry.width() - width()/2),
             (availableGeometry.height() - height())/2);
    } else {
        restoreGeometry(geometry);
    }

}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("language", langActGroup->checkedAction()->data().toString());
}

static inline QString recentFilesKey() {return QStringLiteral("recentFileList");}
static inline QString fileKey() {return QStringLiteral("file");}

QStringList MainWindow::readRecentFiles(QSettings &settings)
{
    QStringList files;
    const int count = settings.beginReadArray(recentFilesKey());
    for(int i=0;i<count;++i){
        settings.setArrayIndex(i);
        files.append(settings.value(fileKey()).toString());
    }
    settings.endArray();
    return files;
}

void MainWindow::writeRecentFiles(const QStringList &files, QSettings &settings)
{
    const int count = files.size();
    settings.beginWriteArray(recentFilesKey());
    for(int i=0;i<count;++i){
        settings.setArrayIndex(i);
        settings.setValue(fileKey(), files.at(i));
    }
    settings.endArray();
}

void MainWindow::prependToRecnetFiles(const QString &filename)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QStringList oldRecentFiles = readRecentFiles(settings);
    QStringList recentFiles = oldRecentFiles;
    recentFiles.removeAll(filename);
    recentFiles.prepend(filename);
    if(oldRecentFiles != recentFiles)
        writeRecentFiles(recentFiles, settings);

    setRecentVisible(!recentFiles.isEmpty());
}

void MainWindow::setRecentVisible(bool visible)
{
    recentSubMenu->setVisible(visible);
    recentSeparatorAct->setVisible(visible);
}

bool MainWindow::hasRecentFiles()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const int count = settings.beginReadArray(recentFilesKey());
    settings.endArray();
    return count > 0;
}

QString MainWindow::currentLanguage()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    return settings.value("language", "english").toString();
}

MdiChild *MainWindow::activateMdiWindow()
{
    if(QMdiSubWindow *subWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild*>(subWindow->widget());
    return nullptr;
}

