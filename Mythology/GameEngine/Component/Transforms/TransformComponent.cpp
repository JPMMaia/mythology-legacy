﻿#include "pch.h"
#include "TransformComponent.h"

using namespace Eigen;
using namespace GameEngine;

TransformComponent::IDType TransformComponent::s_count = 0;

TransformComponent::TransformComponent() :
	m_id(s_count++),
	m_localPosition(Vector3Type::Zero()),
	m_localRotation(QuaternionType::Identity()),
	m_localScale(Vector3Type(1.0f, 1.0f, 1.0f)),
	m_isWorldTransformDirty(true)
{
}
TransformComponent::TransformComponent(Vector3CRType localPosition, QuaternionCRType localRotation, Vector3CRType localScaling) :
	m_id(s_count++),
	m_localPosition(localPosition),
	m_localRotation(localRotation),
	m_localScale(localScaling),
	m_isWorldTransformDirty(true)
{
}

void TransformComponent::FixedUpdate(const Common::Timer& timer)
{
}

void TransformComponent::Move(Vector3CRType axis, float scalar)
{
	SetLocalPosition(GetLocalPosition() + scalar * axis);
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
	SetLocalRotation(GetLocalRotation() * QuaternionType(AngleAxisf(radians, axis)));
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
	auto rotationMatrix = GetLocalRotation().toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 0), rotationMatrix(1, 0), rotationMatrix(2, 0));
}
TransformComponent::Vector3Type TransformComponent::GetLocalY() const
{
	auto rotationMatrix = GetLocalRotation().toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 1), rotationMatrix(1, 1), rotationMatrix(2, 1));
}
TransformComponent::Vector3Type TransformComponent::GetLocalZ() const
{
	auto rotationMatrix = GetLocalRotation().toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 2), rotationMatrix(1, 2), rotationMatrix(2, 2));
}

TransformComponent::Vector3Type TransformComponent::GetWorldX() const
{
	auto rotationMatrix = GetWorldRotation().toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 0), rotationMatrix(1, 0), rotationMatrix(2, 0));
}
TransformComponent::Vector3Type TransformComponent::GetWorldY() const
{
	auto rotationMatrix = GetWorldRotation().toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 1), rotationMatrix(1, 1), rotationMatrix(2, 1));
}
TransformComponent::Vector3Type TransformComponent::GetWorldZ() const
{
	auto rotationMatrix = GetWorldRotation().toRotationMatrix();
	return Vector3Type(rotationMatrix(0, 2), rotationMatrix(1, 2), rotationMatrix(2, 2));
}

const TransformComponent::Vector3Type& TransformComponent::GetLocalPosition() const
{
	return m_localPosition;
}
void TransformComponent::SetLocalPosition(Vector3CRType localPosition)
{
	m_localPosition = localPosition;
	InvalidateWorldTransform();
}

const TransformComponent::QuaternionType& TransformComponent::GetLocalRotation() const
{
	return m_localRotation;
}
void TransformComponent::SetLocalRotation(QuaternionCRType localRotation)
{
	m_localRotation = localRotation;
	m_localRotation.normalize();
	InvalidateWorldTransform();
}

const TransformComponent::Vector3Type& TransformComponent::GetLocalScale() const
{
	return m_localScale;
}
void TransformComponent::SetLocalScaling(Vector3CRType localScaling)
{
	m_localScale = localScaling;
	InvalidateWorldTransform();
}

TransformComponent::Vector3Type TransformComponent::GetWorldPosition() const
{
	// Apply parent's transform:
	return GetParentWorldTransform() * GetLocalPosition();
}
void TransformComponent::SetWorldPosition(Vector3CRType worldPosition)
{
	// Apply inverse of parent's transform:
	SetLocalPosition(GetParentWorldTransform().inverse() * worldPosition);
}

TransformComponent::QuaternionType TransformComponent::GetWorldRotation() const
{
	auto rotation = GetLocalRotation();

	auto transform = this;
	while (!transform->GetParent().expired())
	{
		transform = transform->GetParent().lock().get();
		rotation = transform->GetLocalRotation() * rotation;
	}

	return rotation;
}
void TransformComponent::SetWorldRotation(QuaternionCRType worldRotation)
{
	auto parentsRotation = QuaternionType::Identity();

	auto transform = this;
	while (!transform->GetParent().expired())
	{
		transform = transform->GetParent().lock().get();
		parentsRotation = transform->GetLocalRotation() * parentsRotation;
	}

	SetLocalRotation(parentsRotation.inverse() * worldRotation);
}

