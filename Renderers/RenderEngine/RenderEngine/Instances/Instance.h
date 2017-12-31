#pragma once

#include <cstddef>
#include "RenderEngine/Materials/Material.h"

#include <Eigen/Eigen>

namespace RenderEngine
{
	struct Instance
	{
		Eigen::Affine3f Transform;
		mutable std::size_t Index;
		const Material* Material;
	};
}
