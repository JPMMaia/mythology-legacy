#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/Actors/IRigidDynamic.h"

namespace PhysXEngine
{
	class PhysXRigidDynamic : public PhysicsEngine::IRigidDynamic<>
	{
	public:
		explicit PhysXRigidDynamic(PhysXUniquePointer<physx::PxRigidDynamic>&& data) :
			m_data(std::move(data))
		{
		}

	private:
		PhysXUniquePointer<physx::PxRigidDynamic> m_data;
	};
}
