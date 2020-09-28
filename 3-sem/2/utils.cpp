#include "utils.h"

LinearStrophoid::LinearStrophoid()
	: a(5)
{
}

LinearStrophoid::LinearStrophoid(double a)
	: a(a) 
{
}

int LinearStrophoid::getA()
{
	return this->a;
}

void LinearStrophoid::setA(double a)
{
	this->a = a;
}

std::pair<double, double> LinearStrophoid::calcInCartesian(double x)
{
	if (abs(this->a - x) < LinearStrophoid::ABS_ERROR)
		return std::make_pair(0, 0);
	if (x < -a || x > a)
		throw std::runtime_error("Функция определна на [-" + std::to_string(this->a) + "; " + std::to_string(this->a) + "]");
	double val = x * sqrt((this->a + x) / (this->a - x));
	return std::make_pair(val, -val);
}

double LinearStrophoid::calcInPolar(double angle)
{
	if (abs(cos(angle)) < LinearStrophoid::ABS_ERROR)
		return 0;
	return - (a * cos(angle * 2)) / cos(angle);
}

double LinearStrophoid::getRadiusOfCurvature()
{
	return a * sqrt(2);
}

double LinearStrophoid::getLoopArea()
{
	return pow(a, 2) * (2.0 - M_PI / 2.0);
}

double LinearStrophoid::getLoopVolume()
{
	return pow(a, 3) * M_PI * (2.0 * M_LN2 - 4.0 / 3.0);
}

double LinearStrophoid::getAsymptoteArea()
{
	return pow(a, 2) * (2.0 + M_PI / 2.0);
}
