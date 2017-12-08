#pragma once

#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "Common/Events/Event.h"

#include <memory>
#include <queue>

namespace GameEngine
{
	class MeshEventsQueue
	{
	public:
		using Type = BaseMeshComponent;
		using Pointer = std::shared_ptr<Type>;
		using ConstRef = const Type&;

	public:
		static Common::Event<MeshEventsQueue, std::string, ConstRef> OnCreate;
		static Common::Event<MeshEventsQueue, std::string, ConstRef> OnDelete;

	public:
		static void Flush();

	public:
		static void Create(const Pointer& value);
		static void Delete(const Pointer& value);

	private:
		static std::queue<Pointer> s_createQueue;
		static std::queue<Pointer> s_deleteQueue;
	};
}
