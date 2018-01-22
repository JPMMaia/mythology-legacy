#include "pch.h"
#include "DynamicState.h"

using namespace VulkanEngine;

vk::PipelineDynamicStateCreateInfo DynamicState::Default(const std::vector<vk::DynamicState>& dynamicStates)
{
	return vk::PipelineDynamicStateCreateInfo(
		{},
		static_cast<std::uint32_t>(dynamicStates.size()), dynamicStates.data()
	);
}
