#include "Map.h"

std::shared_ptr<robots::MapObject> robots::Map::getObject(const Coord& pos)
{
	auto object = objects_.find(pos);
	if (object != objects_.end())
		return (*object).second;
	return nullptr;
}