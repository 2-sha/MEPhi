#pragma once

#include "Coord.h"
#include "Map.h"

#include <string>
#include <memory>
#include <stdexcept>

/**
* @defgroup MapObjects
* @ingroup Map
*
* @brief Objects, which can be placed on map
*/

namespace robots
{
	/**
	* @ingroup MapObjects
	*
	* @brief Base class, which must be inherited by concrete map objects
	*/
	class MapObject
	{
	protected:
		Coord pos_;
		std::shared_ptr<Map> map_;


	public:
		MapObject(const Coord& pos = { 0, 0 }, std::shared_ptr<Map> map = nullptr) : pos_(pos), map_(map) {}
		virtual ~MapObject() = default;

		/**
		* @brief Updates object position
		*
		* @param pos New object position
		* 
		* @throw std::out_of_range If new position is out of field
		* @throw std::invalid_argument If new position is busy by another object
		*/
		void setPos(const Coord& pos);

		inline Coord getPos() const { return pos_; }

		inline void attachToMap(std::shared_ptr<Map> map, const Coord& pos) { map_ = map; pos_ = pos; }
	};
}