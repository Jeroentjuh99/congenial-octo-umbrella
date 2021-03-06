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
	void load_images(std::string url, bool training = true);
	void capture_image(bool is_for_training = true);
	void train(double, int, int);
	void verify_objects(cv::Mat picture_data, cv::Mat train_classes);
	void get_objects(cv::Mat, int);
	void save_network(std::string path);

	void load_network(std::string path);

	void save_image(int type, cv::Mat image);
	
	~VisionNN();

private:
	std::vector<image_data::Image_Features> test_pictures;
	std::vector<std::string> types;
	cv::Ptr<cv::ml::ANN_MLP> mlp;
};

