#pragma once

#include <cstdint>

namespace VulkanEngine
{
	struct SubmeshGeometry
	{
		std::uint32_t VertexCount;
		std::uint32_t InstanceCount;
		std::uint32_t FirstVertex;
		std::uint32_t FirstInstance;
	};
}
