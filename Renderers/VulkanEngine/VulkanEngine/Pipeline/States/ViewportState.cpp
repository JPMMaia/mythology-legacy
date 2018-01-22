#include "pch.h"
#include "ViewportState.h"

using namespace VulkanEngine;

vk::PipelineViewportStateCreateInfo ViewportState::Default(const vk::Viewport& viewport, const vk::Rect2D& scissor)
{
	return vk::PipelineViewportStateCreateInfo(
		{},
		1, &viewport,
		1, &scissor
	);
}
