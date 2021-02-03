#include "Map.h"
#include "MapObject.h"

std::shared_ptr<robots::MapObject> robots::Map::getObject(const Coord& pos)
{
	auto object = objects_.find(pos);
	if (object != objects_.end())
		return (*object).second;
	return nullptr;
}

void robots::Map::moveObject(const Coord& pos)
{
	auto obj = objects_[pos];
	if (!obj)
		return;
	objects_.erase(pos);
	objects_[obj->getPos()] = obj;
}

void robots::Map::exploreInRadius(const Coord &pos, unsigned radius, std::function<void(const Coord&, std::shared_ptr<MapObject>)> callback)
{
	for (unsigned i = std::max(0, (int)pos.x - (int)radius); i <= std::min(pos.x + radius, getWidth() - 1); i++)
	{
		int hRadius = radius - abs((int)i - (int)pos.x);
		for (unsigned j = std::max(0, (int)pos.y - hRadius); j <= std::min(pos.y + hRadius, getHeight() - 1); j++)
		{
			if (i == pos.x && j == pos.y)
				continue;
			callback({ i, j }, getObject({ i, j }));
		}
	}
}