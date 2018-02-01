#pragma once

#include <Eigen/Eigen>

namespace VulkanEngine
{
	struct InstanceData
	{
		Eigen::Affine3f ModelMatrix;
	};
}