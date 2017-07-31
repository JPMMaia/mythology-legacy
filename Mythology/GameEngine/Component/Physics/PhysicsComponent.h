#pragma once

#include "GameEngine/Component/IComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

namespace GameEngine
{
	class PhysicsComponent : public IComponent
	{
	public:
		void FixedUpdate(const Common::Timer& timer) override;

		void SetParent(const std::weak_ptr<TransformComponent>& parent, bool worldPositionStays) override;

	private:
		TransformComponent m_transform;
	};
}
