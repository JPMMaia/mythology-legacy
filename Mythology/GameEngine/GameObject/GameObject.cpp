#include "pch.h"
#include "GameObject.h"

using namespace GameEngine;

GameObject::GameObject() :
	m_physics(PhysicsComponent::CreateSharedPointer())
{
}
GameObject::GameObject(const std::shared_ptr<PhysicsComponent>& physics) :
	m_physics(physics)
{
}

void GameObject::AddComponent(const std::string& name, IComponentPointerCR component, bool worldTransformStays)
{
	component->SetParent(m_physics->GetTransform(), worldTransformStays);
	m_components.emplace(name, component);
}
void GameObject::RemoveComponent(const std::string& name, bool worldTransformStays)
{
	auto componentLocation = m_components.find(name);
	if (componentLocation == m_components.end())
		return;

	componentLocation->second->UnsetParent(worldTransformStays);

	m_components.erase(componentLocation);
}
GameObject::IComponentPointerCR GameObject::GetComponent(const std::string& name) const
{
	return m_components.at(name);
}
bool GameObject::HasComponent(const std::string& name) const
{
	return m_components.find(name) != m_components.end();
}

const TransformComponent& GameObject::GetTransform() const
{
	return *m_physics->GetTransform();
}
TransformComponent& GameObject::GetTransform()
{
	return *m_physics->GetTransform();
}
