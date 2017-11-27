#include "stdafx.h"
#include "deel2.h"
#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv/cv.hpp"

deel2::deel2()
{
}

deel2::~deel2()
{
}

void deel2::calibrate_camera(bool createNewFile) {
	std::ifstream file;
	std::string filename = "calibration.yml";
	file.open(filename);
	if(!file || createNewFile) {
		file.close();
		create_calibration_file();
		return;
	}

}

void deel2::create_calibration_file() {
	int numBoards = 10, numCornersHor = 9, numCornersVer = 6, numSquares = numCornersHor * numCornersVer, successes = 0;
	std::vector<std::vector<cv::Point3f>> object_points;
	std::vector<std::vector<cv::Point2f>> image_points;
	std::vector<cv::Point2f> corners;
	std::vector<cv::Point3f> obj;
	cv::Mat image, gray_image;
	
	std::cout << "Use the standard checkerboard pattern 10 times" << std::endl;
	cv::Size board_sz = cv::Size(numCornersHor, numCornersVer);
	cv::VideoCapture capture = cv::VideoCapture(1);

	for (int j = 0; j < numSquares; j++)
		obj.push_back(cv::Point3f(j / numCornersHor, j % numCornersHor, 0.0f));

	capture >> image;


}
