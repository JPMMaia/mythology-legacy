#include "pch.h"
#include "GameManager.h"
#include "GameObject/GameObject.h"

#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Physics/RigidDynamicComponent.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "GameEngine/Commands/Render/RenderCommandList.h"

using namespace Common;
using namespace GameEngine;

GameManager::GameManager(std::unique_ptr<RenderEngine::IRenderer>&& renderer, std::unique_ptr<PhysicsEngine::IManager<>>&& physicsManager) :
	m_renderer(std::move(renderer)),
	m_physicsManager(std::move(physicsManager))
{
}

void GameManager::ProcessInput()
{
	m_mouse.Update();
}

template<class T>
void FixedUpdate(const Timer& timer)
{
	std::for_each(T::begin(), T::end(), [&timer](auto& element)
	{
		element.FixedUpdate(timer);
	});
}
void GameManager::FixedUpdate(const Common::Timer& timer)
{
	// Simulate physics:
	m_physicsScene->FixedUpdate(timer);

	// Update all rigid dynamics:
	m_rigidDynamicManager.FixedUpdate(timer, *m_physicsScene);
}

template<class T>
void FrameUpdate(const Timer& timer)
{
	std::for_each(T::begin(), T::end(), [&timer](auto& element)
	{
		element.FrameUpdate(timer);
	});
}
bool GameManager::FrameUpdate(const Common::Timer& timer)
{
	// Return if next frame is not available:
	if (!m_renderer->IsNextFrameAvailable())
		return false;

	// Execute all pending commands:
	m_renderCommandQueue.Execute();

	// Update all objects that moved during this frame:
	m_rigidDynamicManager.FrameUpdate(timer, *m_renderScene);

	// Update skinned meshes:
	std::for_each(SkinnedMeshComponent::begin(), SkinnedMeshComponent::end(), [&timer](SkinnedMeshComponent& mesh)
	{
		std::for_each(mesh.GetInstancesBegin(), mesh.GetInstancesEnd(), [&timer](SkinnedMeshInstance& instance)
		{
			instance.GetAnimation().FrameUpdate(timer);
		});
	});

	// Update renderer:
	m_renderer->FrameUpdate(timer);

	// Render:
	m_renderer->Render(timer);

	return true;
}
