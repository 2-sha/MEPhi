#include "MapObject.h"

void robots::MapObject::setPos(const Coord& pos)
{
	if (map_ == nullptr)
		return;
	if (!map_->isOnMap(pos))
		throw std::out_of_range("Given position is out of field");
	if (map_->getObject(pos) != nullptr)
		throw std::invalid_argument("Given position on busy");

	auto oldPos = pos_;
	pos_ = pos;
	map_->moveObject(oldPos);
}