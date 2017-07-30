#include "TransformComponent.h"

using namespace Eigen;
using namespace GameEngine;

TransformComponent::IDType TransformComponent::s_count = 0;

TransformComponent::TransformComponent() :
	m_id(s_count++),
	m_translation({0.0f, 0.0f, 0.0f}),
	m_rotation(Quaternion<float>(1.0f, 0.0f, 0.0f, 0.0f)),
	m_scale({1.0f, 1.0f, 1.0f})
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
	m_rotation.normalize();
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

		// Left-multiply world transform by the parent's world inverse transform in order to hold the same world position:
		auto parentWorldInverseTransform = parentLocked->WorldTransform().inverse();
		auto localTransform = parentWorldInverseTransform * CalculateLocalTransform();

		// Apply translation:
		m_translation = localTransform.translation();

		// Apply rotation and scaling:
		Matrix3f rotationMatrix, scalingMatrix;
		localTransform.computeRotationScaling(&rotationMatrix, &scalingMatrix);
		m_rotation = Eigen::AngleAxis<float>(rotationMatrix);
		m_scale = scalingMatrix.diagonal();
	}

	m_dirty = true;
}

const Eigen::Transform<float, 3, Eigen::Projective>& TransformComponent::WorldTransform() const
{
	return m_worldTransform;
}

Eigen::Transform<float, 3, Eigen::Projective> TransformComponent::CalculateLocalTransform() const
{
	Eigen::Transform<float, 3, Eigen::Projective> transform(Eigen::Transform<float, 3, Eigen::Projective>::Identity());
	transform.translate(m_translation);
	transform.rotate(m_rotation);
	transform.scale(m_scale);
	return transform;
}
void TransformComponent::UpdateMatrix()
{
	if (m_dirty)
	{
		// Update world transform using local parameters:
		m_worldTransform = CalculateLocalTransform();

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
