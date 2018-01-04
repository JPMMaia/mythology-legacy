#pragma once

#include "PhysicsEngine/PhysicsTraits.h"

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IMaterialFactory
	{
	public:
		using Material = typename PhysicsTraits<T>::Material;

	public:
		virtual ~IMaterialFactory() = default;

	public:
		virtual Material CreateMaterial(float staticFriction, float dynamicFriction, float restituion) = 0;
	};
}
