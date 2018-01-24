#include "pch.h"
#include "ColorBlendAttachment.h"

using namespace VulkanEngine;

vk::PipelineColorBlendAttachmentState ColorBlendAttachment::Default()
{
	return vk::PipelineColorBlendAttachmentState(
		VK_FALSE,
		vk::BlendFactor::eZero,
		vk::BlendFactor::eZero,
		vk::BlendOp::eAdd,
		vk::BlendFactor::eZero,
		vk::BlendFactor::eZero,
		vk::BlendOp::eAdd,
		vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
	);
}
