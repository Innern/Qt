#pragma execution_character_set("utf-8")
#include "mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // 1. 创建一个QTabWidget对象
    tabWidget = new QTabWidget;
    // 4. 将页面添加到QTabWidget对象中，并为每个选项卡提供一个合适的标签和可选的键盘快捷键
    tabWidget->addTab(new General(QFileInfo(QDir::currentPath())), tr("General"));
    tabWidget->addTab(new Premission(QFileInfo(QDir::currentPath())), tr("Premission"));
    tabWidget->addTab(new Applications(QFileInfo(QDir::currentPath())), tr("Applications"));

    setCentralWidget(tabWidget);

    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::updateCurrentPage);
    connect(tabWidget, &QTabWidget::tabBarClicked, this, &MainWindow::clickedPage);
    connect(tabWidget, &QTabWidget::tabBarDoubleClicked, this, &MainWindow::doubledClickedPage);
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTabBar);

    tabWidget->setElideMode(Qt::ElideRight);

    initMenu();
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateCurrentPage(int index)
{
    qDebug() << tr("当前页面索引：%1").arg(index);
}

void MainWindow::clickedPage(int index)
{
    qDebug() << tr("clicked the page %1").arg(index);
}

void MainWindow::doubledClickedPage(int index)
{
    qDebug() << tr("double clicked the page %1").arg(index);
}

void MainWindow::closeTabBar(int index)
{
    tabWidget->removeTab(index);
    tabBarAutoHideAct->setEnabled(tabWidget->count()<2);
}

void MainWindow::initMenu()
{
    // 属性菜单栏
    QMenu *propertyMenu = menuBar()->addMenu(tr("P&roperty"));

    // 是否以文档模式显示页面
    QAction *documentModeAct = new QAction(tr("&Document Mode"));
    documentModeAct->setCheckable(true);
    connect(documentModeAct, &QAction::triggered, [=](bool checked){
        tabWidget->setDocumentMode(checked);
    });

    propertyMenu->addAction(documentModeAct);

    // 是否可以在选项卡区域内移动
    QAction *movableAct = new QAction(tr("&Movable"));
    movableAct->setCheckable(true);
    connect(movableAct, &QAction::triggered, [=](bool checked){
        tabWidget->setMovable(checked);
    });

    propertyMenu->addAction(movableAct);

    // 选项卡栏数量少于2个时是否自动隐藏
    tabBarAutoHideAct = new QAction(tr("Tab Bar &Hide"));
    tabBarAutoHideAct->setToolTip(tr("It's enable when the tab bar's count less than 2."));
    tabBarAutoHideAct->setCheckable(true);
    connect(tabBarAutoHideAct, &QAction::triggered, [=](bool checked){
        tabWidget->setTabBarAutoHide(checked);
    });

    propertyMenu->addAction(tabBarAutoHideAct);
    tabBarAutoHideAct->setEnabled(tabWidget->count()<2);

    // 是否为每个选项卡栏设置关闭按钮
    QAction *tabsClosableAct = new QAction(tr("Tabs &Closable"));
    tabsClosableAct->setCheckable(true);
    connect(tabsClosableAct, &QAction::triggered, [=](bool checked){
        tabWidget->setTabsClosable(checked);
    });

    propertyMenu->addAction(tabsClosableAct);

    // 是否添加滚动按钮
    QAction *usesScrollButtonsAct = new QAction(tr("&Uses Scroll Buttons"));
    usesScrollButtonsAct->setCheckable(true);
    connect(usesScrollButtonsAct, &QAction::triggered, [=](bool checked){
        tabWidget->setUsesScrollButtons(checked);
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
        tabWidget->setElideMode(Qt::ElideNone);
    });
    connect(leftAct, &QAction::triggered, [=](){
        tabWidget->setElideMode(Qt::ElideLeft);
    });
    connect(rightAct, &QAction::triggered, [=](){
        tabWidget->setElideMode(Qt::ElideRight);
    });
    connect(middleAct, &QAction::triggered, [=](){
        tabWidget->setElideMode(Qt::ElideMiddle);
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
        tabWidget->setTabPosition(QTabWidget::North);
    });
    connect(southAct, &QAction::triggered, [=](){
        tabWidget->setTabPosition(QTabWidget::South);
    });
    connect(westAct, &QAction::triggered, [=](){
        tabWidget->setTabPosition(QTabWidget::West);
    });
    connect(eastAct, &QAction::triggered, [=](){
        tabWidget->setTabPosition(QTabWidget::East);
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
        tabWidget->setTabShape(QTabWidget::Rounded);
    });
    connect(triangularAct, &QAction::triggered, [=](){
        tabWidget->setTabShape(QTabWidget::Triangular);
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


General::General(const QFileInfo &fileInfo, QWidget *parent)
    : QWidget(parent)
{
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);  // 所有行都换行

    formLayout->addRow(tr("&File Name:"), new QLineEdit(fileInfo.fileName()));
    formLayout->addRow(tr("&Path:"), new QLineEdit(fileInfo.absolutePath()));
    formLayout->addRow(tr("&Size:"), new QLineEdit(tr("%1 K").arg(fileInfo.size())));
    formLayout->addRow(tr("Last &Read:"), new QLineEdit(fileInfo.lastRead().toString(Qt::TextDate)));
    formLayout->addRow(tr("Lasr &Modified:"), new QLineEdit(fileInfo.lastModified().toString(Qt::TextDate)));


    setLayout(formLayout);

}

Premission::Premission(const QFileInfo &fileinfo, QWidget *parent)
    : QWidget(parent)
{
    QGroupBox *premissionGroup = new QGroupBox(this);
    QCheckBox *readableCheckBox = new QCheckBox(tr("&Readable"));
    QCheckBox *writeabelCheckBox = new QCheckBox(tr("&Writable"));
    QCheckBox *executableCheckBox = new QCheckBox(tr("&Executable"));
    QVBoxLayout *preLayout = new QVBoxLayout(this);
    preLayout->addWidget(readableCheckBox);
    preLayout->addWidget(writeabelCheckBox);
    preLayout->addWidget(executableCheckBox);
    premissionGroup->setLayout(preLayout);

    QGroupBox *ownershipGroup = new QGroupBox(this);
    QFormLayout *formLayout = new QFormLayout(ownershipGroup);
    formLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
    formLayout->addRow(tr("&Owner:"), new QLineEdit(fileinfo.owner()));
    formLayout->addRow(tr("&Group:"), new QLineEdit(fileinfo.group()));

    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(premissionGroup);
    vLayout->addWidget(ownershipGroup);
    vLayout->addStretch(1);
    setLayout(vLayout);

    readableCheckBox->setChecked(fileinfo.isReadable());
    writeabelCheckBox->setChecked(fileinfo.isWritable());
    executableCheckBox->setChecked(fileinfo.isExecutable());
}


Applications::Applications(const QFileInfo &fileinfo, QWidget *parent)
    :QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(new QLabel(tr("Open With:")));
    QListWidget *listWidget = new QListWidget(this);

    for(int i=0;i<20;i++){
        QString text = QString("Application %1").arg(i+1);
        listWidget->addItem(text);
    }

    vLayout->addWidget(listWidget);
    vLayout->addWidget(new QCheckBox(tr("Always use this application to open this type of file.")));
}
