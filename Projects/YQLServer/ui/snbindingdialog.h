#ifndef SNBINDINGDIALOG_H
#define SNBINDINGDIALOG_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class SNBindingDialog;
}

class SNBindingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SNBindingDialog( const QString exePath , QWidget *parent = 0);
    ~SNBindingDialog();

signals:
    void signal_showTableWidgetItem(const QStringList &list);

private slots:
    void showProcessResult();
    void showProcessFinished(int exitCode ,QProcess::ExitStatus exitStatus);
    void showTableWidgetItem(const QStringList &list);

    void on_pushButton_refresh_clicked();

    void on_pushButton_binding_clicked();

private:
    void getSnList();

private:
    Ui::SNBindingDialog *ui;
    QString m_strExePath;
    QStringList m_snList;
    QProcess *process;
    QStringList cmdList;
};

#endif // SNBINDINGDIALOG_H
