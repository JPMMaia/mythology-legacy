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
		void Initialize();

		void FixedUpdate(const Common::Timer& timer) const;
		void FrameUpdate(const Common::Timer& timer) const;

	public:
		std::weak_ptr<GameEngine::GameManager> GetGameManager() const;

	private:
		std::shared_ptr<GameEngine::GameManager> m_gameManager;
		GameEngine::GameObject m_person;
	};
}
