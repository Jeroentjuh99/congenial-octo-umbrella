#include "stdafx.h"
#include "VisionNN.h"
#include <filesystem>
#include <experimental\filesystem>
#include <iostream>
#include <sstream>
#include <opencv\highgui.h>

VisionNN::VisionNN()
{
}

void VisionNN::capture_image()
{
}

void VisionNN::load_images(std::string path)
{
	image_data* data = new image_data();
	const cv::Size size(720, 640);
	std::vector<image_data::Image_Features> features;
	int typeCounter = 0;
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

				data->createFeatures(MATimage, features);
			}
			typeCounter++;
		}
	}
	test_pictures = features;
	delete data;
}

void VisionNN::train(double error_percentage, int max_iteraties, int hidden_neurons)
{
	delete mlp;
	int categories = 1;
	cv::Mat picture_data = cv::Mat::zeros(test_pictures.size(), 2, CV_32FC1);
	cv::Mat train_classes = cv::Mat::zeros(picture_data.rows, categories, CV_32FC1);
	std::vector<std::string> types = std::vector<std::string>();
	for (int i = 0; i < picture_data.rows; i++)
	{
		for (int j = 0; j < picture_data.cols; j++)
		{
			if (test_pictures[i].featureColumncounted.size() <= 0) break;
			picture_data.at<float>(i,j) = test_pictures[i].featureColumncounted[j];
		}
	}
	for (int i = 0; i < train_classes.rows; i++)
	{
		bool available = false;
		for (int j = 0; j < types.size(); j++)
		{
			if (test_pictures[i].type == types[j]) 
			{ 
				available = true;
				train_classes.at<float>(i, 0) = j;
			}
		}
		if (!available)
		{
			types.push_back(test_pictures[i].type);
			train_classes.at<float>(i, 0) = types.size() - 1;
		}
	}
	mlp = cv::ml::ANN_MLP::create();
	std::vector<int> layers = { picture_data.cols, hidden_neurons, hidden_neurons, train_classes.cols };
	mlp->setLayerSizes(layers);
	mlp->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.0001);
	cv::TermCriteria termCrit = cv::TermCriteria(cv::TermCriteria::Type::MAX_ITER | cv::TermCriteria::Type::EPS, max_iteraties, error_percentage);
	mlp->setTermCriteria(termCrit);
	mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM);
	mlp->train(picture_data, cv::ml::ROW_SAMPLE, train_classes);
	get_objects(picture_data, train_classes);
}

void VisionNN::get_objects(cv::Mat picture_data, cv::Mat train_classes)
{
	float accuracy = 0;
	cv::Mat predictedMat = cv::Mat::zeros(train_classes.rows, train_classes.cols, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++) {
		std::vector<float> predicted = std::vector<float>();
		mlp->predict(picture_data.row(i), predicted);
		for (int x = 0; x < predicted.size(); x++)
		{
			predictedMat.at<float>(i, x) = (int) predicted[x];
			float difference = predicted[x] - train_classes.at<float>(i, x);
			if (difference < 0) { difference = -difference; }
			accuracy += difference;
		}
	}
	accuracy = 1 - (accuracy / (predictedMat.cols * predictedMat.rows));
	std::cout << "Input: " << std::endl;
	std::cout << picture_data << std::endl << std::endl;
	for (int i = 0; i < predictedMat.rows; i++)
	{
		std::cout << "Uitkomst: " << predictedMat.row(i) << " Verwachte Output: " << train_classes.row(i) << std::endl;
	}
	std::cout << "Accuracy: " << std::endl;
	std::cout << accuracy << std::endl << std::endl;

	//for (int i = 0; i < test_pictures.size(); i++)
	//{
	//	save_image(test_pictures[i].type_index, test_pictures[i].image);
	//}
}

void VisionNN::save_image(int type, cv::Mat image)
{
	imwrite("C://opencv//test//" + std::to_string(type) + std::string(".jpg"), image);
}


VisionNN::~VisionNN()
{
}
