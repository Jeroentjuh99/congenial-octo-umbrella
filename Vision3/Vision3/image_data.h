#pragma once

#include <opencv/cv.h>

class image_data
{
public:
	image_data();
	cv::Mat image;
	std::string type;
	double getBendingEnergy();
	double getObjectSize();
	double getHue();
	~image_data();
};

