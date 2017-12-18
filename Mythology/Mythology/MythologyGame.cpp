#include "pch.h"
#include "MythologyGame.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "GameEngine/Geometry/Primitives/RectangleGeometry.h"
#include "GameEngine/Geometry/Importer/SceneImporter.h"
#include "GameEngine/Geometry/Primitives/CustomGeometry.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "Common/Helpers.h"
#include "Interfaces/IFileSystem.h"
#include "GameEngine/Events/MaterialEventsQueue.h"
#include "GameEngine/Events/InstanceEventsQueue.h"
#include "GameEngine/Component/Physics/RigidStaticComponent.h"
#include "GameEngine/Component/Physics/RigidDynamicComponent.h"

#include <cmath>

using namespace Common;
using namespace Eigen;
using namespace GameEngine;
using namespace Mythology;
using namespace physx;

MythologyGame::MythologyGame(const std::shared_ptr<IFileSystem>& fileSystem) :
	m_fileSystem(fileSystem)
{
	Initialize();
}

void MythologyGame::Initialize()
{
	m_gameManager = std::make_shared<GameEngine::GameManager>();

	auto& meshRepository = m_gameManager->GetMeshRepository();
	auto& materialRepository = m_gameManager->GetMaterialRepository();

	auto& physicsManager = m_gameManager->GetPhysicsManager();
	auto& physicsScene = m_gameManager->GetPhysicsScene();
	
	physicsScene.AddMaterial("Default", PhysicsUtilities::MakeSharedPointer(physicsManager->createMaterial(0.5f, 0.5f, 0.6f)));

	// Meshes:
	{
		{
			auto mesh = MeshComponent<BoxGeometry>::CreateSharedPointer("Box", BoxGeometry(1.0f, 1.0f, 1.0f, 0));
			meshRepository.Add(mesh->GetName(), mesh);
		}

		{
			auto mesh = MeshComponent<RectangleGeometry>::CreateSharedPointer("Floor", RectangleGeometry(0.0f, 0.0f, 20.0f, 20.0f, 0.0f, 0));
			meshRepository.Add(mesh->GetName(), mesh);
		}
	}

	// Materials:
	{
		{
			auto material = StandardMaterial::CreateSharedPointer("Wood", Vector4f(1.0f, 1.0f, 1.0f, 1.0f), L"Resources/bamboo-wood/bamboo-wood-semigloss-albedo.dds", 0.0f, 0.8f, L"Resources/white.dds");
			materialRepository.Add(material->GetName(), material);
		}

		{
			auto material = StandardMaterial::CreateSharedPointer("Red", Vector4f(1.0f, 0.0f, 0.0f, 1.0f), L"Resources/white.dds", 0.0f, 0.5f, L"Resources/white.dds");
			materialRepository.Add(material->GetName(), material);
		}

		{
			auto material = StandardMaterial::CreateSharedPointer("Green", Vector4f(0.0f, 1.0f, 0.0f, 1.0f), L"Resources/white.dds", 0.0f, 0.5f, L"Resources/white.dds");
			materialRepository.Add(material->GetName(), material);
		}

		{
			auto material = StandardMaterial::CreateSharedPointer("Blue", Vector4f(0.0f, 0.0f, 1.0f, 1.0f), L"Resources/white.dds", 0.0f, 0.5f, L"Resources/white.dds");
			materialRepository.Add(material->GetName(), material);
		}
	}

	// Person:
	{
		std::string meshName("Box");
		auto instance = meshRepository.Get(meshName)->CreateInstance(materialRepository.Get("Wood"));
		InstanceEventsQueue::AlwaysUpdate(meshName, instance);
		m_person.AddRootComponent(meshName, instance);

		{
			auto component = PointLightComponent::CreateSharedPointer(Eigen::Vector3f(0.8f, 0.8f, 0.8f), 10.0f, 50.0f);
			component->GetTransform().SetLocalPosition(3.0f * Vector3f(0.0f, 1.0f, -2.0f));
			m_person.AddComponent("Light", component);
		}

		{
			auto component = CameraComponent::CreateSharedPointer();
			component->GetTransform().SetLocalRotation(Quaternionf::FromTwoVectors(Vector3f::UnitZ(), Vector3f(0.0f, -1.0f, 2.0f)));
			component->GetTransform().SetLocalPosition(3.0f * Vector3f(0.0f, 1.0f, -2.0f));
			m_person.AddComponent("Camera", component);
		}
	}

	// Axis:
	m_axis = Axis(*m_gameManager.get());

	// Floor:
	{
		m_floor.AddRootComponent("Root", std::make_shared<TransformComponent>());

		std::string meshName("Floor");
		auto instance = meshRepository.Get(meshName)->CreateInstance(materialRepository.Get("Wood"));
		auto rotation90 = std::sqrt(2.0f) / 2.0f;
		instance->GetTransform().SetLocalRotation(Quaternionf(rotation90, 0.0f, rotation90, 0.0f));
		m_floor.AddComponent("Mesh", instance);

		auto physicsPlane = PhysicsUtilities::MakeSharedPointer<PxRigidStatic>(PxCreatePlane(*physicsManager.GetPhysics(), PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *physicsScene.GetMaterial("Default")));
		physicsScene->addActor(*physicsPlane);
		m_floor.AddComponent("RigidStatic", RigidStaticComponent::CreateSharedPointer(m_floor.GetSharedTransform(), physicsPlane));
	}

	{
		std::wstring basePath(L"Resources/");
		std::string modelName = "test";
		SceneImporter::ImportedScene scene;
		SceneImporter::Import(basePath + L"test4.fbx", scene);

		auto skinnedMeshComponent = SkinnedMeshComponent::CreateSharedPointer(modelName);

		const auto& object = scene.Objects[0];
		for (std::size_t i = 0; i < object.Geometries.size(); ++i)
		{
			const auto& geometry = object.Geometries[i];
			const auto& material = scene.Materials[geometry.MaterialIndex];

			const auto& baseColor = material.FloatProperties.at("$clr.diffuse");
			//const auto& albedoMap = basePath + Helpers::GetFilename(Helpers::StringToWString(material.DiffuseTexturePath)) + L".dds";
			const auto& baseColorTextureName = basePath + L"white.dds";
			auto standardMaterial = StandardMaterial::CreateSharedPointer(modelName + std::to_string(i), Vector4f(baseColor[0], baseColor[1], baseColor[2], 1.0f), baseColorTextureName, 0.0f, 0.5f, L"Resources/white.dds");
			materialRepository.Add(standardMaterial->GetName(), standardMaterial);

			skinnedMeshComponent->AddMesh(CustomGeometry<EigenMeshData>(std::move(geometry.MeshData)), standardMaterial);
		}

		auto& importedArmature = scene.Armatures.at(object.ArmatureIndex);
		auto armature = std::make_shared<Armature>(std::move(importedArmature.BoneHierarchy), std::move(importedArmature.BoneTransforms), std::move(importedArmature.Animations));
		m_armatures.emplace(importedArmature.Bones.front(), armature);

		auto instance = skinnedMeshComponent->CreateInstance(armature, object.MeshToParentOfBoneRoot);
		instance->GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
		//instance->GetTransform().SetLocalScaling(Vector3f(1.0f, 1.0f, 1.0f) * 0.01f);
		m_box.AddRootComponent("AnimationInstance", instance);

		m_skinnedMeshes.emplace(modelName, skinnedMeshComponent);
	}

	// Stacks:
	{
		auto stackZ = 10.0f;
		for (std::size_t i = 0; i < 5; ++i)
			CreateStack(PxTransform(PxVec3(0, 0.0f, stackZ -= 10.0f)), 5, *physicsScene.GetMaterial("Default"));
	}

	auto& keyboard = m_gameManager->GetKeyboard();
	keyboard.OnKeyPress += {"CreateProjectile", this, &MythologyGame::CreateProjectile};
	keyboard.OnKeyPress += {"CreateAxis", this, &MythologyGame::CreateAxis};
	keyboard.OnKeyPress += {"DestroyAxis", this, &MythologyGame::DestroyAxis};
}

