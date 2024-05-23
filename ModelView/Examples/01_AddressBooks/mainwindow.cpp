#include "mainwindow.h"
#include "addresswidget.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_cenWidget(new QTabWidget(this))
    , m_addressWidget(new AddressWidget)
{
    createMenus();
    setCentralWidget(m_addressWidget);

    // 当表格视图中选择变化时，由updateActions接收选择改变的信号
    connect(m_addressWidget, &AddressWidget::selectionChanged,
            this, &MainWindow::updateActions);
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

