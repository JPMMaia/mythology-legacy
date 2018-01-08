#pragma once

#include "VulkanEngine/Devices/QueueFamilyIndices.h"
#include "VulkanEngine/VulkanInstance.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/SwapChains/SwapChainSupportDetails.h"

#include <map>
#include <vector>

namespace VulkanEngine
{
	class DeviceManager
	{
	public:
		DeviceManager(const VulkanInstance& instance, const Surface& surface);
		~DeviceManager();

	public:
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

	public:
		VkPhysicalDevice GetPhysicalDevice() const;
		VkDevice GetDevice() const;
		const QueueFamilyIndices& GetQueueFamilyIndices() const;
		VkQueue GetGraphicsQueue() const;

	private:
		void QueryPhysicalDevices(const VulkanInstance& instance, VkSurfaceKHR surface);
		void CreateLogicalDevice(const VulkanInstance& instance, VkPhysicalDevice device, VkSurfaceKHR surface);

	private:
		std::size_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

	private:
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		QueueFamilyIndices m_queueFamilyIndices;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;

	private:
		static const std::vector<const char*> s_deviceExtensions;
	};
}
