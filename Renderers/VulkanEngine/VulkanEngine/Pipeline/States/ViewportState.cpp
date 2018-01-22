#include "pch.h"
#include "ViewportState.h"

using namespace VulkanEngine;

vk::PipelineViewportStateCreateInfo ViewportState::Default(float width, float height, const vk::Extent2D& extent)
{
	vk::Viewport viewport(
		0.0f, 0.0f,
		width, height,
		0.0f, 1.0f
	);

	vk::Rect2D scissor(
		{ 0, 0 },
		extent
	);

	return vk::PipelineViewportStateCreateInfo(
		{},
		1, &viewport,
		1, &scissor
	);
}
