#pragma once

namespace VulkanEngine
{
	class ViewportState
	{
	public:
		static vk::PipelineViewportStateCreateInfo Default(float width, float height, const vk::Extent2D& extent);
	};
}
