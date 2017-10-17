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
	float cmPerPixel = 0;

	VideoCapture video;
	try {
		video = VideoCapture(1);
	} catch (const exception& e) {
		video = VideoCapture(0);
	}


	while (!succes) {
		int key = waitKey(1);
		if (key == 27) {
			return 0;
		}

		video.read(image);
		cvtColor(image, grey_image, CV_BGR2GRAY);

		imshow("original", image);
		imshow("grey image", grey_image);

		GaussianBlur(grey_image, grey_image, Size(9, 9), 2, 2);
		HoughCircles(grey_image, circles, HOUGH_GRADIENT, 1, grey_image.rows / 10, 200, 100, 0, 0);

		if (circles.size() != 0) {
			succes = true;
			Vec3f circleData = circles.back();

			cout << "Radius: " << circleData[2] << " pixels" << endl;
			cout << "Wat is de hoogte in centimeters?" << endl;
			float height;
			cin >> height;

			cmPerPixel = height / (circleData[2] * 2);

			cout << "Je kan de referentie onder de camera vandaan halen" << endl;
		}
	}

	succes = false;
	cout << "Spatiebalk of esc om te stoppen" << endl;

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

			cout << "Radius: " << circleData[2] << " pixels" << endl;
			cout << "Hoogte in cm: " << circleData[2] * 2 * cmPerPixel << endl;

			for (size_t i = 0; i < circles.size(); i++) {
				Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
				int radius = cvRound(circles[i][2]);
				circle(grey_image, center, 3, Scalar(0, 255, 0), -1, 8, 0);
				circle(grey_image, center, radius, Scalar(0, 0, 255), 3, 8, 0);
			}
		}
	}

	cvDestroyAllWindows();
	video.release();

	return 1;

}
