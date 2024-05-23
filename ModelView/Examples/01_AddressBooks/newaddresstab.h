#ifndef NEWADDRESSTAB_H
#define NEWADDRESSTAB_H

#include <QWidget>

class NewAddressTab : public QWidget
{
    Q_OBJECT
public:
    explicit NewAddressTab(QWidget *parent = nullptr);

signals:
    void addEntry();
};

#endif // NEWADDRESSTAB_H
