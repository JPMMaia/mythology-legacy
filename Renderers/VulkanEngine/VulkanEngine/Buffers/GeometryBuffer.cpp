#include "pch.h"
#include "GeometryBuffer.h"

using namespace VulkanEngine;

GeometryBuffer::GeometryBuffer(const DeviceManager& deviceManager, vk::DeviceSize vertexDataSize, vk::DeviceSize indexDataSize, vk::IndexType indexType) :
	Buffer(deviceManager, vertexDataSize + indexDataSize, vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst, vk::MemoryPropertyFlagBits::eDeviceLocal),
	m_vertexDataOffset(indexDataSize),
	m_indexType(indexType)
{
}

void GeometryBuffer::Bind(vk::Device device, vk::CommandBuffer commandBuffer)
{
	commandBuffer.bindVertexBuffers(0, 1, &m_buffer.get(), &m_vertexDataOffset);
	commandBuffer.bindIndexBuffer(*m_buffer, 0, m_indexType);
}
