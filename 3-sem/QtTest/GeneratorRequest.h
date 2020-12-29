#pragma once

#include "BaseRequest.h"

namespace robots
{
	class CalcEnergyRequest : public BaseRequest
	{
	private:
		unsigned summary_;

	public:
		CalcEnergyRequest() : BaseRequest(), summary_(0) {}

		inline void addEnergy(unsigned energy) { summary_ += energy; }
		inline unsigned getEnergy() { return summary_; }
	};
}