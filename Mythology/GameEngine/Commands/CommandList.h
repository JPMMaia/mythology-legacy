#pragma once

#include "ICommand.h"

#include <list>
#include <memory>

namespace GameEngine
{
	class CommandList
	{
		friend class CommandQueue;

	protected:
		std::list<std::unique_ptr<ICommand>> m_commands;
	};
}
