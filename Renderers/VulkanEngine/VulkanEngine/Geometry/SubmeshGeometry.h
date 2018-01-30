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

		SubmeshGeometry() :
			IndexCount(0),
			InstanceCount(0),
			FirstIndex(0),
			VertexOffset(0),
			FirstInstance(0)
		{
		}

		SubmeshGeometry(std::uint32_t indexCount, std::uint32_t instanceCount, std::uint32_t firstIndex, std::uint32_t vertexOffset, std::uint32_t firstInstance) :
			IndexCount(indexCount),
			InstanceCount(instanceCount),
			FirstIndex(firstIndex),
			VertexOffset(vertexOffset),
			FirstInstance(firstInstance)
		{
		}
	};
}
