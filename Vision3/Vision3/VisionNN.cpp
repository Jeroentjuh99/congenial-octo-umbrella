﻿#include "stdafx.h"
#include "VisionNN.h"
#include <iostream>
#include <sstream>
#include <opencv\highgui.h>
#include <windows.h>
#include <fstream>
#include <experimental\filesystem>
#include <istream>
#include <math.h>

VisionNN::VisionNN()
{
}


/**
* \brief This function captures a list of images
* \param is_for_training True if used for training
*/
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

/**
* \brief Loads images from specified folder, also enables tagging when training
* \param path The path of the folder with images
* \param training true if using training mode
*/
void VisionNN::load_images(std::string path, bool training)
{
	std::cout << "Loading images from folder " << path << std::endl;
	image_data* data = new image_data();
	const cv::Size size(720, 640);
	std::vector<image_data::Image_Features> features;
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
	{
		std::stringstream filename;
		filename << p;
		cv::Mat MATimage = cv::imread(filename.str(), CV_LOAD_IMAGE_COLOR);

		cv::resize(MATimage, MATimage, size);//resize image

		data->create_features(MATimage, features, training);
	}
		
	test_pictures = features;
	delete data;
}

/**
* \brief This function will train the Multi Layered Perceptron using the current image data and classification
* \param error_percentage EPS for the minimum error percentage before stopping the training program
* \param max_iteraties Maximum amount of iterations before ending the training program
* \param hidden_neurons Amount of neurons per hidden layer
*/
void VisionNN::train(double error_percentage, int max_iteraties, int hidden_neurons)
{
	//Initialize input and class matrices
	std::cout << "Starting training" << std::endl;
	int outputNeurons = 1;
	int feature_count = test_pictures[0].feature_descriptors.size();
	cv::Mat picture_data = cv::Mat::zeros(test_pictures.size(), feature_count, CV_32FC1);
	cv::Mat train_classes = cv::Mat::zeros(picture_data.rows, outputNeurons, CV_32FC1);
	types = std::vector<std::string>();
	//Fill picture data with features from the test pictures
	for (int i = 0; i < picture_data.rows; i++)
	{
		for (int j = 0; j < picture_data.cols; j++)
		{
			if (test_pictures[i].feature_descriptors.size() <= 0) break;
			picture_data.at<float>(i,j) = test_pictures[i].feature_descriptors[j];
		}
	}
	//Fill answers with expected types
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
	//Configure the MLP with given settings
	mlp = cv::ml::ANN_MLP::create();
	if (hidden_neurons < 2) { hidden_neurons = 2; }
	std::vector<int> layers = { picture_data.cols, hidden_neurons, hidden_neurons, train_classes.cols }; //All the layers of the mlp (number means amount of neurons)
	mlp->setLayerSizes(layers);
	mlp->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, error_percentage); //Configures mlp to use backpropagation
	cv::TermCriteria termCrit = cv::TermCriteria(cv::TermCriteria::Type::MAX_ITER | cv::TermCriteria::Type::EPS, max_iteraties, error_percentage); //Configures Term Criteria
	mlp->setTermCriteria(termCrit);
	mlp->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM); //Symmetrical sigmoid : f(x) = β∗(1−e^(−αx)) / (1 + e^(−αx))

	mlp->train(picture_data, cv::ml::ROW_SAMPLE, train_classes); //Trains the mlp using the rows as input
	verify_objects(picture_data, train_classes); //Verify last training
}

