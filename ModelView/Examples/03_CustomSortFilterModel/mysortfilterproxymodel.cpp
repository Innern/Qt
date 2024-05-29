#include "mysortfilterproxymodel.h"

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

QDate MySortFilterProxyModel::filterMinimumDate() const
{
    return minimumDate;
}

void MySortFilterProxyModel::setFilterMinimumDate(QDate date)
{
    minimumDate = date;
    invalidateFilter();
}

QDate MySortFilterProxyModel::filterMaximumDate() const
{
    return MaximumDate;
}

void MySortFilterProxyModel::setFilterMaximumDate(QDate date)
{
    MaximumDate = date;
    invalidateFilter();
}

bool MySortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // 如果主题和发件人包含给定的正则表达式，并且日期有效，则接受一行
    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex index2 = sourceModel()->index(source_row, 2, source_parent);
    return (sourceModel()->data(index0).toString().contains(filterRegularExpression()) ||
            sourceModel()->data(index1).toString().contains(filterRegularExpression())) &&
            dateInRange(sourceModel()->data(index2).toDate());

}

bool MySortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // // 获取源模型中左右两侧的数据
    // QVariant dataLeft = sourceModel()->data(source_left);
    // QVariant dataRight = sourceModel()->data(source_right);
    // // 根据数据的类型排序，有QDateTime和QString两种类型
    // if(dataLeft.userType() == QMetaType::QDateTime){
    //     return dataLeft.toDateTime() < dataRight.toDateTime();
    // } else {
    //     // 根据发件人的邮箱地址排序
    //     // 获取邮箱地址
    //     const QRegularExpression emailPattern("[\\w\\.]*@[\\w\\.]*");
    //     QString leftString = dataLeft.toString();
    //     if(source_left.column() == 1){  // 列1是发件人
    //         const QRegularExpressionMatch match = emailPattern.match(leftString);
    //         if(match.hasMatch()){
    //             leftString = match.captured(0);
    //         }
    //     }
    //     QString rightString = dataRight.toString();
    //     if(source_right.column() == 1){
    //         const QRegularExpressionMatch match = emailPattern.match(rightString);
    //         if(match.hasMatch()){
    //             rightString = match.captured(0);
    //         }
    //     }

    //     return QString::localeAwareCompare(leftString, rightString) < 0;
    // }

    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);
//! [4]

//! [6]
    if (leftData.userType() == QMetaType::QDateTime) {
        return leftData.toDateTime() < rightData.toDateTime();
    } else {
        static const QRegularExpression emailPattern("[\\w\\.]*@[\\w\\.]*");

        QString leftString = leftData.toString();
        if (source_left.column() == 1) {
            const QRegularExpressionMatch match = emailPattern.match(leftString);
            if (match.hasMatch())
                leftString = match.captured(0);
        }
        QString rightString = rightData.toString();
        if (source_right.column() == 1) {
            const QRegularExpressionMatch match = emailPattern.match(rightString);
            if (match.hasMatch())
                rightString = match.captured(0);
        }

        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}

bool MySortFilterProxyModel::dateInRange(QDate date) const
{
    return (!minimumDate.isValid() || date > minimumDate) &&
            (!MaximumDate.isValid() || date < MaximumDate);
}
