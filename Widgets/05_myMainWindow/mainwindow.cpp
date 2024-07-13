#include "mainwindow.h"
#include "myaction.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QMdiSubWindow>
#include <QSpinBox>
#include <QTextEdit>
#include <QToolButton>
#include <QDate>
#include <QTextFrame>
#include <QDebug>
#include <QDialog>
#include <QLineEdit>

#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 代码编辑菜单栏
    QActionGroup *editGroup = new QActionGroup(this);
    QAction *act_L = editGroup->addAction(tr("&Left"));
    act_L->setCheckable(true);
    QAction *act_R = editGroup->addAction(tr("&Right"));
    act_R->setCheckable(true);
    QAction *act_M = editGroup->addAction(tr("&Middle"));
    act_M->setCheckable(true);
    act_L->setChecked(true);

    ui->menu_Edit->addAction(act_L);
    ui->menu_Edit->addAction(act_R);
    ui->menu_Edit->addAction(act_M);
    ui->menu_Edit->addSeparator();

    // 在工具栏中添加动作
    ui->toolBar->addAction(ui->action_Open);
    ui->toolBar->addAction(ui->action_New);
    ui->toolBar->addAction(ui->action_Save);
    ui->toolBar->addAction(ui->action_Exit);
    ui->toolBar->addSeparator();

    // 向工具栏添加部件
    QToolButton *colorBtn = new QToolButton(this);
    colorBtn->setText(tr("&Color"));
    QMenu *colorMenu = new QMenu(this);
    colorMenu->addAction(tr("红色"));
    colorMenu->addAction(tr("绿色"));
    colorBtn->setMenu(colorMenu);
    colorBtn->setPopupMode(QToolButton::MenuButtonPopup);
    ui->toolBar->addWidget(colorBtn);
    ui->toolBar->addSeparator();

    QSpinBox *scaledSpinbox = new QSpinBox(this);
    ui->toolBar->addWidget(scaledSpinbox);

    // 状态栏 正常信息
    ui->statusbar->addWidget(new QLabel(tr("Page 0, Line 0")));

    // 状态栏 临时信息
    ui->statusbar->showMessage(tr("Ready"), 2000);
    // 状态栏 永久消息
    ui->statusbar->addPermanentWidget(new QLabel(tr("%1").arg(QDate::currentDate().toString("yyyy/MM/dd ddd"))));

    // 添加自定义的菜单动作
    MyAction *action = new MyAction;
    ui->menu_Edit->addAction(action);
    connect(action, &MyAction::getText, this, &MainWindow::setText);
    //    ui->toolBar->addAction(action);

    ui->toolBar->addSeparator();

    // 工具栏增加遍历文档框架动作
    QAction *action_textFrame = new QAction(tr("Frame"), this);
    ui->toolBar->addAction(action_textFrame);
    connect(action_textFrame, &QAction::triggered, this, &MainWindow::showTextFrame);
    // 工具栏增加遍历文本块动作
    QAction *action_textBlock = new QAction(tr("Block"), this);
    ui->toolBar->addAction(action_textBlock);
    connect(action_textBlock, &QAction::triggered, this, &MainWindow::showTextBlock);
    // 工具栏增加设置字体格式动作
    QAction *action_font = new QAction(tr("F&ont"), this);
    action_font->setCheckable(true);
    ui->toolBar->addAction(action_font);
    connect(action_font, &QAction::toggled, this, &MainWindow::setFontFormat);
    // 工具栏增加添加图片动作
    QAction *action_picture = new QAction(tr("Picture"), this);
    ui->toolBar->addAction(action_picture);
    connect(action_picture, &QAction::triggered, this, &MainWindow::insertPicture);
    // 工具栏增加添加表格动作
    QAction *action_table = new QAction(tr("Table"), this);
    ui->toolBar->addAction(action_table);
    connect(action_table, &QAction::triggered, this, &MainWindow::insertTable);
    // 工具栏增加添加列表动作
    QAction *action_list = new QAction(tr("List"), this);
    ui->toolBar->addAction(action_list);
    connect(action_list, &QAction::triggered, this, &MainWindow::insertList);

    ui->toolBar->addSeparator();

    // 工具栏增加查找按钮
    findDlg = new QDialog(this);
    findDlg->setWindowTitle(tr("Find"));
    findLineedit = new QLineEdit(this);
    QPushButton *findNextBtn = new QPushButton(tr("Find Next"), findDlg);
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(findLineedit);
    vlayout->addWidget(findNextBtn);
    findDlg->setLayout(vlayout);
    QAction *act_find = new QAction(tr("&Find"), this);
    ui->toolBar->addAction(act_find);
    connect(act_find, &QAction::triggered, this, &MainWindow::textFind);
    connect(findNextBtn, &QPushButton::clicked, this, &MainWindow::textFindNext);

    ui->toolBar->addSeparator();

    QAction *act_print = new QAction(tr("打印"), this);
    QAction *act_printPreview = new QAction(tr("打印预览"), this);
    QAction *act_pdf = new QAction(tr("生成pdf"), this);
    connect(act_print, &QAction::triggered, this, &MainWindow::doPrint);
    connect(act_printPreview, &QAction::triggered, this, &MainWindow::doPrintPreview);
    connect(act_pdf, &QAction::triggered, this, &MainWindow::createPdf);
    ui->toolBar->addAction(act_print);
    ui->toolBar->addAction(act_printPreview);
    ui->toolBar->addAction(act_pdf);

    on_action_New_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_New_triggered()
{
    static int i=0;
    // 新建文件
    QTextEdit *edit = new QTextEdit(this);

    // 设计文本编辑器
    if(i == 0){
        i++;
        // 设置根框架格式
        QTextDocument *document = edit->document();  // 文档对象
        QTextFrame *rootFrame = document->rootFrame();  // 根文档框架
        QTextFrameFormat format;
        format.setBorder(3);
        format.setBorderBrush(Qt::red);
        rootFrame->setFrameFormat(format);

        // 添加一个子框架
        QTextFrameFormat frameFormat;
        frameFormat.setBackground(Qt::lightGray);
        frameFormat.setMargin(0);
        frameFormat.setPadding(0);
        frameFormat.setBorder(2);
        frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
        QTextCursor cursor = edit->textCursor();
        cursor.insertFrame(frameFormat);

    }

    /*QLabel *label = new QLabel(tr("Set Picture"), this);
    label->setFrameStyle(QFrame::Box | QFrame::Raised);*/
    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(edit);
    //    subWindow->resize(ui->mdiArea->size()/2);
    subWindow->setWindowTitle(tr("子窗口1"));
    subWindow->show();

}

