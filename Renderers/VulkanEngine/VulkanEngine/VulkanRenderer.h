#pragma once

#include "RenderEngine/IRenderer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"
#include "VulkanEngine/VulkanInstance.h"
#include "VulkanEngine/Devices/DeviceManager.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/SwapChains/SwapChain.h"
#include "VulkanEngine/Pipeline/PipelineStateManager.h"
#include "VulkanEngine/Commands/CommandPool.h"
#include "VulkanEngine/Synchronization/Semaphore.h"
#include "VulkanEngine/RenderItems/RenderItem.h"

#if !defined(NDEBUG)
#include "VulkanEngine/Helpers/DebugMessageHandler.h"
#endif

namespace VulkanEngine
{
	class Renderer : public RenderEngine::IRenderer
	{
	public:
		explicit Renderer(const std::vector<const char*>& enabledExtensions, std::unique_ptr<ISurface> surfaceInterface);
		virtual ~Renderer();

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;

	public:
		bool FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	public:
		bool IsNextFrameAvailable() override;

	private:
		static std::vector<vk::UniqueFramebuffer> CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, const vk::Extent2D& extent, const vk::RenderPass& renderPass);
		static vk::Viewport CreateViewport(const vk::Extent2D& extent);
		static vk::Rect2D CreateScissor(const vk::Extent2D& extent);
		static std::vector<vk::UniqueCommandBuffer> CreateCommandBuffers(const vk::Device& device, const vk::CommandPool& commandPool, std::uint32_t count);
		static RenderItem CreateTriangle(const DeviceManager& deviceManager);

	private:
		void RecordCommands();

	private:
		static void EnumerateAvailableExtensions();

	private:
		VulkanInstance m_instance;
#if !defined(NDEBUG)
		DebugMessageHandler m_debugMessageHandler;
#endif
		Surface m_surface;
		DeviceManager m_deviceManager;
		SwapChain m_swapChain;
		RenderPass m_renderPass;
		PipelineStateManager m_pipelineStateManager;
		std::vector<vk::UniqueFramebuffer> m_framebuffers;
		vk::Viewport m_viewport;
		vk::Rect2D m_scissor;
		CommandPool m_commandPool;
		std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		Semaphore m_imageAvailableSemaphore;
		Semaphore m_renderFinishedSemaphore;
		RenderItem m_triangle;
	};
}
