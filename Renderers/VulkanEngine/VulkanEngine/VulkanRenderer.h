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
		explicit Renderer(const std::vector<const char*>& enabledExtensions, const ISurfaceBuilder& surfaceBuilder);
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
		PipelineStateManager m_pipelineStateManager;
		SwapChain m_swapChain;
		CommandPool m_commandPool;
		std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		Semaphore m_imageAvailableSemaphore;
		Semaphore m_renderFinishedSemaphore;
		RenderItem m_triangle;
	};
}
