#include "pch.h"
#include "PhysicsManager.h"

using namespace physx;
using namespace GameEngine;

PxDefaultAllocator PhysicsManager::s_defaultAllocatorCallback;
PxDefaultErrorCallback PhysicsManager::s_defaultErrorCallback;

PhysicsManager::PhysicsManager()
{
	auto deleter = [](auto* object)
	{
		object->release();
	};

	// Create foundation
	m_foundation = UniquePointer<PxFoundation>(PxCreateFoundation(PX_FOUNDATION_VERSION, s_defaultAllocatorCallback, s_defaultErrorCallback));
	if (!m_foundation)
		throw std::runtime_error("PxCreateFoundation failed!");

#if defined(_DEBUG)
	{
		// Create physics visual debugger:
		m_transport = UniquePointer<PxPvdTransport>(PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10));
		m_physicsVisualDebugger = UniquePointer<PxPvd>(PxCreatePvd(*m_foundation));
		m_physicsVisualDebugger->connect(*m_transport, PxPvdInstrumentationFlag::eALL);
	}
	PxPvd* physicsVisualDebugger = m_physicsVisualDebugger.get();
#else
	PxPvd* physicsVisualDebugger = nullptr;
#endif

	// Create physics object:
	auto recordMemoryAllocations = true;
	m_physics = SharedPointer<PxPhysics>(PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), recordMemoryAllocations, physicsVisualDebugger));
	if (!m_physics)
		throw std::runtime_error("PxCreatePhysics failed!");

	// Create cpu dispatcher:
	m_cpuDispatcher = UniquePointer<PxDefaultCpuDispatcher>(PxDefaultCpuDispatcherCreate(2));
}

PhysicsScene PhysicsManager::CreateScene()
{
	// Fill scene description:
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = m_cpuDispatcher.get();
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// Create scene:
	PhysXUniquePointer<physx::PxScene> scene(m_physics->createScene(sceneDesc));

#if defined(_DEBUG)
	{
		auto pvdClient = scene->getScenePvdClient();
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif

	return PhysicsScene(std::move(scene));
}
