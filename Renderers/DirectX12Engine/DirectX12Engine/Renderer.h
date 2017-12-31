#pragma once

#include "RenderEngine/IRenderer.h"
#include "RenderEngine/IScene.h"
#include "DeviceResources.h"
#include "Pipeline/RootSignatureManager.h"
#include "Pipeline/PipelineStateManager.h"
#include "Resources/DescriptorHeap.h"
#include "Textures/Texture.h"
#include "Command/CommandListManager.h"

#include <array>

namespace DirectX12Engine
{
	class Renderer : public RenderEngine::IRenderer
	{
	public:
		explicit Renderer(const std::shared_ptr<DeviceResources>& deviceResources);

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;

	public:
		bool FrameUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	public:
		const std::shared_ptr<RenderEngine::IScene>& GetScene() const;
		void SetScene(const std::shared_ptr<RenderEngine::IScene>& scene);

		CommandListManager& GetCommandListManager();

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
		std::array<float, 4> m_clearColor;

		std::shared_ptr<RenderEngine::IScene> m_scene;
		Texture m_positions;
		Texture m_baseColorAndMetallicness;
		Texture m_normalsAndRoughness;
		Texture m_depthStencil;

		Texture m_albedoTexture;
	};
}
