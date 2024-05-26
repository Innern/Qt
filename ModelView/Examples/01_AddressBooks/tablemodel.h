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
*    @file:    tablemodel.h
*    @brief:   继承自QAbstractTableModel，表格模型子类，为应用程序提供显示的数据
*    @author:  2314902703@qq.com
*    @date:    2024-05-22  09:54
*/
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>

struct Contact
{
    QString name;
    QString address;
    Contact() {
        name = QString();
        address = QString();
    }
    Contact(const QString &arg1, const QString &arg2) {
        name = arg1;
        address = arg2;
    }

    bool operator==(const Contact &other) const
    {
        return name == other.name && address == other.address;
    }

};

inline QDataStream &operator <<(QDataStream &stream, const Contact &contact)
{
    return stream << contact.name << contact.address;
}
inline QDataStream &operator >>(QDataStream &stream, Contact &contact)
{
    return stream >> contact.name >> contact.address;
}

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);
    TableModel(const QList<Contact> contacts, QObject *parent = nullptr);

    // 模型的行数和列数
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    // 指定索引指定角色的数据
    QVariant data(const QModelIndex &index, int role) const override;
    // 可编辑的模型，需要获取索引项，根据指定的索引设置数据
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    // 在模型中插入行和删除行
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // 显示标头
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // 为模型提供支持拖放的功能
    // 1. 设置支持的拖放操作
    Qt::DropActions supportedDropActions() const override;
    // 2. 设置在拖放操作中导出的条目的数据的编码类型
    QStringList mimeTypes() const override;
    // 3. 将拖放到数据放入QMimeData中
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    // 4. 将拖放操作的数据放入模型中
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    // 获取当前列表
    const QList<Contact> &getContacts() const;


signals:

private:
    QList<Contact> m_lists;
};

#endif // TABLEMODEL_H