void MainWindow::on_actionShow_Dock_triggered()
{
    ui->dockWidget->show();
}

void MainWindow::setText(const QString &text)
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    edit->setText(text);
}

void MainWindow::showTextFrame()
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    // 获取文档
    QTextDocument *document = edit->document();
    // 获取文档根框架
    QTextFrame *rootFrame = document->rootFrame();
    // 遍历
    QTextFrame::iterator it;
    for(it = rootFrame->begin(); it != rootFrame->end(); ++it){
        QTextFrame *frame = it.currentFrame();  // 当前文本框架
        QTextBlock block = it.currentBlock();  // 当前文本块
        if(frame){
            qDebug() << tr("Frame.");
        }
        if(block.isValid()){
            qDebug() << tr("block: %1").arg(block.text());
        }
    }

}

void MainWindow::showTextBlock()
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    // 获取文档结构
    QTextDocument *document = edit->document();
    // 获取第一个文本块
    QTextBlock block = document->firstBlock();
    for(int i=0; i<document->blockCount(); i++){
        qDebug() << tr("文本块 %1, 文本块首行行号为 %2, 长度为 %3, 内容为 %4")
                    .arg(i+1)
                    .arg(block.firstLineNumber())
                    .arg(block.length())
                    .arg(block.text());
        block = block.next();
    }
}

void MainWindow::setFontFormat(bool checked)
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    if(checked){

        // 获取文本光标
        QTextCursor cursor = edit->textCursor();
        // 文本块格式
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignCenter);
        cursor.insertBlock(blockFormat);
        QTextCharFormat charFormat;
        charFormat.setBackground(Qt::lightGray);
        charFormat.setForeground(Qt::blue);
        charFormat.setFont(QFont(tr("宋体"), 12, QFont::Bold, true));
        charFormat.setFontUnderline(true);  // 设置下划线
        cursor.setCharFormat(charFormat);
        cursor.insertText(tr("测试字体"));
    } else {
        QTextBlockFormat blockFormat = edit->document()->firstBlock().blockFormat();
        QTextCharFormat charFormat = edit->document()->firstBlock().charFormat();
        QTextCursor cursor = edit->textCursor();
        cursor.setBlockFormat(blockFormat);
        cursor.setCharFormat(charFormat);
    }


}

void MainWindow::insertTable()
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    QTextCursor cursor = edit->textCursor();
    QTextTableFormat tableFormat;
    tableFormat.setAlignment(Qt::AlignCenter);
    tableFormat.setCellPadding(10);
    tableFormat.setCellSpacing(2);
    cursor.insertTable(4,3);
}

void MainWindow::insertList()
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    QTextCursor cursor = edit->textCursor();
    cursor.insertList(QTextListFormat::ListCircle);
}

void MainWindow::insertPicture()
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    QTextCursor cursor = edit->textCursor();
    cursor.insertImage(QImage(":/imgs/logo.png"));
}

void MainWindow::textFind()
{
    findLineedit->selectAll();
    findLineedit->setFocus();
    findDlg->show();
}

void MainWindow::textFindNext()
{
    QString text = findLineedit->text();
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    // 使用查找函数查找指定字符串
    bool found = edit->find(text, QTextDocument::FindBackward);
    if(found){  // 查找成功输出字符串所在行和列
        qDebug() << tr("行号 %1 列号 %2")
                    .arg(edit->textCursor().blockNumber())
                    .arg(edit->textCursor().columnNumber());
    }
}

void MainWindow::doPrint()
{
    QPrinter printer;  // 表示一个打印设备
    QPrintDialog dlg(&printer, this);
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    if(edit->textCursor().hasSelection()){
        dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    }
    if(dlg.exec() == QDialog::Accepted){
        edit->print(&printer);  // 执行打印操作
    }

}

void MainWindow::doPrintPreview()
{
    QPrinter printer;
    QPrintPreviewDialog preview(&printer, this);
    // 要生成预览页面时，发射paintRequested信号
    connect(&preview, &QPrintPreviewDialog::paintRequested,
            this, &MainWindow::printPreview);
    preview.exec();
}

void MainWindow::printPreview(QPrinter *printer)
{
    QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
    edit->print(printer);
}

void MainWindow::createPdf()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("导出pdf文件"),
                                                    QString(), "*.pdf");
    if(!filename.isEmpty()){
        if(QFileInfo(filename).suffix().isEmpty()){
            filename.append(".pdf");
        }

        QPrinter printer;
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        QTextEdit *edit = static_cast<QTextEdit*>(ui->mdiArea->activeSubWindow()->widget());
        edit->print(&printer);
    }
}
