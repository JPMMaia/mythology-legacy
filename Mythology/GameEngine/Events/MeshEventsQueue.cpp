#include "pch.h"
#include "MeshEventsQueue.h"

using namespace GameEngine;

Common::Event<MeshEventsQueue, std::string, MeshEventsQueue::EventArg> MeshEventsQueue::OnCreate;
Common::Event<MeshEventsQueue, std::string, MeshEventsQueue::EventArg> MeshEventsQueue::OnDelete;

MeshEventsQueue::Container MeshEventsQueue::s_createQueue;
MeshEventsQueue::Container MeshEventsQueue::s_deleteQueue;

void MeshEventsQueue::Flush()
{
	auto flushQueue = [](Container& queue, const auto& onEvent)
	{
		if (!queue.empty())
		{
			onEvent(queue);
			queue.clear();
		}
	};

	flushQueue(s_createQueue, OnCreate);
	flushQueue(s_deleteQueue, OnDelete);
}

void MeshEventsQueue::Create(const Pointer& value)
{
	s_createQueue.push_back(value);
}
void MeshEventsQueue::Delete(const Pointer& value)
{
	s_deleteQueue.push_back(value);
}
