#pragma once

namespace Common
{
	namespace EigenHelpers
	{
		template<typename VectorType>
		VectorType LinearInterpolate(const VectorType& a, const VectorType& b, float percentage)
		{
			return (1.0f - percentage) * a + percentage * b;
		}
	}
}