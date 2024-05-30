#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imageviewfram.h"
#include <QTimer>

#include <opencv.hpp>

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_oldImage(QImage()),
    m_newImage(QImage()),
    pAlgoImg(nullptr)
{
    ui->setupUi(this);
    initWidget();
    initParam();
}

MainWindow::~MainWindow()
{
    m_scene.clear();

    if(pAlgoImg)
        delete pAlgoImg;

    delete ui;

    cv::destroyAllWindows();

}

void MainWindow::on_action_Open_File_triggered()
{
    // 文件对话框
    QFileDialog dialog;
    // 初始化文件对话框
    initImageFileDialog(dialog, QFileDialog::AcceptOpen);
    // 运行文件对话框
    while (dialog.exec() && !loadFile(dialog.selectedFiles().constFirst())) {

    }

    // 获取选择的所有文件
    m_listFilenames = dialog.selectedFiles();
    qDebug() << tr("Select files:");
    foreach (const QString &filename, m_listFilenames) {
        qDebug() << filename;
    }
}

void MainWindow::initWidget()
{
    int index = 0;
    QStringList tmpList;
    tmpList << QStringLiteral("Constant") << QStringLiteral("Replicate") << QStringLiteral("Reflect")
            << QStringLiteral("Wrap") << QStringLiteral("Refect_101");
    ui->comboBox_FilterBorderType->addItems(tmpList);
    ui->comboBox_FilterBorderType->setCurrentIndex(4);
    m_borderType = cv::BORDER_DEFAULT;
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_CONSTANT));
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_REPLICATE));
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_REFLECT));
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_REFLECT));
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_WRAP));
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_REFLECT_101));
    //    ui->comboBox_FilterBorderType->setItemData(index++, QVariant::fromValue(cv::BORDER_TRANSPARENT));

    // 默认raw图信息不显示
    ui->dockWidget_ImageInfo->hide();

    ui->comboBox_RawBits->setItemData(index++, QVariant::fromValue(8));
    ui->comboBox_RawBits->setItemData(index++, QVariant::fromValue(10));
    ui->comboBox_RawBits->setItemData(index++, QVariant::fromValue(12));
    ui->comboBox_RawBits->setItemData(index++, QVariant::fromValue(16));

    ImageViewFram *viewFrame = new ImageViewFram;
    viewFrame->view()->setScene(&m_scene);

    setCentralWidget(viewFrame);

    connect(ui->spinBox_RawWidth, SIGNAL(valueChanged(int)), this, SLOT(updateRawImageInfo()));
    connect(ui->spinBox_RawHeight, SIGNAL(valueChanged(int)), this, SLOT(updateRawImageInfo()));
    connect(ui->comboBox_RawBayer, &QComboBox::currentTextChanged, this, &MainWindow::updateRawImageInfo);
    connect(ui->comboBox_RawBits, &QComboBox::currentTextChanged, this, &MainWindow::updateRawImageInfo);

}

void MainWindow::initParam()
{
    pAlgoImg = new Algo_ImageProcess;
    QStringList imageLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);

    m_oldImage = QImage();
    m_newImage = QImage();

    m_listFilenames.clear();

    m_srcImage = cv::Mat();
}

