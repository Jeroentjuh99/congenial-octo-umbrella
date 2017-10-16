#include "Week6.h"



Week6::Week6()
{
	image = imread("D:\\Users\\Remco\\Documents\\Github\\congenial-octo-umbrella\\vision project\\vision project\\krassen.jpg", CV_LOAD_IMAGE_COLOR);
}

void Week6::Opdracht2()
{
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	Mat dst;

	Canny(gray_image, dst, 70, 200);

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", dst);

	waitKey();
}

Week6::~Week6()
{
}
