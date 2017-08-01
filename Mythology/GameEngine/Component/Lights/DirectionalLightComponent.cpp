#include "DirectionalLightComponent.h"

using namespace GameEngine;

BaseComponent::Vector3Type DirectionalLightComponent::s_initialDirection(0.0f, -1.0f, 0.0f);

DirectionalLightComponent::DirectionalLightComponent() :
	m_strength(Vector3Type::Zero())
{
}
DirectionalLightComponent::DirectionalLightComponent(const Vector3Type& strength) :
	m_strength(strength)
{
}

const BaseComponent::Vector3Type& DirectionalLightComponent::GetStrength() const
{
	return m_strength;
}
void DirectionalLightComponent::SetStrength(const Vector3Type& strength)
{
	m_strength = strength;
}

BaseComponent::Vector3Type DirectionalLightComponent::GetLocalDirection() const
{
	return GetTransform().GetLocalRotation().toRotationMatrix() * s_initialDirection;
}
void DirectionalLightComponent::SetLocalDirection(const Vector3Type& localDirection)
{
	GetTransform().SetLocalRotation(QuaternionType::FromTwoVectors(s_initialDirection, localDirection));
}
