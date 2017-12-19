#include "stdafx.h"
#include "perceptron.h"
#include "BPN.h"
#include "VisionNN.h"

bool USE_NEW_CAL_FILE = true;

int main( int argc, char* argv[] ) {
	VisionNN nn = VisionNN();
	nn.load_images("C://opencv//trainingsset");
	nn.train(0.0001);
}
