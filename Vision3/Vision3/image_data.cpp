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
	cv::Mat th3;
	cv::adaptiveThreshold(image, th3, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
	cv::imshow("image", th3);
	cvWaitKey(0);
}

image_data::~image_data() {}
