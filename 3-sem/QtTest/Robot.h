#pragma once

#include "Platform.h"
#include "Coord.h"

#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

namespace robots
{
	class Controller;

	class Robot : public Platform
	{
	private:
		std::stack<Coord> route_;
		std::weak_ptr<Controller> manager_;

	public:
		Robot(
			unsigned consumption,
			unsigned slotsNum,
			unsigned price
		) : Platform(consumption, slotsNum, price) {};

		inline void setManager(std::shared_ptr<Controller> manager) { manager_ = manager; }
		inline std::weak_ptr<Controller> getManager() { return manager_; }

		inline void setRoute(const std::stack<Coord>& route) { route_ = route; }
		inline bool hasTask() const { return !route_.empty(); }
		void move();
	};
}