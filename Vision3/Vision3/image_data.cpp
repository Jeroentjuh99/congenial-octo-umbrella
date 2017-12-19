#include "image_data.h"



image_data::image_data(cv::Mat image)
{
	image_data::image = image;
}

double image_data::getBendingEnergy()
{
	return 0.0;
}

double image_data::getObjectSize()
{
	return 0.0;
}

double image_data::getHue()
{
	return 0.0;
}

double image_data::get_ratio()
{
	return 0.0;
}


image_data::~image_data()
{
}
