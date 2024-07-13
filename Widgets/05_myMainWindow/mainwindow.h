#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QDialog;
class QLineEdit;
class QPrinter;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_New_triggered();

    void on_actionShow_Dock_triggered();

    /// 向文本编辑器中添加文本
    void setText(const QString &text);
    /// 遍历文档框架
    void showTextFrame();
    /// 遍历所有文本块
    void showTextBlock();
    /// 设置字体格式
    void setFontFormat(bool checked);
    void insertTable();  // 插入表格
    void insertList();  // 插入列表
    void insertPicture();  // 插入图片

    /// 文本查找和查找下一个
    void textFind();
    void textFindNext();

    /// 打印相关
    void doPrint();
    void doPrintPreview();
    void printPreview(QPrinter *printer);
    void createPdf();

private:
    Ui::MainWindow *ui;
    QDialog *findDlg;
    QLineEdit *findLineedit;
};
#endif // MAINWINDOW_H
