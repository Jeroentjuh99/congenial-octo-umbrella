/* Func: Het principe van de opslag van callibratie data en het later
         weer inlezen van deze data.
   uitleg: http://docs.opencv.org/modules/core/doc/xml_yaml_persistence.html
   Jan Oostindie
   dd 30-9-2015
*/

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;


int main(int, char** argv)
{
    // YML-file aanmaken
	FileStorage fs("b:\\test.yml", FileStorage::WRITE);

	// voorbeeld van callibratie data
	Mat cameraMatrix = (Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
	Mat distCoeffs = (Mat_<double>(5, 1) << 0.1, 0.01, -0.001, 0, 0);
	
	// wegschrijven van callibratie data naar de YML-file 
	fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;
	
	// de file afsluiten
	fs.release();

	// YML-file openen
	FileStorage fs2("b:\\test.yml", FileStorage::READ);

	// callibratie data ophalen
	Mat cameraMatrix2, distCoeffs2;
	fs2["cameraMatrix"] >> cameraMatrix2;
	fs2["distCoeffs"] >> distCoeffs2;

	// callibratie matrices tonen op het scherm
	cout << "camera matrix: " << cameraMatrix2 << endl;
	cout << "distortion coeffs: " << distCoeffs2 << endl;

	// sluiten van de YML-file
	fs2.release();

	cin.get();

	return 0;
}