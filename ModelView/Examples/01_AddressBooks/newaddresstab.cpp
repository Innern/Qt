#include "newaddresstab.h"

#include <QtWidgets>

NewAddressTab::NewAddressTab(QWidget *parent)
    : QWidget{parent}
{
    QLabel *display = new QLabel(tr("There are currently no contacts in your address book."
                                    "\nClick [Add] to add new contacts."));

    QPushButton *addBtn = new QPushButton("Add");
    // Add按钮点击，发送addEntry()信号，由AddressWidget接收addEntry()信号
    connect(addBtn, &QPushButton::clicked, this, &NewAddressTab::addEntry);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(display);
    mainLayout->addWidget(addBtn,0, Qt::AlignCenter);
    setLayout(mainLayout);

}
