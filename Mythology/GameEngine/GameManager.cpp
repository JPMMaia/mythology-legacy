#include "GameManager.h"
#include "GameObject/GameObject.h"

using namespace GameEngine;

void GameManager::Initialize()
{
}

void GameManager::FixedUpdate(const Common::Timer& timer)
{
	for (auto iterator = m_transforms.Begin(); iterator != m_transforms.End(); ++iterator)
		iterator->GetElement().FixedUpdate(timer);
}
void GameManager::FrameUpdate(const Common::Timer& timer)
{
}

TransformComponent& GameManager::CreateTransform()
{
	return m_transforms.NewElement();
}
MeshComponent<BoxGeometry>& GameManager::CreateBox(const BoxGeometry& geometry)
{
	auto& transform = m_transforms.NewElement();
	auto& box = m_boxes.NewElement(&transform, geometry);
	OnBoxCreated(this, box);
	return box;
}
