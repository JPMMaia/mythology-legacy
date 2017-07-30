#include "pch.h"
#include "PipelineStateManager.h"
#include "Common/Helpers.h"
#include "Core/Utilities/DirectXHelper.h"

#include <filesystem>

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
		std::vector<D3D12_INPUT_ELEMENT_DESC> positionNormalInputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		m_inputLayouts.emplace("PositionNormal", std::move(positionNormalInputLayout));

		std::vector<D3D12_INPUT_ELEMENT_DESC> positionTextureCoordinatesInputLayout =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		m_inputLayouts.emplace("PositionTextureCoordinates", std::move(positionTextureCoordinatesInputLayout));
	}

	// GBufferPass:
	{
		m_shaders["GBufferPassVS"] = Shader::CompileShader(L"Shaders/Standard/GBufferPassVertexShader.hlsl", nullptr, "main", "vs_5_1");
		m_shaders["GBufferPassPS"] = Shader::CompileShader(L"Shaders/Standard/GBufferPassPixelShader.hlsl", nullptr, "main", "ps_5_1");
	}

	// LightingPass:
	{
		m_shaders["LightingPassVS"] = Shader::CompileShader(L"Shaders/Standard/LightingPassVertexShader.hlsl", nullptr, "main", "vs_5_1");
		m_shaders["LightingPassPS"] = Shader::CompileShader(L"Shaders/Standard/LightingPassPixelShader.hlsl", nullptr, "main", "ps_5_1");
	}
}

void PipelineStateManager::InitializePipelineStateObjects(const RootSignatureManager& rootSignatureManager)
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	// GBufferPass:
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
		const auto& inputLayout = m_inputLayouts.at("PositionNormal");
		state.InputLayout = { inputLayout.data(), static_cast<uint32_t>(inputLayout.size()) };
		state.pRootSignature = rootSignatureManager.GetRootSignature("RootSignature");
		state.VS = m_shaders["GBufferPassVS"].GetShaderBytecode();
		state.PS = m_shaders["GBufferPassPS"].GetShaderBytecode();
		state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		state.SampleMask = UINT_MAX;
		state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		state.NumRenderTargets = 3;
		state.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		state.RTVFormats[1] = DXGI_FORMAT_B8G8R8A8_UNORM;
		state.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		state.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		state.SampleDesc.Count = 1;
		state.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		DX::ThrowIfFailed(d3dDevice->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&pipelineState)));
		m_pipelineStateObjects.emplace("GBufferPass", std::move(pipelineState));
	}

	// LightingPass:
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC state = {};
		const auto& inputLayout = m_inputLayouts.at("PositionTextureCoordinates");
		state.InputLayout = { inputLayout.data(), static_cast<uint32_t>(inputLayout.size()) };
		state.pRootSignature = rootSignatureManager.GetRootSignature("RootSignature");
		state.VS = m_shaders["LightingPassVS"].GetShaderBytecode();
		state.PS = m_shaders["LightingPassPS"].GetShaderBytecode();
		state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		state.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		state.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		state.SampleMask = UINT_MAX;
		state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		state.NumRenderTargets = 1;
		state.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
		state.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		state.SampleDesc.Count = 1;
		state.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		DX::ThrowIfFailed(d3dDevice->CreateGraphicsPipelineState(&state, IID_PPV_ARGS(&pipelineState)));
		m_pipelineStateObjects.emplace("LightingPass", std::move(pipelineState));
	}
}
