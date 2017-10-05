// Demonstratie: gebruik van callibratie matrices 
// - inlezen van een YML-file met de callibratie data van de camera
// - testen van de callibratie waarden door beelden op te nemen met 
//   de camera, het orginele beeld te tonen en het gecallibreerde beeld.
// Opmerking: met een lens met een kleine brandpuntsafstand, b.v. 6 mm
//            is het effect goed te zien.
// Jan Oostindie, dd 30-9-2015

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	// YML-file met callibratie data openen
	FileStorage fs("b:\\test\\ueyecallib.yml", FileStorage::READ);

	// callibratie data ophalen
	Mat intrinsic, distCoeffs;
	fs["intrinsic"] >> intrinsic;
	fs["distCoeffs"] >> distCoeffs;

	// callibratie matrices tonen op het scherm
	cout << "intrinsic matrix: " << intrinsic << endl;
	cout << "distortion coeffs: " << distCoeffs << endl;

	// sluiten van de YML-file
	fs.release();
	
	/***** callibratie data gebruiken *****/

	// Now that we have the distortion coefficients, we can undistort the images. 
	// Here's a small loop that will do this: 
	Mat imageUndistorted;

	VideoCapture capture = VideoCapture(1);
	Mat image;
	while (1)
	{
		capture >> image;
		undistort(image, imageUndistorted, intrinsic, distCoeffs);

		imshow("original", image);
		imshow("undistorted", imageUndistorted);
		waitKey(1);
	}

	waitKey();
	cin.get();
}
