#pragma once

#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "blobdetectionavans.h"

class Week3
{
public:

	Mat image;

	Week3();
	void BlobDetectie();
	void Opgave6();
	~Week3();
};
