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
		SwapChainSupportDetails QuerySwapChainSupport(const vk::SurfaceKHR& surface) const;

	public:
		const vk::PhysicalDevice& GetPhysicalDevice() const;
		const QueueFamilyIndices& GetQueueFamilyIndices() const;
		const vk::Device& GetDevice() const;
		const vk::Queue& GetGraphicsQueue() const;
		const vk::Queue& GetPresentQueue() const;

	private:
		static vk::PhysicalDevice QueryPhysicalDevices(const vk::Instance& instance, const vk::SurfaceKHR& surface);
		static std::size_t RateDeviceSuitability(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
		static bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device);
		static SwapChainSupportDetails QuerySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

	private:
		static vk::UniqueDevice CreateLogicalDevice(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices);

	private:
		static QueueFamilyIndices FindQueueFamilies(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);
		static vk::Queue CreateGraphicsQueue(const vk::Device& device, int graphicsFamily);
		static vk::Queue CreatePresentQueue(const vk::Device& device, int presentFamily);

	private:
		vk::PhysicalDevice m_physicalDevice;
		QueueFamilyIndices m_queueFamilyIndices;
		vk::UniqueDevice m_device;
		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		vk::SurfaceFormatKHR m_surfaceFormat;

	private:
		static const std::vector<const char*> s_deviceExtensions;
	};
}
