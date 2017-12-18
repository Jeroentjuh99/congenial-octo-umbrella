// Demo: Training of a Neural Network / Back-Propagation algorithm 
// Jan Oostindie, Avans Hogeschool, dd 6-12-2016
// email: jac.oostindie@avans.nl

#include "stdafx.h"
#include <opencv2/core.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include "avansvisionlib20.h" // versie 2.0 (!)
#include <iomanip>
#include "BPN.h"

using namespace cv;
using namespace std;

// Maximale fout die toegestaan wordt in de output voor de training input
const double MAX_OUTPUT_ERROR = 1E-10;

// maximaal aantal runs dat uitgevoerd wordt bij het trainen
const int MAXRUNS = 10000;

BPNinit::BPNinit() {
	// IT, OT: input trainingset, output trainingset
	Mat ITset, OTset;

	// V0, W0   : weightfactor matrices
	// dV0, dW0 : weightfactor correction matrices
	Mat V0, W0, dW0, dV0;

	// default number of hiddenNeurons. The definite number is user input  
	// inputNeurons and outputNeurons are implicitly determined via
	// the trainingset, i.e.: inputNeurons = ITset.cols ; outputNeurons = OTset.cols;
	int hiddenNeurons = 2;

	cout << endl << "Load testtrainingset..." << endl << endl;
	loadTrainingSet1( ITset, OTset );
	//loadBinaryTrainingSet1(ITset, OTset);

	cout << "Training Input " << endl << endl;
	cout << ITset << endl << endl;
	cout << "Training Output " << endl << endl;
	cout << OTset << endl << endl;

	cout << " ===> BPN format: " << endl <<
			"BPN Inputlayer  = " << ITset.cols << "  neurons" << endl <<
			"BPN Outputlayer = " << OTset.cols << "  neurons" << endl << endl;
	cout << "Please choose a number of hidden neurons: ";
	cin >> hiddenNeurons;
	cout << "Thank you!" << endl << endl << endl;

	cout << "Initialize BPN ..." << endl;
	initializeBPN( ITset.cols, hiddenNeurons, OTset.cols, V0, dV0, W0, dW0 );
	//testBPN(ITset, OTset, V0, dV0, W0, dW0);

	cout << "initial values of weight matrices V0 and W0" << endl;
	cout << "*******************************************" << endl;
	cout << V0 << endl << endl << W0 << endl << endl;
	cout << "Press ENTER => ";
	string dummy;
	getline( cin, dummy );
	getline( cin, dummy );

	// IT: current training input of the inputlayer 
	// OT: desired training output of the BPN
	// OH: output of the hiddenlayer
	// OO: output of the outputlayer
	Mat IT, OT, OH, OO;

	// outputError0: error on output for the current input and weighfactors V0, W0
	// outputError1: error on output for the current input and new calculated 
	//               weighfactors, i.e. V1, W1
	double outputError0, outputError1, sumSqrDiffError = MAX_OUTPUT_ERROR + 1;
	Mat V1, W1;

	int runs = 0;
	while ( ( sumSqrDiffError > MAX_OUTPUT_ERROR ) && ( runs < MAXRUNS ) ) {

		sumSqrDiffError = 0;

		for ( int inputSetRowNr = 0; inputSetRowNr < ITset.rows; inputSetRowNr++ ) {

			IT = transpose( getRow( ITset, inputSetRowNr ) );

			OT = transpose( getRow( OTset, inputSetRowNr ) );

			calculateOutputHiddenLayer( IT, V0, OH );

			calculateOutputBPN( OH, W0, OO );

			adaptVW( OT, OO, OH, IT, W0, dW0, V0, dV0, W1, V1 );

			calculateOutputBPNError( OO, OT, outputError0 );

			calculateOutputBPNError( BPN( IT, V1, W1 ), OT, outputError1 );

			sumSqrDiffError += ( outputError1 - outputError0 ) * ( outputError1 - outputError0 );

			V0 = V1;
			W0 = W1;
		}
		cout << "sumSqrDiffError = " << sumSqrDiffError << endl;
		runs++;
	}

	cout << "BPN Training is ready!" << endl << endl;
	cout << "Runs = " << runs << endl << endl;

	Mat inputVectorTrainingSet, outputVectorTrainingSet, outputVectorBPN;

	// druk voor elke input vector uit de trainingset de output vector uit trainingset af 
	// tezamen met de output vector die het getrainde BPN (zie V0, W0) genereerd bij de 
	// betreffende input vector.
	cout << setw( 16 ) << " " << "Training Input" << setw( 12 ) << "|" << " Expected Output "
			<< setw( 1 ) << "|" << " Output BPN " << setw( 6 ) << "|" << endl << endl;
	for ( int row = 0; row < ITset.rows; row++ ) {

		// haal volgende inputvector op uit de training set
		inputVectorTrainingSet = transpose( getRow( ITset, row ) );

		// druk de inputvector af in een regel afgesloten met | 
		for ( int r = 0; r < inputVectorTrainingSet.rows; r++ )
			cout << setw( 8 ) << getEntry( inputVectorTrainingSet, r, 0 );
		cout << setw( 2 ) << "|";

		// haal bijbehorende outputvector op uit de training set
		outputVectorTrainingSet = transpose( getRow( OTset, row ) );

		// druk de outputvector van de training set af in dezelfde regel afgesloten met | 
		for ( int r = 0; r < outputVectorTrainingSet.rows; r++ )
			cout << setw( 8 ) << round( getEntry( outputVectorTrainingSet, r, 0 ) );
		cout << setw( 2 ) << "|";

		// bepaal de outputvector die het getrainde BPN oplevert 
		// bij de inputvector uit de trainingset  
		outputVectorBPN = BPN( inputVectorTrainingSet, V0, W0 );

		// druk de output vector van het BPN af in dezelfde regel afgesloten met |
		for ( int r = 0; r < outputVectorBPN.rows; r++ )
			cout << setw( 8 ) << round( getEntry( outputVectorBPN, r, 0 ) );
		cout << setw( 2 ) << "|";

		cout << endl;
	}

	cout << endl << endl << "Press ENTER for exit";
	getline( cin, dummy );
}
