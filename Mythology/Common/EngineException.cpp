#include "Common/EngineException.h"

#ifdef _DEBUG
#include <Windows.h>
#endif

#include <sstream>

using namespace Common;
using namespace std;

EngineException::EngineException(const std::string& message) :
	std::exception(message.c_str())
{
#ifdef _DEBUG
	OutputDebugStringA(message.c_str());
#endif
}