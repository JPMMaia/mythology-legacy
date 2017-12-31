#pragma once

#include "PhysicsUtilities.h"

#include <unordered_map>

namespace GameEngine
{
	class PhysicsScene
	{
	public:
		PhysicsScene(PhysXUniquePointer<physx::PxScene>&& scene);
		PhysicsScene(const PhysicsScene&) = delete;
		PhysicsScene& operator=(const PhysicsScene&) = delete;
		PhysicsScene(PhysicsScene&&) = default;
		PhysicsScene& operator=(PhysicsScene&&) = default;

	public:
		void FixedUpdate(const Common::Timer& timer) const;

	public:
		const physx::PxScene* operator->() const
		{
			return m_scene.get();
		}
		physx::PxScene* operator->()
		{
			return m_scene.get();
		}

	public:
		void AddMaterial(const std::string& name, const PhysXSharedPointer<physx::PxMaterial>& material)
		{
			m_materials.emplace(name, material);
		}
		void DeleteMaterial(const std::string& name)
		{
			m_materials.erase(name);
		}
		const PhysXSharedPointer<physx::PxMaterial>& GetMaterial(const std::string& name)
		{
			return m_materials.at(name);
		}

	private:
		PhysXUniquePointer<physx::PxScene> m_scene;
		std::unordered_map<std::string, PhysXSharedPointer<physx::PxMaterial>> m_materials;
	};
}
