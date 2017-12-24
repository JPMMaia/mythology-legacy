#pragma once

#include "Component/Transforms/TransformComponent.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "GameEngine/Repositories/MaterialRepository.h"
#include "GameEngine/Repositories/MeshRepository.h"
#include "GameEngine/Physics/PhysicsManager.h"

namespace Common
{
	class Timer;
}

namespace GameEngine
{
	class GameManager : public std::enable_shared_from_this<GameManager>
	{
	public:
		GameManager();

	public:
		void ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer);

	public:
		Keyboard& GetKeyboard() { return m_keyboard; }
		Mouse& GetMouse() { return m_mouse; }
		MeshRepository& GetMeshRepository() { return m_meshes; }
		MaterialRepository& GetMaterialRepository() { return m_materials; }
		GameEngine::PhysicsManager& GetPhysicsManager() { return m_physicsManager; }
		GameEngine::PhysicsScene& GetPhysicsScene() { return m_physicsScene; }

	private:
		MeshRepository m_meshes;
		MaterialRepository m_materials;
		Keyboard m_keyboard;
		Mouse m_mouse;
		GameEngine::PhysicsManager m_physicsManager;
		GameEngine::PhysicsScene m_physicsScene;
	};
}
