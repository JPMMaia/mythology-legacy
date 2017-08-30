#pragma once

#include "PhysicsUtilities.h"

namespace GameEngine
{
	class PhysicsScene
	{
	public:
		PhysicsScene(PhysXUniquePointer<physx::PxScene>&& scene);

	public:
		void FixedUpdate(const Common::Timer& timer) const;

	private:
		PhysXUniquePointer<physx::PxScene> m_scene;
	};
}
