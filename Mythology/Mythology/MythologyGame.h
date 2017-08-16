#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"

namespace Common 
{
	class Timer;
}

namespace GameEngine
{
	class IFileSystem;
}

namespace Mythology
{
	class MythologyGame
	{
	public:
		MythologyGame(const std::shared_ptr<GameEngine::IFileSystem>& directory);

		void Initialize();

		void ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer) const;

	public:
		std::shared_ptr<GameEngine::GameManager> GameManager() const;

	public:
		const GameEngine::GameObject& GetPerson() const { return m_person; }

	private:
		std::shared_ptr<GameEngine::IFileSystem> m_fileSystem;
		std::shared_ptr<GameEngine::GameManager> m_gameManager;
		
		std::unordered_map<std::string, std::shared_ptr<GameEngine::BaseMeshComponent>> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<GameEngine::StandardMaterial>> m_materials;
		GameEngine::GameObject m_person;
		GameEngine::GameObject m_floor;
		GameEngine::GameObject m_axis;
		GameEngine::GameObject m_box;
	};
}
