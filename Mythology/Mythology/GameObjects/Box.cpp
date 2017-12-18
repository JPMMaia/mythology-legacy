#include "pch.h"
#include "Box.h"
#include "GameEngine/Component/Physics/RigidDynamicComponent.h"

using namespace GameEngine;
using namespace Mythology;
using namespace physx;

Box::Box(GameEngine::GameManager& gameManager, const physx::PxTransform& transform)
{
	auto& meshRepository = gameManager.GetMeshRepository();
	auto& materialRepository = gameManager.GetMaterialRepository();
	auto& physicsManager = gameManager.GetPhysicsManager();
	auto& physicsScene = gameManager.GetPhysicsScene();

	// Create box:
	std::string meshName("Box");
	auto instance = meshRepository.Get(meshName)->CreateInstance(materialRepository.Get("Wood"));
	m_gameObject.AddRootComponent("Mesh", instance);
	InstanceEventsQueue::AlwaysUpdate(meshName, instance); // TODO

	// Create shape:
	constexpr float halfExtent = 0.5f;
	static auto shape = PhysicsUtilities::MakeSharedPointer<PxShape>(
		physicsManager->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *physicsScene.GetMaterial("Default"))
		);

	// Create rigid dynamic:
	auto body = PhysicsUtilities::CreateRigidDynamic(*physicsManager.GetPhysics(), transform, *shape, 10.0f);
	physicsScene->addActor(*body);
	m_gameObject.AddComponent("RigidDynamic", RigidDynamicComponent::CreateSharedPointer(m_gameObject.GetSharedTransform(), body));
}
