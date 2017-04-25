#include "pch.h"
#include "PipelineStateManager.h"
#include "Common/Helpers.h"
#include "Core/Utilities/DirectXHelper.h"

using namespace Common;
using namespace DirectX12Engine;

PipelineStateManager::PipelineStateManager(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}

void PipelineStateManager::CreateDeviceDependentResources(const RootSignatureManager& rootSignatureManager)
{
	InitializeShadersAndInputLayout();
	InitializePipelineStateObjects(rootSignatureManager);
}

void PipelineStateManager::SetPipelineState(ID3D12GraphicsCommandList* commandList, const std::string& name) const
{
	commandList->SetPipelineState(m_pipelineStateObjects.at(name).Get());
}
ID3D12PipelineState* PipelineStateManager::GetPipelineState(const std::string& name) const
{
	return m_pipelineStateObjects.at(name).Get();
}

void PipelineStateManager::InitializeShadersAndInputLayout()
{
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		m_inputLayouts.emplace("PositionInputLayout", std::move(inputLayout));
	}

	m_shaders["SampleVS"] = Shader::CompileShader(L"GraphicsEngine/Shaders/SampleVertexShader.hlsl", nullptr, "main", "vs_5_1");
	m_shaders["SamplePS"] = Shader::CompileShader(L"GraphicsEngine/Shaders/SamplePixelShader.hlsl", nullptr, "main", "ps_5_1");
}

void PipelineStateManager::InitializePipelineStateObjects(const RootSignatureManager& rootSignatureManager)
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
		const auto& inputLayout = m_inputLayouts.at("PositionInputLayout");
		state.InputLayout = { inputLayout.data(), static_cast<uint32_t>(inputLayout.size()) };
		state.pRootSignature = rootSignatureManager.GetRootSignature("RootSignature");
		state.VS = m_shaders["SampleVS"].GetShaderBytecode();
		state.PS = m_shaders["SamplePS"].GetShaderBytecode();
		state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		state.SampleMask = UINT_MAX;
		state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		state.NumRenderTargets = 1;
		state.RTVFormats[0] = m_deviceResources->GetBackBufferFormat();
		state.DSVFormat = m_deviceResources->GetDepthBufferFormat();
		state.SampleDesc.Count = m_deviceResources->GetSampleCount();
		state.SampleDesc.Quality = m_deviceResources->GetSampleQuality();

		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		DX::ThrowIfFailed(d3dDevice->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&pipelineState)));
		m_pipelineStateObjects.emplace("Sample", std::move(pipelineState));
	}
}
