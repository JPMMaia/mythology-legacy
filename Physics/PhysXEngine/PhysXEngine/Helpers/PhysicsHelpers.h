#pragma once

#include <functional>
#include <memory>

#include <Eigen/Eigen>
#include <PxPhysicsAPI.h>

namespace PhysXEngine
{
	template<typename T>
	using PhysXUniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

	template<typename T>
	using PhysXSharedPointer = std::shared_ptr<T>;

	namespace PhysicsHelpers
	{
		template<typename T>
		PhysXUniquePointer<T> MakeUniquePointer(T* rawPointer)
		{
			return PhysXUniquePointer<T>(rawPointer, [](T* pointer)
			{
				pointer->release();
			});
		}

		template<typename T>
		PhysXSharedPointer<T> MakeSharedPointer(T* rawPointer)
		{
			return PhysXSharedPointer<T>(rawPointer, [](T* pointer)
			{
				pointer->release();
			});
		}

		Eigen::Affine3f ToEigen(const physx::PxTransform& pxTransform);
		physx::PxTransform ToPhysX(const Eigen::Affine3f& eigenTransform);
		physx::PxVec3 ToPhysX(const Eigen::Vector3f& vector);

		PhysXSharedPointer<physx::PxRigidDynamic> CreateRigidDynamic(physx::PxPhysics& physics, const physx::PxTransform& transform, physx::PxShape& shape, float mass);
	}
}