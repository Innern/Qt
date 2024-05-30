#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVariant>
#include <QStringList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class CopyFilesThread;
class RawInfoDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief keyPressEvent 函数功能：键盘按下事件
     * @param event  键盘事件
     */
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_oldPath_toolButton_clicked();

    void on_newPath_toolButton_clicked();

    void on_copy_pushButton_clicked();


    void on_fileConvert_ComboBox_currentIndexChanged(const QString &arg1);

    void on_action_Key_Shortcut_triggered();

private:
    /**
     * @brief InitUI UI界面初始化设置
     */
    void InitUI();
    /**
     * @brief LoadOptions 从配置文件加载变量
     */
    void LoadOptions();
    /**
     * @brief SaveOptions 保存变量数据到配置文件
     */
    void SaveOptions();
    /**
     * @brief Cache2UI 变量显示到UI控件
     */
    void Cache2UI();
    /**
     * @brief UI2Cache UI控件值赋给变量
     */
    void UI2Cache();
    /**
     * @brief FileConvert 函数功能：文件转换，目前支持：raw10 -> raw8、raw -> bmp
     * @param fileConvertType 文件转换类型：Raw10ToRaw8、RawToBmp
     * @param oldFilename  源文件名，包含路径及后缀
     * @param fileBaseName 文件名，不包含路径及后缀
     */
    void FileConvert(const QString &fileConvertType, const QString &oldFilename,const QString &fileBaseName);
    /**
     * @brief GetFileList  函数功能：获取指定路径下指定类型的文件
     * @param dirpath  要获取文件的路径
     * @param fileFilter  文件过滤符
     * @return
     */
    QStringList GetFileList(const QString &dirpath, const QString &fileFilter = "");
    /**
     * @brief AddLog 函数功能：显示操作信息到界面上
     * @param fmt  要显示的信息
     */
    void AddLog(const char *fmt,...);

    /**
     * @brief read_write_binary  函数功能：读取或保存二进制文件，可以是二进制数据或者类似raw图等
     * @param filename  文件名
     * @param p  读取或保存的buffer
     * @param elementSize  读取或保存的buffer的一个元素的字节大小，比如 unsigned short为2个字节，unsigned char为1个字节
     * @param size  读取或保存文件的大小
     * @param op  操作类型，rb=读取二进制文件，wb=保存二进制文件
     * @return   读取或保存文件成功返回true，打开文件失败返回false
     */
    bool read_write_binary(const char* filename,void *p, size_t elementSize,size_t size, const char *op);

    /**
     * @brief RemoveFile 函数功能：如果文件存在，则删除文件
     * @param filepath  文件所在路径
     * @param filename  // 文件名
     */
    void RemoveFile(const QString & filepath,const QString &filename);
    /**
     * @brief CreateDirectory  函数功能：创建目录
     * @param filepath  要创建的文件目录
     */
    void CreateDirectory(const QString &filepath);
private:
    /**
     * @brief CheckFiles 函数功能：检查界面设置是否有误
     * @return  界面设置没有问题返回true，否则返回false
     */
    bool CheckFiles();
private:
    Ui::MainWindow *ui;
    QString m_strOldPath;
    QString m_strNewPath;
    QString m_strFileFilter;
    QString m_strNewFileFilter;
    QString m_strPreOldPath;  // 上一次选择的文件存放的旧路径
    QString m_strPreNewPath;  // 上一次选择的文件存放的新路径

    QString m_strCfg;  // 保存程序配置文件

    QStringList m_sFileConvertList;
    QString m_strFileConvert;
    QString m_strRawBayers;

    int m_iRawWidth;
    int m_iRawHeight;
    bool m_isRaw10;

    CopyFilesThread *thread;  // 拷贝文件线程
    RawInfoDialog *m_pRawInfoDlg;  // raw图相关信息
};
#endif // MAINWINDOW_H
