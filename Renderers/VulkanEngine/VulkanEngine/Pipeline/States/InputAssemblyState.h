#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class InputAssemblyState
	{
	public:
		static VkPipelineInputAssemblyStateCreateInfo Default();
	};
}
