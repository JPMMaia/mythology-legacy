#include "pch.h"
#include "UniformBuffer.h"

using namespace VulkanEngine;

UniformBuffer::UniformBuffer(const DeviceManager & deviceManager, vk::DeviceSize size) :
	Buffer(deviceManager, size, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
{
}

void UniformBuffer::SetData(vk::Device device, const void* source, vk::DeviceSize offset, vk::DeviceSize size)
{
	void* destination = device.mapMemory(m_deviceMemory.get(), offset, size, {});
	std::memcpy(destination, source, size);
	device.unmapMemory(m_deviceMemory.get());
}
