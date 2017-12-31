#include "pch.h"
#include "Person.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"

using namespace GameEngine;
using namespace Mythology;
using namespace physx;
using namespace Eigen;

Person::Person(GameEngine::GameManager& gameManager)
{
	auto& meshRepository = gameManager.GetMeshRepository();
	auto& materialRepository = gameManager.GetMaterialRepository();
	auto& physicsManager = gameManager.GetPhysicsManager();
	auto& physicsScene = gameManager.GetPhysicsScene();

	RenderCommandList renderCommandList(gameManager.GetRenderScene());

	std::string meshName("Box");
	auto instance = meshRepository.Get(meshName)->CreateInstance(renderCommandList, materialRepository.Get("Wood"));
	InstanceEventsQueue::AlwaysUpdate(meshName, instance);
	m_gameObject.AddRootComponent(meshName, instance);

	{
		auto component = PointLightComponent::CreateSharedPointer(Eigen::Vector3f(0.8f, 0.8f, 0.8f), 10.0f, 50.0f);
		component->GetTransform().SetLocalPosition(3.0f * Vector3f(0.0f, 1.0f, -2.0f));
		m_gameObject.AddComponent("Light", component);
	}

	{
		auto component = CameraComponent::CreateSharedPointer();
		component->GetTransform().SetLocalRotation(Quaternionf::FromTwoVectors(Vector3f::UnitZ(), Vector3f(0.0f, -1.0f, 2.0f)));
		component->GetTransform().SetLocalPosition(3.0f * Vector3f(0.0f, 1.0f, -2.0f));
		m_gameObject.AddComponent("Camera", component);
	}

	constexpr auto halfExtent = 0.5f;
	static auto shape = PhysicsUtilities::MakeSharedPointer<PxShape>(
		physicsManager->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *physicsScene.GetMaterial("Default"))
		);

	auto rigidDynamic = PhysicsUtilities::CreateRigidDynamic(*physicsManager.GetPhysics(), physx::PxTransform(physx::PxIdentity), *shape, 50.0f);
	physicsScene->addActor(*rigidDynamic);
	rigidDynamic->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	m_rigidDynamicComponent = RigidDynamicComponent::CreateSharedPointer(m_gameObject.GetSharedTransform(), rigidDynamic);
	m_gameObject.AddComponent("RigidDynamic", m_rigidDynamicComponent);

	gameManager.GetRenderCommandQueue().Submit(renderCommandList);
}

void Person::FixedUpdate(const Common::Timer& timer, GameManager& gameManager)
{
	static constexpr auto movementSensibility = 0.125f;
	static constexpr auto mouseSensibility = 1.0f / 512.0f;
	static constexpr auto tiltSensibility = 0.0625f;

	auto& keyboard = gameManager.GetKeyboard();
	auto& mouse = gameManager.GetMouse();
	
	auto transform = (*m_rigidDynamicComponent)->getGlobalPose();
	const auto& cameraTransform = m_gameObject.GetTransform();

	{
		Vector3f deltaPosition(0.0f, 0.0f, 0.0f);

		if (keyboard.IsKeyDown('W'))
			deltaPosition += cameraTransform.GetLocalZ();
		if (keyboard.IsKeyDown('A'))
			deltaPosition += cameraTransform.GetLocalX();
		if (keyboard.IsKeyDown('S'))
			deltaPosition -= cameraTransform.GetLocalZ();
		if (keyboard.IsKeyDown('D'))
			deltaPosition -= cameraTransform.GetLocalX();

		deltaPosition *= movementSensibility;

		transform.p += PxVec3(deltaPosition.x(), deltaPosition.y(), deltaPosition.z());
	}

	{
		auto& rotation = transform.q;
		if (keyboard.IsKeyDown('Q'))
			rotation *= PxQuat(-tiltSensibility, PxVec3(0.0f, 0.0f, 1.0f));
		if (keyboard.IsKeyDown('E'))
			rotation *= PxQuat(tiltSensibility, PxVec3(0.0f, 0.0f, 1.0f));

		auto deltaMovement = mouse.DeltaMovement();
		rotation *= PxQuat(mouseSensibility * deltaMovement[1], PxVec3(1.0f, 0.0f, 0.0f));
		rotation *= PxQuat(-mouseSensibility * deltaMovement[0], PxVec3(0.0f, 1.0f, 0.0f));
	}

	(*m_rigidDynamicComponent)->setKinematicTarget(transform);
}

GameObject::PointerType<CameraComponent> Person::GetCamera() const
{
	return m_gameObject.GetComponent<CameraComponent>("Camera");
}
