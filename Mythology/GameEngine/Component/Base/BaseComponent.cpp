#include "BaseComponent.h"

using namespace GameEngine;

BaseComponent::BaseComponent() :
	m_transform(std::make_shared<TransformComponent>())
{
}

void BaseComponent::FixedUpdate(const Common::Timer& timer)
{
	m_transform->FixedUpdate(timer);
}

void BaseComponent::SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays)
{
	m_transform->SetParent(parent, worldTransformStays);
}
void BaseComponent::UnsetParent(bool worldTransformStays)
{
	m_transform->UnsetParent(worldTransformStays);
}

const TransformComponent& BaseComponent::GetTransform() const
{
	return *m_transform;
}
TransformComponent& BaseComponent::GetTransform()
{
	return *m_transform;
}
