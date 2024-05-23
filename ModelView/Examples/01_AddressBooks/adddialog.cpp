#include "adddialog.h"

#include <QtWidgets>

AddDialog::AddDialog(QWidget *parent)
    : QDialog{parent}
    , nameLineEdit(new QLineEdit(this))
    , addressTextEdit(new QTextEdit(this))
{

    QFormLayout *formLayout = new QFormLayout;

    formLayout->addRow(tr("&Name:"), nameLineEdit);
    formLayout->addRow(tr("&Address:"), addressTextEdit);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox ::Cancel, this);
    formLayout->addWidget(btnBox);


    connect(btnBox, &QDialogButtonBox::accepted, this, &AddDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &AddDialog::reject);

    setLayout(formLayout);

}

QString AddDialog::name() const
{
    return nameLineEdit->text();
}

QString AddDialog::address() const
{
    return addressTextEdit->toPlainText();
}

void AddDialog::editAddress(const QString &name, const QString &address)
{
    nameLineEdit->setReadOnly(true);
    nameLineEdit->setText(name);
    addressTextEdit->setPlainText(address);
}
