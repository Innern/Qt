#include "algo_imageprocess.h"

#include <math.h>

Algo_ImageProcess::Algo_ImageProcess()
{

}

void Algo_ImageProcess::RGB2HSI(const unsigned char *psrc, unsigned char *pdst, int width, int height){

    cv::Mat src(height, width, CV_8UC3);
    src.data = (unsigned char*)psrc;
    cv::Mat dst;
    cv::cvtColor(src,dst,cv::COLOR_RGB2HSV);
    memcpy(pdst, dst.data, dst.total()*dst.channels());
}

void Algo_ImageProcess::RGB2Gray(const unsigned char *psrc, unsigned char *pdst, int width, int height)
{
    cv::Mat src(height, width, CV_8UC3);
    src.data = (unsigned char*)psrc;
    cv::Mat dst;
    cv::cvtColor(src,dst,cv::COLOR_RGB2GRAY);

    memcpy(pdst, dst.data, dst.total()*dst.channels());
}

void Algo_ImageProcess::GrayscaleConversion_u16To8(const unsigned short *psrc, unsigned char *pdst, int width, int height, int channels)
{
    /// 灰度转换计算
    /// g_m = g - min(g)
    /// g_s = K * [g_m / max(g_m)]

    // 找到原始图像最小值
    unsigned short min = UINT_MAX;

    for(int i=0; i<width*height*channels; ++i)
        min = min > psrc[i] ? psrc[i] : min;

    unsigned short *pM = new unsigned short[width*height*channels];
    memset(pM, 0, sizeof(pM[0])*width*height*channels);

    // 获取g_m图像
    unsigned short max = 0;
    for(int i=0; i<width*height*channels; ++i) {
        pM[i] = psrc[i] - min;
        max = max < pM[i] ? pM[i] : max;
    }

    // 固定到8bit范围
    int K = 255;
    for(int i=0; i<width*height*channels; ++i) {
        pdst[i] = (unsigned char)K * (pM[i] * 1.0 / max);
    }

    if(pM)
        delete[] pM;
    pM = nullptr;
}

void Algo_ImageProcess::GrayscaleConversion_16To8(const short *psrc, unsigned char *pdst, int width, int height, int channels)
{
    /// 灰度转换计算
    /// g_m = g - min(g)
    /// g_s = K * [g_m / max(g_m)]

    // 找到原始图像最小值
    short min = INT_MAX;

    for(int i=0; i<width*height*channels; ++i)
        min = min > psrc[i] ? psrc[i] : min;

    short *pM = new short[width*height*channels];
    memset(pM, 0, sizeof(pM[0])*width*height*channels);

    // 获取g_m图像
    short max = 0;
    for(int i=0; i<width*height*channels; ++i) {
        pM[i] = psrc[i] - min;
        max = max < pM[i] ? pM[i] : max;
    }

    // 固定到8bit范围
    int K = 255;
    for(int i=0; i<width*height*channels; ++i) {
        pdst[i] = (unsigned char)K * (pM[i] * 1.0 / max);
    }

    if(pM)
        delete[] pM;
    pM = nullptr;
}

void Algo_ImageProcess::GrayscaleConversion_double2uchar(const float *psrc, unsigned char *pdst, int width, int height, int channels)
{
    /// 灰度转换计算
    /// g_m = g - min(g)
    /// g_s = K * [g_m / max(g_m)]

    // 找到原始图像最小值
    float min = 65535;

    for(int i=0; i<width*height*channels; ++i)
        min = min > psrc[i] ? psrc[i] : min;

    float *pM = new float[width*height*channels];
    memset(pM, 0, sizeof(pM[0])*width*height*channels);

    // 获取g_m图像
    float max = 0;
    for(int i=0; i<width*height*channels; ++i) {
        pM[i] = psrc[i] - min;
        max = max < pM[i] ? pM[i] : max;
    }

    // 固定到8bit范围
    int K = 255;
    for(int i=0; i<width*height*channels; ++i) {
        pdst[i] = (unsigned char)K * (pM[i] * 1.0 / max);
    }

    if(pM)
        delete[] pM;
    pM = nullptr;
}

void Algo_ImageProcess::GrayscaleConversion_Reverse(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels)
{
    for(int i=0;i<width*height*channels; ++i){
        *pdst++ = 255 - *psrc++;
    }
}

