#include "MythologyGame.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "Libraries/Eigen/Geometry"
#include "GameEngine/Geometry/Primitives/RectangleGeometry.h"
#include "GameEngine/Geometry/Importer/SceneImporter.h"
#include "GameEngine/Geometry/Primitives/CustomGeometry.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "Common/Helpers.h"
#include "Interfaces/IFileSystem.h"

#include <cmath>

using namespace Common;
using namespace Eigen;
using namespace Mythology;
using namespace GameEngine;

MythologyGame::MythologyGame(const std::shared_ptr<IFileSystem>& fileSystem) :
	m_fileSystem(fileSystem)
{
	Initialize();
}

void MythologyGame::Initialize()
{
	m_gameManager = std::make_shared<GameEngine::GameManager>();

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
		auto instance = m_meshes.at("Floor")->CreateInstance(m_materials.at("Wood"));
		instance->GetTransform().SetWorldRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
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
