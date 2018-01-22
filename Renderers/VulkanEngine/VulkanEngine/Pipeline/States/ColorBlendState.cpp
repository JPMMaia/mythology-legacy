#include "pch.h"
#include "ColorBlendState.h"

using namespace VulkanEngine;

vk::PipelineColorBlendStateCreateInfo ColorBlendState::Default(const std::vector<vk::PipelineColorBlendAttachmentState>& attachments)
{
	return vk::PipelineColorBlendStateCreateInfo(
		{},
		VK_FALSE, vk::LogicOp::eCopy,
		static_cast<std::uint32_t>(attachments.size()), attachments.data()
	);
}
