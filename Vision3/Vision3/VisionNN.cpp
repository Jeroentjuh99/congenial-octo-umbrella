#include "VisionNN.h"
#include <filesystem>
#include <experimental\filesystem>
#include <iostream>
#include <sstream>
#include <opencv2\core\core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>

VisionNN::VisionNN()
{
}

void VisionNN::load_images(std::string path)
{
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
	{
		if (std::experimental::filesystem::is_directory(std::experimental::filesystem::status(p)))
		{
			std::stringstream stream;
			stream << p;
			for (auto &d : std::experimental::filesystem::directory_iterator(stream.str()))
			{
				std::stringstream filename;
				filename << d << std::endl;
				cv::Mat MATimage = cv::imread(filename.str(), CV_LOAD_IMAGE_COLOR);
				image_data data = image_data();
				data.image = MATimage;
				data.type = stream.str();
				test_pictures.push_back(data);
			}
		}
	}
}

void VisionNN::capture_image()
{
}

void VisionNN::train(double errorPercentage)
{
}

void VisionNN::get_objects()
{
}


VisionNN::~VisionNN()
{
}
