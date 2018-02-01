#pragma once

#include "RenderEngine/IRenderer.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"
#include "VulkanEngine/Instance.h"
#include "VulkanEngine/Devices/DeviceManager.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/SwapChains/SwapChain.h"
#include "VulkanEngine/Pipeline/PipelineStateManager.h"
#include "VulkanEngine/Commands/CommandPool.h"
#include "VulkanEngine/Synchronization/Semaphore.h"
#include "VulkanEngine/RenderItems/RenderItem.h"
#include "VulkanEngine/Commands/UploadDataManager.h"
#include "RenderEngine/Geometry/MeshData.h"
#include "RenderEngine/IScene.h"

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
		void RecreateDeviceDependentResources() override;
		void RecreateWindowSizeDependentResources() override;

	public:
		bool FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	public:
		bool IsNextFrameAvailable() override;

	public:
		template <class VertexType, class IndexType>
		void UploadBufferData(Buffer& buffer, vk::DeviceSize vertexBufferSize, vk::DeviceSize indexBufferSize, const RenderEngine::MeshData<VertexType, IndexType>& meshData)
		{
			auto& uploadBuffer = m_uploadDataManager.CreateUploadBuffer(m_deviceManager, buffer.GetSize());
			uploadBuffer.SetData(m_deviceManager.GetDevice(), meshData.Indices.data(), 0, indexBufferSize);
			uploadBuffer.SetData(m_deviceManager.GetDevice(), meshData.Vertices.data(), indexBufferSize, vertexBufferSize);

			{
				auto commandBuffer = m_uploadDataManager.CreateCommandBuffer(vk::CommandBufferLevel::ePrimary);
				commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

				uploadBuffer.CopyTo(m_deviceManager.GetDevice(), commandBuffer, *buffer);

				commandBuffer.end();
			}
		}

	public:
		DeviceManager& GetDeviceManager() { return m_deviceManager; }
		DescriptorManager& GetDescriptorManager() { return m_descriptorManager; }

	public:
		void SetScene(std::shared_ptr<RenderEngine::IScene> scene);

	private:
		static std::vector<vk::UniqueFramebuffer> CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, const vk::Extent2D& extent, const vk::RenderPass& renderPass);
		static vk::Viewport CreateViewport(const vk::Extent2D& extent);
		static vk::Rect2D CreateScissor(const vk::Extent2D& extent);
		static RenderItem CreateTriangle(const DeviceManager& deviceManager, UploadDataManager& uploadDataManager);

	private:
		void RecordCommands();

	private:
		static void EnumerateAvailableExtensions();

	private:
		Instance m_instance;
#if !defined(NDEBUG)
		DebugMessageHandler m_debugMessageHandler;
#endif
		Surface m_surface;
		DeviceManager m_deviceManager;
		SwapChain m_swapChain;
		DescriptorManager m_descriptorManager;
		RenderPass m_renderPass;
		PipelineStateManager m_pipelineStateManager;
		std::vector<vk::UniqueFramebuffer> m_framebuffers;
		vk::Viewport m_viewport;
		vk::Rect2D m_scissor;
		CommandPool m_defaultCommandPool;
		CommandPool m_temporaryCommandPool;
		std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		UploadDataManager m_uploadDataManager;
		Semaphore m_imageAvailableSemaphore;
		Semaphore m_renderFinishedSemaphore;
		std::shared_ptr<RenderEngine::IScene> m_scene;
	};
}
