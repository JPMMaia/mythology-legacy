#include "PointLightComponent.h"

using namespace GameEngine;

PointLightComponent::PointLightComponent() :
	m_strength(Vector3Type::Zero()),
	m_position(Vector3Type::Zero()),
	m_falloffStart(0.0f),
	m_falloffEnd(0.0f)
{
}
PointLightComponent::PointLightComponent(const Vector3Type& strength, const Vector3Type& position, float falloffStart, float falloffEnd) :
	m_strength(strength),
	m_position(position),
	m_falloffStart(falloffStart),
	m_falloffEnd(falloffEnd)
{
}

const BaseComponent::Vector3Type& PointLightComponent::Strength() const
{
	return m_strength;
}
BaseComponent::Vector3Type& PointLightComponent::Strength()
{
	return m_strength;
}

const BaseComponent::Vector3Type& PointLightComponent::Position() const
{
	return m_position;
}
BaseComponent::Vector3Type& PointLightComponent::Position()
{
	return m_position;
}

float PointLightComponent::FalloffStart() const
{
	return m_falloffStart;
}
float& PointLightComponent::FalloffStart()
{
	return m_falloffStart;
}

float PointLightComponent::FalloffEnd() const
{
	return m_falloffEnd;
}
float& PointLightComponent::FalloffEnd()
{
	return m_falloffEnd;
}
