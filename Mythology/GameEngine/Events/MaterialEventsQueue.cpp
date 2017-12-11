#include "pch.h"
#include "MaterialEventsQueue.h"

using namespace GameEngine;

Common::Event<MaterialEventsQueue, std::string, MaterialEventsQueue::EventArg> MaterialEventsQueue::OnCreate;
Common::Event<MaterialEventsQueue, std::string, MaterialEventsQueue::EventArg> MaterialEventsQueue::OnUpdate;
Common::Event<MaterialEventsQueue, std::string, MaterialEventsQueue::EventArg> MaterialEventsQueue::OnDelete;

MaterialEventsQueue::Container MaterialEventsQueue::s_createQueue;
MaterialEventsQueue::Container MaterialEventsQueue::s_updateQueue;
MaterialEventsQueue::Container MaterialEventsQueue::s_deleteQueue;

void MaterialEventsQueue::Flush()
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
	flushQueue(s_updateQueue, OnUpdate);
	flushQueue(s_deleteQueue, OnDelete);
}

void GameEngine::MaterialEventsQueue::Create(const Pointer& material)
{
	s_createQueue.push_back(material);
}
void GameEngine::MaterialEventsQueue::Update(const Pointer& material)
{
	s_updateQueue.push_back(material);
}
void GameEngine::MaterialEventsQueue::Delete(const Pointer& material)
{
	s_deleteQueue.push_back(material);
}
