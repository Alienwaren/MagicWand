#include "pch.h"
#include "ColorDetector.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


ColorDetector::ColorDetector(int h, int s, int v, int h_l, int s_l, int v_l)
	: h_(h), s_(s), v_(v), h_l_(h_l), s_l_(s_l), v_l_(v_l)
{
}

cv::Mat ColorDetector::detect_color(const cv::Mat frame)
{
	cv::Mat hsv_frame;
	cv::Mat hsv_frame_thresholded;
	cvtColor(frame, hsv_frame, cv::COLOR_BGR2HSV);
	inRange(hsv_frame, cv::Scalar(h_l_, s_l_, v_l_), cv::Scalar(h_, s_, v_), hsv_frame_thresholded);

	//morphological opening (remove small objects from the foreground)
	erode(hsv_frame_thresholded, hsv_frame_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	dilate(hsv_frame_thresholded, hsv_frame_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

	//morphological closing (fill small holes in the foreground)
	dilate(hsv_frame_thresholded, hsv_frame_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	erode(hsv_frame_thresholded, hsv_frame_thresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

	return hsv_frame_thresholded;
}

std::tuple<float, float> ColorDetector::make_image_moments(cv::Mat& target_frame)
{
	cv::Moments img_moments = moments(target_frame);
	const double dM01 = img_moments.m01;
	const double dM10 = img_moments.m10;
	const double dArea = img_moments.m00;

	if (dArea > 10000)
	{
		//calculate the position of the ball
		float posX = dM10 / dArea;
		float posY = dM01 / dArea;

		return std::make_tuple(posX, posY);
	}
	return std::make_tuple(-1, -1);
}

void ColorDetector::set_hsv(int h, int s, int v, int h_l, int s_l, int v_l)
{
	h_ = h > 0 ? h : 0;
	s_ = s > 0 ? s : 0;
	v_ = v > 0 ? v : 0;
	h_l_ = h_l > 0 ? h_l : 0;
	s_l_ = s_l > 0 ? s_l : 0;
	v_l_ = v_l > 0 ? v_l : 0;
}

ColorDetector::~ColorDetector()
= default;
