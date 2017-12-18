#include "pch.h"
#include "RigidDynamicComponent.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

using namespace GameEngine;
using namespace physx;

RigidDynamicComponent::RigidDynamicComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidDynamic>& rigidDynamic) :
	BaseComponent(transform),
	m_rigidDynamic(rigidDynamic)
{
	m_rigidDynamic->userData = this;
	m_rigidDynamic->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
}

void RigidDynamicComponent::FrameUpdate(const Common::Timer& timer)
{
	UpdateTransform();
}

void RigidDynamicComponent::UpdateTransform()
{
	if (!m_rigidDynamic->isSleeping())
	{
		auto globalPose = m_rigidDynamic->getGlobalPose();
		auto& transform = GetTransform();

		const auto& position = globalPose.p;
		transform.SetLocalPosition(Eigen::Vector3f(position.x, position.y, position.z));

		const auto& rotation = globalPose.q;
		transform.SetLocalRotation(Eigen::Quaternionf(rotation.w, rotation.x, rotation.y, rotation.z));
	}
}
