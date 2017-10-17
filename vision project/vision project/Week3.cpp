#include "Week3.h"

Week3::Week3()
{
	image = imread("C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\vision project\\vision project\\test-images\\dice5.bmp", CV_LOAD_IMAGE_COLOR);
}

void Week3::BlobDetectie()
{
		// Window maken waarin de beelden "live" getoond worden
		namedWindow("MyVideo", CV_WINDOW_AUTOSIZE);
		namedWindow("BlobDetection", CV_WINDOW_AUTOSIZE);
	
			// De afbeelding converteren naar een grijswaarde afbeelding
			Mat gray_image;
			cvtColor(image, gray_image, CV_BGR2GRAY);
	
			imshow("MyVideo", gray_image);
	
			Mat binaryImage;
			threshold(gray_image, binaryImage, 70, 255, THRESH_BINARY);
	
			Mat binary16S;
			binaryImage.convertTo(binary16S, CV_16S);
	
			vector<Point2d*> firstpixelVec2;
			vector<Point2d*> posVec2;
			vector<int> areaVec2;
			Mat blob_image;
			int numberOfObjects = labelBLOBsInfo(binary16S, blob_image, firstpixelVec2, posVec2, areaVec2);
	
			std::cout << "Number of objects: " << numberOfObjects << std::endl;
			std::cout << "Number of objects attempt 2: " << firstpixelVec2.size() << std::endl;
	
			// Het tonen van grijswaarde beeld
			show16SImageStretch(blob_image, "BlobDetection");
	
			//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
			if (waitKey(1) == 27)
			{
				cout << "esc key is pressed by user" << endl;
			}
}

void Week3::Opgave6()
{
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	Mat dst;

	threshold(gray_image, dst, 80, 255, THRESH_BINARY);

	Mat erodedImage;
	Mat structureElement = getStructuringElement(CV_SHAPE_RECT, Size(3,3));
	erode(dst, erodedImage, structureElement);

	Mat substractedImage;
	subtract(dst, erodedImage, substractedImage);

	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", substractedImage);

	waitKey();
}

Week3::~Week3()
{
}
