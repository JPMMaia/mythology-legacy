#pragma once

#include "Buffer.h"

namespace VulkanEngine
{
	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer(const DeviceManager& deviceManager, vk::DeviceSize size);

	public:
		void SetData(vk::Device device, const void* source, vk::DeviceSize offset, vk::DeviceSize size);
	};
}
