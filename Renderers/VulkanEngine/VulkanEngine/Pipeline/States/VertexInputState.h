#pragma once

namespace VulkanEngine
{
	class VertexInputState
	{
	public:
		static vk::PipelineVertexInputStateCreateInfo Default(const vk::VertexInputBindingDescription& bindingDescription, const std::vector<vk::VertexInputAttributeDescription>& attributeDescriptions);
	};
}
