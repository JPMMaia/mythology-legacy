#pragma once

#include "Buffer.h"

namespace VulkanEngine
{
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(const DeviceManager& deviceManager, vk::DeviceSize size);

	public:
		void Bind(vk::Device device, vk::CommandBuffer commandBuffer);
	};
}
