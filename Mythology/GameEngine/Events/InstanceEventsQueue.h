#pragma once

#include "Common/Events/Event.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"

#include <unordered_map>
#include <unordered_set>

namespace GameEngine
{
	class InstanceEventsQueue
	{
	public:
		using Type = InstancedMeshComponent;
		using Pointer = std::shared_ptr<Type>;
		using Container = std::unordered_map<std::string, std::unordered_set<Pointer>>;
		using EventArg = const Container&;

	public:
		static Common::Event<InstanceEventsQueue, std::string, EventArg> OnCreate;
		static Common::Event<InstanceEventsQueue, std::string, EventArg> OnUpdate;
		static Common::Event<InstanceEventsQueue, std::string, EventArg> OnDelete;

	public:
		static void Flush();

	public:
		static void Create(const std::string& meshName, const Pointer& instance);
		static void AlwaysUpdate(const std::string& meshName, const Pointer& instance);
		static void NeverUpdate(const std::string& meshName, const Pointer& instance);
		static void Delete(const std::string& meshName, const Pointer& instance);

	private:
		static Container s_createQueue;
		static Container s_updateQueue;
		static Container s_deleteQueue;
	};
}
