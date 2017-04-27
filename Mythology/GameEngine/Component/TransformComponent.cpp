#include "TransformComponent.h"

using namespace GameEngine;
/*
void TransformComponent::Update()
{
	if(m_dirty)
	{
		auto translationMatrix = XMMatrixTranslationFromVector(XMLoadFloat3(&m_translation));
		auto rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&m_rotation));
		auto scaleMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));
		
		m_transform = scaleMatrix * rotationMatrix * translationMatrix;

		m_dirty = false;
	}
}

const DirectX::XMFLOAT3& TransformComponent::Translation() const
{
	return m_translation;
}
void TransformComponent::SetTranslation(const DirectX::XMFLOAT3& translation)
{
	m_translation = translation;
	m_dirty = true;
}

const DirectX::XMFLOAT4& TransformComponent::Rotation() const
{
	return m_rotation;
}
void TransformComponent::SetRotation(const DirectX::XMFLOAT4& rotation)
{
	m_rotation = rotation;
	m_dirty = true;
}

const DirectX::XMFLOAT3& TransformComponent::Scale() const
{
	return m_scale;
}
void TransformComponent::SetScale(const DirectX::XMFLOAT3& scale)
{
	m_scale = scale;
	m_dirty = true;
}
*/