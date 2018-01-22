#include "pch.h"
#include "InputAssemblyState.h"

using namespace VulkanEngine;

vk::PipelineInputAssemblyStateCreateInfo InputAssemblyState::TriangleList()
{
	return vk::PipelineInputAssemblyStateCreateInfo(
		{},
		vk::PrimitiveTopology::eTriangleList,
		VK_FALSE
	);
}
