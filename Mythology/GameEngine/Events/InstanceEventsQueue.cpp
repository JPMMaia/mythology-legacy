#include "pch.h"
#include "InstanceEventsQueue.h"

using namespace GameEngine;

Common::Event<InstanceEventsQueue, std::string, InstanceEventsQueue::EventArg> InstanceEventsQueue::OnCreate;
Common::Event<InstanceEventsQueue, std::string, InstanceEventsQueue::EventArg> InstanceEventsQueue::OnUpdate;
Common::Event<InstanceEventsQueue, std::string, InstanceEventsQueue::EventArg> InstanceEventsQueue::OnDelete;

InstanceEventsQueue::Container InstanceEventsQueue::s_createQueue;
InstanceEventsQueue::Container InstanceEventsQueue::s_updateQueue;
InstanceEventsQueue::Container InstanceEventsQueue::s_deleteQueue;

void InstanceEventsQueue::Flush()
{
	auto flushQueue = [](auto& queue, const auto& onEvent)
	{
		if (!queue.empty())
		{
			onEvent(queue);
			queue.clear();
		}
	};

	flushQueue(s_createQueue, OnCreate);
	
	if (!s_updateQueue.empty())
		OnUpdate(s_updateQueue);

	flushQueue(s_deleteQueue, OnDelete);
}

void InstanceEventsQueue::Create(const std::string& meshName, const Pointer& instance)
{
	auto instances = s_createQueue.find(meshName);
	if (instances == s_createQueue.end())
	{
		s_createQueue.emplace(meshName, std::unordered_set<Pointer> { instance });
		return;
	}

	instances->second.emplace(instance);
}
void InstanceEventsQueue::AlwaysUpdate(const std::string& meshName, const Pointer& instance)
{
	auto instances = s_updateQueue.find(meshName);
	if (instances == s_updateQueue.end())
	{
		s_updateQueue.emplace(meshName, std::unordered_set<Pointer> { instance });
		return;
	}

	instances->second.emplace(instance);
}
void InstanceEventsQueue::NeverUpdate(const std::string& meshName, const Pointer& instance)
{
	auto instances = s_updateQueue.find(meshName);
	if (instances == s_updateQueue.end())
		return;

	instances->second.erase(instance);

	if (instances->second.empty())
		s_updateQueue.erase(instances);
}
void InstanceEventsQueue::Delete(const std::string& meshName, const Pointer& instance)
{
	NeverUpdate(meshName, instance);

	auto instances = s_deleteQueue.find(meshName);
	if (instances == s_deleteQueue.end())
	{
		s_deleteQueue.emplace(meshName, std::unordered_set<Pointer> { instance });
		return;
	}

	instances->second.emplace(instance);
}
