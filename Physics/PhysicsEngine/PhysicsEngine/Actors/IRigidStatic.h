#pragma once

#include "PhysicsEngine/Actors/IActor.h"

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IRigidStatic : public IActor<T>
	{
	};
}
