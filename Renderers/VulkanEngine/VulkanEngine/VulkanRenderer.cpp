#include "pch.h"
#include "VulkanRenderer.h"

#include "VulkanEngine/Geometry/Vertex.h"

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
	m_renderFinishedSemaphore(m_deviceManager.GetDevice()),
	m_triangle(CreateTriangle(m_deviceManager))
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
	auto device = m_deviceManager.GetDevice();

	std::uint32_t imageIndex;
	device.acquireNextImageKHR(m_swapChain, std::numeric_limits<std::uint64_t>::max(), m_imageAvailableSemaphore, {}, &imageIndex);

	std::array<vk::Semaphore, 1> signalSemaphores = { m_renderFinishedSemaphore };

	{
		std::array<vk::Semaphore, 1> waitSemaphores = { m_imageAvailableSemaphore };
		std::array<vk::PipelineStageFlags, 1> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		vk::SubmitInfo submitInfo(
			static_cast<std::uint32_t>(waitSemaphores.size()), waitSemaphores.data(),
			waitStages.data(),
			1, &m_commandBuffers[imageIndex].get(),
			static_cast<std::uint32_t>(signalSemaphores.size()), signalSemaphores.data()
		);

		const auto& graphicsQueue = m_deviceManager.GetGraphicsQueue();
		graphicsQueue.submit(1, &submitInfo, {});
	}

	{
		std::array<vk::SwapchainKHR, 1> swapChains = { m_swapChain };

		vk::PresentInfoKHR presentInfo(
			static_cast<std::uint32_t>(signalSemaphores.size()), signalSemaphores.data(),
			static_cast<std::uint32_t>(swapChains.size()), swapChains.data(),
			&imageIndex,
			nullptr
		);

		const auto& presentQueue = m_deviceManager.GetPresentQueue();
		presentQueue.presentKHR(presentInfo);	
		presentQueue.waitIdle();
	}

	return true;
}

bool Renderer::IsNextFrameAvailable()
{
	return true;
}

std::vector<vk::UniqueCommandBuffer> Renderer::CreateCommandBuffers(const vk::Device& device, const vk::CommandPool& commandPool, std::uint32_t count)
{
	vk::CommandBufferAllocateInfo commandBufferInfo(
		commandPool,
		vk::CommandBufferLevel::ePrimary,
		count
	);
	return device.allocateCommandBuffersUnique(commandBufferInfo);
}
RenderItem Renderer::CreateTriangle(const DeviceManager& deviceManager)
{
	auto device = deviceManager.GetDevice();

	static const std::vector<Vertex> vertices = 
	{
		{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } },
		{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, -0.5f },{ 0.0f, 0.0f, 1.0f } }
	};
	
	VertexBuffer vertexBuffer(deviceManager, static_cast<vk::DeviceSize>(vertices.size() * sizeof(Vertex)));
	vertexBuffer.CopyData(device, vertices.data());

	SubmeshGeometry submesh = { static_cast<std::uint32_t>(vertices.size()), 1, 0, 0 };
	return RenderItem(std::move(vertexBuffer), submesh);
}

void Renderer::RecordCommands()
{
	for (std::size_t i = 0; i < m_commandBuffers.size(); ++i)
	{
		auto& commandBuffer = m_commandBuffers[i].get();

		vk::CommandBufferBeginInfo beginInfo(
			vk::CommandBufferUsageFlagBits::eSimultaneousUse
		);
		commandBuffer.begin(beginInfo);
		{
			std::array<float, 4> clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			vk::ClearValue clearValue(clearColor);
			vk::RenderPassBeginInfo renderPassInfo(
				m_pipelineStateManager.GetRenderPass(),
				m_swapChain.GetFrameBuffer(i),
				vk::Rect2D({ 0, 0 }, m_deviceManager.GetSwapExtent()),
				1, &clearValue
			);
			commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
			{
				static std::array<vk::Viewport, 1> viewports = 
				{
					vk::Viewport(
						0.0f, 0.0f, 
						static_cast<float>(m_surface.GetWidth()), static_cast<float>(m_surface.GetHeight()), 
						0.0f, 1.0f
					)
				};
				commandBuffer.setViewport(0, viewports);

				static std::array<vk::Rect2D, 1> scissors =
				{
					vk::Rect2D(vk::Offset2D(0, 0), m_deviceManager.GetSwapExtent())
				};
				commandBuffer.setScissor(0, scissors);
				
				commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelineStateManager.GetGraphicsPipeline());
				m_triangle.Draw(commandBuffer);
			}
			commandBuffer.endRenderPass();
		}
		commandBuffer.end();
	}
}

void Renderer::EnumerateAvailableExtensions()
{
	auto extensions = vk::enumerateInstanceExtensionProperties();

	std::cout << "available extensions:" << std::endl;
	for (const auto& extension : extensions)
		std::cout << "\t" << extension.extensionName << std::endl;
}
