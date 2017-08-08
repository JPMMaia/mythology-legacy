#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"

namespace Common 
{
	class Timer;
}

namespace Mythology
{
	class MythologyGame
	{
	public:
		MythologyGame();

		void Initialize();

		void ProcessInput() const;
		void FixedUpdate(const Common::Timer& timer) const;
		void FrameUpdate(const Common::Timer& timer) const;

	public:
		std::shared_ptr<GameEngine::GameManager> GameManager() const;

	private:
		std::shared_ptr<GameEngine::GameManager> m_gameManager;
		GameEngine::GameObject m_person;
	};
}
