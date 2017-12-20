#include "stdafx.h"
#include "image_data.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "avansvisionlib20.h"
#include <opencv2/videostab/inpainting.hpp>
#include "opencv2/features2d.hpp"
#include <filesystem>

image_data::image_data() {}

//Feature detection from opencv doesn't work
Mat get_features_BRISK(Mat img) {
	cv::Ptr<cv::BRISK> briskDetector = cv::BRISK::create();
	std::vector<KeyPoint> keypoints;
	Mat *features = new Mat, mask;
	briskDetector->detectAndCompute(img, mask, keypoints, *features);
	return *features;
}

//Same here
Mat get_features_orb(Mat img)
{
	Mat *features = new Mat;
	Ptr<ORB> orb = ORB::create();
	Mat mask;
	vector<KeyPoint> kp = vector<KeyPoint>();
	orb->detectAndCompute(img, mask, kp, *features);
	return *features;
}

void image_data::createFeatures(cv::Mat& image, std::vector<Image_Features>& features) {
	Mat th1, denoised, gauss;
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int thresh = 80;
	cvtColor(image, th1, COLOR_BGR2GRAY);
	fastNlMeansDenoising(th1, denoised);
	GaussianBlur(denoised, gauss, Size(3, 3), 7);
	
	Canny(gauss, canny_output, thresh, 255, 3);
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	for ( vector<Point> element : contours ) {
		Rect _boundingRect = boundingRect(element);
		RotatedRect boundingbox = minAreaRect(element);
		if (_boundingRect.width < 50 || _boundingRect.height < 50) continue;
		//Mat m = image(_boundingRect);
		//Mat p = gauss(_boundingRect);
		Mat x = image.clone();

		Vec3b mycolor(255, 255, 0), mycolor2(0,255,255);
		for (int i = 0; i < element.size(); i++) {
			x.at<Vec3b>(element[i].y, element[i].x) = mycolor;
		}

		x = x(_boundingRect);

		imshow("Found object", x);
		waitKey(1);
		std::cout << "Welk type object is dit? X voor overslaan" << std::endl;
		std::string type;
		std::cin >> type;
		cvDestroyWindow("Found object");
		if(type == "x" || type == "X") {
			continue;
		}

	/*	if (!std::experimental::filesystem::exists("c://opencv//trainingsset//" + type)) {
			std::experimental::filesystem::create_directory("c://opencv//trainingsset//" + type);
		}
		int i = 0;
		for (auto &d : std::experimental::filesystem::directory_iterator("c://opencv//trainingsset//" + type)){
			i++;
		}
		imwrite("c://opencv//traininsset//" + type + std::to_string( i ) + ".jpg", m);
		cvWaitKey(1);*/ //<- saving image doesn't work... again...

		Image_Features found_features;
		vector<double> data;
		data.push_back(element.size());
		data.push_back(bendingEnergy(element) / element.size());
		vector<Point> points;
		int b = enclosedPixels(element, points);
		data.push_back(b);


		found_features.featureColumncounted = data;
		found_features.type = type;
		features.push_back(found_features);
	}
}

image_data::~image_data() {}
