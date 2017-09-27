//Standard c++ includes
#include "stdafx.h"
#include <iostream>

//OpenCV includes
#include "Week2.h"

int main(int argc, char* argv[]) {

	Week2 week2 = Week2();
	week2.Opgave4();
	// Wacht op een muiskklik in het window van de afbeelding
	waitKey(0);

	// Gebruikersinformatie
	cout << "ROI is getoond op het scherm " << endl;
	cout << "Druk op ENTER voor vervolg ==> " << endl;
	string dummy;

	getline(cin, dummy);

	return 0;
}