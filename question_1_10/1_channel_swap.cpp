//
// Created by lidajun on 2020/9/27.
//
#include <opencv2/opencv.hpp>
#include "arm_neon.h"
using namespace cv;

// RGB -> BGR hwc结构的channel_swap，RGB->BGR与BGR->RGB操作一样
cv::Mat channel_swap(cv::Mat img){
    int height = img.rows;
    int width = img.cols;
    cv::Mat dst = cv::Mat::zeros(img.size(), CV_8UC3);
//  opencv的Mat中存储数据的存储方式：BGRBGR交替
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
//          R -> B
            dst.at<Vec3b>(x, y)[0] = img.at<Vec3b>(x, y)[2];
//          G -> G
            dst.at<Vec3b>(x, y)[1] = img.at<Vec3b>(x, y)[1];
//          B -> R
            dst.at<Vec3b>(x, y)[2] = img.at<Vec3b>(x, y)[0];
        }
    }
    return dst;
}

// 图像通道转换方法二，指针交换数据
void channel_swap2(uchar* data, int size){
//    uchar* ptr = img.data;
//    int img_size = img.cols * img.rows * img.channels();
    uchar* ptr = data;
    for (int i = 0; i < size; i+=3) {
        uchar tmp = *ptr;
        *ptr = *(ptr + 2);
        *(ptr + 2) = tmp;
        ptr += 3;
    }
}

// 利用ARM向量寄存器加速
#if USE_NEON
#define CHANNELS 3
void rgb2bgr_with_neon(uint8_t *img, int height, int width)
{
    const int total_bytes = height * width * CHANNELS;
    const int stride_bytes = 48;
    const int left_bytes = total_bytes % stride_bytes;
    const int multiply_bytes = total_bytes - left_bytes;

    for (int i = 0; i < multiply_bytes; i += stride_bytes)
    {
        uint8_t *target = img + i;

        // swap R and B channel with NEON
        uint8x16x3_t a = vld3q_u8(target);
        uint8x16x3_t b;
        b.val[0] = a.val[2];
        b.val[1] = a.val[1];
        b.val[2] = a.val[0];
        vst3q_u8(target, b);
    }

    // handling non-multiply array lengths
    for (int i = multiply_bytes; i < total_bytes; i += CHANNELS)
    {
        const auto r_channel = *(img + i);
        *(img + i) = *(img + i + 2);
        *(img + i + 2) = r_channel;
    }
}
#endif

// TODO 利用AVX或者SSE优化图像通道转换

int main()
{
    cv::Mat src;
    src = imread("../imori.jpg", cv::IMREAD_COLOR);
    // channel swap
    cv::imshow("src", src);
    cv::waitKey(0);
    cv::Mat out = channel_swap(src);
    channel_swap2(src.data, src.cols * src.rows * src.channels());

    cv::imwrite("out.jpg", out);
    cv::imshow("sample", out);
    cv::imshow("ptr result", src);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}