#ifndef ALGO_IMAGEPROCESS_H
#define ALGO_IMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

enum RawBayer{
    RawBayer_RGGB = 0,
    RawBayer_GRBG = 1,
    RawBayer_GBRG = 2,
    RawBayer_BGGR = 3,
    RawBayer_Null,
};

enum SpatialFilterType{
    FilterType_Mean = 0,
    FilterType_Box = 1,
    FilterType_Median = 2,
    FilterType_Gaussian = 3,
    FilterType_Bilateral = 4,
    FilterType_Laplace = 5,
    FilterType_8Laplace = 6,
    FilterType_HighLift = 7,  // 钝化掩蔽 | 高提升滤波
    FilterType_Sobel = 8,
};


class Algo_ImageProcess
{
public:
    struct ImageInfo
    {
        int width;
        int height;
        int rawBits;
        RawBayer rawBayer;
        unsigned short *rawBuf;
        unsigned char *rgb24Buf;
        unsigned char *yBuf;
        ImageInfo() {
            width = 0;
            height = 0;
            rawBits = 0;
            rawBayer = RawBayer_Null;
            rawBuf = nullptr;
            rgb24Buf = nullptr;
            yBuf = nullptr;
        }
    };

public:
    Algo_ImageProcess();


    void RGB2HSI(const unsigned char *psrc, unsigned char *pdst, int width, int height);

    void RGB2Gray(const unsigned char *psrc, unsigned char *pdst, int width, int height);

    void GrayscaleConversion_u16To8(const unsigned short *psrc, unsigned char *pdst, int width, int height, int channels);

    void GrayscaleConversion_16To8(const short *psrc, unsigned char *pdst, int width, int height, int channels);

    void GrayscaleConversion_double2uchar(const float *psrc, unsigned char *pdst, int width, int height, int channels);

    void GrayscaleConversion_Reverse(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels);

    void GrayscaleConversion_Log(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels, double c);

    void GrayscaleConversion_Power(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels, double c, double gamma);

    void GrayscaleConversion_BitLayering(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels);

    void Histogram_normalize(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels);

    void Histogram_equalization(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels);

    void Histogram_matching(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels);

    void Histogram_local(const unsigned char *psrc, unsigned char *pdst, int width, int height, int channels);

    void Histogram_statistics(const unsigned char *psrc, unsigned char *pdst, int width, int height, double c, double k0, double k1, double k2, double k3);

    void SpatialFilter(const char *filename);

    void SpatialFilter_Mean(const unsigned char *psrc, unsigned char *pdst, int width, int height, int kSize, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_Mean(const char *filename, int kSize, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_Mean(const cv::InputArray &src, int kSize, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_Box(const unsigned char *psrc, unsigned char *pdst, int width, int height, int kSize, bool normalize = true, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_Box(const char *filename, int kSize, bool normalize = true, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_Box(const cv::InputArray &src, int kSize, bool normalize = true, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_Median(const cv::InputArray &src, int kSize);

    void SpatialFilter_Gaussian(const cv::InputArray &src, int kSize, double sigmaX, double sigmaY = 0, int borderType = cv::BORDER_DEFAULT);

    void SpatialFilter_Bilateral(const cv::InputArray &src, int kSize, double sigmaColor, double sigmaSpace = 0, int borderType = cv::BORDER_DEFAULT);

    void SpatialFilter_laplace(const cv::InputArray &src, double c = 1, int borderType = cv::BORDER_DEFAULT);

    void SpatialFilter_laplace(const unsigned char *psrc, unsigned char *pdst, int width, int height, int borderType = cv::BORDER_DEFAULT, int borderValue = 0);

    void SpatialFilter_HighLift(const cv::InputArray &src, double k, int kSize, double sigmaX, double sigmaY = 0, int borderType = cv::BORDER_DEFAULT);

    void SpatialFilter_Sobel(const cv::InputArray &src, int kSize, double scale = 1, double delta = 0, int borderType = cv::BORDER_DEFAULT);

private:
    void GetMeanStd(const unsigned char *psrc, int width, int height, double &mean, double &std);
};

#endif // ALGO_IMAGEPROCESS_H
