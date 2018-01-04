#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/Materials/IMaterialFactory.h"

namespace PhysXEngine
{
	class PhysXMaterialFactory : public PhysicsEngine::IMaterialFactory<>
	{
	public:
		explicit PhysXMaterialFactory(const PhysXSharedPointer<physx::PxPhysics>& physics) :
			m_physics(physics)
		{
		}

	public:
		Material CreateMaterial(float staticFriction, float dynamicFriction, float restitution) override
		{
			return PhysicsHelpers::MakeSharedPointer<physx::PxMaterial>(
				m_physics->createMaterial(staticFriction, dynamicFriction, restitution)
				);
		}

	private:
		PhysXSharedPointer<physx::PxPhysics> m_physics;
	};
}
