#include "pch.h"
#include "GameManager.h"
#include "GameObject/GameObject.h"

#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"

using namespace Common;
using namespace GameEngine;

GameManager::GameManager()
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
void GameManager::FixedUpdate(const Common::Timer& timer) const
{
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
void GameManager::FrameUpdate(const Common::Timer& timer) const
{
	::FrameUpdate<PhysicsComponent>(timer);

	std::for_each(SkinnedMeshComponent::begin(), SkinnedMeshComponent::end(), [&timer](SkinnedMeshComponent& mesh)
	{
		std::for_each(mesh.GetInstancesBegin(), mesh.GetInstancesEnd(), [&timer](SkinnedMeshInstance& instance)
		{
			instance.GetAnimation().FrameUpdate(timer);
		});
	});
}
