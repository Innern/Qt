#pragma execution_character_set("utf-8")
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

// 创建连接
static bool createConnection()
{
    // 配置数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("default.db");

    // 连接数据库
    if(!db.open())
    {
        QMessageBox::critical(0,
                              ("ERROR"),
                              QString("Database %2 open fail.\nError : %1").arg(db.lastError().text()).arg(db.databaseName()),
                              QMessageBox::Yes);
        return false;
    }

//    QMessageBox::information(0,
//                             ("SUCCESS"),
//                             QString("Database %1 open OK!").arg(db.databaseName()),
//                             QMessageBox::Yes);
//    qDebug() << QObject::tr("Open database success!");
    // 向数据库中加入密码表
    QSqlQuery query(db);
    query.exec("create table password(pwd varchar primar key);");
    query.exec("insert into password (pwd) values('123456');");

    return true;

}

// 操作数据库
static bool queryTable(const QString &sql)
{
    QSqlQuery query;

    // 执行sql语句
    if(!query.exec(sql))
    {
        QMessageBox::critical(0,
                              ("ERROR"),
                              QString("Error!!!\nSQL = '%1'.\nError : %2").arg(sql).arg(query.lastError().text()),
                              QMessageBox::Yes);
        return false;
    }
    else
    {
//        QMessageBox::information(0,
//                              ("SUCCESS"),
//                              QString("Success!!!\nSQL = '%1'").arg(sql).arg(query.lastError().text()),
//                                 QMessageBox::Yes);
        return true;
    }
}

#endif // CONNECTION_H
