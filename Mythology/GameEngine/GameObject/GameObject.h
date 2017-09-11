#pragma once

#include "GameEngine/Component/IComponent.h"
#include "GameEngine/Component/Physics/PhysicsComponent.h"

#include <unordered_map>

namespace GameEngine
{
	class GameObject
	{
	public:
		template<class T>
		using PointerType = std::shared_ptr<T>;
		using IComponentPointer = PointerType<IComponent>;
		using IComponentPointerCR = const IComponentPointer&;

	public:
		GameObject();
		explicit GameObject(const std::shared_ptr<PhysicsComponent>& physics);

	public:
		void AddComponent(const std::string& name, IComponentPointerCR component, bool worldTransformStays = false);
		void RemoveComponent(const std::string& name, bool worldTransformStays = false);
		
		IComponentPointerCR GetComponent(const std::string& name) const;
		
		template<class T>
		PointerType<T> GetComponent(const std::string& name) const
		{
			return std::static_pointer_cast<T>(GetComponent(name));
		}
		
		bool HasComponent(const std::string& name) const;

	public:
		const TransformComponent& GetTransform() const;
		TransformComponent& GetTransform();

	private:
		std::shared_ptr<PhysicsComponent> m_physics;
		std::unordered_map<std::string, std::shared_ptr<IComponent>> m_components;
	};
}
