#pragma once

#include "Common/Timers/Timer.h"
#include "PhysicsEngine/IScene.h"
#include "GameEngine/Component/Physics/RigidDynamicComponent.h"
#include "RenderEngine/IScene.h"

#include <mutex>

namespace GameEngine
{
	class RigidDynamicManager
	{
	public:
		void FixedUpdate(const Common::Timer& timer, const PhysicsEngine::IScene<>& physicsScene);
		void FrameUpdate(const Common::Timer& timer, RenderEngine::IScene& renderScene);

	private:
		std::mutex m_mutex;
		std::vector<PhysicsEngine::IRigidDynamic<>*> m_activeComponents;
	};
}
