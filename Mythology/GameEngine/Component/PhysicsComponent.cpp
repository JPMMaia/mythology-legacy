#include "PhysicsComponent.h"

using namespace GameEngine;

void PhysicsComponent::FixedUpdate(const Common::Timer& timer)
{
}

void PhysicsComponent::SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays)
{
	m_transform.SetParent(parent, worldPositionStays);
}
