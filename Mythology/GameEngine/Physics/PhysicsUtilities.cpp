#include "pch.h"
#include "PhysicsUtilities.h"

using namespace Eigen;
using namespace GameEngine;
using namespace physx;

Affine3f PhysicsUtilities::ToEigen(const PxTransform & pxTransform)
{
	auto pxMatrix = PxMat44(pxTransform);

	Matrix4f eigenMatrix;

	for(auto i = 0; i < 4; ++i)
		for(auto j = 0; j < 4; ++j)
			eigenMatrix(i, j) = pxMatrix(i, j);

	return Affine3f(eigenMatrix);
}

physx::PxVec3 PhysicsUtilities::ToPhysX(const Eigen::Vector3f & vector)
{
	return physx::PxVec3(vector.x(), vector.y(), vector.z());
}

physx::PxTransform PhysicsUtilities::ToPhysX(const Eigen::Affine3f& eigenTransform)
{
	physx::PxMat44 pxMatrix;

	for (auto i = 0; i < 4; ++i)
		for (auto j = 0; j < 4; ++j)
			pxMatrix(i, j) = eigenTransform(i, j);

	return physx::PxTransform(pxMatrix);
}

PhysXSharedPointer<physx::PxRigidDynamic> PhysicsUtilities::CreateRigidDynamic(PxPhysics& physics, const PxTransform& transform, PxShape& shape, float mass)
{
	auto body = MakeSharedPointer<physx::PxRigidDynamic>(physics.createRigidDynamic(transform));
	body->attachShape(shape);
	PxRigidBodyExt::updateMassAndInertia(*body, mass);
	return body;
}
