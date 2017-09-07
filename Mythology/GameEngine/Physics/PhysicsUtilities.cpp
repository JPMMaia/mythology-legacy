#include "pch.h"
#include "PhysicsUtilities.h"

using namespace Eigen;
using namespace GameEngine;
using namespace physx;

Affine3f PhysicsUtilities::PhysXToEigenTransform(const PxTransform & pxTransform)
{
	auto pxMatrix = PxMat44(pxTransform);

	Matrix4f eigenMatrix;

	for(auto i = 0; i < 4; ++i)
		for(auto j = 0; j < 4; ++j)
			eigenMatrix(i, j) = pxMatrix(i, j);

	return Affine3f(eigenMatrix);
}

PhysXSharedPointer<physx::PxRigidDynamic> PhysicsUtilities::CreateRigidDynamic(PxPhysics& physics, const PxTransform& transform, PxShape& shape, float mass)
{
	auto body = MakeSharedPointer<physx::PxRigidDynamic>(physics.createRigidDynamic(transform));
	body->attachShape(shape);
	PxRigidBodyExt::updateMassAndInertia(*body, mass);
	return body;
}
