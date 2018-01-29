#pragma once

#include "VulkanEngine/Devices/DeviceManager.h"

namespace VulkanEngine
{
	class Buffer
	{
	public:
		Buffer(const DeviceManager& deviceManager, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperties);

	public:
		vk::Buffer Get();

	private:
		static vk::UniqueBuffer CreateBuffer(vk::Device device, vk::DeviceSize size, vk::BufferUsageFlags usage);
		static vk::UniqueDeviceMemory AllocateDeviceMemory(const DeviceManager& deviceManager, vk::Buffer buffer, vk::MemoryPropertyFlags memoryProperties);

	private:
		void BindBufferMemory(const vk::Device& device);

	protected:
		vk::DeviceSize m_size;
		vk::UniqueBuffer m_buffer;
		vk::UniqueDeviceMemory m_deviceMemory;
	};
}
