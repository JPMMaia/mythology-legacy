export module Common.Exceptions;

import std.core;

export namespace Common
{
	export class EngineException : public std::exception
	{
	public:
		EngineException() = default;
		explicit EngineException(const std::string& message) :
			m_message(message)
		{
		}

		const char* what() const noexcept override
		{
			return m_message.c_str();
		}

	private:
		std::string m_message;
	};
}
