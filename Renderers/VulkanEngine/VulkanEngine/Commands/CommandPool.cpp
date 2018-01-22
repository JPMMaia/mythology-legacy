#include "pch.h"
#include "CommandPool.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

CommandPool::CommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices) :
	m_commandPool(CreateCommandPool(device, queueFamilyIndices))
{
}

CommandPool::operator const vk::CommandPool&() const
{
	return m_commandPool.get();
}

vk::UniqueCommandPool CommandPool::CreateCommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices)
{
	vk::CommandPoolCreateInfo poolInfo(
		vk::CommandPoolCreateFlags(),
		queueFamilyIndices.GraphicsFamily
	);

	return device.createCommandPoolUnique(poolInfo);
}
