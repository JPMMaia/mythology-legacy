#include "TransformComponent.h"

using namespace Eigen;
using namespace GameEngine;

TransformComponent::IDType TransformComponent::s_count = 0;

TransformComponent::TransformComponent() :
	m_id(s_count++),
	m_localTranslation(Vector3Type::Zero()),
	m_localRotation(QuaternionType::Identity()),
	m_localScaling(Vector3Type(1.0f, 1.0f, 1.0f))
{
}
TransformComponent::TransformComponent(const Vector3Type& localPosition, const QuaternionType& localRotation, const Vector3Type& localScaling) :
	m_id(s_count++),
	m_localTranslation(localPosition),
	m_localRotation(localRotation),
	m_localScaling(localScaling)
{
}

void TransformComponent::FixedUpdate(const Common::Timer& timer)
{
}

const TransformComponent::Vector3Type& TransformComponent::LocalTranslation() const
{
	return m_localTranslation;
}
void TransformComponent::SetLocalTranslation(const Vector3Type& localTranslation)
{
	m_localTranslation = localTranslation;
}

const TransformComponent::QuaternionType& TransformComponent::LocalRotation() const
{
	return m_localRotation;
}
void TransformComponent::SetLocalRotation(const QuaternionType& localRotation)
{
	m_localRotation = localRotation;
	m_localRotation.normalize();
}

const TransformComponent::Vector3Type& TransformComponent::LocalScaling() const
{
	return m_localScaling;
}
void TransformComponent::SetLocalScaling(const Vector3Type& localScaling)
{
	m_localScaling = localScaling;
}

TransformComponent::Vector3Type TransformComponent::WorldPosition() const
{
	// Apply parent's transform:
	return CalculateParentsTransform() * m_localTranslation;
}
void TransformComponent::SetWorldPosition(const Vector3Type& worldPosition)
{
	// Apply inverse of parent's transform:
	m_localTranslation = CalculateParentsTransform().inverse() * worldPosition;
}

TransformComponent::QuaternionType TransformComponent::WorldRotation() const
{
	auto rotation = m_localRotation;

	auto transform = shared_from_this();
	while (!transform->m_parent.expired())
	{
		transform = transform->m_parent.lock();
		rotation = transform->m_localRotation * rotation;
	}

	return rotation;
}
void TransformComponent::SetWorldRotation(const QuaternionType& worldRotation)
{
	auto parentsRotation = QuaternionType::Identity();

	auto transform = shared_from_this();
	while (!transform->m_parent.expired())
	{
		transform = transform->m_parent.lock();
		parentsRotation = transform->m_localRotation * parentsRotation;
	}

	m_localRotation = parentsRotation.inverse() * worldRotation;
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
		// Left-multiply world transform by the parent's world inverse transform in order to hold the same world position:
		auto parentWorldInverseTransform = parentLocked->WorldTransform().inverse();
		auto localTransform = parentWorldInverseTransform * CalculateLocalTransform();

		// Apply translation:
		m_localTranslation = localTransform.translation();

		// Apply rotation and scaling:
		Matrix3f rotationMatrix, scalingMatrix;
		localTransform.computeRotationScaling(&rotationMatrix, &scalingMatrix);
		m_localRotation = Eigen::AngleAxis<float>(rotationMatrix);
		m_localScaling = scalingMatrix.diagonal();
	}
}

TransformComponent::TransformType TransformComponent::WorldTransform() const
{
	return CalculateParentsTransform() * CalculateLocalTransform();
}

TransformComponent::TransformType TransformComponent::CalculateLocalTransform() const
{
	auto transform(TransformType::Identity());
	transform.translate(m_localTranslation);
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
