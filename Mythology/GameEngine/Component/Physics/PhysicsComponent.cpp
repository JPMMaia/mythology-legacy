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

void PhysicsComponent::FrameUpdate(const Common::Timer& timer)
{
	if (m_rigidActor)
	{
		auto transform = PhysicsUtilities::ToEigen(m_rigidActor->getGlobalPose());
		m_transform->SetWorldTransform(transform);
	}
}

const std::shared_ptr<TransformComponent>& PhysicsComponent::GetTransform() const
{
	return m_transform;
}
