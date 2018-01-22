#pragma once

namespace VulkanEngine
{
	class MultisampleState
	{
	public:
		static vk::PipelineMultisampleStateCreateInfo Default();
	};
}