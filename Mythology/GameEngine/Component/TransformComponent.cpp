#include "TransformComponent.h"
#include "Libraries/tue/transform.hpp"

using namespace GameEngine;

void TransformComponent::FixedUpdate(const Common::Timer& timer)
{
	if (m_dirty)
	{
		auto translationMatrix = tue::transform::translation_mat(m_translation);
		auto rotationMatrix = tue::transform::rotation_mat(m_rotation);
		auto scaleMatrix = tue::transform::scale_mat(m_scale);

		m_transform = scaleMatrix * rotationMatrix * translationMatrix;

		m_dirty = false;
	}
}

const TransformComponent::ThreeDType& TransformComponent::Translation() const
{
	return m_translation;
}

void TransformComponent::SetTranslation(const ThreeDType& translation)
{
	m_translation = translation;
	m_dirty = true;
}

const TransformComponent::QuaternionType& TransformComponent::Rotation() const
{
	return m_rotation;
}

void TransformComponent::SetRotation(const QuaternionType& rotation)
{
	m_rotation = rotation;
	m_dirty = true;
}

const TransformComponent::ThreeDType& TransformComponent::Scale() const
{
	return m_scale;
}

void TransformComponent::SetScale(const ThreeDType& scale)
{
	m_scale = scale;
	m_dirty = true;
}
