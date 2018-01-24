#include "pch.h"
#include "MultisampleState.h"

using namespace VulkanEngine;

vk::PipelineMultisampleStateCreateInfo MultisampleState::Default()
{
	return vk::PipelineMultisampleStateCreateInfo(
		{},
		vk::SampleCountFlagBits::e1,
		VK_FALSE
	);
}