/**
* \brief This function will verify the output the last training using the answers and picture data
* \param picture_data The features of the detected pictures
* \param train_classes The answers/types of the trained pictures
*/
void VisionNN::verify_objects(cv::Mat picture_data, cv::Mat train_classes)
{
	float accuracy = 0;
	cv::Mat predictedMat = cv::Mat::zeros(train_classes.rows, train_classes.cols, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++) {
		std::vector<float> predicted = std::vector<float>();
		mlp->predict(picture_data.row(i), predicted);  //asks the mlp for output for the given features
		//Adds predicted data to Mat object
		for (int x = 0; x < predicted.size(); x++)
		{
			predictedMat.at<float>(i, x) = round(predicted[x]);
			float difference = predicted[x] - train_classes.at<float>(i, x);
			if (difference < 0) { difference = -difference; }
			accuracy += difference;
		}
	}
	//Calculates accuracy using the output of the AI
	accuracy = 1 - (accuracy / (predictedMat.cols * predictedMat.rows * types.size() - 1));

	//Writes debug data to the console
	std::cout << "Input: " << std::endl;
	std::cout << picture_data << std::endl << std::endl;
	for (int i = 0; i < predictedMat.rows; i++)
	{
		int index = (int) train_classes.at<float>(i, 0);
		std::string typeName;
		if (index >= types.size())
		{
			typeName = std::to_string(index);
		}
		else
		{
			typeName = types[index];
		}
		test_pictures[i].type_index = index;
		std::cout << "Uitkomst na training: " << predictedMat.row(i) << " Verwachte Output: " << train_classes.row(i) << " (" << typeName << ")" << std::endl;
	}
	std::cout << "Accuracy: " << std::endl;
	std::cout << accuracy * 100 << std::endl << std::endl;

	//for (int i = 0; i < test_pictures.size(); i++)
	//{
	//	save_image(test_pictures[i].type_index, test_pictures[i].image);
	//}
}

/**
* \brief Prints output of the Multi Layered Perceptron when using the current loaded images as input
* \param output_data Mat with output data of the perceptron
* \param nrOfOutputCols Amount of output columns in the output mat object
*/
void VisionNN::get_objects(cv::Mat output_data, int nrOfOutputCols)
{
	//Parse features in picture data Mat object
	int feature_count = test_pictures[0].feature_descriptors.size();
	cv::Mat picture_data = cv::Mat::zeros(test_pictures.size(), feature_count, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++)
	{
		for (int j = 0; j < picture_data.cols; j++)
		{
			if (test_pictures[i].feature_descriptors.size() <= 0) break;
			picture_data.at<float>(i, j) = test_pictures[i].feature_descriptors[j];
		}
	}
	cv::Mat predictedMat = cv::Mat::zeros(picture_data.rows, nrOfOutputCols, CV_32FC1);
	for (int i = 0; i < picture_data.rows; i++) {
		std::vector<float> predicted = std::vector<float>();
		mlp->predict(picture_data.row(i), predicted);
		for (int x = 0; x < predicted.size(); x++)
		{
			predictedMat.at<float>(i, x) = round(predicted[x]);
		}
	}
	output_data = predictedMat;
	//Output debug data to console
	std::cout << "Input: " << std::endl;
	std::cout << picture_data << std::endl << std::endl;
	for (int i = 0; i < predictedMat.rows; i++)
	{
		int index = round(predictedMat.at<float>(i, 0));
		std::string typeName;
		if (index >= types.size())
		{
			typeName = std::to_string(index);
		}
		else
		{
			typeName = types[index];
		}
		std::cout << "Uitkomst: " << predictedMat.row(i) << " (" << typeName << ")" << std::endl;
	}

	//for (int i = 0; i < test_pictures.size(); i++)
	//{
	//	save_image(test_pictures[i].type_index, test_pictures[i].image);
	//}
}

/**
* \brief Saves current network and type information at specified path
* \param path Path of the folder to save the network to
*/
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

/**
* \brief Loads perceptron and type data using the given path to the directory
* \param path Folder of perceptron and type data
*/
void VisionNN::load_network(std::string path)
{
	mlp = cv::ml::ANN_MLP::create();
	std::string mlp_path = path + std::string("//mlp.yaml");
	FILE *file;
	if(!fopen_s(&file, mlp_path.c_str(), "r")) {
		fclose(file);
	}
	else {
		std::cout << "Error loading neural network at: " << mlp_path << std::endl;
		return;
	}
	std::cout << "Loading neural network at: " << mlp_path << std::endl;
	mlp = mlp->load(mlp_path);
	types = std::vector<std::string>();
	cv::FileStorage fs;
	fs.open(path + "//data.yaml", cv::FileStorage::READ);
	if(!fs.isOpened()){ std::cout << "Error loading type data at: " << path << std::endl; return; }
	int counter = 0;
	while (!fs["var" + std::to_string(counter)].empty())
	{
		std::string element;
		fs["var" + std::to_string(counter)] >> element;
		types.push_back(element);
		counter++;
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
