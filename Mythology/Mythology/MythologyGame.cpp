#include "MythologyGame.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "Libraries/Eigen/Geometry"
#include "GameEngine/Geometry/Primitives/RectangleGeometry.h"
#include "GameEngine/Geometry/Importer/SceneImporter.h"
#include "GameEngine/Geometry/Primitives/CustomGeometry.h"
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
		std::wstring basePath(L"Resources/tiny/");
		std::string modelName = "tiny";
		SceneImporter::ImportedScene scene;
		SceneImporter::Import(basePath + L"tiny.x", scene);

		/*for (std::size_t i = 0; i < scene.Geometries.size(); ++i)
		{
			const auto& geometry = scene.Geometries[i];
			const auto& material = scene.Materials[geometry.MaterialIndex];

			auto mesh = MeshComponent<CustomGeometry<EigenMeshData>>::CreateSharedPointer(modelName + std::to_string(i), CustomGeometry<EigenMeshData>(std::move(geometry.MeshData)));
			m_meshes.emplace(mesh->GetName(), mesh);

			const auto& baseColor = material.FloatProperties.at("$clr.diffuse");
			const auto& albedoMap = basePath + Helpers::GetFilename(Helpers::StringToWString(material.DiffuseTexturePath)) + L".dds";
			//const auto& albedoMap = basePath + L"WhiteAlbedo" + L".dds";
			auto standardMaterial = StandardMaterial::CreateSharedPointer(modelName + std::to_string(i), Vector4f(baseColor[0], baseColor[1], baseColor[2], 1.0f), albedoMap);
			m_materials.emplace(standardMaterial->GetName(), standardMaterial);

			auto instance = mesh->CreateInstance(standardMaterial);
			//instance->GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
			instance->GetTransform().SetLocalScaling(Vector3f(1.0f, 1.0f, 1.0f) * 0.01f);
			m_box.AddComponent("Instance" + std::to_string(i), instance);
		}*/
		
		//m_tiny = SkinnedModelInstance(std::move(scene.Objects));
	}

	/*{
		SceneImporter::ImportedScene scene;
		SceneImporter::Import(L"Resources/kasumi/DOA5_Kasumi_MMD.fbx", scene);

		{
			auto workingDirectory = m_fileSystem->GetWorkingDirectory();

			auto filePath = workingDirectory + L"kasumi.bin";
			//std::ofstream file(filePath, std::ios::out | std::fstream::binary);
			//file << scene;
			std::ifstream file(filePath, std::ios::in | std::ios::binary);
			file >> scene;
		}

		for(std::size_t i = 0; i < scene.Geometries.size(); ++i)
		{
			const auto& geometry = scene.Geometries[i];
			const auto& material = scene.Materials[geometry.MaterialIndex];

			auto mesh = MeshComponent<CustomGeometry<EigenMeshData>>::CreateSharedPointer(CustomGeometry<EigenMeshData>(std::move(geometry.MeshData)));
			m_meshes.emplace("kasumi" + std::to_string(i), mesh);
			
			const auto& baseColor = material.FloatProperties.at("$clr.diffuse");
			const auto& albedoMap = L"Resources/kasumi/" + Helpers::GetFilename(Helpers::StringToWString(material.DiffuseTexturePath)) + L".dds";
			auto standardMaterial = StandardMaterial::CreateSharedPointer("kasumi" + std::to_string(i), Vector4f(baseColor[0], baseColor[1], baseColor[2], 1.0f), albedoMap);
			m_materials.emplace(standardMaterial->GetName(), standardMaterial);

			auto instance = mesh->CreateInstance(standardMaterial);
			instance->GetTransform().SetWorldRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
			m_box.AddComponent("Instance" + std::to_string(i), instance);
		}

		//m_box.GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
	}*/
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
	m_tiny.FrameUpdate(timer);

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
