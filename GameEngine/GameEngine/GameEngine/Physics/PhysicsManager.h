#pragma once

#include "PhysicsScene.h"

namespace GameEngine
{
	class PhysicsManager
	{
	private:
		template<typename T>
		using UniquePointer = PhysXUniquePointer<T>;

		template<typename T>
		using SharedPointer = PhysXSharedPointer<T>;

	public:
		PhysicsManager();
		PhysicsManager(const PhysicsManager&) = delete;
		PhysicsManager& operator=(const PhysicsManager&) = delete;
		PhysicsManager(PhysicsManager&&) = default;
		PhysicsManager& operator=(PhysicsManager&&) = default;

	public:
		PhysicsScene CreateScene();

	public:
		const SharedPointer<physx::PxPhysics>& GetPhysics() const;

	public:
		physx::PxPhysics* operator->();

	private:
		UniquePointer<physx::PxFoundation> m_foundation;
#if defined(_DEBUG)
		UniquePointer<physx::PxPvdTransport> m_transport;
		UniquePointer<physx::PxPvd> m_physicsVisualDebugger;
#endif
		SharedPointer<physx::PxPhysics> m_physics;
		UniquePointer<physx::PxDefaultCpuDispatcher> m_cpuDispatcher;

	private:
		static physx::PxDefaultAllocator s_defaultAllocatorCallback;
		static physx::PxDefaultErrorCallback s_defaultErrorCallback;
	};
}