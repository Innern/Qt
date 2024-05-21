#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QDialogButtonBox;
QT_END_NAMESPACE

class General: public QWidget
{
public:
    explicit General(const QFileInfo &fileInfo, QWidget *parent = Q_NULLPTR);
};

class Premission: public QWidget
{
public:
    explicit Premission(const QFileInfo &fileinfo, QWidget *parent=Q_NULLPTR);
};

class Applications: public QWidget{
public:
    explicit Applications(const QFileInfo &fileinfo, QWidget *parent = Q_NULLPTR);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateCurrentPage(int index);
    void clickedPage(int index);
    void doubledClickedPage(int index);
    void closeTabBar(int index);
private:
    void initMenu();

private:
    QTabWidget *tabWidget;
    QAction *tabBarAutoHideAct;
    QDialogButtonBox *btnBox;
};
#endif // MAINWINDOW_H
