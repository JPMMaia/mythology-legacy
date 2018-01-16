#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace VulkanEngine
{
	class DynamicState
	{
	public:
		static VkPipelineDynamicStateCreateInfo Default(const std::vector<VkDynamicState>& dynamicStates);
	};
}
