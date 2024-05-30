#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "algo_imageprocess.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>



namespace Ui {
class MainWindow;
}
QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Open_File_triggered();

    void on_actionOpen_Raw_File_triggered();

    void updateRawImageInfo();
    void on_pushButton_OK_clicked();

    void on_actionRGB2HSI_triggered();

    void on_actionRGB2Gray_triggered();

    void on_action_Sum_triggered();

    void on_action_Reverse_triggered();

    void on_action_Log_triggered();

    void on_action_Power_triggered();

    void on_action_Bit_layering_triggered();

    void on_action_Normalize_triggered();

    void on_action_Save_File_triggered();

    void on_action_Equalization_triggered();

    void on_action_Matching_triggered();

    void on_action_Local_triggered();

    void on_actionOpen_TIF_triggered();

    void on_action_Statistics_triggered();

    void on_pushButton_SpatialFilterOk_clicked();

    void on_comboBox_FilterBorderType_currentIndexChanged(int index);

    void on_comboBox_SpatialFilterTypes_currentIndexChanged(const QString &arg1);

    void on_checkBox_SpatialFilterNormalize_stateChanged(int arg1);

    void on_spinBox_BorderValue_valueChanged(int arg1);

    void on_spinBox_FilterKSize_valueChanged(int arg1);

    void on_doubleSpinBox_FilterScale_valueChanged(double arg1);

    void on_action_Concentric_Circular_reflector_triggered();

private:
    void initWidget();
    void initParam();
    void initImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode mode, const QStringList &fileFilters = QStringList());
    bool loadFile(const QString &filename);
    bool loadTif();
    bool saveFile(const QString &filename);
    bool loadRawFile(const QString &filename);
    void outputImageInfo(const QImage &img);

    // 填充场景
    void populateScene(const QPixmap &pixmap);

    void updateFilterType();
    void updateBorderType();

    void noRepeatClick(QWidget *obj, int delay_ms = 500);
private:
    Ui::MainWindow *ui;
    Algo_ImageProcess *pAlgoImg;
    QGraphicsScene m_scene;
    Algo_ImageProcess::ImageInfo m_rawImgInfo;
    QString m_fullFileName;
    QImage m_oldImage;
    QImage m_newImage;

    QStringList m_listFilenames;

    // 滤波参数
    SpatialFilterType m_filterType;
    int m_borderType;
    cv::Mat m_srcImage;

};

#endif // MAINWINDOW_H
