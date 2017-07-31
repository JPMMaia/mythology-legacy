#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class SpotLightComponent : public BaseComponent
	{
	public:
		SpotLightComponent();
		SpotLightComponent(const Vector3Type& strength, const Vector3Type& position, const Vector3Type& direction, float falloffStart, float falloffEnd, float spotPower);

	public:
		const Vector3Type& Strength() const;
		Vector3Type& Strength();

		const Vector3Type& Position() const;
		Vector3Type& Position();

		const Vector3Type& Direction() const;
		Vector3Type& Direction();

		float FalloffStart() const;
		float& FalloffStart();

		float FalloffEnd() const;
		float& FalloffEnd();

		float SpotPower() const;
		float& SpotPower();

	private:
		Vector3Type m_strength;
		Vector3Type m_position;
		Vector3Type m_direction;
		float m_falloffStart;
		float m_falloffEnd;
		float m_spotPower;
	};
}
