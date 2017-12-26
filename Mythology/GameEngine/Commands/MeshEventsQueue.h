#pragma once

#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "Common/Events/Event.h"

#include <deque>
#include <memory>

namespace GameEngine
{
	class MeshEventsQueue
	{
	public:
		using Type = BaseMeshComponent;
		using Pointer = std::shared_ptr<Type>;
		using Container = std::deque<Pointer>;
		using EventArg = const Container&;

	public:
		static Common::Event<MeshEventsQueue, std::string, EventArg> OnCreate;
		static Common::Event<MeshEventsQueue, std::string, EventArg> OnDelete;

	public:
		static void Flush();

	public:
		static void Create(const Pointer& value);
		static void Delete(const Pointer& value);

	private:
		static Container s_createQueue;
		static Container s_deleteQueue;
	};
}
