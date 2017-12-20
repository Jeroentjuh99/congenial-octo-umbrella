#include "stdafx.h"
#include "perceptron.h"
#include "BPN.h"
#include "VisionNN.h"

bool USE_NEW_CAL_FILE = true;

int main( int argc, char* argv[] ) {
	//perceptron* p = new perceptron();
	/*BPNinit* p = new BPNinit();
	delete p;*/

	VisionNN nn;
	nn.load_images("C://opencv//trainingsset");
	nn.train(0.00001, 10000, 200);

	std::cin.ignore();
	return 0;
}
