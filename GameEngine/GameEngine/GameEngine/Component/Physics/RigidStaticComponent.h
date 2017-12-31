#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "GameEngine/Memory/UseStorage.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

namespace GameEngine
{
	class RigidStaticComponent : public BaseComponent, public UseStorage<RigidStaticComponent>
	{
	public:
		RigidStaticComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidStatic>& rigidStatic);

	private:
		std::shared_ptr<physx::PxRigidStatic> m_rigidStatic;
	};
}
