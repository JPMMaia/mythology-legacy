#pragma once

#include "Component/Transforms/TransformComponent.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "GameEngine/Repositories/MaterialRepository.h"
#include "GameEngine/Repositories/MeshRepository.h"

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
		void FixedUpdate(const Common::Timer& timer) const;
		void FrameUpdate(const Common::Timer& timer) const;

	public:
		Keyboard& GetKeyboard() { return m_keyboard; }
		Mouse& GetMouse() { return m_mouse; }
		MeshRepository& GetMeshRepository() { return m_meshes; }
		MaterialRepository& GetMaterialRepository() { return m_materials; }

	private:
		MeshRepository m_meshes;
		MaterialRepository m_materials;
		Keyboard m_keyboard;
		Mouse m_mouse;
	};
}
