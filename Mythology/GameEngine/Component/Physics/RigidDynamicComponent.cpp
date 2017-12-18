#include "pch.h"
#include "RigidDynamicComponent.h"
#include "GameEngine/Physics/PhysicsUtilities.h"

using namespace GameEngine;
using namespace physx;
using namespace std;
using namespace std::chrono;

RigidDynamicComponent::RigidDynamicComponent(const std::shared_ptr<TransformComponent>& transform, const PhysXSharedPointer<physx::PxRigidDynamic>& rigidDynamic) :
	BaseComponent(transform),
	m_rigidDynamic(rigidDynamic),
	m_lastGlobalPose(rigidDynamic->getGlobalPose()),
	m_lastAngularVelocity(rigidDynamic->getAngularVelocity()),
	m_lastLinearVelocity(rigidDynamic->getLinearVelocity())
{
	m_rigidDynamic->userData = this;
	m_rigidDynamic->setActorFlag(PxActorFlag::eSEND_SLEEP_NOTIFIES, false);
}

void RigidDynamicComponent::FrameUpdate(const Common::Timer& timer)
{
	if (!m_rigidDynamic->isSleeping())
	{
		auto& transform = GetTransform();

		if (m_rigidDynamic->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC))
		{
			auto globalPose = m_rigidDynamic->getGlobalPose();

			const auto& nextPosition = globalPose.p;
			transform.SetLocalPosition(Eigen::Vector3f(nextPosition.x, nextPosition.y, nextPosition.z));

			const auto& nextRotation = globalPose.q;
			transform.SetLocalRotation(Eigen::Quaternionf(nextRotation.w, nextRotation.x, nextRotation.y, nextRotation.z));
		}
		else
		{
			auto fixedDeltaSeconds = duration_cast<duration<float>>(timer.GetTimePerUpdate()).count();
			auto deltaSeconds = fixedDeltaSeconds * (1.0f + static_cast<float>(timer.GetLeftover()));

			// Calculate position:
			{
				auto currentLinearVelocity = m_rigidDynamic->getLinearVelocity();
				auto linearAcceleration = (currentLinearVelocity - m_lastLinearVelocity) / fixedDeltaSeconds;
				auto nextPosition = m_lastGlobalPose.p + m_lastLinearVelocity * deltaSeconds + 0.5f * linearAcceleration * deltaSeconds * deltaSeconds;
				transform.SetLocalPosition(Eigen::Vector3f(nextPosition.x, nextPosition.y, nextPosition.z));
			}

			// Calculate rotation:
			{
				auto currentAngularVelocity = m_rigidDynamic->getAngularVelocity();
				auto angularAcceleration = (currentAngularVelocity - m_lastAngularVelocity) / fixedDeltaSeconds;

				auto deltaRotation = m_lastAngularVelocity * deltaSeconds + 0.5f * angularAcceleration * deltaSeconds * deltaSeconds;
				auto nextRotation = m_lastGlobalPose.q * PxQuat(deltaRotation.x, PxVec3(1.0f, 0.0f, 0.0f));
				nextRotation *= PxQuat(deltaRotation.y, PxVec3(0.0f, 1.0f, 0.0f));
				nextRotation *= PxQuat(deltaRotation.z, PxVec3(0.0f, 0.0f, 1.0f));

				transform.SetLocalRotation(Eigen::Quaternionf(nextRotation.w, nextRotation.x, nextRotation.y, nextRotation.z));
			}
		}

		m_lastGlobalPose = m_rigidDynamic->getGlobalPose();
		m_lastAngularVelocity = m_rigidDynamic->getAngularVelocity();
		m_lastLinearVelocity = m_rigidDynamic->getLinearVelocity();
	}
}

physx::PxRigidDynamic* GameEngine::RigidDynamicComponent::operator->()
{
	return m_rigidDynamic.get();
}
const physx::PxRigidDynamic* GameEngine::RigidDynamicComponent::operator->() const
{
	return m_rigidDynamic.get();
}