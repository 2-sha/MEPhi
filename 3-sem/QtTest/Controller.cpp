#include "Controller.h"
#include "ControllerRequest.h"
#include "SensorRequest.h"
#include "Robot.h"
#include "Coord.h"
#include "Obstacle.h"
#include "CommandCenter.h"

#include <queue>

#include <QDebug>

bool robots::Controller::work(BaseRequest& request)
{
	if (typeid(request) == typeid(FindControllersRequest))
	{
		scan();
		return true;
	}

	if (typeid(request) == typeid(ConnectRequest))
	{
		if (!canBeManaged_ && manager_.lock() == nullptr)
		{
			ConnectRequest& connectRequest = static_cast<ConnectRequest&>(request);
			connectRequest.connect(host_);
			setManager(connectRequest.getSender());
			// TODO: Merge visited
		}
		return true;
	}

	if (typeid(request) == typeid(InitExplorationRequest))
	{
		initExploration();
		return true;
	}

	if (typeid(request) == typeid(ExploreRequest))
	{
		explore();
		if (!hasTask())
			static_cast<ExploreRequest&>(request).endExplore();
		return true;
	}

	return false;
}

void robots::Controller::scan()
{
	if (!host_)
		throw std::runtime_error("Module is not attached to platform");
	auto map = getMap();
	if (!map)
		throw std::runtime_error("Platform is not attached to map");

	map->exploreInRadius(host_->getPos(), actionRange_, [&](const Coord& pos, std::shared_ptr<MapObject> obj) {

		auto platform = std::dynamic_pointer_cast<Platform>(obj);
		if (!platform || robotsNum_ == maxRobotsNum_)
			return;

		ConnectRequest request(shared_from_this());
		platform->dispatch(request);
		bool isObjectAdd;
		auto res = request.result();
		// If connection is success
		if (res != nullptr)
		{
			inferiors_.push_back(res);
			robotsNum_++;
			isObjectAdd = true;
		}

		auto robot = std::dynamic_pointer_cast<Robot>(platform);
		if (robot && robot->getManager().lock() == nullptr)
		{
			robots_.push_back(robot);
			robot->setManager(shared_from_this());
			if (!isObjectAdd)
				robotsNum_++;
			isObjectAdd = true;
		}

	});

	for (auto inferiror : inferiors_)
		inferiror.lock()->dispatch(FindControllersRequest());
}

std::stack<robots::Coord> robots::Controller::makeRoute(const Coord& start, const Coord& end)
{
	if (!host_)
		throw std::runtime_error("Module is not attached to platform");
	auto map = getMap();
	if (!map)
		throw std::runtime_error("Platform is not attached to map");
	if (map->getObject(end) != nullptr)
		return {};

	std::unordered_map<Coord, bool> visited;
	std::unordered_map<Coord, Coord> prev;
	std::queue<Coord> pointsToVisit;

	auto tryAddPoint = [&](const Coord& from, const Coord &to) {
		if (map->isOnMap(to) && visited.find(to) == visited.end() && !busyFields_.contains(to)
			&& host_->getPos().calcDist(to) <= this->getActionRange())
		{
			pointsToVisit.push(to);
			prev[to] = from;
		}
	};

	visited[start] = true;
	pointsToVisit.push(start);
	while (!pointsToVisit.empty())
	{
		Coord point = pointsToVisit.front();
		if (point == end)
			break;

		tryAddPoint(point, { point.x, point.y + 1 });
		tryAddPoint(point, { point.x + 1, point.y });
		tryAddPoint(point, { point.x, point.y - 1 });
		tryAddPoint(point, { point.x - 1, point.y });

		visited[point] = true;
		pointsToVisit.pop();
	}
	if (prev.find(end) == prev.end())
		return {};

	std::stack<Coord> route;
	Coord cur = end;
	while (cur != start)
	{
		route.push(cur);
		cur = prev[cur];
	}
	route.push(cur);
	route.pop();

	return route;
}

void robots::Controller::initExploration()
{
	if (!host_)
		throw std::runtime_error("Module is not attached to platform");
	auto map = getMap();
	if (!map)
		throw std::runtime_error("Platform is not attached to map");

	map->exploreInRadius(host_->getPos(), actionRange_, [&](const Coord& pos, std::shared_ptr<MapObject> obj) {
		//if (!map->explored.contains(pos) && obj == nullptr)
		//toExplore_[pos] = true;
		toExplore_.insert(pos);
	});
}

void robots::Controller::explore()
{
	if (!this->hasTask())
		return;
	auto& explored = getMap()->explored;
	auto& visited = getMap()->visited;
	for (auto robot : robots_)
	{
		auto robotPtr = robot.lock();

		toExplore_.erase(robotPtr->getPos());
		visited.insert(robotPtr->getPos());
		explored.insert(robotPtr->getPos());

		auto sr = ScanRequest();
		robotPtr->dispatch(sr);
		for (const auto &[pos, obj] : sr.getObjects())
		{
			explored.insert(pos);
			if (obj == nullptr)
				continue;
			if (typeid(*obj) == typeid(Obstacle) || typeid(*obj) == typeid(CommandCenter))
			{
				toExplore_.erase(pos);
				busyFields_.insert(pos);
			}
		}

		qDebug() << "Pos: " << robotPtr->getPos();
		qDebug() << "Remained: " << toExplore_.size();

		Coord curPos = robotPtr->getPos();
		std::unordered_set<Coord, bool>::iterator target = toExplore_.end();
		unsigned minDist = UINT32_MAX;
		for (auto it = toExplore_.begin(); it != toExplore_.end(); it++)
		{
			if (visited.contains(*it))
				continue;
			unsigned dist = curPos.calcDist(*it);
			if (dist < minDist)
			{
				minDist = dist;
				target = it;
			}
		}

		if (target != toExplore_.end())
		{
			auto route = makeRoute(curPos, *target);
			// If we can't build route to this point, we mark it as unreachable
			if (!route.empty())
			{
				robotPtr->setRoute(route);
				robotPtr->move();
			}
			else
				toExplore_.erase(*target);
		}
	}
	qDebug();
}