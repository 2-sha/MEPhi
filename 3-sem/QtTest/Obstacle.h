#pragma once

#include "MapObject.h"

#include <string>
#include <memory>

namespace robots
{

	class Obstacle : public MapObject
	{
	public:
		Obstacle() : MapObject() {};
	};
}