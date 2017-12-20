#pragma once

#include <opencv/cv.h>

class image_data {
public:
	struct ImageFeature {
		std::string type;
		double bendingEnery;
		double objectSize;
		double hue, saturation, brightness;
		int nrOfHoles;
		int type_index;
		cv::Mat image;
	};

	image_data();
	void createFeature( cv::Mat& image, ImageFeature& feature );
	~image_data();
private:
	cv::Mat image;
};
