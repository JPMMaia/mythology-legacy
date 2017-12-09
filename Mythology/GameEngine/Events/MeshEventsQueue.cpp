#include "pch.h"
#include "MeshEventsQueue.h"

using namespace GameEngine;

Common::Event<MeshEventsQueue, std::string, MeshEventsQueue::EventArg> MeshEventsQueue::OnCreate;
Common::Event<MeshEventsQueue, std::string, MeshEventsQueue::EventArg> MeshEventsQueue::OnDelete;

MeshEventsQueue::Container MeshEventsQueue::s_createQueue;
MeshEventsQueue::Container MeshEventsQueue::s_deleteQueue;

void MeshEventsQueue::Flush()
{
	OnCreate(s_createQueue);
	s_createQueue.clear();

	OnDelete(s_deleteQueue);
	s_deleteQueue.clear();
}

void MeshEventsQueue::Create(const Pointer& value)
{
	s_createQueue.push_back(value);
}
void MeshEventsQueue::Delete(const Pointer& value)
{
	s_deleteQueue.push_back(value);
}
