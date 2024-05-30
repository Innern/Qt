#ifndef MODIFYPSWDIALOG_H
#define MODIFYPSWDIALOG_H

#include <QDialog>

namespace Ui {
class ModifyPSWDialog;
}

class ModifyPSWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyPSWDialog(QWidget *parent = 0);
    ~ModifyPSWDialog();

private slots:

    void on_LineEdit_OldPSW_returnPressed();

    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::ModifyPSWDialog *ui;
    bool m_enModify;
};

#endif // MODIFYPSWDIALOG_H
