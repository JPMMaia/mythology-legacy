#pragma once

#include "PhysicsEngine/PhysicsTraits.h"

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IGeometryFactory
	{
	public:
		using Geometry = typename PhysicsTraits<T>::Geometry;

	public:
		virtual ~IGeometryFactory() = default;

	public:
		virtual Geometry CreateSphere(float radius) = 0;
		virtual Geometry CreateBox(float halfX, float halfY, float halfZ) = 0;
	};
}
