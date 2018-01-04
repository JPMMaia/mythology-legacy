#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysXEngine/Actors/PhysXRigidDynamic.h"
#include "PhysXEngine/Actors/PhysXRigidStatic.h"
#include "PhysicsEngine/Actors/IActorFactory.h"

namespace PhysXEngine
{
	class PhysXActorFactory : public PhysicsEngine::IActorFactory<>
	{
	public:
		explicit PhysXActorFactory(const PhysXSharedPointer<physx::PxPhysics>& physics);

	public:
		std::unique_ptr<PhysicsEngine::IRigidDynamic<>> CreateRigidDynamic(const Geometry& geometry, const Eigen::Affine3f& transform, const Material& material, float density) override;
		std::unique_ptr<PhysicsEngine::IRigidStatic<>> CreateRigidStatic(const Geometry& geometry, const Eigen::Affine3f& transform, const Material& material) override;

	private:
		PhysXSharedPointer<physx::PxPhysics> m_physics;
	};
}
