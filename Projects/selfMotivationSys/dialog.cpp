#include "dialog.h"
#include "ui_dialog.h"

#include <QColorDialog>
#include <QMenu>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->fontComboBox_fontType, SIGNAL(currentFontChanged()), this, SLOT(changeFont()));
    connect(ui->comboBox_fontSize, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentFontSizeChanged(QString)));

    initCombox();

    m_fontColor = Qt::white;

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initCombox()
{
    QStringList strList;
    for (int i=3;i<48;i++)
    {
        strList << QString::number(i);
    }
    ui->comboBox_fontSize->addItems(strList);
    ui->comboBox_fontSize->setCurrentIndex(0);
}

void Dialog::setInsertTextColor(QColor col)
{
    QPalette palete = ui->plainTextEdit_panRecord->palette();
    palete.setColor(QPalette::Active, QPalette::Text, col);
    palete.setColor(QPalette::Inactive, QPalette::Text, col);
    ui->plainTextEdit_panRecord->setPalette(palete);
}

void Dialog::changeFont()
{
    setPlainTextFont();
}

void Dialog::currentFontSizeChanged(const QString &indexStr)
{
    setPlainTextFont();
}

void Dialog::getInputData()
{
    m_strPlainTextContext = ui->plainTextEdit_panRecord->toPlainText();
    emit sendText(m_strPlainTextContext);//发送信号
}

void Dialog::createMenu(const QPoint &pos)
{
    QMenu *menu = ui->plainTextEdit_panRecord->createStandardContextMenu();
    menu->exec(pos);
}

void Dialog::setPlainTextFont()
{
    //设置字体样式
    QFont font = ui->fontComboBox_fontType->currentFont();
    // 	ui.plainTextEdit_panRecord->setFont(font);

    //设置字体大小
    ui->comboBox_fontSize->setCurrentText(m_strFontSize);
    int iFontSize = ui->comboBox_fontSize->currentText().toInt();
    font.setPointSize(iFontSize);
    ui->plainTextEdit_panRecord->setFont(font);
}

void Dialog::on_pushButton_selectColor_clicked()
{
    QColorDialog colorDlg;
    colorDlg.setCurrentColor(Qt::black);
    colorDlg.exec();
    QColor color = colorDlg.selectedColor();

    setInsertTextColor(color);
}
