#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class RasterizerState
	{
	public:
		static VkPipelineRasterizationStateCreateInfo Default();
	};
}