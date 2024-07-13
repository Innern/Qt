#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QMdiArea;
class QMdiSubWindow;
class QAction;
class QMenu;
class QSignalMapper;  // 信号映射器，实现对多个相同部件的相同信号进行映射
class QSettings;
class QActionGroup;
QT_END_NAMESPACE
class MdiChild;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 当前界面选择的语言
    static QString currentLanguage();

protected:
    void closeEvent(QCloseEvent *event) override;

    // 支持拖放操作
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void newFile();
    void open();
    void saveAs();
    void switchLayoutDirection();
    void cut();
    void copy();
    void paste();
    void redo();
    void undo();
    void about();
    // 当窗口活动变化时更新菜单显示
    void updateMenus();
    void updateWindowMenus();

    // 与信号映射器的mapped()信号关联
    void setActiveSubWindow(QWidget *w);

    // 与最近打开文件列表有关的函数
    // 获取最近打开文件列表
    void updateRecentFileAction();
    void openRecentFile();

    void updateLanguage();
private:
    void createMenu();
    MdiChild *createMdiChild();
    bool openFile(const QString &filename);
    bool loadFile(const QString &filename);
    QMdiSubWindow *findMdiSubWindow(const QString &filename);

    // 读写程序设置
    void readSettings();
    void writeSettings();
    QStringList readRecentFiles(QSettings &settings);
    void writeRecentFiles(const QStringList &files, QSettings &settings);
    void prependToRecnetFiles(const QString &filename);
    void setRecentVisible(bool visible);
    bool hasRecentFiles();

    MdiChild *activateMdiWindow();

private:
    enum{MaxRecentFiles = 5};

    Ui::MainWindow *ui;
    QMdiArea *mdiArea;
#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
#endif
    QAction *redoAction;
    QAction *undoAction;
    QAction *recentFileAct[MaxRecentFiles];
    QAction *recentSubMenu;
    QAction *recentSeparatorAct;
    QAction *saveAsAct;
    QMenu *windowMenu;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QActionGroup *langActGroup;

    QSignalMapper *windowMapper;  // 信号映射器
};
#endif // MAINWINDOW_H
