#pragma once

namespace GameEngine
{
	class PhysicsManager
	{
	public:
		template<typename T>
		using Pointer = std::unique_ptr<T, std::function<void(T*)>>;

	public:
		PhysicsManager();

	private:
		Pointer<physx::PxFoundation> m_foundation;
		Pointer<physx::PxPvd> m_physicsVisualDebugger;
		Pointer<physx::PxPhysics> m_physics;

	private:
		static physx::PxDefaultAllocator s_defaultAllocatorCallback;
		static physx::PxDefaultErrorCallback s_defaultErrorCallback;
	};
}