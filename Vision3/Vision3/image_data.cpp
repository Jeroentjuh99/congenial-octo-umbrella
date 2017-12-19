#include "stdafx.h"
#include "image_data.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "avansvisionlib20.h"

image_data::image_data() {}

double calculateObjectSize( vector<Point>& contour ) {
	return contour.size();
}

double calculateBendingEnergy( vector<Point>& contour ) {
	return bendingEnergy( contour );
}

void image_data::createFeature( Mat& image, ImageFeature& feature ) {
	Mat th2, th3, hsv_image;
	vector<Mat> layers;
	cvtColor( image, th2, COLOR_BGR2GRAY );
	GaussianBlur( th2, th2, Size( 3, 3 ), 7 );
	adaptiveThreshold( th2, th3, 100, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 7 );
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;
	findContours( th3, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	cvtColor( image, hsv_image, COLOR_BGR2HSV );
	split( hsv_image, layers );

	for ( size_t i = 0; i < contours.size(); i++ ) {
		if(contours[i].size() <= 100)
			continue;
		Rect _boundingRect = boundingRect( contours[i] );
		Scalar mean_color0 = mean( layers[0]( _boundingRect ) );
		Scalar mean_color1 = mean( layers[1]( _boundingRect ) );
		Scalar mean_color2 = mean( layers[2]( _boundingRect ) );

		feature.hue = mean_color0[0];
		feature.saturation = mean_color1[0];
		feature.brightness = mean_color2[0];
		feature.bendingEnery = calculateBendingEnergy( contours[i] );
		feature.objectSize = calculateObjectSize( contours[i] );
	}
	int count = 0;
	for ( int i = 0; i < contours.size(); i = hierarchy[i][0] ) {
		if (contours[i].size() <= 100)
			continue;
		Rect r = boundingRect( contours[i] );
		if ( hierarchy[i][2] < 0 ) {
			count++;
		}
	}
	feature.nrOfHoles = count;
}

image_data::~image_data() {}
