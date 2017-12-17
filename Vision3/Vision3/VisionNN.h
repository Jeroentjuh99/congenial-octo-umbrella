#pragma once

#include <string>

class VisionNN
{
public:
	VisionNN();
	void load_images(std::string url);
	void capture_image();
	void train(double errorPercentage);
	void get_objects();

	~VisionNN();
};

