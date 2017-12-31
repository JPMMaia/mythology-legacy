#include "pch.h"
#include "SpotLightComponent.h"

using namespace GameEngine;

BaseComponent::Vector3Type SpotLightComponent::s_initialDirection(0.0f, -1.0f, 0.0f);

SpotLightComponent::SpotLightComponent() :
	m_strength(Vector3Type::Zero()),
	m_falloffStart(0.0f),
	m_falloffEnd(0.0f),
	m_spotPower(0.0f)
{
}
SpotLightComponent::SpotLightComponent(const Vector3Type& strength, float falloffStart, float falloffEnd, float spotPower) :
	m_strength(strength),
	m_falloffStart(falloffStart),
	m_falloffEnd(falloffEnd),
	m_spotPower(spotPower)
{
}

const BaseComponent::Vector3Type& SpotLightComponent::GetStrength() const
{
	return m_strength;
}
void SpotLightComponent::SetStrength(const Vector3Type& strength)
{
	m_strength = strength;
}

float SpotLightComponent::GetFalloffStart() const
{
	return m_falloffStart;
}
void SpotLightComponent::SetFalloffStart(float falloffStart)
{
	m_falloffStart = falloffStart;
}

float SpotLightComponent::GetFalloffEnd() const
{
	return m_falloffEnd;
}
void SpotLightComponent::SetFalloffEnd(float falloffEnd)
{
	m_falloffEnd = falloffEnd;
}

float SpotLightComponent::GetSpotPower() const
{
	return m_spotPower;
}
void SpotLightComponent::SetSpotPower(float spotPower)
{
	m_spotPower = spotPower;
}

BaseComponent::Vector3Type SpotLightComponent::GetLocalDirection() const
{
	return GetTransform().GetLocalRotation().toRotationMatrix() * s_initialDirection;
}
void SpotLightComponent::SetLocalDirection(const Vector3Type& localDirection)
{
	GetTransform().SetLocalRotation(QuaternionType::FromTwoVectors(s_initialDirection, localDirection));
}

BaseComponent::Vector3Type SpotLightComponent::GetLocalPosition() const
{
	return GetTransform().GetLocalPosition();
}
void SpotLightComponent::SetLocalPosition(const Vector3Type& localPosition)
{
	GetTransform().SetLocalPosition(localPosition);
}

BaseComponent::Vector3Type SpotLightComponent::GetWorldPosition() const
{
	return GetTransform().GetWorldPosition();
}
