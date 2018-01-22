#pragma once

#include "VulkanEngine/Devices/QueueFamilyIndices.h"

namespace VulkanEngine
{
	class CommandPool
	{
	public:
		CommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices);

	public:
		operator const vk::CommandPool&() const;

	private:
		static vk::UniqueCommandPool CreateCommandPool(vk::Device device, const QueueFamilyIndices& queueFamilyIndices);

	private:
		vk::UniqueCommandPool m_commandPool;
	};
}