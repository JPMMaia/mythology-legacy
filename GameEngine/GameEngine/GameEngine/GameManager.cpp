#include "pch.h"
#include "GameManager.h"
#include "GameObject/GameObject.h"

#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Physics/RigidDynamicComponent.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "GameEngine/Commands/Render/RenderCommandList.h"

using namespace Common;
using namespace GameEngine;

GameManager::GameManager(const std::shared_ptr<RenderEngine::IScene>& renderScene) :
	m_renderScene(renderScene),
	m_physicsScene(m_physicsManager.CreateScene())
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
	m_physicsScene.FixedUpdate(timer);

	{
		physx::PxU32 activeActorsCount;
		m_physicsScene->getActiveActors(activeActorsCount);
	}

	::FixedUpdate<CameraComponent>(timer);
}

template<class T>
void FrameUpdate(const Timer& timer)
{
	std::for_each(T::begin(), T::end(), [&timer](auto& element)
	{
		element.FrameUpdate(timer);
	});
}
void GameManager::FrameUpdate(const Common::Timer& timer)
{
	::FrameUpdate<RigidDynamicComponent>(timer);

	std::for_each(SkinnedMeshComponent::begin(), SkinnedMeshComponent::end(), [&timer](SkinnedMeshComponent& mesh)
	{
		std::for_each(mesh.GetInstancesBegin(), mesh.GetInstancesEnd(), [&timer](SkinnedMeshInstance& instance)
		{
			instance.GetAnimation().FrameUpdate(timer);
		});
	});
}
