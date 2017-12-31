#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Component/Meshes/StandardMaterial.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Geometry/Animation/Armature.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "GameEngine/Physics/PhysicsManager.h"
#include "GameEngine/Repositories/MeshRepository.h"
#include "Mythology/GameObjects/Axis.h"
#include "Mythology/GameObjects/Box.h"
#include "Mythology/GameObjects/Person.h"

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

		void Initialize(const std::shared_ptr<RenderEngine::IScene>& renderScene);

		void ProcessInput();
		void FixedUpdate(const Common::Timer& timer);
		void FrameUpdate(const Common::Timer& timer);

	public:
		std::shared_ptr<GameEngine::GameManager> GameManager() const;

	public:
		GameEngine::GameObject::PointerType<GameEngine::CameraComponent> GetMainCamera() const;

	private:
		void CreateStack(const physx::PxTransform & transform, std::size_t size, const physx::PxMaterial& material);
		void CreateProjectile(std::uint8_t key);
		void CreateAxis(std::uint8_t key);
		void DestroyAxis(std::uint8_t key);

	private:
		std::shared_ptr<GameEngine::IFileSystem> m_fileSystem;
		std::shared_ptr<GameEngine::GameManager> m_gameManager;
		
		std::unordered_map<std::string, std::shared_ptr<GameEngine::SkinnedMeshComponent>> m_skinnedMeshes;
		std::unordered_map<std::string, std::shared_ptr<GameEngine::Armature>> m_armatures;
		Axis m_axis;
		Person m_person;
		GameEngine::GameObject m_floor;
		GameEngine::GameObject m_box;
		std::deque<Box> m_boxes;
		std::deque<GameEngine::GameObject> m_projectiles;
	};
}
