#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
class QTreeView;
class QDateEdit;
class QSortFilterProxyModel;
QT_END_NAMESPACE

class FilterLineEdit;
class MySortFilterProxyModel;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setSourceModel(QAbstractItemModel *model);

private slots:
    void textFilterChanged();
    void dateFilterChanged();
private:

    QTreeView *m_originalView;
    QTreeView *m_proxyView;
    MySortFilterProxyModel *m_proxyModel;

    FilterLineEdit *patternLineEdit;
    QDateEdit *fromDateEdit;
    QDateEdit *toDateEdit;

};
#endif // WIDGET_H
