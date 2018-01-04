#pragma once

#include "PhysicsEngine/Actors/IActor.h"

#include <Eigen/Eigen>

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IScene
	{
	public:
		virtual ~IScene() = default;

	public:
		virtual void FixedUpdate(const Common::Timer& timer) const = 0;

	public:
		virtual void AddActor(IActor<T>& actor) = 0;
		virtual void RemoveActor(IActor<T>& actor) = 0;
	};
}
