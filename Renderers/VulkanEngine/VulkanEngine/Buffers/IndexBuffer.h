#pragma once

#include "Buffer.h"

namespace VulkanEngine
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(const DeviceManager& deviceManager, vk::DeviceSize size, vk::IndexType indexType);

	public:
		void Bind(vk::Device device, vk::CommandBuffer commandBuffer);

	private:
		vk::IndexType m_indexType;
	};
}
