#pragma once

#include <opencv/cv.h>

class image_data {
public:
	struct Image_Features {
		std::string type = "";
		std::vector<double> featureColumncounted;
	};

	image_data();
	void create_features( cv::Mat& image, std::vector<Image_Features>& features, bool is_training = true );
	~image_data();
private:
	cv::Mat image;
	std::string last_item = "";
	void assign_type( Image_Features& );
	cv::Vec3b mycolor = cv::Vec3b( 150, 100, 255 );
	void cut_preview( cv::Mat& input, cv::Rect& bounding_box, std::vector<cv::Point>& contour ) const;
	void create_contours( cv::Mat& image, cv::Mat& canny_output, std::vector<std::vector<cv::Point>>& contours,
	                      std::vector<cv::Vec4i>& hierarchy, int threshold_value ) const;
};
