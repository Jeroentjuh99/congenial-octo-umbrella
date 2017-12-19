#include "stdafx.h"
#include "image_data.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

image_data::image_data() {}

double calculateObjectSize() {
	return 1;
}

double calculateBendingEnergy() {
	return 1;
}

void image_data::createFeature( cv::Mat& image, ImageFeature& feature ) {
	cv::Mat th2,th3;
	cv::imshow("image", image);
	cvWaitKey(0);
	cv::cvtColor(image, th2, cv::COLOR_BGR2GRAY);
	cv::imshow("image", th2);
	cvWaitKey(0);
	cv::GaussianBlur(th2, th2, cv::Size(3, 3), 7);
	cv::adaptiveThreshold(th2, th3, 100, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 15, 7);
	cv::imshow("image", th3);
	cvWaitKey(0);
}

image_data::~image_data() {}
