#pragma once

#include <cstddef>

#include <Eigen/Eigen>

namespace RenderEngine
{
	struct Instance
	{
		Eigen::Transform<float, 3, Eigen::Projective> Transform;
		std::size_t Index;
		std::size_t MaterialIndex;
	};
}
