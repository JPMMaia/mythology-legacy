#pragma once

#include "VulkanEngine/Devices/QueueFamilyIndices.h"

namespace VulkanEngine
{
	class CommandPool
	{
	public:
		static CommandPool CreateDefaultCommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices);
		static CommandPool CreateTemporaryCommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices);

	private:
		CommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices, vk::CommandPoolCreateFlags createFlags);

	public:
		operator const vk::CommandPool&() const;

	public:
		vk::UniqueCommandBuffer CreateCommandBuffer(vk::Device device, vk::CommandBufferLevel level) const;
		std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(vk::Device device, vk::CommandBufferLevel level, std::uint32_t count) const;

	private:
		static vk::UniqueCommandPool CreateCommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices, vk::CommandPoolCreateFlags createFlags);

	private:
		vk::UniqueCommandPool m_commandPool;
	};
}
