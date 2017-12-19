#include "stdafx.h"
#include "VisionNN.h"
#include <filesystem>
#include <experimental\filesystem>
#include <iostream>
#include <sstream>
#include <opencv2\core\core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/ml/ml.hpp>
#include <opencv2/imgproc.hpp>

VisionNN::VisionNN()
{
}

void VisionNN::load_images(std::string path)
{
	image_data* data = new image_data();
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
	{
		if (std::experimental::filesystem::is_directory(std::experimental::filesystem::status(p)))
		{
			std::stringstream stream;
			stream << p;
			
			for (auto &d : std::experimental::filesystem::directory_iterator(stream.str()))
			{
				std::stringstream filename;
				filename << d;
				cv::Mat MATimage = cv::imread(filename.str(), CV_LOAD_IMAGE_COLOR);

				cv::Size size(720, 640);//the dst image size,e.g.100x100
				cv::resize(MATimage, MATimage, size);//resize image

				image_data::ImageFeature feature;

				feature.type = stream.str();
				data->createFeature(MATimage, feature);
				test_pictures.push_back(feature);
			}
			
		}
	}
	delete data;
}

void VisionNN::capture_image()
{
}

void VisionNN::train(double errorPercentage)
{
	int categories = 5;
	cv::Mat picture_data = cv::Mat::zeros(1, 4, CV_32F);
	cv::ml::ANN_MLP *mlp = cv::ml::ANN_MLP::create();
	cv::Mat_<int> layers(3, 1);
	layers(0) = 8;
	layers(1) = 5;
	layers(2) = 5;
	mlp->setLayerSizes(layers);
	mlp->setTermCriteria(cvTermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 300, 0.0001));
	mlp->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.0001);
	cv::Mat train_classes = cv::Mat::zeros(picture_data.rows, categories, CV_32FC1);
	for (int i = 0; i<train_classes.rows; i++)
	{
		train_classes.at<float>(i, picture_data.at<int>(i)) = 1.f;
	}
	mlp->train(picture_data, cv::ml::ROW_SAMPLE, train_classes);
}

void VisionNN::get_objects()
{
}


VisionNN::~VisionNN()
{
}
