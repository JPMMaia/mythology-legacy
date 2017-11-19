#include "pch.h"
#include "GameObject.h"

using namespace GameEngine;

void GameObject::AddRootComponent(const std::string& name, const std::shared_ptr<BaseComponent>& component)
{
	m_rootName = name;
	m_transform = component->GetSharedTransform();
	m_components.emplace(name, component);
}
void GameObject::AddRootComponent(const std::string& name, const std::shared_ptr<TransformComponent>& component)
{
	m_rootName = name;
	m_transform = component;
	m_components.emplace(name, component);
}
void GameObject::AddComponent(const std::string& name, IComponentPointerCR component, bool worldTransformStays)
{
	assert(m_transform != nullptr);
	component->SetParent(m_transform, worldTransformStays);
	m_components.emplace(name, component);
}
void GameObject::RemoveComponent(const std::string& name, bool worldTransformStays)
{
	if (m_rootName == name)
		throw std::invalid_argument("The root component can't be removed!");

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
const std::shared_ptr<TransformComponent>& GameObject::GetSharedTransform() const
{
	return m_transform;
}
