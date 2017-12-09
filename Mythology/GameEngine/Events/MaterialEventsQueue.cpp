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
	OnCreate(s_createQueue);
	s_createQueue.clear();

	OnUpdate(s_updateQueue);
	s_updateQueue.clear();

	OnDelete(s_deleteQueue);
	s_deleteQueue.clear();
}

void GameEngine::MaterialEventsQueue::Create(const Pointer& material)
{
	s_createQueue.push_back(material);
}

inline void GameEngine::MaterialEventsQueue::Update(const Pointer& material)
{
	s_updateQueue.push_back(material);
}

inline void GameEngine::MaterialEventsQueue::Delete(const Pointer& material)
{
	s_deleteQueue.push_back(material);
}
