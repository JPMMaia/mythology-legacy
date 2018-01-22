#include "pch.h"
#include "PipelineLayout.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

PipelineLayout::PipelineLayout(const vk::Device& device) :
	m_pipelineLayout(CreatePipelineLayout(device))
{
}

PipelineLayout::operator const vk::PipelineLayout&() const
{
	return m_pipelineLayout.get();
}

vk::UniquePipelineLayout PipelineLayout::CreatePipelineLayout(const vk::Device& device)
{
	vk::PipelineLayoutCreateInfo createInfo;
	return device.createPipelineLayoutUnique(createInfo);
}
