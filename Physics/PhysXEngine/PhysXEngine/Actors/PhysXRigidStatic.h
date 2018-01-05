#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/Actors/IRigidStatic.h"

namespace PhysXEngine
{
	class PhysXRigidStatic : public PhysicsEngine::IRigidStatic<>
	{
	public:
		explicit PhysXRigidStatic(PhysXUniquePointer<physx::PxRigidStatic>&& actor);

	public:
		operator Actor&() override;
		operator const Actor&() const override;

	public:
		void GetGlobalPose(Eigen::Vector3f& position, Eigen::Quaternionf& rotation) const override;

	public:
		void* GetUserData() const override;
		void SetUserData(void* userData) override;

	private:
		PhysXUniquePointer<physx::PxRigidStatic> m_actor;
	};
}
