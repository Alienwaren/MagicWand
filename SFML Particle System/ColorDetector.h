#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H
#include <opencv2/core/mat.hpp>


class ColorDetector
{
public:
	ColorDetector(int h, int s, int v, int h_l, int s_l, int v_l);
	cv::Mat detect_color(cv::Mat frame);
	std::tuple<float, float> make_image_moments(cv::Mat& target_frame);
	void set_hsv(int h, int s, int v, int h_l, int s_l, int v_l);
	~ColorDetector();
private:
	int h_ = 0;
	int s_ = 0;
	int v_ = 0;
	int h_l_ = 0;
	int s_l_ = 0;
	int v_l_ = 0;
	double last_x_ = 0;
	double last_y_ = 0;
};

#endif /COLORDETECTOR_H
