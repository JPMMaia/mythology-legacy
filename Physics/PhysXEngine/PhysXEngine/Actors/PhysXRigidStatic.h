#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/Actors/IRigidStatic.h"

namespace PhysXEngine
{
	class PhysXRigidStatic : public PhysicsEngine::IRigidStatic<>
	{
	public:
		explicit PhysXRigidStatic(PhysXUniquePointer<physx::PxRigidStatic>&& data) :
			m_data(std::move(data))
		{
		}

	private:
		PhysXUniquePointer<physx::PxRigidStatic> m_data;
	};
}
