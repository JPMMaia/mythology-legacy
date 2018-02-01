#pragma once

#include <Eigen/Eigen>

namespace VulkanEngine
{
	struct PassData
	{
		Eigen::Affine3f ViewMatrix;
		Eigen::Affine3f ProjectionMatrix;
	};
}
