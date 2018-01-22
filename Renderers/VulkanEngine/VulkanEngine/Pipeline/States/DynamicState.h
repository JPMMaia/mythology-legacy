#pragma once

namespace VulkanEngine
{
	class DynamicState
	{
	public:
		static vk::PipelineDynamicStateCreateInfo Default(const std::vector<vk::DynamicState>& dynamicStates);
	};
}
