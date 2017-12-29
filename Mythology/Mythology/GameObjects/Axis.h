#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"

namespace Mythology
{
	class Axis
	{
	public:
		Axis() = default;
		explicit Axis(GameEngine::GameManager& gameManager);

	private:
		GameEngine::GameObject m_gameObject;
	};
}