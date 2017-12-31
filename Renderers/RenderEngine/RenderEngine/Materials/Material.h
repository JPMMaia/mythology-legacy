#pragma once

#include <Eigen/Eigen>

#include <string>

namespace RenderEngine
{
	struct Material
	{
		mutable std::size_t Index;
		Eigen::Vector4f BaseColorFactor;
		std::wstring BaseColorTextureName;
		float MetallicFactor;
		float RoughnessFactor;
		std::wstring MetallicRoughnessTextureName;
	};
}
