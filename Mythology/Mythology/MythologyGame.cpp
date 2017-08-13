#include "MythologyGame.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "Libraries/Eigen/Geometry"
#include "GameEngine/Geometry/Primitives/RectangleGeometry.h"

#include <cmath>

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
		auto mesh = MeshComponent<BoxGeometry>::CreateSharedPointer(BoxGeometry(1.0f, 1.0f, 1.0f, 0));
		m_meshes.emplace("Box", mesh);
	}

	{
		auto floor = MeshComponent<RectangleGeometry>::CreateSharedPointer(RectangleGeometry(0.0f, 0.0f, 20.0f, 20.0f, 0.0f, 0));
		m_meshes.emplace("Floor", floor);
	}

	// Person:
	{
		m_person.AddComponent("Box", m_meshes.at("Box")->CreateInstance(0));

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
			auto instance = box->CreateInstance(1);
			instance->GetTransform().SetLocalScaling({ 2.0f, 0.1f, 0.1f });
			instance->GetTransform().SetLocalPosition({ 1.0f, 0.0f, 0.0f });
			m_axis.AddComponent("X-axis", instance);
		}

		{
			auto instance = box->CreateInstance(2);
			instance->GetTransform().SetLocalScaling({ 0.1f, 2.0f, 0.1f });
			instance->GetTransform().SetLocalPosition({ 0.0f, 1.0f, 0.0f });
			m_axis.AddComponent("Y-axis", instance);
		}

		{
			auto instance = box->CreateInstance(3);
			instance->GetTransform().SetLocalScaling({ 0.1f, 0.1f, 2.0f });
			instance->GetTransform().SetLocalPosition({ 0.0f, 0.0f, 1.0f });
			m_axis.AddComponent("Z-axis", instance);
		}
	}

	// Floor:
	{
		auto instance = m_meshes.at("Floor")->CreateInstance(0);
		instance->GetTransform().SetWorldRotation(Quaternionf(AngleAxisf(static_cast<float>(-M_PI_2), Vector3f::UnitX())));
		m_floor.AddComponent("Mesh", instance);
	}
}

void MythologyGame::ProcessInput()
{
	m_gameManager->ProcessInput();
}
void MythologyGame::FixedUpdate(const Common::Timer& timer)
{
	m_gameManager->FixedUpdate(timer);

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
void MythologyGame::FrameUpdate(const Common::Timer& timer) const
{
	m_gameManager->FrameUpdate(timer);
}

std::shared_ptr<GameManager> MythologyGame::GameManager() const
{
	return m_gameManager;
}
