#pragma once

#include "VulkanEngine/Commands/CommandPool.h"
#include "VulkanEngine/Buffers/UploadBuffer.h"

#include <deque>
#include <vector>

namespace VulkanEngine
{
	class UploadDataManager
	{
	public:
		UploadDataManager(vk::Device device, vk::CommandPool commandPool);
		~UploadDataManager();

	public:
		vk::CommandBuffer CreateCommandBuffer(vk::CommandBufferLevel level);
		UploadBuffer& CreateUploadBuffer(const DeviceManager& deviceManager, vk::DeviceSize size);

	public:
		void SubmitAllAndWait(vk::Queue transferQueue);

	private:
		void ClearCommandBuffers();

	private:
		vk::Device m_device;
		vk::CommandPool m_commandPool;
		std::vector<vk::CommandBuffer> m_commandBuffers;
		std::deque<UploadBuffer> m_uploadBuffers;
	};
}
