#include "Platform.h"
#include "Generator.h"
#include "Coord.h"
#include "Module.h"

void robots::Platform::proccessModules()
{
	std::sort(modules_.begin(), modules_.end(), [](const auto& left, const auto& right) { return left->priority < right->priority; });

	energyLevel_ = std::accumulate(modules_.begin(), modules_.end(), 0, [](unsigned sum, auto cur) {
		auto generator = std::dynamic_pointer_cast<Generator>(cur);
		if (generator)
			return sum + generator->getEnergy();
		return sum;
	});

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
	if (!isWork())
		return false;

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

unsigned robots::Platform::calcPrice()
{
	return std::accumulate(modules_.begin(), modules_.end(), platformPrice_, [](unsigned sum, auto cur) {
		return sum + cur->getPrice();
	});
}