void MainWindow::initImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode mode, const QStringList &fileFilters)
{
    // 1. 第一次打开文件对话框，设置文件目录
    static bool firstDialog = true;

    if(firstDialog){
        firstDialog = false;
        // 获取图片文件的目录
        const QStringList imageLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        qDebug() << imageLocations;
        // 文件对话框的打开路径
        dialog.setDirectory(imageLocations.isEmpty() ? QDir::currentPath() : imageLocations.constFirst());

    }

    // 设置可以选择多个文件，默认只能选择一个文件
    if(mode == QFileDialog::AcceptOpen)
        dialog.setFileMode(QFileDialog::ExistingFiles);

    // 设置支持的图片类型
    if(fileFilters.isEmpty()){
        QStringList mimeTypeFilters;
        const QByteArrayList supportedMimeType = (mode == QFileDialog::AcceptOpen ? QImageReader::supportedMimeTypes()
                                                                                  : QImageWriter::supportedMimeTypes());
        foreach (const QByteArray mimeTypename, supportedMimeType) {
            mimeTypeFilters << mimeTypename;
        }
        qDebug() << mimeTypeFilters;
        mimeTypeFilters.sort();
        // 设置文件对话框的过滤器
        dialog.setMimeTypeFilters(mimeTypeFilters);

        // 选择默认的图片类型
        dialog.selectMimeTypeFilter("image/bmp");
    } else {

        dialog.setNameFilters(fileFilters);
    }

    // 如果是保存文件，默认保存类型为bmp
    if(mode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("bmp");
}

bool MainWindow::loadFile(const QString &filename)
{
    m_fullFileName = filename;
    // 加载文件
    if(filename.endsWith(".raw")){
        // 加载raw
        ui->dockWidget_ImageInfo->show();
        qDebug() << tr("Load raw file");

    } else {
        // 加载支持的图片
        ui->dockWidget_ImageInfo->hide();

        if(!m_oldImage.load(filename) || m_oldImage.isNull()){
            QMessageBox::warning(this, tr("Warning"), tr("Open the image file fail, %1").arg(filename), QMessageBox::Ok);
            return false;
        }
        m_oldImage = m_oldImage.convertToFormat(QImage::Format_RGB888);
        outputImageInfo(m_oldImage);
        populateScene(QPixmap::fromImage(m_oldImage));
        m_newImage = m_oldImage.copy();
        m_newImage.fill(0);

        if(!m_srcImage.empty())
            m_srcImage.release();
        m_srcImage = cv::imread(m_fullFileName.toStdString().data(), cv::IMREAD_GRAYSCALE);
        return !m_srcImage.empty();
    }

    return true;
}

bool MainWindow::loadTif()
{

    return true;
}

bool MainWindow::saveFile(const QString &filename)
{
    m_fullFileName = filename;
    // 保存文件
    if(filename.endsWith(".raw")){
        // 保存raw
        qDebug() << tr("Haven't add the function save raw.");

    } else {
        // 保存图片
        ui->dockWidget_ImageInfo->hide();

        if(!m_newImage.save(filename)){
            QMessageBox::warning(this, tr("Warning"), tr("Save the image file fail, %1").arg(filename), QMessageBox::Ok);
            return false;
        }

    }

    return true;
}

bool MainWindow::loadRawFile(const QString &filename)
{
    QFileInfo fi(filename);
    qint64 fileSize = m_rawImgInfo.width * m_rawImgInfo.height * (ceil(m_rawImgInfo.rawBits/8.0));
    qDebug() << tr("Setup image size:") << fileSize;
    if(fileSize != fi.size()){
        QMessageBox::warning(this, tr("Warning"), tr("The file size is wrong!"), QMessageBox::Ok);
        return false;
    }

    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, tr("Warning"), tr("Open the image file fail, %1!").arg(file.errorString()), QMessageBox::Ok);
        return false;
    }

    uchar *pFileBuf = new uchar[fileSize];
    memset(pFileBuf, 0, sizeof(pFileBuf[0])*fileSize);
    file.read((char*)pFileBuf, fileSize);

    m_oldImage = QImage(m_rawImgInfo.width, m_rawImgInfo.height, QImage::Format_Grayscale8);
    uchar *pImageBuf = m_oldImage.bits();
    qDebug() << pImageBuf;
    if(m_rawImgInfo.rawBits > 8){
        for(int i=0; i<fileSize; i+=2){
            pFileBuf[i/2] = (((pFileBuf[i+1]*256 + pFileBuf[i]) >> 2) & 0xff);  // 小端
        }
    }
    for(int i=0; i<m_rawImgInfo.width*m_rawImgInfo.height; ++i) {
        pImageBuf[i] = pFileBuf[i];
    }

    m_newImage = m_oldImage.copy();
    m_newImage.fill(0);
    populateScene(QPixmap::fromImage(m_oldImage));

    return true;
}

void MainWindow::outputImageInfo(const QImage &img)
{
    qDebug() << tr("Image size = %1 * %2").arg(img.size().width()).arg(img.size().height());
    qDebug() << tr("Image width = %1").arg(img.width());
    qDebug() << tr("Image height = %1").arg(img.height());
    qDebug() << tr("Image format = %1").arg(img.format());
    qDebug() << tr("Image depth = %1").arg(img.depth());
    qDebug() << tr("Image bytesPerLine = %1").arg(img.bytesPerLine());
    qDebug() << tr("Image colorCount = %1").arg(img.colorCount());
    qDebug() << tr("Image devicePixelRatio = %1").arg(img.devicePixelRatio());
    qDebug() << tr("Image sizeInBytes = %1").arg(img.sizeInBytes());
}

void MainWindow::populateScene(const QPixmap &pixmap)
{
    m_scene.clear();

    QGraphicsPixmapItem *item = m_scene.addPixmap(pixmap);
}

