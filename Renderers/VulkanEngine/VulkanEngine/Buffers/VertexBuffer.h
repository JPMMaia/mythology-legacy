#pragma once

#include "VulkanEngine/Devices/DeviceManager.h"

namespace VulkanEngine
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const DeviceManager& deviceManager, vk::DeviceSize size);

	public:
		void Bind(const vk::CommandBuffer& commandBuffer);
		void CopyData(const vk::Device& device, const void* source);

	private:
		static vk::UniqueBuffer CreateBuffer(const vk::Device& device, vk::DeviceSize size);
		static vk::UniqueDeviceMemory AllocateDeviceMemory(const DeviceManager& deviceManager, const vk::Buffer& buffer);

	private:
		void BindBufferMemory(const vk::Device& device);

	private:
		vk::DeviceSize m_size;
		vk::UniqueBuffer m_buffer;
		vk::UniqueDeviceMemory m_deviceMemory;
	};
}
