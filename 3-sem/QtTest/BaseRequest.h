#pragma once 

/**
* @defgroup Request
*
* @brief Requests, that platform modules can dispatch
*/

namespace robots
{

	/**
	* @ingroup Request
	*
	* @brief Base class for all types of requests
	*/
	class BaseRequest
	{
	public:
		virtual ~BaseRequest() = default;
	};
}