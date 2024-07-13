#include "myaction.h"

#include <QLabel>
#include <QLineEdit>
#include <QSplitter>

MyAction::MyAction(QObject *parent) : QWidgetAction(parent)
{

    edit = new QLineEdit;
    connect(edit, &QLineEdit::returnPressed, this, &MyAction::sendText);

}

void MyAction::sendText()
{
    emit getText(edit->text());
    edit->clear();
}

QWidget *MyAction::createWidget(QWidget *parent)
{
    // 如果父部件是菜单栏或工具栏就创建
    if(parent->inherits("QMenu") || parent->inherits("QToolBar")){
        QSplitter *splitter = new QSplitter(parent);
        QLabel *label = new QLabel(tr("Input then Enter:"));
        splitter->addWidget(label);
        splitter->addWidget(edit);
        return splitter;
    }
    return 0;
}
