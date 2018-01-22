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
	m_presentQueue(CreatePresentQueue(m_device.get(), m_queueFamilyIndices.PresentFamily)),
	m_swapChainSupportDetails(QuerySwapChainSupport(m_physicalDevice, surface)),
	m_swapSurfaceFormat(ChooseSwapSurfaceFormat(m_swapChainSupportDetails.Formats)),
	m_swapPresentMode(ChooseSwapPresentMode(m_swapChainSupportDetails.PresentModes)),
	m_swapExtent(ChooseSwapExtent(m_swapChainSupportDetails.Capabilities, surface.GetWidth(), surface.GetHeight())),
	m_imageCount(ChooseImageCount(m_swapChainSupportDetails.Capabilities))
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
const SwapChainSupportDetails& DeviceManager::GetSwapChainSupportDetails() const
{
	return m_swapChainSupportDetails;
}
const vk::SurfaceFormatKHR& DeviceManager::GetSwapSurfaceFormat() const
{
	return m_swapSurfaceFormat;
}
vk::SurfaceTransformFlagBitsKHR DeviceManager::GetPreTransform() const
{
	return m_swapChainSupportDetails.Capabilities.currentTransform;
}
vk::PresentModeKHR DeviceManager::GetSwapPresentMode() const
{
	return m_swapPresentMode;
}
const vk::Extent2D& DeviceManager::GetSwapExtent() const
{
	return m_swapExtent;
}
std::uint32_t DeviceManager::GetImageCount() const
{
	return m_imageCount;
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

SwapChainSupportDetails DeviceManager::QuerySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
{
	SwapChainSupportDetails details;

	details.Capabilities = device.getSurfaceCapabilitiesKHR(surface);	
	details.Formats = device.getSurfaceFormatsKHR(surface);
	details.PresentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}
vk::SurfaceFormatKHR DeviceManager::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
		return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			return availableFormat;
	}

	return availableFormats[0];
}
vk::PresentModeKHR DeviceManager::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
{
	auto bestMode = vk::PresentModeKHR::eFifo;

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == vk::PresentModeKHR::eMailbox)
			return availablePresentMode;
		else if (availablePresentMode == vk::PresentModeKHR::eImmediate)
			bestMode = availablePresentMode;
	}

	return bestMode;
}
vk::Extent2D DeviceManager::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, std::uint32_t width, std::uint32_t height)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;

	vk::Extent2D actualExtent(width, height);
	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
	return actualExtent;
}
std::uint32_t DeviceManager::ChooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
{
	auto imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		imageCount = capabilities.maxImageCount;

	return imageCount;
}