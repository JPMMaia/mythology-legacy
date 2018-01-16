#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class VertexInputState
	{
	public:
		static VkPipelineVertexInputStateCreateInfo Default();
	};
}
