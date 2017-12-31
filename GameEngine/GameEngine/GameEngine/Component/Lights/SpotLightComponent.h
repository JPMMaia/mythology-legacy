#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class SpotLightComponent : public BaseComponent
	{
	public:
		SpotLightComponent();
		SpotLightComponent(const Vector3Type& strength, float falloffStart, float falloffEnd, float spotPower);

	public:

		const Vector3Type& GetStrength() const;
		void SetStrength(const Vector3Type& strength);

		float GetFalloffStart() const;
		void SetFalloffStart(float falloffStart);

		float GetFalloffEnd() const;
		void SetFalloffEnd(float falloffEnd);

		float GetSpotPower() const;
		void SetSpotPower(float spotPower);

		Vector3Type GetLocalDirection() const;
		void SetLocalDirection(const Vector3Type& localDirection);

		Vector3Type GetLocalPosition() const;
		void SetLocalPosition(const Vector3Type& localPosition);

		Vector3Type GetWorldPosition() const;

	private:
		static Vector3Type s_initialDirection;

	private:
		Vector3Type m_strength;
		float m_falloffStart;
		float m_falloffEnd;
		float m_spotPower;
	};
}
