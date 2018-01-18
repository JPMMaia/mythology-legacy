#include "pch.h"
#include "VulkanRenderer.h"

#include <iostream>
#include <vector>

using namespace VulkanEngine;

Renderer::Renderer(const std::vector<const char*>& enabledExtensions, const ISurfaceBuilder& surfaceBuilder) :
	m_instance(enabledExtensions),
#if !defined(NDEBUG)
	m_debugMessageHandler(m_instance),
#endif
	m_surface(surfaceBuilder, m_instance),
	m_deviceManager(m_instance, m_surface),
	m_pipelineStateManager(m_deviceManager.GetDevice(), m_deviceManager.GetSwapSurfaceFormat().format, static_cast<float>(m_surface.GetWidth()), static_cast<float>(m_surface.GetHeight()), m_deviceManager.GetSwapExtent()),
	m_swapChain(m_deviceManager.GetDevice(), m_surface, m_deviceManager, m_pipelineStateManager.GetRenderPass()),
	m_commandPool(m_deviceManager.GetDevice(), m_deviceManager.GetQueueFamilyIndices()),
	m_commandBuffers(CreateCommandBuffers(m_deviceManager.GetDevice(), m_commandPool, m_deviceManager.GetImageCount()))
{
	RecordCommands();
}

void Renderer::CreateDeviceDependentResources()
{
}
void Renderer::CreateWindowSizeDependentResources()
{
}

bool Renderer::FrameUpdate(const Common::Timer& timer)
{
	return false;
}
bool Renderer::Render(const Common::Timer& timer)
{
	return false;
}

bool Renderer::IsNextFrameAvailable()
{
	return true;
}

std::vector<VkCommandBuffer> Renderer::CreateCommandBuffers(VkDevice device, VkCommandPool commandPool, std::uint32_t count)
{
	std::vector<VkCommandBuffer> commandBuffers(count);

	VkCommandBufferAllocateInfo commandBufferInfo = {};
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferInfo.commandPool = commandPool;
	commandBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	ThrowIfFailed(vkAllocateCommandBuffers(device, &commandBufferInfo, commandBuffers.data()));	

	return commandBuffers;
}
void Renderer::RecordCommands()
{
	for (size_t i = 0; i < m_commandBuffers.size(); ++i)
	{
		auto& commandBuffer = m_commandBuffers[i];

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_pipelineStateManager.GetRenderPass();
		renderPassInfo.framebuffer = m_swapChain.GetFrameBuffer(i);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_deviceManager.GetSwapExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineStateManager.GetGraphicsPipeline());
		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		ThrowIfFailed(vkEndCommandBuffer(commandBuffer));
	}
}

void Renderer::EnumerateAvailableExtensions()
{
	std::uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions)
		std::cout << "\t" << extension.extensionName << std::endl;
}
