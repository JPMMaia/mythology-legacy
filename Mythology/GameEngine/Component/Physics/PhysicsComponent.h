#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "GameEngine/Memory/UseStorage.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

namespace GameEngine
{
	class PhysicsComponent : public BaseComponent, public UseStorage<PhysicsComponent>
	{
	public:
		explicit PhysicsComponent(const std::shared_ptr<TransformComponent>& transform);
		PhysicsComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidActor>& rigidActor);

	public:
		void FrameUpdate(const Common::Timer& timer);

	public:
		const TransformComponent& GetTransform() const;

	private:
		std::shared_ptr<TransformComponent> m_transform;
		std::shared_ptr<physx::PxRigidActor> m_rigidActor;
	};
}
