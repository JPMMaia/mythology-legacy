#pragma once

#include "Component/Meshes/MeshComponent.h"
#include "Component/Transforms/TransformComponent.h"
#include "Geometry/Primitives/BoxGeometry.h"
#include "Common/Events/Event.h"
#include "Component/Cameras/CameraComponent.h"

#include <array>

namespace Common
{
	class Timer;
}

namespace GameEngine
{
	class GameManager : public std::enable_shared_from_this<GameManager>
	{
	public:
		Common::Event<GameManager, std::string, void*, const MeshComponent<BoxGeometry>&> OnBoxCreatedEvent;

	public:
		GameManager();

	public:
		void FixedUpdate(const Common::Timer& timer) const;
		void FrameUpdate(const Common::Timer& timer) const;

	private:
		std::array<TransformComponent, 20> m_transforms;
		std::array<MeshComponent<BoxGeometry>, 20> m_boxes;
		std::array<CameraComponent, 1> m_cameras;
	};
}
