#include "Controller.h"
#include "ControllerRequest.h"
#include "Robot.h"
#include "Coord.h"

bool robots::Controller::work(BaseRequest& request)
{
	if (typeid(request) == typeid(ScanRequest))
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
		}
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

	Coord pos = host_->getPos();
	for (unsigned i = std::max(0, static_cast<int>(pos.x) - static_cast<int>(actionRange_)); i <= std::min(pos.x + actionRange_, map->getWidth() - 1); i++)
	{
		int hRadius = actionRange_ - abs(static_cast<int>(i) - static_cast<int>(pos.x));
		for (unsigned j = std::max(0, static_cast<int>(pos.y) - hRadius); j <= std::min(pos.y + hRadius, map->getHeight() - 1); j++)
		{
			if (i == pos.x && j == pos.y)
				continue;
			auto obj = std::dynamic_pointer_cast<Platform>(map->getObject({ i, j }));
			if (!obj)
				continue;

			ConnectRequest request(shared_from_this());
			obj->dispatch(request);
			bool isObjectAdd;
			auto platform = request.result();
			// If connection is success
			if (platform != nullptr)
			{
				inferiors_.push_back(platform);
				robotsNum_++;
				isObjectAdd = true;
			}

			auto robot = std::dynamic_pointer_cast<Robot>(obj);
			if (robot && robot->getManager().lock() == nullptr)
			{
				robots_.push_back(robot);
				robot->setManager(shared_from_this());
				if (!isObjectAdd)
					robotsNum_++;
				isObjectAdd = true;
			}

			if (robotsNum_ == maxRobotsNum_)
				return;
		}
	}

	for (auto inferiror : inferiors_)
		inferiror.lock()->dispatch(ScanRequest());
}

std::stack<robots::Coord> robots::Controller::makeRoute(const Coord& from, const Coord& to)
{
	if (!host_)
		throw std::runtime_error("Module is not attached to platform");
	auto map = getMap();
	if (!map)
		throw std::runtime_error("Platform is not attached to map");

	return makeRoute(map, actionRange_, from, to);
}

std::stack<robots::Coord> robots::Controller::makeRoute(std::shared_ptr<Map> map, unsigned radius, const Coord& start, const Coord& end)
{
	if (map->getObject(end) != nullptr)
		return {};

	std::unordered_map<Coord, bool> visited;
	std::unordered_map<Coord, Coord> prev;
	std::queue<Coord> pointsToVisit;

	auto tryAddPoint = [&](const Coord& from, const Coord &to) {
		if (map->isOnMap(to) && map->getObject(to) == nullptr && visited.find(to) == visited.end() && start.calcDist(to) <= radius)
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

	return route;
}