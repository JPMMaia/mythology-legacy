#pragma once

namespace VulkanEngine
{
	class ViewportState
	{
	public:
		static vk::PipelineViewportStateCreateInfo Default(const vk::Viewport& viewport, const vk::Rect2D& scissor);
	};
}
