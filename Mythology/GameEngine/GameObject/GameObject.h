#pragma once

#include "GameEngine/Component/IComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

#include <unordered_map>

namespace GameEngine
{
	class GameObject
	{
	public:
		GameObject();

	public:
		void AddComponent(const std::string& name, IComponent& component);
		void RemoveComponent(const std::string& name);
		IComponent& GetComponent(const std::string& name) const;
		bool HasComponent(const std::string& name) const;

		const TransformComponent& Transform() const;
		TransformComponent& Transform();

	private:
		TransformComponent m_transform;
		std::unordered_map<std::string, IComponent*> m_components;
	};
}
