#include "tablemodel.h"

#include <QMimeData>
#include <QDataStream>
#include <QDebug>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

TableModel::TableModel(const QList<Contact> contacts, QObject *parent)
    : QAbstractTableModel(parent)
    , m_lists(contacts)
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_lists.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(index.row()>m_lists.size() || index.row()<0)
        return QVariant();

    if(Qt::DisplayRole == role){
        Contact contact = m_lists.at(index.row());
        switch (index.column()) {
        case 0:
            return contact.name;
        case 1:
            return contact.address;
        }
    }
    return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable
            | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;  // 可编辑的 | 可拖拽 | 可释放
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && role == Qt::EditRole){
        Contact contact = m_lists.value(index.row());
        switch (index.column()) {
        case 0:
            contact.name = value.toString();
            break;
        case 1:
            contact.address = value.toString();
            break;
        default:
            return false;
        }

        m_lists.replace(index.row(), contact);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    return false;
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row+count-1);

    for(int i=0;i<count;i++){
        m_lists.insert(row,Contact());
    }

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), row, row+count-1);

    for(int i=0;i<count;i++){
        m_lists.removeAt(row);
    }

    endRemoveRows();
    return true;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::DisplayRole != role)
        return QVariant();

    if(Qt::Horizontal == orientation){
        switch (section) {
        case 0:
            return QVariant("name");
        case 1:
            return QVariant("address");
        default:
            break;
        }
    }
    return QVariant();
}
const QString customMimeType = "application/vnd.text.list";  // 自定义的拖放数据的编码类型
Qt::DropActions TableModel::supportedDropActions() const
{
    // 支持移动和复制两种操作
    return Qt::MoveAction | Qt::CopyAction;
}

QStringList TableModel::mimeTypes() const
{
    QStringList mimeTypes;
    mimeTypes << customMimeType;
    return mimeTypes;
}

QMimeData *TableModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray dragData;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QDataStream stream(&dragData, QIODevice::WriteOnly);
#else
    QDataStream stream(&dragData, QDataStream::WriteOnly);
#endif
    // 将所有拖动的数据传入mime类型中
    int row=-1;
    for (const QModelIndex index : indexes) {
        if(index.isValid() && row != index.row()){
            row = index.row();
            Contact contact = m_lists.at(row);
            stream << row << contact.name << contact.address;  // mime数据格式： 行
        }
    }
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(customMimeType, dragData);

    return mimeData;
}

const QList<Contact> &TableModel::getContacts() const
{
    return m_lists;
}

bool TableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if(action == Qt::IgnoreAction)
        return true;
    if(!data->hasFormat(customMimeType))
        return false;
    if(column >1)
        return false;

    // 从mime类型中获取要插入的数据
    QByteArray  encodedData = data->data(customMimeType);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
#else
    QDataStream stream(&encodedData, QDataStream::ReadOnly);
#endif
    QList<Contact> contacts;
    Contact contact;
    int rows = 0;
    int willRemoveRow=-1;
    while (!stream.atEnd()) {
        stream >> willRemoveRow;
        stream >> contact;
        contacts << contact;
        rows++;
        removeRows(willRemoveRow, 1, QModelIndex());
    }
    // 开始插入行
    int beginRow = 0;
    if(row != -1)
        beginRow = row;
    else if(parent.isValid())
        beginRow = parent.row();
    else
        beginRow = rowCount(QModelIndex());

    insertRows(beginRow, rows, QModelIndex());
    for(const Contact item : contacts){
        QModelIndex colIndex = index(beginRow, 0, QModelIndex());
        setData(colIndex, item.name, Qt::EditRole);
        colIndex = index(beginRow, 1, QModelIndex());
        setData(colIndex, item.address, Qt::EditRole);
        beginRow++;
    }

    return true;

}
