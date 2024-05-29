#ifndef MYSORTFILTERPROXYMODEL_H
#define MYSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QDate>

class MySortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MySortFilterProxyModel(QObject *parent = nullptr);

    QDate filterMinimumDate() const;
    void setFilterMinimumDate(QDate date);
    QDate filterMaximumDate() const;
    void setFilterMaximumDate(QDate date);

protected:
    // 仅接受具有有效日期的行
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    // 使模型能够按照发件人的电子邮件地址对其排序。
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    // 判断输入的日期是否有效
    bool dateInRange(QDate date) const;

private:
    QDate minimumDate;
    QDate MaximumDate;
};

#endif // MYSORTFILTERPROXYMODEL_H
