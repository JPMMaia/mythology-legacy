#include "pch.h"
#include "RootSignatureManager.h"
#include "Core/Utilities/d3dx12.h"
#include "Common/Helpers.h"
#include "Core/Utilities/DirectXHelper.h"
#include "Core/Textures/Samplers.h"

using namespace Common;
using namespace DirectX12Engine;
using namespace Microsoft::WRL;

RootSignatureManager::RootSignatureManager(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}

void RootSignatureManager::CreateDeviceDependentResources()
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	{
		std::array<CD3DX12_ROOT_PARAMETER, 5> rootParameters;		
		rootParameters[0].InitAsShaderResourceView(0, 1);
		rootParameters[1].InitAsShaderResourceView(1, 1);
		rootParameters[2].InitAsConstantBufferView(0);

		// Textures:
		std::array<D3D12_DESCRIPTOR_RANGE, 1> descriptorRanges1 = 
		{ 
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 4, 0, 2)
		};
		rootParameters[3].InitAsDescriptorTable(static_cast<UINT>(descriptorRanges1.size()), descriptorRanges1.data());

		// G-Buffer descriptor table:
		std::array<D3D12_DESCRIPTOR_RANGE, 1> descriptorRanges2 =
		{
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0)
		};
		rootParameters[4].InitAsDescriptorTable(static_cast<UINT>(descriptorRanges2.size()), descriptorRanges2.data());

		auto flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDescription;
		auto staticSamplers = Samplers::GetStaticSamplers();
		rootSignatureDescription.Init(static_cast<UINT>(rootParameters.size()), rootParameters.data(), static_cast<UINT>(staticSamplers.size()), staticSamplers.data(), flags);

		{
			ComPtr<ID3DBlob> pSignature;
			ComPtr<ID3DBlob> pError;
			DX::ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDescription, D3D_ROOT_SIGNATURE_VERSION_1, pSignature.GetAddressOf(), pError.GetAddressOf()));

			ComPtr<ID3D12RootSignature> rootSignature;
			DX::ThrowIfFailed(d3dDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

			NAME_D3D12_OBJECT(rootSignature);
			m_rootSignatures.emplace("RootSignature", std::move(rootSignature));
		}
	}
}

void RootSignatureManager::SetGraphicsRootSignature(ID3D12GraphicsCommandList* commandList, const std::string& name) const
{
	commandList->SetGraphicsRootSignature(m_rootSignatures.at(name).Get());
}

void RootSignatureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex, CD3DX12_GPU_DESCRIPTOR_HANDLE baseDescriptor)
{
	commandList->SetGraphicsRootDescriptorTable(rootParameterIndex, baseDescriptor);
}

ID3D12RootSignature* RootSignatureManager::GetRootSignature(const std::string& name) const
{
	return m_rootSignatures.at(name).Get();
}
