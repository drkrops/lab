#include "pApproximation.h"

void approximatePoly::makeApproximationUtil(std::vector<point>& segment) {
	if (segment.size() < 1)
		return;

	std::vector<point>::iterator furthest = findFurthest(segment);
	std::vector<point>::iterator v0 = segment.begin(), vn = segment.end() - 1;
	if (getDistance(*v0, *vn, *furthest) < accuracy)
		return;

	std::vector<point>::iterator insert_pos = findInApprPoints(v0);
	points_in_approximation.insert(insert_pos, *furthest);

	std::vector<point> left_segment(v0, furthest);
	makeApproximationUtil(left_segment);

	std::vector<point> right_segment(furthest, vn);
	makeApproximationUtil(right_segment);
}


std::vector<point>::iterator approximatePoly::findFurthest(std::vector<point>& segment) {
	point v0 = segment[0], vn = segment[segment.size() - 1];

	float max_distance = 0;
	auto furthest_point = segment.begin();
	for (auto it = segment.begin(); it != segment.end(); it++) {
		float temp_d = getDistance(v0, vn, *it);
		if (temp_d > max_distance) {
			max_distance = temp_d;
			furthest_point = it;
		}
	}

	return furthest_point;
}

float approximatePoly::getDistance(point p1, point p2, point p) {
	// Функция возвращает расстояние от точки p до прямой, проведённой через точки p1, p2

	// y = kx + m
	float k, m;
	k = (p2.y - p1.y) / (p2.x - p1.x);
	m = p2.y - k * p2.x;

	// ax + by + c = 0; distance = |ax0 + by0 + c| / sqrt(a^2 + b^2)
	return std::abs(k * p.x - p.y + m) / std::sqrt(k * k + 1); // b = -1;
}

std::vector<point>::iterator approximatePoly::findInApprPoints(std::vector<point>::iterator it) {
	for (std::vector<point>::iterator ret = points_in_approximation.begin(); ret != points_in_approximation.end(); ret++)
		if (*ret == *it)
			return ret;
	return points_in_approximation.end();
}

approximatePoly::approximatePoly(std::vector<point> vertexes, float accuracy) {
	for (auto it = vertexes.begin(); it != vertexes.end(); it++)
		all_points.push_back(*it);
	this->accuracy = accuracy;
}

void approximatePoly::makeApproximation() {
	points_in_approximation.clear(); // на случай, если аппроксимируется не в первый раз
	points_in_approximation.push_back(all_points[0]);
	makeApproximationUtil(all_points);
	points_in_approximation.push_back(all_points[all_points.size() - 1]);
}

std::vector<point> approximatePoly::getPoints() {
	return points_in_approximation;
}