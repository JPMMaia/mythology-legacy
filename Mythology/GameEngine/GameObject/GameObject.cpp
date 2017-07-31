#include "GameObject.h"

using namespace GameEngine;

GameObject::GameObject()
{
}

void GameObject::AddComponent(const std::string& name, IComponent& component)
{
	m_components.emplace(name, &component);
}
void GameObject::RemoveComponent(const std::string& name)
{
	m_components.erase(name);
}
IComponent& GameObject::GetComponent(const std::string& name) const
{
	return *m_components.at(name);
}
bool GameObject::HasComponent(const std::string& name) const
{
	return m_components.find(name) != m_components.end();
}

const TransformComponent& GameObject::Transform() const
{
	return m_transform;
}
TransformComponent& GameObject::Transform()
{
	return m_transform;
}
