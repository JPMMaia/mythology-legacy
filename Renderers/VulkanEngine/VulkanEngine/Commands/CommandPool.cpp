#include "pch.h"
#include "CommandPool.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

CommandPool::CommandPool(VkDevice device, const QueueFamilyIndices& queueFamilyIndices) :
	m_device(device),
	m_commandPool(CreateCommandPool(device, queueFamilyIndices))
{
}
CommandPool::~CommandPool()
{
	if(m_commandPool)
		vkDestroyCommandPool(m_device, m_commandPool, nullptr);
}

CommandPool::operator VkCommandPool() const
{
	return m_commandPool;
}

VkCommandPool CommandPool::CreateCommandPool(VkDevice device, const QueueFamilyIndices& queueFamilyIndices)
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily;
	poolInfo.flags = 0;

	VkCommandPool commandPool;
	ThrowIfFailed(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool));
	return commandPool;
}
