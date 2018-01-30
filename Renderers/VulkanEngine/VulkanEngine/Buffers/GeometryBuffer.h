#pragma once

#include "Buffer.h"

namespace VulkanEngine
{
	class GeometryBuffer : public Buffer
	{
	public:
		GeometryBuffer(const DeviceManager& deviceManager, vk::DeviceSize vertexDataSize, vk::DeviceSize indexDataSize, vk::IndexType indexType);

	public:
		void Bind(vk::Device device, vk::CommandBuffer commandBuffer);

	private:
		vk::DeviceSize m_vertexDataOffset;
		vk::IndexType m_indexType;
	};
}