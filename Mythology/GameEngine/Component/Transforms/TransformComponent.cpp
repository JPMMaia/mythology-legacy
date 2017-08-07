﻿#include "TransformComponent.h"

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
TransformComponent::TransformComponent(Vector3CRType localPosition, QuaternionCRType localRotation, Vector3CRType localScaling) :
	m_id(s_count++),
	m_localPosition(localPosition),
	m_localRotation(localRotation),
	m_localScaling(localScaling)
{
}

void TransformComponent::FixedUpdate(const Common::Timer& timer)
{
}

void TransformComponent::Move(Vector3CRType axis, float scalar)
{
	m_localPosition += scalar * axis;
}
void TransformComponent::MoveLocalX(float scalar)
{
	Move(GetLocalX(), scalar);
}
void TransformComponent::MoveLocalY(float scalar)
{	
	Move(GetLocalY(), scalar);
}
void TransformComponent::MoveLocalZ(float scalar)
{
	Move(GetLocalZ(), scalar);
}

void TransformComponent::Rotate(Vector3CRType axis, float radians)
{
	m_localRotation *= QuaternionType(AngleAxisf(radians, axis));
}
void TransformComponent::RotateLocalX(float radians)
{
	Rotate(GetLocalX(), radians);
}
void TransformComponent::RotateLocalY(float radians)
{
	Rotate(GetLocalY(), radians);
}
void TransformComponent::RotateLocalZ(float radians)
{
	Rotate(GetLocalZ(), radians);
}

TransformComponent::Vector3Type TransformComponent::GetLocalX() const
{
	auto rotationMatrix = m_localRotation.toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 0), rotationMatrix(1, 0), rotationMatrix(2, 0));
}
TransformComponent::Vector3Type TransformComponent::GetLocalY() const
{
	auto rotationMatrix = m_localRotation.toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 1), rotationMatrix(1, 1), rotationMatrix(2, 1));
}
TransformComponent::Vector3Type TransformComponent::GetLocalZ() const
{
	auto rotationMatrix = m_localRotation.toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 2), rotationMatrix(1, 2), rotationMatrix(2, 2));
}

const TransformComponent::Vector3Type& TransformComponent::GetLocalPosition() const
{
	return m_localPosition;
}
void TransformComponent::SetLocalPosition(Vector3CRType localPosition)
{
	m_localPosition = localPosition;
}

const TransformComponent::QuaternionType& TransformComponent::GetLocalRotation() const
{
	return m_localRotation;
}
void TransformComponent::SetLocalRotation(QuaternionCRType localRotation)
{
	m_localRotation = localRotation;
	m_localRotation.normalize();
}

const TransformComponent::Vector3Type& TransformComponent::GetLocalScaling() const
{
	return m_localScaling;
}
void TransformComponent::SetLocalScaling(Vector3CRType localScaling)
{
	m_localScaling = localScaling;
}

TransformComponent::Vector3Type TransformComponent::GetWorldPosition() const
{
	// Apply parent's transform:
	return CalculateParentsTransform() * m_localPosition;
}
void TransformComponent::SetWorldPosition(Vector3CRType worldPosition)
{
	// Apply inverse of parent's transform:
	m_localPosition = CalculateParentsTransform().inverse() * worldPosition;
}

TransformComponent::QuaternionType TransformComponent::GetWorldRotation() const
{
	auto rotation = m_localRotation;

	auto transform = this;
	while (!transform->m_parent.expired())
	{
		transform = transform->m_parent.lock().get();
		rotation = transform->m_localRotation * rotation;
	}

	return rotation;
}
void TransformComponent::SetWorldRotation(QuaternionCRType worldRotation)
{
	auto parentsRotation = QuaternionType::Identity();

	auto transform = this;
	while (!transform->m_parent.expired())
	{
		transform = transform->m_parent.lock().get();
		parentsRotation = transform->m_localRotation * parentsRotation;
	}

	m_localRotation = parentsRotation.inverse() * worldRotation;
}

const std::weak_ptr<TransformComponent>& TransformComponent::GetParent() const
{
	return m_parent;
}
void TransformComponent::SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays)
{
	// If has parent, remove child:
	if (!m_parent.expired())
	{
		auto oldParent = m_parent.lock();
		oldParent->m_children.erase(oldParent->m_children.find(m_id));

		if (worldTransformStays)
			UpdateTransformValuesToHoldWorldTransform(oldParent, false);
	}

	// Set parent:
	m_parent = parent;

	// If new parent is valid:
	if(!m_parent.expired())
	{
		// Add as a child to parent:
		auto newParent = m_parent.lock();
		newParent->m_children.emplace(m_id, shared_from_this());

		if (worldTransformStays)
			UpdateTransformValuesToHoldWorldTransform(newParent, true);
	}
}
void TransformComponent::UnsetParent(bool worldTransformStays)
{
	SetParent({}, worldTransformStays);
}

TransformComponent::TransformType TransformComponent::GetWorldTransform() const
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
void TransformComponent::UpdateTransformValuesToHoldWorldTransform(const std::shared_ptr<TransformComponent>& parent, bool isNewParent)
{
	// Get the parent's world transform:
	auto parentWorldTransform = parent->GetWorldTransform();

	// If it is a new parent, we need the inverse transform:
	if (isNewParent)
		parentWorldTransform = parentWorldTransform.inverse();

	// Apply parent's transform:
	auto localTransform = parentWorldTransform * CalculateLocalTransform();

	// Apply translation:
	m_localPosition = localTransform.translation();

	// Apply rotation and scaling:
	Matrix3f rotationMatrix, scalingMatrix;
	localTransform.computeRotationScaling(&rotationMatrix, &scalingMatrix);
	m_localRotation = Eigen::AngleAxis<float>(rotationMatrix);
	m_localScaling = scalingMatrix.diagonal();
}