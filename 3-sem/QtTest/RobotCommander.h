#pragma once

#include "Robot.h"
#include "Coord.h"

namespace robots
{
	class RobotCommander : public Robot
	{
	public:
		RobotCommander(
			const unsigned consumption,
			const unsigned slotsNum,
			const unsigned price
		) : Robot(consumption, slotsNum, price) {};
	};
}