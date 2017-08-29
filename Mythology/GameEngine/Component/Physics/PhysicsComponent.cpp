#include "pch.h"
#include "PhysicsComponent.h"
#include "PxPhysicsAPI.h"

using namespace GameEngine;
using namespace physx;

PhysicsComponent::PhysicsComponent()
{	
	static PxDefaultErrorCallback gDefaultErrorCallback;
	static PxDefaultAllocator gDefaultAllocatorCallback;

	auto foundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!foundation)
		throw std::exception("PxCreateFoundation failed!");
}

