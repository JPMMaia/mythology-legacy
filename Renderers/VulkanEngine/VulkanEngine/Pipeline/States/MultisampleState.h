#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class MultisampleState
	{
	public:
		static VkPipelineMultisampleStateCreateInfo Default();
	};
}