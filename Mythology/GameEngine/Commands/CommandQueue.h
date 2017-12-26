#pragma once

#include "CommandList.h"

#include <functional>
#include <mutex>
#include <shared_mutex>

namespace GameEngine
{
	class CommandQueue
	{
	public:
		using Queue = std::list<std::unique_ptr<ICommand>>;
		using List = CommandList;

	public:
		void Submit(List& eventsList)
		{
			std::unique_lock<std::shared_mutex> lock(m_commandsQueueMutex);

			m_commandsQueue.splice(m_commandsQueue.end(), eventsList.m_commands);
		}
		void Execute()
		{
			Queue commandsQueue;
			{
				std::shared_lock<std::shared_mutex> lock(m_commandsQueueMutex);
				std::swap(commandsQueue, m_commandsQueue);
			}

			for (const auto& command : commandsQueue)
			{
				command->Execute();
			}
		}

	private:
		Queue m_commandsQueue;
		std::shared_mutex m_commandsQueueMutex;
	};
}
