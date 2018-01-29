#include "pch.h"
#include "UploadDataManager.h"

using namespace VulkanEngine;

UploadDataManager::UploadDataManager(vk::Device device, vk::CommandPool commandPool) :
	m_device(device),
	m_commandPool(commandPool)
{
}
UploadDataManager::~UploadDataManager()
{
	if (!m_commandBuffers.empty())	
		ClearCommandBuffers();
}

vk::CommandBuffer UploadDataManager::CreateCommandBuffer(vk::CommandBufferLevel level)
{
	vk::CommandBufferAllocateInfo commandBufferInfo(
		m_commandPool,
		level,
		1
	);

	vk::CommandBuffer commandBuffer;
	m_device.allocateCommandBuffers(&commandBufferInfo, &commandBuffer);
	m_commandBuffers.emplace_back(commandBuffer);
	
	return commandBuffer;
}
UploadBuffer& UploadDataManager::CreateUploadBuffer(const DeviceManager& deviceManager, vk::DeviceSize size)
{
	m_uploadBuffers.emplace_back(deviceManager, size);
	return m_uploadBuffers.back();
}

void UploadDataManager::SubmitAllAndWait(vk::Queue transferQueue)
{
	// Submit command queues:
	vk::SubmitInfo submitInfo(
		0, nullptr, nullptr,
		static_cast<std::uint32_t>(m_commandBuffers.size()), m_commandBuffers.data()
	);
	transferQueue.submit(1, &submitInfo, {});

	// Wait for all the commands to be processed by the GPU:
	transferQueue.waitIdle();

	// Clear resources:
	m_uploadBuffers.clear();
	ClearCommandBuffers();
}

void UploadDataManager::ClearCommandBuffers()
{
	m_device.freeCommandBuffers(m_commandPool, m_commandBuffers);
	m_commandBuffers.clear();
}
