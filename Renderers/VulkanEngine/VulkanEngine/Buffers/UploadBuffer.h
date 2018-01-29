#pragma once

#include "Buffer.h"

namespace VulkanEngine
{
	class UploadBuffer : public Buffer
	{
	public:
		UploadBuffer(const DeviceManager& deviceManager, vk::DeviceSize size);

	public:
		void SetData(vk::Device device, const void* source);
		void CopyTo(vk::Device device, vk::CommandBuffer commandBuffer, vk::Buffer destinationBuffer);
	};
}
