#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class PointLightComponent : public BaseComponent, public UseStorage<PointLightComponent>
	{
	public:
		PointLightComponent();
		PointLightComponent(const Vector3Type& strength, float falloffStart, float falloffEnd);

	public:
		const Vector3Type& GetStrength() const;
		void SetStrength(const Vector3Type& strength);

		float GetFalloffStart() const;
		void SetFalloffStart(float falloffStart);

		float GetFalloffEnd() const;
		void SetFalloffEnd(float falloffEnd);

		Vector3Type GetLocalPosition() const;
		void SetLocalPosition(const Vector3Type& localPosition);
		
		Vector3Type GetWorldPosition() const;

	private:
		Vector3Type m_strength;
		float m_falloffStart;
		float m_falloffEnd;
	};
}
