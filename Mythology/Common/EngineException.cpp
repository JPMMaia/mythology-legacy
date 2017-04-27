#include "Common/EngineException.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

#include <sstream>

using namespace Common;
using namespace std;

EngineException::EngineException(const std::string& message)
{
#ifdef _DEBUG
	OutputDebugStringA(message.c_str());
#endif
}

const char* EngineException::what() const noexcept
{
	return m_message.c_str();
}
