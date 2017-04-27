#pragma once

#include "GameEngine/Component/TransformComponent.h"

#include <unordered_map>
#include <memory>

namespace GameEngine
{
	class GameObject
	{
	public:
		void AddComponent(const std::string& name, const std::weak_ptr<IComponent>& component);
		void RemoveComponent(const std::string& name);
		std::weak_ptr<IComponent> GetComponent(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::weak_ptr<IComponent>> m_components;
	};
}
