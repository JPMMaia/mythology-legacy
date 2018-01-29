#include "pch.h"
#include "VertexBuffer.h"

using namespace VulkanEngine;

VertexBuffer::VertexBuffer(const DeviceManager& deviceManager, vk::DeviceSize size) :
	Buffer(deviceManager, size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal)
{
}

void VertexBuffer::Bind(vk::Device device, vk::CommandBuffer commandBuffer)
{
	std::array<vk::Buffer, 1> buffers = { m_buffer.get() };
	std::array<vk::DeviceSize, 1> offsets = { 0 };
	commandBuffer.bindVertexBuffers(
		0,
		static_cast<std::uint32_t>(buffers.size()), buffers.data(),
		offsets.data()
	);
}
