#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Physics/RigidDynamicComponent.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"

#include <memory>

namespace Mythology
{
	class Person
	{
	public:
		Person() = default;
		explicit Person(GameEngine::GameManager& gameManager);

	public:
		void FixedUpdate(const Common::Timer& timer, GameEngine::GameManager& gameManager);

	public:
		GameEngine::GameObject::PointerType<GameEngine::CameraComponent> GetCamera() const;

	private:
		GameEngine::GameObject m_gameObject;
		std::shared_ptr<GameEngine::RigidDynamicComponent> m_rigidDynamicComponent;
	};
}
