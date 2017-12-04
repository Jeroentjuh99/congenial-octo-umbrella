#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.hpp"

class BoundingBoxClass {
public:
	BoundingBoxClass();
	~BoundingBoxClass();

	// func: delivers bounding Boxes of contours
	// pre: contours contains the contours for which bounding boxes have to be delivered
	// post: bbs contains all bounding boxes. The index corresponds to the index of contours.
	// I.e. bbs[i] belongs to contours[i]
	static int allBoundingBoxes( const std::vector<std::vector<cv::Point>>& contours,
	                             std::vector<std::vector<cv::Point>>& bbs );


};
