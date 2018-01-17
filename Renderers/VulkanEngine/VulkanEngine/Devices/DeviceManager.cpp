#include "pch.h"
#include "DeviceManager.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <algorithm>
#include <set>

using namespace VulkanEngine;

const std::vector<const char*> DeviceManager::s_deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

DeviceManager::DeviceManager(const VulkanInstance& instance, const Surface& surface) :
	m_physicalDevice(QueryPhysicalDevices(instance, surface)),
	m_queueFamilyIndices(FindQueueFamilies(m_physicalDevice, surface)),
	m_device(CreateLogicalDevice(instance, m_physicalDevice, surface, m_queueFamilyIndices)),
	m_graphicsQueue(CreateGraphicsQueue(m_device, m_queueFamilyIndices.GraphicsFamily)),
	m_presentQueue(CreatePresentQueue(m_device, m_queueFamilyIndices.PresentFamily)),
	m_swapChainSupportDetails(QuerySwapChainSupport(m_physicalDevice, surface)),
	m_swapSurfaceFormat(ChooseSwapSurfaceFormat(m_swapChainSupportDetails.Formats)),
	m_swapPresentMode(ChooseSwapPresentMode(m_swapChainSupportDetails.PresentModes)),
	m_swapExtent(ChooseSwapExtent(m_swapChainSupportDetails.Capabilities, surface.GetWidth(), surface.GetHeight())),
	m_imageCount(ChooseImageCount(m_swapChainSupportDetails.Capabilities))
{
}
DeviceManager::~DeviceManager()
{
	if (m_device)
		vkDestroyDevice(m_device, nullptr);
}

VkPhysicalDevice DeviceManager::GetPhysicalDevice() const
{
	return m_physicalDevice;
}
const QueueFamilyIndices& DeviceManager::GetQueueFamilyIndices() const
{
	return m_queueFamilyIndices;
}
VkDevice DeviceManager::GetDevice() const
{
	return m_device;
}
VkQueue DeviceManager::GetGraphicsQueue() const
{
	return m_graphicsQueue;
}
VkQueue DeviceManager::GetPresentQueue() const
{
	return m_presentQueue;
}
const SwapChainSupportDetails& DeviceManager::GetSwapChainSupportDetails() const
{
	return m_swapChainSupportDetails;
}
VkSurfaceFormatKHR DeviceManager::GetSwapSurfaceFormat() const
{
	return m_swapSurfaceFormat;
}
VkSurfaceTransformFlagBitsKHR DeviceManager::GetPreTransform() const
{
	return m_swapChainSupportDetails.Capabilities.currentTransform;
}
VkPresentModeKHR DeviceManager::GetSwapPresentMode() const
{
	return m_swapPresentMode;
}
VkExtent2D DeviceManager::GetSwapExtent() const
{
	return m_swapExtent;
}
std::uint32_t DeviceManager::GetImageCount() const
{
	return m_imageCount;
}

VkPhysicalDevice DeviceManager::QueryPhysicalDevices(const VulkanInstance& instance, VkSurfaceKHR surface)
{
	std::uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	std::map<std::size_t, VkPhysicalDevice> physicalDevices;
	for (const auto& device : devices)
	{
		auto score = RateDeviceSuitability(device, surface);
		physicalDevices.emplace(std::make_pair(score, device));
	}

	auto bestPhysicalDeviceIt = physicalDevices.rbegin();
	if (bestPhysicalDeviceIt->first == 0)
		throw std::runtime_error("Failed to find a suitable GPU!");

	return bestPhysicalDeviceIt->second;
}
std::size_t DeviceManager::RateDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	std::size_t score = 0;

	if (!FindQueueFamilies(device, surface).IsComplete())
		return score;

	if (!CheckDeviceExtensionSupport(device))
		return score;

	auto swapChainSupport = QuerySwapChainSupport(device, surface);
	auto swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
	if (!swapChainAdequate)
		return score;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;
	score += deviceProperties.limits.maxImageDimension2D;

	return score;
}
bool DeviceManager::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	std::uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(s_deviceExtensions.begin(), s_deviceExtensions.end());

	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	return requiredExtensions.empty();
}

VkDevice DeviceManager::CreateLogicalDevice(const VulkanInstance& instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const QueueFamilyIndices& queueFamilyIndices)
{
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	std::set<int> uniqueQueueFamilies = 
	{ 
		queueFamilyIndices.GraphicsFamily, queueFamilyIndices.PresentFamily 
	};

	auto queuePriority = 1.0f;
	for (auto queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<std::uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<std::uint32_t>(s_deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = s_deviceExtensions.data();

#if !defined(NDEBUG)
	auto validationLayers = instance.GetValidationLayers();
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#endif

	VkDevice device;
	ThrowIfFailed(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));
	return device;
}

QueueFamilyIndices DeviceManager::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;

	std::uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.GraphicsFamily = i;

		VkBool32 presentSupport;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if (queueFamily.queueCount > 0 && presentSupport)
			indices.PresentFamily = i;

		if (indices.IsComplete())
			break;

		++i;
	}

	return indices;
}
VkQueue DeviceManager::CreateGraphicsQueue(VkDevice device, int graphicsFamily)
{
	VkQueue graphicsQueue;
	vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
	return graphicsQueue;
}
VkQueue DeviceManager::CreatePresentQueue(VkDevice device, int presentFamily)
{
	VkQueue presentQueue;
	vkGetDeviceQueue(device, presentFamily, 0, &presentQueue);
	return presentQueue;
}

SwapChainSupportDetails DeviceManager::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.Formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.PresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
	}

	return details;
}
VkSurfaceFormatKHR DeviceManager::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;
	}

	return availableFormats[0];
}
VkPresentModeKHR DeviceManager::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	auto bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			bestMode = availablePresentMode;
	}

	return bestMode;
}
VkExtent2D DeviceManager::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR & capabilities, std::uint32_t width, std::uint32_t height)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;

	VkExtent2D actualExtent = { width, height };
	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
	return actualExtent;
}
std::uint32_t DeviceManager::ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
	uint32_t imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		imageCount = capabilities.maxImageCount;

	return imageCount;
}