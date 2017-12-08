#include "pch.h"
#include "MeshEventsQueue.h"

using namespace GameEngine;

Common::Event<MeshEventsQueue, std::string, MeshEventsQueue::ConstRef> MeshEventsQueue::OnCreate;
Common::Event<MeshEventsQueue, std::string, MeshEventsQueue::ConstRef> MeshEventsQueue::OnDelete;

std::queue<MeshEventsQueue::Pointer> MeshEventsQueue::s_createQueue;
std::queue<MeshEventsQueue::Pointer> MeshEventsQueue::s_deleteQueue;

void MeshEventsQueue::Flush()
{
	while (!s_createQueue.empty())
	{
		OnCreate(*s_createQueue.front());
		s_createQueue.pop();
	}

	while (!s_deleteQueue.empty())
	{
		OnDelete(*s_deleteQueue.front());
		s_deleteQueue.pop();
	}
}

void MeshEventsQueue::Create(const Pointer& value)
{
	s_createQueue.push(value);
}
void MeshEventsQueue::Delete(const Pointer& value)
{
	s_deleteQueue.push(value);
}
