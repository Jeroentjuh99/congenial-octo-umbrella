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

void Week5::Kerstballen() {
	Mat image = imread("C:\\Users\\jeroe\\Google Drive\\vision\\circles.png", CV_LOAD_IMAGE_COLOR);
	imshow("original", image);
	Mat hsb_image;
	cvtColor(image, hsb_image, COLOR_BGR2HSV);



	waitKey();
}