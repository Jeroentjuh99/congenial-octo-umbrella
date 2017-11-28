#pragma once

#include <vector>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class deel2
{
public:
	deel2();
	void calibrate_camera(bool);
	int enclosedPixels(const vector<Point>&contourVec, vector<Point>&regionPixels);
	void testEnclosedPixels();
	~deel2();

private:
	void create_calibration_file();
	void recursiveFiller(Mat image, Point startPos, int currentWave);
};

