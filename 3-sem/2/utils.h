#pragma once
#define _USE_MATH_DEFINES // Для доступа к M_PI
#include <math.h>
#include <stdexcept>
#include <string>
#include <utility> // pair

class LinearStrophoid
{
private:
	double a;

public:
	static constexpr double ABS_ERROR = 0.000000000001;

	LinearStrophoid();
	LinearStrophoid(double a);

	int getA();
	void setA(double a);

	std::pair<double, double> calcInCartesian(double x);
	double calcInPolar(double angle);

	double getRadiusOfCurvature();
	double getLoopArea();
	double getLoopVolume();
	double getAsymptoteArea();
};