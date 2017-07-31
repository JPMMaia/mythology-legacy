#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class PointLightComponent : public BaseComponent
	{
	public:
		PointLightComponent();
		PointLightComponent(const Vector3Type& strength, const Vector3Type& position, float falloffStart, float falloffEnd);

	public:
		const Vector3Type& Strength() const;
		Vector3Type& Strength();

		const Vector3Type& Position() const;
		void SetPosition(const Vector3Type& position);

		float FalloffStart() const;
		float& FalloffStart();

		float FalloffEnd() const;
		float& FalloffEnd();

	private:
		Vector3Type m_strength;
		float m_falloffStart;
		float m_falloffEnd;
	};
}