#include "tablemodel.h"

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
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;  // 可编辑的
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
    beginRemoveRows(parent, row, row+count-1);

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

const QList<Contact> &TableModel::getContacts() const
{
    return m_lists;
}
