#include "mdichild.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSaveFile>
#include <QTextStream>

MdiChild::MdiChild(QWidget *parent) : QTextEdit(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isUntitled = true;
    setAcceptDrops(false);  // 不接受拖放事件
}

void MdiChild::newFile()
{
    // 新建窗口的一些操作，比如设置标题栏
    static int num = 0;  // 记录新建的子窗口数量
    curFile = tr("document%1.txt").arg(++num);
    isUntitled = true;
    setWindowTitle(curFile + "[*]");

    // [*]占位符
    connect(document(), &QTextDocument::contentsChanged,
            this, &MdiChild::documentWasModified);

}

bool MdiChild::loadFile(const QString &filename)
{
    // 加载文件内容
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot read file %1:\n%2").arg(filename).arg(file.errorString()));
        return false;
    }
    // 读取文件内容并显示到文本编辑器中
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    setText(in.readAll());
    QApplication::restoreOverrideCursor();
    file.close();

    // 设置当前文件
    setCurrentFile(filename);

    connect(document(), &QTextDocument::contentsChanged,
            this, &MdiChild::documentWasModified);

    return true;
}

bool MdiChild::save()
{
    // 保存文件分为保存打开的文件和新建的文件
    if(isUntitled){
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if(maybeSave()){
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
    setWindowModified(document()->isModified());
}

void MdiChild::setCurrentFile(const QString &filename)
{
    curFile = QFileInfo(filename).canonicalFilePath();
    isUntitled = false;
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
    document()->setModified(false);
    setWindowModified(false);

}

QString MdiChild::strippedName(const QString &filename)
{
    return QFileInfo(filename).fileName();
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

bool MdiChild::saveFile(const QString &filename)
{
    QSaveFile file(filename);
    if(!file.open(QFile::WriteOnly)){
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot open file %1:\n%2").arg(filename).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << toPlainText();
    if(!file.commit()){
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot write file %1:\n%2").arg(filename).arg(file.errorString()));
        QApplication::restoreOverrideCursor();
        return false;
    }
    QApplication::restoreOverrideCursor();

    setCurrentFile(filename);
    return true;
}

bool MdiChild::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if(!filename.isEmpty())
        return saveFile(filename);
    return false;
}

bool MdiChild::maybeSave()
{
    // 如果当前窗口有更改，询问是否要保存更改
    if(!document()->isModified())
        return true;

    QMessageBox::StandardButton btn = QMessageBox::question(this, tr("MDI"),
                                                            tr("%1 has been modified.\n"
                                                               "Do you want to save your changes?").arg(userFriendlyCurrentFile()),
                                                            QMessageBox::Save | QMessageBox::Discard
                                                            | QMessageBox::Cancel);
    switch (btn) {
    case QMessageBox::Save:{
        return save();
    }
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
