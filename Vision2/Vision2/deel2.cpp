#include "stdafx.h"
#include "deel2.h"
#include <iostream>
#include <fstream>
#include "opencv2/opencv.hpp"
#include "opencv/cv.hpp"
#include "avansvisionlib.h"
#include <experimental\filesystem>

deel2::deel2()
{
}

deel2::~deel2()
{
}

/**
* \brief De methode die de uiteindelijke flood fill uitvoert.
* \param image De afbeelding waar het flood fill in uitgevoerd wordt.
* \param startPoint Beginpunt van de flood fill.
* \param replacement_color De kleur waarmee de afbeelding ingekleurd wordt.
*/
void deel2::flood_fill(Mat image, Point startPoint, int replacement_color)
{
	vector<Point> nodes = vector<Point>();

	//Voeg het start point toe aan de lijst met nodes
	nodes.push_back(startPoint);

	for (int i = 0; i < nodes.size(); i++)
	{
		//Check of de pixel niet buiten de afbeelding valt
		if (nodes[i].x < 0 || nodes[i].y < 0)
			continue;


		/*if (i % 500 > 498)
		{
			imshow("test", image);
			waitKey();
		}*/

		//Probeer data van een pixel op te halen.
		try
		{
			uchar test = image.at<uchar>(nodes[i]);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		//Wanneer de pixel leeg is vul deze dan met de opgegeven waarde.
		if (image.at<uchar>(nodes[i]) == 0)
		{
			image.at<uchar>(nodes[i]) = replacement_color;
			//Wanneer de pixel inderdaad leeg was, zoek dan verder naar de pixels buiten de afgehandelde pixel.
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
}

/**
* \brief Berekent de flood fill pixels en geeft ze weer in een afbeelding.
* \param contourVec De pixel locaties voor de contour.
* \param regionPixels De pixel locaties na de flood fill.
* \param cols Aantal kolommen in de afbeelding.
* \param rows Aantal rijen in de afbeelding.
*/
int deel2::enclosedPixels(const vector<Point>& contourVec, vector<Point>& regionPixels, int cols, int rows)
{
	regionPixels = vector<Point>();
	int maxX = 0;
	int maxY = 0;
	int minX = 100000;
	int minY = 100000;
	//Haal de maximale x en y waarden uit de contouren
	for (Point point : contourVec)
	{
		if (point.x < minX)
		{
			minX = point.x;
		}
		if (point.y < minY)
		{
			minY = point.y;
		}
		if (point.x > maxX)
		{
			maxX = point.x;
		}
		if (point.y > maxY)
		{
			maxY = point.y;
		}
	}
	//Maak een mat aan om de contour in te zetten.
	IplImage* ipImage = cvCreateImage(CvSize(cols, rows), IPL_DEPTH_8U, 1);
	Mat mat = cvarrToMat(ipImage);
	mat = Scalar(0, 0, 0);

	//Teken het contour in de afbeelding.
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

	//Definieer een start positie voor de flood fill.

	Point startPos = Point(minX + (maxX - minX) / 2, minY + (maxY - minY) / 2);

	//Definieer een grijstint voor het opvullen van het contour.
	int value = 200;

	//Voor de flood fill uit op de afbeelding.
	flood_fill(mat, startPos, value);

	//Haal alle pixellocaties op met de opgegeven grijstint. Plaats deze in de lijst met regionPixels.
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

	//Geef een voorbeeld van de flood fill weer.
	namedWindow("test", cv::WINDOW_AUTOSIZE);
	imshow("test", mat);

	waitKey(0);

	return 0;
}


/**
* \brief Test de enclosed pixels/flood fill functie.
*/
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
	
	//Maak een afbeelding aan en geef hier de contouren in weer.
	IplImage* ipImage = cvCreateImage(CvSize(binary16S.cols, binary16S.rows), IPL_DEPTH_8U, 3);
	Mat contourImage = cvarrToMat(ipImage);
	contourImage = Scalar(255, 255, 255);

	drawContours(contourImage, contourVecs, -1, CV_RGB(255, 0, 0));

	imshow("contours", contourImage);
	
	waitKey();

	//Kijk voor elke contour naar welke pixels binnen deze contour vallen.
	for (std::vector<cv::Point> contour : contourVecs)
	{
		enclosedPixels(contour, regionPixels, binaryImage.cols, binaryImage.rows);
		std::cout << regionPixels << std::endl;
	}
}

int deel2::CutTrainingSet(const std::string image, const std::string classifier) {
	std::vector<std::vector<cv::Point>> Contours;
	cv::Mat MATimage = cv::imread(image, CV_LOAD_IMAGE_COLOR);
	cv::Mat gray_image;
	cv::cvtColor(MATimage, gray_image, CV_BGR2GRAY);
	cv::Mat binaryImage;
	threshold(gray_image, binaryImage, 165, 1, CV_THRESH_BINARY_INV);
	allContours(binaryImage, Contours);
	int amount = 0;

	for (int i = 0; i < Contours.size(); i++) {

		if (!std::experimental::filesystem::exists("c://opencv//" + classifier)) {
			std::experimental::filesystem::create_directory("c://opencv//" + classifier);
		}

		std::vector<cv::Point> regionPixels;
		try 
		{
			enclosedPixels(Contours[i], regionPixels, binaryImage.cols, binaryImage.rows);
			if (regionPixels.size() < 10)
				continue;
		}
		catch (Exception e)
		{
			continue;
		}

		//Maak een afbeelding aan en geef hier de contouren in weer.
		IplImage* ipImage = cvCreateImage(CvSize(binaryImage.cols, binaryImage.rows), IPL_DEPTH_8U, 1);
		Mat contourImage = cvarrToMat(ipImage);
		contourImage = Scalar(50, 50, 50);

		for (cv::Point point : regionPixels)
		{
			uchar color = MATimage.at<uchar>(point);
			contourImage.at<uchar>(point) = 250;
		}

		cv::imwrite("c://opencv//" + classifier + "//" + classifier + "" + std::to_string(i) + ".jpg", contourImage);
	}
	imwrite("c://opencv//" + classifier + "//" + classifier + ".jpg", MATimage);
	return amount;
}