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

#include <cmath>

using namespace Common;
using namespace Eigen;
using namespace GameEngine;
using namespace Mythology;
using namespace physx;

MythologyGame::MythologyGame(const std::shared_ptr<IFileSystem>& fileSystem) :
	m_fileSystem(fileSystem),
	m_physicsScene(m_physicsManager.CreateScene())
{
	Initialize();
}

void MythologyGame::Initialize()
{
	m_gameManager = std::make_shared<GameEngine::GameManager>();

	m_physicsMaterial = m_physicsManager->createMaterial(0.5f, 0.5f, 0.6f);

	// Meshes:
	{
		{
			auto mesh = MeshComponent<BoxGeometry>::CreateSharedPointer("Box", BoxGeometry(1.0f, 1.0f, 1.0f, 0));
			m_meshes.emplace(mesh->GetName(), mesh);
		}

		{
			auto mesh = MeshComponent<RectangleGeometry>::CreateSharedPointer("Floor", RectangleGeometry(0.0f, 0.0f, 20.0f, 20.0f, 0.0f, 0));
			m_meshes.emplace(mesh->GetName(), mesh);
		}
	}

	// Materials:
	{
		{
			auto material = StandardMaterial::CreateSharedPointer("Wood", Vector4f(1.0f, 1.0f, 1.0f, 1.0f), L"Resources/bamboo-wood/bamboo-wood-semigloss-albedo.dds");
			m_materials.emplace(material->GetName(), material);
		}

		{
			auto material = StandardMaterial::CreateSharedPointer("Red", Vector4f(1.0f, 0.0f, 0.0f, 1.0f), L"Resources/white.dds");
			m_materials.emplace(material->GetName(), material);
		}

		{
			auto material = StandardMaterial::CreateSharedPointer("Green", Vector4f(0.0f, 1.0f, 0.0f, 1.0f), L"Resources/white.dds");
			m_materials.emplace(material->GetName(), material);
		}

		{
			auto material = StandardMaterial::CreateSharedPointer("Blue", Vector4f(0.0f, 0.0f, 1.0f, 1.0f), L"Resources/white.dds");
			m_materials.emplace(material->GetName(), material);
		}
	}

	// Person:
	{
		m_person.AddComponent("Box", m_meshes.at("Box")->CreateInstance(m_materials.at("Wood")));

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
	{
		auto& box = m_meshes.at("Box");

		{
			auto instance = box->CreateInstance(m_materials.at("Red"));
			instance->GetTransform().SetLocalScaling({ 2.0f, 0.1f, 0.1f });
			instance->GetTransform().SetLocalPosition({ 1.0f, 0.0f, 0.0f });
			m_axis.AddComponent("X-axis", instance);
		}

		{
			auto instance = box->CreateInstance(m_materials.at("Green"));
			instance->GetTransform().SetLocalScaling({ 0.1f, 2.0f, 0.1f });
			instance->GetTransform().SetLocalPosition({ 0.0f, 1.0f, 0.0f });
			m_axis.AddComponent("Y-axis", instance);
		}

		{
			auto instance = box->CreateInstance(m_materials.at("Blue"));
			instance->GetTransform().SetLocalScaling({ 0.1f, 0.1f, 2.0f });
			instance->GetTransform().SetLocalPosition({ 0.0f, 0.0f, 1.0f });
			m_axis.AddComponent("Z-axis", instance);
		}
	}

	// Floor:
	{
		auto physicsPlane = PhysicsUtilities::MakeSharedPointer<PxRigidStatic>(PxCreatePlane(*m_physicsManager.GetPhysics(), PxPlane(0.0f, 1.0f, 0.0f, 0.0f), *m_physicsMaterial));
		m_physicsScene->addActor(*physicsPlane);
		m_floor = GameObject(PhysicsComponent::CreateSharedPointer(physicsPlane));

		auto instance = m_meshes.at("Floor")->CreateInstance(m_materials.at("Wood"));
		auto rotation90 = std::sqrt(2.0f) / 2.0f;
		instance->GetTransform().SetLocalRotation(Quaternionf(rotation90, 0.0f, rotation90, 0.0f));
		m_floor.AddComponent("Mesh", instance);
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
			const auto& albedoMap = basePath + L"white.dds";
			auto standardMaterial = StandardMaterial::CreateSharedPointer(modelName + std::to_string(i), Vector4f(baseColor[0], baseColor[1], baseColor[2], 1.0f), albedoMap);
			m_materials.emplace(standardMaterial->GetName(), standardMaterial);

			skinnedMeshComponent->AddMesh(CustomGeometry<EigenMeshData>(std::move(geometry.MeshData)), standardMaterial);
		}
		
		auto& importedArmature = scene.Armatures.at(object.ArmatureIndex);
		auto armature = std::make_shared<Armature>(std::move(importedArmature.BoneHierarchy), std::move(importedArmature.BoneTransforms), std::move(importedArmature.Animations));
		m_armatures.emplace(importedArmature.Bones.front(), armature);

		auto instance = skinnedMeshComponent->CreateInstance(armature, object.MeshToParentOfBoneRoot);
		instance->GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
		//instance->GetTransform().SetLocalScaling(Vector3f(1.0f, 1.0f, 1.0f) * 0.01f);
		m_box.AddComponent("AnimationInstance", instance);

		m_skinnedMeshes.emplace(modelName, skinnedMeshComponent);
	}

	// Stacks:
	{
		auto stackZ = 10.0f;
		for (std::size_t i = 0; i < 5; ++i)
			CreateStack(PxTransform(PxVec3(0, 0.0f, stackZ -= 10.0f)), 5, *m_physicsMaterial);
	}

	auto& keyboard = m_gameManager->GetKeyboard();
	keyboard.OnKeyPress += {"CreateProjectile", this, &MythologyGame::CreateProjectile};
}

