/*! @file
 *  @brief Contains demonstration of Polynom Approximation function.
 *  @author Stanislav Ivashkov
 *  @date 13 May 2023
 */
#include <stdexcept>

#include <kernels.h>
#include <detail/types.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#include "idemo.hpp"



class demo_polyApproximation : public idemo {
public:
	demo_polyApproximation() : idemo("polyApproximation")
	{
	}
	void prepare(const std::filesystem::path& resource_dir) override {
		auto path = resource_dir / std::filesystem::path("apple.png");
		if (!std::filesystem::exists(path))
			throw std::invalid_argument("Cannot find file " + path.string());

		img = cv::imread(path.string(), cv::IMREAD_GRAYSCALE);
		img_col = cv::imread(path.string(), cv::IMREAD_COLOR);
		cv::namedWindow("original", cv::WINDOW_AUTOSIZE);
		cv::imshow("original", img);

		cv::Mat out;
		cv::threshold(img, out, 80, 255, cv::THRESH_BINARY);
		cv::imshow("threshhold (for contour finding)", out);

		std::vector<std::vector<cv::Point>> contours;
		std::vector<cv::Vec4i> hierarchy;
		std::vector<cv::Point> approximated;
		cv::findContours(out, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
		cv::Mat mask = img_col.clone();
		cv::drawContours(mask, contours, 234, cv::Scalar(0, 0, 255), 1); // 234 - костыль дл€ выделени€ контура €блока, который € не знаю как обойти
		cv::imshow("Finded contour", mask);

		contour = contours[234];

		cv::namedWindow("params", cv::WINDOW_GUI_EXPANDED);
		cv::createTrackbar("accuracy:", "params", nullptr, 8, on_change_callback, this);
	}

	void execute_opencv() override {
		cv::Mat appr_img = img_col.clone();
		cv::approxPolyDP(contour, cv_approximated, accuracy, 1);
		cv::polylines(appr_img, cv_approximated, 1, cv::Scalar(0, 0, 255), 1, 1);
		cv::imshow("Approximated OpenCV", appr_img);
	}

	void execute_openvx() override {
		std::vector<point> vx_contour;
		for (int i = 0; i < contour.size(); i++) {
			point P;
			P.x = (float)contour[i].x;
			P.y = (float)contour[i].y;
			vx_contour.push_back(P);
		}

		if (auto status = poly_approximate(vx_contour, vx_approximated, accuracy); status != VX_SUCCESS)
			throw std::runtime_error("OpenVX approximation failed: " + std::to_string(status));

		approximatePoly vx = approximatePoly(vx_contour, accuracy);
		vx.makeApproximation();
		vx_approximated = vx.getPoints();

		std::vector<cv::Point> openvx_appr_Point = convertPointToCVPoint(vx_approximated);

		cv::Mat appr_img_vx = img_col.clone();
		cv::polylines(appr_img_vx, openvx_appr_Point, 1, cv::Scalar(0, 255, 255), 1, 1);
		cv::imshow("Approximated OpenVX", appr_img_vx);
	}

	void make_diff() override {
		cv::Mat difference = cv::Mat::zeros(img.size(), CV_8UC3);
		cv::polylines(difference, cv_approximated, 1, cv::Scalar(0, 0, 255), 2, 1);

		std::vector<cv::Point> openvx_appr_Point = convertPointToCVPoint(vx_approximated);
		cv::polylines(difference, openvx_appr_Point, 1, cv::Scalar(0, 255, 255), 1, 1);

		cv::imshow("difference(two contours)", difference);
	}

private:
	//! Callback to set accuracy.
	static void on_change_callback(int value, void* data)
	{
		auto self = static_cast<demo_polyApproximation*>(data);
		self->accuracy = (float)value;
	}

	std::vector<cv::Point> convertPointToCVPoint(std::vector<point>& vect) {
		std::vector<cv::Point> ret;
		for (std::vector<point>::iterator it = vect.begin(); it != vect.end(); it++)
			ret.push_back(cv::Point((int)(*it).x, (int)(*it).y)); // т.к. x, y в point - float
		return ret;
	}

	cv::Mat img;
	cv::Mat img_col;
	std::vector<cv::Point> contour;

	float accuracy;
	std::vector<cv::Point> cv_approximated;
	std::vector<point> vx_approximated;
};

static demo_polyApproximation demo_approximation_instance;