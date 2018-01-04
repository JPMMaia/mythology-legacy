#include "pch.h"
#include "PhysXActorFactory.h"

using namespace PhysXEngine;

PhysXActorFactory::PhysXActorFactory(const PhysXSharedPointer<physx::PxPhysics>& physics) :
	m_physics(physics)
{
}

std::unique_ptr<PhysicsEngine::IRigidDynamic<>> PhysXEngine::PhysXActorFactory::CreateRigidDynamic(const Geometry& geometry, const Eigen::Affine3f& transform, const Material& material, float density)
{
	return std::make_unique<PhysXRigidDynamic>(
		PhysicsHelpers::MakeUniquePointer(
			physx::PxCreateDynamic(
				*m_physics,
				PhysicsHelpers::ToPhysX(transform),
				geometry,
				*material,
				density
			)
		)
		);
}

std::unique_ptr<PhysicsEngine::IRigidStatic<>> PhysXEngine::PhysXActorFactory::CreateRigidStatic(const Geometry & geometry, const Eigen::Affine3f & transform, const Material& material)
{
	return std::make_unique<PhysXRigidStatic>(
		PhysicsHelpers::MakeUniquePointer(
			physx::PxCreateStatic(
				*m_physics,
				PhysicsHelpers::ToPhysX(transform),
				geometry,
				*material
			)
		)
		);
}
