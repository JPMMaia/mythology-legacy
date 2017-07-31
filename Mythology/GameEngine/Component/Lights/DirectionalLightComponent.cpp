#include "DirectionalLightComponent.h"

using namespace GameEngine;

DirectionalLightComponent::DirectionalLightComponent() :
	m_strength(Vector3Type::Zero()),
	m_direction(Vector3Type::Zero())
{
}
DirectionalLightComponent::DirectionalLightComponent(const Vector3Type& strength, const Vector3Type& direction) :
	m_strength(strength),
	m_direction(direction)
{
}

const BaseComponent::Vector3Type& DirectionalLightComponent::Strength() const
{
	return m_strength;
}
BaseComponent::Vector3Type& DirectionalLightComponent::Strength()
{
	return m_strength;
}

const BaseComponent::Vector3Type& DirectionalLightComponent::Direction() const
{
	return m_direction;
}
BaseComponent::Vector3Type& DirectionalLightComponent::Direction()
{
	return m_direction;
}
