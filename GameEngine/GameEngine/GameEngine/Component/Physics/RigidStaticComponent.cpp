#include "pch.h"
#include "RigidStaticComponent.h"

using namespace GameEngine;

RigidStaticComponent::RigidStaticComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidStatic>& rigidStatic) :
	BaseComponent(transform),
	m_rigidStatic(rigidStatic)
{
	auto globalPose = m_rigidStatic->getGlobalPose();

	const auto& position = globalPose.p;
	transform->SetLocalPosition(Eigen::Vector3f(position.x, position.y, position.z));

	const auto& rotation = globalPose.q;
	transform->SetLocalRotation(Eigen::Quaternionf(rotation.w, rotation.x, rotation.y, rotation.z));
}
