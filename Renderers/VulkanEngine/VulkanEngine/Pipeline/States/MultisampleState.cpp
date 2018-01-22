#include "pch.h"
#include "MultisampleState.h"

using namespace VulkanEngine;

vk::PipelineMultisampleStateCreateInfo MultisampleState::Default()
{
	return vk::PipelineMultisampleStateCreateInfo(
		{},
		vk::SampleCountFlagBits::e1,
		VK_FALSE,
		1.0f, 
		nullptr,
		VK_FALSE, 
		VK_FALSE
	);
}
