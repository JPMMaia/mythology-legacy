#pragma once

namespace GameEngine
{
	template<typename T>
	using PhysXUniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

	template<typename T>
	using PhysXSharedPointer = std::unique_ptr<T, std::function<void(T*)>>;

	namespace PhysicsUtilities
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

		Eigen::Affine3f PhysXToEigenTransform(const physx::PxTransform& pxTransform);
	}
}