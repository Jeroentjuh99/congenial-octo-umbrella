#include "stdafx.h"
#include "deel1.h"
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "avansvisionlib.h"

// Functionaliteit: Testprogramma voor het testen van de allContours() functie 
// Zie module Vision C++ voor Gevorderden Opdracht 2 van Fase 1 
// De demo laat met avansvisionlib en OpenCV de contouren zien.
//
// De algoritmiek wordt in de eerste workshop toegelicht.
//
// Jan Oostindie, dd 8-11-2016
//

using namespace cv;
using namespace std;

deel1::deel1() {}

int deel1::deel1Code() {
	//Changed input method for the image path for simplicity
	string path = "C:\\Users\\jeroe\\Google Drive\\vision\\Vision2\\images bij de opdracht beschrijving\\monsters.jpg";
	cout << "De imagefile = " << path << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread( path, CV_LOAD_IMAGE_COLOR );
	if ( !image.data ) {
		cout << "Could not open or find the image \n" << endl;
		waitKey();
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor( image, gray_image, CV_BGR2GRAY );

	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: " << path << " met succes geconverteerd naar grijswaarde beeld.\n" << endl;

	imshow( "Original", gray_image );
	waitKey( 0 );

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
	threshold( gray_image, binaryImage, 165, 1, CV_THRESH_BINARY_INV );

	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	Mat binary16S;
	binaryImage.convertTo( binary16S, CV_16S );

	show16SImageStretch( binary16S, "Binary image" );

	// haal de contouren uit het binary image
	vector<vector<Point>> contours;
	cout << "Contouren bepalen...";
	//Changed image so we won't have a stretched image with wrong data
	int aantal = allContours( binaryImage, contours ); // <<<<*** De opdracht is dus om deze functie te maken *** << gedaan

	// druk alle punten van alle gevonden contouren af
	string line;
	cout << "Aantal gevonden contouren = " << contours.size() << endl;
	getline( cin, line );
	for ( int i = 0; i < contours.size(); i++ ) {
		cout << "*** Contour " << i + 1 << " ***" << endl;
		cout << "Press ENTER to continue....";
		getline( cin, line );
		for ( int j = 0; j < contours[i].size(); j++ ) {
			cout << "(" << contours[i][j].x << "," << contours[i][j].y << ")" << endl;
		}
	}

	// Creeer een witte image
	//Mat contourImage = cvCreateImage(cvSize(binary16S.cols, binary16S.rows), IPL_DEPTH_8U, 3);
	//contourImage = Scalar(255, 255, 255);

	IplImage* iplimage = cvCreateImage( cvSize( binary16S.cols, binary16S.rows ), IPL_DEPTH_8U, 3 );
	Mat contourImage = cvarrToMat( iplimage );
	contourImage = Scalar( 255, 255, 255 );

	// teken de contouren op de witte image
	drawContours( contourImage, contours, -1, CV_RGB(255, 0, 0) );

	// druk het image met de contouren af
	imshow( "Found contours", contourImage );

	waitKey();

	//string pipo;
	//cin >> pipo;

	for ( int i = 0; i < contours.size(); i++ ) {
		cout << "BendingEnergy contour " << i << ": " << bendingEnergy( contourImage, contours[i] ) << endl;
	}

	waitKey();

	return 0;
}

deel1::~deel1() {}
