#include "mainwindow.h"
#include "addresswidget.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_addressWidget(new AddressWidget)
{
    createMenus();
    createPropetryMenu();
    setCentralWidget(m_addressWidget);

    // 当表格视图中选择变化时，由updateActions接收选择改变的信号
    connect(m_addressWidget, &AddressWidget::selectionChanged, this, &MainWindow::updateActions);
    connect(m_addressWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTabBar);
}

MainWindow::~MainWindow() {}

void MainWindow::openFile()
{
    m_addressWidget->readFromFile();
}

void MainWindow::saveFile()
{
    m_addressWidget->writeToFile();
}

void MainWindow::updateActions(QItemSelection selected)
{
    QModelIndexList indexList = selected.indexes();  // 获取当前选择的项目的所有索引
    if(!indexList.isEmpty()){
        editEntry->setEnabled(true);
        removeEntry->setEnabled(true);
    } else {
        editEntry->setEnabled(false);
        removeEntry->setEnabled(false);
    }
}

void MainWindow::closeTabBar(int index)
{
    m_addressWidget->removeTab(index);
    tabBarAutoHideAct->setEnabled(m_addressWidget->count()<2);
}

void MainWindow::createMenus()
{
    // 创建菜单
    // File菜单
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(QKeySequence::Open);
    fileMenu->addAction(openAct);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcut(QKeySequence::Save);
    fileMenu->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    fileMenu->addSeparator();

    QAction *exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcut(Qt::Key_Escape);
    fileMenu->addAction(exitAct);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    // Tool菜单
    QMenu *toolMenu = menuBar()->addMenu(tr("&Tool"));
    QAction *addEntry = new QAction(tr("&Add Entry..."), this);
    addEntry->setShortcut(QKeySequence::New);
    toolMenu->addAction(addEntry);
    connect(addEntry, &QAction::triggered, m_addressWidget, &AddressWidget::showAddEntryDialog);

    editEntry = new QAction(tr("&Edit Entry..."), this);
    editEntry->setEnabled(false);
    editEntry->setShortcut(QKeySequence("Ctrl+I"));
    toolMenu->addAction(editEntry);
    connect(editEntry, &QAction::triggered, m_addressWidget, &AddressWidget::editEntry);

    removeEntry = new QAction(tr("&Remove Entry..."), this);
    removeEntry->setEnabled(false);
    removeEntry->setShortcut(QKeySequence::Delete);
    toolMenu->addSeparator();
    toolMenu->addAction(removeEntry);
    connect(removeEntry, &QAction::triggered, m_addressWidget, &AddressWidget::removeEntry);
}

