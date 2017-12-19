#pragma once

#include <opencv/cv.h>

class image_data
{
public:
	image_data(cv::Mat);
	std::string type;
	double getBendingEnergy();
	double getObjectSize();
	double getHue();
	double get_ratio();
	~image_data();
private:
	cv::Mat image;
};

