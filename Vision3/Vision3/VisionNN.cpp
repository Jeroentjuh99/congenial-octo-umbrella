#include "stdafx.h"
#include "VisionNN.h"
#include <iostream>
#include <sstream>
#include <opencv\highgui.h>
#include <windows.h>
#include <fstream>
#include <experimental\filesystem>

VisionNN::VisionNN()
{
}

void VisionNN::capture_image(bool is_for_training)
{
	std::vector<cv::Mat> images;
	image_data* data_parser = new image_data();
	cv::VideoCapture input = cv::VideoCapture(1);
	cv::Mat image;
	std::vector<image_data::Image_Features> features;
	bool should_stop = false;

	std::cout << "Maak foto's van de items met spatie. Deze zullen later omgezet worden. " << std::endl <<"Druk op esc wanneer u klaar bent" << std::endl;

	while ( should_stop == false ) {
		input.read(image);
		cv::imshow("Camera input", image);
		int key = cv::waitKey(1);
		if(key == 27) {
			should_stop = true;
			if(images.size() > 0) {
				break;
			} else {
				return;
			}
		}
		if(key == ' ') {
			images.push_back(image.clone());
			Sleep(500);
		}
	}

	cv::destroyWindow("Camera input");

	for ( cv::Mat saved_image : images ) {
		data_parser->create_features(saved_image, features, is_for_training);
	}
	test_pictures = features;
	delete data_parser;
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

				data->create_features(MATimage, features);
			}
			typeCounter++;
		}
	}
	test_pictures = features;
	delete data;
}

void VisionNN::train(double error_percentage, int max_iteraties, int hidden_neurons)
{
	std::cout << "Starting training" << std::endl;
	delete mlp;
	int categories = 1;
	cv::Mat picture_data = cv::Mat::zeros(test_pictures.size(), 2, CV_32FC1);
	cv::Mat train_classes = cv::Mat::zeros(picture_data.rows, categories, CV_32FC1);
	types = std::vector<std::string>();
	for (int i = 0; i < picture_data.rows; i++)
	{
		for (int j = 0; j < picture_data.cols; j++)
		{
			if (test_pictures[i].feature_descriptors.size() <= 0) break;
			picture_data.at<float>(i,j) = test_pictures[i].feature_descriptors[j];
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
				test_pictures[i].type_index = j;
				train_classes.at<float>(i, 0) = j;
			}
		}
		if (!available)
		{
			types.push_back(test_pictures[i].type);
			test_pictures[i].type_index = types.size() - 1;
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
	verify_objects(picture_data, train_classes);
}

void VisionNN::verify_objects(cv::Mat picture_data, cv::Mat train_classes)
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
		int index = (int) train_classes.at<float>(i, 0);
		std::string typeName = types[index];
		test_pictures[i].type_index = index;
		std::cout << "Uitkomst: " << predictedMat.row(i) << " Verwachte Output: " << train_classes.row(i) << " (" << typeName << ")" << std::endl;
	}
	std::cout << "Accuracy: " << std::endl;
	std::cout << accuracy << std::endl << std::endl;

	//for (int i = 0; i < test_pictures.size(); i++)
	//{
	//	save_image(test_pictures[i].type_index, test_pictures[i].image);
	//}
}

void VisionNN::get_objects(cv::Mat output_data, int nrOfOutputCols)
{
	cv::Mat picture_data = cv::Mat::zeros(test_pictures.size(), 2, CV_32FC1);
	types = std::vector<std::string>();
	for (int i = 0; i < picture_data.rows; i++)
	{
		for (int j = 0; j < picture_data.cols; j++)
		{
			if (test_pictures[i].featureColumncounted.size() <= 0) break;
			picture_data.at<float>(i, j) = test_pictures[i].featureColumncounted[j];
		}
	}
	float accuracy = 0;
	cv::Mat predictedMat = cv::Mat::zeros(picture_data.rows, nrOfOutputCols, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++) {
		std::vector<float> predicted = std::vector<float>();
		mlp->predict(picture_data.row(i), predicted);
		for (int x = 0; x < predicted.size(); x++)
		{
			predictedMat.at<float>(i, x) = (int)predicted[x];
		}
	}
	output_data = predictedMat;
	accuracy = 1 - (accuracy / (predictedMat.cols * predictedMat.rows));
	std::cout << "Input: " << std::endl;
	std::cout << picture_data << std::endl << std::endl;
	for (int i = 0; i < predictedMat.rows; i++)
	{
		std::cout << "Uitkomst: " << predictedMat.row(i) << std::endl;
	}

	//for (int i = 0; i < test_pictures.size(); i++)
	//{
	//	save_image(test_pictures[i].type_index, test_pictures[i].image);
	//}
}

void VisionNN::save_network(std::string path)
{
	std::string mlp_path = path + std::string("//mlp.yaml");
	std::cout << "Saving Neural Network at: " << mlp_path << std::endl;
	mlp->save(mlp_path);
	cv::FileStorage fs(path + "//data.yaml", cv::FileStorage::WRITE);
	for (int i = 0; i < types.size(); i++)
	{
		fs << "var" + std::to_string(i) << types[i];
	}
	fs.release();
}

void VisionNN::load_network(std::string path)
{
	mlp = cv::ml::ANN_MLP::create();
	std::string mlp_path = path + std::string("//mlp.yaml");
	std::cout << "Loading neural network at: " << mlp_path << std::endl;
	mlp = mlp->load(mlp_path);
	types = std::vector<std::string>();
	cv::FileStorage fs(path + "//data.yaml", cv::FileStorage::READ);
	int counter = 0;
	while (fs["var" + std::to_string(counter)].empty())
	{
		std::string element;
		fs["var" + std::to_string(counter)] >> element;
		types.push_back(element);
	}
	fs.release();
}

void VisionNN::save_image(int type, cv::Mat image)
{
	imwrite("C://opencv//test//" + std::to_string(type) + std::string(".jpg"), image);
}


VisionNN::~VisionNN()
{
}
