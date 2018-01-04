#pragma once

#include "PhysicsEngine/PhysicsTraits.h"
#include "PhysicsEngine/Actors/IRigidDynamic.h"
#include "PhysicsEngine/Actors/IRigidStatic.h"

#include <Eigen/Eigen>

namespace PhysicsEngine
{
	template<class T = class PhysicsTraitsImplementation>
	class IActorFactory
	{
	public:
		using Geometry = typename PhysicsTraits<T>::Geometry;
		using Material = typename PhysicsTraits<T>::Material;

	public:
		virtual ~IActorFactory() = default;

	public:
		virtual std::unique_ptr<IRigidDynamic<T>> CreateRigidDynamic(const Geometry& geometry, const Eigen::Affine3f& transform, const Material& material, float density) = 0;
		virtual std::unique_ptr<IRigidStatic<T>> CreateRigidStatic(const Geometry& geometry, const Eigen::Affine3f& transform, const Material& material) = 0;
	};
}
