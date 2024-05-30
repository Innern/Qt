#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    bool enterMainWindow();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_OK_clicked();

private:
    void setQss();
    bool checkMatch();
private:
    Ui::LoginDialog *ui;
    bool m_enterMainWindow;
};

#endif // LOGINDIALOG_H
