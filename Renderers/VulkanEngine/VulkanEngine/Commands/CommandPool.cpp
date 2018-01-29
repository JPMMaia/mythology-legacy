#include "pch.h"
#include "CommandPool.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

CommandPool CommandPool::CreateDefaultCommandPool(vk::Device device, const QueueFamilyIndices & queueFamilyIndices)
{
	return CommandPool(device, queueFamilyIndices, {});
}
CommandPool CommandPool::CreateTemporaryCommandPool(vk::Device device, const QueueFamilyIndices & queueFamilyIndices)
{
	return CommandPool(device, queueFamilyIndices, vk::CommandPoolCreateFlagBits::eTransient);
}

CommandPool::CommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices, vk::CommandPoolCreateFlags createFlags) :
	m_commandPool(CreateCommandPool(device, queueFamilyIndices, createFlags))
{
}

CommandPool::operator const vk::CommandPool&() const
{
	return m_commandPool.get();
}

vk::UniqueCommandBuffer CommandPool::CreateCommandBuffer(vk::Device device, vk::CommandBufferLevel level) const
{
	vk::CommandBufferAllocateInfo commandBufferInfo(
		m_commandPool.get(),
		level,
		1
	);

	vk::CommandBuffer commandBuffer;
	device.allocateCommandBuffers(&commandBufferInfo, &commandBuffer);
	return vk::UniqueCommandBuffer(commandBuffer, vk::CommandBufferDeleter(device, m_commandPool.get()));
}
std::vector<vk::UniqueCommandBuffer> CommandPool::CreateCommandBuffers(vk::Device device, vk::CommandBufferLevel level, std::uint32_t count) const
{
	vk::CommandBufferAllocateInfo commandBufferInfo(
		m_commandPool.get(),
		level,
		count
	);

	return device.allocateCommandBuffersUnique(commandBufferInfo);
}

vk::UniqueCommandPool CommandPool::CreateCommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices, vk::CommandPoolCreateFlags createFlags)
{
	vk::CommandPoolCreateInfo poolInfo(
		createFlags,
		queueFamilyIndices.GraphicsFamily
	);

	return device.createCommandPoolUnique(poolInfo);
}
