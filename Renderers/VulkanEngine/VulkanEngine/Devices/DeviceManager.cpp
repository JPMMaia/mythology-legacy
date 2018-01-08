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

DeviceManager::DeviceManager(const VulkanInstance& instance, const Surface& surface)
{
	QueryPhysicalDevices(instance, surface);
	CreateLogicalDevice(instance, m_physicalDevice, surface);
}
DeviceManager::~DeviceManager()
{
	if (m_device)
		vkDestroyDevice(m_device, nullptr);
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

VkPhysicalDevice DeviceManager::GetPhysicalDevice() const
{
	return m_physicalDevice;
}
VkDevice DeviceManager::GetDevice() const
{
	return m_device;
}
const QueueFamilyIndices& DeviceManager::GetQueueFamilyIndices() const
{
	return m_queueFamilyIndices;
}
VkQueue DeviceManager::GetGraphicsQueue() const
{
	return m_graphicsQueue;
}

void DeviceManager::QueryPhysicalDevices(const VulkanInstance& instance, VkSurfaceKHR surface)
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

	m_physicalDevice = bestPhysicalDeviceIt->second;
}
void DeviceManager::CreateLogicalDevice(const VulkanInstance& instance, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	m_queueFamilyIndices = FindQueueFamilies(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = { m_queueFamilyIndices.GraphicsFamily, m_queueFamilyIndices.PresentFamily };

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

	// Create logical device:
	ThrowIfFailed(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device));

	// Create queues:
	vkGetDeviceQueue(m_device, m_queueFamilyIndices.GraphicsFamily, 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, m_queueFamilyIndices.PresentFamily, 0, &m_presentQueue);
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
