#pragma once

#include "vulkan/vulkan.h"

#include <memory>

#include "RenderEngine/IRenderer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"
#include "VulkanEngine/VulkanInstance.h"
#include "VulkanEngine/Devices/DeviceManager.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/SwapChains/SwapChain.h"
#include "VulkanEngine/Pipeline/PipelineStateManager.h"

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
		void EnumerateAvailableExtensions();

	private:
		VulkanInstance m_instance;
#if !defined(NDEBUG)
		DebugMessageHandler m_debugMessageHandler;
#endif
		Surface m_surface;
		DeviceManager m_deviceManager;
		SwapChain m_swapChain;
		PipelineStateManager m_pipelineStateManager;
	};
}
