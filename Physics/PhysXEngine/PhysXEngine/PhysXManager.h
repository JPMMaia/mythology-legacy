#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/IManager.h"

namespace PhysXEngine
{
	class PhysXManager : public PhysicsEngine::IManager<>
	{
	private:
		template<typename T>
		using UniquePointer = PhysXUniquePointer<T>;

		template<typename T>
		using SharedPointer = PhysXSharedPointer<T>;

	public:
		PhysXManager();
		PhysXManager(const PhysXManager&) = delete;
		PhysXManager& operator=(const PhysXManager&) = delete;
		PhysXManager(PhysXManager&&) = default;
		PhysXManager& operator=(PhysXManager&&) = default;

	private:
		void Initialize();

	public:
		std::unique_ptr<PhysicsEngine::IScene<>> CreateScene() override;
		std::unique_ptr<PhysicsEngine::IGeometryFactory<>> CreateGeometryFactory() override;
		std::unique_ptr<PhysicsEngine::IMaterialFactory<>> CreateMaterialFactory() override;
		std::unique_ptr<PhysicsEngine::IActorFactory<>> CreateActorFactory() override;

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
