#include "stdafx.h"
#include "deel2.h"
#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv/cv.hpp"
#include "avansvisionlib.h"

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

void deel2::recursiveFiller(Mat image, Point startPoint, int currentWave)
{
	currentWave++;
	image.at<uchar>(startPoint) = currentWave;

	Point up = Point(startPoint.x, startPoint.y + 1);
	Point down = Point(startPoint.x, startPoint.y - 1);
	Point left = Point(startPoint.x - 1, startPoint.y);
	Point right = Point(startPoint.x + 1, startPoint.y);

	if (currentWave > 500)
	{
		Mat binary16S;
		image.convertTo(binary16S, CV_16S);

		show16SImageStretch(binary16S);
	}

	if (image.at<uchar>(right) == 0)
	{
		recursiveFiller(image, right, currentWave);
	}
	if (image.at<uchar>(left) == 0)
	{
		recursiveFiller(image, left, currentWave);
	}
	if (image.at<uchar>(down) == 0)
	{
		recursiveFiller(image, down, currentWave);
	}
	if (image.at<uchar>(up) == 0)
	{
		recursiveFiller(image, up, currentWave);
	}
}

int deel2::enclosedPixels(const vector<Point>& contourVec, vector<Point>& regionPixels)
{
	regionPixels = vector<Point>();
	int maxX = 0;
	int maxY = 0;
	for (Point point : contourVec)
	{
		if (point.x > maxX)
		{
			maxX = point.x;
		}
		if (point.y > maxY)
		{
			maxY = point.y;
		}
	}
	Mat mat = cv::Mat::zeros(maxX, maxY, CV_8UC3);
	for (Point point : contourVec)
	{
		mat.at<uchar>(point) = 100;
	}

	Point startPos = Point(maxX / 2, maxY / 2);

	mat.at<uchar>(startPos) = 1;

	int currentWave = 1;

	recursiveFiller(mat, startPos, currentWave);

	imshow("test", mat);

	waitKey(0);

	return 0;
}



void deel2::testEnclosedPixels()
{
	//Changed input method for the image path for simplicity
	string path = "C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\Vision2\\Vision2\\pictures\\Rectangle-small.png";
	cout << "De imagefile = " << path << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread(path, CV_LOAD_IMAGE_COLOR);
	if (!image.data) {
		cout << "Could not open or find the image \n" << endl;
		waitKey();
		return;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: " << path << " met succes geconverteerd naar grijswaarde beeld.\n" << endl;

	imshow("Original", gray_image);
	waitKey(0);

	/** ! Gebruik een region of interest voor de rummikub images **/
	/*Mat imageROI = gray_image(Rect(34, 28, 976, 82));
	imshow("imageROI", imageROI);
	waitKey(0);*/

	// Grijswaarde afbeelding thresholden
	Mat binaryImage;

	/** ! Gebruik een region of interest voor de rummikub figuren **/
	// threshold(gray_image, binaryImage, 65, 1, CV_THRESH_BINARY_INV);

	// Stel een threshold in. Laad evt de figuur eerst in VisionLab om een goede
	// threshold te bepalen.
	// ! Comment this line out when using rummikub images
	threshold(gray_image, binaryImage, 165, 1, CV_THRESH_BINARY_INV);

	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	Mat binary16S;
	binaryImage.convertTo(binary16S, CV_16S);

	std::vector<cv::Point> regionPixels = std::vector<cv::Point>();
	std::vector<std::vector<cv::Point>> contourVecs = std::vector<std::vector<cv::Point>>();
	allContours(binaryImage, contourVecs);
	
	for (std::vector<cv::Point> contour : contourVecs)
	{
		enclosedPixels(contour, regionPixels);
	}
}
