#pragma once

#include "Core/Shader/Shader.h"
#include "Core/DeviceResources.h"

#include <unordered_map>
#include "RootSignatureManager.h"

namespace DirectX12Engine
{
	class Renderer;

	class PipelineStateManager
	{
	public:
		explicit PipelineStateManager(const std::shared_ptr<DeviceResources>& deviceResources);

		void CreateDeviceDependentResources(const RootSignatureManager& rootSignatureManager);

		void SetPipelineState(ID3D12GraphicsCommandList* commandList, const std::string& name) const;

		ID3D12PipelineState* GetPipelineState(const std::string& name) const;

	private:
		void InitializeShadersAndInputLayout();
		void InitializePipelineStateObjects(const RootSignatureManager& rootSignatureManager);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		std::unordered_map<std::string, std::vector<D3D12_INPUT_ELEMENT_DESC>> m_inputLayouts;
		std::unordered_map<std::string, Shader> m_shaders;
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> m_pipelineStateObjects;
	};
}