void MainWindow::updateFilterType()
{
    const QString arg1 = ui->comboBox_SpatialFilterTypes->currentText();
    if(arg1.startsWith("mean", Qt::CaseInsensitive)){
        m_filterType = FilterType_Mean;

    } else if(arg1.startsWith("box", Qt::CaseInsensitive)){
        m_filterType = FilterType_Box;

    } else if(arg1.startsWith("median", Qt::CaseInsensitive)){
        m_filterType = FilterType_Median;

    } else if(arg1.startsWith("gaussian", Qt::CaseInsensitive)){
        m_filterType = FilterType_Gaussian;

    } else if(arg1.startsWith("bilateral", Qt::CaseInsensitive)){
        m_filterType = FilterType_Bilateral;

    } else if(arg1.startsWith("laplace", Qt::CaseInsensitive)){
        m_filterType = FilterType_Laplace;

    } else if(arg1.startsWith("8laplace", Qt::CaseInsensitive)){
        m_filterType = FilterType_8Laplace;
    } else if(arg1.startsWith("highlift", Qt::CaseInsensitive)){
        m_filterType = FilterType_HighLift;
    } else if(arg1.startsWith("sobel", Qt::CaseInsensitive)){
        m_filterType = FilterType_Sobel;
    }
}

void MainWindow::updateBorderType()
{
    // 边界填充类型
    int idx = ui->comboBox_FilterBorderType->currentIndex();
    if(0 == idx){
        m_borderType = cv::BORDER_CONSTANT;  //  iiiiii|abcdefgh|iiiiii  复制指定的常量扩展边界
    } else if(1 == idx){
        m_borderType = cv::BORDER_REPLICATE;  // aaaaaa|abcdefgh|hhhhhh  复制边缘的像素扩展边界
    } else if(2 == idx){
        m_borderType = cv::BORDER_REFLECT;  // fedcba|abcdefgh|hgfedcba  镜像复制扩展边界
    } else if(3 == idx){
        m_borderType = cv::BORDER_WRAP;  // cdefgh|abcdefgh|abcdef  复制对边的像素扩展边界
    } else if(4 == idx){
        m_borderType = cv::BORDER_REFLECT_101;  // gfedcb|abcdefgh|gfedcb  通过镜像复制扩展边界，但是边界像素除外
    }
}

void MainWindow::noRepeatClick(QWidget *obj, int delay_ms)
{
    obj->setEnabled(false);

    QTimer::singleShot(delay_ms,this, [=](){
        obj->setEnabled(true);
    });
}

void MainWindow::on_actionOpen_Raw_File_triggered()
{
    QFileDialog dialog;
    QStringList fileFilters;
    fileFilters << "Raw Image(*.raw)";

    initImageFileDialog(dialog,QFileDialog::AcceptOpen, fileFilters);

    while (dialog.exec() && !loadFile(dialog.selectedFiles().constFirst())) {

    }

}

void MainWindow::updateRawImageInfo()
{
    // 确保获取的是最新修改的值
    ui->spinBox_RawWidth->interpretText();

    m_rawImgInfo.width = ui->spinBox_RawWidth->value();
    m_rawImgInfo.height = ui->spinBox_RawHeight->value();
    m_rawImgInfo.rawBits = ui->comboBox_RawBits->currentData().toInt();
    const QString bayer = ui->comboBox_RawBayer->currentText();
    if(bayer == "RGGB"){
        m_rawImgInfo.rawBayer = RawBayer_RGGB;
    } else if(bayer == "GRBG"){
        m_rawImgInfo.rawBayer = RawBayer_GRBG;
    } else if(bayer == "GBRG"){
        m_rawImgInfo.rawBayer = RawBayer_GBRG;
    } else if(bayer == "BGGR"){
        m_rawImgInfo.rawBayer = RawBayer_BGGR;
    }
}

void MainWindow::on_pushButton_OK_clicked()
{
    // 获取最新raw数据
    updateRawImageInfo();

    // 加载raw图并显示
    loadRawFile(m_fullFileName);
}

void MainWindow::on_actionRGB2HSI_triggered()
{
    pAlgoImg->RGB2HSI(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height());

    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_actionRGB2Gray_triggered()
{
    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), QImage::Format_Grayscale8);

    pAlgoImg->RGB2Gray(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height());
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Sum_triggered()
{
    qDebug() << tr("Math-Sum");
    if(m_listFilenames.isEmpty() || m_listFilenames.size() < 2){
        qDebug() << tr("At least 2 images are required!");
        return;
    }

    const int width = m_oldImage.width();
    const int height = m_oldImage.height();
    const int bytes = m_oldImage.sizeInBytes();
    ushort *pdst = new ushort[bytes];
    memset(pdst, 0, sizeof(pdst[0])*bytes);
    int imgCnt = 0;
    foreach (const QString filename, m_listFilenames) {
        QImage img(filename);
        img = img.convertToFormat(QImage::Format_RGB888);
        const int newBytes = img.sizeInBytes();
        if(newBytes != bytes){
            qDebug() << tr("The pictures size mismatch!");
            if(pdst)
                delete[] pdst;
            pdst = nullptr;
            return;
        }

        for(int j=0; j<bytes; ++j){
            pdst[j] += img.bits()[j];
        }

        imgCnt++;

    }

    pAlgoImg->GrayscaleConversion_u16To8(pdst, m_newImage.bits(), width, height, m_newImage.depth()/8);

    //    m_newImage = QImage((uchar*)pdst, width, height, m_oldImage.format());

    populateScene(QPixmap::fromImage(m_newImage));

}

