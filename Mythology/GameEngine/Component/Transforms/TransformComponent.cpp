#include "TransformComponent.h"

using namespace Eigen;
using namespace GameEngine;

TransformComponent::IDType TransformComponent::s_count = 0;

TransformComponent::TransformComponent() :
	m_id(s_count++),
	m_localPosition(Vector3Type::Zero()),
	m_localRotation(QuaternionType::Identity()),
	m_localScaling(Vector3Type(1.0f, 1.0f, 1.0f))
{
}
TransformComponent::TransformComponent(const Vector3Type& localPosition, const QuaternionType& localRotation, const Vector3Type& localScaling) :
	m_id(s_count++),
	m_localPosition(localPosition),
	m_localRotation(localRotation),
	m_localScaling(localScaling)
{
}

void TransformComponent::FixedUpdate(const Common::Timer& timer)
{
}

const TransformComponent::Vector3Type& TransformComponent::LocalPosition() const
{
	return m_localPosition;
}
void TransformComponent::SetLocalPosition(const Vector3Type& translation)
{
	m_localPosition = translation;
	UpdateMatrix();
}

const TransformComponent::QuaternionType& TransformComponent::LocalRotation() const
{
	return m_localRotation;
}
void TransformComponent::SetLocalRotation(const QuaternionType& rotation)
{
	m_localRotation = rotation;
	m_localRotation.normalize();
	UpdateMatrix();
}

const TransformComponent::Vector3Type& TransformComponent::LocalScaling() const
{
	return m_localScaling;
}
void TransformComponent::SetLocalScaling(const Vector3Type& scaling)
{
	m_localScaling = scaling;
	UpdateMatrix();
}

const TransformComponent::Vector3Type& TransformComponent::WorldPosition() const
{
	static auto x = CalculateParentsTransform() * m_localPosition;
	return x;
}
void TransformComponent::SetWorldPosition(const Vector3Type& translation)
{
}

const TransformComponent::QuaternionType& TransformComponent::WorldRotation() const
{
	return m_localRotation;
}
void TransformComponent::SetWorldRotation(const QuaternionType& rotation)
{
}

const TransformComponent::Vector3Type& TransformComponent::WorldScaling() const
{
	static auto x = CalculateParentsTransform() * m_localScaling;
	return x;
}
void TransformComponent::SetWorldScaling(const Vector3Type& scale)
{
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
		parentLocked->UpdateMatrix();

		// Left-multiply world transform by the parent's world inverse transform in order to hold the same world position:
		auto parentWorldInverseTransform = parentLocked->WorldTransform().inverse();
		auto localTransform = parentWorldInverseTransform * CalculateLocalTransform();

		// Apply translation:
		m_localPosition = localTransform.translation();

		// Apply rotation and scaling:
		Matrix3f rotationMatrix, scalingMatrix;
		localTransform.computeRotationScaling(&rotationMatrix, &scalingMatrix);
		m_localRotation = Eigen::AngleAxis<float>(rotationMatrix);
		m_localScaling = scalingMatrix.diagonal();
	}
}

const TransformComponent::TransformType& TransformComponent::WorldTransform() const
{
	return CalculateParentsTransform() * CalculateLocalTransform();
}

TransformComponent::TransformType TransformComponent::CalculateLocalTransform() const
{
	auto transform(TransformType::Identity());
	transform.translate(m_localPosition);
	transform.rotate(m_localRotation);
	transform.scale(m_localScaling);
	return transform;
}
TransformComponent::TransformType TransformComponent::CalculateParentsTransform() const
{
	auto transform(TransformType::Identity());
	
	if(!m_parent.expired())
	{
		auto parent = m_parent.lock();

		TransformType parentTransform;
		if (parent->GetParent().expired())
			parentTransform = parent->CalculateLocalTransform();
		else
			parentTransform = parent->CalculateParentsTransform();

		transform = parentTransform * transform;
	}

	return transform;
}

void TransformComponent::UpdateMatrix()
{
	// TODO test for parent validity:

	/*// Update world transform using local parameters:
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
	}*/
}
