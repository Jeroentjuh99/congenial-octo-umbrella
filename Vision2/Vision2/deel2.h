#pragma once

#include <vector>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class deel2
{
public:
	deel2();
	int enclosedPixels(const vector<Point>&contourVec, vector<Point>&regionPixels, int, int);
	void testEnclosedPixels();
	int CutTrainingSet(const std::string image, const std::string classifier);
	~deel2();

private:
	void flood_fill(Mat image, Point startPoint, int replacement_color);
};

