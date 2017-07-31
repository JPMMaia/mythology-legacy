#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class DirectionalLightComponent : public BaseComponent
	{
	public:
		DirectionalLightComponent();
		DirectionalLightComponent(const Vector3Type& strength, const Vector3Type& direction);

	public:
		const Vector3Type& Strength() const;
		Vector3Type& Strength();

		const Vector3Type& Direction() const;
		Vector3Type& Direction();

	private:
		Vector3Type m_strength;
		Vector3Type m_direction;
	};
}
