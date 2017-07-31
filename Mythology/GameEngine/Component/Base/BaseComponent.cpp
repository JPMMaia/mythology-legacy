#include "BaseComponent.h"

using namespace GameEngine;

void BaseComponent::FixedUpdate(const Common::Timer& timer)
{
	m_transform.FixedUpdate(timer);
}

void BaseComponent::SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays)
{
	m_transform.SetParent(parent, worldPositionStays);
}

const TransformComponent& BaseComponent::Transform() const
{
	return m_transform;
}
TransformComponent& BaseComponent::Transform()
{
	return m_transform;
}
