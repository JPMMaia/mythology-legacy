#pragma once

#include "PhysXEngine/Helpers/PhysicsHelpers.h"

namespace PhysicsEngine
{
	class PhysicsTraitsImplementation
	{
	public:
		using Geometry = physx::PxGeometry;
		using Material = PhysXEngine::PhysXSharedPointer<physx::PxMaterial>;
	};
}