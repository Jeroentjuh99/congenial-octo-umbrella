#pragma once

#include <string>
#include <vector>
#include "image_data.h"

class VisionNN
{
public:
	VisionNN();
	void load_images(std::string url);
	void capture_image();
	void train(double errorPercentage);
	void get_objects();

	~VisionNN();

private:
	std::vector<image_data::ImageFeature> test_pictures;
};

