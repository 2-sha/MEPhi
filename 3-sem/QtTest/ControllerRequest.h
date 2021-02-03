#pragma once

#include "BaseRequest.h"

namespace robots
{
	class Platform;
	class Controller;

	/**
	* @ingroup Request
	*
	* @brief Controller will try recursively send ConnectRequest to available robots in its radius
	* @details Will try to connect to available controllers and robots, after that will emit 
	          the same operations on it subordinates
	*/
	class FindControllersRequest : public BaseRequest
	{
	public:
		FindControllersRequest() : BaseRequest() {}
	};

	/**
	* @ingroup Request
	*
	* @brief Explore the area and create list of points, that should be visited
	*/
	class InitExplorationRequest : public BaseRequest
	{
	public:
		InitExplorationRequest() : BaseRequest() {}
	};

	/**
	* @ingroup Request
	*
	* @brief Explore points, that was determined after InitExploration request
	* @details Controller will move controlled robots to explore the available area
	*/
	class ExploreRequest : public BaseRequest
	{
	private:
		bool b = true;

	public:
		ExploreRequest() : BaseRequest() {}

		/**
		* @brief Is controller explore all available area
		*/
		bool hasTasks() { return b; }

		/**
		* @brief Called by controller, if there is no available points
		*/
		void endExplore() { b = false; }
	};

	/**
	* @ingroup Request
	*
	* @brief Through this request one controller tries to get control on another robot 
	* @details If controller, that recivied the request, can be managed and don't have an owner yet, it accept the request
	*/
	class ConnectRequest : public BaseRequest
	{
	private:
		std::shared_ptr<Platform> target_;
		std::shared_ptr<Controller> sender_;

	public:
		ConnectRequest(std::shared_ptr<Controller> sender) : BaseRequest(), target_(nullptr), sender_(sender) {}

		/**
		* @brief Accept conenection
		*
		* @param subject Pointer to object, which recieve request
		*/
		inline void connect(std::shared_ptr<Platform> subject) { target_ = subject; }

		inline std::shared_ptr<Controller> getSender() { return sender_; }

		/**
		* @brief Get result of connection
		*
		* @return Pointer to the target or nullptr
		*/
		inline std::shared_ptr<Platform> result() { return target_; }
	};
}