// Functionaliteit: Testprogramma voor het testen van de allContours() functie 
// Zie module Vision C++ voor Gevorderden Opdracht 2 van Fase 1 
// De demo laat met avansvisionlib en OpenCV de contouren zien.
//
// De algoritmiek wordt in de eerste workshop toegelicht.
//
// Jan Oostindie, dd 8-11-2016
//
#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "avansvisionlib.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
	string path = "C:\\Users\\aares\\Documents\\Github\\congenial-octo-umbrella\\Vision2\\Vision2\\pictures\\Rectangle.png";	
	cout << "De imagefile = " << path << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread(path, CV_LOAD_IMAGE_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: " << path << " met succes geconverteerd naar grijswaarde beeld." << endl;

	////////////////////////////////////////////////////////////////////////////////

	imshow("Original", gray_image);

	/** ! Gebruik een region of interest voor de rummikub images **/
	// Mat imageROI = gray_image(Rect(34, 28, 976, 82));
	// imshow("imageROI", imageROI);
	// waitKey(0);

	Mat cannyImage;
	std::vector<vector<Point>> contours = std::vector<vector<Point>>();
	vector<Vec4i> hierarchy;
	Canny(gray_image, cannyImage, 165, 165 * 2);
	findContours(cannyImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	RNG rng(12345);

	Mat drawing = Mat::zeros(cannyImage.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(cannyImage, contours, i, color, 2, 8, hierarchy);
	}

	Mat oldImage = image;
	for (int i = 0; i < contours.size(); i++)
	{
		image = oldImage;
		int powerLevel = bendingEnergy(image, contours[i]);
		std::cout << "object: " << i << " has a power level of: " << powerLevel << std::endl;
		imshow("Contour " + std::to_string(i), image);
		if (powerLevel > 9000)
		{
			std::cout << "IT'S OVER 9000" << std::endl;
		}
	}

	imshow("Canny", cannyImage);

	waitKey();

	return 0;
}
