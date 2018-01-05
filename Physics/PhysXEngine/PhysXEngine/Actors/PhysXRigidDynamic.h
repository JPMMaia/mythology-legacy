#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/Actors/IRigidDynamic.h"

namespace PhysXEngine
{
	class PhysXRigidDynamic : public PhysicsEngine::IRigidDynamic<>
	{
	public:
		explicit PhysXRigidDynamic(PhysXUniquePointer<physx::PxRigidDynamic>&& actor);

	public:
		operator Actor&() override;
		operator const Actor&() const override;

	public:
		void GetGlobalPose(Eigen::Vector3f& position, Eigen::Quaternionf& rotation) const override;

	public:
		void* GetUserData() const override;
		void SetUserData(void* userData) override;

	public:
		void FixedUpdate(const Common::Timer& timer) override;

	public:
		Eigen::Affine3f CalculateFrameTransform(const Common::Timer& timer) override;

	private:
		PhysXUniquePointer<physx::PxRigidDynamic> m_actor;
		physx::PxTransform m_currentGlobalPose;
		physx::PxTransform m_lastGlobalPose;
		physx::PxVec3 m_currentAngularVelocity;
		physx::PxVec3 m_lastAngularVelocity;
		physx::PxVec3 m_currentLinearVelocity;
		physx::PxVec3 m_lastLinearVelocity;
	};
}
