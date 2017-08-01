#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class PointLightComponent : public BaseComponent
	{
	public:
		PointLightComponent();
		PointLightComponent(const Vector3Type& strength, float falloffStart, float falloffEnd);

	public:
		const Vector3Type& Strength() const;
		Vector3Type& Strength();

		float FalloffStart() const;
		float& FalloffStart();

		float FalloffEnd() const;
		float& FalloffEnd();

		Vector3Type WorldPosition() const;

	private:
		Vector3Type m_strength;
		float m_falloffStart;
		float m_falloffEnd;
	};
}
