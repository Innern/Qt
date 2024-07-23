/**
* This file is part of the QtWidgets module of the Qt Toolkit.
* Copyright (C) 2016 The Qt Company Ltd.
* Commercial License Usage
* Licensees holding valid commercial Qt licenses may use this file in
* accordance with the commercial license agreement provided with the
* Software or, alternatively, in accordance with the terms contained in
* a written agreement between you and The Qt Company. For licensing terms
* and conditions see https://www.qt.io/terms-conditions. For further
* information use the contact form at https://www.qt.io/contact-us.
*/
/**
*    @file:    connection.h
*    @brief:   连接、打开数据库
*    @author:  2314902703@qq.com
*    @date:    2024-03-17  19:32
*/
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QMessageBox>


static bool createConnection()
{
    // 连接、打开数据库
    foreach (const QString &driver, QSqlDatabase::drivers()) {
        if(driver.compare("qsqlite", Qt::CaseInsensitive) == 0){
            QSqlDatabase db = QSqlDatabase::addDatabase(driver);
            db.setDatabaseName("Raffle.db");
            if(!db.open()){
                QMessageBox::warning(0, QString("Warning"), QString("Open database fail.\n%1").arg(db.lastError().text()));
                return false;
            }
//            QSqlQuery query;
//            if(!query.exec("create table luckyBody(id integer primary key autoincrement,"
//                       "nameID text, prizeID int,"
//                       "foreign key(nameID) references staff(id),"
//                       "foreign key(prizeID) references prizes(id))")){
//                QMessageBox::critical(0, QString("Error"), QString("%1").arg(query.lastError().text()));
//            }
            return true;
        }
    }
    QMessageBox::warning(0, QString("Warning"), QString("Open database fail."));

    return false;
}

#endif // CONNECTION_H
