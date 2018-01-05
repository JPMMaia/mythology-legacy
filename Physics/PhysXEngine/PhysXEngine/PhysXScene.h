#pragma once

#include "PhysXEngine/PhysXTraits.h"
#include "PhysXEngine/Helpers/PhysicsHelpers.h"
#include "PhysicsEngine/IScene.h"

#include <memory>
#include <unordered_map>

namespace PhysXEngine
{
	class PhysXScene : public PhysicsEngine::IScene<>
	{
	public:
		PhysXScene(PhysXUniquePointer<physx::PxScene>&& scene);
		PhysXScene(const PhysXScene&) = delete;
		PhysXScene& operator=(const PhysXScene&) = delete;
		PhysXScene(PhysXScene&&) = default;
		PhysXScene& operator=(PhysXScene&&) = default;

	public:
		void FixedUpdate(const Common::Timer& timer) const override;

	public:
		std::vector<PhysicsEngine::IRigidDynamic<>*> GetActiveActors() const override;

	public:
		void AddActor(PhysicsEngine::IActor<>& actor) override;
		void RemoveActor(PhysicsEngine::IActor<>& actor) override;

	private:
		PhysXUniquePointer<physx::PxScene> m_scene;
		std::unordered_map<std::string, PhysXSharedPointer<physx::PxMaterial>> m_materials;
	};
}
