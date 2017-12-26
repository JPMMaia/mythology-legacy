#pragma once

#include "GameEngine/Component/Meshes/StandardMaterial.h"
#include "Common/Events/Event.h"

#include <memory>
#include <queue>

namespace GameEngine
{
	class MaterialEventsQueue
	{
	public:
		using Type = StandardMaterial;
		using Pointer = std::shared_ptr<Type>;
		using Container = std::deque<Pointer>;
		using EventArg = const Container&;

	public:
		static Common::Event<MaterialEventsQueue, std::string, EventArg> OnCreate;
		static Common::Event<MaterialEventsQueue, std::string, EventArg> OnUpdate;
		static Common::Event<MaterialEventsQueue, std::string, EventArg> OnDelete;

	public:
		static void Flush();

	public:
		static void Create(const Pointer& material);
		static void Update(const Pointer& material);
		static void Delete(const Pointer& material);

	private:
		static Container s_createQueue;
		static Container s_updateQueue;
		static Container s_deleteQueue;
	};
}