void MythologyGame::ProcessInput()
{
	m_gameManager->ProcessInput();
}
void MythologyGame::FixedUpdate(const Common::Timer& timer)
{
	m_gameManager->FixedUpdate(timer);
}
void MythologyGame::FrameUpdate(const Common::Timer& timer)
{
	m_gameManager->FrameUpdate(timer);

	auto& cameraTransform = m_person.GetTransform();

	static constexpr auto movementSensibility = 0.125f;
	auto& keyboard = m_gameManager->GetKeyboard();
	if (keyboard.IsKeyDown('W'))
		cameraTransform.MoveLocalZ(movementSensibility);
	if (keyboard.IsKeyDown('A'))
		cameraTransform.MoveLocalX(movementSensibility);
	if (keyboard.IsKeyDown('S'))
		cameraTransform.MoveLocalZ(-movementSensibility);
	if (keyboard.IsKeyDown('D'))
		cameraTransform.MoveLocalX(-movementSensibility);

	static constexpr auto tiltSensibility = 0.0625f;
	if (keyboard.IsKeyDown('Q'))
		cameraTransform.Rotate(Vector3f::UnitZ(), -tiltSensibility);
	if (keyboard.IsKeyDown('E'))
		cameraTransform.Rotate(Vector3f::UnitZ(), tiltSensibility);

	static constexpr auto mouseSensibility = 1.0f / 512.0f;
	auto& mouse = m_gameManager->GetMouse();
	auto deltaMovement = mouse.DeltaMovement();
	cameraTransform.Rotate(Vector3f::UnitX(), mouseSensibility * deltaMovement[1]);
	cameraTransform.Rotate(Vector3f::UnitY(), -mouseSensibility * deltaMovement[0]);
}