void Algo_ImageProcess::GrayscaleConversion_Log(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels, double c)
{
    float *pTmp = new float[width*height*channels];
    memset(pTmp, 0, sizeof(pTmp[0]) * width * height * channels);

    for(int i=0;i<width*height*channels; ++i){
        pTmp[i] = (c * std::log(1 + psrc[i]));
    }

    GrayscaleConversion_double2uchar(pTmp, pdst, width, height, channels);

    if(pTmp)
        delete[] pTmp;
    pTmp = nullptr;
}

void Algo_ImageProcess::GrayscaleConversion_Power(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels, double c, double gamma)
{
    if(c == 1 && gamma == 1){
        memcpy(pdst, psrc, width*height*channels*sizeof(psrc[0]));
        return;
    }

    float *pTmp = new float[width*height*channels];
    memset(pTmp, 0, sizeof(pTmp[0]) * width * height * channels);
    for(int i=0;i<width*height*channels; ++i){
        pTmp[i] = (c * std::pow(psrc[i], gamma));
    }

    GrayscaleConversion_double2uchar(pTmp, pdst, width, height, channels);

    if(pTmp)
        delete[] pTmp;
    pTmp = nullptr;
}

void Algo_ImageProcess::GrayscaleConversion_BitLayering(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels)
{
    cv::Mat src(height, width, CV_8UC3);
    src.data = (unsigned char*)psrc;
    cv::Mat dst;

    unsigned char offset = 1;

    do{
        cv::Mat mTmp(height, width, CV_8UC3, cv::Scalar(offset, offset, offset));

        dst = src & mTmp;
        //        cv::bitwise_and(src, mTmp, dst);
        uchar *ptmp = dst.data;
        for(int i=0;i<dst.cols * dst.rows * dst.channels(); ++i){
            ptmp[i] = (ptmp[i]!= 0) ? 255 : 0;
        }
        cv::cvtColor(dst,dst,cv::COLOR_RGB2BGR);

        cv::imshow("Bit Layering", dst);
        cv::waitKey(0);


    } while(0 != (offset = offset << 1));

    cv::cvtColor(dst,dst,cv::COLOR_BGR2RGB);

    memcpy(pdst, dst.data, dst.total()*dst.channels());
    cv::destroyAllWindows();
}

void Algo_ImageProcess::Histogram_normalize(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels)
{
    int pHist[255];
    memset(pHist, 0, sizeof(pHist));

    // 计算每个灰度的像素数
    for(int i=0;i<width*height*channels; ++i){
        pHist[psrc[i]]++;
    }

    // 归一化处理
    float pNormHist[255]={0};
    for(int i=0; i<255; ++i){
        pNormHist[i] = 1.0*pHist[i] / (width*height*channels);
    }

    uchar pTmp[255]={0};
    GrayscaleConversion_double2uchar(pNormHist,pTmp,255, 1, 1);


    // 绘制折线
    // 创建用于绘制折线的图片
    cv::Mat mImage = cv::Mat::zeros(480, 640, CV_8UC3);
    mImage.setTo(cv::Scalar(100,0,0));

    // 拟合点
    std::vector<cv::Point> pts;
    for(int i=0;i<255; i++){
        pts.push_back(cv::Point(i*2, int( mImage.rows - pTmp[i])));
    }


    // 将拟合点绘制在空白图上
    for(int i=0;i<pts.size(); ++i){
        cv::circle(mImage, pts[i], 3, cv::Scalar(0,200,200), 2, 8, 0);
    }

    // 绘制折线
    cv::polylines(mImage, pts, false, cv::Scalar(0,0,255), 1, 8, 0);

    cv::imshow("Polylines", mImage);
    cv::waitKey(0);

    cv::destroyAllWindows();

}

void Algo_ImageProcess::Histogram_equalization(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels)
{
    // 计算每个灰度级的像素数量
    int nGrayCount[255]={0};
    int sizeByte = width*height*channels;
    for(int i=0; i<sizeByte; ++i){
        nGrayCount[psrc[i]]++;
    }

    // 计算每个灰度级的概率
    float fGrayP[255]={0};
    for(int i=0;i<255;++i){
        fGrayP[i] = nGrayCount[i]*1.0/sizeByte;
    }

    // 计算映射
    float s[255] = {0};
    for(int i=0;i<255;++i){
        s[i] = 0;
        for(int j=0;j<i;++j){
            s[i] += 255.0 * fGrayP[j];
        }
    }

    // 四舍五入映射
    int nS[255] = {0};
    for(int i=0; i<255; ++i){
        nS[i] = int(s[i] + 0.5);
    }

    // 均衡化
    for(int i=0;i<sizeByte; ++i){
        pdst[i] = nS[psrc[i]];
    }

}

