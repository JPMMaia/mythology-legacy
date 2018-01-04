#include "pch.h"
#include "PhysXScene.h"

using namespace PhysXEngine;
using namespace physx;

PhysXScene::PhysXScene(PhysXUniquePointer<physx::PxScene>&& scene) :
	m_scene(std::move(scene))
{
	m_scene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
}

void PhysXScene::FixedUpdate(const Common::Timer& timer) const
{
	auto deltaSeconds = std::chrono::duration<PxReal>(timer.GetTimePerUpdate()).count();
	m_scene->simulate(deltaSeconds);
	m_scene->fetchResults(true);
}

void PhysXScene::AddActor(PhysicsEngine::IActor<>& actor)
{
}
void PhysXScene::RemoveActor(PhysicsEngine::IActor<>& actor)
{
}
