#pragma once

namespace VulkanEngine
{
	class RasterizerState
	{
	public:
		static vk::PipelineRasterizationStateCreateInfo Default();
	};
}