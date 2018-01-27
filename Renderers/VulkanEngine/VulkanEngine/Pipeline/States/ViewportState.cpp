#include "pch.h"
#include "ViewportState.h"

using namespace VulkanEngine;

vk::PipelineViewportStateCreateInfo ViewportState::Default()
{
	static vk::Viewport viewport(0.0f, 0.0f, 800.0f, 600.0f, 0.0f, 1.0f);
	static vk::Rect2D scissor({ 0, 0 }, { 800, 600 });
	return Default(viewport, scissor);
}
vk::PipelineViewportStateCreateInfo ViewportState::Default(const vk::Viewport& viewport, const vk::Rect2D& scissor)
{
	return vk::PipelineViewportStateCreateInfo(
		{},
		1, &viewport,
		1, &scissor
	);
}
