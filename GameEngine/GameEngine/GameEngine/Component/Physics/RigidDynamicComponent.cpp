#include "pch.h"
#include "RigidDynamicComponent.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

using namespace GameEngine;
using namespace std;
using namespace std::chrono;

RigidDynamicComponent::RigidDynamicComponent(std::unique_ptr<PhysicsEngine::IRigidDynamic<>>&& rigidDynamic, const std::shared_ptr<GameObject>& gameObject) :
	m_rigidDynamic(std::move(rigidDynamic)),
	m_gameObject(gameObject)
{
	m_rigidDynamic->SetUserData(this);
}

void RigidDynamicComponent::UpdateTransform(const Eigen::Affine3f& transform)
{
	m_gameObject->GetTransform().SetWorldTransform(transform);
}

const GameObject& GameEngine::RigidDynamicComponent::GetGameObject() const
{
	return *m_gameObject;
}
