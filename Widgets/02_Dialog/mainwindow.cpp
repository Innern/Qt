#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ClassWizard/classwizard.h"
#include "LicenseWizard/licensewizard.h"
#include "ui_mainwindow.h"
#include "welcomedialog.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    errorDlg = new QErrorMessage(this);

    connect(ui->radioButton_wizard_aeroStyle, &QRadioButton::clicked, this, &MainWindow::updateWizardStyle);
    connect(ui->radioButton_wizard_classicStyle, &QRadioButton::clicked, this, &MainWindow::updateWizardStyle);
    connect(ui->radioButton_wizard_modernStyle, &QRadioButton::clicked, this, &MainWindow::updateWizardStyle);
    connect(ui->radioButton_wizard_macStyle, &QRadioButton::clicked, this, &MainWindow::updateWizardStyle);

    ui->radioButton_wizard_aeroStyle->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFileDialog::Options MainWindow::fileOptions()
{
    QFileDialog::Options options = QFileDialog::Options();
    if(ui->checkBox_fileOption_showDirsOnly->isChecked())
        options |= QFileDialog::ShowDirsOnly;
    if(ui->checkBox_fileOption_donotResolveSymlinks->isChecked())
        options |= QFileDialog::DontResolveSymlinks;
    if(ui->checkBox_fileOption_donotConfirmOverwrite->isChecked())
        options |= QFileDialog::DontConfirmOverwrite;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    if(ui->checkBox_fileOption_donotUseSheet->isChecked())
        options |= QFileDialog::DontUseSheet;
#endif
    if(ui->checkBox_fileOption_donotUseNativeDialog->isChecked())
        options |= QFileDialog::DontUseNativeDialog;
    if(ui->checkBox_fileOption_readOnly->isChecked())
        options |= QFileDialog::ReadOnly;
    if(ui->checkBox_fileOption_hideNameFilterDetails->isChecked())
        options |= QFileDialog::HideNameFilterDetails;
    if(ui->checkBox_fileOption_donotUseCustomDirectoryIcons->isChecked())
        options |= QFileDialog::DontUseCustomDirectoryIcons;

    return options;
}

QWizardPage *MainWindow::createPage(const QString &title)
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(title);
    return page;
}


void MainWindow::on_pushButton_reWelcome_clicked()
{
    close();  // 隐藏主界面
    WelcomeDialog dlg;
    if(dlg.exec() == QDialog::Accepted)
        show();
}

void MainWindow::on_pushButton_getColor_clicked()
{
    QColorDialog::ColorDialogOptions options = QColorDialog::ColorDialogOptions();
    if(ui->checkBox_colorDlg_noButtons->isChecked())
        options |= QColorDialog::NoButtons;
    if(ui->checkBox_colorDlg_showAlpha->isChecked())
        options |= QColorDialog::ShowAlphaChannel;
    if(ui->checkBox_colorDlg_donotUseNativeDialog->isChecked())
        options |= QColorDialog::DontUseNativeDialog;

    QColor selectedColor = QColorDialog::getColor(Qt::red, this, tr("Select Font"), options);
    if(selectedColor.isValid()){
        ui->label_getColor->setPalette(QPalette(selectedColor));

        ui->label_getColor->setText(tr("rgba:(%1, %2, %3, %4)").arg(selectedColor.red())
                                    .arg(selectedColor.green())
                                    .arg(selectedColor.blue())
                                    .arg(selectedColor.alpha()));
    }
}

void MainWindow::on_pushButton_getExistingDir_clicked()
{
    QFileDialog::Options options = fileOptions();
    QString dir = QFileDialog::getExistingDirectory(this,QString(), QString(), options);
    ui->label_getExistingDir->setText(dir);
}

void MainWindow::on_pushButton_getOpenFileName_clicked()
{
    QFileDialog::Options options = fileOptions();
    QString filename = QFileDialog::getOpenFileName(this,QString(), QString(), QString(), nullptr, options);
    ui->label_getOpenFileName->setText(filename);
}

