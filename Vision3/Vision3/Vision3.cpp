#include "stdafx.h"
#include "perceptron.h"
#include "BPN.h"

bool USE_NEW_CAL_FILE = true;

int main( int argc, char* argv[] ) {
	perceptron* p = new perceptron();
	//BPNinit* p = new BPNinit();
	delete p;
	return 0;
}
