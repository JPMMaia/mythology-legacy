#include "pch.h"
#include "PhysXScene.h"

using namespace PhysXEngine;
using namespace physx;

PhysXScene::PhysXScene(PhysXUniquePointer<physx::PxScene>&& scene) :
	m_scene(std::move(scene))
{
	m_scene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
	m_scene->setFlag(PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS, true);
}

void PhysXScene::FixedUpdate(const Common::Timer& timer) const
{
	// Simulate one step:
	auto deltaSeconds = std::chrono::duration<PxReal>(timer.GetTimePerUpdate()).count();
	m_scene->simulate(deltaSeconds);
	m_scene->fetchResults(true);
}

std::vector<PhysicsEngine::IRigidDynamic<>*> PhysXEngine::PhysXScene::GetActiveActors() const
{
	physx::PxU32 activeActorsCount;
	auto activeActors = m_scene->getActiveActors(activeActorsCount);

	std::vector<PhysicsEngine::IRigidDynamic<>*> output(activeActorsCount);
	for (std::size_t i = 0; i < activeActorsCount; ++i)
		output[i] = reinterpret_cast<PhysicsEngine::IRigidDynamic<>*>(activeActors[i]->userData);

	return output;
}

void PhysXScene::AddActor(PhysicsEngine::IActor<>& actor)
{
	m_scene->addActor(actor);
}
void PhysXScene::RemoveActor(PhysicsEngine::IActor<>& actor)
{
	m_scene->removeActor(actor);
}
