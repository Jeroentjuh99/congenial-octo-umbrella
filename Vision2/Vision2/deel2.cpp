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

void deel2::flood_fill(Mat image, Point startPoint, int replacement_color)
{
	vector<Point> nodes = vector<Point>();
	nodes.push_back(startPoint);

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].x < 0 || nodes[i].y < 0)
			continue;

		if (i % 500 > 498)
		{
			imshow("test", image);
			waitKey();
		}

		try
		{
			uchar test = image.at<uchar>(nodes[i]);
		}
		catch (const std::exception&)
		{
			int ripfest = 9001;
		}
		if (image.at<uchar>(nodes[i]) == 0)
		{
			image.at<uchar>(nodes[i]) = replacement_color;
		}
		else
		{
			continue;
		}

		Point up = Point(nodes[i].x, nodes[i].y + 1);
		Point down = Point(nodes[i].x, nodes[i].y - 1);
		Point left = Point(nodes[i].x - 1, nodes[i].y);
		Point right = Point(nodes[i].x + 1, nodes[i].y);

		nodes.push_back(right);
		nodes.push_back(left);
		nodes.push_back(down);
		nodes.push_back(up);
	}

}

int deel2::enclosedPixels(const vector<Point>& contourVec, vector<Point>& regionPixels, int cols, int rows)
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
	IplImage* ipImage = cvCreateImage(CvSize(cols, rows), IPL_DEPTH_8U, 1);
	Mat mat = cvarrToMat(ipImage);
	mat = Scalar(0, 0, 0);

	for (Point point : contourVec)
	{
		try
		{
			mat.at<uchar>(point) = 200;
		}
		catch (Exception e)
		{
			int iets = 1;
		}
	}

	Point startPos = Point(maxX / 2, maxY / 2);

	int currentWave = 200;

	flood_fill(mat, startPos, currentWave);

	for (int y = 0; y < mat.rows; y++)
	{
		for (int x = 0; x < mat.cols; x++)
		{
			if (mat.at<uchar>(Point(x, y)) == 200)
			{
				regionPixels.push_back(Point(x, y));
			}
		}
	}

	namedWindow("test", cv::WINDOW_AUTOSIZE);
	imshow("test", mat);

	waitKey(0);

	return 0;
}



void deel2::testEnclosedPixels()
{
	//Changed input method for the image path for simplicity
	string path = "C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\Vision2\\Vision2\\pictures\\Rectangle.png";
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

	show16SImageStretch(binary16S, "Binary image");

	std::vector<cv::Point> regionPixels = std::vector<cv::Point>();
	std::vector<std::vector<cv::Point>> contourVecs = std::vector<std::vector<cv::Point>>();
	allContours(binaryImage, contourVecs);
	
	IplImage* ipImage = cvCreateImage(CvSize(binary16S.cols, binary16S.rows), IPL_DEPTH_8U, 3);
	Mat contourImage = cvarrToMat(ipImage);
	contourImage = Scalar(255, 255, 255);

	drawContours(contourImage, contourVecs, -1, CV_RGB(255, 0, 0));

	imshow("contours", contourImage);
	
	waitKey();

	for (std::vector<cv::Point> contour : contourVecs)
	{
		enclosedPixels(contour, regionPixels, binaryImage.cols, binaryImage.rows);
		std::cout << regionPixels << std::endl;
	}
}
