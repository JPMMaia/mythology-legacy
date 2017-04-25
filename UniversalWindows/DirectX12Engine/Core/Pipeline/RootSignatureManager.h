#pragma once
#include <unordered_map>
#include "Core/DeviceResources.h"

namespace DirectX12Engine
{
	class RootSignatureManager
	{
	public:
		explicit RootSignatureManager(const std::shared_ptr<DeviceResources>& deviceResources);

		void CreateDeviceDependentResources();

		void SetGraphicsRootSignature(ID3D12GraphicsCommandList* commandList, const std::string& name) const;
		static void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex, CD3DX12_GPU_DESCRIPTOR_HANDLE baseDescriptor);

		ID3D12RootSignature* GetRootSignature(const std::string& name) const;
				
	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12RootSignature>> m_rootSignatures;
	};
}
