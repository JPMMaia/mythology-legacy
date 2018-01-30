#include "pch.h"
#include "UploadBuffer.h"

using namespace VulkanEngine;

UploadBuffer::UploadBuffer(const DeviceManager& deviceManager, vk::DeviceSize size) :
	Buffer(deviceManager, size, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
{
}

void UploadBuffer::SetData(vk::Device device, const void* source, vk::DeviceSize offset, vk::DeviceSize size)
{
	// Map:
	void* destination = device.mapMemory(m_deviceMemory.get(), offset, size, {});

	// Copy:
	std::memcpy(destination, source, size);

	// Unmap:
	device.unmapMemory(m_deviceMemory.get());
}
void UploadBuffer::CopyTo(vk::Device device, vk::CommandBuffer commandBuffer, vk::Buffer destinationBuffer)
{
	vk::BufferCopy bufferCopy(0, 0, m_size);
	commandBuffer.copyBuffer(m_buffer.get(), destinationBuffer, 1, &bufferCopy);
}
