//
// Created by misaka21 on 24-6-3.
//

#ifndef CRANE_CV_STD_DETECTOR_H
#define CRANE_CV_STD_DETECTOR_H
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
class detector
{
private:
    /* data */
    // 全局变量存储HSV阈值
    int low_H = 0, low_S = 0, low_V = 0;
    int high_H = 180, high_S = 255, high_V = 255;
    int minareaThreshold = 500; // 面积阈值，用于筛选轮廓
    int maxareaThreshold = 1000;
    int vertexThreshold = 6;  // 顶点数量阈值

    void on_trackbar(int, void *)
    {
        // 该函数将在滑块调整时被调用，但这里不需要执行任何操作
    }


public:
    //detector(/* args */);
    auto wood_detect(cv::Mat &img) -> std::vector<cv::Rect>;
    auto detect_weights(cv::Mat &inputImage) -> std::vector<cv::Point2f>;
    //~detector();
    void createTrackbars()
    {
        // 创建窗口
        cv::namedWindow("Trackbars", cv::WINDOW_AUTOSIZE);

        // 创建滑块
        cv::createTrackbar(
            "Low H", "Trackbars", &low_H, 180, [](int value, void *userdata)
            { static_cast<detector *>(userdata)->on_trackbar(value, nullptr); },
            this);

        cv::createTrackbar(
            "High H", "Trackbars", &high_H, 180, [](int value, void *userdata)
            { static_cast<detector *>(userdata)->on_trackbar(value, nullptr); },
            this);

        cv::createTrackbar(
            "Low S", "Trackbars", &low_S, 255, [](int value, void *userdata)
            { static_cast<detector *>(userdata)->on_trackbar(value, nullptr); },
            this);

        cv::createTrackbar(
            "High S", "Trackbars", &high_S, 255, [](int value, void *userdata)
            { static_cast<detector *>(userdata)->on_trackbar(value, nullptr); },
            this);

        cv::createTrackbar(
            "Low V", "Trackbars", &low_V, 255, [](int value, void *userdata)
            { static_cast<detector *>(userdata)->on_trackbar(value, nullptr); },
            this);

        cv::createTrackbar(
            "High V", "Trackbars", &high_V, 255, [](int value, void *userdata)
            { static_cast<detector *>(userdata)->on_trackbar(value, nullptr); },
            this);
    }
};

// detector::detector(/* args */)
// {
// }

// detector::~detector()
// {
// }


#endif //CRANE_CV_STD_DETECTOR_H
