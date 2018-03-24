#pragma once

#include "VulkanEngine/Devices/DeviceManager.h"

namespace VulkanEngine
{
	class TextureImage
	{
	public:
		TextureImage(const DeviceManager& deviceManager, vk::DeviceSize size, vk::Format format, vk::Extent2D extent, vk::ImageUsageFlags usage);

	public:
		vk::Image operator*();

	private:
		static vk::UniqueImage CreateImage(vk::Device device, vk::DeviceSize size, vk::Format format, vk::Extent2D extent, vk::ImageUsageFlags usage);
		static vk::UniqueDeviceMemory AllocateDeviceMemory(const DeviceManager& deviceManager, vk::Image image, vk::MemoryPropertyFlags memoryProperties);

	private:
		void BindImageMemory(const vk::Device& device);

	protected:
		vk::UniqueImage m_image;
		vk::UniqueDeviceMemory m_deviceMemory;
	};
}
