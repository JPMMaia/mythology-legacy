#pragma once

#include "PhysicsEngine/Actors/IActor.h"

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IRigidDynamic : public IActor<T>
	{
	};
}
