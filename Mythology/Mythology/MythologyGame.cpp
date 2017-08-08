#include "MythologyGame.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "Libraries/Eigen/Geometry"

using namespace Eigen;
using namespace Mythology;
using namespace GameEngine;

MythologyGame::MythologyGame()
{
	Initialize();
}

void MythologyGame::Initialize()
{
	m_gameManager = std::make_shared<GameEngine::GameManager>();

	{
		auto boxComponentPointer = new MeshComponent<BoxGeometry>(BoxGeometry(1.0f, 1.0f, 1.0f, 0));
		m_person.AddComponent("Mesh", std::shared_ptr<MeshComponent<BoxGeometry>>(boxComponentPointer));
	}

	{
		auto pointLightComponentPointer = new PointLightComponent(Eigen::Vector3f(0.8f, 0.8f, 0.8f), 2.0f, 5.0f);
		std::shared_ptr<PointLightComponent> pointLightComponent(pointLightComponentPointer);
		pointLightComponent->SetLocalPosition({ 0.0f, 2.0f, -1.0f });
		m_person.AddComponent("Light", pointLightComponent);
	}
	
	{
		auto cameraComponentPointer = new CameraComponent();
		std::shared_ptr<CameraComponent> cameraComponent(cameraComponentPointer);
		cameraComponent->GetTransform().SetWorldPosition({ 0.0f, 0.0f, 0.0f });
		//cameraComponent->GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(std::acos(-1.0f), Vector3f::UnitY())));
		//cameraComponent->GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(0.0f * std::acos(-1.0f), Vector3f::UnitY())));
		//cameraComponent->GetTransform().SetLocalRotation(Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), Eigen::Vector3f(0.0f, -2.0f, 1.0f)));
		//cameraComponent->GetTransform().SetLocalPosition({ 0.0f, 2.0f, -2.0f });
		//cameraComponent->GetTransform().SetLocalRotation(Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), Eigen::Vector3f(0.0f, -2.0f, 2.0f)));
		m_person.AddComponent("Camera", cameraComponent);
	}
}

void MythologyGame::ProcessInput() const
{
	m_gameManager->ProcessInput();

	auto& cameraTransform = m_person.GetComponent<CameraComponent>("Camera")->GetTransform();
	
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
		cameraTransform.RotateLocalZ(-tiltSensibility);
	if (keyboard.IsKeyDown('E'))
		cameraTransform.RotateLocalZ(tiltSensibility);
	
	static constexpr auto mouseSensibility = 1.0f / 512.0f;
	auto& mouse = m_gameManager->GetMouse();
	auto deltaMovement = mouse.DeltaMovement();
	cameraTransform.Rotate(Vector3f::UnitX(), -mouseSensibility * deltaMovement[1]);
	//cameraTransform.RotateLocalY(-mouseSensibility * deltaMovement[0]);
}
void MythologyGame::FixedUpdate(const Common::Timer& timer) const
{
	m_gameManager->FixedUpdate(timer);
}
void MythologyGame::FrameUpdate(const Common::Timer& timer) const
{
	m_gameManager->FrameUpdate(timer);
}

std::shared_ptr<GameManager> MythologyGame::GameManager() const
{
	return m_gameManager;
}
