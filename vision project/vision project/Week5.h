#pragma once
#include <vector>
#include <opencv2/core.hpp>
#include <opencv/cv.hpp>

class Week5
{
public:
	Week5();
	~Week5();
	void Kerstballen();
	std::vector<cv::Mat> TransformCorrections(cv::Mat src);
};

