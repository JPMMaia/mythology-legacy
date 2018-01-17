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
		VkPhysicalDevice GetPhysicalDevice() const;
		const QueueFamilyIndices& GetQueueFamilyIndices() const;
		VkDevice GetDevice() const;
		VkQueue GetGraphicsQueue() const;
		VkQueue GetPresentQueue() const;
		const SwapChainSupportDetails& GetSwapChainSupportDetails() const;
		VkSurfaceFormatKHR GetSwapSurfaceFormat() const;
		VkSurfaceTransformFlagBitsKHR GetPreTransform() const;
		VkPresentModeKHR GetSwapPresentMode() const;
		VkExtent2D GetSwapExtent() const;
		std::uint32_t GetImageCount() const;

	private:
		static VkPhysicalDevice QueryPhysicalDevices(const VulkanInstance& instance, VkSurfaceKHR surface);
		static std::size_t RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	private:
		static VkDevice CreateLogicalDevice(const VulkanInstance& instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const QueueFamilyIndices& queueFamilyIndices);

	private:
		static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkQueue CreateGraphicsQueue(VkDevice device, int graphicsFamily);
		static VkQueue CreatePresentQueue(VkDevice device, int presentFamily);		

	private:
		static SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, std::uint32_t width, std::uint32_t height);
		static std::uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

	private:
		VkPhysicalDevice m_physicalDevice;
		QueueFamilyIndices m_queueFamilyIndices;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
		SwapChainSupportDetails m_swapChainSupportDetails;
		VkSurfaceFormatKHR m_swapSurfaceFormat;
		VkPresentModeKHR m_swapPresentMode;
		VkExtent2D m_swapExtent;
		std::uint32_t m_imageCount;

	private:
		static const std::vector<const char*> s_deviceExtensions;
	};
}
