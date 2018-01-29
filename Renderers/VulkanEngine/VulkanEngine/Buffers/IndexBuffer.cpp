#include "pch.h"
#include "IndexBuffer.h"

using namespace VulkanEngine;

IndexBuffer::IndexBuffer(const DeviceManager& deviceManager, vk::DeviceSize size, vk::IndexType indexType) :
	Buffer(deviceManager, size, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal),
	m_indexType(indexType)
{
}

void IndexBuffer::Bind(vk::Device device, vk::CommandBuffer commandBuffer)
{
	commandBuffer.bindIndexBuffer(*m_buffer, {}, m_indexType);
}
