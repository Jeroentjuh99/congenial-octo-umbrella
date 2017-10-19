#include "Week5.h"

#include "opencv/highgui.h"
#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "blobdetectionavans.h"

Week5::Week5()
{
}


Week5::~Week5()
{
}

void Week5::HSVScheiding(string imagePath)
{
	Mat image = imread(imagePath, CV_LOAD_IMAGE_COLOR);
	Mat hsv_image;
	cvtColor(image, hsv_image, COLOR_BGR2HSV);

	vector<Mat> layers;
	split(hsv_image, layers);

	imshow("Origineel", image);
	imshow("Hue", layers[0]);
	imshow("Saturation", layers[1]);
	imshow("Value", layers[2]);

	waitKey();
}

void Week5::Kerstballen() {
	Mat image = imread("C:\\Users\\jeroe\\Google Drive\\vision\\circles.png", CV_LOAD_IMAGE_COLOR);
	TelKleuren(image);
}

void Week5::KerstballenOnderCamera()
{
	VideoCapture video = VideoCapture(1);
	Mat image;
	
	while (true) {
		video.read(image);

		int key = waitKey(1);
		if (key == 27) {
			return;
		}
		if (key == ' ') {
			Week5::TelKleuren(image);
		}
	}
}

void Week5::TelKleuren(cv::Mat image) {
	Mat hsv_image;
	vector<Mat> layers;
	vector<Vec3f> circles;
	vector<vector<Vec3i>> colors;
	cvtColor(image, hsv_image, COLOR_BGR2HSV);
	split(hsv_image, layers);

	HoughCircles(layers[1], circles, HOUGH_GRADIENT, 1, hsv_image.rows / 6, 256, 5, 0, 50);

	if (circles.size() != 0) {
		for (size_t i = 0; i < circles.size(); i++) {
			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			circle(image, center, 3, Scalar(0, 255, 0), 1, 8, 0); //show centerpoint
			circle(image, center, radius, Scalar(0, 0, 150), 1, 8, 0); //show line
			//putText(image, std::to_string((int)i), center, FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 255, 0), 2, 8, false); //show circle number

			Vec3i color = Vec3i(layers[0].at<uchar>(center), layers[1].at<uchar>(center), layers[2].at<uchar>(center));
			bool isNewColor = true;
			for (int i = 0; i < colors.size(); i++)
			{
				if (colors[i][0] == color) {
					colors[i].push_back(color);
					isNewColor = false;
					break;
				}
			}
			if (isNewColor) {
				colors.push_back(vector<Vec3i>{color});
			}
		}
	}

	for each (vector<Vec3i> colorCollection in colors)
	{
		std::cout << "HSV(" << colorCollection[0][0] << ", " << colorCollection[0][1] << ", " << colorCollection[0][2] << ") heeft " << colorCollection.size() << " cirkels." << std::endl;
	}

	imshow("image", image);

	waitKey();
}
