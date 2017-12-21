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
	nn.load_images("C://neuralnetwork//trainingsset");
	nn.train(0.00001, 10000, 200);
	nn.save_network("C://neuralnetwork//mlp.yaml");

	std::cin.ignore();
	return 0;
}