void MythologyGame::ProcessInput()
{
	m_gameManager->ProcessInput();
}
void MythologyGame::FixedUpdate(const Common::Timer& timer)
{
	m_physicsScene.FixedUpdate(timer);

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
	// Create shape and material:
	auto halfExtent = 0.5f;
	auto shape = PhysicsUtilities::MakeSharedPointer<PxShape>(
		m_physicsManager->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), material)
		);

	for (std::size_t i = 0; i < size; ++i)
	{
		for (std::size_t j = 0; j < size - i; ++j)
		{
			// Calculate the local transform:
			PxTransform localTransform(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);

			// Create rigid dynamic:
			auto body = PhysicsUtilities::CreateRigidDynamic(*m_physicsManager.GetPhysics(), transform.transform(localTransform), *shape, 10.0f);
			m_physicsScene->addActor(*body);

			// Create box with physics component:
			GameObject boxObject(PhysicsComponent::CreateSharedPointer(body));

			// Add mesh to the object:
			boxObject.AddComponent("Mesh", m_meshes.at("Box")->CreateInstance(m_materials.at("Wood")));

			m_boxes.emplace_back(boxObject);
		}
	}
}

void MythologyGame::CreateProjectile(std::uint8_t key)
{
	if (key != ' ')
		return;

	const auto& cameraTransform = GetMainCamera()->GetTransform();
	PxTransform transform(PhysicsUtilities::ToPhysX(cameraTransform.GetWorldTransform()));
	PxSphereGeometry geometry(3.0f);
	PxVec3 velocity(PhysicsUtilities::ToPhysX(cameraTransform.GetWorldZ() * 20.0f));

	auto body = PhysicsUtilities::MakeSharedPointer<PxRigidDynamic>(PxCreateDynamic(*m_physicsManager.GetPhysics(), transform, geometry, *m_physicsMaterial, 10.0f));
	body->setAngularDamping(0.5f);
	body->setLinearVelocity(velocity);
	m_physicsScene->addActor(*body);

	GameObject boxObject(PhysicsComponent::CreateSharedPointer(body));
	boxObject.AddComponent("Mesh", m_meshes.at("Box")->CreateInstance(m_materials.at("Wood")));
	m_boxes.emplace_back(boxObject);
}
