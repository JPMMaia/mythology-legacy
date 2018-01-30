#include "pch.h"
#include "Renderer.h"

#include "VulkanEngine/Geometry/Vertex.h"
#include "VulkanEngine/Scenes/RenderParameters.h"

using namespace VulkanEngine;

Renderer::Renderer(const std::vector<const char*>& enabledExtensions, std::unique_ptr<ISurface> surfaceInterface) :
	m_instance(enabledExtensions),
#if !defined(NDEBUG)
	m_debugMessageHandler(m_instance),
#endif
	m_surface(std::move(surfaceInterface), m_instance),
	m_deviceManager(m_instance, m_surface),
	m_swapChain(m_deviceManager.GetDevice(), m_surface, m_deviceManager.QuerySwapChainSupport(m_surface), m_deviceManager.GetQueueFamilyIndices(), m_renderPass),
	m_renderPass(m_deviceManager.GetDevice(), m_swapChain.GetSurfaceFormat().format),
	m_pipelineStateManager(m_deviceManager.GetDevice(), m_renderPass),
	m_framebuffers(CreateFrameBuffers(m_deviceManager.GetDevice(), m_swapChain.GetImageViews(), m_swapChain.GetExtent(), m_renderPass)),
	m_viewport(CreateViewport(m_swapChain.GetExtent())),
	m_scissor(CreateScissor(m_swapChain.GetExtent())),
	m_defaultCommandPool(CommandPool::CreateDefaultCommandPool(m_deviceManager.GetDevice(), m_deviceManager.GetQueueFamilyIndices())),
	m_temporaryCommandPool(CommandPool::CreateTemporaryCommandPool(m_deviceManager.GetDevice(), m_deviceManager.GetQueueFamilyIndices())),
	m_commandBuffers(m_defaultCommandPool.CreateCommandBuffers(m_deviceManager.GetDevice(), vk::CommandBufferLevel::ePrimary, m_swapChain.GetImageCount())),
	m_uploadDataManager(m_deviceManager.GetDevice(), m_temporaryCommandPool),
	m_imageAvailableSemaphore(m_deviceManager.GetDevice()),
	m_renderFinishedSemaphore(m_deviceManager.GetDevice())
{
}
Renderer::~Renderer()
{
	m_deviceManager.GetDevice().waitIdle();
}

void Renderer::RecreateDeviceDependentResources()
{
}
void Renderer::RecreateWindowSizeDependentResources()
{
	const auto& device = m_deviceManager.GetDevice();

	// Wait for the GPU to complete all the work:
	device.waitIdle();

	// Create a new swap chain:
	m_swapChain = SwapChain(device, m_surface, m_deviceManager.QuerySwapChainSupport(m_surface), m_deviceManager.GetQueueFamilyIndices(), m_renderPass, m_swapChain);

	// Create new frame buffers:
	m_framebuffers = CreateFrameBuffers(device, m_swapChain.GetImageViews(), m_swapChain.GetExtent(), m_renderPass);

	// Update viewport and scissor:
	m_viewport = CreateViewport(m_swapChain.GetExtent());
	m_scissor = CreateScissor(m_swapChain.GetExtent());

	// Create command buffers:
	m_commandBuffers = m_defaultCommandPool.CreateCommandBuffers(device, vk::CommandBufferLevel::ePrimary, m_swapChain.GetImageCount());
	RecordCommands();
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

void Renderer::SetScene(std::shared_ptr<RenderEngine::IScene> scene)
{
	m_scene = std::move(scene);

	RecordCommands();
	m_uploadDataManager.SubmitAllAndWait(m_deviceManager.GetGraphicsQueue());
}

std::vector<vk::UniqueFramebuffer> Renderer::CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, const vk::Extent2D& extent, const vk::RenderPass& renderPass)
{
	std::vector<vk::UniqueFramebuffer> frameBuffers(imageViews.size());

	for (std::size_t i = 0; i < frameBuffers.size(); i++)
	{
		std::array<vk::ImageView, 1> attachments =
		{
			imageViews[i].get()
		};

		vk::FramebufferCreateInfo framebufferInfo(
			{},
			renderPass,
			static_cast<std::uint32_t>(attachments.size()), attachments.data(),
			extent.width, extent.height,
			1
		);

		frameBuffers[i] = device.createFramebufferUnique(framebufferInfo);
	}

	return frameBuffers;
}
vk::Viewport Renderer::CreateViewport(const vk::Extent2D& extent)
{
	return vk::Viewport(
		0.0f, 0.0f,
		static_cast<float>(extent.width), static_cast<float>(extent.height),
		0.0f, 1.0f
	);
}
vk::Rect2D Renderer::CreateScissor(const vk::Extent2D& extent)
{
	return vk::Rect2D({ 0, 0 }, extent);
}

RenderItem Renderer::CreateTriangle(const DeviceManager& deviceManager, UploadDataManager& uploadDataManager)
{
	static const std::vector<Vertex> vertices =
	{
		{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
		{ { -0.5f, 0.5f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, -0.5f },{ 0.0f, 0.0f, 1.0f } },
		{ { 0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
	};

	using IndexType = std::uint16_t;
	static const std::vector<IndexType> indices =
	{
		0, 1, 2,
		2, 1, 3,
	};

	auto vertexBufferSize = static_cast<vk::DeviceSize>(vertices.size() * sizeof(Vertex));
	auto indexBufferSize = static_cast<vk::DeviceSize>(indices.size() * sizeof(IndexType));
	GeometryBuffer geometryBuffer(deviceManager, vertexBufferSize, indexBufferSize, vk::IndexType::eUint16);

	{				
		auto& uploadBuffer = uploadDataManager.CreateUploadBuffer(deviceManager, geometryBuffer.GetSize());
		uploadBuffer.SetData(deviceManager.GetDevice(), indices.data(), 0, indexBufferSize);
		uploadBuffer.SetData(deviceManager.GetDevice(), vertices.data(), indexBufferSize, vertexBufferSize);

		// Submit commands in order to copy the values from the upload buffer to the vertex buffer:
		{
			auto commandBuffer = uploadDataManager.CreateCommandBuffer(vk::CommandBufferLevel::ePrimary);
			commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

			uploadBuffer.CopyTo(deviceManager.GetDevice(), commandBuffer, *geometryBuffer);

			commandBuffer.end();
		}		
	}

	SubmeshGeometry submesh = { static_cast<std::uint32_t>(indices.size()), 1, 0, 0, 0 };
	return RenderItem(std::move(geometryBuffer), std::move(submesh));
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
				m_renderPass,
				m_framebuffers[i].get(),
				m_scissor,
				1, &clearValue
			);
			commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
			{
				static std::array<vk::Viewport, 1> viewports =
				{
					m_viewport
				};
				commandBuffer.setViewport(0, viewports);

				static std::array<vk::Rect2D, 1> scissors =
				{
					m_scissor
				};
				commandBuffer.setScissor(0, scissors);

				commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipelineStateManager.GetGraphicsPipeline());
				m_scene->Render({ commandBuffer, RenderLayer::Opaque });
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
