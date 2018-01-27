#include "pch.h"
#include "DeviceManager.h"
#include "VulkanEngine/Configurations/ValidationLayers.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <algorithm>
#include <set>

using namespace VulkanEngine;

const std::vector<const char*> DeviceManager::s_deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

DeviceManager::DeviceManager(const vk::Instance& instance, const Surface& surface) :
	m_physicalDevice(QueryPhysicalDevices(instance, surface)),
	m_queueFamilyIndices(FindQueueFamilies(m_physicalDevice, surface)),
	m_device(CreateLogicalDevice(instance, m_physicalDevice, surface, m_queueFamilyIndices)),
	m_graphicsQueue(CreateGraphicsQueue(m_device.get(), m_queueFamilyIndices.GraphicsFamily)),
	m_presentQueue(CreatePresentQueue(m_device.get(), m_queueFamilyIndices.PresentFamily))
{
}

std::uint32_t DeviceManager::FindBufferMemoryIndexType(std::uint32_t typeFilter, vk::MemoryPropertyFlags properties) const
{
	auto memoryProperties = m_physicalDevice.getMemoryProperties();
	for (std::uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("Failed to find a suitable memory index type!");
}

SwapChainSupportDetails DeviceManager::QuerySwapChainSupport(const vk::SurfaceKHR& surface) const
{
	return QuerySwapChainSupport(m_physicalDevice, surface);
}

const vk::PhysicalDevice& DeviceManager::GetPhysicalDevice() const
{
	return m_physicalDevice;
}
const QueueFamilyIndices& DeviceManager::GetQueueFamilyIndices() const
{
	return m_queueFamilyIndices;
}
const vk::Device& DeviceManager::GetDevice() const
{
	return m_device.get();
}
const vk::Queue& DeviceManager::GetGraphicsQueue() const
{
	return m_graphicsQueue;
}
const vk::Queue& DeviceManager::GetPresentQueue() const
{
	return m_presentQueue;
}

vk::PhysicalDevice DeviceManager::QueryPhysicalDevices(const vk::Instance& instance, const vk::SurfaceKHR& surface)
{
	// Enumerate physical devices:
	auto devices = instance.enumeratePhysicalDevices();

	// Rate the physical devices:
	std::map<std::size_t, std::reference_wrapper<const vk::PhysicalDevice>> physicalDevices;
	for (const auto& device : devices)
	{
		auto score = RateDeviceSuitability(device, surface);
		physicalDevices.emplace(score, device);
	}

	// Choose the physical device with highest score:
	auto bestPhysicalDeviceIt = physicalDevices.rbegin();
	if (bestPhysicalDeviceIt->first == 0)
		throw std::runtime_error("Failed to find a suitable GPU!");

	return bestPhysicalDeviceIt->second;
}
std::size_t DeviceManager::RateDeviceSuitability(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
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

	auto deviceProperties = device.getProperties();
	if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
		score += 1000;
	score += deviceProperties.limits.maxImageDimension2D;

	return score;
}
bool DeviceManager::CheckDeviceExtensionSupport(const vk::PhysicalDevice& device)
{
	auto availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(s_deviceExtensions.begin(), s_deviceExtensions.end());

	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);

	return requiredExtensions.empty();
}
SwapChainSupportDetails DeviceManager::QuerySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR & surface)
{
	SwapChainSupportDetails details;

	details.Capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.Formats = device.getSurfaceFormatsKHR(surface);
	details.PresentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}

vk::UniqueDevice DeviceManager::CreateLogicalDevice(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices)
{
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	std::set<int> uniqueQueueFamilies =
	{
		queueFamilyIndices.GraphicsFamily, queueFamilyIndices.PresentFamily
	};

	auto queuePriority = 1.0f;
	for (auto queueFamily : uniqueQueueFamilies)
	{
		queueCreateInfos.emplace_back(
			vk::DeviceQueueCreateInfo(
				{},
				queueFamily,
				1, &queuePriority
			)
		);
	}

	vk::PhysicalDeviceFeatures deviceFeatures;
	auto validationLayers = ValidationLayers::Get();
	vk::DeviceCreateInfo createInfo(
		{},
		static_cast<std::uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(),
		static_cast<uint32_t>(validationLayers.size()), validationLayers.data(),
		static_cast<std::uint32_t>(s_deviceExtensions.size()), s_deviceExtensions.data(),
		&deviceFeatures
		);

	return physicalDevice.createDeviceUnique(createInfo);
}

QueueFamilyIndices DeviceManager::FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
{
	QueueFamilyIndices indices;

	auto queueFamilies = device.getQueueFamilyProperties();

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			indices.GraphicsFamily = i;

		auto presentSupport = device.getSurfaceSupportKHR(i, surface);
		if (queueFamily.queueCount > 0 && presentSupport)
			indices.PresentFamily = i;

		if (indices.IsComplete())
			break;

		++i;
	}

	return indices;
}
vk::Queue DeviceManager::CreateGraphicsQueue(const vk::Device& device, int graphicsFamily)
{
	return device.getQueue(graphicsFamily, 0);
}
vk::Queue DeviceManager::CreatePresentQueue(const vk::Device& device, int presentFamily)
{
	return device.getQueue(presentFamily, 0);
}
