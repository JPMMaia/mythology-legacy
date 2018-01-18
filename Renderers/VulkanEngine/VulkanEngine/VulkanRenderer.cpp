#include "pch.h"
#include "VulkanRenderer.h"

#include <array>
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
	m_commandBuffers(CreateCommandBuffers(m_deviceManager.GetDevice(), m_commandPool, m_deviceManager.GetImageCount())),
	m_imageAvailableSemaphore(m_deviceManager.GetDevice()),
	m_renderFinishedSemaphore(m_deviceManager.GetDevice())
{
	RecordCommands();
}
Renderer::~Renderer()
{
	vkDeviceWaitIdle(m_deviceManager.GetDevice());
}

void Renderer::CreateDeviceDependentResources()
{
}
void Renderer::CreateWindowSizeDependentResources()
{
}

bool Renderer::FrameUpdate(const Common::Timer& timer)
{
	return true;
}
bool Renderer::Render(const Common::Timer& timer)
{
	auto presentQueue = m_deviceManager.GetPresentQueue();
	vkQueueWaitIdle(presentQueue);

	uint32_t imageIndex;
	ThrowIfFailed(vkAcquireNextImageKHR(m_deviceManager.GetDevice(), m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex));

	std::array<VkSemaphore, 1> signalSemaphores = { m_renderFinishedSemaphore };

	{
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		std::array<VkSemaphore, 1> waitSemaphores = { m_imageAvailableSemaphore };
		std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = static_cast<std::uint32_t>(waitSemaphores.size());
		submitInfo.pWaitSemaphores = waitSemaphores.data();
		submitInfo.pWaitDstStageMask = waitStages.data();

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

		submitInfo.signalSemaphoreCount = static_cast<std::uint32_t>(signalSemaphores.size());
		submitInfo.pSignalSemaphores = signalSemaphores.data();

		ThrowIfFailed(vkQueueSubmit(m_deviceManager.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
	}

	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = static_cast<std::uint32_t>(signalSemaphores.size());;
		presentInfo.pWaitSemaphores = signalSemaphores.data();

		std::array<VkSwapchainKHR, 1> swapChains = { m_swapChain };
		presentInfo.swapchainCount = static_cast<std::uint32_t>(swapChains.size());
		presentInfo.pSwapchains = swapChains.data();
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(presentQueue, &presentInfo);
	}

	return true;
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
