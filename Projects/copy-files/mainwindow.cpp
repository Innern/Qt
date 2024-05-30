#pragma execution_character_set("utf-8")
#include "copyfilesthread.h"
#include "Forms/rawinfodialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdarg.h>  // 可变参数列表

#include <QDate>  // 日期
#include <QTime>  // 事件
#include <QScrollBar>
#include <QFileDialog>  // 文件对话框
#include <QFile>  // 文件操作
#include <QFileInfo>  // 文件信息
#include <QSettings>  // 注册表/配置文件操作
#include <QDir>  // 目录操作
#include <QKeyEvent>  // 键盘事件
#include <QMessageBox>

#include <opencv.hpp>

#define PATH_PRE_OLD_PATH   "Path/oldPath"
#define PATH_PRE_NEW_PATH   "Path/newPath"
#define PATH_OLD_FILE_FILTER   "FileFilter/old_Filter"
#define PATH_NEW_FILE_FILTER   "FileFilter/new_Filter"
#define PATH_FILE_CONVERT   "FileConvert/fileConvertType"
#define PATH_FILE_CONVERT_ENABLE   "FileConvert/fileConvertTypeEnable"

#define IMAGE_RAW_WIDTH   "RawImage/width"
#define IMAGE_RAW_HEIGHT   "RawImage/height"
#define IMAGE_IS_RAW10   "RawImage/Raw10"
#define IMAGE_RAW_BAYER   "RawImage/RawBayer"

#define FILE_CONVERT_NULL  "null"
#define FILE_CONVERT_RAW10_TO_RAW8  "Raw10ToRaw8"
#define FILE_CONVERT_RAW_TO_BMP  "RawToBmp"


