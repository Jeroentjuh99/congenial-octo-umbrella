// Demo: Training of a Neural Network / Back-Propagation algorithm 
// Jan Oostindie, Avans Hogeschool, dd 6-12-2016
// email: jac.oostindie@avans.nl

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "VisionNN.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VisionNN neural_network = VisionNN();
	neural_network.load_images("C:/opencv/trainingsset");
}

