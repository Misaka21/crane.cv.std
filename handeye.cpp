//
// Created by misaka21 on 24-7-9.
//


#include <iostream>
#include "handeye.h"

cv::Point2d handeye::detect_weight(cv::Mat &inputImage) {
	this-> _center= {0,0};
	this-> _radius=0;
	// 转换为灰度图
	cv::Mat gray;
	cv::cvtColor (inputImage, gray, cv::COLOR_BGR2GRAY);

	// 使用大津法二值化
	cv::Mat binary;
	cv::threshold (gray, binary, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

	// 形态学操作：先膨胀后腐蚀
	cv::Mat morphed;
	cv::Mat kernel = cv::getStructuringElement (cv::MORPH_ELLIPSE, cv::Size (5, 5));
	cv::dilate (binary, morphed, kernel);
	cv::erode (morphed, morphed, kernel);

	// 查找轮廓
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours (morphed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	this->_contours=contours;
	// 筛选轮廓并识别圆形
	for (const auto &contour: contours) {
		double area = cv::contourArea (contour);
		if (area > 1000) { // 筛选大小
			cv::Point2f center;
			float radius;
			cv::minEnclosingCircle (contour, center, radius);

			double perimeter = cv::arcLength (contour, true);
			double circularity = 4 * M_PI * area / (perimeter * perimeter);

			if (circularity > 0.8) { // 筛选圆形
				// 标注圆心和边界
				//cv::circle (inputImage, center, 2, cv::Scalar (0, 0, 255), -1);
				//cv::circle (inputImage, center, radius, cv::Scalar (0, 255, 0), 2);

				// 显示结果
				//cv::imshow ("Detected Circle", inputImage);
				//cv::waitKey (1);

				this-> _center=center;
				this-> _radius=radius; // 返回圆心点
				std::cout<<center.y+this->_yerr<<std::endl;
				return {center.x+this->_xerr,
						center.y+this->_yerr};
			}
		}
	}
	return {-1,-1};
}
void handeye::Display(const cv::Mat& inputImage) {
	// 创建一个用于显示结果的图像副本
	cv::Mat displayImage = inputImage.clone();

	// 绘制所有轮廓
	for (const auto& contour : _contours) {
		cv::drawContours(displayImage, _contours, -1, cv::Scalar(255, 0, 0), 2);
	}

	// 如果半径大于零，则绘制圆和圆心
	if (_radius > 0) {
		cv::circle(displayImage, _center, static_cast<int>(_radius), cv::Scalar(0, 255, 0), 2);  // 圆
		cv::circle(displayImage, _center, 2, cv::Scalar(0, 0, 255), -1);  // 圆心
		// 绘制水平线
		cv::line(displayImage, cv::Point(_center.x - 2000, _center.y+_yerr),
				 cv::Point(_center.x + 2000, _center.y+_yerr), cv::Scalar(255,0,255), 2);
		// 绘制垂直线
		cv::line(displayImage, cv::Point(_center.x+_xerr, _center.y - 2000),
				 cv::Point(_center.x+_xerr, _center.y + 2000), cv::Scalar(255,0,255), 2);

	}

	// 显示圆心坐标和半径
	std::string centerText = "Center: (" + std::to_string(int(_center.x)) + ", " + std::to_string(int(_center.y)) + ")";
	std::string radiusText = "Radius: " + std::to_string(int(_radius));
	cv::putText(displayImage, centerText, cv::Point(_center.x + 10, _center.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
	cv::putText(displayImage, radiusText, cv::Point(_center.x + 10, _center.y + 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);

	// 显示结果
	cv::imshow("Detected Circle", displayImage);
	cv::waitKey(1); // 按任意键继续
}