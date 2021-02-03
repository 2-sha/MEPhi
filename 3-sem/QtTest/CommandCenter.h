#pragma once

#include "Platform.h"
#include "Coord.h"

namespace robots
{
	class CommandCenter : public Platform
	{
	public:
		CommandCenter(
			unsigned consumption,
			unsigned slotsNum,
			unsigned price
		) : Platform(consumption, slotsNum, price) {};
	};
}