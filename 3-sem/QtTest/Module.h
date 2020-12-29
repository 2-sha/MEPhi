#pragma once

#include "Platform.h"

#include <memory>
#include <utility>
#include <any>
#include <optional>

namespace robots
{
	class Platform;
	class BaseRequest;
	class Map;

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

		inline void attachToPlatform(std::shared_ptr<Platform> host) { host_ = host; }

		virtual bool work(BaseRequest& request) { return false; }
	};
}