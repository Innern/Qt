#ifndef ADDRESSWIDGET_H
#define ADDRESSWIDGET_H

#include <QTabWidget>
#include <QStandardPaths>
#include <QStringList>
#include <QItemSelection>

class TableModel;
class NewAddressTab;

class AddressWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit AddressWidget(QWidget *parent = nullptr);

    void readFromFile();
    void writeToFile();
public slots:
    void showAddEntryDialog();
    void addEntry(const QString &name, const QString &address);  // 添加项目
    void editEntry();  // 编辑所有项目
    void removeEntry();  // 移除所有项目
signals:
    void selectionChanged(QItemSelection selected);
private:
    void createTabs();

private:

    const QString filename =
            QStandardPaths::standardLocations(QStandardPaths::TempLocation).value(0)
            + QStringLiteral("/addressbook.dat");

    TableModel *m_tableModel;
    NewAddressTab *m_newAddressTab;
};

#endif // ADDRESSWIDGET_H
