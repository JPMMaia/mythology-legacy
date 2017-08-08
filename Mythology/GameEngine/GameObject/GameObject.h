#pragma once

#include "GameEngine/Component/IComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

#include <unordered_map>

namespace GameEngine
{
	class GameObject
	{
	public:
		using IComponentPointer = std::shared_ptr<IComponent>;
		using IComponentPointerCR = const IComponentPointer&;

	public:
		GameObject();

	public:
		void AddComponent(const std::string& name, IComponentPointerCR component, bool worldTransformStays = false);
		void RemoveComponent(const std::string& name, bool worldTransformStays = false);
		IComponentPointerCR GetComponent(const std::string& name) const;
		bool HasComponent(const std::string& name) const;

		const TransformComponent& GetTransform() const;
		TransformComponent& GetTransform();

	private:
		std::shared_ptr<TransformComponent> m_transform;
		std::unordered_map<std::string, std::shared_ptr<IComponent>> m_components;
	};
}
