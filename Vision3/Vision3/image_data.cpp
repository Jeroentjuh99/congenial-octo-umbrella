#include "stdafx.h"
#include "image_data.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "avansvisionlib20.h"
#include <opencv2/videostab/inpainting.hpp>
#include <filesystem>

image_data::image_data() {}

double calculateObjectSize( vector<Point>& contour ) {
	return contour.size();
}

double calculateBendingEnergy( vector<Point>& contour ) {
	return bendingEnergy( contour );
}

Mat get_features_orb(Mat img)
{
	Mat features;
	Ptr<ORB> orb = ORB::create();
	Mat mask;
	vector<KeyPoint> kp = vector<KeyPoint>();
	orb->detectAndCompute(img, mask, kp, features);
	return features;
}

void image_data::createFeatures(cv::Mat& image, std::vector<Image_Features>& features) {
	Mat th1, denoised, gauss;

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int thresh = 90;
	cvtColor(image, th1, COLOR_BGR2GRAY);
	fastNlMeansDenoising(th1, denoised);
	GaussianBlur(denoised, gauss, Size(3, 3), 7);

	Canny(gauss, canny_output, thresh, thresh * 2, 3);
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for ( vector<Point> element : contours ) {
		Rect _boundingRect = boundingRect(element);
		if (_boundingRect.width < 50 || _boundingRect.height < 50) continue;
		Mat m = image(_boundingRect);
		Mat p = gauss(_boundingRect);
		imshow("Found object", m);
		waitKey(1);
		std::cout << "Welk type object is dit? X voor overslaan" << std::endl;
		std::string type;
		std::cin >> type;
		cvDestroyWindow("Found object");
		if(type == "x" || type == "X") {
			continue;
		}
		if (!std::experimental::filesystem::exists("c://opencv//trainingsset//" + type)) {
			std::experimental::filesystem::create_directory("c://opencv//trainingsset//" + type);
		}
		int i = 0;
		for (auto &d : std::experimental::filesystem::directory_iterator("C://opencv//trainingsset//" + type)){
			i++;
		}
		imwrite("C://opencv//traininsset//" + type + std::to_string( i ) + ".jpg", m);

		Image_Features found_features;
		Mat feature = get_features_orb(p);
		vector<double> countColumn;
		for(int c = 0; c < feature.cols; c++) {
			double amount = 0;
			for(int r = 0; r < feature.rows; r++) {
				amount += feature.at<int>(r, c);
			}
			countColumn.push_back(amount);
		}
		found_features.featureColumncounted = countColumn;
		found_features.type = type;
		features.push_back(found_features);
	}
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
		/*Rect _boundingRect = boundingRect( contours[i] );
		Scalar mean_color0 = mean( layers[0]( _boundingRect ) );
		Scalar mean_color1 = mean( layers[1]( _boundingRect ) );
		Scalar mean_color2 = mean( layers[2]( _boundingRect ) );

		feature.hue = mean_color0[0];
		feature.saturation = mean_color1[0];
		feature.brightness = mean_color2[0];*/
		

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
	
	std::vector<KeyPoint> keypoints;
	FAST(th2, keypoints, 20);
	std::cout << keypoints.size() << std::endl;
}

image_data::~image_data() {}
