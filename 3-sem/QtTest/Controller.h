#pragma once

#include "Module.h"

#include <memory>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <list>

namespace robots
{
	class BaseRequest;
	class Plarform;
	class Robot;
	class Coord;
	class Map;

	class Controller : public Module, public std::enable_shared_from_this<Controller>
	{
	private:
		unsigned actionRange_;
		unsigned maxRobotsNum_;
		unsigned robotsNum_;
		bool canBeManaged_ = false;

		std::weak_ptr<Controller> manager_;
		std::vector<std::weak_ptr<Platform>> inferiors_;
		std::vector<std::weak_ptr<Robot>> robots_;

		std::unordered_set<Coord> toExplore_;
		std::unordered_set<Coord> busyFields_;

		void initExploration();
		void explore();
		void scan();


	public:
		Controller(unsigned consumption, unsigned price, unsigned maxRobotsNum, unsigned actionRange, 
			std::shared_ptr<Platform> host = nullptr)
			: Module(consumption, price, host), maxRobotsNum_(maxRobotsNum), actionRange_(actionRange) {}

		bool work(BaseRequest& request) override;
		bool hasTask() { return !toExplore_.empty(); }

		void disableManagment() { canBeManaged_ = true; }
		inline void setManager(std::shared_ptr<Controller> manager) { manager_ = manager; canBeManaged_ = false; }
		inline std::weak_ptr<Controller> getManager() { return canBeManaged_ ? std::weak_ptr<Controller>() : manager_; }

		std::stack<Coord> makeRoute(const Coord& start, const Coord& end);

		inline unsigned getMaxRobotsNum() { return maxRobotsNum_; }
		inline unsigned getActionRange() { return actionRange_; }
	};
}