#include "stdafx.h"
#include "BoundingBoxClass.h"
#include "avansvisionlib.h"
#include <filesystem>

BoundingBoxClass::BoundingBoxClass() {}

BoundingBoxClass::~BoundingBoxClass() {}

int BoundingBoxClass::allBoundingBoxes( const std::vector<std::vector<cv::Point>>& contours,
                                        std::vector<std::vector<cv::Point>>& bbs ) {
	int amount = 0;
	for ( const std::vector<cv::Point> contour : contours ) {

		//Coordinates for the bounding box
		//[1,2]
		//[3,4]
		cv::Point coord1 = contour[0], coord2 = contour[0], coord3 = contour[0], coord4 = contour[0];
		std::vector<cv::Point> BBox;

		//Foreach contout create the right coordinates
		for ( const cv::Point point : contour ) {
			if ( point.x <= coord1.x ) {
				coord1.x = coord3.x = point.x;
			} else if ( point.x >= coord2.x ) {
				coord2.x = coord4.x = point.x;
			}
			if ( point.y <= coord1.y ) {
				coord1.y = coord2.y = point.y;
			} else if ( point.y >= coord3.y ) {
				coord3.y = coord4.y = point.y;
			}
		}

		if ((coord4.x - coord1.x >= 10) && (coord4.y - coord1.y >= 10)) {
			//Push the coordinates in the bbs vector
			BBox.insert(BBox.end(), { coord1, coord2, coord3, coord4 });
			bbs.push_back(BBox);
			amount++;
		}
	}
	return amount;
}

int BoundingBoxClass::CutTrainingSet( const std::string image, const std::string classifier ) {
	std::vector<std::vector<cv::Point>> Contours, BoundingBoxes;
	cv::Mat MATimage = cv::imread(image,CV_LOAD_IMAGE_COLOR);
	cv::Mat gray_image;
	cv::cvtColor(MATimage, gray_image, CV_BGR2GRAY);
	cv::Mat binaryImage;
	threshold(gray_image, binaryImage, 165, 1, CV_THRESH_BINARY_INV);
	allContours( binaryImage, Contours );
	
	int amount = allBoundingBoxes( Contours, BoundingBoxes );
	for(int i = 0; i < BoundingBoxes.size(); i++) {

		if (!std::experimental::filesystem::exists("c://opencv//" + classifier)) {
			std::experimental::filesystem::create_directory("c://opencv//" + classifier);
		}

		cv::Mat ROI = MATimage(cv::Rect(BoundingBoxes[i][0].x, BoundingBoxes[i][0].y, BoundingBoxes[i][3].x - BoundingBoxes[i][0].x, BoundingBoxes[i][3].y - BoundingBoxes[i][0].y));
		
		cv::imwrite("c://opencv//" + classifier + "//" + classifier + "" + std::to_string( i ) + ".jpg", ROI);
	}
	imwrite("c://opencv//" + classifier + "//" + classifier + ".jpg", MATimage);
	return amount;
}
