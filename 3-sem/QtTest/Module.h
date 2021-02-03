#pragma once

#include "Platform.h"

#include <memory>
#include <utility>
#include <any>
#include <optional>

/**
* @defgroup Module
*
* @brief Modules can be installed on platforms and controll it
*/

namespace robots
{
	class Platform;
	class BaseRequest;
	class Map;

	/**
	* @ingroup Module
	*
	* @brief Base class for all types of modules
	*/
	class Module
	{
	protected:
		std::shared_ptr<Platform> host_;

		// Classes friendship doesn't spread to their children, so this func helps to do it
		inline std::shared_ptr<Map> getMap() { return host_->map_; }

	private:
		unsigned consumption_;
		unsigned price_;
		bool isOn_;

	public:
		unsigned priority;

		Module(unsigned consumption, unsigned price, std::shared_ptr<Platform> host = nullptr)
			: consumption_(consumption), price_(price), isOn_(true), priority(0) {}
		virtual ~Module() = default;

		inline void turnOn() { isOn_ = true; }
		inline void turnOff() { isOn_ = false; }
		inline bool isOn() const { return isOn_; }

		inline unsigned getConsumption() const { return consumption_; }
		inline unsigned getPrice() const { return price_; }

		inline void attachToPlatform(std::shared_ptr<Platform> host) { host_ = host; }

		/**
		* @brief Handle request
		* @details If module can handle this type of request, it returns true and do some work. 
		           It implements pattern "chain"
		*
		* @param request Request, which be handle
		* @return true if module can handle the request, false if can't
		*/
		virtual bool work(BaseRequest& request) { return false; }
	};
}