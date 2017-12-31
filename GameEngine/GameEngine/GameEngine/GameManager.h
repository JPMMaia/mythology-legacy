#pragma once

#include "Component/Transforms/TransformComponent.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "GameEngine/Repositories/MaterialRepository.h"
#include "GameEngine/Repositories/MeshRepository.h"
#include "GameEngine/Physics/PhysicsManager.h"
#include "GameEngine/Commands/Render/RenderCommandQueue.h"

#include <memory>

namespace Common
{
	class Timer;
}

namespace GameEngine
{
	class GameManager : public std::enable_shared_from_this<GameManager>
	{
	public:
		GameManager(const std::shared_ptr<RenderEngine::IScene>& renderScene);

	public:
		void ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer);

	public:
		RenderEngine::IScene & GetRenderScene() { return *m_renderScene; }
		Keyboard& GetKeyboard() { return m_keyboard; }
		Mouse& GetMouse() { return m_mouse; }
		MeshRepository& GetMeshRepository() { return m_meshes; }
		MaterialRepository& GetMaterialRepository() { return m_materials; }
		PhysicsManager& GetPhysicsManager() { return m_physicsManager; }
		PhysicsScene& GetPhysicsScene() { return m_physicsScene; }
		RenderCommandQueue& GetRenderCommandQueue() { return m_renderCommandQueue; }

	private:
		std::shared_ptr<RenderEngine::IScene> m_renderScene;
		MeshRepository m_meshes;
		MaterialRepository m_materials;
		Keyboard m_keyboard;
		Mouse m_mouse;
		PhysicsManager m_physicsManager;
		PhysicsScene m_physicsScene;
		RenderCommandQueue m_renderCommandQueue;
	};
}
