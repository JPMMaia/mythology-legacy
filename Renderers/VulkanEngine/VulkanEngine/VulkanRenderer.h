#pragma once

#include "vulkan/vulkan.h"

#include <memory>
#include <vector>

#include "RenderEngine/IRenderer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"
#include "VulkanEngine/VulkanInstance.h"
#include "VulkanEngine/Devices/DeviceManager.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/SwapChains/SwapChain.h"
#include "VulkanEngine/Pipeline/PipelineStateManager.h"
#include "VulkanEngine/Commands/CommandPool.h"

#if !defined(NDEBUG)
#include "VulkanEngine/Helpers/DebugMessageHandler.h"
#endif

namespace VulkanEngine
{
	class Renderer : public RenderEngine::IRenderer
	{
	public:
		explicit Renderer(const std::vector<const char*>& enabledExtensions, const ISurfaceBuilder& surfaceBuilder);

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;

	public:
		bool FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	public:
		bool IsNextFrameAvailable() override;

	private:
		static std::vector<VkCommandBuffer> CreateCommandBuffers(VkDevice device, VkCommandPool commandPool, std::uint32_t count);
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
		std::vector<VkCommandBuffer> m_commandBuffers;
	};
}
