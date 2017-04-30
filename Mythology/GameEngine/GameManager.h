#pragma once

#include "GameObject/GameObject.h"
#include "Common/MemoryPool/MemoryPool.h"
#include "Component/MeshComponent.h"
#include "Geometry/Primitives/BoxGeometry.h"
#include "Common/Events/Event.h"

namespace Common 
{
	class Timer;
}

namespace GameEngine
{
	class GameManager
	{
	public:
		
		//Events::Event<>;

	public:
		void Initialize();

		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer);

	public:
		TransformComponent& CreateTransform();
		MeshComponent<BoxGeometry>& CreateBox(const BoxGeometry& geometry);

	private:
		Common::MemoryPool<TransformComponent, 10> m_transforms;
		Common::MemoryPool<MeshComponent<BoxGeometry>, 10> m_boxes;
	};
}
