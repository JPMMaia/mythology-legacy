#pragma once

#include "GameEngine/GameManager.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Component/Meshes/StandardMaterial.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Geometry/Animation/Armature.h"
#include "GameEngine/Component/Meshes/SkinnedMeshComponent.h"
#include "GameEngine/Physics/PhysicsManager.h"

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
		void FrameUpdate(const Common::Timer& timer);

	public:
		std::shared_ptr<GameEngine::GameManager> GameManager() const;

	public:
		GameEngine::GameObject::PointerType<GameEngine::CameraComponent> GetMainCamera() const;

	private:
		void CreateStack(const physx::PxTransform & transform, std::size_t size, const physx::PxMaterial& material);
		void CreateProjectile(std::uint8_t key);

	private:
		std::shared_ptr<GameEngine::IFileSystem> m_fileSystem;
		std::shared_ptr<GameEngine::GameManager> m_gameManager;
		GameEngine::PhysicsManager m_physicsManager;
		GameEngine::PhysicsScene m_physicsScene;
		physx::PxMaterial* m_physicsMaterial;
		
		std::unordered_map<std::string, std::shared_ptr<GameEngine::BaseMeshComponent>> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<GameEngine::SkinnedMeshComponent>> m_skinnedMeshes;
		std::unordered_map<std::string, std::shared_ptr<GameEngine::StandardMaterial>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<GameEngine::Armature>> m_armatures;
		GameEngine::GameObject m_person;
		GameEngine::GameObject m_floor;
		GameEngine::GameObject m_axis;
		GameEngine::GameObject m_box;
		std::deque<GameEngine::GameObject> m_boxes;
	};
}
