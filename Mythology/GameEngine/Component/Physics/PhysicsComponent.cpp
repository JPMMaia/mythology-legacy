#include "pch.h"
#include "PhysicsComponent.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

using namespace GameEngine;
using namespace physx;

PhysicsComponent::PhysicsComponent(const std::shared_ptr<TransformComponent>& transform) :
	m_transform(transform),
	m_rigidActor(nullptr)
{
}
PhysicsComponent::PhysicsComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidActor>& rigidActor) :
	m_transform(transform),
	m_rigidActor(rigidActor)
{
}

void PhysicsComponent::FrameUpdate(const Common::Timer& timer)
{
	if (m_rigidActor)
	{		
		auto transform = m_rigidActor->getGlobalPose();
		
		const auto& position = transform.p;
		m_transform->SetLocalPosition(Eigen::Vector3f(position.x, position.y, position.z));
		
		const auto& rotation = transform.q;
		m_transform->SetLocalRotation(Eigen::Quaternionf(rotation.w, rotation.x, rotation.y, rotation.z));
	}
}

const TransformComponent& PhysicsComponent::GetTransform() const
{
	return *m_transform;
}
