#pragma once

#include <string>

namespace Common
{
	class EngineException : public std::exception
	{
	public:
		EngineException() = default;
		explicit EngineException(const std::string& message);

		const char* what() const noexcept override;

	private:
		std::string m_message;
	};
}
