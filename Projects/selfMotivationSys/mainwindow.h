#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#pragma execution_character_set("utf-8")

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Dialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_sign_clicked();
    void updateTime();

    void saveText(const QString& text);


private:

    ///ui设置
    void setMessageLabel();

private:
    Ui::MainWindow *ui;
    Dialog *m_planWidget;

    QLabel *m_label; //状态栏label

    qint64 m_iSignDays;

    QString m_strEXEPath;
};
#endif // MAINWINDOW_H
