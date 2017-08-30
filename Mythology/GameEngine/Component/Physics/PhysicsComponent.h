#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "GameEngine/Memory/UseStorage.h"

namespace GameEngine
{
	class PhysicsComponent : public BaseComponent, public UseStorage<PhysicsComponent>
	{
	public:
		PhysicsComponent();


	};
}