void MainWindow::on_pushButton_getOpenFileNames_clicked()
{
    QFileDialog::Options options = fileOptions();
    QStringList dirList = QFileDialog::getOpenFileNames(this,QString(), QString(), QString(), nullptr, options);
    ui->label_getOpenFileNames->setText(dirList.join(QString(";")));
}

void MainWindow::on_pushButton_getSaveFileName_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this);
    ui->label_getSaveFileName->setText(filename);
    //    QFileDialog fileDlg;
    //    QFileDialog::Options options = fileOptions();
    //    fileDlg.setOptions(options);
    //    fileDlg.exec();
}

void MainWindow::on_pushButton_getFont_clicked()
{
    bool ok;
    QFontDialog::FontDialogOptions options = QFontDialog::FontDialogOptions();

    if(ui->checkBox_fontDlg_noButtons->isChecked())
        options |= QFontDialog::NoButtons;
    if(ui->checkBox_fontDlg_donotUseNativeDialog->isChecked())
        options |= QFontDialog::DontUseNativeDialog;
    if(ui->checkBox_fontDlg_scalable->isChecked())
        options |= QFontDialog::ScalableFonts;
    else
        options |= QFontDialog::NonScalableFonts;
    if(ui->checkBox_fontDlg_monospaced->isChecked())
        options |= QFontDialog::MonospacedFonts;
    if(ui->checkBox_fontDlg_proportional->isChecked())
        options |= QFontDialog::ProportionalFonts;

    QFont font = QFontDialog::getFont(&ok, QFont("Helvetica"), this, QString(), options);

    if(ok){
        ui->pushButton_getFont->setFont(font);
        ui->label_getFont->setText(font.toString());
    } else {
        qDebug() << tr("Don't select font");
    }

}

void MainWindow::on_pushButton_getText_clicked()
{
    QString text = QInputDialog::getText(this, tr("Input Text"), tr("Please input text"));
    ui->label_getText->setText(text);
}

void MainWindow::on_pushButton_getInt_clicked()
{
    int data =QInputDialog::getInt(this, tr("Input Int"), tr("Please input integer data:"));
    ui->label_getInt->setText(tr("%1").arg(data));
}

void MainWindow::on_pushButton_getDouble_clicked()
{
    double data = QInputDialog::getDouble(this, tr("Input Double"), tr("Please input double data:"));
    ui->label_getDouble->setText(tr("%1").arg(data));
}

void MainWindow::on_pushButton_getItem_clicked()
{
    QStringList items;
    items << tr("item1") << tr("item2");
    QString item = QInputDialog::getItem(this, tr("Input Item"), tr("Please input or select a item"),items, 0);
    ui->label_getItem->setText(item);
}

void MainWindow::on_pushButton_getMultiLineText_clicked()
{
    QString text = QInputDialog::getMultiLineText(this, tr("Input Multiple Text"), tr("Please input multiple line text"));
    ui->label_getMultiLineText->setText(text);
}

void MainWindow::on_pushButton_msg_question_clicked()
{
    QMessageBox::StandardButton btn = QMessageBox::question(this, tr("Question Dialog"),
                                                            tr("Do you know Qt?"),
                                                            QMessageBox::Yes | QMessageBox::No);
    if(btn == QMessageBox::Yes)
        qDebug() << tr("Question Dialog");
}

void MainWindow::on_pushButton_msg_information_clicked()
{
    QMessageBox::StandardButton btn = QMessageBox::information(this, tr("Information Dialog"),
                                                               tr("This is Qt book."),
                                                               QMessageBox::Ok);
    if(btn == QMessageBox::Ok)
        qDebug() << tr("Information Dialog");
}

void MainWindow::on_pushButton_msg_warning_clicked()
{
    QMessageBox::StandardButton btn = QMessageBox::warning(this, tr("Warning Dialog"),
                                                           tr("You cann't over!"),
                                                           QMessageBox::Abort);
    if(btn == QMessageBox::Abort)
        qDebug() << tr("Warning Dialog");
}

