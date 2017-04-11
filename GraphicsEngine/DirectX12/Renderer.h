#pragma once

#include "Interfaces/IRenderer.h"
#include "DeviceResources.h"
#include "Pipeline/RootSignatureManager.h"
#include "Pipeline/PipelineStateManager.h"
#include "Interfaces/IScene.h"
#include "Pipeline/DescriptorHeap.h"
#include "Utilities/UploadBuffer.h"
#include "Command/CommandListManager.h"
#include "Shader/ShaderBufferTypes.h"
#include "Resources/ResourceInstance.h"
#include "RenderItem/StandardRenderItem.h"

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
		CommandListManager m_commandListManager;
		std::shared_ptr<IScene> m_scene;
		D3D12_RECT m_scissorRect;
	};
}
