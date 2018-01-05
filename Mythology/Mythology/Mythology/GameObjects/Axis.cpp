#include "pch.h"
#include "Axis.h"

using namespace Mythology;
using namespace GameEngine;

Axis::Axis(GameManager& gameManager)
{
	auto& renderCommandQueue = gameManager.GetRenderCommandQueue();
	auto& meshRepository = gameManager.GetMeshRepository();
	auto& materialRepository = gameManager.GetMaterialRepository();

	RenderCommandList renderCommandList(gameManager.GetRenderScene());

	std::string meshName("Box");
	const auto& box = gameManager.GetMeshRepository().Get(meshName);
	m_gameObject.AddRootComponent("Root", std::make_shared<TransformComponent>());

	{
		auto instance = box->CreateInstance(renderCommandList, meshName, materialRepository.Get("Red"));
		// TODO associate instance with the transform...
		instance->GetTransform().SetLocalScaling({ 2.0f, 0.1f, 0.1f });
		instance->GetTransform().SetLocalPosition({ 1.0f, 0.0f, 0.0f });
		m_gameObject.AddComponent("X-axis", instance);
	}

	{
		auto instance = box->CreateInstance(materialRepository.Get("Green"));
		instance->GetTransform().SetLocalScaling({ 0.1f, 2.0f, 0.1f });
		instance->GetTransform().SetLocalPosition({ 0.0f, 1.0f, 0.0f });
		m_gameObject.AddComponent("Y-axis", instance);
	}

	{
		auto instance = box->CreateInstance(materialRepository.Get("Blue"));
		instance->GetTransform().SetLocalScaling({ 0.1f, 0.1f, 2.0f });
		instance->GetTransform().SetLocalPosition({ 0.0f, 0.0f, 1.0f });
		m_gameObject.AddComponent("Z-axis", instance);
	}
}
