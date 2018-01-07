#pragma once

#include "VulkanEngine/VulkanInstance.h"

#include <map>
#include <vector>

namespace VulkanEngine
{
	class DeviceManager
	{
	private:
		struct QueueFamilyIndices
		{
			int GraphicsFamily = -1;

			bool IsComplete() const
			{
				return GraphicsFamily >= 0;
			}
		};

	public:
		explicit DeviceManager(const VulkanInstance& instance);
		~DeviceManager();

	public:
		VkPhysicalDevice GetPhysicalDevice(std::size_t index) const;
		std::size_t GetPhysicalDeviceCount() const;

		VkQueue GetGraphicsQueue() const;

	private:
		void QueryPhysicalDevices(const VulkanInstance& instance);
		void CreateLogicalDevice(const VulkanInstance& instance, VkPhysicalDevice device);

	private:
		static std::size_t RateDeviceSuitability(VkPhysicalDevice device);
		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	private:
		std::multimap<std::size_t, VkPhysicalDevice> m_physicalDevices;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
	};
}
