#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractItemModel>

QT_BEGIN_NAMESPACE
class QTreeView;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QSortFilterProxyModel;
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setSourceModel(QAbstractItemModel *model);

private slots:
    void updateFilterRegularExpression();
    void filterColumnChanged(int index);
    void sortCaseSensitiveChanged(bool checked = false);

private:
    void createUI();
private:
    enum Syntax{
        RegularExpression,
        Wildcard,
        FixedString
    };
    QTreeView *m_originalView;
    QTreeView *m_sortFilterView;
    QSortFilterProxyModel *m_proxyModel;
    QLineEdit *patternLineEdit;
    QComboBox *syntaxComboBox;
    QComboBox *columnComboBox;
    QCheckBox *filterCaseSensitiveCheck;
    QCheckBox *sortCaseSensitiveCheck;

};
#endif // WIDGET_H
