#include "pch.h"
#include "EngineException.h"

#include <sstream>

using namespace Common;
using namespace std;

EngineException::EngineException(const std::string& message) :
	std::exception(message.c_str())
{
}