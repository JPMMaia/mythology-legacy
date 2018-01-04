#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysicsEngine/Geometries/IGeometryFactory.h"

namespace PhysXEngine
{
	class PhysXGeometryFactory : public PhysicsEngine::IGeometryFactory<>
	{
	public:
		Geometry CreateSphere(float radius) override
		{
			return physx::PxSphereGeometry(radius);
		}
		Geometry CreateBox(float halfX, float halfY, float halfZ) override
		{
			return physx::PxBoxGeometry(halfX, halfY, halfZ);
		}
	};
}
