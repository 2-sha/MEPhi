#pragma once

#include "BaseRequest.h"

#include <unordered_map>
#include <memory>

namespace robots
{
	class MapObject;

	/**
	* @ingroup Request
	*
	* @brief Scans the area for any kind of items (even empty)
	*/
	class ScanRequest : public BaseRequest
	{
	private:
		std::unordered_map<Coord, std::shared_ptr<MapObject>> objects_;

	public:
		ScanRequest() : BaseRequest() {}

		void addObject(std::shared_ptr<MapObject> obj, const Coord &pos) { objects_.emplace(pos, obj); }
		auto getObjects() { return objects_; }
	};
}