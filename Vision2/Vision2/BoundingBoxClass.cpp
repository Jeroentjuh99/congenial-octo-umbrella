#include "stdafx.h"
#include "BoundingBoxClass.h"

BoundingBoxClass::BoundingBoxClass() {}

BoundingBoxClass::~BoundingBoxClass() {}

int BoundingBoxClass::allBoundingBoxes( const std::vector<std::vector<cv::Point>>& contours,
                                        std::vector<std::vector<cv::Point>>& bbs ) {
	int amount = 0;
	for ( const std::vector<cv::Point> contour : contours ) {

		//Coordinates for the bounding box
		//[1,2]
		//[3,4]
		cv::Point coord1, coord2, coord3, coord4;
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

		//Push the coordinates in the bbs vector
		BBox.insert( BBox.end(), { coord1, coord2, coord3, coord4 } );
		bbs.push_back( BBox );
		amount++;
	}
	return amount;
}
