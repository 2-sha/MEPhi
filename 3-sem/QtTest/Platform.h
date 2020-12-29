#pragma once

#include "MapObject.h"

#include <algorithm>
#include <numeric>
#include <memory>
#include <string>
#include <vector>
#include <any>
#include <optional>

namespace robots
{
	class Module;
	class BaseRequest;

	/**
	* @ingroup MapObjects
	*
	* @brief Base class for all robots
	*/
	class Platform : public MapObject, public std::enable_shared_from_this<Platform>
	{
	private:
		std::string description_;
		unsigned platformConsumption_;
		unsigned slotsNum_;
		unsigned price_;
		unsigned allConsumption_;
		unsigned energyLevel_;
		std::vector<std::shared_ptr<Module>> modules_;

	public:
		Platform( 
			const unsigned consumption,
			const unsigned slotsNum,
			const unsigned price,
			const std::string& description = ""
		) : MapObject(), platformConsumption_(consumption), allConsumption_(consumption), slotsNum_(slotsNum),
			price_(price), description_(description), energyLevel_(0) {};

		/**
		* @brief Sort modules by their priority and recalculate summary consumption and energy level
		* 
		* @details If energy level is too low this function turns off modules with the lowest priority
		*/
		void proccessModules();

		/**
		* @brief Get summary energy and consumption levels
		*
		* @return std::pai with consumption level as first param and energy level as second
		*/
		std::pair<unsigned, unsigned> getEnergyStatus() const { return { allConsumption_, energyLevel_ }; }

		/**
		* @brief Send given query to modules and return answer
		* @details This function sends a request to each of the modules until one of them returns true.
		*          Implements pattern Chain of Responsibility and Command in ModuleRequest.
		*
		* @param request It can be any class, which inherits ModuleRequest class
		* @return false, if request was not proccessed, and true if some handler catch it
		*/
		bool dispatch(BaseRequest& request);

		std::vector<std::shared_ptr<Module>> getModules() { return modules_; }

		/**
		* @brief Simple objects factory, which create modules and attach them to platform
		*
		* @param priority Priotiry for module
		* @param args Parameters to be passed to the module constructor
		* @return shared_ptr to the const created module
		* 
		* @throw std::overflow_error If all module slots are busy
		*/
		template<typename ObjectType, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<Module, ObjectType>>>
		std::shared_ptr<ObjectType> addModule(const unsigned&& priority, Args&&... args)
		{
			if (slotsNum_ == modules_.size())
				throw std::overflow_error("All slots are busy");

			auto mod = std::make_shared<ObjectType>(std::forward<Args>(args)...);
			mod->priority = priority;
			mod->attachToPlatform(shared_from_this());
			modules_.push_back(mod);

			proccessModules();
			return mod;
		}

		// This line gives Module access to map_ field
		friend class Module;
	};
}