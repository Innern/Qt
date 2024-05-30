#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_loadPath = "";


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    m_loadPath = QFileDialog::getExistingDirectory(this,tr("select directory"),QDir::currentPath());
    if(m_loadPath.isEmpty())
    {
        QMessageBox::warning(this,tr("WARNING"),tr("NOT SELECT FILE"));
        return;
    }

    ui->lineEdit->setText(m_loadPath);
}

void MainWindow::on_lineEdit_editingFinished()
{
    m_loadPath = ui->lineEdit->text();
}

void MainWindow::on_pushButton_Convert_clicked()
{
    QTextCodec* codec = QTextCodec::codecForName("utf-8");
    QString strCSVFile = m_loadPath + "\\.csv";
    QFile csvFile(strCSVFile);
    if(csvFile.exists())
        csvFile.remove();

    qDebug() <<tr("获取")  << m_loadPath << tr("目录的文件") << endl;
    QStringList listFiles = GetFilesInCurrentDir(m_loadPath);

    QVector<QString> strvecColumn;
    QVector<QVector<QString> > strContent;
    strvecColumn.clear();

    if(!csvFile.open(QIODevice::Text | QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("WARNING"),tr("OPEN FILE ERROR"));
        return;
    }
    for(QString strFile:listFiles)
    {
        strFile = m_loadPath + "\\" + strFile;
        QFile file(strFile);
        if(!file.open(QIODevice::Text | QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,tr("WARNING"),tr("OPEN FILE ERROR"));
            return;
        }
        QTextStream textStream(&file);
        textStream.setCodec(codec);
        QString strLine = textStream.readLine();
        strvecColumn.clear();
        while (!strLine.isEmpty()) {

            if( "," != strLine[strLine.length()-1])
            {
                strLine += ",";
                strvecColumn.push_back(strLine);
                strLine = textStream.readLine();
            }

        }
        file.close();
        strContent.push_back(strvecColumn);

    }

    QTextStream textStream(&csvFile);

    QString strline = "";
    for (uint16_t col = 0;col<strvecColumn.size();col++) {

        strline = "";
        for(uint16_t row = 0;row<strContent.size();row++)
        {
            strline.append(strContent[row][col]);
        }
        ui->plainTextEdit_txt->appendPlainText(strline);
        ui->plainTextEdit_csv->appendPlainText(strline);
        textStream << strline << endl;
    }

    csvFile.close();

    QString newFile = m_loadPath + "\\summary.csv";

    if(QFile::exists(newFile))
    {
        QFile::remove(newFile);
    }
    if(!csvFile.copy(newFile))
    {
        QMessageBox::warning(this,tr("WARNING"),tr("COPY FILES FAIL"));
        return;
    }

    if(!csvFile.remove())
    {
        QMessageBox::warning(this,tr("WARNING"),tr("REMOVE FILES FAIL"));
        return ;
    }
}

QStringList MainWindow::GetFilesInCurrentDir(const QString &path)
{
    QStringList nameFliter;
    QDir dir(path);
    nameFliter << "*.txt" ;

    QStringList listFiles;
    listFiles = dir.entryList(nameFliter,QDir::Files|QDir::Readable,QDir::Name);
    return listFiles;
}


