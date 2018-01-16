#include "pch.h"
#include "PipelineLayout.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

PipelineLayout::PipelineLayout(VkDevice device) :
	m_device(device),
	m_pipelineLayout(CreatePipelineLayout(device))
{
}
PipelineLayout::~PipelineLayout()
{
	if (m_pipelineLayout)
		vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
}

PipelineLayout::operator VkPipelineLayout() const
{
	return m_pipelineLayout;
}

VkPipelineLayout PipelineLayout::CreatePipelineLayout(VkDevice device)
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = 0;

	VkPipelineLayout pipelineLayout;
	ThrowIfFailed(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout));
	return pipelineLayout;
}
