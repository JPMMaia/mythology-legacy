#include "pch.h"
#include "RasterizerState.h"

using namespace VulkanEngine;

vk::PipelineRasterizationStateCreateInfo RasterizerState::Default()
{
	return vk::PipelineRasterizationStateCreateInfo(
		{},
		VK_FALSE,
		VK_FALSE,
		vk::PolygonMode::eFill,
		vk::CullModeFlagBits::eNone,
		vk::FrontFace::eCounterClockwise,
		VK_FALSE,
		0.0f,
		0.0f,
		0.0f, 
		0.0f
		);
}
