#include "Generator.h"

bool robots::Generator::work(BaseRequest& request)
{
	// We don't interrupt chain, because there can be more generators
	if (typeid(request) == typeid(CalcEnergyRequest))
		static_cast<CalcEnergyRequest&>(request).addEnergy(energy_);

	return false;
}