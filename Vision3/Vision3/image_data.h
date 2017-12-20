#pragma once

#include <opencv/cv.h>

class image_data {
public:
	struct Image_Features {
		std::string type;
		std::vector<double> featureColumncounted;
	};

	image_data();
	void createFeatures(cv::Mat& image, std::vector<Image_Features>& features);
	~image_data();
private:
	cv::Mat image;
};
