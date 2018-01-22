#include "pch.h"
#include "VertexInputState.h"

using namespace VulkanEngine;

vk::PipelineVertexInputStateCreateInfo VertexInputState::Default(const vk::VertexInputBindingDescription& bindingDescription, const std::vector<vk::VertexInputAttributeDescription>& attributeDescriptions)
{
	return vk::PipelineVertexInputStateCreateInfo(
		{},
		1, &bindingDescription,
		static_cast<std::uint32_t>(attributeDescriptions.size()), attributeDescriptions.data()
	);
}
