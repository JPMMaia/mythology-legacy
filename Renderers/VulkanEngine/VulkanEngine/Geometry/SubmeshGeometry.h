#pragma once

#include <cstdint>

namespace VulkanEngine
{
	struct SubmeshGeometry
	{
		std::uint32_t IndexCount;
		std::uint32_t InstanceCount;
		std::uint32_t FirstIndex;
		std::uint32_t VertexOffset;
		std::uint32_t FirstInstance;
	};
}
