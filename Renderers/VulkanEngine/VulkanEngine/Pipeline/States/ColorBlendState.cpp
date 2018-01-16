#include "pch.h"
#include "ColorBlendState.h"

using namespace VulkanEngine;

VkPipelineColorBlendStateCreateInfo ColorBlendState::Default(const std::vector<VkPipelineColorBlendAttachmentState>& attachments)
{
	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = static_cast<std::uint32_t>(attachments.size());
	colorBlending.pAttachments = attachments.data();
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	return colorBlending;
}
