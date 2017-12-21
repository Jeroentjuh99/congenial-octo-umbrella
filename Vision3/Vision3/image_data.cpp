#include "stdafx.h"
#include "image_data.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "avansvisionlib20.h"
#include <opencv2/videostab/inpainting.hpp>
#include "opencv2/features2d.hpp"
#include <filesystem>
#include <thread>

image_data::image_data() {}

//Feature detection from opencv doesn't work
Mat get_features_BRISK( Mat img ) {
	Ptr<BRISK> briskDetector = BRISK::create();
	vector<KeyPoint> keypoints;
	Mat *features = new Mat, mask;
	briskDetector->detectAndCompute( img, mask, keypoints, *features );
	return *features;
}

//Same here
Mat get_features_orb( Mat img ) {
	Mat* features = new Mat;
	Ptr<ORB> orb = ORB::create();
	Mat mask;
	vector<KeyPoint> kp = vector<KeyPoint>();
	orb->detectAndCompute( img, mask, kp, *features );
	return *features;
}

void image_data::createFeatures( Mat& image, vector<Image_Features>& features ) {
	Mat th1, denoised, gauss;
	Mat canny_output;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	int thresh = 100;
	cvtColor( image, th1, COLOR_BGR2GRAY );
	fastNlMeansDenoising( th1, denoised );
	GaussianBlur( denoised, gauss, Size( 3, 3 ), 7 );
	threshold( gauss, gauss, thresh, 255, THRESH_BINARY_INV );
	Canny( gauss, canny_output, thresh, 255, 3 );
	dilate( canny_output, canny_output, Mat() );
	findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point( 0, 0 ) );
	imshow( "All contours", canny_output );
	waitKey( 1 );

	for ( vector<Point> element : contours ) {
		Rect _boundingRect = boundingRect( element );
		RotatedRect boundingbox = minAreaRect( element );
		if ( _boundingRect.width < 10 || _boundingRect.height < 10 )
			continue;
		Mat x = image.clone();

		Vec3b mycolor( 255, 255, 0 );
		for ( int i = 0; i < element.size(); i++ ) {
			x.at<Vec3b>( element[i].y, element[i].x ) = mycolor;
		}

		x = x( _boundingRect );

		destroyWindow( "Found object" );
		imshow( "Found object", x );
		waitKey( 1 );

		if ( this->last_item == "" )
			cout << "Welk type object is dit? X voor overslaan" << endl;
		else {
			cout << "Welk type object is dit? X voor overslaan of enter voor " << this->last_item << endl;
		}
		string type;
		getline( cin, type );

		if ( type == "x" || type == "X" ) {
			continue;
		}
		if ( this->last_item != "" && type.empty() ) {
			type = this->last_item;
		}
		this->last_item = type;
		Image_Features found_features;
		vector<double> data;
		data.push_back( element.size() );
		data.push_back( bendingEnergy( element ) / element.size() );
		data.push_back( boundingbox.angle );
		data.push_back( boundingbox.size.height / boundingbox.size.width );

		found_features.featureColumncounted = data;
		found_features.type = type;
		features.push_back( found_features );
	}
}

image_data::~image_data() {}
