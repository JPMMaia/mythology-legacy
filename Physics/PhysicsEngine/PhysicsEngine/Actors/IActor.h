#pragma once

#include "PhysicsEngine/PhysicsTraits.h"

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IActor
	{
	public:
		using Actor = typename PhysicsTraits<T>::Actor;

	public:
		virtual operator Actor&() = 0;
		virtual operator const Actor&() const = 0;

	public:
		virtual void GetGlobalPose(Eigen::Vector3f& position, Eigen::Quaternionf& rotation) const = 0;

	public:
		virtual void* GetUserData() const = 0;
		virtual void SetUserData(void* data) = 0;
	};
}