const std::weak_ptr<TransformComponent>& TransformComponent::GetParent() const
{
	return m_parent;
}
void TransformComponent::SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldTransformStays)
{
	if (!parent.expired() && this == parent.lock().get())
		return;

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
	if (!m_parent.expired())
	{
		// Add as a child to parent:
		auto newParent = m_parent.lock();
		newParent->m_children.emplace(m_id, shared_from_this());

		if (worldTransformStays)
			UpdateTransformValuesToHoldWorldTransform(newParent, true);
	}

	if (!worldTransformStays)
		InvalidateWorldTransform();
}
void TransformComponent::UnsetParent(bool worldTransformStays)
{
	SetParent({}, worldTransformStays);
}

TransformComponent::TransformType TransformComponent::GetLocalTransform() const
{
	auto transform(TransformType::Identity());
	transform.translate(m_localPosition);
	transform.rotate(m_localRotation);
	transform.scale(m_localScale);
	return transform;
}
void TransformComponent::SetLocalTransform(const TransformType& localTransform)
{
	SetLocalTransform(localTransform, true);
}

TransformComponent::TransformType TransformComponent::GetWorldTransform() const
{
	if (m_isWorldTransformDirty)
	{
		m_worldTransform = GetParentWorldTransform() * GetLocalTransform();
		m_isWorldTransformDirty = false;
	}

	return m_worldTransform;
}
void TransformComponent::SetWorldTransform(const TransformType& worldTransform)
{
	if (m_worldTransform.isApprox(worldTransform))
		return;

	auto localTransform = worldTransform;
	if (!m_parent.expired())
	{
		// Get the parent's world transform:
		auto parentWorldTransform = m_parent.lock()->GetWorldTransform();

		// Apply parent's transform to calculate the local transform:
		localTransform = parentWorldTransform.inverse() * localTransform;
	}

	// Set local transform:
	SetLocalTransform(localTransform, false);

	// Set world transform:
	m_worldTransform = worldTransform;
	m_isWorldTransformDirty = false;

	// Invalidate all children's world transforms:
	for (auto& child : m_children)
	{
		child.second.lock()->InvalidateWorldTransform();
	}
}

TransformComponent::TransformType TransformComponent::GetParentWorldTransform() const
{
	if (m_parent.expired())
		return TransformType::Identity();

	return m_parent.lock()->GetWorldTransform();
}
void TransformComponent::UpdateTransformValuesToHoldWorldTransform(const std::shared_ptr<TransformComponent>& parent, bool isNewParent)
{
	// Get the parent's world transform:
	auto parentWorldTransform = parent->GetWorldTransform();

	// If it is a new parent, we need the inverse transform:
	if (isNewParent)
		parentWorldTransform = parentWorldTransform.inverse();

	// Apply parent's transform to calculate the local transform:
	auto localTransform = parentWorldTransform * GetLocalTransform();

	// Set local transform:
	SetLocalTransform(localTransform);
}

void TransformComponent::InvalidateWorldTransform()
{
	// Invalidate world transform:
	m_isWorldTransformDirty = true;

	// Invalidate all children's world transforms:
	for (auto& child : m_children)
	{
		child.second.lock()->InvalidateWorldTransform();
	}
}

void TransformComponent::SetLocalTransform(const TransformType& localTransform, bool invalidateWorldTransform)
{
	// Apply rotation and scaling:
	Matrix3f rotationMatrix, scalingMatrix;
	localTransform.computeRotationScaling(&rotationMatrix, &scalingMatrix);
	m_localScale = scalingMatrix.diagonal();
	m_localRotation = Eigen::AngleAxis<float>(rotationMatrix);

	// Apply translation:
	m_localPosition = localTransform.translation();

	assert(GetLocalTransform().isApprox(localTransform));

	if (invalidateWorldTransform)
		InvalidateWorldTransform();
}
