#pragma once

#include "IComponent.h"
#include "TransformComponent.h"

namespace GameEngine
{
	class PhysicsComponent : public IComponent
	{
	public:
		void FixedUpdate(const Common::Timer& timer) override;

	private:
		TransformComponent m_transform;
	};
}
