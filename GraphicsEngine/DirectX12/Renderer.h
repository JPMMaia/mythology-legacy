#pragma once

#include "Interfaces/IRenderer.h"
#include "DeviceResources.h"
#include "Pipeline/RootSignatureManager.h"
#include "Pipeline/PipelineStateManager.h"
#include "Interfaces/IScene.h"

namespace GraphicsEngine
{
	class Renderer : public IRenderer
	{
	public:
		explicit Renderer(const std::shared_ptr<DeviceResources>& deviceResources);

		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;

		void SaveState() override;
		void LoadState() override;

		void FrameUpdate(const Common::Timer& timer) override;
		void FixedUpdate(const Common::Timer& timer) override;
		bool Render(const Common::Timer& timer) override;

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;
		RootSignatureManager m_rootSignatureManager;
		PipelineStateManager m_pipelineStateManager;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
		std::shared_ptr<IScene> m_scene;
	};
}
