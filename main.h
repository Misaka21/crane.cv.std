//
// Created by misaka21 on 24-6-4.
//

#ifndef CRANE_CV_STD_MAIN_H
#define CRANE_CV_STD_MAIN_H

#include "detector.h"



#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath> // For atan2 and M_PI

std::vector<float> calculateAngles(const std::vector<cv::Point2f>& points, const cv::Rect& rect) {
    // 计算矩形的中心点
    cv::Point2f center(rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f);

    std::vector<float> angles;
    for (const auto& point : points) {
        // 计算向量
        float dx = point.x - center.x;
        float dy = point.y - center.y;

        // 计算角度
        float angle = atan2(dy, dx) * 180.0f / M_PI;  // 转换为度
        angles.push_back(angle);
    }

    return angles;
}

std::vector<int> matchAngles(const std::vector<float>& measuredAngles, float tolerance) {
    // 预设的角度列表
    std::vector<float> presetAngles = {-30, -90, -150, 150, 90, 30};

    // 结果数组，初始化为0，大小与预设角度列表一致
    std::vector<int> results(presetAngles.size(), 0);

    // 遍历每个测量角度
    for (const auto& angle : measuredAngles) {
        // 检查每个预设角度
        for (size_t i = 0; i < presetAngles.size(); ++i) {
            if (fabs(angle - presetAngles[i]) < tolerance ||
             fabs(angle - presetAngles[i] + 360) < tolerance ||
             fabs(angle - presetAngles[i] - 360) < tolerance) {
                // 如果角度在容忍范围内，置1
                results[i] = 1;
            }
        }
    }

    return results;
}


#endif //CRANE_CV_STD_MAIN_H
