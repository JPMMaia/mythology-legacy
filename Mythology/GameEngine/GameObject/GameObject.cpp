#include "GameObject.h"

using namespace GameEngine;

void GameObject::AddComponent(const std::string& name, const std::weak_ptr<IComponent>& component)
{
	m_components.emplace(name, component);
}
void GameObject::RemoveComponent(const std::string& name)
{
	m_components.erase(name);
}
std::weak_ptr<IComponent> GameObject::GetComponent(const std::string& name) const
{
	return m_components.at(name);
}
