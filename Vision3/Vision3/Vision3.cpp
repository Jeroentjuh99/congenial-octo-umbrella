#include "stdafx.h"
#include "perceptron.h"
#include "BPN.h"
#include "VisionNN.h"

void nn_menu() {
	VisionNN nn;
	const double foutpercentage = 1e-10;
	int max_iterations = 10000, hidden_layers = 200;
	bool quit = false;
	while ( quit == false ) {
		std::cout << "Wat wilt u doen?" << std::endl <<
				"1. Train Neural Network met afbeeldingen uit C://neuralnetwork//trainingsset" << std::endl <<
				"2. Train Neural Network met de camera" << std::endl << "3. Laad vorig Neural Network in" << std::endl <<
				"4. Test abeeldingen uit C://neuralnetwork//test" << std::endl << "5. Test objecten onder de camera" << std::endl <<
				"6. Sluiten" << std::endl;
		int keuze;
		std::cin >> keuze;
		std::cout << std::endl;
		switch ( keuze ) {
			case 6:
				quit = true;
				break;
			case 1:
				std::cout << "Hoeveel iteraties? Standaard is " << std::to_string( max_iterations ) << std::endl;
				std::cin >> max_iterations;
				std::cout << "Hoeveel hidden neuronen in de layers? Standaard is " << std::to_string( hidden_layers ) << ", de minimale waarde is 2." << std::endl;
				std::cin >> hidden_layers;
				nn.load_images( "C://neuralnetwork//trainingsset");
				nn.train( foutpercentage, max_iterations, hidden_layers );
				nn.save_network( "C://neuralnetwork" );
				break;
			case 2:
				std::cout << "Hoeveel iteraties? Standaard is " << std::to_string( max_iterations ) << std::endl;
				std::cin >> max_iterations;
				std::cout << "Hoeveel hidden neuronen? Standaard is " << std::to_string( hidden_layers ) << ", de minimale waarde is 2." << std::endl;
				std::cin >> hidden_layers;
				nn.capture_image();
				nn.train( foutpercentage, max_iterations, hidden_layers );
				nn.save_network( "C://neuralnetwork" );
				break;
			case 3:
				nn.load_network( "C://neuralnetwork" );
				break;
			case 4:
				nn.load_images("C://neuralnetwork//test", false);
				nn.load_network("C://neuralnetwork");
				nn.get_objects(cv::Mat(), 1);
				break;
			case 5:
				nn.capture_image( false );
				nn.get_objects(cv::Mat(), 1);
				break;
			default:
				break;
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

int main( int argc, char* argv[] ) {
	//perceptron* p = new perceptron();
	/*BPNinit* p = new BPNinit();
	delete p;*/

	nn_menu();
	std::cin.ignore();
	return 0;
}
