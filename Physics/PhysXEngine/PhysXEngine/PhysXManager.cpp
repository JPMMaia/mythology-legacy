#include "pch.h"
#include "PhysXManager.h"
#include "PhysXScene.h"
#include "PhysXEngine/Geometries/PhysXGeometryFactory.h"
#include "PhysXEngine/Materials/PhysXMaterialFactory.h"
#include "PhysXEngine/Actors/PhysXActorFactory.h"

using namespace PhysXEngine;
using namespace PhysicsHelpers;
using namespace physx;

PxDefaultAllocator PhysXManager::s_defaultAllocatorCallback;
PxDefaultErrorCallback PhysXManager::s_defaultErrorCallback;

PhysXManager::PhysXManager()
{
	Initialize();
}

void PhysXManager::Initialize()
{
	// Create foundation
	m_foundation = MakeUniquePointer<PxFoundation>(PxCreateFoundation(PX_FOUNDATION_VERSION, s_defaultAllocatorCallback, s_defaultErrorCallback));
	if (!m_foundation)
		throw std::runtime_error("PxCreateFoundation failed!");

#if defined(_DEBUG)
	{
		// Create physics visual debugger:
		m_transport = MakeUniquePointer<PxPvdTransport>(PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10));
		m_physicsVisualDebugger = MakeUniquePointer<PxPvd>(PxCreatePvd(*m_foundation));
		m_physicsVisualDebugger->connect(*m_transport, PxPvdInstrumentationFlag::eALL);
	}
	PxPvd* physicsVisualDebugger = m_physicsVisualDebugger.get();
#else
	PxPvd* physicsVisualDebugger = nullptr;
#endif

	// Create physics object:
	auto recordMemoryAllocations = true;
	m_physics = MakeSharedPointer<PxPhysics>(PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), recordMemoryAllocations, physicsVisualDebugger));
	if (!m_physics)
		throw std::runtime_error("PxCreatePhysics failed!");

	// Create cpu dispatcher:
	m_cpuDispatcher = MakeUniquePointer<PxDefaultCpuDispatcher>(PxDefaultCpuDispatcherCreate(2));
}

std::unique_ptr<PhysicsEngine::IScene<>> PhysXManager::CreateScene()
{
	// Fill scene description:
	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = m_cpuDispatcher.get();
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;

	// Create scene:
	auto scene = MakeUniquePointer<physx::PxScene>(m_physics->createScene(sceneDesc));

#if defined(_DEBUG)
	{
		auto pvdClient = scene->getScenePvdClient();
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
#endif

	return std::make_unique<PhysXScene>(std::move(scene));	
}
std::unique_ptr<PhysicsEngine::IMaterialFactory<>> PhysXManager::CreateMaterialFactory()
{
	return std::make_unique<PhysXMaterialFactory>(m_physics);
}
std::unique_ptr<PhysicsEngine::IGeometryFactory<>> PhysXManager::CreateGeometryFactory()
{
	return std::make_unique<PhysXGeometryFactory>();
}
std::unique_ptr<PhysicsEngine::IActorFactory<>> PhysXManager::CreateActorFactory()
{
	return std::make_unique<PhysXActorFactory>(m_physics);
}
