#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace VulkanEngine
{
	class ColorBlendState
	{
	public:
		static VkPipelineColorBlendStateCreateInfo Default(const std::vector<VkPipelineColorBlendAttachmentState>& attachments);
	};
}