void Algo_ImageProcess::Histogram_matching(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels)
{
    // 1. 获取直方图均衡化的图像
    Histogram_equalization(psrc, pdst, width, height, channels);

    // 2. 计算 G(z_q) 的值
    // 首先获取目标图像的直方图

    // 计算 G(z_q) 值

    // 3. 找到 G(z_q) 最接近 s_k 时对应的 z_q 值

    // 4. 将直方图均衡化后的图像像素映射到直方图规定化图像中的对应像素，形成直方图规定化后的图像

}

void Algo_ImageProcess::Histogram_local(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels)
{
    // 局部直方图处理
    //memcpy(pdst, psrc, sizeof(pdst[0])*width*height*channels);
    unsigned char pTmpEqu[9] = {0};
    for(int row=1; row<height-1; ++row){
        for(int col=1; col<width-1; ++col){
            unsigned char pTmp[9] = {
                *(psrc+row*width+col - width - 1),  *(psrc+row*width+col - width),  *(psrc+row*width+col - width + 1),
                *(psrc+row*width+col - 1),          *(psrc+row*width+col),          *(psrc+row*width+col + 1),
                *(psrc+row*width+col + width - 1),  *(psrc+row*width+col+ width),   *(psrc+row*width+col+ width + 1)
            };

            Histogram_equalization(pTmp, pTmpEqu,3,3,channels);
            *(pdst+row*width+col) = *(pTmpEqu+4);
        }
    }

    int tmp = 0;
}

void Algo_ImageProcess::Histogram_statistics(const unsigned char *psrc, unsigned char *pdst, int width, int height, double c, double k0, double k1, double k2, double k3)
{
    // 直方图统计量增强图像
    memcpy(pdst, psrc, sizeof(psrc[0])*width*height);

    // 计算整张图像的均值和标准差
    double meanG, sigmaG;  // 整体均值和标准差
    GetMeanStd(psrc, width, height, meanG, sigmaG);

    double meanS, sigmaS;  // 局部均值和标准差
    bool bCandidate = false;
    for(int row=1; row < height-1; ++row){
        for(int col=1; col<width-1; ++col){

            // 计算局部均值及标准差
            const unsigned char atmp[] = {
                *(psrc+row*width+col - width - 1),  *(psrc+row*width+col - width),  *(psrc+row*width+col - width + 1),
                *(psrc+row*width+col - 1),          *(psrc+row*width+col),          *(psrc+row*width+col + 1),
                *(psrc+row*width+col + width - 1),  *(psrc+row*width+col+ width),   *(psrc+row*width+col+ width + 1)
            };

            GetMeanStd(atmp, 3, 3, meanS, sigmaS);

            // 判断是否进行处理
            bCandidate = ((k0 * meanG <= meanS) && (meanS <= k1 * meanG));
            bCandidate &= ((k2 * sigmaG <= sigmaS) && (sigmaS <= k3 * sigmaG));
            if(bCandidate){

                *(pdst+ row*width+col) = ((*(psrc + row*width+col) * c) > 255) ? 255 : (*(psrc + row*width+col) * c);
            }

        }
    }
}

#define WINDOW_NAME "fliter"

/// @brief 中值滤波
/// @param pos 滑动条位置
/// @param arg 传入参数
void on_meanFliter(int pos, void *arg)
{
    cv::Mat src = *(cv::Mat *)arg;
    cv::Size kernel(2 * pos + 1, 2 * pos + 1);
    cv::Mat dst;
    cv::blur(src, dst, kernel);
    cv::imshow(WINDOW_NAME, dst);
}

void on_medianFliter(int pos, void *arg)
{
    cv::Mat src = *(cv::Mat *)arg;
    int ksize = 2 * pos + 1;
    cv::Mat dst;
    cv::medianBlur(src, dst, ksize);
    cv::imshow(WINDOW_NAME, dst);
}

