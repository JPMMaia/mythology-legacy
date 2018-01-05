#pragma once

#include "GameEngine/Component/IComponent.h"
#include "PhysicsEngine/Actors/IRigidStatic.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class RigidStaticComponent : public IComponent, public UseStorage<RigidStaticComponent>
	{
	public:
		RigidStaticComponent(std::unique_ptr<PhysicsEngine::IRigidStatic<>>&& rigidStatic, const std::shared_ptr<TransformComponent>& transform);

	private:
		std::unique_ptr<PhysicsEngine::IRigidStatic<>> m_rigidStatic;
	};
}
