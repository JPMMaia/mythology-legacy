#pragma once

#include <Eigen/Eigen>

#include <string>

namespace RenderEngine
{
	struct Material
	{
		Eigen::Vector4f m_baseColorFactor;
		std::wstring m_baseColorTextureName;
		float m_metallicFactor;
		float m_roughnessFactor;
		std::wstring m_metallicRoughnessTextureName;
	};
}
