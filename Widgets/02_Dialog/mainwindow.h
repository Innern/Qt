#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QFileDialog>
#include <QWizard>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QDialog;
class QErrorMessage;
class QWizardPage;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_reWelcome_clicked();

    void on_pushButton_getColor_clicked();

    void on_pushButton_getExistingDir_clicked();

    void on_pushButton_getOpenFileName_clicked();

    void on_pushButton_getOpenFileNames_clicked();

    void on_pushButton_getSaveFileName_clicked();

    void on_pushButton_getFont_clicked();

    void on_pushButton_getText_clicked();

    void on_pushButton_getInt_clicked();

    void on_pushButton_getDouble_clicked();

    void on_pushButton_getItem_clicked();

    void on_pushButton_getMultiLineText_clicked();

    void on_pushButton_msg_question_clicked();

    void on_pushButton_msg_information_clicked();

    void on_pushButton_msg_warning_clicked();

    void on_pushButton_msg_critical_clicked();

    void on_pushButton_msg_about_clicked();

    void on_pushButton_progressDialog_clicked();

    void on_pushButton_errorMessage_clicked();

    void on_pushButton_clicked();

    void on_pushButton_trivialWizard_clicked();

private slots:
    void updateWizardStyle();
    void on_pushButton_classWizard_clicked();

private:
    QFileDialog::Options fileOptions();
    QWizardPage *createPage(const QString &title);
private:
    Ui::MainWindow *ui;
    QErrorMessage *errorDlg;
    QWizard::WizardStyle wizardStyle;

};
#endif // MAINWINDOW_H
