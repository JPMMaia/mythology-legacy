#include "pch.h"
#include "PhysicsScene.h"

using namespace physx;
using namespace GameEngine;

PhysicsScene::PhysicsScene(PhysXUniquePointer<physx::PxScene>&& scene) :
	m_scene(std::move(scene))
{
	m_scene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);
}

void GameEngine::PhysicsScene::FixedUpdate(const Common::Timer& timer) const
{
	auto deltaSeconds = std::chrono::duration<PxReal>(timer.GetTimePerUpdate()).count();
	m_scene->simulate(deltaSeconds);
	m_scene->fetchResults(true);
}
