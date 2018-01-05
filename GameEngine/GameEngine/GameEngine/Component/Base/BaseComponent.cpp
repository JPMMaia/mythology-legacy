#include "pch.h"
#include "BaseComponent.h"

using namespace GameEngine;

BaseComponent::BaseComponent() :
	m_transform(std::make_shared<TransformComponent>())
{
}

BaseComponent::BaseComponent(const std::shared_ptr<TransformComponent>& transform) :
	m_transform(transform)
{
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

const std::shared_ptr<TransformComponent>& BaseComponent::GetSharedTransform() const
{
	return m_transform;
}

void BaseComponent::SetTransform(const std::shared_ptr<TransformComponent>& transform)
{
	m_transform = transform;
}
