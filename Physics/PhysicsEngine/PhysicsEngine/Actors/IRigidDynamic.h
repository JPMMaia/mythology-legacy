#pragma once

#include "PhysicsEngine/Actors/IActor.h"

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IRigidDynamic : public IActor<T>
	{
	public:
		virtual void FixedUpdate(const Common::Timer& timer) = 0;

	public:
		virtual Eigen::Affine3f CalculateFrameTransform(const Common::Timer& timer) = 0;
	};
}
