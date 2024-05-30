#ifndef WARNINGDLG_H
#define WARNINGDLG_H

#include <QDialog>

namespace Ui {
class warningDlg;
}

class warningDlg : public QDialog
{
    Q_OBJECT

public:
    explicit warningDlg(QWidget *parent = nullptr);
    ~warningDlg();
signals:
    void sendExit(bool exit);
private slots:
    void on_pushButton_exit_clicked();

private:
    Ui::warningDlg *ui;
};

#endif // WARNINGDLG_H
