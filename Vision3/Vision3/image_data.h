#pragma once

#include <opencv/cv.h>

class image_data {
public:
	/**
	 * \brief The information struct of the features of the items in the image
	 * \param type The type name of the item
	 * \param feature_descriptors Vector of double which are all the descriptors we use about the item
	 */
	struct Image_Features {
		std::string type = "";
		std::vector<double> feature_descriptors;
		int type_index;
	};

	image_data();
	void create_features( cv::Mat& image, std::vector<Image_Features>& features, bool is_training = true );
	~image_data();

private:
	std::string last_item = "";
	void assign_type( Image_Features& );
	cv::Vec3b mycolor = cv::Vec3b( 150, 100, 255 );
	void cut_preview( cv::Mat& input, cv::Rect& bounding_box, std::vector<cv::Point>& contour ) const;
	void create_contours( cv::Mat& image, cv::Mat& canny_output, std::vector<std::vector<cv::Point>>& contours,
	                      std::vector<cv::Vec4i>& hierarchy, int threshold_value ) const;
};
