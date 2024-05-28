#include "widget.h"

#include <QApplication>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QDateTime>

void addDetailData(QAbstractItemModel *model, const QString &subject,
                   const QString sender, const QDateTime &date)
{
    // 插入一行
    model->insertRow(0);
    // 插入数据
    model->setData(model->index(0,0), subject);
    model->setData(model->index(0,1), sender);
    model->setData(model->index(0,2), date);
}

QAbstractItemModel *createModel(QObject *parent)
{
    // 创建模型对象
    QStandardItemModel *model = new QStandardItemModel(parent);
    // 设置列数
    model->setColumnCount(3);
    // 设置列标头
    model->setHeaderData(0, Qt::Horizontal, ("Subjec"));
    model->setHeaderData(1, Qt::Horizontal, ("Sender"));
    model->setHeaderData(2, Qt::Horizontal, ("Date"));

    // 插入数据
    addDetailData(model, ("Happy New Year!"), ("Grace K. <grace@software-inc.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Radically new comcept"), ("Grace K. <grace@software-inc.com>"),
                  QDateTime(QDate(2006, 12, 22), QTime(9, 44)));
    addDetailData(model, ("Accounts"), ("pascale@nospam,com"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Expenses"), ("Joe Bloggs <joe@bloggs.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Re: Expenses"), ("Andy <andy@nospam.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Re: Accounts"), ("Joe Bloggs <joe@bloggs.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Re: Accounts"), ("Andy <andy@nospam.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Sports"), ("Linda Smith <linda.smith@nospam.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("AW: Sports"), ("Rolf Newschweinstein <rolfn@nospam.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));
    addDetailData(model, ("Re: Sports"), ("Petra Schmidt <petras@nospam.com>"),
                  QDateTime(QDate(2006, 12, 31), QTime(17, 03)));

    return model;
}

int main(int argc, char *argv[])
{

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if(QT_VERSION > QT_VERSION_CHECK(5,6,0))
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    Widget w;
    w.setSourceModel(createModel(&w));
    w.show();
    return a.exec();
}
