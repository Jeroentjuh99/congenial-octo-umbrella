#pragma once

#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "blobdetectionavans.h"

class Week2
{
public:
	Mat image;

	Week2();
	void Opgave1();
	void Opgave2();
	void Opgave3(Mat);
	void Opgave4();
	~Week2();
};