void on_gaussianFliter(int pos, void *arg)
{
    cv::Mat src = *(cv::Mat *)arg;
    cv::Size kernel(2 * pos + 1, 2 * pos + 1);
    cv::Mat dst;
    cv::GaussianBlur(src, dst, kernel, 0, 0); // sigmaX=sigmaY=0表示使用默认的高斯分布公式：sigma = (n_x-1)/2*0.30+0.80，n_x = kernel.width-1
    cv::imshow(WINDOW_NAME, dst);
}

void on_boxFliter(int pos, void *arg)
{
    cv::Mat src = *(cv::Mat *)arg;
    cv::Size kernel(2 * pos + 1, 2 * pos + 1);
    cv::Mat dst;
    cv::boxFilter(src, dst, -1, kernel, cv::Point(-1, -1), true);
    cv::imshow(WINDOW_NAME, dst);
}

void on_bilateralFliter(int pos, void *arg)
{
    cv::Mat src = *(cv::Mat *)arg;
    cv::Size kernel(2 * pos + 1, 2 * pos + 1);
    cv::Mat dst;
    int d = pos;
    double sigmaColor = d * 2;
    double sigmaSpace = d / 2.0;
    cv::bilateralFilter(src, dst, d, sigmaColor, sigmaSpace);
    cv::imshow(WINDOW_NAME, dst);
}

void Algo_ImageProcess::SpatialFilter(const char *filename)
{
    cv::Mat mSrc = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if(mSrc.empty()){
        return;
    }

    int maxKernelSize = 31;

    // 创建窗口
    cv::namedWindow(WINDOW_NAME);
    // 创建滑动条，通过滑动条改变核的大小，改变滤波
    int g_meanKernel = 0; // 均值滤波核
    int g_max_meanKernel = maxKernelSize;
    cv::createTrackbar("mean_kernel", WINDOW_NAME, &g_meanKernel, g_max_meanKernel, on_meanFliter, (void *)&mSrc);

    int g_medianKernel = 0; // 中值滤波核
    int g_max_medianKernel = maxKernelSize;
    cv::createTrackbar("median_kernel", WINDOW_NAME, &g_medianKernel, g_max_medianKernel, on_medianFliter, (void *)&mSrc);

    int g_boxKernel = 0; // 盒式滤波核
    int g_max_boxKernel = maxKernelSize;
    cv::createTrackbar("box_kernel", WINDOW_NAME, &g_boxKernel, g_max_boxKernel, on_boxFliter, (void *)&mSrc);

    int g_gaussianKernel = 0; // 高斯滤波核
    int g_max_gaussianKernel = maxKernelSize*5;
    cv::createTrackbar("gaussian_kernel", WINDOW_NAME, &g_gaussianKernel, g_max_gaussianKernel, on_gaussianFliter, (void *)&mSrc);

    int g_bilateralKernel = 0; // 双边滤波核
    int g_max_bilateralKernel = maxKernelSize*5;
    cv::createTrackbar("bilateral_kernel", WINDOW_NAME, &g_bilateralKernel, g_max_bilateralKernel, on_bilateralFliter, (void *)&mSrc);

    cv::waitKey(0);
    cv::destroyAllWindows();


}

void Algo_ImageProcess::SpatialFilter_Mean(const unsigned char *psrc, unsigned char *pdst, int width, int height, int kSize, int borderType, int borderValue)
{
    cv::Mat src(height, width, CV_8UC1);
    src.data = (unsigned char*)psrc;

    // 平滑处理
    cv::Mat mDst;
    cv::blur(src, mDst,cv::Size(kSize, kSize),cv::Point(-1, -1), borderType);
    cv::namedWindow("Mean Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Mean Filter", mDst);
    cv::imwrite("MeanFilter.tif", mDst);

}

