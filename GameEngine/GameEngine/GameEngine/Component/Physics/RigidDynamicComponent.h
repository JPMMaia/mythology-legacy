#pragma once

#include "GameEngine/Component/IComponent.h"
#include "PhysicsEngine/Actors/IRigidDynamic.h"
#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class GameObject;

	class RigidDynamicComponent : public IComponent, public UseStorage<RigidDynamicComponent>
	{
	public:
		RigidDynamicComponent(std::unique_ptr<PhysicsEngine::IRigidDynamic<>>&& rigidDynamic, const std::shared_ptr<GameObject>& gameObject);

	public:
		void UpdateTransform(const Eigen::Affine3f& transform);

	public:
		const GameObject& GetGameObject() const;

	private:
		std::shared_ptr<GameObject> m_gameObject;
		std::unique_ptr<PhysicsEngine::IRigidDynamic<>> m_rigidDynamic;
	};
}
