#ifndef MAIA_UTILITIES_ENGINEEXCEPTION_H_INCLUDED
#define MAIA_UTILITIES_ENGINEEXCEPTION_H_INCLUDED

#include <string>

namespace Maia
{
	namespace Utilities
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
}

#endif
