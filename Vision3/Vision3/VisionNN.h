#pragma once

#include <string>
#include <vector>
#include "image_data.h"
#include <opencv2/ml.hpp>
#include <opencv2\opencv.hpp>

class VisionNN
{
public:
	VisionNN();
	void load_images(std::string url);
	void capture_image();
	void train(double, int, int);
	void get_objects();

	~VisionNN();

private:
	std::vector<image_data::ImageFeature> test_pictures;
};

