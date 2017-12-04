#pragma once
#include <opencv2/core.hpp>

class CameraCalibration {
public:
	CameraCalibration();
	void calibrate_camera( bool );
	void undistort_view( cv::Mat*, cv::Mat* );
	~CameraCalibration();

private:
	void create_calibration_file() const;
	cv::Mat intrinsic, distCoeffs;
};