void MainWindow::createPropetryMenu()
{
    // 属性菜单栏
    QMenu *propertyMenu = menuBar()->addMenu(tr("&Property"));

    // 是否以文档模式显示页面
    QAction *documentModeAct = new QAction(tr("&Document Mode"));
    documentModeAct->setCheckable(true);
    connect(documentModeAct, &QAction::triggered, [=](bool checked){
        m_addressWidget->setDocumentMode(checked);
    });

    propertyMenu->addAction(documentModeAct);

    // 是否可以在选项卡区域内移动
    QAction *movableAct = new QAction(tr("&Movable"));
    movableAct->setCheckable(true);
    connect(movableAct, &QAction::triggered, [=](bool checked){
        m_addressWidget->setMovable(checked);
    });

    propertyMenu->addAction(movableAct);

    // 选项卡栏数量少于2个时是否自动隐藏
    tabBarAutoHideAct = new QAction(tr("Tab Bar &Hide"));
    tabBarAutoHideAct->setToolTip(tr("It's enable when the tab bar's count less than 2."));
    tabBarAutoHideAct->setCheckable(true);
    connect(tabBarAutoHideAct, &QAction::triggered, [=](bool checked){
        m_addressWidget->setTabBarAutoHide(checked);
    });

    propertyMenu->addAction(tabBarAutoHideAct);
    tabBarAutoHideAct->setEnabled(m_addressWidget->count()<2);

    // 是否为每个选项卡栏设置关闭按钮
    QAction *tabsClosableAct = new QAction(tr("Tabs &Closable"));
    tabsClosableAct->setCheckable(true);
    connect(tabsClosableAct, &QAction::triggered, [=](bool checked){
        m_addressWidget->setTabsClosable(checked);
    });

    propertyMenu->addAction(tabsClosableAct);

    // 是否添加滚动按钮
    QAction *usesScrollButtonsAct = new QAction(tr("&Uses Scroll Buttons"));
    usesScrollButtonsAct->setCheckable(true);
    connect(usesScrollButtonsAct, &QAction::triggered, [=](bool checked){
        m_addressWidget->setUsesScrollButtons(checked);
    });

    propertyMenu->addAction(usesScrollButtonsAct);


    // 根据菜单栏的选择项设置消隐模式
    QMenu *elideModeMenu = propertyMenu->addMenu(tr("&Elide Mode"));

    // 设置互斥按钮组
    QActionGroup *elideModeGroup = new QActionGroup(this);
    QAction *noneAct = elideModeGroup->addAction(tr("Elide &Node"));
    QAction *leftAct = elideModeGroup->addAction(tr("Elide &Left"));
    QAction *rightAct = elideModeGroup->addAction(tr("Elide &Right"));
    QAction *middleAct = elideModeGroup->addAction(tr("Elide &Middle"));

    connect(noneAct, &QAction::triggered, [=](){
        m_addressWidget->setElideMode(Qt::ElideNone);
    });
    connect(leftAct, &QAction::triggered, [=](){
        m_addressWidget->setElideMode(Qt::ElideLeft);
    });
    connect(rightAct, &QAction::triggered, [=](){
        m_addressWidget->setElideMode(Qt::ElideRight);
    });
    connect(middleAct, &QAction::triggered, [=](){
        m_addressWidget->setElideMode(Qt::ElideMiddle);
    });

    // 设置可选的
    noneAct->setCheckable(true);
    leftAct->setCheckable(true);
    rightAct->setCheckable(true);
    middleAct->setCheckable(true);

    // 添加到菜单栏
    elideModeMenu->addAction(noneAct);
    elideModeMenu->addAction(leftAct);
    elideModeMenu->addAction(rightAct);
    elideModeMenu->addAction(middleAct);

    // 设置默认选择的项
    noneAct->setChecked(true);

    // 根据菜单栏的选择项设置tabPosition
    QMenu *positionMenu = propertyMenu->addMenu(tr("&Position"));

    // 设置互斥按钮组
    QActionGroup *posGroup = new QActionGroup(this);
    QAction *northAct = posGroup->addAction(tr("&North"));
    QAction *southAct = posGroup->addAction(tr("&South"));
    QAction *westAct = posGroup->addAction(tr("&West"));
    QAction *eastAct = posGroup->addAction(tr("&East"));

    connect(northAct, &QAction::triggered, [=](){
        m_addressWidget->setTabPosition(QTabWidget::North);
    });
    connect(southAct, &QAction::triggered, [=](){
        m_addressWidget->setTabPosition(QTabWidget::South);
    });
    connect(westAct, &QAction::triggered, [=](){
        m_addressWidget->setTabPosition(QTabWidget::West);
    });
    connect(eastAct, &QAction::triggered, [=](){
        m_addressWidget->setTabPosition(QTabWidget::East);
    });

    // 设置可选的
    northAct->setCheckable(true);
    southAct->setCheckable(true);
    westAct->setCheckable(true);
    eastAct->setCheckable(true);

    // 添加到菜单栏
    positionMenu->addAction(northAct);
    positionMenu->addAction(southAct);
    positionMenu->addAction(westAct);
    positionMenu->addAction(eastAct);

    // 设置默认选择的项
    northAct->setChecked(true);


    // 根据菜单栏的选择项设置tabShape
    QMenu *shapeMenu = propertyMenu->addMenu(tr("&Shape"));

    // 设置互斥按钮组
    QActionGroup *shapeGroup = new QActionGroup(this);
    QAction *roundedAct = shapeGroup->addAction(tr("&Rounded"));
    QAction *triangularAct = shapeGroup->addAction(tr("&Triangular"));

    connect(roundedAct, &QAction::triggered, [=](){
        m_addressWidget->setTabShape(QTabWidget::Rounded);
    });
    connect(triangularAct, &QAction::triggered, [=](){
        m_addressWidget->setTabShape(QTabWidget::Triangular);
    });

    // 设置可选的
    roundedAct->setCheckable(true);
    triangularAct->setCheckable(true);

    // 添加到菜单栏
    shapeMenu->addAction(roundedAct);
    shapeMenu->addAction(triangularAct);

    // 设置默认选择的项
    roundedAct->setChecked(true);

}