void SAFE_DELETE_ARRAY( void *p)
{
    if(p)
        delete[] p;
    p=nullptr;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pRawInfoDlg(Q_NULLPTR)
{
    ui->setupUi(this);
    m_strCfg = QDir::currentPath() + "config.ini";
    // 初始化界面设置
    InitUI();
    // 加载配置
    LoadOptions();

    // 线程操作相关
    thread = new CopyFilesThread;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int keys = event->key();
    switch (keys) {
    case Qt::Key_O:  // 清空显示
    {
        on_oldPath_toolButton_clicked();
    }
        break;
    case Qt::Key_N:  // 清空显示
    {
        on_newPath_toolButton_clicked();
    }
        break;
    case Qt::Key_C:  // 清空显示
    {
        ui->showInformation_plainTextEdit->clear();
    }
        break;
    case Qt::Key_Q:
    case Qt::Key_Escape:
    {
        SaveOptions();
        exit(0);
    }
        break;
    }
}

void MainWindow::AddLog(const char *fmt,...)
{
    QPlainTextEdit *plainTextEdit = ui->showInformation_plainTextEdit;
    // 设置当前控件为只读
    if(!ui->showInformation_plainTextEdit->isReadOnly())
    {
        ui->showInformation_plainTextEdit->setReadOnly(true);
    }

    // 设置光标到文本末尾
    plainTextEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);

    va_list list;
    va_start(list,fmt);
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString str = QString().vsprintf(fmt,list);
    va_end(list);
    QString strtext = "";
    strtext.sprintf("[%04d-%02d-%02d %02d:%02d:%02d.%03d]  %s\r\n",date.year(),date.month(),date.day(),time.hour(),time.minute(),time.second(),time.msec(),str.toStdString().data());

    ui->showInformation_plainTextEdit->insertPlainText(strtext);

    // 移动滚动条到底部
    QScrollBar * scrollbar = ui->showInformation_plainTextEdit->verticalScrollBar();
    if(scrollbar)
    {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

bool MainWindow::read_write_binary(const char *filename, void *p, size_t elementSize, size_t size, const char *op)
{
    FILE *pfile = nullptr;
    pfile = fopen(filename,op);
    if(!pfile)
        return false;
    if("rb" == op)
    {
        fread(p,elementSize,size,pfile);
    }
    else
    {
        fwrite(p,elementSize,size,pfile);
    }

    fclose(pfile);
    pfile = nullptr;
    return true;
}

void MainWindow::RemoveFile(const QString &filepath, const QString &filename)
{
    if(QFileInfo(filename).exists())  // 如果已经存在文件，就删除
    {
        QDir(filepath).remove(filename);
    }
}

void MainWindow::CreateDirectory(const QString &filepath)
{
    if(!QDir(filepath).exists())  // 如果路径不存在则创建目录
    {
        QDir(filepath).mkdir(filepath);
    }
}

bool MainWindow::CheckFiles()
{
    if(m_strFileFilter.isEmpty())
        AddLog("未设置文件过滤符，默认遍历指定路径下的所有文件");
    else
        AddLog("过滤文件类型：%s",m_strFileFilter.toStdString().data());

    if(m_strNewFileFilter.isEmpty())
        m_strNewFileFilter = m_strFileFilter;

    if(m_strOldPath.isEmpty())
    {
        AddLog("select old path is empty!");
        return false;
    }
    m_strOldPath += "/";
    AddLog("select old path : %s",m_strOldPath.toStdString().data());
    if(m_strNewPath.isEmpty())
    {
        m_strNewPath = QDir::currentPath() + "/copy/";
        AddLog("select new path is empty, set current path : %s",m_strNewPath.toStdString().data());
    }
    else
    {
        m_strNewPath += "/";
        AddLog("select new path : %s",m_strNewPath.toStdString().data());
    }

    return true;
}


void MainWindow::on_oldPath_toolButton_clicked()
{
    m_strOldPath = QFileDialog::getExistingDirectory(this,tr("选择一个目录"),m_strPreOldPath,QFileDialog::ShowDirsOnly);
    if(m_strOldPath.isEmpty())
    {
        AddLog("未选择目录");
        return;
    }

    m_strPreOldPath = m_strOldPath;
    // 界面显示选择的文件
    ui->oldPathLineEdit->setText(m_strOldPath);
}

void MainWindow::on_newPath_toolButton_clicked()
{
    m_strNewPath = QFileDialog::getExistingDirectory(this,tr("选择一个目录"),m_strPreNewPath,QFileDialog::ShowDirsOnly);
    if(m_strNewPath.isEmpty())
    {
        AddLog("未选择目录");
        return;
    }

    m_strPreNewPath = m_strNewPath;
    // 把当前选择的路径保存下来
    ui->newPathLineEdit->setText(m_strNewPath);
}

void MainWindow::on_copy_pushButton_clicked()
{
    SaveOptions();
    if(!CheckFiles())
        return;

    AddLog("获取指定目录下的文件...");
    QStringList filesList = GetFileList(m_strOldPath,m_strFileFilter);
    if(filesList.isEmpty())
    {
        AddLog("未获取到任何文件");
        return;
    }

    CreateDirectory(m_strNewPath);  // 如果路径不存在则创建目录

    AddLog("开始复制文件...");
    int nIndex=0;
    for(QString oldFilename : filesList)
    {
        nIndex++;
        AddLog("%dth old file : %s", nIndex,oldFilename.toStdString().data());
        // 获取当前文件的名字
        QString fileBaseName = QFileInfo(oldFilename).baseName();  // 获取文件名
        QString newFilename = m_strNewPath +  fileBaseName + m_strNewFileFilter;  // 复制的文件
        AddLog("%dth new file : %s", nIndex, newFilename.toStdString().data());

        RemoveFile(m_strNewPath,newFilename);  // 如果已经存在文件，就删除

        // 复制文件到指定路径下
        if(!QFile::copy(oldFilename,newFilename))
        {
            AddLog("%dth copy file %s fail !!!", nIndex, fileBaseName.toStdString().data());
        }
        // 转换文件
        FileConvert(m_strFileConvert,oldFilename,fileBaseName);
        // 实时更新显示信息
        QApplication::processEvents();
    }

}

QStringList MainWindow::GetFileList(const QString &dirpath, const QString &fileFilter/* = " "*/)
{
    QStringList filesList;
    QDir dir(dirpath);
    QFileInfoList filesInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    for(QFileInfo fileInfo:filesInfoList)
    {
        if(fileInfo.isDir())  // 如果获取的是目录，获取该目录下的所有文件
        {
            GetFileList(fileInfo.absoluteFilePath());
        }
        else if(fileInfo.isFile())  // 如果是文件，根据过滤器获取文件
        {
            QString strtmp = fileInfo.absoluteFilePath();
            if(strtmp.endsWith(fileFilter))
            {
                filesList.push_back(strtmp);
            }
        }
    }

    return filesList;
}


void MainWindow::on_fileConvert_ComboBox_currentIndexChanged(const QString &arg1)
{
    m_strFileConvert = arg1;

    if(arg1.contains("raw", Qt::CaseInsensitive)) {
        if(!m_pRawInfoDlg)
            m_pRawInfoDlg = new RawInfoDialog(this);
        if(m_pRawInfoDlg->exec() == QDialog::Accepted){
            ui->copy_pushButton->setEnabled(true);
            m_iRawWidth = m_pRawInfoDlg->m_width;
            m_iRawHeight = m_pRawInfoDlg->m_height;
            m_isRaw10 = (m_pRawInfoDlg->m_bits == RawInfoDialog::Bits_Raw10 ? true : false);
            switch (m_pRawInfoDlg->m_bayer) {
            case RawInfoDialog::Bayer_RGGB:
                m_strRawBayers = "rggb";
                break;
            case RawInfoDialog::Bayer_GRBG:
                m_strRawBayers = "grbg";
                break;
            case RawInfoDialog::Bayer_GBRG:
                m_strRawBayers = "gbrg";
                break;
            case RawInfoDialog::Bayer_BGGR:
                m_strRawBayers = "bggr";
                break;
            }
        } else{
            ui->copy_pushButton->setEnabled(false);
        }
    }

}

void MainWindow::on_action_Key_Shortcut_triggered()
{
    QString strText="";
    strText.sprintf("O：加载源目录\r\nN：加载目标目录\r\nC：清空显示信息\r\nQ/Esc：退出程序");
    QMessageBox::information(this,"快捷键设置",strText,QMessageBox::Yes);

}

void MainWindow::InitUI()
{
    // 文件转换列表插入列表条目
    m_sFileConvertList.append(FILE_CONVERT_NULL);
    m_sFileConvertList.append(FILE_CONVERT_RAW10_TO_RAW8);
    m_sFileConvertList.append(FILE_CONVERT_RAW_TO_BMP);
    ui->fileConvert_ComboBox->insertItems(0,m_sFileConvertList);


}

void MainWindow::LoadOptions()
{
    QSettings setting(m_strCfg,QSettings::IniFormat);
    // 获取上一次数据
    m_strPreOldPath = setting.value(PATH_PRE_OLD_PATH,QVariant(QDir::currentPath())).toString();
    m_strPreNewPath = setting.value(PATH_PRE_NEW_PATH,QVariant(QDir::currentPath())).toString();
    m_strFileFilter = setting.value(PATH_OLD_FILE_FILTER,QVariant(".txt")).toString();
    m_strNewFileFilter = setting.value(PATH_NEW_FILE_FILTER,QVariant(".txt")).toString();
    m_strFileConvert = setting.value(PATH_FILE_CONVERT,QVariant("Raw10ToRaw8")).toString();
    m_iRawWidth = setting.value(IMAGE_RAW_WIDTH,QVariant(0)).toInt();
    m_iRawHeight = setting.value(IMAGE_RAW_HEIGHT,QVariant(0)).toInt();
    m_isRaw10 = setting.value(IMAGE_IS_RAW10,QVariant(false)).toBool();
    m_strRawBayers = setting.value(IMAGE_RAW_BAYER,QVariant("rggb")).toString();

    // 把变量显示到界面上
    Cache2UI();
}

void MainWindow::SaveOptions()
{
    // 获取界面控件的数据
    UI2Cache();
    // 保存变量数据到配置文件
    QSettings setting(m_strCfg,QSettings::IniFormat);

    setting.setValue(PATH_PRE_OLD_PATH,QVariant(m_strOldPath));
    setting.setValue(PATH_PRE_NEW_PATH,QVariant(m_strNewPath));
    setting.setValue(PATH_OLD_FILE_FILTER,QVariant(m_strFileFilter));
    setting.setValue(PATH_NEW_FILE_FILTER,QVariant(m_strNewFileFilter));
    setting.setValue(PATH_FILE_CONVERT,QVariant(m_strFileConvert));

    setting.setValue(IMAGE_RAW_WIDTH,QVariant(m_iRawWidth));
    setting.setValue(IMAGE_RAW_HEIGHT,QVariant(m_iRawHeight));
    setting.setValue(IMAGE_IS_RAW10,QVariant(m_isRaw10));
    setting.setValue(IMAGE_RAW_BAYER,QVariant(m_strRawBayers));

}

void MainWindow::Cache2UI()
{
    ui->oldPathLineEdit->setText(m_strPreOldPath);
    ui->newPathLineEdit->setText(m_strPreNewPath);
    ui->filesFilterLineEdit->setText(m_strFileFilter);
    ui->newFileSuffixLineEdit->setText(m_strNewFileFilter);
    ui->fileConvert_ComboBox->setCurrentText(m_strFileConvert);
}

void MainWindow::UI2Cache()
{
    m_strOldPath = ui->oldPathLineEdit->text();
    m_strNewPath = ui->newPathLineEdit->text();
    m_strFileFilter = ui->filesFilterLineEdit->text();
    m_strNewFileFilter = ui->newFileSuffixLineEdit->text();
    m_strFileConvert = ui->fileConvert_ComboBox->currentText();
}

void MainWindow::FileConvert(const QString &fileConvertType, const QString &oldFilename,const QString &fileBaseName)
{
    if("null" == fileConvertType)
        return;

    unsigned short *praw10 = new unsigned short[m_iRawWidth * m_iRawHeight];
    unsigned char *praw8 = new unsigned char[m_iRawWidth * m_iRawHeight];

    QString newFile = m_strNewPath + fileBaseName;
    try
    {
        if(!praw8 || !praw10)
        {
            AddLog("申请内存失败");
            SAFE_DELETE_ARRAY(praw8);
            SAFE_DELETE_ARRAY(praw10);
            return;
        }

        if(FILE_CONVERT_RAW10_TO_RAW8 == fileConvertType)
        {
            newFile.append("_raw8.raw");
        }
        else if(FILE_CONVERT_RAW_TO_BMP == fileConvertType)
        {
            newFile.append(".bmp");
        }
        else
        {
            // 不做转换，预留
        }
        RemoveFile(m_strNewPath,newFile);  // 如果已经存在文件，就删除

        if(m_isRaw10)
        {
            AddLog("raw10 to bmp : %s",newFile.toStdString().data());
            // raw10需要转换成raw8
            if(!read_write_binary(oldFilename.toStdString().data(),praw10,sizeof(praw10[0]),m_iRawWidth*m_iRawHeight,"rb"))
            {
                AddLog("读取文件 %s 失败！！！",oldFilename.toStdString().data());
                throw false;
            }
            for(int i=0;i<m_iRawWidth*m_iRawHeight;++i)
            {
                praw8[i] = ((praw10[i]>>2) & 0xff);
            }
        }
        else
        {
            AddLog("raw8 to bmp : %s",newFile.toStdString().data());
            if(!read_write_binary(oldFilename.toStdString().data(),praw8,sizeof(praw8[0]),m_iRawWidth*m_iRawHeight,"rb"))
            {
                AddLog("读取文件 %s 失败！！！",oldFilename.toStdString().data());
                throw false;
            }
        }



        if(fileConvertType == FILE_CONVERT_RAW10_TO_RAW8)  // raw10 转 raw8
        {
            if(!read_write_binary(newFile.toStdString().data(),praw8,sizeof(praw8[0]),m_iRawWidth*m_iRawHeight,"wb"))
            {
                AddLog("保存文件 %s 失败！！！",oldFilename.toStdString().data());
                throw false;
            }
        }
        else if(fileConvertType == FILE_CONVERT_RAW_TO_BMP)
        {
            int nCorlorType = cv::COLOR_BayerBG2BGR;
            cv::Mat raw8mat(m_iRawHeight,m_iRawWidth,CV_8UC1,praw8);

            if("rggb" == m_strRawBayers)
                nCorlorType = cv::COLOR_BayerRG2RGB;
            else if("grbg" == m_strRawBayers)
                nCorlorType = cv::COLOR_BayerGR2RGB;
            else if("gbrg" == m_strRawBayers)
                nCorlorType = cv::COLOR_BayerGB2RGB;
            else
                nCorlorType = cv::COLOR_BayerBG2RGB;

            cv::Mat bgrmat;
            cv::cvtColor(raw8mat,bgrmat,nCorlorType);
            cv::imwrite(newFile.toStdString().data(),bgrmat);

        }
        else
        {
            // 不做转换，预留
        }
    }
    catch(...)
    {
        AddLog("[%s] fail!!!",__FUNCTION__);
    }

    SAFE_DELETE_ARRAY(praw8);
    SAFE_DELETE_ARRAY(praw10);
}
