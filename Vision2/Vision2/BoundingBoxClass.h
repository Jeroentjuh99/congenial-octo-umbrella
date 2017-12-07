#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.hpp"

class BoundingBoxClass {
public:
	//Add your own classifier and threshold values when using a new image
	enum classifier {
		bladeren,
		rummikubbin
	};

	BoundingBoxClass();
	~BoundingBoxClass();

	// func: delivers bounding Boxes of contours
	// pre: contours contains the contours for which bounding boxes have to be delivered
	// post: bbs contains all bounding boxes. The index corresponds to the index of contours.
	// I.e. bbs[i] belongs to contours[i]
	static int allBoundingBoxes( const std::vector<std::vector<cv::Point>>& contours,
	                             std::vector<std::vector<cv::Point>>& bbs );

	/**
	 * \brief This function reads a trainingset and cuts it into individual pieces
	 * \param image Path of the trainingset
	 * \param classifier Classifier of the objects
	 * \return Amount of blobs found
	 */
	int CutTrainingSet( std::string& image, classifier cl ) const;

private:
	static void ThresholdImage( cv::Mat&, cv::Mat&, classifier );
};
