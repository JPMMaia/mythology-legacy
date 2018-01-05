#include "pch.h"
#include "PhysXRigidStatic.h"

using namespace PhysXEngine;

PhysXRigidStatic::PhysXRigidStatic(PhysXUniquePointer<physx::PxRigidStatic>&& actor) :
	m_actor(std::move(actor))
{
	m_actor->userData = this;
}

PhysXRigidStatic::operator Actor&()
{
	return *m_actor;
}
PhysXRigidStatic::operator const Actor&() const
{
	return *m_actor;
}

void PhysXRigidStatic::GetGlobalPose(Eigen::Vector3f& position, Eigen::Quaternionf& rotation) const
{
	auto globalPose = m_actor->getGlobalPose();
	position = Eigen::Vector3f(globalPose.p.x, globalPose.p.y, globalPose.p.z);
	rotation = Eigen::Quaternionf(globalPose.q.w, globalPose.q.x, globalPose.q.y, globalPose.q.z);
}

void* PhysXRigidStatic::GetUserData() const
{
	return m_actor->userData;
}
void PhysXRigidStatic::SetUserData(void* userData)
{
	m_actor->userData = userData;
}
