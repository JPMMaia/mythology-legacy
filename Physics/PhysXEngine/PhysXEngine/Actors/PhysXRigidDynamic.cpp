#include <pch.h>
#include "PhysXRigidDynamic.h"

#include <chrono>

using namespace Eigen;
using namespace physx;
using namespace PhysXEngine;
using namespace std::chrono;

PhysXRigidDynamic::PhysXRigidDynamic(PhysXUniquePointer<physx::PxRigidDynamic>&& actor) :
	m_actor(std::move(actor))
{
	m_actor->userData = this;
}

PhysXRigidDynamic::operator Actor&()
{
	return *m_actor;
}
PhysXRigidDynamic::operator const Actor&() const
{
	return *m_actor;
}

void PhysXRigidDynamic::GetGlobalPose(Eigen::Vector3f & position, Eigen::Quaternionf & rotation) const
{
	auto globalPose = m_actor->getGlobalPose();
	position = Eigen::Vector3f(globalPose.p.x, globalPose.p.y, globalPose.p.z);
	rotation = Eigen::Quaternionf(globalPose.q.w, globalPose.q.x, globalPose.q.y, globalPose.q.z);
}

void *PhysXEngine::PhysXRigidDynamic::GetUserData() const
{
	return m_actor->userData;
}
void PhysXEngine::PhysXRigidDynamic::SetUserData(void * userData)
{
	m_actor->userData = userData;
}

void PhysXEngine::PhysXRigidDynamic::FixedUpdate(const Common::Timer & timer)
{
	m_lastGlobalPose = m_currentGlobalPose;
	m_lastAngularVelocity = m_currentAngularVelocity;
	m_lastLinearVelocity = m_currentLinearVelocity;

	m_currentGlobalPose = m_actor->getGlobalPose();
	m_currentAngularVelocity = m_actor->getAngularVelocity();
	m_currentLinearVelocity = m_actor->getLinearVelocity();
}

Eigen::Affine3f PhysXEngine::PhysXRigidDynamic::CalculateFrameTransform(const Common::Timer& timer)
{
	auto fixedDeltaSeconds = duration_cast<duration<float>>(timer.GetTimePerUpdate()).count();
	auto deltaSeconds = fixedDeltaSeconds * (1.0f + static_cast<float>(timer.GetLeftover()));

	// Position:
	auto linearAcceleration = (m_currentLinearVelocity - m_lastLinearVelocity) / fixedDeltaSeconds;
	auto nextPosition = m_lastGlobalPose.p + m_lastLinearVelocity * deltaSeconds + 0.5f * linearAcceleration * deltaSeconds * deltaSeconds;

	// Rotation:
	auto angularAcceleration = (m_currentAngularVelocity - m_lastAngularVelocity) / fixedDeltaSeconds;
	auto deltaRotation = m_lastAngularVelocity * deltaSeconds + 0.5f * angularAcceleration * deltaSeconds * deltaSeconds;
	auto nextRotation = m_lastGlobalPose.q * PxQuat(deltaRotation.x, PxVec3(1.0f, 0.0f, 0.0f));
	nextRotation *= PxQuat(deltaRotation.y, PxVec3(0.0f, 1.0f, 0.0f));
	nextRotation *= PxQuat(deltaRotation.z, PxVec3(0.0f, 0.0f, 1.0f));

	Eigen::Affine3f transform;
	transform.fromPositionOrientationScale(
		Eigen::Vector3f(nextPosition.x, nextPosition.y, nextPosition.z),
		Eigen::Quaternionf(nextRotation.w, nextRotation.x, nextRotation.y, nextRotation.z),
		Eigen::Vector3f::Ones()
	);

	return transform;
}
