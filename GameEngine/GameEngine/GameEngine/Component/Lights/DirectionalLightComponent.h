#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class DirectionalLightComponent : public BaseComponent
	{
	public:
		DirectionalLightComponent();
		explicit DirectionalLightComponent(const Vector3Type& strength);

	public:
		const Vector3Type& GetStrength() const;
		void SetStrength(const Vector3Type& strength);

		Vector3Type GetLocalDirection() const;
		void SetLocalDirection(const Vector3Type& localDirection);

	private:
		static Vector3Type s_initialDirection;

	private:
		Vector3Type m_strength;
	};
}
