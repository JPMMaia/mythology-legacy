#pragma once

#include "GameEngine/Component/IComponent.h"

#include <unordered_map>

namespace GameEngine
{
	class GameObject
	{
	public:
		void AddComponent(const std::string& name, IComponent& component);
		void RemoveComponent(const std::string& name);
		IComponent* GetComponent(const std::string& name) const;

	private:
		std::unordered_map<std::string, IComponent*> m_components;
	};
}
