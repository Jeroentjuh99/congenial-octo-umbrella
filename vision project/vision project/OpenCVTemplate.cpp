//Standard c++ includes
#include "stdafx.h"
#include <iostream>

//OpenCV include
#include "Week2.h"
#include "RadiusDetection.h"
#include "Week5.h"

int main(int argc, char* argv[]) {

	//Week2 week2 = Week2();
	//week2.Opgave4();
	// Wacht op een muiskklik in het window van de afbeelding
	//waitKey(0);

//	RadiusDetection detect = RadiusDetection();
//	detect.DetectRadius();

	Week5 week5 = Week5();
	week5.Kerstballen();
//>>>>>>> ce9b417364a14e4e69d4b5067f5ec4d677b64984
//
//	Mat image = imread("C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\vision project\\vision project\\test-images\\dice5.bmp", CV_LOAD_IMAGE_COLOR);
//
//	// Window maken waarin de beelden "live" getoond worden
//	namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);
//	namedWindow("BlobDetection", CV_WINDOW_AUTOSIZE);
//
//		// De afbeelding converteren naar een grijswaarde afbeelding
//		Mat gray_image;
//		cvtColor(image, gray_image, CV_BGR2GRAY);
//
//		imshow("MyVideo", gray_image);
//
//		Mat binaryImage;
//		threshold(gray_image, binaryImage, 70, 255, THRESH_BINARY);
//
//		Mat binary16S;
//		binaryImage.convertTo(binary16S, CV_16S);
//
//		vector<Point2d *> firstpixelVec2;
//		vector<Point2d *> posVec2;
//		vector<int> areaVec2;
//		Mat blob_image;
//		int numberOfObjects = labelBLOBsInfo(binary16S, blob_image, firstpixelVec2, posVec2, areaVec2);
//
//		std::cout << "Number of objects: " << numberOfObjects << std::endl;
//		std::cout << "Number of objects attempt 2: " << firstpixelVec2.size() << std::endl;
//
//		// Het tonen van grijswaarde beeld
//		show16SImageStretch(blob_image, "BlobDetection");
//
//		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
//		if (waitKey(1) == 27)
//		{
//			cout << "esc key is pressed by user" << endl;
//		}
//	return 0;
}