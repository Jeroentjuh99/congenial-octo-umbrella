//Standard c++ includes
#include "stdafx.h"
#include <iostream>

//OpenCV include
#include "Week2.h"
#include "Week3.h"
#include "RadiusDetection.h"
#include "Week5.h"
#include "Week6.h"

int main(int argc, char* argv[]) {

	//Week2 week2 = Week2();
	//week2.Opgave4();

//	RadiusDetection detect = RadiusDetection();
//	detect.DetectRadius();

	Week5 week5 = Week5();
	//week5.HSVScheiding("C:\\Users\\jeroe\\Desktop\\stuff\\wallpapers\\coffee.jpg");
	week5.Kerstballen();
	//week5.KerstballenOnderCamera();
		
//	Week6 week6 = Week6();
//	week6.Opdracht2();
	
	return 0;
}