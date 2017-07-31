#include "SpotLightComponent.h"

using namespace GameEngine;

SpotLightComponent::SpotLightComponent() :
	m_strength(Vector3Type::Zero()),
	m_position(Vector3Type::Zero()),
	m_direction(Vector3Type::Zero()),
	m_falloffStart(0.0f),
	m_falloffEnd(0.0f),
	m_spotPower(0.0f)
{
}
SpotLightComponent::SpotLightComponent(const Vector3Type& strength, const Vector3Type& position, const Vector3Type& direction, float falloffStart, float falloffEnd, float spotPower) :
	m_strength(strength),
	m_position(position),
	m_direction(direction),
	m_falloffStart(falloffStart),
	m_falloffEnd(falloffEnd),
	m_spotPower(spotPower)
{
}

const BaseComponent::Vector3Type& SpotLightComponent::Strength() const
{
	return m_strength;
}
BaseComponent::Vector3Type& SpotLightComponent::Strength()
{
	return m_strength;
}

const BaseComponent::Vector3Type& SpotLightComponent::Position() const
{
	return m_position;
}
BaseComponent::Vector3Type& SpotLightComponent::Position()
{
	return m_position;
}

const BaseComponent::Vector3Type& SpotLightComponent::Direction() const
{
	return m_direction;
}
BaseComponent::Vector3Type& SpotLightComponent::Direction()
{
	return m_direction;
}

float SpotLightComponent::FalloffStart() const
{
	return m_falloffStart;
}
float& SpotLightComponent::FalloffStart()
{
	return m_falloffStart;
}

float SpotLightComponent::FalloffEnd() const
{
	return m_falloffEnd;
}
float& SpotLightComponent::FalloffEnd()
{
	return m_falloffEnd;
}

float SpotLightComponent::SpotPower() const
{
	return m_spotPower;
}
float& SpotLightComponent::SpotPower()
{
	return m_spotPower;
}
