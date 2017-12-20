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
	};

	struct Image_Features {
		std::string type;
		std::vector<double> featureColumncounted;
	};

	image_data();
	void createFeatures(cv::Mat& image, std::vector<Image_Features>& features);
	void createFeature( cv::Mat& image, ImageFeature& feature );
	~image_data();
private:
	cv::Mat image;
};
