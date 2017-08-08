#include "GameObject.h"

using namespace GameEngine;

GameObject::GameObject() :
	m_transform(new TransformComponent)
{
}

void GameObject::AddComponent(const std::string& name, IComponentPointerCR component, bool worldTransformStays)
{
	component->SetParent(m_transform, worldTransformStays);
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
	return *m_transform;
}
TransformComponent& GameObject::GetTransform()
{
	return *m_transform;
}
