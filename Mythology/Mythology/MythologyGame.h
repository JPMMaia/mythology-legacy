#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"

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

		void ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer) const;

	public:
		std::shared_ptr<GameEngine::GameManager> GameManager() const;

	public:
		const GameEngine::GameObject& GetPerson() const { return m_person; }

	private:
		std::shared_ptr<GameEngine::GameManager> m_gameManager;
		
		std::unordered_map<std::string, std::shared_ptr<GameEngine::MeshComponent<GameEngine::BoxGeometry>>> m_meshes;
		GameEngine::GameObject m_person;
	};
}
