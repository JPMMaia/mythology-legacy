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

	public:
		const physx::PxScene* operator->() const
		{
			return m_scene.get();
		}
		physx::PxScene* operator->()
		{
			return m_scene.get();
		}

	private:
		PhysXUniquePointer<physx::PxScene> m_scene;
	};
}
