#pragma once
#include <vector>
#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include <string>

class Week5
{
public:
	Week5();
	~Week5();
	void HSVScheiding(std::string imagePath);
	void Kerstballen();
	void KerstballenOnderCamera();


private:
	void TelKleuren(cv::Mat image);
};

