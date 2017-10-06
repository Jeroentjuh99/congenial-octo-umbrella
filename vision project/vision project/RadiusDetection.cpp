#include "RadiusDetection.h"
#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include "opencv/highgui.h"
#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "blobdetectionavans.h"

RadiusDetection::RadiusDetection() {}


RadiusDetection::~RadiusDetection() {}

int RadiusDetection::DetectRadius() const {
	int radiusPixels, radiusCM;
	bool succes = false;
	Mat image, grey_image;
	vector<Vec3f> circles;

	VideoCapture video;
	try {
		video = VideoCapture(1);
	} catch (const std::exception& e) {
		video = VideoCapture(0);
	}

	while (!succes) {
		int key = waitKey(1);
		if (key == 27) {
			return 0;
		}
		if (key == ' ') {
			succes = true;
		}

		video.read(image);
		cvtColor(image, grey_image, CV_BGR2GRAY);

		imshow("original", image);
		imshow("grey image", grey_image);

		GaussianBlur(grey_image, grey_image, Size(9, 9), 2, 2);
		HoughCircles(grey_image, circles, HOUGH_GRADIENT, 1, grey_image.rows / 10, 200, 100, 0, 0);

		if (circles.size() != 0) {
			Vec3f circleData = circles.back();

			std::cout << "Radius: " << circleData[2] << " pixels" << std::endl;
		}
	}

	cvDestroyAllWindows();
	video.release();

	return 1;

}
