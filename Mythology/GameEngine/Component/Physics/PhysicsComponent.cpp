#include "pch.h"
#include "PhysicsComponent.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

using namespace GameEngine;
using namespace physx;

PhysicsComponent::PhysicsComponent() :
	m_transform(std::make_shared<TransformComponent>()),
	m_rigidActor(nullptr)
{
}
PhysicsComponent::PhysicsComponent(const PhysXSharedPointer<physx::PxRigidActor>& rigidActor) :
	m_transform(std::make_shared<TransformComponent>()),
	m_rigidActor(rigidActor)
{
}

const std::shared_ptr<TransformComponent>& PhysicsComponent::GetTransform() const
{
	if (m_rigidActor)
	{
		auto transform = PhysicsUtilities::PhysXToEigenTransform(m_rigidActor->getGlobalPose());
		m_transform->SetWorldTransform(transform);
	}

	return m_transform;
}
