#include "Platform.h"
#include "Module.h"
#include "GeneratorRequest.h"
#include "Coord.h"

void robots::Platform::proccessModules()
{
	std::sort(modules_.begin(), modules_.end(), [](const auto& left, const auto& right) { return left->priority < right->priority; });

	CalcEnergyRequest calcEnergyRequest;
	dispatch(calcEnergyRequest);
	energyLevel_ = calcEnergyRequest.getEnergy();

	allConsumption_ = platformConsumption_;
	for (const auto& it : modules_)
	{
		if (allConsumption_ + it->getConsumption() <= energyLevel_)
		{
			it->turnOn();
			allConsumption_ += it->getConsumption();
		}
		else
			it->turnOff();
	}
}

bool robots::Platform::dispatch(BaseRequest& request)
{
	bool res = false;
	auto ptr = modules_.begin();
	while (!res && ptr != modules_.end())
	{
		if ((*ptr)->isOn())
			res = (*ptr)->work(request);
		ptr++;
	}
	return res;
}