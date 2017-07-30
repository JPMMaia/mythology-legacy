#pragma once

#pragma once

#include "IComponent.h"

namespace GameEngine
{
	class LightComponent : public IComponent
	{
	public:
		LightComponent();

	public:
		void FixedUpdate(const Common::Timer& timer) override;

	private:
		
	};
}
