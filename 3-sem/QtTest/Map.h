#pragma once

#include "Coord.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

/**
* @defgroup Map
*
* @brief Describes map, which holds all objects
*/

namespace robots
{
	class MapObject;

	/**
	* @ingroup Map
	* 
	* @brief Describes the environment and objects on the field
	* @details Through this class you can add objects, get them and interact
	*/
	class Map : public std::enable_shared_from_this<Map>
	{
	private:
		const unsigned width_;
		const unsigned height_;
		std::unordered_map<Coord, std::shared_ptr<MapObject>> objects_;

	public:
		Map(const unsigned width = 0, const unsigned height = 0) : width_(width), height_(height) {};

		inline unsigned getWidth() const { return width_; }
		inline unsigned getHeight() const { return height_; }

		/**
		* @brief Checks, if objects in limits of map
		* 
		* @param pos Position of the object being checked
		*/
		inline bool isOnMap(const Coord& pos) const { return pos.x < width_ && pos.y < height_; }

		/**
		* @brief Get const object by its coordinates
		* 
		* @return shared_ptr to the const object or nullptr if it's not found
		*/
		std::shared_ptr<MapObject> getObject(const Coord& pos);

		std::unordered_map<Coord, std::shared_ptr<MapObject>> getObjectsList() { return objects_; }

		/**
		* @brief Simple objects factory, which create objects and attach them to map
		*
		* @param pos Position of object on map
		* @param args Parameters to be passed to the object constructor
		* @return shared_ptr to the const created object
		* 
		* @throw std::invalid_argument If object is out of map or the position is busy
		*/
		template<typename ObjectType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<MapObject, ObjectType>>>
		std::shared_ptr<ObjectType> addObject(const Coord& pos, Args&&... args)
		{
			auto obj = std::make_shared<ObjectType>(std::forward<Args>(args)...);

			if (!isOnMap(pos))
				throw std::invalid_argument("Object position out of map");
			if (objects_.find(pos) != objects_.end())
				throw std::invalid_argument("Position is busy");

			obj->attachToMap(shared_from_this(), pos);
			objects_.emplace(obj->getPos(), obj);
			return obj;
		}
	};
}