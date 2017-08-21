#include "PointLightComponent.h"

using namespace GameEngine;

PointLightComponent::PointLightComponent() :
	m_strength(Vector3Type::Zero()),
	m_falloffStart(0.0f),
	m_falloffEnd(0.0f)
{
}
PointLightComponent::PointLightComponent(const Vector3Type& strength, float falloffStart, float falloffEnd) :
	m_strength(strength),
	m_falloffStart(falloffStart),
	m_falloffEnd(falloffEnd)
{
}

const BaseComponent::Vector3Type& PointLightComponent::GetStrength() const
{
	return m_strength;
}
void PointLightComponent::SetStrength(const Vector3Type& strength)
{
	m_strength = strength;
}

float PointLightComponent::GetFalloffStart() const
{
	return m_falloffStart;
}
void PointLightComponent::SetFalloffStart(float falloffStart)
{
	m_falloffStart = falloffStart;
}

float PointLightComponent::GetFalloffEnd() const
{
	return m_falloffEnd;
}
void PointLightComponent::SetFalloffEnd(float falloffEnd)
{
	m_falloffEnd = falloffEnd;
}

BaseComponent::Vector3Type PointLightComponent::GetLocalPosition() const
{
	return GetTransform().GetLocalPosition();
}
void PointLightComponent::SetLocalPosition(const Vector3Type& localPosition)
{
	GetTransform().SetLocalPosition(localPosition);
}

BaseComponent::Vector3Type PointLightComponent::GetWorldPosition() const
{
	return GetTransform().GetWorldPosition();
}
