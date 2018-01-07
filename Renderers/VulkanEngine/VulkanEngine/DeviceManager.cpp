#include "pch.h"
#include "DeviceManager.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

DeviceManager::DeviceManager(const VulkanInstance& instance)
{
	QueryPhysicalDevices(instance);
	CreateLogicalDevice(instance, GetPhysicalDevice(0));
}

DeviceManager::~DeviceManager()
{
	if (m_device)
		vkDestroyDevice(m_device, nullptr);
}

VkPhysicalDevice DeviceManager::GetPhysicalDevice(std::size_t index) const
{
	auto deviceLocation = m_physicalDevices.rbegin();
	std::advance(deviceLocation, index);
	return deviceLocation->second;
}
std::size_t DeviceManager::GetPhysicalDeviceCount() const
{
	return m_physicalDevices.size();
}

VkQueue DeviceManager::GetGraphicsQueue() const
{
	return m_graphicsQueue;
}

void DeviceManager::QueryPhysicalDevices(const VulkanInstance& instance)
{
	std::uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if (deviceCount == 0)
		throw std::runtime_error("Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		auto score = RateDeviceSuitability(device);
		m_physicalDevices.emplace(std::make_pair(score, device));
	}

	if (m_physicalDevices.rbegin()->first == 0)
		throw std::runtime_error("Failed to find a suitable GPU!");
}
void DeviceManager::CreateLogicalDevice(const VulkanInstance& instance, VkPhysicalDevice physicalDevice)
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.GraphicsFamily;
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;

#if !defined(NDEBUG)
	auto validationLayers = instance.GetValidationLayers();
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
#endif

	// Create logical device:
	ThrowIfFailed(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device));

	// Create graphics queue:
	vkGetDeviceQueue(m_device, indices.GraphicsFamily, 0, &m_graphicsQueue);
}

std::size_t DeviceManager::RateDeviceSuitability(VkPhysicalDevice device)
{
	std::size_t score = 0;

	if (!FindQueueFamilies(device).IsComplete())
		return score;

	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;
	score += deviceProperties.limits.maxImageDimension2D;
	
	return score;
}
DeviceManager::QueueFamilyIndices DeviceManager::FindQueueFamilies(VkPhysicalDevice device)
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

		if (indices.IsComplete()) 
			break;

		++i;
	}

	return indices;
}
