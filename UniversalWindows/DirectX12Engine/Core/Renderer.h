#pragma once

#include "GraphicsEngineInterfaces/IRenderer.h"
#include "GraphicsEngineInterfaces/IScene.h"
#include "DeviceResources.h"
#include "Pipeline/RootSignatureManager.h"
#include "Pipeline/PipelineStateManager.h"
#include "Resources/DescriptorHeap.h"
#include "Command/CommandListManager.h"
#include "Textures/Texture.h"

#include <array>

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

		void ProcessInput() const;
		void FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	public:
		CommandListManager& GetCommandListManager() { return m_commandListManager; }
		void SetScene(const std::shared_ptr<IScene>& scene);

	private:
		void BeginRender();
		void EndRender();

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		RootSignatureManager m_rootSignatureManager;
		PipelineStateManager m_pipelineStateManager;
		CommandListManager m_commandListManager;
		D3D12_RECT m_scissorRect;
		DescriptorHeap m_rtvDescriptorHeap;
		DescriptorHeap m_dsvDescriptorHeap;
		DescriptorHeap m_srvDescriptorHeap;
		DescriptorHeap m_texturesDescriptorHeap;
		std::array<float, 4> m_clearColor;

		std::shared_ptr<IScene> m_scene;
		Texture m_positions;
		Texture m_albedo;
		Texture m_normals;
		Texture m_depthStencil;

		Texture m_albedoTexture;
	};
}
