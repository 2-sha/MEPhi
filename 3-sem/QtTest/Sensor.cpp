#include "Sensor.h"
#include "SensorRequest.h"
#include <algorithm>

bool robots::Sensor::work(BaseRequest& request)
{
	if (typeid(request) != typeid(ScanRequest))
		return false;

	ScanRequest& scanRequest = static_cast<ScanRequest&>(request);
	if (!host_)
		throw std::runtime_error("Module is not attached to platform");
	auto map = getMap();
	if (!map)
		throw std::runtime_error("Platform is not attached to map");

	map->exploreInRadius(host_->getPos(), radius_, [&](const Coord& pos, std::shared_ptr<MapObject> obj) {
		scanRequest.addObject(obj, pos);
	});

	return true;
}