#include "stdafx.h"
#include "CameraCalibration.h"
#include <iostream>
#include <fstream>
#include "opencv/cv.hpp"
#include <windows.h>

CameraCalibration::CameraCalibration() {}

CameraCalibration::~CameraCalibration() {}

void CameraCalibration::calibrate_camera(bool createNewFile) {
	std::ifstream file;
	std::string filename = "calibration.yml";
	file.open(filename);
	if (!file || createNewFile) {
		file.close();
		create_calibration_file();
		return;
	}
	file.close();
	cv::FileStorage fs(filename, cv::FileStorage::READ);
	fs["intrinsic"] >> intrinsic;
	fs["distCoeffs"] >> distCoeffs;
	fs.release();
}

void CameraCalibration::undistort_view(cv::Mat* image, cv::Mat* imageUndistorted) {
	if (*intrinsic.size.p == 0 || *distCoeffs.size.p == 0)
		calibrate_camera(true);
	undistort(*image, *imageUndistorted, intrinsic, distCoeffs);
}

void CameraCalibration::create_calibration_file() const {
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

	while (successes < numBoards) {
		capture >> image;
		cvtColor(image, gray_image, CV_BGR2GRAY);
		imshow("orginal", image);
		imshow("gray image", gray_image);
		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		if (found) {
			cornerSubPix(gray_image, corners, cv::Size(11, 11), cv::Size(-1, -1),
			             cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray_image, board_sz, corners, found);
			capture >> image;
			imshow("orginal", image);
			imshow("gray image", gray_image);
			image_points.push_back(corners);
			object_points.push_back(obj);
			successes++;
			Sleep(500);
		}
	}

	cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
	cv::Mat distCoeffs;
	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;

	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;

	calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
	cv::FileStorage fs("c:\\results\\ueyecallib.yml", cv::FileStorage::WRITE);
	fs << "intrinsic" << intrinsic << "distCoeffs" << distCoeffs;
	fs.release();
}
