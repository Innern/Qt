#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE
class AddressWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void saveFile();
    void updateActions(QItemSelection selected);

private:
    void createMenus();
private:
    QAction *editEntry;
    QAction *removeEntry;
    QTabWidget *m_cenWidget;
    AddressWidget *m_addressWidget;
};
#endif // MAINWINDOW_H
