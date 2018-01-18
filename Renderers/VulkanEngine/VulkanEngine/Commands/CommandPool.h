#pragma once

#include <vulkan/vulkan.h>

#include "VulkanEngine/Devices/QueueFamilyIndices.h"

namespace VulkanEngine
{
	class CommandPool
	{
	public:
		CommandPool(VkDevice device, const QueueFamilyIndices& queueFamilyIndices);
		~CommandPool();

	public:
		operator VkCommandPool() const;

	private:
		static VkCommandPool CreateCommandPool(VkDevice device, const QueueFamilyIndices& queueFamilyIndices);

	private:
		VkDevice m_device;
		VkCommandPool m_commandPool;
	};
}