#include "stdafx.h"
#include "BoundingBoxClass.h"
#include "avansvisionlib.h"
#include <filesystem>

BoundingBoxClass::BoundingBoxClass() {}

BoundingBoxClass::~BoundingBoxClass() {}

/**
 * \brief This function seeks out all the BoundingBoxes from contours
 * \param contours Input of all the contours in an binary image
 * \param bbs Output vector with all the BoundingBox points per contour
 * \return Amount of bounding boxes
 */
int BoundingBoxClass::allBoundingBoxes( const std::vector<std::vector<cv::Point>>& contours,
                                        std::vector<std::vector<cv::Point>>& bbs ) {
	int amount = 0;
	for ( const std::vector<cv::Point> contour : contours ) {

		//Coordinates for the bounding box
		//[1,2]
		//[3,4]
		cv::Point coord1 = contour[0], coord2 = contour[0], coord3 = contour[0], coord4 = contour[0];
		std::vector<cv::Point> BBox;

		//Foreach contour create the right coordinates
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

		//Minimum size of a BoundingBox is 10x10 pixels
		if ( ( coord4.x - coord1.x >= 10 ) && ( coord4.y - coord1.y >= 10 ) ) {
			//Push the coordinates in the bbs vector
			BBox.insert( BBox.end(), { coord1, coord2, coord3, coord4 } );
			bbs.push_back( BBox );
			amount++;
		}
	}
	return amount;
}

/**
 * \brief This function cuts a training set in pieces and saves it as individual images
 * \param image Path to the image
 * \param cl BoundingBoxClass::classifier enum to specify which image it is (used for thresholding)
 * \return Amount of bounding boxes
 */
int BoundingBoxClass::CutTrainingSet( std::string& image, const classifier cl ) const {
	std::vector<std::vector<cv::Point>> contours, bounding_boxes;

	//Remember the name of the image without extention
	const std::string name = image.substr( image.find_last_of( "\\" ),
	                                       image.find_last_of( "." ) - image.find_last_of( "\\" ) );
	const cv::Mat MATimage = cv::imread( image, CV_LOAD_IMAGE_COLOR );

	//Temp Mat objects
	cv::Mat *gray_image = new cv::Mat, *binary_image = new cv::Mat;
	cvtColor( MATimage, *gray_image, CV_BGR2GRAY );

	//Threshold image
	ThresholdImage( *gray_image, *binary_image, cl );

	//Get the contours in the image
	allContours( *binary_image, contours );
	delete gray_image, binary_image;

	//Get all bounding boxes
	const int amount = allBoundingBoxes( contours, bounding_boxes );
	for ( int i = 0; i < bounding_boxes.size(); i++ ) {

		//Create folder if it doesn't exist
		if ( !std::experimental::filesystem::exists( "c://opencv//" + name ) ) {
			std::experimental::filesystem::create_directory( "c://opencv//" + name );
		}

		//Create the ROI starting the smalles coordinate and then the with and height (largest coordinate - smalles coordinate)
		cv::Mat ROI = MATimage( cv::Rect( bounding_boxes[i][0].x, bounding_boxes[i][0].y,
		                                  bounding_boxes[i][3].x - bounding_boxes[i][0].x,
		                                  bounding_boxes[i][3].y - bounding_boxes[i][0].y ) );

		//Save the image
		imwrite( "c://opencv//" + name + "//" + name + "" + std::to_string( i ) + ".jpg", ROI );
	}
	imwrite( "c://opencv//" + name + "//" + name + ".jpg", MATimage );
	return amount;
}

/**
 * \brief This function will threshold the image correctly. Specify your threshold when using new images
 * \param input Input image to threshold
 * \param output Thresholded image
 * \param cl BoundingBoxClass::classifier enum to specify which image it is (used to select the right threshold)
 */
void BoundingBoxClass::ThresholdImage( cv::Mat& input, cv::Mat& output, const classifier cl ) {
	switch ( cl ) {
		case bladeren:
		case rummikubbin:
			threshold( input, output, 165, 1, CV_THRESH_BINARY_INV );
			break;
	}
}
