#pragma once

#include "Module.h"

namespace robots
{
	class Sensor : public Module
	{
	private:
		unsigned radius_;

	public:
		Sensor(unsigned consumption, unsigned price, unsigned radius, std::shared_ptr<Platform> host = nullptr)
			: Module(consumption, price, host), radius_(radius) {}

		inline unsigned getRadius() const { return radius_; }

		bool work(BaseRequest& request) override;
	};
}