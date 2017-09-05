#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class PhysicsComponent : public BaseComponent, public UseStorage<PhysicsComponent>
	{
	public:
		PhysicsComponent();
		explicit PhysicsComponent(physx::PxRigidActor* rigidActor);

	public:
		const std::shared_ptr<TransformComponent>& GetTransform() const;

	private:
		std::shared_ptr<TransformComponent> m_transform;
		physx::PxRigidActor* m_rigidActor;
	};
}
