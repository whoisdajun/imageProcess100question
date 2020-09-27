//
// Created by lidajun on 2020/9/27.
//
#include <opencv2/opencv.hpp>

using namespace cv;

// RGB -> BGR 对于RGB与BGR通道的相互转换，尤其是
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
int main()
{
    cv::Mat src;
    src = imread("../imori.jpg", cv::IMREAD_COLOR);
    // channel swap
    cv::imshow("src", src);
    cv::waitKey(0);
    cv::Mat out = channel_swap(src);

    cv::imwrite("out.jpg", out);
    cv::imshow("sample", out);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}