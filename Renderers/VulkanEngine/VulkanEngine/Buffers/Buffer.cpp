#include "pch.h"
#include "Buffer.h"

using namespace VulkanEngine;

Buffer::Buffer(const DeviceManager& deviceManager, vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperties) :
	m_size(size),
	m_buffer(CreateBuffer(deviceManager.GetDevice(), size, usage)),
	m_deviceMemory(AllocateDeviceMemory(deviceManager, m_buffer.get(), memoryProperties))
{
	BindBufferMemory(deviceManager.GetDevice());
}

vk::Buffer Buffer::Get()
{
	return m_buffer.get();
}

vk::UniqueBuffer Buffer::CreateBuffer(vk::Device device, vk::DeviceSize size, vk::BufferUsageFlags usage)
{
	vk::BufferCreateInfo createInfo(
		{},
		size,
		usage,
		vk::SharingMode::eExclusive
	);

	return device.createBufferUnique(createInfo);
}
vk::UniqueDeviceMemory Buffer::AllocateDeviceMemory(const DeviceManager& deviceManager, vk::Buffer buffer, vk::MemoryPropertyFlags memoryProperties)
{
	auto device = deviceManager.GetDevice();

	auto memoryRequirements = device.getBufferMemoryRequirements(buffer);
	auto memoryTypeIndex = deviceManager.FindBufferMemoryIndexType(
		memoryRequirements.memoryTypeBits,
		memoryProperties
	);

	vk::MemoryAllocateInfo allocateInfo(
		memoryRequirements.size,
		memoryTypeIndex
	);

	return device.allocateMemoryUnique(allocateInfo);
}

void Buffer::BindBufferMemory(const vk::Device& device)
{
	device.bindBufferMemory(m_buffer.get(), m_deviceMemory.get(), 0);
}
