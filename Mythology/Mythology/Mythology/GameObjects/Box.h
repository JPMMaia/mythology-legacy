#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"

namespace Mythology
{
	class Box
	{
	public:
		Box() = default;
		explicit Box(GameEngine::GameManager& gameManager, const physx::PxTransform& transform);

	private:
		GameEngine::GameObject m_gameObject;
	};
}
