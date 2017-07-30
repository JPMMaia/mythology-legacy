#include "TransformComponent.h"
#include "Libraries/tue/transform.hpp"

using namespace GameEngine;

TransformComponent::IDType TransformComponent::s_count = 0;

TransformComponent::TransformComponent() :
	m_id(s_count++),
	m_translation(tue::fvec3::zero()),
	m_rotation(tue::fquat::identity()),
	m_scale(tue::fvec3(1.0f, 1.0f, 1.0f))
{
}

void TransformComponent::FixedUpdate(const Common::Timer& timer)
{
	UpdateMatrix();
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

const std::weak_ptr<TransformComponent>& TransformComponent::GetParent() const
{
	return m_parent;
}
void TransformComponent::SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays)
{
	// If has parent, remove child:
	if (!m_parent.expired())
	{
		auto oldParent = m_parent.lock();
		oldParent->m_children.erase(oldParent->m_children.find(m_id));
	}

	// Set parent:
	m_parent = parent;

	// Add as a child to parent:
	auto parentLocked = m_parent.lock();
	parentLocked->m_children.emplace(m_id, shared_from_this());

	if (worldPositionStays)
	{
		// Ensure that parent is updated:
		if (parentLocked->m_dirty)
			parentLocked->UpdateMatrix();

		auto parentWorldTransform = parentLocked->WorldTransform();
		
	}	

	m_dirty = true;
}

const TransformComponent::MatrixType& TransformComponent::WorldTransform() const
{
	return m_worldTransform;
}

void TransformComponent::UpdateMatrix()
{
	if (m_dirty)
	{
		auto translationMatrix = tue::transform::translation_mat(m_translation);
		auto rotationMatrix = tue::transform::rotation_mat(m_rotation);
		auto scaleMatrix = tue::transform::scale_mat(m_scale);

		// Update world transform using local parameters:
		m_worldTransform = scaleMatrix * rotationMatrix * translationMatrix;

		// If parent is valid, then apply parent transform:
		if (!m_parent.expired())
		{
			auto lockedParent = m_parent.lock();

			// Ensure that parent is updated:
			if (lockedParent->m_dirty)
				lockedParent->UpdateMatrix();

			// Apply parent transform:
			m_worldTransform = lockedParent->m_worldTransform * m_worldTransform;
		}

		m_dirty = false;
	}
}
