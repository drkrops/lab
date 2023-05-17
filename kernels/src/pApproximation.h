#include <iostream>
#include <vector>
#include <cmath>

// Структура для удобства работы с точками для аппроксимации
struct point {
	float x, y;

	bool operator ==(const point p) const{
		return (x == p.x && y == p.y);
	}
};

class approximatePoly {
private:
	std::vector<point> all_points;
	std::vector<point> points_in_approximation;
	float accuracy;

	void makeApproximationUtil(std::vector<point>& segment);
	std::vector<point>::iterator findFurthest(std::vector<point>& segment);
	float getDistance(point p1, point p2, point p);
	std::vector<point>::iterator findInApprPoints(std::vector<point>::iterator it);

public:
	approximatePoly(std::vector<point> vertexes, float accuracy);
	void makeApproximation();
	std::vector<point> getPoints();
};