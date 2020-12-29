#include "Robot.h"

void robots::Robot::move()
{
	if (route_.empty())
		return;
	try
	{
		setPos(route_.top());
	}
	catch (std::exception& ex)
	{
		// Clear the queue
		std::swap(route_, std::stack<Coord>());
		throw ex;
	}
	route_.pop();
}
