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
	m_foundation = Pointer<PxFoundation>(PxCreateFoundation(PX_FOUNDATION_VERSION, s_defaultAllocatorCallback, s_defaultErrorCallback));
	if (!m_foundation)
		throw std::runtime_error("PxCreateFoundation failed!");

	// Create physics visual debugger:
	/*m_physicsVisualDebugger = Pointer<PxPvd>(PxCreatePvd(*m_foundation));
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_physicsVisualDebugger->connect(*transport, PxPvdInstrumentationFlag::eALL);*/

	// Create physics object:
	auto recordMemoryAllocations = true;
	m_physics = Pointer<PxPhysics>(PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), recordMemoryAllocations, m_physicsVisualDebugger.get()));
	if (!m_physics)
		throw std::runtime_error("PxCreatePhysics failed!");
}
