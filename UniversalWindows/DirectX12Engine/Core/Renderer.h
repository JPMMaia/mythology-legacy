#pragma once

#include "GraphicsEngineInterfaces/IRenderer.h"
#include "GraphicsEngineInterfaces/IScene.h"
#include "DeviceResources.h"
#include "Pipeline/RootSignatureManager.h"
#include "Pipeline/PipelineStateManager.h"
#include "Pipeline/DescriptorHeap.h"
#include "Utilities/UploadBuffer.h"
#include "Command/CommandListManager.h"

namespace DirectX12Engine
{
	class Renderer : public GraphicsEngine::IRenderer
	{
	public:
		explicit Renderer(const std::shared_ptr<DeviceResources>& deviceResources);

		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;

		void SaveState() override;
		void LoadState() override;

		void FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	private:
		void BeginRender();
		void EndRender();

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		RootSignatureManager m_rootSignatureManager;
		PipelineStateManager m_pipelineStateManager;
		CommandListManager m_commandListManager;
		D3D12_RECT m_scissorRect;
		std::shared_ptr<GraphicsEngine::IScene> m_scene;
	};
}
