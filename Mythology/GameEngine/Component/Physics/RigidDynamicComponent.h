#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "GameEngine/Memory/UseStorage.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

namespace GameEngine
{
	class RigidDynamicComponent : public BaseComponent, public UseStorage<RigidDynamicComponent>
	{
	public:
		RigidDynamicComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidDynamic>& rigidDynamic);

	public:
		void FrameUpdate(const Common::Timer& timer);

	private:
		std::shared_ptr<physx::PxRigidDynamic> m_rigidDynamic;
		physx::PxTransform m_lastGlobalPose;
		physx::PxVec3 m_lastAngularVelocity;
		physx::PxVec3 m_lastLinearVelocity;
	};
}
