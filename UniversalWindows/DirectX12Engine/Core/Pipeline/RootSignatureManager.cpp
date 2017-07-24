#include "pch.h"
#include "RootSignatureManager.h"
#include "Core/Utilities/d3dx12.h"
#include "Common/Helpers.h"
#include "Core/Utilities/DirectXHelper.h"

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
		std::array<CD3DX12_ROOT_PARAMETER, 3> rootParameters;		
		rootParameters[0].InitAsShaderResourceView(0, 1);
		rootParameters[1].InitAsShaderResourceView(1, 1);
		rootParameters[2].InitAsConstantBufferView(0);

		auto flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDescription;
		rootSignatureDescription.Init(static_cast<UINT>(rootParameters.size()), rootParameters.data(), 0, nullptr, flags);

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
