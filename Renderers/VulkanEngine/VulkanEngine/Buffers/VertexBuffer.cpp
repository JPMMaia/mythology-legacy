#include "pch.h"
#include "VertexBuffer.h"

using namespace VulkanEngine;

VertexBuffer::VertexBuffer(const DeviceManager& deviceManager, vk::DeviceSize size) :
	m_size(size),
	m_buffer(CreateBuffer(deviceManager.GetDevice(), size)),
	m_deviceMemory(AllocateDeviceMemory(deviceManager, m_buffer.get()))
{
	BindBufferMemory(deviceManager.GetDevice());
}

void VertexBuffer::Bind(const vk::CommandBuffer& commandBuffer)
{
	std::array<vk::Buffer, 1> buffers = { m_buffer.get() };
	std::array<vk::DeviceSize, 1> offsets = { 0 };
	commandBuffer.bindVertexBuffers(
		0,
		static_cast<std::uint32_t>(buffers.size()), buffers.data(),
		offsets.data()
	);
}

void VertexBuffer::CopyData(const vk::Device& device, const void* source)
{
	// Map:
	void* destination = device.mapMemory(m_deviceMemory.get(), 0, m_size, {});
	
	// Copy:
	std::memcpy(destination, source, m_size);

	// Unmap:
	device.unmapMemory(m_deviceMemory.get());
}

vk::UniqueBuffer VertexBuffer::CreateBuffer(const vk::Device& device, vk::DeviceSize size)
{
	vk::BufferCreateInfo createInfo(
		{},
		size,
		vk::BufferUsageFlagBits::eVertexBuffer,
		vk::SharingMode::eExclusive
	);
	
	return device.createBufferUnique(createInfo);
}

vk::UniqueDeviceMemory VertexBuffer::AllocateDeviceMemory(const DeviceManager& deviceManager, const vk::Buffer& buffer)
{
	auto device = deviceManager.GetDevice();

	auto memoryRequirements = device.getBufferMemoryRequirements(buffer);
	auto memoryTypeIndex = deviceManager.FindBufferMemoryIndexType(
		memoryRequirements.memoryTypeBits, 
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	);

	vk::MemoryAllocateInfo allocateInfo(
		memoryRequirements.size,
		memoryTypeIndex
	);

	return device.allocateMemoryUnique(allocateInfo);
}

void VertexBuffer::BindBufferMemory(const vk::Device& device)
{
	device.bindBufferMemory(m_buffer.get(), m_deviceMemory.get(), 0);
}