void MainWindow::on_action_Reverse_triggered()
{
    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->GrayscaleConversion_Reverse(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Log_triggered()
{
    const double c = QInputDialog::getDouble(this, tr("Log"), tr("Please input c:"), 1);


    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->GrayscaleConversion_Log(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8, c);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}


void MainWindow::on_action_Power_triggered()
{
    const QString text = QInputDialog::getText(this, tr("Power"), tr("Please input c&gamma, space between:"), QLineEdit::Normal, tr("1 1"));
    QStringList vals = text.split(" ");
    const double c = vals.isEmpty() ? 1 : vals.at(0).toDouble();
    const double gamma = vals.isEmpty() ? 1 : vals.at(1).toDouble();
    qDebug() << tr("c = %1, gamma = %2").arg(c).arg(gamma);

    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->GrayscaleConversion_Power(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8, c, gamma);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Bit_layering_triggered()
{
    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->GrayscaleConversion_BitLayering(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Normalize_triggered()
{
    m_oldImage = m_oldImage.convertToFormat(QImage::Format_Grayscale8);

    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->Histogram_normalize(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Save_File_triggered()
{
    // 文件对话框
    QFileDialog dialog(this, tr("Save file"));
    // 初始化文件对话框
    initImageFileDialog(dialog, QFileDialog::AcceptSave);
    // 运行文件对话框
    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {

    }

}

void MainWindow::on_action_Equalization_triggered()
{
    m_oldImage = m_oldImage.convertToFormat(QImage::Format_Grayscale8);

    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->Histogram_equalization(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Matching_triggered()
{
    qDebug() << tr("Histogram matching...");

    m_oldImage = m_oldImage.convertToFormat(QImage::Format_Grayscale8);

    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());

    pAlgoImg->Histogram_matching(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_action_Local_triggered()
{
    qDebug() << tr("Histogram local...");

    m_oldImage = m_oldImage.convertToFormat(QImage::Format_Grayscale8);

    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());
    m_newImage.fill(255);

    pAlgoImg->Histogram_local(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), m_newImage.depth()/8);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_actionOpen_TIF_triggered()
{
    // 文件对话框
    QFileDialog dialog;
    QStringList fileFilters;
    fileFilters << "TIF Image(*.tif *.tiff)";

    // 初始化文件对话框
    initImageFileDialog(dialog, QFileDialog::AcceptOpen, fileFilters);
    // 运行文件对话框
    while (dialog.exec() && !loadFile(dialog.selectedFiles().constFirst())) {

    }

    // 获取选择的所有文件
    m_listFilenames = dialog.selectedFiles();
    qDebug() << tr("Select files:");
    foreach (const QString &filename, m_listFilenames) {
        qDebug() << filename;
    }

}

void MainWindow::on_action_Statistics_triggered()
{
    // 需要输入 C k_0 k_1 k_2 k_3
    qDebug() << tr("Histogram statistics...");

    const QString text = QInputDialog::getText(this, tr("Power"), tr("Please input C k_0 k_1 k_2 k_3, space between:"), QLineEdit::Normal, tr("22.8 0 0.1 0 0.1"));
    QStringList vals = text.split(" ");
    const double c = vals.isEmpty() ? 1 : vals.at(0).toDouble();
    const double k0 = vals.isEmpty() ? 1 : vals.at(1).toDouble();
    const double k1 = vals.isEmpty() ? 1 : vals.at(2).toDouble();
    const double k2 = vals.isEmpty() ? 1 : vals.at(3).toDouble();
    const double k3 = vals.isEmpty() ? 1 : vals.at(4).toDouble();
    qDebug() << tr("c = %1, k_0 = %2, k_1 = %3, k_2 = %4, k_3 = %5").arg(c).arg(k0).arg(k1).arg(k2).arg(k3);

    m_oldImage = m_oldImage.convertToFormat(QImage::Format_Grayscale8);

    m_newImage = QImage(m_oldImage.width(), m_oldImage.height(), m_oldImage.format());
    m_newImage.fill(0);

    pAlgoImg->Histogram_statistics(m_oldImage.bits(), m_newImage.bits(), m_oldImage.width(), m_oldImage.height(), c, k0, k1, k2, k3);
    outputImageInfo(m_newImage);
    populateScene(QPixmap::fromImage(m_newImage));
}

void MainWindow::on_pushButton_SpatialFilterOk_clicked()
{
    if(m_oldImage.isNull())
        return;

    updateFilterType();
    updateBorderType();

    const bool normalize = ui->checkBox_SpatialFilterNormalize->isChecked();
    const float param1 = ui->doubleSpinBox_FilterScale->value();
    const float param2 = ui->doubleSpinBox_FilterDelta->value();
    const int borderValue = ui->spinBox_BorderValue->value();
    int kSize = ui->spinBox_FilterKSize->value();
    kSize = (kSize % 2) ? kSize : kSize+1;

    if(m_filterType == FilterType_Mean){

        if(cv::BORDER_WRAP != m_borderType) {

            pAlgoImg->SpatialFilter_Mean(m_srcImage, kSize, m_borderType, borderValue);
        }

    } else if(FilterType_Box == m_filterType){
        pAlgoImg->SpatialFilter_Box(m_srcImage, kSize, normalize, m_borderType, borderValue);

    } else if(FilterType_Median == m_filterType){
        pAlgoImg->SpatialFilter_Median(m_srcImage, kSize);

    } else if(FilterType_Gaussian == m_filterType){
        pAlgoImg->SpatialFilter_Gaussian(m_srcImage, kSize, param1, param2, m_borderType);

    } else if(FilterType_Bilateral == m_filterType){
        // 双边滤波
        pAlgoImg->SpatialFilter_Bilateral(m_srcImage, kSize, param1, param2, m_borderType);

    } else if(FilterType_Laplace == m_filterType){

        pAlgoImg->SpatialFilter_laplace(m_srcImage.data, NULL, m_srcImage.cols, m_srcImage.rows, m_borderType);

    } else if(FilterType_8Laplace == m_filterType){

        pAlgoImg->SpatialFilter_laplace(m_srcImage, param1, m_borderType);

    }  else if(FilterType_HighLift == m_filterType){

        pAlgoImg->SpatialFilter_HighLift(m_srcImage, param2, kSize, param1, 0,m_borderType);
    }  else if(FilterType_Sobel == m_filterType){

        pAlgoImg->SpatialFilter_Sobel(m_srcImage, kSize, param1, param2, m_borderType);
    }

}

void MainWindow::on_comboBox_FilterBorderType_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    noRepeatClick(ui->comboBox_FilterBorderType);

    on_pushButton_SpatialFilterOk_clicked();

}

void MainWindow::on_comboBox_SpatialFilterTypes_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    noRepeatClick(ui->comboBox_SpatialFilterTypes);

    on_pushButton_SpatialFilterOk_clicked();
}

void MainWindow::on_checkBox_SpatialFilterNormalize_stateChanged(int arg1)
{
    Q_UNUSED(arg1);

    noRepeatClick(ui->checkBox_SpatialFilterNormalize);

    on_pushButton_SpatialFilterOk_clicked();
}

void MainWindow::on_spinBox_BorderValue_valueChanged(int arg1)
{
    Q_UNUSED(arg1);

    noRepeatClick(ui->spinBox_BorderValue);

    on_pushButton_SpatialFilterOk_clicked();
}

void MainWindow::on_spinBox_FilterKSize_valueChanged(int arg1)
{
    Q_UNUSED(arg1);

    noRepeatClick(ui->spinBox_FilterKSize);

    on_pushButton_SpatialFilterOk_clicked();
}

void MainWindow::on_doubleSpinBox_FilterScale_valueChanged(double arg1)
{
    Q_UNUSED(arg1);

    noRepeatClick(ui->doubleSpinBox_FilterScale);

    on_pushButton_SpatialFilterOk_clicked();
}

void MainWindow::on_action_Concentric_Circular_reflector_triggered()
{
    qDebug() << tr("Generate Concentric Circular Reflector");

    // z = 1/2 * [1 + cos(x*x + y*y)]

    float scale = 8.2;
    float step = 0.0275;

    int width = ceil(2 * scale / step);

    cv::Mat mCircle = cv::Mat::zeros(width, width, CV_32FC1);
    float *pdata = (float*)mCircle.data;
    for(float y=-scale; y<=scale; y+=0.0275){
        for(float x=-scale; x<=scale; x+=0.0275){

        }
    }


}
