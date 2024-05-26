/************************************************************
* Copyright (C) 2024. All rights reserved.
* @File name       : addresswidget.cpp
* @version         : V1.0
* @Author          : 2314902703@qq.com
* @Date            : 2024-05-22 09:27
* @Function List   :
* @Description     : 地址簿应用程序的中心部件
*************************************************************/
#include "addresswidget.h"
#include "tablemodel.h"
#include "newaddresstab.h"
#include "adddialog.h"

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QHeaderView>

AddressWidget::AddressWidget(QWidget *parent)
    : QTabWidget{parent}
    , m_tableModel(new TableModel(this))
    , m_newAddressTab(new NewAddressTab(this))
{
    addTab(m_newAddressTab, tr("Address Book"));
    // 接收 NewAddressTab的addEntry()信号
    connect(m_newAddressTab, &NewAddressTab::addEntry, this, &AddressWidget::showAddEntryDialog);

    createTabs();
}

void AddressWidget::readFromFile()
{
    QList<Contact> contacts;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::information(this, tr("Unable to open file."),
                                 file.errorString());
        return;
    }
    QDataStream in(&file);
    in >> contacts;
    // file.close();
    if(contacts.isEmpty()){
        QMessageBox::information(this, tr("No  contacts in file"),
                                 tr("The file you are attempting to open contains no contacts."));
        return;
    } else {
        foreach (auto contact, contacts) {
            addEntry(contact.name, contact.address);
        }
    }

}

void AddressWidget::writeToFile()
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::information(this, tr("Unable to open file."),
                                 file.errorString());
        return;
    }

    QDataStream out(&file);
    out << m_tableModel->getContacts();
    file.close();

}

void AddressWidget::showAddEntryDialog()
{
    AddDialog dialog;
    if(dialog.exec() == QDialog::Accepted){
        addEntry(dialog.name(), dialog.address());
    }
}

void AddressWidget::addEntry(const QString &name, const QString &address)
{
    if(!m_tableModel->getContacts().contains(Contact(name, address))){
        m_tableModel->insertRows(0, 1, QModelIndex());

        QModelIndex index = m_tableModel->index(0,0,QModelIndex());
        m_tableModel->setData(index, name, Qt::EditRole);
        index = m_tableModel->index(0, 1, QModelIndex());
        m_tableModel->setData(index, address, Qt::EditRole);
        removeTab(indexOf(m_newAddressTab));
    } else {
        QMessageBox::information(this, tr("Duplicate Name"),
                                 tr("The name \"%1\" already exists.").arg(name));
    }
}

void AddressWidget::editEntry()
{
    // 获取当前的选项卡
    QTableView *tableView = static_cast<QTableView*>(currentWidget());
    // 代理模型
    QSortFilterProxyModel *filterModel = static_cast<QSortFilterProxyModel*>(tableView->model());
    // 表格视图的选择模型
    QItemSelectionModel *selectionModel = tableView->selectionModel();

    // 获取选择模型的所有索引
    QModelIndexList indexList = selectionModel->selectedRows();

    QString name;
    QString address;
    int row=-1;

    for (const QModelIndex index : indexList) {
        row = filterModel->mapToSource(index).row();  // mapToSource()通过将代理模型的索引转换为源模型的索引
        QModelIndex nameIndex = m_tableModel->index(row, 0, QModelIndex());
        name = nameIndex.data(Qt::DisplayRole).toString();
        QModelIndex addressIndex = m_tableModel->index(row, 1, QModelIndex());
        address = addressIndex.data(Qt::DisplayRole).toString();
    }

    AddDialog addDialog;
    addDialog.setWindowTitle(tr("Edit a Contact"));
    addDialog.editAddress(name, address);

    if(addDialog.exec() == QDialog::Accepted){
        const QString newAddress = addDialog.address();
        if(newAddress != address){
            const QModelIndex addressIndex = m_tableModel->index(row, 1, QModelIndex());
            m_tableModel->setData(addressIndex, newAddress, Qt::EditRole);
        }
    }

}

void AddressWidget::removeEntry()
{
    // 获取当前选项卡的部件，即表格视图
    QTableView *tableView = static_cast<QTableView*>(currentWidget());
    // 获取代理模型
    QSortFilterProxyModel *filterModel = static_cast<QSortFilterProxyModel*>(tableView->model());
    // 获取表格视图的选择模型
    QItemSelectionModel *selectedModel = tableView->selectionModel();

    // 获取选择模型选择的所有索引
    QModelIndexList indexList = selectedModel->selectedRows();

    for (const QModelIndex index : indexList) {
        // 根据代理模型获取源模型的索引的行
        int row = filterModel->mapToSource(index).row();
        // 删除该行
        m_tableModel->removeRow(row, QModelIndex());
    }
    // 如果模型中数据为空，添加newAddressTab选项卡
    if(m_tableModel->rowCount(QModelIndex()) == 0){
        insertTab(0, m_newAddressTab, tr("Address Book"));
    }
}

void AddressWidget::createTabs()
{
    const QStringList labels = {"ABC", "DEF", "GHI", "JKL", "MNO", "PQR", "STU", "VW", "XYZ"};

    foreach (const QString &label, labels) {

        QTableView *tableView = new QTableView;

        QSortFilterProxyModel *filterModel = new QSortFilterProxyModel(this);
        filterModel->setSourceModel(m_tableModel);
        filterModel->setFilterRegularExpression(QRegularExpression(QString("^[%1].*").arg(label), QRegularExpression::CaseInsensitiveOption));
        filterModel->setFilterKeyColumn(0);  // 设置过滤的是哪一列

        tableView->setModel(filterModel);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选择一行
        tableView->horizontalHeader()->setStretchLastSection(true);  // 最后一个可见列占用所有剩余空间
        tableView->verticalHeader()->hide();  // 隐藏垂直标题栏
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 编辑策略
        tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);  // 单选
        tableView->setSortingEnabled(false);

        // 启用拖放操作
        tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableView->setDragEnabled(true);  // 设置拖放功能可用
        tableView->setAcceptDrops(true);  // 启用拖放
        tableView->setDropIndicatorShown(true);  // 显示要拖放的位置

        connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &AddressWidget::selectionChanged);
        connect(this, &QTabWidget::currentChanged, this, [this, tableView](int tabIndex){
            if(widget(tabIndex) == tableView)
                emit selectionChanged(tableView->selectionModel()->selection());
        });

        addTab(tableView, label);
    }

}
