#pragma once

#include <vulkan/vulkan.h>

namespace VulkanEngine
{
	class ViewportState
	{
	public:
		static VkPipelineViewportStateCreateInfo Default(float width, float height, VkExtent2D extent);
	};
}
