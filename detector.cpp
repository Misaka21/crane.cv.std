//
// Created by misaka21 on 24-6-3.
//

#include "detector.h"

auto detector::wood_detect(cv::Mat &img) -> std::vector<cv::Rect>
{
    cv::Mat frame = img.clone(), frame_HSV, frame_threshold, kernel;
    
    cv::cvtColor(img, frame_HSV, cv::COLOR_BGR2HSV);
    cv::inRange(frame_HSV, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), frame_threshold);
    
    // 创建膨胀和腐蚀操作的核（结构元素）
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    // 腐蚀操作
    cv::erode(frame_threshold, frame_threshold, kernel);
    //cv::imshow("Eroded Image", dst);

    // 膨胀操作
    cv::dilate(frame_threshold, frame_threshold, kernel);
    //cv::imshow("Dilated Image", dst);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(frame_threshold, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Rect> detectedRectangles; // 存储检测到的矩形

    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > minareaThreshold && area > maxareaThreshold)
        {
            // 近似轮廓，减少点数量
            std::vector<cv::Point> approx;
            cv::approxPolyDP(contours[i], approx, 0.02 * cv::arcLength(contours[i], true), true);
            int vertexCount = approx.size();

            if (vertexCount <= vertexThreshold)
            {
                cv::Rect rect = cv::boundingRect(approx);
                cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
                cv::drawContours(frame, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(255, 0, 0), 2);

                // 计算矩形中心坐标
                cv::Point rectCenter(rect.x + rect.width / 2, rect.y + rect.height / 2);

                // 构建显示的文本信息：矩形中心坐标
                std::string label1 = "Center: (" + std::to_string(rectCenter.x) + ", " + std::to_string(rectCenter.y) + ")";
                cv::putText(frame, label1, cv::Point(rectCenter.x, rectCenter.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);

                // 构建显示的文本信息：轮廓面积和边数
                std::string label2 = "Area: " + std::to_string((int)area) + ", Vertices: " + std::to_string(vertexCount);
                cv::putText(frame, label2, cv::Point(rectCenter.x, rectCenter.y + 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
                detectedRectangles.push_back(rect);
            }
        }
    }

    cv::imshow("Original", frame);
    cv::imshow("Threshold", frame_threshold);
    return detectedRectangles; // 返回检测到的矩形集合
}
// 定义一个用于识别和标注圆形的函数
auto detector::detect_weights(cv::Mat &inputImage)->std::vector<cv::Point2f>
{
    // 转换为灰度图
    cv::Mat gray,frame=inputImage.clone();
    cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);

    // 二值化，目标为黑色区域
    cv::Mat binary;
    cv::threshold(gray, binary, 50, 255, cv::THRESH_BINARY_INV);
    //cv::imshow("Binary Image", binary);

    // 形态学操作：先膨胀后腐蚀
    cv::Mat morphed;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::dilate(binary, morphed, kernel);
    cv::erode(morphed, morphed, kernel);

    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morphed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point2f> detectedCenters; // 存储检测到的圆心

    // 筛选轮廓并识别圆形
    for (size_t i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > 1000)
        { // 筛选大小
            // 拟合圆
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(contours[i], center, radius);

            // 计算轮廓和圆的近似度
            double perimeter = cv::arcLength(contours[i], true);
            double circularity = 4 * M_PI * area / (perimeter * perimeter);

            if (circularity > 0.8)
            { // 筛选圆形
                // 绘制圆心
                cv::circle(frame, center, 2, cv::Scalar(0, 0, 255), -1);
                // 绘制圆边界
                cv::circle(frame, center, radius, cv::Scalar(0, 255, 0), 2);

                // 标注信息：圆心坐标和面积
                std::string centerText = "Center: (" + std::to_string((int)center.x) + ", " + std::to_string((int)center.y) + ")";
                std::string areaText = "Area: " + std::to_string((int)area);

                cv::putText(frame, centerText, cv::Point(center.x + 10, center.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
                cv::putText(frame, areaText, cv::Point(center.x + 10, center.y + 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
                detectedCenters.push_back(center); // 添加圆心到向量中
            }
        }
    }

    // 显示结果
    cv::imshow("Detected-6", frame);
    return detectedCenters; // 返回检测到的圆心点集
}