void MainWindow::on_pushButton_msg_critical_clicked()
{
    QMessageBox::StandardButton btn = QMessageBox::critical(this, tr("Critical Dialog"),
                                                            tr("Found a fatal critical! Must close all files now!"),
                                                            QMessageBox::YesAll);
    if(btn == QMessageBox::YesToAll)
        qDebug() << tr("Critical Dialog");
}

void MainWindow::on_pushButton_msg_about_clicked()
{
    QMessageBox::about(this, tr("About Dialog"),
                       tr("This is Qt about dialog"));
}

void MainWindow::on_pushButton_progressDialog_clicked()
{
    QProgressDialog dialog(tr("文件复制进度"), tr("取消"), 0, 50000, this);
    dialog.setWindowTitle(tr("进度对话框"));  // 设置窗口标题
    dialog.setWindowModality(Qt::WindowModal);  // 将对话框设置为模态
    dialog.show();
    for(int i=0; i<50000; ++i){
        dialog.setValue(i);  // 设置进度条的当前值
        QCoreApplication::processEvents();  // 避免界面冻结
        if(dialog.wasCanceled())
            break;
    }
    dialog.setValue(50000);  // 显示100%
    qDebug() << tr("复制结束！");
}

void MainWindow::on_pushButton_errorMessage_clicked()
{
    errorDlg->setWindowTitle(tr("错误信息对话框"));
    errorDlg->showMessage(tr("这里是出错信息"));
}

void MainWindow::on_pushButton_clicked()
{
    QWizard wizard(this);
    wizard.setWizardStyle(wizardStyle);
    wizard.setWindowTitle(tr("向导对话框"));
    wizard.addPage(createPage(tr("介绍")));
    wizard.addPage(createPage(tr("用户选择信息")));
    wizard.addPage(createPage(tr("结束")));
    wizard.exec();
}

QWizardPage *createIntroPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Introduction");

    QLabel *label = new QLabel("This wizard will help you register your copy "
                               "of Super Product Two.");
    label->setWordWrap(true);  // 设置换行

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);
    return page;
}

QWizardPage *createRegistrationPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Registration");  // 设置标题
    page->setSubTitle("Please fill both fields");  // 设置子标题

    QFormLayout *layout = new QFormLayout;
    QLineEdit *nameLineedit = new QLineEdit("zoya");
    QLineEdit *emailLineedit = new QLineEdit("zoya@qq.com");
    layout->addRow(new QLabel("&Name:"), nameLineedit);
    layout->addRow(new QLabel("&Email address:"), emailLineedit);
    page->setLayout(layout);

    return page;
}

QWizardPage *createConclusionPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Conslusion");

    QLabel *label = new QLabel("You are now successfully registered. Have a "
                               "nice day");
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}
void MainWindow::on_pushButton_trivialWizard_clicked()
{
    QWizard wizard(this);
    wizard.setWizardStyle(wizardStyle);
    wizard.setWindowTitle(tr("Trivial Wizard"));
    wizard.addPage(createIntroPage());
    wizard.addPage(createRegistrationPage());
    wizard.addPage(createConclusionPage());

    wizard.exec();
}

void MainWindow::updateWizardStyle()
{
    if(ui->radioButton_wizard_classicStyle->isChecked())
        wizardStyle = QWizard::ClassicStyle;
    else if(ui->radioButton_wizard_modernStyle->isChecked())
        wizardStyle = QWizard::ModernStyle;
    else if(ui->radioButton_wizard_macStyle->isChecked())
        wizardStyle = QWizard::MacStyle;
    else if(ui->radioButton_wizard_aeroStyle->isChecked())
        wizardStyle = QWizard::AeroStyle;
}

void MainWindow::on_pushButton_classWizard_clicked()
{
    ClassWizard wizard(this);
    wizard.setWizardStyle(wizardStyle);

    wizard.exec();
}

void MainWindow::on_pushButton_licenseWizard_clicked()
{
    LicenseWizard wizard(this);
    wizard.setWizardStyle(wizardStyle);

    wizard.exec();
}
