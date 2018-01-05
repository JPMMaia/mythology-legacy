#pragma once

namespace PhysicsEngine
{
	template<class T>
	class PhysicsTraits
	{
	public:
		using Actor = typename T::Actor;
		using Geometry = typename T::Geometry;
		using Material = typename T::Material;
	};
}
