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
	std::cout << "loading images" << std::endl;
	nn.load_images("C://opencv//trainingsset");
	std::cout << "starting training" << std::endl;
	nn.train(0.000001, 10000, 100);

	return 0;
}