std::shared_ptr<GameManager> MythologyGame::GameManager() const
{
	return m_gameManager;
}

GameObject::PointerType<CameraComponent> MythologyGame::GetMainCamera() const
{
	return m_person.GetComponent<CameraComponent>("Camera");
}

void MythologyGame::CreateStack(const physx::PxTransform& transform, std::size_t size, const physx::PxMaterial& material)
{
	for (std::size_t i = 0; i < size; ++i)
	{
		for (std::size_t j = 0; j < size - i; ++j)
		{
			// Calculate the local transform:
			PxTransform localTransform(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * 0.5f);
			
			// Create box:
			m_boxes.emplace_back(*m_gameManager, transform.transform(localTransform));
		}
	}
}

void MythologyGame::CreateProjectile(std::uint8_t key)
{
	if (key != ' ')
		return;

	auto& meshRepository = m_gameManager->GetMeshRepository();
	auto& materialRepository = m_gameManager->GetMaterialRepository();
	auto& physicsManager = m_gameManager->GetPhysicsManager();
	auto& physicsScene = m_gameManager->GetPhysicsScene();

	GameObject boxObject;
	std::string meshName("Box");
	auto instance = meshRepository.Get(meshName)->CreateInstance(materialRepository.Get("Wood"));
	boxObject.AddRootComponent("Mesh", instance);
	InstanceEventsQueue::AlwaysUpdate(meshName, instance);

	{
		const auto& cameraTransform = GetMainCamera()->GetTransform();
		PxTransform transform(PhysicsUtilities::ToPhysX(cameraTransform.GetWorldTransform()));
		PxSphereGeometry geometry(3.0f);
		PxVec3 velocity(PhysicsUtilities::ToPhysX(cameraTransform.GetWorldZ() * 20.0f));

		auto body = PhysicsUtilities::MakeSharedPointer<PxRigidDynamic>(PxCreateDynamic(*physicsManager.GetPhysics(), transform, geometry, *physicsScene.GetMaterial("Default"), 10.0f));
		body->setAngularDamping(0.5f);
		body->setLinearVelocity(velocity);
		physicsScene->addActor(*body);

		boxObject.AddComponent("RigidDynamic", RigidDynamicComponent::CreateSharedPointer(boxObject.GetSharedTransform(), body));
	}

	m_projectiles.emplace_back(std::move(boxObject));
}
void MythologyGame::CreateAxis(std::uint8_t key)
{
	if (key != 'K')
		return;

	m_axis = Axis(*m_gameManager.get());
}
void MythologyGame::DestroyAxis(std::uint8_t key)
{
	if (key != 'L')
		return;

	m_axis = Axis();
}