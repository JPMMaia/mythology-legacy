#pragma once

#include <string>

namespace Common
{
	class EngineException : public std::exception
	{
	public:
		EngineException() = default;
		explicit EngineException(const std::string& message);
	};
}
