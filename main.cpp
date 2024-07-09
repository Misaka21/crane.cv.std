#include <opencv2/opencv.hpp>
#include <iostream>
#include "main.h"

int main()
{
	// 创建一个VideoCapture对象
	cv::VideoCapture cap(0); // 0代表默认摄像头

	// 检查摄像头是否成功打开
	if (!cap.isOpened())
	{
		std::cerr << "Error: Unable to open the camera" << std::endl;
		return -1;
	}

	cv::Mat frame;
	cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

	detector detector;
	handeye lefthand;
	// 持续读取帧
	while (true)
	{
		cap >> frame; // 读取新的帧

		// 检查帧是否为空
		if (frame.empty())
		{
			std::cerr << "Error: Blank frame grabbed" << std::endl;
			break;
		}
		std::vector<cv::Rect> woodRects;
		std::vector<cv::Point2f> weights;
		detector.createTrackbars();
		woodRects=detector.wood_detect(frame);
		weights=detector.detect_weights(frame);

		lefthand.detect_weight (frame);
		lefthand.Display (frame);
		// 显示帧
		cv::imshow("Webcam", frame);

		// 按 'ESC' 键退出循环
		if (cv::waitKey(1) == 27)
			break; // 27 是ESC键的ASCII码
	}

	// 释放VideoCapture对象
	cap.release();
	cv::destroyAllWindows(); // 关闭所有OpenCV创建的窗口

	return 0;
}
