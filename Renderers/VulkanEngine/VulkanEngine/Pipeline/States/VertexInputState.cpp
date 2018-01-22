#include "pch.h"
#include "VertexInputState.h"

using namespace VulkanEngine;

vk::PipelineVertexInputStateCreateInfo VertexInputState::Default()
{
	return vk::PipelineVertexInputStateCreateInfo();
}
