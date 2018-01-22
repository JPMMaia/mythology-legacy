#pragma once

namespace VulkanEngine
{
	class ColorBlendState
	{
	public:
		static vk::PipelineColorBlendStateCreateInfo Default(const std::vector<vk::PipelineColorBlendAttachmentState>& attachments);
	};
}
