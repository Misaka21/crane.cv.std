//
// Created by misaka21 on 24-7-9.
//

#ifndef CRANE_CV_STD_HANDEYE_H
#define CRANE_CV_STD_HANDEYE_H

#include <opencv2/core/mat.hpp>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
class handeye {
public:
	cv::Point2d detect_weight(cv::Mat &inputImage);
	void Display(const cv::Mat& inputImage);
	void settf(int xerr,int yerr){
		this->_xerr=xerr;
		this->_yerr=yerr;
	}
private:
	std::vector<std::vector<cv::Point>> _contours; // 存储二值化图像的轮廓
	cv::Point2f _center;    // 检测到的圆心
	float _radius;          // 检测到的圆的半径
	int _xerr,_yerr=-100;
};


#endif //CRANE_CV_STD_HANDEYE_H
