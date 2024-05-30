#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT
public:
    enum UserMode{User_Operator, User_Administrator};
public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();
    bool isAdministrator() const;
    bool closeLoginDialog() const;
private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void on_comboBox_userOp_currentTextChanged(const QString &arg1);

    void on_pushButton_ModifyPSW_clicked();

    void on_comboBox_userOp_activated(int index);

private:
    Ui::LoginDlg *ui;
    bool m_isAdministrator;
    bool m_bClose;
};

#endif // LOGINDLG_H
