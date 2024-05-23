#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE

class AddDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddDialog(QWidget *parent = nullptr);

    QString name() const;
    QString address() const;
    void editAddress(const QString &name, const QString &address);

private:
    QLineEdit *nameLineEdit;
    QTextEdit *addressTextEdit;
};

#endif // ADDDIALOG_H
