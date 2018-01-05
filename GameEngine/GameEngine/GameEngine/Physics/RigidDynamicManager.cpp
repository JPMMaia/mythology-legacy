#include "pch.h"
#include "RigidDynamicManager.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"

#include <algorithm>

using namespace GameEngine;

void RigidDynamicManager::FixedUpdate(const Common::Timer& timer, const PhysicsEngine::IScene<>& physicsScene)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_activeComponents = physicsScene.GetActiveActors();
	for (auto* activeComponent : m_activeComponents)
		activeComponent->FixedUpdate(timer);
}
void RigidDynamicManager::FrameUpdate(const Common::Timer& timer, RenderEngine::IScene& renderScene)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	for (auto activeComponent : m_activeComponents)
	{
		// Calculate the transform:
		auto transform = activeComponent->CalculateFrameTransform(timer);

		// Update the objects's transform:
		auto* component = reinterpret_cast<RigidDynamicComponent*>(activeComponent->GetUserData());
		component->UpdateTransform(transform);

		// Update render scene:
		{
			const auto& gameObject = component->GetGameObject();
			
			auto instances = gameObject.GetInstances();
			for (const auto& instancePair : instances)
			{
				auto& instance = *instancePair.second;

				renderScene.UpdateInstance(instance.GetMeshName(), *instance.GetInstanceData());
			}
		}
	}
}
