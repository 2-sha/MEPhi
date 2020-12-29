#pragma once

#include "BaseRequest.h"

namespace robots
{
	class Platform;
	class Controller;

	class ScanRequest : public BaseRequest
	{
	public:
		ScanRequest() : BaseRequest() {}
	};

	class ConnectRequest : public BaseRequest
	{
	private:
		std::shared_ptr<Platform> target_;
		std::shared_ptr<Controller> sender_;

	public:
		ConnectRequest(std::shared_ptr<Controller> sender) : BaseRequest(), target_(nullptr), sender_(sender) {}

		inline void connect(std::shared_ptr<Platform> subject) { target_ = subject; }
		inline std::shared_ptr<Controller> getSender() { return sender_; }
		inline std::shared_ptr<Platform> result() { return target_; }
	};
}