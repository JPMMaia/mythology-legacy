#pragma once

#include "VulkanEngine/Devices/QueueFamilyIndices.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/SwapChains/SwapChainSupportDetails.h"

#include <map>
#include <vector>

namespace VulkanEngine
{
	class DeviceManager
	{
	public:
		DeviceManager(const vk::Instance& instance, const Surface& surface);

	public:
		std::uint32_t FindBufferMemoryIndexType(std::uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;

	public:
		const vk::PhysicalDevice& GetPhysicalDevice() const;
		const QueueFamilyIndices& GetQueueFamilyIndices() const;
		const vk::Device& GetDevice() const;
		const vk::Queue& GetGraphicsQueue() const;
		const vk::Queue& GetPresentQueue() const;
		const SwapChainSupportDetails& GetSwapChainSupportDetails() const;
		const vk::SurfaceFormatKHR& GetSwapSurfaceFormat() const;
		vk::SurfaceTransformFlagBitsKHR GetPreTransform() const;
		vk::PresentModeKHR GetSwapPresentMode() const;
		const vk::Extent2D& GetSwapExtent() const;
		std::uint32_t GetImageCount() const;

	private:
		static vk::PhysicalDevice QueryPhysicalDevices(const vk::Instance& instance, const vk::SurfaceKHR& surface);
		static std::size_t RateDeviceSuitability(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
		static bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device);

	private:
		static vk::UniqueDevice CreateLogicalDevice(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices);

	private:
		static QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
		static vk::Queue CreateGraphicsQueue(const vk::Device& device, int graphicsFamily);
		static vk::Queue CreatePresentQueue(const vk::Device& device, int presentFamily);

	private:
		static SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
		static vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		static vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
		static vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, std::uint32_t width, std::uint32_t height);
		static std::uint32_t ChooseImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);

	private:
		vk::PhysicalDevice m_physicalDevice;
		QueueFamilyIndices m_queueFamilyIndices;
		vk::UniqueDevice m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		SwapChainSupportDetails m_swapChainSupportDetails;
		vk::SurfaceFormatKHR m_swapSurfaceFormat;
		vk::PresentModeKHR m_swapPresentMode;
		vk::Extent2D m_swapExtent;
		std::uint32_t m_imageCount;

	private:
		static const std::vector<const char*> s_deviceExtensions;
	};
}
