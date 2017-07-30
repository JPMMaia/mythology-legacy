#pragma once

#include "Core/Utilities/d3dx12.h"
#include "DDSTextureLoader.h"

#include <d3d12.h>
#include <wrl/Client.h>
#include <unordered_map>

namespace DirectX12Engine
{
	class DeviceResources;
	class DescriptorHeap;

	class RWTexture
	{
	public:
		RWTexture() = default;

	public:
		void CreateResource(const DeviceResources& deviceResources, UINT64 width, UINT64 height, DXGI_FORMAT format, D3D12_RESOURCE_STATES initialResourceState, D3D12_CLEAR_VALUE* optimizedClearValue, D3D12_RESOURCE_FLAGS resourceFlags);
		void LoadTextureFromFile(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* graphicsCommandList, const std::wstring& filename, D3D12_RESOURCE_FLAGS resourceFlags, unsigned int loadFlags, DirectX::DDS_ALPHA_MODE& alphaMode, bool& isCubeMap, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

		void CreateDepthStencilView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateRenderTargetView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateShaderResourceView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);
		void CreateUnorderedAccessView(const std::shared_ptr<DeviceResources>& deviceResources, DescriptorHeap& descriptorHeap, const std::string& name);

		D3D12_CPU_DESCRIPTOR_HANDLE CPUDescriptorHandle(const std::string& name) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GPUDescriptorHandle(const std::string& name) const;

		ID3D12Resource* GetResource() const;
		
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
		DXGI_FORMAT m_format;
		std::unordered_map<std::string, CD3DX12_CPU_DESCRIPTOR_HANDLE> m_cpuDescriptorHandles;
		std::unordered_map<std::string, CD3DX12_GPU_DESCRIPTOR_HANDLE> m_gpuDescriptorHandles;
	};
}
