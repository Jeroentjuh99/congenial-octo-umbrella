#include "stdafx.h"
#include "image_data.h"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "avansvisionlib20.h"
#include <opencv2/videostab/inpainting.hpp>
#include <opencv2/highgui.hpp>

#define MIN_SIZE 10

image_data::image_data() {}

#pragma region not functional opencv code
//Feature detection from opencv doesn't work
Mat get_features_BRISK( const Mat img ) {
	Ptr<BRISK> brisk_detector = BRISK::create();
	vector<KeyPoint> keypoints;
	Mat *features = new Mat, mask;
	brisk_detector->detectAndCompute( img, mask, keypoints, *features );
	return *features;
}

//Same here
Mat get_features_orb( const Mat img ) {
	Mat *features = new Mat, mask;
	Ptr<ORB> orb = ORB::create();
	vector<KeyPoint> kp = vector<KeyPoint>();
	orb->detectAndCompute( img, mask, kp, *features );
	return *features;
}
#pragma endregion

void image_data::create_features( Mat& image, vector<Image_Features>& features, const bool is_training ) {
	Mat preview_image = Mat::zeros( 1, 1, CV_8S );
	Mat canny_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	create_contours( image, canny_output, contours, hierarchy, 100 );

	imshow( "All contours", canny_output );
	imshow( "Found object", preview_image );
	waitKey( 1 );

	for ( vector<Point> element : contours ) {
		Rect bounding_rect = boundingRect( element );
		const RotatedRect boundingbox = minAreaRect( element );
		if ( bounding_rect.width < MIN_SIZE || bounding_rect.height < MIN_SIZE )
			continue;

		preview_image = image.clone();
		cut_preview( preview_image, bounding_rect, element );
		destroyWindow( "Found object" );
		imshow( "Found object", preview_image );
		waitKey( 1 );

		Image_Features found_features;

		if ( is_training ) {
			assign_type( found_features );
			if ( found_features.type == "discard_please" )
				continue;
		}

		vector<double> data;
		data.push_back( element.size() );
		data.push_back( bendingEnergy( element ) / element.size() );
		data.push_back( boundingbox.angle );
		data.push_back( boundingbox.size.height / boundingbox.size.width );

		found_features.featureColumncounted = data;
		features.push_back( found_features );
	}
}

image_data::~image_data() {}

void image_data::assign_type( Image_Features& feature ) {
	if ( this->last_item == "" )
		cout << "Welk type object is dit? X voor overslaan" << endl;
	else {
		cout << "Welk type object is dit? X voor overslaan of enter voor " << this->last_item << endl;
	}
	string type;
	getline( cin, type );

	if ( type == "x" || type == "X" ) {
		feature.type = "discard_please";
		return;
	}
	if ( this->last_item != "" && type.empty() ) {
		type = this->last_item;
	}
	this->last_item = type;
	feature.type = type;
}

void image_data::cut_preview( Mat& input, Rect& bounding_box, vector<Point>& contour ) const {
	for ( int i = 0; i < contour.size(); i++ ) {
		input.at<Vec3b>( contour[i].y, contour[i].x ) = mycolor;
	}

	input = input( bounding_box );
}

void image_data::create_contours( Mat& image, Mat& canny_output, vector<vector<Point>>& contours,
                                  vector<Vec4i>& hierarchy,
                                  const int threshold_value ) const {
	Mat temp;
	cvtColor( image, temp, COLOR_BGR2GRAY );
	fastNlMeansDenoising( temp, temp );
	GaussianBlur( temp, temp, Size( 3, 3 ), 7 );
	threshold( temp, temp, threshold_value, 255, THRESH_BINARY_INV );
	Canny( temp, temp, threshold_value, 255, 3 );
	dilate( temp, canny_output, Mat() );
	findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point( 0, 0 ) );
}
