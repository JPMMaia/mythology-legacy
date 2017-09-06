#pragma once

namespace GameEngine
{
	template<typename T>
	using PhysXUniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

	template<typename T>
	using PhysXSharedPointer = std::unique_ptr<T, std::function<void(T*)>>;

	namespace PhysicsUtilities
	{
		Eigen::Affine3f PhysXToEigenTransform(const physx::PxTransform& pxTransform);
	}
}