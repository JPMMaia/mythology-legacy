#include "pch.h"
#include "RigidStaticComponent.h"

using namespace GameEngine;

RigidStaticComponent::RigidStaticComponent(std::unique_ptr<PhysicsEngine::IRigidStatic<>>&& rigidStatic, const std::shared_ptr<TransformComponent>& transform) :
	m_rigidStatic(std::move(rigidStatic))
{
	m_rigidStatic->SetUserData(this);

	Eigen::Vector3f position;
	Eigen::Quaternionf rotation;
	m_rigidStatic->GetGlobalPose(position, rotation);

	transform->SetWorldPosition(position);
	transform->SetWorldRotation(rotation);
}