void Algo_ImageProcess::SpatialFilter_Mean(const char *filename, int kSize, int borderType, int borderValue)
{
    cv::Mat mSrc = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if(mSrc.empty()){
        printf("read image fail!\n");
        return;
    }

    cv::Mat mDst;
    cv::blur(mSrc,mDst,cv::Size(kSize, kSize), cv::Point(-1, -1), borderType);
    cv::namedWindow("Mean Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Mean Filter", mDst);
    cv::imwrite("MeanFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Mean(const cv::InputArray &src, int kSize, int borderType, int borderValue)
{
    cv::Mat mDst;
    cv::blur(src,mDst,cv::Size(kSize, kSize), cv::Point(-1, -1), borderType);
    cv::namedWindow("Mean Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Mean Filter", mDst);
    cv::imwrite("MeanFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Box(const unsigned char *psrc, unsigned char *pdst, int width, int height, int kSize, bool normalize, int borderType, int borderValue)
{
    cv::Mat src(height, width, CV_8UC1);
    src.data = (unsigned char*)psrc;

    // 平滑处理
    cv::Mat mDst;
    cv::boxFilter(src, mDst, -1, cv::Size(kSize, kSize),cv::Point(-1, -1), normalize, borderType);
    cv::namedWindow("Box Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Box Filter", mDst);
    cv::imwrite("BoxFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Box(const char *filename, int kSize, bool normalize, int borderType, int borderValue)
{
    cv::Mat mSrc = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    if(mSrc.empty()){
        printf("read image fail!\n");
        return;
    }

    // 平滑处理
    cv::Mat mDst;
    cv::boxFilter(mSrc, mDst, -1, cv::Size(kSize, kSize),cv::Point(-1, -1), normalize, borderType);
    cv::namedWindow("Box Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Box Filter", mDst);
    cv::imwrite("BoxFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Box(const cv::InputArray &src, int kSize, bool normalize, int borderType, int borderValue)
{
    // 平滑处理
    cv::Mat mDst;
    cv::boxFilter(src, mDst, -1, cv::Size(kSize, kSize),cv::Point(-1, -1), normalize, borderType);
    cv::namedWindow("Box Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Box Filter", mDst);
    cv::imwrite("BoxFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Median(const cv::InputArray &src, int kSize)
{
    // 平滑处理
    cv::Mat mDst;
    cv::medianBlur(src, mDst, kSize);
    cv::namedWindow("Median Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Median Filter", mDst);
    cv::imwrite("MedianFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Gaussian(const cv::InputArray &src, int kSize, double sigmaX, double sigmaY, int borderType)
{
    // 平滑处理
    cv::Mat mDst;
    cv::GaussianBlur(src, mDst, cv::Size(kSize, kSize), sigmaX, sigmaY, borderType);

    // 阈值处理
    //    cv::Mat mThre;
    //    cv::threshold(mDst, mThre, 0.4*255, 255, cv::THRESH_BINARY);
    cv::namedWindow("Gaussian Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Gaussian Filter", mDst);
    cv::imwrite("GaussianFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_Bilateral(const cv::InputArray &src, int kSize, double sigmaColor, double sigmaSpace, int borderType)
{
    // 平滑处理
    cv::Mat mDst;
    cv::bilateralFilter(src, mDst, kSize, sigmaColor, sigmaSpace, borderType);

    // 阈值处理
    //    cv::Mat mThre;
    //    cv::threshold(mDst, mThre, 0.4*255, 255, cv::THRESH_BINARY);
    cv::namedWindow("Bilateral Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Bilateral Filter", mDst);
    cv::imwrite("BilateralFilter.tif", mDst);
}

void Algo_ImageProcess::SpatialFilter_laplace(const cv::InputArray &src, double c, int borderType)
{
    // 使用对角线参与计算的拉普拉斯核对图像进行卷积操作

    // 声明拉普拉斯核
    cv::Mat kernel = (cv::Mat_<float>(3,3) <<
                      -1, -1, -1,
                      -1, 8, -1,
                      -1, -1, -1);
    // 使用自定义核计算卷积
    cv::Mat mDst;
    cv::filter2D(src, mDst, -1, kernel);

    cv::Mat dst;
    cv::add(src, mDst, dst);

    cv::namedWindow("Laplacian8 Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Laplacian8 Filter", dst);
    cv::imwrite("Laplacian8Filter.tif", dst);

    cv::Mat kernel1 = (cv::Mat_<float>(3,3) <<
                       0, -1, 0,
                       -1, 4, -1,
                       0, -1, 0);
    // 使用自定义核计算卷积
    cv::Mat mDst1;
    cv::filter2D(src, mDst1, -1, kernel1);

    cv::Mat dst1;
    cv::add(src, mDst1, dst1);


    cv::namedWindow("Laplacian Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Laplacian Filter", dst1);
    cv::imwrite("LaplacianFilter.tif", dst1);

}

void Algo_ImageProcess::SpatialFilter_laplace(const unsigned char *psrc, unsigned char *pdst, int width, int height, int borderType, int borderValue)
{
    // 扩展边界
    cv::Mat src(height, width, CV_8UC1);
    src.data = (unsigned char*)psrc;

    cv::Mat mBorder;
    cv::copyMakeBorder(src, mBorder, 1,1,1,1, borderType);

    // 计算
    int newWidth = mBorder.cols;
    int newHeight = mBorder.rows;

    cv::Mat mDelta = cv::Mat::zeros(height, width, CV_16SC1);

    /*
     * -1   -1  -1
     * -1   8   -1
     * -1   -1  -1
    */

    for(int row=0; row<height-1; ++row){
        unsigned short *pRow = mDelta.ptr<unsigned short>(row);
        for(int col=0; col<width; ++col){

            /*
             * -1   -1  -1
             * -1   8   -1
             * -1   -1  -1
            */
            pRow[col] += (-8 * mBorder.at<uchar>(row+1, col+1)) +
                    mBorder.at<uchar>(row, col) + mBorder.at<uchar>(row, col+1) + mBorder.at<uchar>(row, col+2) +
                    mBorder.at<uchar>(row+1, col) +mBorder.at<uchar>(row+1, col+2) +
                    mBorder.at<uchar>(row+2, col)+mBorder.at<uchar>(row+2, col+1) +mBorder.at<uchar>(row+2, col+2);


            //            /*
            //             * 0   -1  0
            //             * -1   4   -1
            //             * 0   -1  0
            //            */
            //            pRow[col] += (-4 * mBorder.at<uchar>(row+1, col+1)) +
            //                    mBorder.at<uchar>(row, col+1) +
            //                    mBorder.at<uchar>(row+1, col) +mBorder.at<uchar>(row+1, col+2) +
            //                    mBorder.at<uchar>(row+2, col+1);


        }
    }

    // 归一化到255

    short *pdata = (short*)mDelta.data;
    //    for(int row=0; row<height; ++row){
    //        for(int col=0; col<width;++col){
    //            pdata[row*width + col] = mDelta.at<ushort>(row+1, col+1);
    //        }
    //    }
    cv::Mat dst = cv::Mat::zeros(height, width, CV_8UC1);
    GrayscaleConversion_16To8(pdata, dst.data, width, height, 1);
    //    delete[] pdata;
    cv::namedWindow("Laplacian Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Laplacian Filter", dst);
    cv::imwrite("LaplacianFilter.tif", dst);

}

void Algo_ImageProcess::SpatialFilter_HighLift(const cv::InputArray &src, double k, int kSize, double sigmaX, double sigmaY, int borderType)
{
    // 模糊图像
    cv::Mat mBlur;
    cv::GaussianBlur(src, mBlur, cv::Size(kSize, kSize), sigmaX, sigmaY, borderType);

    // 原图像减去模糊图像 与原图像
    float *pdata = new float[mBlur.cols * mBlur.rows];
    cv::Mat mSrc = src.getMat();
    uchar *pSrc = mSrc.data;
    uchar *pBlur = mBlur.data;
    for(int i=0; i<mBlur.cols * mBlur.rows; ++i){
        pdata[i] = (pSrc[i] - pBlur[i]) * k + pSrc[i];
    }

    cv::Mat dst = cv::Mat::zeros(mBlur.rows, mBlur.cols, CV_8UC1);
    uchar *pdst = dst.data;

    GrayscaleConversion_double2uchar(pdata, pdst, dst.cols, dst.rows, 1);

    //    cv::Mat mMask = cv::Mat::zeros(mBlur.rows, mBlur.cols, CV_16SC1);
    //    cv::subtract(src, mBlur, mMask, cv::noArray(), CV_16S);
    //    cv::Mat mShowMask = cv::Mat::zeros(mBlur.rows, mBlur.cols, mBlur.type());
    //    GrayscaleConversion_16To8((ushort*)mMask.data, mShowMask.data, mShowMask.cols, mShowMask.rows,1);
    //    cv::imshow("mask", mShowMask);
    //    cv::imwrite("HighLiftMaskFilter.tif", mShowMask);

    //    // 模板与原图像相加
    //    cv::Mat msrc = src.getMat();
    //    cv::Mat m16Src;
    //    msrc.convertTo(m16Src, CV_16SC1);
    //    cv::Mat mDst;
    //    cv::addWeighted(m16Src, 1, mMask, k, 0, mDst, -1);

    //    cv::Mat dst = cv::Mat::zeros(mDst.rows, mDst.cols, CV_8UC1);
    //    GrayscaleConversion_16To8((ushort*)mDst.data, dst.data, dst.cols, dst.rows, 1);

    cv::namedWindow("HighLift Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("HighLift Filter", dst);
    cv::imwrite("HighLiftFilter.tif", dst);



}

void Algo_ImageProcess::SpatialFilter_Sobel(const cv::InputArray &src, int kSize, double scale, double delta, int borderType)
{
    cv::Mat mSrc = src.getMat();

    //    cv::Mat mDst = cv::Mat::zeros(mSrc.rows, mSrc.cols, CV_32FC1);
    //    cv::Sobel(mSrc, mDst, CV_32F, 1, 0, kSize, scale, delta, borderType);

    //    float *pdst = (float*)mDst.data;
    //    cv::Mat dst = cv::Mat::zeros(mDst.rows, mDst.cols, CV_8UC1);
    //    GrayscaleConversion_double2uchar(pdst, dst.data, dst.cols, dst.rows, 1);


    // 计算 g_x g_y
    cv::Mat sobelX, sobelY;
    //    cv::Mat kernelX = (cv::Mat_<short>(3,3) <<
    //                       -1, -2,-1,
    //                       0, 0, 0,
    //                       1, 2, 1);
    //    cv::Mat kernelY = (cv::Mat_<short>(3,3) <<
    //                       -1, 0, 1,
    //                       -2, 0, 2,
    //                       -1, 0, 1);
    //    cv::filter2D(mSrc,sobelX, CV_32F, kernelX);
    //    cv::filter2D(mSrc,sobelY, CV_32F, kernelY);

    cv::Sobel(mSrc, sobelX, CV_32F, 1, 0,kSize, scale, delta, borderType);
    cv::Sobel(mSrc, sobelY, CV_32F, 0, 1,kSize, scale, delta, borderType);

    {
        // 显示与 sobel算子卷积后的结果
        cv::Mat mx = cv::Mat::zeros(sobelX.rows, sobelX.cols, CV_8UC1);
        cv::Mat my = cv::Mat::zeros(sobelY.rows, sobelY.cols, CV_8UC1);
        float *px = (float*)sobelX.data;
        float *py = (float*)sobelY.data;

        GrayscaleConversion_double2uchar(px, mx.data, mx.cols, mx.rows, 1);
        GrayscaleConversion_double2uchar(py, my.data, my.cols, my.rows, 1);

        cv::imshow("sobelX", mx);
        cv::imshow("sobelY", my);
    }


    // 计算 M(x,y)
    cv::Mat mDst = cv::Mat::zeros(mSrc.rows, mSrc.cols, CV_32FC1);

    for(int row = 0; row<mDst.rows; ++row){
        float *pdst = mDst.ptr<float>(row);
        float *px = sobelX.ptr<float>(row);
        float *py = sobelY.ptr<float>(row);
        for(int col = 0; col<mDst.cols; ++col){
            pdst[col] = sqrt(pow(px[col],2) + pow(py[col],2));
        }
    }

    float *pdst = (float*)mDst.data;
    cv::Mat dst = cv::Mat::zeros(mDst.rows, mDst.cols, CV_8UC1);
    GrayscaleConversion_double2uchar(pdst, dst.data, dst.cols, dst.rows,1);

    cv::namedWindow("Sobel Filter", cv::WINDOW_AUTOSIZE);
    cv::imshow("Sobel Filter", dst);
    cv::imwrite("SobelFilter.tif", dst);
}

void Algo_ImageProcess::GetMeanStd(const unsigned char *psrc, int width, int height, double &mean, double &std)
{
    mean = std = 0;
    long long sum = 0;
    for(int i=0; i<width*height; ++i){
        sum += *(psrc+i);
    }

    mean = sum*1.0 / (width*height);

    for(int i=0; i<width*height; ++i){
        std += std::pow(std::abs(mean - *(psrc+i)), 2);
    }

    std = std::sqrt(std/(width*height));
}
