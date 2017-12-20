#include "stdafx.h"
#include "VisionNN.h"
#include <filesystem>
#include <experimental\filesystem>
#include <iostream>
#include <sstream>


VisionNN::VisionNN()
{
}

void VisionNN::load_images(std::string path)
{
	image_data* data = new image_data();
	const cv::Size size(720, 640);
	std::vector<image_data::Image_Features> features;
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

				cv::resize(MATimage, MATimage, size);//resize image

				image_data::ImageFeature feature;

				feature.type = stream.str();
				data->createFeatures(MATimage, features);
				test_pictures.push_back(feature);
			}
		}
	}
	delete data;
}

void VisionNN::capture_image()
{
}

void VisionNN::train(double error_percentage, int max_iteraties, int hidden_neurons)
{
	int categories = 1;
	cv::Mat picture_data = cv::Mat::zeros(5, 5, CV_32FC1);
	cv::Mat train_classes = cv::Mat::zeros(picture_data.rows, categories, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++)
	{
		picture_data.at<float>(i, 0) =  5 - i;
	}
	for (int i = 0; i < train_classes.rows; i++)
	{
		train_classes.at<float>(i, 0) = i;
	}
	cv::Ptr<cv::ml::ANN_MLP> mlp = cv::ml::ANN_MLP::create();
	std::vector<int> layers = { picture_data.cols, hidden_neurons, train_classes.cols };
	mlp->setLayerSizes(layers);
	mlp->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.0001);
	cv::TermCriteria termCrit = cv::TermCriteria(cv::TermCriteria::Type::MAX_ITER | cv::TermCriteria::Type::EPS, max_iteraties, error_percentage);
	mlp->setTermCriteria(termCrit);
	mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);
	mlp->train(picture_data, cv::ml::ROW_SAMPLE, train_classes);
	float accuracy = 0;
	cv::Mat predictedMat = cv::Mat::zeros(train_classes.rows, train_classes.cols, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++) {
		std::vector<float> predicted = std::vector<float>();
		mlp->predict(picture_data.row(i), predicted);
		for (int x = 0; x < predicted.size(); x++)
		{
			predictedMat.at<float>(i, x) = predicted[x];
			float difference = predicted[x] - train_classes.at<float>(i, x);
			if (difference < 0) { difference = -difference; }
			accuracy += difference;
		}
	}
	accuracy = accuracy / (predictedMat.cols * predictedMat.rows);
	std::cout << "Input: " << std::endl;
	std::cout << picture_data << std::endl << std::endl;
	std::cout << "Verwachte Output: " << std::endl;
	std::cout << train_classes << std::endl << std::endl;
	std::cout << "Uitkomst: " << std::endl;
	std::cout << predictedMat << std::endl <<std::endl;
	std::cout << "Accuracy: " << std::endl;
	std::cout << 1 - accuracy << std::endl << std::endl;
}

void VisionNN::get_objects()
{
}


VisionNN::~VisionNN()
{
}
