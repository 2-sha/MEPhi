#pragma once

#include "Module.h"
#include "GeneratorRequest.h"

namespace robots
{
	class Platform;

	class Generator : public Module
	{
	private:
		unsigned energy_;

	public:
		Generator(unsigned energy, unsigned price, std::shared_ptr<Platform> host = nullptr)
			: Module(0, price, host), energy_(energy) {}

		bool work(BaseRequest& request) override;
	};
}
