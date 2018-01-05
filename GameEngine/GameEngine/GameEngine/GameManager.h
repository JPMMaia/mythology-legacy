#pragma once

#include "RenderEngine/IRenderer.h"
#include "PhysicsEngine/IManager.h"
#include "Component/Transforms/TransformComponent.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "GameEngine/Repositories/MaterialRepository.h"
#include "GameEngine/Repositories/MeshRepository.h"
#include "GameEngine/Commands/Render/RenderCommandQueue.h"
#include "GameEngine/Physics/RigidDynamicManager.h"

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
		GameManager(std::unique_ptr<RenderEngine::IRenderer>&& renderer, std::unique_ptr<PhysicsEngine::IManager<>>&& physicsManager);

	public:
		void ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		bool FrameUpdate(const Common::Timer& timer);

	public:
		Keyboard & GetKeyboard() { return m_keyboard; }
		Mouse& GetMouse() { return m_mouse; }

	public:
		RenderEngine::IRenderer& GetRenderer() { return *m_renderer;  }
		RenderEngine::IScene& GetRenderScene() { return *m_renderScene; }
		void SetRenderScene(const std::shared_ptr<RenderEngine::IScene>& renderScene) { m_renderScene = renderScene; }
		RenderCommandQueue& GetRenderCommandQueue() { return m_renderCommandQueue; }
		MeshRepository& GetMeshRepository() { return m_meshes; }
		MaterialRepository& GetMaterialRepository() { return m_materials; }

	public:
		PhysicsEngine::IManager<>& GetPhysicsManager() { return *m_physicsManager; }
		PhysicsEngine::IScene<>& GetPhysicsScene() { return *m_physicsScene; }
		void SetPhysicsScene(const std::shared_ptr<PhysicsEngine::IScene<>>& physicsScene) { m_physicsScene = physicsScene; }

	private:
		Keyboard m_keyboard;
		Mouse m_mouse;
		
		std::unique_ptr<RenderEngine::IRenderer> m_renderer;
		std::shared_ptr<RenderEngine::IScene> m_renderScene;
		RenderCommandQueue m_renderCommandQueue;
		MeshRepository m_meshes;
		MaterialRepository m_materials;

		std::unique_ptr<PhysicsEngine::IManager<>> m_physicsManager;
		std::shared_ptr<PhysicsEngine::IScene<>> m_physicsScene;
		RigidDynamicManager m_rigidDynamicManager;
	};
}
